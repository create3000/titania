/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/PointingDeviceSensor/Hit.h"
#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Types/Geometry.h"
#include "../Navigation/NavigationInfo.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Shape/Appearance.h"

namespace titania {
namespace X3D {

const Component   Shape::component      = Component ("Shape", 3);
const std::string Shape::typeName       = "Shape";
const std::string Shape::containerField = "children";

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
	if (getAppearance () and getAppearance () -> isTransparent ())
		return true;

	if (getGeometry () and getGeometry () -> isTransparent ())
		return true;

	return false;
}

GeometryType
Shape::getGeometryType () const
{
	return getGeometry () -> getGeometryType ();
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
Shape::intersects (const Box3d & box,
                   const ClipPlaneContainerArray & clipPlanes,
                   const Matrix4d & modelViewMatrix)
{
	return getGeometry () -> intersects (box, clipPlanes, modelViewMatrix);
}

void
Shape::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (getGeometry ())
	{
		switch (type)
		{
			case TraverseType::POINTER:
			{
				pointer (renderObject);
				break;
			}
			case TraverseType::CAMERA:
			{
				break;
			}
			case TraverseType::COLLISION:
			{
				renderObject -> addCollisionShape (this);
				break;
			}
			case TraverseType::DEPTH:
			{
				renderObject -> addDepthShape (this);
				break;
			}
			case TraverseType::DISPLAY:
			{
				if (renderObject -> addDisplayShape (this))
					getAppearance () -> traverse (type, renderObject);

				break;
			}
		}

		getGeometry () -> traverse (type, renderObject);
	}
}

void
Shape::pointer (X3DRenderObject* const renderObject)
{
	// All geometries must be picked

	const auto browser = renderObject -> getBrowser ();

	if (not browser -> isPointerInRectangle (renderObject -> getViewVolumes () .back () .getScissor ()))
		return;

	const Box3d bbox = getBBox () * renderObject -> getModelViewMatrix () .get ();

	if (not renderObject -> getViewVolumes () .back () .intersects (bbox))
		return;

	HierarchyGuard guard (renderObject -> getBrowser (), this);
	HierarchyGuard guardGeometry (renderObject -> getBrowser (), getGeometry ());

	switch (browser -> getSelectionType ())
	{
		case SelectionType::DEFAULT:
			touch (renderObject);
			break;
		case SelectionType::LASSO:
			lasso (renderObject);
			break;
		case SelectionType::CUT:
			cut (renderObject);
			break;
	}
}

void
Shape::touch (X3DRenderObject* const renderObject)
{
	try
	{
		std::vector <IntersectionPtr> intersections;

		const auto   browser            = renderObject -> getBrowser ();
		const auto & modelViewMatrix    = renderObject -> getModelViewMatrix () .get ();
		const auto   invModelViewMatrix = inverse (modelViewMatrix);
		const auto   hitRay             = browser -> getHitRay () * invModelViewMatrix;

		if (not getGeometry () -> intersects (hitRay, renderObject -> getClipPlanes (), modelViewMatrix, intersections))
			return;

		// Finally we have intersections and must now find the closest hit in front of the camera.

		// Transform hitPoints to absolute space.
		for (auto & intersection : intersections)
			intersection -> setPoint (intersection -> point * modelViewMatrix);
	
		// Sort desc
		std::sort (intersections .begin (), intersections .end (),
		[ ] (const IntersectionPtr & lhs, const IntersectionPtr & rhs)
		{
			return lhs -> getPoint () .z () > rhs -> getPoint () .z ();
		});

		// Find first point that is not greater than near plane;
		const auto nearestIntersection = std::lower_bound (intersections .cbegin (), intersections .cend (), -renderObject -> getNavigationInfo () -> getNearValue (),
		[ ] (const IntersectionPtr & lhs, const float & rhs)
		{
			return lhs -> getPoint () .z () > rhs;
		});

		// There are only intersections behind the camera.
		if (nearestIntersection == intersections .end ())
			return;

		const auto & intersection = *nearestIntersection;

		// Transform normals to absolute space.
		intersection -> setNormal     (normalize (invModelViewMatrix .mult_matrix_dir (intersection -> getNormal ())));
		intersection -> setFaceNormal (normalize (invModelViewMatrix .mult_matrix_dir (intersection -> getFaceNormal ())));

		browser -> addHit (modelViewMatrix, intersection, this, renderObject -> getLayer (), renderObject -> getDepthOffset () .top ());
	}
	catch (const std::domain_error &)
	{
		return;
	}
}

void
Shape::lasso (X3DRenderObject* const renderObject)
{
	const auto browser = renderObject -> getBrowser ();

	std::vector <IntersectionPtr> intersections;

	getGeometry () -> intersects (renderObject,
	                              browser -> getSelectionBuffer (),
	                              browser -> getDepthBuffer (),
	                              intersections);
}

void
Shape::cut (X3DRenderObject* const renderObject)
{
	const auto browser = renderObject -> getBrowser ();

	getGeometry () -> cut (renderObject, browser -> getCutLine ());
}

void
Shape::depth (const X3DShapeContainer* const context)
{
	getGeometry () -> depth (context);
}

void
Shape::draw (ShapeContainer* const context)
{
	getGeometry () -> draw (context);
}

} // X3D
} // titania
