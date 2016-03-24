/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "Shape.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/PointingDeviceSensor/Hit.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/ViewVolume.h"
#include "../../Tools/Shape/ShapeTool.h"
#include "../../Types/Geometry.h"
#include "../Layering/X3DLayerNode.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shape/Appearance.h"

namespace titania {
namespace X3D {

const ComponentType Shape::component      = ComponentType::SHAPE;
const std::string   Shape::typeName       = "Shape";
const std::string   Shape::containerField = "children";

Shape::Shape (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DShapeNode ()
{
	addType (X3DConstants::Shape);

	addField (inputOutput,    "metadata",   metadata ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());
	addField (inputOutput,    "appearance", appearance ());
	addField (inputOutput,    "geometry",   geometry ());
}

X3DBaseNode*
Shape::create (X3DExecutionContext* const executionContext) const
{
	return new Shape (executionContext);
}

bool
Shape::isTransparent () const
{
	if (getAppearance () -> isTransparent ())
		return true;

	if (getGeometry () and getGeometry () -> isTransparent ())
		return true;

	return false;
}

bool
Shape::isLineGeometry () const
{
	return getGeometry () -> isLineGeometry ();
}

Box3d
Shape::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		if (getGeometry ())
			return getGeometry () -> getBBox ();

		else
			return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

bool
Shape::intersects (const CollisionSphere3d & sphere, const CollectableObjectArray & localObjects)
{
	return getGeometry () -> intersects (sphere, localObjects);
}

void
Shape::traverse (const TraverseType type)
{
	if (getGeometry ())
	{
		getGeometry () -> traverse (type);

		switch (type)
		{
			case TraverseType::POINTER:
			{
				pointer ();
				break;
			}
			case TraverseType::CAMERA:
			{
				break;
			}
			case TraverseType::COLLISION:
			case TraverseType::DEPTH:
			{
				getCurrentLayer () -> addCollision (this);
				break;
			}
			case TraverseType::DISPLAY:
			{
				getCurrentLayer () -> addShape (this);
				break;
			}
		}
	}
}

void
Shape::pointer ()
{
	// All geometries must be picked

	if (not getBrowser () -> isPointerInRectangle (getCurrentLayer () -> getViewVolumeStack () .back () .getScissor ()))
		return;

	const Box3d bbox = getBBox () * getModelViewMatrix () .get ();

	if (not getCurrentLayer () -> getViewVolumeStack () .back () .intersects (bbox))
		return;

	std::vector <IntersectionPtr> itersections;

	if (getBrowser () -> getSelectionBuffer ())
	{
		getGeometry () -> intersects (getBrowser () -> getSelectionBuffer (), getBrowser () -> getDepthBuffer (), itersections);
		return;
	}

	const Line3d hitRay = getBrowser () -> getHitRay (getModelViewMatrix () .get (), ProjectionMatrix4d (), Viewport4i ());

	if (not getGeometry () -> intersects (hitRay, itersections))
		return;

	// Finally we have intersections and must now find the closest hit in front of the camera.

	// Transform hitPoints to absolute space.
	for (auto & itersection : itersections)
		itersection -> point = itersection -> point * getModelViewMatrix () .get ();

	// Sort desc
	std::sort (itersections .begin (), itersections .end (),
	           [ ] (const IntersectionPtr &lhs, const IntersectionPtr &rhs) -> bool
	           {
	              return lhs -> point .z () > rhs -> point .z ();
				  });

	// Find first point that is not greater than near plane;
	const auto itersection = std::lower_bound (itersections .cbegin (), itersections .cend (), -getCurrentNavigationInfo () -> getNearPlane (),
	                                           [ ] (const IntersectionPtr &lhs, const float & rhs) -> bool
	                                           {
	                                              return lhs -> point .z () > rhs;
															 });

	// There are only intersections behind the camera.
	if (itersection == itersections .end ())
		return;

	try
	{
		// Transform hitNormal to absolute space.
		(*itersection) -> normal = normalize (inverse (getModelViewMatrix () .get ()) .mult_matrix_dir ((*itersection) -> normal));
	}
	catch (const std::domain_error &)
	{
		return;
	}

	getBrowser () -> addHit (getModelViewMatrix () .get (), *itersection, this, getCurrentLayer ());
}

void
Shape::collision (const CollisionContainer* const context)
{
	getGeometry () -> collision (context);
}

void
Shape::draw (const ShapeContainer* const context)
{
	getGeometry () -> draw (context);
}

void
Shape::addTool ()
{
	X3DShapeNode::addTool (new ShapeTool (this));
}

} // X3D
} // titania
