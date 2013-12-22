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

#include "../../Bits/Cast.h"
#include "../../Browser/Picking/Hit.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/ViewVolume.h"
#include "../../Types/Geometry.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shape/Appearance.h"

namespace titania {
namespace X3D {

const std::string Shape::componentName  = "Shape";
const std::string Shape::typeName       = "Shape";
const std::string Shape::containerField = "children";

Shape::Shape (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DShapeNode (),
	geometryNode (nullptr)
{
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

void
Shape::initialize ()
{
	X3DShapeNode::initialize ();

	geometry () .addInterest (this, &Shape::set_geometry);

	set_geometry ();
}

void
Shape::set_geometry ()
{
	geometryNode = x3d_cast <X3DGeometryNode*> (geometry ());
}

bool
Shape::isTransparent () const
{
	if (getAppearance () -> isTransparent ())
		return true;

	if (geometryNode and geometryNode -> isTransparent ())
		return true;

	return false;
}

bool
Shape::isLineGeometry () const
{
	return geometryNode -> isLineGeometry ();
}

Box3f
Shape::getBBox ()
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		if (geometryNode)
			return geometryNode -> getBBox ();

		else
			return Box3f ();
	}

	return Box3f (bboxSize (), bboxCenter ());
}

bool
Shape::intersect (const Sphere3f & sphere, const Matrix4f & matrix, const CollectableObjectArray & localObjects)
{
	return geometryNode -> intersect (sphere, matrix, localObjects);
}

void
Shape::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::PICKING:
		{
			pick ();
			break;
		}
		case TraverseType::NAVIGATION:
		case TraverseType::COLLISION:
		{
			if (geometryNode)
				getBrowser () -> getRenderers () .top () -> addCollision (this);

			break;
		}
		case TraverseType::COLLECT:
		{
			if (geometryNode)
				getBrowser () -> getRenderers () .top () -> addShape (this);

			break;
		}
		default:
			break;
	}
}

void
Shape::pick ()
{
	// All geometries must be picked

	if (geometryNode)
	{
		if (getBrowser () -> intersect (glIsEnabled (GL_SCISSOR_TEST) ? Scissor4i () : Viewport4i ()))
		{
			auto modelViewMatrix = ModelViewMatrix4f ();

			if (getCurrentViewpoint () -> getViewVolume () .intersect (getBBox () * modelViewMatrix))
			{
				Line3f hitRay = getBrowser () -> getHitRay (); // Attention!! returns a Line3d

				std::deque <IntersectionPtr> itersections;

				if (geometryNode -> intersect (hitRay, itersections))
				{
					for (auto & itersection : itersections)
						itersection -> hitPoint = itersection -> hitPoint * modelViewMatrix;

					// Sort desc
					std::sort (itersections .begin (), itersections .end (),
					           [ ] (const IntersectionPtr &lhs, const IntersectionPtr &rhs) -> bool
					           {
					              return lhs -> hitPoint .z () > rhs -> hitPoint .z ();
								  });

					// Find first point that is not greater than near plane;
					auto itersection = std::lower_bound (itersections .cbegin (), itersections .cend (), -getCurrentNavigationInfo () -> getNearPlane (),
					                                     [ ] (const IntersectionPtr &lhs, const float & rhs) -> bool
					                                     {
					                                        return lhs -> hitPoint .z () > rhs;
																	 });

					if (itersection not_eq itersections .end ())
						getBrowser () -> addHit (ModelViewMatrix4d (), *itersection, this);
				}
			}
		}
	}
}

void
Shape::drawCollision ()
{
	geometryNode -> draw (false, false, false);
}

void
Shape::drawGeometry ()
{
	geometryNode -> draw ();
}

} // X3D
} // titania
