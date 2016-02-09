/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "IndexedFaceSetTool.h"

#include "../../Browser/Networking/config.h"
#include "../../Editing/FaceSelection.h"

#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/Rendering/Coordinate.h"

namespace titania {
namespace X3D {

constexpr double SELECTION_DISTANCE = 6;

IndexedFaceSetTool::IndexedFaceSetTool (IndexedFaceSet* const node) :
	                                 X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                X3DBaseTool <IndexedFaceSet> (node),
	X3DComposedGeometryNodeTool <IndexedFaceSet> (),
	                                   selection (new FaceSelection ()),
	                            selectedVertices ()
{
	addType (X3DConstants::IndexedFaceSetTool);

	this -> addField (inputOutput, "paintSelection",    paintSelection ());
	this -> addField (inputOutput, "selection_changed", selection_changed ());
	this -> addField (inputOutput, "normalTool",        normalTool ());
	this -> addField (inputOutput, "coordTool",         coordTool ());
}

void
IndexedFaceSetTool::initialize ()
{
	X3DComposedGeometryNodeTool <IndexedFaceSet>::initialize ();

	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &IndexedFaceSetTool::set_loadState);
	getCoord () .addInterest (this, &IndexedFaceSetTool::set_coord);
	selection_changed () .addInterest (this, &IndexedFaceSetTool::set_selection);

	selection -> setGeometry (getNode ());

	set_loadState ();
	set_coord ();
}

void
IndexedFaceSetTool::set_loadState ()
{
	try
	{
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");

		touchSensor -> isOver ()           .addInterest (this, &IndexedFaceSetTool::set_over);
		touchSensor -> hitPoint_changed () .addInterest (this, &IndexedFaceSetTool::set_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &IndexedFaceSetTool::set_touchTime);
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_coord ()
{
	selection -> setCoord (getCoord ());
}

void
IndexedFaceSetTool::set_over (const bool over)
{
	try
	{
		if (not over)
		{
			const auto coord = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("ActivePointCoord");

			coord -> point () .clear ();
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_hitPoint (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");

		// Determine face and faces

		selection -> setHitPoint (hitPoint, touchSensor -> hitTriangle_changed ());

		if (selection -> isEmpty ())
			return;

		// Set selected point

		set_active_selection (hitPoint);

		if (touchSensor -> isActive () and paintSelection ())
			set_touchTime ();
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_selection (const MFVec3f & vertices)
{
	__LOG__ << std::endl;
	__LOG__ << getBrowser () -> hasControlKey () << std::endl;
	__LOG__ << getBrowser () -> hasShiftKey () << std::endl;
	__LOG__ << std::endl;

	for (const auto & vertex : vertices)
	{
		selection -> setHitPoint (vertex .getValue ());
		set_point (vertex);
	}
}

void
IndexedFaceSetTool::set_touchTime ()
{
	try
	{
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");
		const auto hitPoint    = touchSensor -> hitPoint_changed () .getValue ();

		set_point (hitPoint);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_point (const Vector3f & hitPoint)
{
	try
	{
		if (selection -> isEmpty ())
			return;
	
		const auto touchSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <TouchSensor> ("TouchSensor");
		const auto coord       = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("SelectionCoord");
		const auto index       = selection -> getIndices () [0];
		const auto point       = getCoord () -> get1Point (index);

		if (get_distance (hitPoint, point) > SELECTION_DISTANCE)
			return;

		if (not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey () and not paintSelection ())
		{
			coord -> point () .clear ();
			selectedVertices .clear ();
		}

		if (getBrowser () -> hasControlKey ())
		{
			const auto iter = std::find (coord -> point () .begin (), coord -> point () .end (), point);

			if (iter not_eq coord -> point () .end ())
				coord -> point () .erase (iter);

			selectedVertices .erase (index);
			return;
		}
		else if (not selectedVertices .emplace (index) .second)
			return;

		coord -> point () .emplace_back (point);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_active_selection (const X3D::Vector3f & hitPoint)
{
	try
	{
		const auto lineCoord  = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("ActiveLineCoord");
		const auto pointCoord = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("ActivePointCoord");
		const auto vertices   = selection -> getVertices (selection -> getFace () .first);
		const auto index      = selection -> getIndices () [0];
		const auto point      = getCoord () -> get1Point (index);

		if (vertices .size () < 3)
			return;

		// Active point

		if (get_distance (hitPoint, point) > SELECTION_DISTANCE)
			pointCoord -> point () .clear ();
		else
		{
			pointCoord -> point () .set1Value (0, point);
			lineCoord  -> point () .clear ();
			return;
		}

		// Active line

		if (paintSelection ())
			lineCoord -> point () .clear ();

		else
		{
			for (size_t i = 0, size = vertices .size (); i < size; ++ i)
			{
				if (int32_t (index) == coordIndex () [vertices [i]])
				{
					const size_t index0    = vertices [(i + vertices .size () - 1) % vertices .size ()];
					const size_t index1    = vertices [(i + 1) % vertices .size ()];

					const auto   point0    = getCoord () -> get1Point (coordIndex () [index0]);
					const auto   point1    = getCoord () -> get1Point (coordIndex () [index1]);

					const auto   line0     = Line3d (point0, point, math::point_type ());
					const auto   line1     = Line3d (point1, point, math::point_type ());

					const auto   distance0 = get_distance (hitPoint, line0 .closest_point (hitPoint));
					const auto   distance1 = get_distance (hitPoint, line1 .closest_point (hitPoint));
	
					if (distance0 > SELECTION_DISTANCE and distance1 > SELECTION_DISTANCE)
					{
						lineCoord -> point () .clear ();
						break;
					}
	
					if (distance0 < distance1)
					{
						lineCoord -> point () .set1Value (0, point0);
						lineCoord -> point () .set1Value (1, point);
					}
					else
					{
						lineCoord -> point () .set1Value (0, point1);
						lineCoord -> point () .set1Value (1, point);
					}
	
					break;
				}
			}
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

double
IndexedFaceSetTool::get_distance (const X3D::Vector3d & point1, const X3D::Vector3d & point2)
{
	const auto p1 = X3D::ViewVolume::projectPoint (point1, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	const auto p2 = X3D::ViewVolume::projectPoint (point2, getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	return abs (X3D::Vector2d (p1. x (), p1 .y ()) - X3D::Vector2d (p2. x (), p2 .y ()));
}

void
IndexedFaceSetTool::dispose ()
{
	selection .reset  ();

	X3DComposedGeometryNodeTool <IndexedFaceSet>::dispose ();
}

} // X3D
} // titania
