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
#include "../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"

namespace titania {
namespace X3D {

constexpr double SELECTION_DISTANCE = 6;

IndexedFaceSetTool::IndexedFaceSetTool (IndexedFaceSet* const node) :
	                                 X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
	                X3DBaseTool <IndexedFaceSet> (node),
	X3DComposedGeometryNodeTool <IndexedFaceSet> (),
	                                   coordNode (),
	                                   selection (new FaceSelection ()),
	                              selectedPoints ()
{
	addType (X3DConstants::IndexedFaceSetTool);

	addField (inputOutput, "paintSelection",    paintSelection ());
	addField (inputOutput, "selection_changed", selection_changed ());
	addField (inputOutput, "normalTool",        normalTool ());
	addField (inputOutput, "coordTool",         coordTool ());
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
		const auto planeSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <PlaneSensor> ("PlaneSensor");
		const auto lineSet     = getCoordinateTool () -> getInlineNode () -> getExportedNode <IndexedLineSet> ("ActiveLineSet");

		touchSensor -> isOver ()           .addInterest (this, &IndexedFaceSetTool::set_over);
		touchSensor -> hitPoint_changed () .addInterest (this, &IndexedFaceSetTool::set_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &IndexedFaceSetTool::set_touchTime);

		planeSensor -> isActive ()            .addInterest (this, &IndexedFaceSetTool::set_active);
		planeSensor -> translation_changed () .addInterest (this, &IndexedFaceSetTool::set_translation);

		coord () .addInterest (lineSet -> coord ());

		lineSet -> coord () = coord ();
		set_coord_point ();
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this, &IndexedFaceSetTool::set_coord_point);

	coordNode = getCoord ();

	if (coordNode)
		coordNode -> addInterest (this, &IndexedFaceSetTool::set_coord_point);

	set_coord_point ();
}

void
IndexedFaceSetTool::set_coord_point ()
{
	try
	{
		const auto planeSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <PlaneSensor> ("PlaneSensor");
		const auto coord       = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("SelectionCoord");
	
		// Update selected points
	
		std::vector <Vector3d> points;
	
		for (auto & pair : selectedPoints)
			points .emplace_back (getCoord () -> get1Point (pair .first));
	
		std::sort (points .begin (), points .end ());
	
		coord -> point () .assign (points .begin (), points .end ());
	
		if (planeSensor -> isActive ())
			return;
	
		for (auto & pair : selectedPoints)
			pair .second = getCoord () -> get1Point (pair .first);
	
		// Update selection

		selection -> setCoord (getCoord ());
	}
	catch (const X3DError &)
	{ }
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
		const auto planeSensor = getCoordinateTool () -> getInlineNode () -> getExportedNode <PlaneSensor> ("PlaneSensor");
		const auto lineSet     = getCoordinateTool () -> getInlineNode () -> getExportedNode <IndexedLineSet> ("ActiveLineSet");
		const auto pointCoord  = getCoordinateTool () -> getInlineNode () -> getExportedNode <Coordinate> ("ActivePointCoord");

		if (planeSensor -> isActive ())
			return;

		// Determine face and faces

		selection -> setIndices (hitPoint, touchSensor -> hitTriangle_changed ());

		if (selection -> getIndices () .empty ())
			return;

		// Set selected point

		selection -> setFaces (hitPoint);

		set_active_selection (hitPoint);

		if (touchSensor -> isActive () and paintSelection ())
			set_touchTime ();

		// Setup PlaneSensor

		if (pointCoord -> point () .empty ())
		{
			if (lineSet -> coordIndex () .empty ())
				planeSensor -> enabled () = false;
			else
			{
				// Translate along line

				const auto vector       = getCoord () -> get1Point (lineSet -> coordIndex () [0]) - getCoord () -> get1Point (lineSet -> coordIndex () [1]);
				const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);
	
				planeSensor -> enabled ()      = true;
				planeSensor -> axisRotation () = axisRotation;
				planeSensor -> maxPosition ()  = Vector2f (-1, 0);
			}
		}
		else
		{
			// Translate over screen plane

			const auto vector       = inverse (getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
			const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);

			planeSensor -> enabled ()      = true;
			planeSensor -> axisRotation () = axisRotation;
			planeSensor -> maxPosition ()  = Vector2f (-1, -1);
		}
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_selection (const MFVec3f & vertices)
{
	for (const auto & vertex : vertices)
	{
		selection -> setIndices (vertex .getValue ());
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
		if (selection -> getIndices () .empty ())
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
			selectedPoints .clear ();
		}

		const auto iter = std::lower_bound (coord -> point () .begin (),
                                          coord -> point () .end (),
                                          point,
                                          [ ] (const SFVec3f & value, const Vector3f & point) { return value .getValue () < point; });

		if (getBrowser () -> hasControlKey ())
		{
			if (iter not_eq coord -> point () .end () and *iter == point)
				coord -> point () .erase (iter);

			selectedPoints .erase (index);
			return;
		}
		else if (not selectedPoints .emplace (index, point) .second)
			return;

		coord -> point () .emplace (iter, point);
	}
	catch (const X3D::X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_active_selection (const Vector3f & hitPoint)
{
	try
	{
		const auto lineSet    = getCoordinateTool () -> getInlineNode () -> getExportedNode <IndexedLineSet> ("ActiveLineSet");
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
			lineSet -> coordIndex () .clear ();
			return;
		}

		// Active line

		if (paintSelection ())
			lineSet -> coordIndex () .clear ();

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
						lineSet -> coordIndex () .clear ();
						break;
					}
	
					if (distance0 < distance1)
					{
						lineSet -> coordIndex () .set1Value (0, coordIndex () [index0]);
						lineSet -> coordIndex () .set1Value (1, index);
					}
					else
					{
						lineSet -> coordIndex () .set1Value (0, coordIndex () [index1]);
						lineSet -> coordIndex () .set1Value (1, index);
					}
	
					break;
				}
			}
		}
	}
	catch (const X3DError &)
	{ }
}

void
IndexedFaceSetTool::set_active (const bool active)
{
	if (active)
		return;

	set_coord_point ();
}

void
IndexedFaceSetTool::set_translation (const Vector3f & translation)
{
	__LOG__ << translation << std::endl;

	for (const auto & pair : selectedPoints)
		getCoord () -> set1Point (pair .first, pair .second + Vector3d (translation));
}

double
IndexedFaceSetTool::get_distance (const Vector3d & point1, const Vector3d & point2)
{
	const auto p1 = ViewVolume::projectPoint (point1, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	const auto p2 = ViewVolume::projectPoint (point2, getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	return abs (Vector2d (p1. x (), p1 .y ()) - Vector2d (p2. x (), p2 .y ()));
}

void
IndexedFaceSetTool::dispose ()
{
	selection .reset ();

	X3DComposedGeometryNodeTool <IndexedFaceSet>::dispose ();
}

} // X3D
} // titania
