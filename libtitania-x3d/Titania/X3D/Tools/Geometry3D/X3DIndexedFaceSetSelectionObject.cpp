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

#include "X3DIndexedFaceSetSelectionObject.h"

#include "../Rendering/CoordinateTool.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/NURBS/CoordinateDouble.h"
#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Editing/Selection/FaceSelection.h"


namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8;

X3DIndexedFaceSetSelectionObject::Fields::Fields () :
	       selectable (new SFBool (true)),
	 replaceSelection (new SFBool ())
{ }

X3DIndexedFaceSetSelectionObject::X3DIndexedFaceSetSelectionObject () :
	X3DComposedGeometryNodeTool (),
	                     fields (),
	                touchSensor (),
	                planeSensor (),
	           activePointCoord (),
	        activeEdgesGeometry (),
	             selectionCoord (),
	      selectedEdgesGeometry (),
	      selectedFacesGeometry (),
	                  coordNode (),
	               activePoints (),
	                 activeFace (0),
	                  selection (new FaceSelection (getExecutionContext ())),
	                masterPoint (-1),
	             selectedPoints (),
	              selectedEdges (),
	              selectedFaces ()
{
	//addType (X3DConstants::X3DIndexedFaceSetSelectionObject);

	selectable ()       .isHidden (true);
	replaceSelection () .isHidden (true);

	addChildren (touchSensor,
	             planeSensor,
	             activePointCoord,
	             activeEdgesGeometry,
	             selectionCoord,
	             selectedFacesGeometry,
	             selection);
	            
}

void
X3DIndexedFaceSetSelectionObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_loadState);

	getCoord ()      .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_coord);
	set_selection () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_selection_);

	selection -> geometry () = getNode <IndexedFaceSet> ();
	selection -> setup ();

	set_loadState ();
	set_coord ();
}

void
X3DIndexedFaceSetSelectionObject::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		touchSensor           = inlineNode -> getExportedNode <TouchSensor> ("TouchSensor");
		planeSensor           = inlineNode -> getExportedNode <PlaneSensor> ("PlaneSensor");
		activePointCoord      = inlineNode -> getExportedNode <CoordinateDouble> ("ActivePointCoord");
		activeEdgesGeometry   = inlineNode -> getExportedNode <IndexedLineSet> ("ActiveEdgesGeometry");
		selectionCoord        = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");
		selectedEdgesGeometry = inlineNode -> getExportedNode <IndexedLineSet> ("SelectedEdgesGeometry");
		selectedFacesGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("SelectedFacesGeometry");

		touchSensor -> isOver ()           .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_over);
		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime);

		planeSensor -> isActive () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_plane_sensor_active);

		convex () .addInterest (activeFaceGeometry    -> convex ());
		convex () .addInterest (selectedFacesGeometry -> convex ());

		activeFaceGeometry    -> convex () = convex ();
		selectedFacesGeometry -> convex () = convex ();

		set_coord_point ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetSelectionObject::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this, &X3DIndexedFaceSetSelectionObject::set_coord_point);

	coordNode = getCoord ();

	if (coordNode)
		coordNode -> addInterest (this, &X3DIndexedFaceSetSelectionObject::set_coord_point);

	set_coord_point ();
}

void
X3DIndexedFaceSetSelectionObject::set_coord_point ()
{
	try
	{
		updateActivePoints ();
		updateSelectedPoints ();

		if (planeSensor -> isActive ())
			return;
	
		for (auto & selectedPoint : selectedPoints)
			selectedPoint .second = getCoord () -> get1Point (selectedPoint .first);
	}
	catch (const X3DError &)
	{ }
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_over (const bool over)
{
	if (not over)
	{
		activeEdgesGeometry -> coordIndex () .clear ();
		activePointCoord -> point () .clear ();
	}
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint ()
{
	if (planeSensor -> isActive ())
		return;

	// Find closest points.

	const auto coincidentPoints = selection -> getCoincidentPoints (touchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return;

	// Set active points.

	setActiveSelection (touchSensor -> getHitPoint (), coincidentPoints);

	if (touchSensor -> isActive () and not replaceSelection ())
		set_touch_sensor_touchTime ();
}

void
X3DIndexedFaceSetSelectionObject::set_selection_ (const MFVec3d & hitPoints)
{
	if (not selectable ())
		return;

	std::vector <int32_t> points;

	for (const auto & hitPoint : hitPoints)
	{
		const auto coincidentPoints = selection -> getCoincidentPoints (hitPoint);
		points .insert (points .end (), coincidentPoints .begin (), coincidentPoints .end ());
	}

	// In this order.
	selectPoints (points);
	selectFaces  (points);

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime ()
{
	if (not selectable ())
		return;

	// In this order.
	selectPoints (activePoints);

	if (activePoints .size () < 3)
		selectFaces  (activePoints);
	else
		selectFace (activeFace);

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
}

void
X3DIndexedFaceSetSelectionObject::set_plane_sensor_active (const bool active)
{
	if (active)
		return;

	set_coord_point ();
}

void
X3DIndexedFaceSetSelectionObject::setActiveSelection (const Vector3d & hitPoint, const std::vector <int32_t> & coincidentPoints)
{
	const auto index    = coincidentPoints [0];
	const auto point    = getCoord () -> get1Point (index);
	const auto faces    = selection -> getAdjacentFaces (coincidentPoints);
	const auto face     = selection -> getNearestFace (hitPoint, faces) .first;
	const auto vertices = selection -> getFaceVertices (face);

	activeFace = face;
	activePoints .clear ();

	if (vertices .size () >= 3)
	{
		// Active points for near point or face
	
		if (getDistance (hitPoint, point) > SELECTION_DISTANCE)
		{
			const auto edge     = selection -> getEdge (hitPoint, vertices);
			const auto distance = getDistance (hitPoint, edge .segment .line () .closest_point (hitPoint));

			if (distance > SELECTION_DISTANCE)
			{
				// Face
				for (const auto & vertex : vertices)
					activePoints .emplace_back (coordIndex () [vertex]);
			}
			else
			{
				// Edge
				activePoints = { coordIndex () [edge .index0], coordIndex () [edge .index1] };
			}
		}
		else
		{
			// Point
			activePoints = { index };
		}
	}

	updateActiveFace ();
	updateActivePoints ();
}

void
X3DIndexedFaceSetSelectionObject::updateActivePoints ()
{
	size_t i = 0;

	for (const auto activePoint : activePoints)
		activePointCoord -> point () .set1Value (i ++, getCoord () -> get1Point (activePoint));
	
	activePointCoord -> point () .resize (i);
}

void
X3DIndexedFaceSetSelectionObject::updateActiveFace ()
{
	size_t i = 0;

	if (not activePoints .empty ())
	{
		for (const auto activePoint : activePoints)
			activeEdgesGeometry -> coordIndex () .set1Value (i ++, activePoint);

		activeEdgesGeometry -> coordIndex () .set1Value (i ++, activePoints [0]);
		activeEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	activeEdgesGeometry -> coordIndex () .resize (i);
}

void
X3DIndexedFaceSetSelectionObject::selectPoints (const std::vector <int32_t> & points)
{
	if (replaceSelection () and not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey ())
	{
		selectionCoord -> point () .clear ();
		selectedPoints .clear ();
	}

	if (getBrowser () -> hasControlKey ())
		removeSelectedPoints (points);

	else
		addSelectedPoints (points);
}

void
X3DIndexedFaceSetSelectionObject::selectFaces (const std::vector <int32_t> & points)
{
	if (replaceSelection () and not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey ())
	{
		selectedFacesGeometry -> coordIndex () .clear ();
		selectedFaces .clear ();
		selectedEdges .clear ();
	}

	if (getBrowser () -> hasControlKey ())
	{
		for (const auto & point : points)
		{
			std::set <size_t> faces;

			for (const auto & face : selection -> getAdjacentFaces (point))
				faces .emplace (face .first);

			removeSelectedFaces (faces);
		}
	}
	else
	{
		for (const auto & point : points)
		{
			std::set <size_t> faces;

			for (const auto & face : selection -> getAdjacentFaces (point))
				faces .emplace (face .first);

			addSelectedFaces (faces);
		}
	}
}

void
X3DIndexedFaceSetSelectionObject::selectFace (const size_t face)
{
	if (replaceSelection () and not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey ())
	{
		selectedFacesGeometry -> coordIndex () .clear ();
		selectedFaces .clear ();
		selectedEdges .clear ();
	}

	if (getBrowser () -> hasControlKey ())
		removeSelectedFaces ({ face });

	else
		addSelectedFaces ({ face });
}

void
X3DIndexedFaceSetSelectionObject::addSelectedPoints (const std::vector <int32_t> & points)
{
	if (points .empty ())
		masterPoint = -1;
	else
		masterPoint = points .back ();

	for (const auto & point : points)
		selectedPoints .emplace (point, getCoord () -> get1Point (point));
}

void
X3DIndexedFaceSetSelectionObject::removeSelectedPoints (const std::vector <int32_t> & points)
{
	masterPoint = -1;

	for (const auto & point : points)
		selectedPoints .erase (point);
}

void
X3DIndexedFaceSetSelectionObject::updateSelectedPoints ()
{
	size_t i = 0;

	for (auto & selectedPoint : selectedPoints)
		selectionCoord -> point () .set1Value (i ++, getCoord () -> get1Point (selectedPoint .first));

	selectionCoord -> point () .resize (i);
}

void
X3DIndexedFaceSetSelectionObject::addSelectedEdges (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = i;
		auto i1 = (i0 + 1) % size;

		auto index0 = coordIndex () [vertices [i0]] .getValue ();
		auto index1 = coordIndex () [vertices [i1]] .getValue ();

		if (index0 > index1)
		{
			std::swap (i0, i1);
			std::swap (index0, index1);
		}

		selectedEdges [std::make_pair (index0, index1)] .emplace (std::make_pair (i0, i1)); 
	}
}

void
X3DIndexedFaceSetSelectionObject::removeSelectedEdges (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = i;
		auto i1 = (i0 + 1) % size;

		auto index0 = coordIndex () [vertices [i0]] .getValue ();
		auto index1 = coordIndex () [vertices [i1]] .getValue ();

		if (index0 > index1)
		{
			std::swap (i0, i1);
			std::swap (index0, index1);
		}

		selectedEdges [std::make_pair (index0, index1)] .erase (std::make_pair (i0, i1)); 
	}
}

void
X3DIndexedFaceSetSelectionObject::updateSelectedEdges ()
{
	size_t i = 0;

	for (const auto edge : selectedEdges)
	{
	   if (edge .second .size () not_eq 1)
	      continue;

		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .first);
		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .second);
		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	selectedEdgesGeometry -> coordIndex () .resize (i);
}

void
X3DIndexedFaceSetSelectionObject::addSelectedFaces (const std::set <size_t> & faces)
{
	for (const auto & face : faces)
	{
		const auto vertices = selection -> getFaceVertices (face);

		if (isInSelection (vertices))
		{
			addSelectedEdges (vertices);
			selectedFaces .emplace (face);
		}
	}
}

void
X3DIndexedFaceSetSelectionObject::removeSelectedFaces (const std::set <size_t> & faces)
{
	for (const auto & face : faces)
	{
		const auto vertices = selection -> getFaceVertices (face);

		if (isInSelection (vertices))
		   continue;

		removeSelectedEdges (vertices);
		selectedFaces .erase (face);
	}
}

void
X3DIndexedFaceSetSelectionObject::updateSelectedFaces ()
{
	size_t i = 0;

	for (const auto index : selectedFaces)
	{
		for (const auto vertex : selection -> getFaceVertices (index))
			selectedFacesGeometry -> coordIndex () .set1Value (i ++, coordIndex () [vertex]);
	
		selectedFacesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	selectedFacesGeometry -> coordIndex () .resize (i);
}

bool
X3DIndexedFaceSetSelectionObject::isInSelection (const std::vector <size_t> & vertices) const
{
	for (const auto & vertex : vertices)
	{
		if (selectedPoints .count (coordIndex () [vertex]))
			continue;

		return false;
	}

	return true;
}

double
X3DIndexedFaceSetSelectionObject::getDistance (const Vector3d & point1, const Vector3d & point2)
{
	const auto p1 = ViewVolume::projectPoint (point1, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	const auto p2 = ViewVolume::projectPoint (point2, getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	return abs (Vector2d (p1. x (), p1 .y ()) - Vector2d (p2. x (), p2 .y ()));
}

X3DIndexedFaceSetSelectionObject::~X3DIndexedFaceSetSelectionObject ()
{ }

} // X3D
} // titania
