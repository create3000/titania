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
#include "../../Rendering/Tessellator.h"

namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8;

X3DIndexedFaceSetSelectionObject::Fields::Fields () :
	                selectable (new SFBool (true)),
	             selectionType (new SFString ("POINTS")),
	            paintSelection (new SFBool ()),
	              addSelection (new MFInt32 ()),
	          replaceSelection (new MFInt32 ()),
	    selectedPoints_changed (new SFInt32 ()),
	     selectedEdges_changed (new SFInt32 ()),
	     selectedFaces_changed (new SFInt32 ())
{ }

X3DIndexedFaceSetSelectionObject::X3DIndexedFaceSetSelectionObject () :
	X3DComposedGeometryNodeTool (),
	                     fields (),
	                touchSensor (),
	                planeSensor (),
	              hotPointCoord (),
	           hotEdgesGeometry (),
	           activePointCoord (),
	        activeEdgesGeometry (),
	             selectionCoord (),
	      selectedEdgesGeometry (),
	      selectedFacesGeometry (),
	                  coordNode (),
	                  selection (new FaceSelection (getExecutionContext ())),
	                  hotPoints (),
	                    hotEdge (),
	                    hotFace (0),
	               activePoints (),
	                 activeEdge (),
	                 activeFace (0),
	                       type (SelectionType::POINTS),
	                masterPoint (-1),
	             selectedPoints (),
	              selectedEdges (),
	              selectedFaces ()
{
	addType (X3DConstants::X3DIndexedFaceSetSelectionObject);

	selectable ()             .isHidden (true);
	selectionType ()          .isHidden (true);
	paintSelection ()         .isHidden (true);
	addSelection ()           .isHidden (true);
	replaceSelection ()       .isHidden (true);
	selectedPoints_changed () .isHidden (true);
	selectedEdges_changed ()  .isHidden (true);
	selectedFaces_changed ()  .isHidden (true);

	addChildren (touchSensor,
	             planeSensor,
	             hotPointCoord,
	             hotEdgesGeometry,
	             activePointCoord,
	             activeEdgesGeometry,
	             selectionCoord,
	             selectedEdgesGeometry,
	             selectedFacesGeometry,
	             coordNode,
	             selection);
	            
}

void
X3DIndexedFaceSetSelectionObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_loadState);

	selectionType ()    .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_selectionType);
	getCoord ()         .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_coord);
	addSelection ()     .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_addSelection_);
	replaceSelection () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_replaceSelection_);

	selection -> geometry () = getNode <IndexedFaceSet> ();
	selection -> setup ();

	set_selectionType ();
	set_loadState ();
	set_coord ();
}

void
X3DIndexedFaceSetSelectionObject::set_selectionType ()
{
	try
	{
	   static const std::map <std::string, SelectionType> selectionTypes = {
			std::make_pair ("POINTS", SelectionType::POINTS),
			std::make_pair ("EDGES",  SelectionType::EDGES),
			std::make_pair ("FACES",  SelectionType::FACES),
		};

		__LOG__ << selectionType () << std::endl;

		type = selectionTypes .at (selectionType ());
	}
	catch (const std::out_of_range &)
	{
		__LOG__ << selectionType () << std::endl;
	   type = SelectionType::POINTS;
	}

	//select ({ }, true);
}

void
X3DIndexedFaceSetSelectionObject::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   hotFaceGeometry    = inlineNode -> getExportedNode <IndexedFaceSet> ("HotFaceGeometry");
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		touchSensor           = inlineNode -> getExportedNode <TouchSensor> ("TouchSensor");
		planeSensor           = inlineNode -> getExportedNode <PlaneSensor> ("PlaneSensor");
		hotPointCoord         = inlineNode -> getExportedNode <CoordinateDouble> ("HotPointCoord");
		hotEdgesGeometry      = inlineNode -> getExportedNode <IndexedLineSet> ("HotEdgesGeometry");
		activePointCoord      = inlineNode -> getExportedNode <CoordinateDouble> ("ActivePointCoord");
		activeEdgesGeometry   = inlineNode -> getExportedNode <IndexedLineSet> ("ActiveEdgesGeometry");
		selectionCoord        = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");
		selectedEdgesGeometry = inlineNode -> getExportedNode <IndexedLineSet> ("SelectedEdgesGeometry");
		selectedFacesGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("SelectedFacesGeometry");

		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint);
		touchSensor -> isOver ()           .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_over);
		touchSensor -> isActive ()         .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_active);
		touchSensor -> touchTime ()        .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime);

		planeSensor -> isActive () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_plane_sensor_active);

		convex () .addInterest (activeFaceGeometry    -> convex ());
		convex () .addInterest (selectedFacesGeometry -> convex ());

		hotFaceGeometry       -> convex () = convex ();
		activeFaceGeometry    -> convex () = convex ();
		selectedFacesGeometry -> convex () = convex ();

		set_coord_point ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
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
		updateMagicPoints ();
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
X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint ()
{
	if (planeSensor -> isActive ())
		return;

	// Find closest points.

	const auto coincidentPoints = selection -> getCoincidentPoints (touchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return;

	// Set active points.

	setMagicSelection (touchSensor -> getHitPoint (), coincidentPoints);

	if (touchSensor -> isActive () and paintSelection ())
		set_touch_sensor_touchTime ();
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_over (const bool over)
{
	if (over)
		return;

	activePoints .clear ();
	activeFace = -1;

	updateMagicSelection ();
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_active (const bool active)
{
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime ()
{
	if (not selectable ())
		return;

	const bool replace = not paintSelection () and not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey ();

	if (replace)
	{
		selectedPoints .clear ();
		selectedEdges  .clear ();
		selectedFaces  .clear ();
	}
		
	switch (type)
	{
		case SelectionType::POINTS:
			selectPoints (activePoints);
			break;

		case SelectionType::EDGES:
			selectEdge (activeEdge);
			break;

		case SelectionType::FACES:
		{
			if (activePoints .size () < 3)
				selectFaces (activePoints);
			else
				selectFace (activeFace);

			break;
		}
	}

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
	updateMagicPoints ();
}

void
X3DIndexedFaceSetSelectionObject::set_selection (const std::vector <Vector3d> & hitPoints)
{
	if (not selectable ())
		return;

	std::vector <int32_t> points;

	for (const auto & hitPoint : hitPoints)
	{
		const auto coincidentPoints = selection -> getCoincidentPoints (hitPoint);
		points .insert (points .end (), coincidentPoints .begin (), coincidentPoints .end ());
	}

	const bool replace = not paintSelection () and not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey ();

	select (points, replace);
}

void
X3DIndexedFaceSetSelectionObject::set_addSelection_ ()
{
	select (std::vector <int32_t> (addSelection () .begin (), addSelection () .end ()), false);
}

void
X3DIndexedFaceSetSelectionObject::set_replaceSelection_ ()
{
	__LOG__ << replaceSelection () .size () << std::endl;

	select (std::vector <int32_t> (replaceSelection () .begin (), replaceSelection () .end ()), true);
}

void
X3DIndexedFaceSetSelectionObject::set_plane_sensor_active (const bool active)
{
	if (active)
		return;

	set_coord_point ();
}

///  Determine and update hot and active points, edges and face
void
X3DIndexedFaceSetSelectionObject::setMagicSelection (const Vector3d & hitPoint, const std::vector <int32_t> & coincidentPoints)
{
	const auto index    = coincidentPoints [0];
	const auto point    = getCoord () -> get1Point (index);
	const auto faces    = selection -> getAdjacentFaces (coincidentPoints);
	const auto face     = selection -> getNearestFace (hitPoint, faces) .first;
	const auto vertices = selection -> getFaceVertices (face);

	hotFace = face;
	hotEdge   .clear ();
	hotPoints .clear ();

	activeFace = face;
	activeEdge   .clear ();
	activePoints .clear ();

	if (vertices .size () >= 3)
	{
		const auto edge          = selection -> getEdge (hitPoint, vertices);
		const auto edgeDistance  = getDistance (hitPoint, edge .segment .line () .closest_point (hitPoint));
		const auto pointDistance = getDistance (hitPoint, point);

		// Hot points for near point or face
	
		if (pointDistance > SELECTION_DISTANCE)
		{
			if (edgeDistance > SELECTION_DISTANCE)
			{
				// Face
				for (const auto & vertex : vertices)
					hotPoints .emplace_back (coordIndex () [vertex]);
			}
			else
			{
				// Edge
				hotEdge   = { edge .index0, edge .index1 };
				hotPoints = { coordIndex () [edge .index0], coordIndex () [edge .index1] };
			}
		}
		else
		{
			// Point
			hotPoints = { index };
		}

		// Active points for near point or face
	
		switch (type)
		{
			case SelectionType::POINTS:
			{
				if (pointDistance <= SELECTION_DISTANCE)
					activePoints = { index };

				break;
			}
			case SelectionType::EDGES:
			{
				if (edge .isEdge and edgeDistance <= SELECTION_DISTANCE and pointDistance > std::sqrt (2) * SELECTION_DISTANCE)
				{
					activeEdge   = { edge .index0, edge .index1 };
					activePoints = { coordIndex () [edge .index0], coordIndex () [edge .index1] };
				}

				break;
			}
			case SelectionType::FACES:
			{
				for (const auto & vertex : vertices)
					activePoints .emplace_back (coordIndex () [vertex]);

				break;
			}
		}
	}

	updateMagicSelection ();
}

void
X3DIndexedFaceSetSelectionObject::updateMagicSelection ()
{
	updateMagicFace ();
	updateMagicPoints ();
}

///  Update hot and active points and edges geometries.
void
X3DIndexedFaceSetSelectionObject::updateMagicPoints ()
{
	{
		size_t i = 0;

		for (const auto & hotPoint : hotPoints)
			hotPointCoord -> point () .set1Value (i ++, getCoord () -> get1Point (hotPoint));
		
		hotPointCoord -> point () .resize (i);
	}

	{
		size_t i = 0;

		for (const auto & activePoint : activePoints)
			activePointCoord -> point () .set1Value (i ++, getCoord () -> get1Point (activePoint));
		
		activePointCoord -> point () .resize (i);
	}
}

///  Update hot and active face geometry.
void
X3DIndexedFaceSetSelectionObject::updateMagicFace ()
{
	{
		size_t i = 0;

		if (not hotPoints .empty ())
		{
			for (const auto & hotPoint : hotPoints)
				hotEdgesGeometry -> coordIndex () .set1Value (i ++, hotPoint);

			hotEdgesGeometry -> coordIndex () .set1Value (i ++, hotPoints [0]);
			hotEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
		}

		hotEdgesGeometry -> coordIndex () .resize (i);
	}

	{
		size_t i = 0;

		if (not activePoints .empty ())
		{
			for (const auto & activePoint : activePoints)
				activeEdgesGeometry -> coordIndex () .set1Value (i ++, activePoint);

			activeEdgesGeometry -> coordIndex () .set1Value (i ++, activePoints [0]);
			activeEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
		}

		activeEdgesGeometry -> coordIndex () .resize (i);
	}
}

///  Select points, edges or faces depending on selection type.
void
X3DIndexedFaceSetSelectionObject::select (const std::vector <int32_t> & points, const bool replace)
{
	if (replace)
	{
		selectedPoints .clear ();
		selectedEdges  .clear ();
		selectedFaces  .clear ();
	}

	switch (type)
	{
		case SelectionType::POINTS:
			selectPoints (points);
			break;

		case SelectionType::EDGES:
			selectEdges (points);
			break;

		case SelectionType::FACES:
			selectFaces  (points);
			break;
	}

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
	updateMagicPoints ();
}

///  Select points.
void
X3DIndexedFaceSetSelectionObject::selectPoints (const std::vector <int32_t> & points)
{
	if (getBrowser () -> hasControlKey ())
		removeSelectedPoints (points);

	else
		addSelectedPoints (points);
}

///  Select edges.
void
X3DIndexedFaceSetSelectionObject::selectEdges (const std::vector <int32_t> & points)
{
	const std::set <int32_t> pointIndex (points .begin (), points .end ());

	if (getBrowser () -> hasControlKey ())
	{
		for (const auto & point : points)
		{
			for (const auto & face : selection -> getAdjacentFaces (point))
			{
				const auto vertices = selection -> getFaceVertices (face .first);

				for (size_t i = 0, size = vertices .size (); i < size; ++ i)
				{
					const auto i0 = vertices [i];
					const auto i1 = vertices [(i + 1) % size];

					if (pointIndex .count (coordIndex () [i0]) and pointIndex .count (coordIndex () [i1]))
						removeSelectedEdges ({ i0, i1 });
				}
			}
		}
	}
	else
	{
		for (const auto & point : points)
		{
			for (const auto & face : selection -> getAdjacentFaces (point))
			{
				const auto vertices = selection -> getFaceVertices (face .first);

				for (size_t i = 0, size = vertices .size (); i < size; ++ i)
				{
					const auto i0 = vertices [i];
					const auto i1 = vertices [(i + 1) % size];

					if (pointIndex .count (coordIndex () [i0]) and pointIndex .count (coordIndex () [i1]))
						addSelectedEdges ({ i0, i1 });
				}
			}
		}
	}
	
	selectedPoints .clear ();

	for (const auto & edge : selectedEdges)
	{
		if (edge .second .empty ())
			continue;

		selectedPoints .emplace (edge .first .first,  getCoord () -> get1Point (edge .first .first));
		selectedPoints .emplace (edge .first .second, getCoord () -> get1Point (edge .first .second));
	}
}

///  Select one edge.
void
X3DIndexedFaceSetSelectionObject::selectEdge (const std::vector <size_t> & edge)
{
	if (getBrowser () -> hasControlKey ())
		removeSelectedEdges (edge);

	else
		addSelectedEdges (edge);
	
	selectedPoints .clear ();

	for (const auto & edge : selectedEdges)
	{
		if (edge .second .empty ())
			continue;

		selectedPoints .emplace (edge .first .first,  getCoord () -> get1Point (edge .first .first));
		selectedPoints .emplace (edge .first .second, getCoord () -> get1Point (edge .first .second));
	}
}

///  Select faces.
void
X3DIndexedFaceSetSelectionObject::selectFaces (const std::vector <int32_t> & points)
{
	selectPoints (points);

	for (const auto & point : points)
	{
		std::set <size_t> faces;

		for (const auto & face : selection -> getAdjacentFaces (point))
			faces .emplace (face .first);

		if (getBrowser () -> hasControlKey ())
			removeSelectedFaces (faces);
		else
			addSelectedFaces (faces);
	}

	selectedPoints .clear ();

	for (const auto & face : selectedFaces)
		for (const auto & vertex : selection -> getFaceVertices (face))
		   selectedPoints .emplace (coordIndex () [vertex], getCoord () -> get1Point (coordIndex () [vertex]));
}

///  Select one face.
void
X3DIndexedFaceSetSelectionObject::selectFace (const size_t face)
{
	for (const auto & vertex : selection -> getFaceVertices (face))
		selectPoints ({ coordIndex () [vertex] });

	if (getBrowser () -> hasControlKey ())
		removeSelectedFaces ({ face });

	else
		addSelectedFaces ({ face });
}

///  Add @a points to selection of points.
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

///  Remove @a points to selection of points.
void
X3DIndexedFaceSetSelectionObject::removeSelectedPoints (const std::vector <int32_t> & points)
{
	masterPoint = -1;

	for (const auto & point : points)
		selectedPoints .erase (point);
}

///  Update selected points geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedPoints ()
{
	size_t i = 0;

	for (auto & selectedPoint : selectedPoints)
		selectionCoord -> point () .set1Value (i ++, getCoord () -> get1Point (selectedPoint .first));

	selectionCoord -> point () .resize (i);

	selectedPoints_changed () = i;
}

///  Determine edges from @a vertices and add them to selected edges.
void
X3DIndexedFaceSetSelectionObject::addSelectedEdges (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = vertices [i];
		auto i1 = vertices [(i + 1) % size];

		auto index0 = coordIndex () [i0] .getValue ();
		auto index1 = coordIndex () [i1] .getValue ();

		if (index0 > index1)
		{
			std::swap (i0, i1);
			std::swap (index0, index1);
		}

		selectedEdges [std::make_pair (index0, index1)] .emplace (std::make_pair (i0, i1)); 
	}
}

///  Determine edges from @a vertices and remove them from selected edges.
void
X3DIndexedFaceSetSelectionObject::removeSelectedEdges (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = vertices [i];
		auto i1 = vertices [(i + 1) % size];

		auto index0 = coordIndex () [i0] .getValue ();
		auto index1 = coordIndex () [i1] .getValue ();

		if (index0 > index1)
		{
			std::swap (i0, i1);
			std::swap (index0, index1);
		}

		selectedEdges [std::make_pair (index0, index1)] .erase (std::make_pair (i0, i1)); 
	}
}

///  Update selected edges geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedEdges ()
{
	size_t i = 0;

	for (const auto & edge : selectedEdges)
	{
	   if ((type == SelectionType::FACES and edge .second .size () not_eq 1) or edge .second .empty ())
			continue;

		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .first);
		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .second);
		selectedEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	selectedEdgesGeometry -> coordIndex () .resize (i);

	selectedEdges_changed () = i / 3;
}

///  Add @a faces to selection of faces.
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

///  Remove @a faces from selection of faces.
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

///  Update selected faces geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedFaces ()
{
	size_t i = 0;

	for (const auto & index : selectedFaces)
	{
		for (const auto & vertex : selection -> getFaceVertices (index))
			selectedFacesGeometry -> coordIndex () .set1Value (i ++, coordIndex () [vertex]);
	
		selectedFacesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	selectedFacesGeometry -> coordIndex () .resize (i);

	selectedFaces_changed () = selectedFaces .size ();
}

///  Returns true if all points destribed by @a vertices are in the set of selected of points, otherwise false. 
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

///  Returns the sceen distance in pixels between @a point1 and @a point2.
double
X3DIndexedFaceSetSelectionObject::getDistance (const Vector3d & point1, const Vector3d & point2)
{
	const auto p1 = ViewVolume::projectPoint (point1, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	const auto p2 = ViewVolume::projectPoint (point2, getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	return abs (Vector2d (p1. x (), p1 .y ()) - Vector2d (p2. x (), p2 .y ()));
}

///  Returns the sceen area in pixels of @a polygon.
double
X3DIndexedFaceSetSelectionObject::getArea (const std::vector <size_t> & vertices)
{
	double area = 0;

	if (convex ())
	{
		for (size_t i = 0, size = vertices .size () - 1; i < size; ++ i)
		{
			auto p0 = ViewVolume::projectPoint (getCoord () -> get1Point (coordIndex () [vertices [0]]),     getModelViewMatrix (), getProjectionMatrix (), getViewport ());
			auto p1 = ViewVolume::projectPoint (getCoord () -> get1Point (coordIndex () [vertices [i]]),     getModelViewMatrix (), getProjectionMatrix (), getViewport ());
			auto p2 = ViewVolume::projectPoint (getCoord () -> get1Point (coordIndex () [vertices [i + 1]]), getModelViewMatrix (), getProjectionMatrix (), getViewport ());

			p0 .z (0);
			p1 .z (0);
			p2 .z (0);

			area += math::area (p0, p1, p2);
		}
	}
	else
	{
		opengl::tessellator <size_t> tessellator;

		tessellator .begin_polygon ();
		tessellator .begin_contour ();

		for (const auto vertex : vertices)
		{
			const auto point = getCoord () -> get1Point (coordIndex () [vertex]);
			auto       p     = ViewVolume::projectPoint (point, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
			p .z (0);

			tessellator .add_vertex (p);
		}

		tessellator .end_contour ();
		tessellator .end_polygon ();

		const auto triangles = tessellator .triangles ();

		for (size_t v = 0, size = triangles .size (); v < size; v += 3)
			area += math::area (triangles [v] .point (), triangles [v + 1] .point (), triangles [v + 2] .point ());
	}

	return area;
}

void
X3DIndexedFaceSetSelectionObject::undoRestoreSelection (const UndoStepPtr & undoStep)
{
	std::vector <int32_t> points;

	for (const auto & selectedPoint : getSelectedPoints ())
		points .emplace_back (selectedPoint .first);

	undoStep -> addUndoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelection, SFNode (this), points, true);
}

void
X3DIndexedFaceSetSelectionObject::redoRestoreSelection (const UndoStepPtr & undoStep)
{
	std::vector <int32_t> points;

	for (const auto & selectedPoint : getSelectedPoints ())
		points .emplace_back (selectedPoint .first);

	undoStep -> addRedoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelection, SFNode (this), points, true);
}

void
X3DIndexedFaceSetSelectionObject::restoreSelection (const SFNode & node, const std::vector <int32_t> & points, const bool replace)
{
	X3DPtr <X3DIndexedFaceSetSelectionObject> tool (node);

	if (tool)
	{
		if (replace)
			tool -> replaceSelection () .assign (points .begin (), points .end ());
		else
			tool -> addSelection () .assign (points .begin (), points .end ());		
	}
}

X3DIndexedFaceSetSelectionObject::~X3DIndexedFaceSetSelectionObject ()
{ }

} // X3D
} // titania
