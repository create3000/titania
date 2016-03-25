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

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Components/Rendering/IndexedLineSet.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Rendering/Tessellator.h"
#include "../../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8;

X3DIndexedFaceSetSelectionObject::Fields::Fields () :
	                select (new SFBool (true)),
	        paintSelection (new SFBool ()),
	         selectionType (new SFString ("POINTS")),
	             selectAll (new SFTime ()),
	           deselectAll (new SFTime ()),
	      replaceSelection (new MFInt32 ()),
	          addSelection (new MFInt32 ()),
	       removeSelection (new MFInt32 ()),
	  replaceSelectedEdges (new MFInt32 ()),
	      addSelectedEdges (new MFInt32 ()),
	   removeSelectedEdges (new MFInt32 ()),
	  replaceSelectedFaces (new MFInt32 ()),
	      addSelectedFaces (new MFInt32 ()),
	   removeSelectedFaces (new MFInt32 ()),
	selectedPoints_changed (new SFInt32 ()),
	 selectedEdges_changed (new SFInt32 ()),
	 selectedHoles_changed (new SFInt32 ()),
	 selectedFaces_changed (new SFInt32 ())
{ }

X3DIndexedFaceSetSelectionObject::X3DIndexedFaceSetSelectionObject () :
	X3DComposedGeometryNodeTool (),
	                     fields (),
	                touchSensor (),
	                planeSensor (),
	                  hotSwitch (),
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
	              selectedHoles (),
	              selectedFaces (),
	                  translate (false),
	                translation ()
{
	addType (X3DConstants::X3DIndexedFaceSetSelectionObject);

	addChildren (touchSensor,
	             planeSensor,
	             hotSwitch,
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

	select ()               .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_select);
	selectionType ()        .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_selectionType);
	getCoord ()             .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_coord);
	selectAll ()            .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_selectAll_);
	deselectAll ()          .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_deselectAll_);
	replaceSelection ()     .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_replaceSelection_);
	addSelection ()         .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_addSelection_);
	removeSelection ()      .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_removeSelection_);
	replaceSelectedEdges () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_replaceSelectedEdges_);
	addSelectedEdges ()     .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_addSelectedEdges_);
	removeSelectedEdges ()  .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_removeSelectedEdges_);
	replaceSelectedFaces () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_replaceSelectedFaces_);
	addSelectedFaces ()     .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_addSelectedFaces_);
	removeSelectedFaces ()  .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_removeSelectedFaces_);

	selection -> geometry () = getNode <IndexedFaceSet> ();
	selection -> setup ();

	//set_select ();
	set_selectionType ();
	set_loadState ();
	set_coord ();
}

void
X3DIndexedFaceSetSelectionObject::setTranslate (const bool value)
{
	translate = value;

	set_coord_point ();
}

void
X3DIndexedFaceSetSelectionObject::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   hotFaceGeometry    = inlineNode -> getExportedNode <IndexedFaceSet> ("HotFaceGeometry");
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		touchSensor           = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");
		planeSensor           = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		hotSwitch             = inlineNode -> getExportedNode <Switch>           ("HotSwitch");
		hotPointCoord         = inlineNode -> getExportedNode <CoordinateDouble> ("HotPointCoord");
		hotEdgesGeometry      = inlineNode -> getExportedNode <IndexedLineSet>   ("HotEdgesGeometry");
		activePointCoord      = inlineNode -> getExportedNode <CoordinateDouble> ("ActivePointCoord");
		activeEdgesGeometry   = inlineNode -> getExportedNode <IndexedLineSet>   ("ActiveEdgesGeometry");
		selectionCoord        = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");
		selectedEdgesGeometry = inlineNode -> getExportedNode <IndexedLineSet>   ("SelectedEdgesGeometry");
		selectedFacesGeometry = inlineNode -> getExportedNode <IndexedFaceSet>   ("SelectedFacesGeometry");

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

		set_select ();
		set_coord_point ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetSelectionObject::set_select ()
{
	try
	{
		if (select ())
			hotSwitch -> whichChoice () = false;
	}
	catch (const X3DError &)
	{ }
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

		type = selectionTypes .at (selectionType ());
	}
	catch (const std::out_of_range &)
	{
	   type = SelectionType::POINTS;
	}

	//select ({ }, true);
}

void
X3DIndexedFaceSetSelectionObject::set_selectAll_ ()
{
	auto       vertices = getPolygonVertices ();
	const auto last     = std::unique (vertices .begin (), vertices .end ());

	vertices .erase (last, vertices .end ());

	set_selection (vertices, SelectActionType::REPLACE);
}

void
X3DIndexedFaceSetSelectionObject::set_deselectAll_ ()
{
	select ({ }, SelectActionType::REPLACE);
}

void
X3DIndexedFaceSetSelectionObject::set_replaceSelection_ ()
{
	select (std::vector <int32_t> (replaceSelection () .begin (), replaceSelection () .end ()), SelectActionType::REPLACE);
}

void
X3DIndexedFaceSetSelectionObject::set_addSelection_ ()
{
	select (std::vector <int32_t> (addSelection () .begin (), addSelection () .end ()), SelectActionType::ADD);
}

void
X3DIndexedFaceSetSelectionObject::set_removeSelection_ ()
{
	select (std::vector <int32_t> (removeSelection () .begin (), removeSelection () .end ()), SelectActionType::REMOVE);
}

void
X3DIndexedFaceSetSelectionObject::set_replaceSelectedEdges_ ()
{
	selectEdges (std::vector <int32_t> (replaceSelectedEdges () .begin (), replaceSelectedEdges () .end ()), SelectActionType::REPLACE);
	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_addSelectedEdges_ ()
{
	selectEdges (std::vector <int32_t> (addSelectedEdges () .begin (), addSelectedEdges () .end ()), SelectActionType::ADD);
	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_removeSelectedEdges_ ()
{
	selectEdges (std::vector <int32_t> (removeSelectedEdges () .begin (), removeSelectedEdges () .end ()), SelectActionType::REMOVE);
	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_replaceSelectedFaces_ ()
{
	selectFaces (std::vector <int32_t> (replaceSelectedFaces () .begin (), replaceSelectedFaces () .end ()), SelectActionType::REPLACE);
	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_addSelectedFaces_ ()
{
	selectFaces (std::vector <int32_t> (addSelectedFaces () .begin (), addSelectedFaces () .end ()), SelectActionType::ADD);
	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_removeSelectedFaces_ ()
{
	selectFaces (std::vector <int32_t> (removeSelectedFaces () .begin (), removeSelectedFaces () .end ()), SelectActionType::REMOVE);
	updateGeometries ();
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

		if (getTranslate ())
			return;

		for (auto & selectedPoint : selectedPoints)
			selectedPoint .second = getCoord () -> get1Point (selectedPoint .first);
	}
	catch (const X3DError & error)
	{ }
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint ()
{
	if (not select () and not paintSelection ())
	   return;

	if (getTranslate ())
		return;
	else if (not paintSelection () and touchSensor -> isActive ())
      return;

	if (not setMagicSelection ())
	   return;

	if (paintSelection () and touchSensor -> isActive ())
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
	if (not select () and not paintSelection ())
	   return;

	if (abs (translation))
		return;

	switch (getSelectionType ())
	{
		case SelectionType::POINTS:
			selectPoints (activePoints, getSelectActionType ());
			break;

		case SelectionType::EDGES:
			selectEdge (activeEdge, getSelectActionType ());
			break;

		case SelectionType::FACES:
		{
			std::vector <int32_t> points;

			for (const auto & face : activeFaces)
			{
				for (const auto & vertex : getFaceSelection () -> getFaceVertices (face))
					points .emplace_back (coordIndex () [vertex]);
			}

			selectFaces (points, getSelectActionType ());
			break;
		}
	}

	updateGeometries ();
}

void
X3DIndexedFaceSetSelectionObject::set_selection (const std::vector <Vector3d> & hitPoints)
{
	if (getTranslate ())
		return;

	set_selection (hitPoints, getSelectActionType ());
}

void
X3DIndexedFaceSetSelectionObject::set_selection (const std::vector <Vector3d> & hitPoints, const SelectActionType selectAction)
{
	std::vector <int32_t> points;

	for (const auto & hitPoint : hitPoints)
	{
		const auto coincidentPoints = selection -> getCoincidentPoints (hitPoint);
		points .insert (points .end (), coincidentPoints .begin (), coincidentPoints .end ());
	}

	select (points, selectAction);
}

void
X3DIndexedFaceSetSelectionObject::set_plane_sensor_active ()
{
	if (not select ())
	   return;

	hotSwitch -> whichChoice () = planeSensor -> isActive ();

	if (planeSensor -> isActive ())
		return;

	set_coord_point ();
}

///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetSelectionObject::setMagicSelection ()
{
	// Find closest points.

	const auto coincidentPoints = getFaceSelection () -> getCoincidentPoints (touchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return false;
			
	const auto & hitPoint = touchSensor -> getHitPoint ();
	auto         index    = coincidentPoints [0];
	const auto   faces    = getFaceSelection () -> getAdjacentFaces (coincidentPoints);
	auto         face     = getFaceSelection () -> getClosestFace (hitPoint, faces) .index;

	const auto point    = getCoord () -> get1Point (index);
	const auto vertices = getFaceSelection () -> getFaceVertices (face);

	hotFace = face;
	hotFaces  .clear ();
	hotPoints .clear ();

	activeFace = face;
	activeEdge   .clear ();
	activePoints .clear ();

	if (vertices .size () >= 3)
	{
		const auto edge          = getFaceSelection () -> getClosestEdge (hitPoint, vertices);
		const auto edgeDistance  = getDistance (hitPoint, edge .segment .line () .closest_point (hitPoint));
		const auto pointDistance = getDistance (hitPoint, point);

		// Hot edge and points for near point or face

		hotEdge = { edge .index0, edge .index1 };
	
		if (pointDistance > SELECTION_DISTANCE)
		{
			if (edgeDistance > SELECTION_DISTANCE)
			{
				// Face
				for (const auto & vertex : vertices)
					hotPoints .emplace_back (coordIndex () [vertex]);

				hotFaces = { face };
			}
			else
			{
				// Edge
				hotPoints = { coordIndex () [edge .index0], coordIndex () [edge .index1] };
				hotFaces  = getFaceSelection () -> getAdjacentFaces (edge); 
			}
		}
		else
		{
			// Point
			hotPoints = { index };

			for (const auto face : faces)
				hotFaces .emplace_back (face .index); 
		}

		// Active points for near point or face
	
		activePoints = hotPoints;
		activeEdge   = hotEdge;
		activeFaces  = hotFaces;
	}

	updateMagicSelection ();
	return true;
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
	try
	{
		size_t i = 0;

		for (const auto & hotPoint : hotPoints)
			hotPointCoord -> point () .set1Value (i ++, getCoord () -> get1Point (hotPoint));
		
		hotPointCoord -> point () .resize (i);
	}
	catch (const X3DError &)
	{ }

	try
	{
		size_t i = 0;

		for (const auto & activePoint : activePoints)
			activePointCoord -> point () .set1Value (i ++, getCoord () -> get1Point (activePoint));
		
		activePointCoord -> point () .resize (i);
	}
	catch (const X3DError &)
	{ }
}

///  Update hot and active face geometry.
void
X3DIndexedFaceSetSelectionObject::updateMagicFace ()
{
	try
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
	catch (const X3DError &)
	{ }

	try
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
	catch (const X3DError &)
	{ }
}

///  Clear selection.
void
X3DIndexedFaceSetSelectionObject::clearSelection ()
{
	selectedPoints .clear ();
	selectedEdges  .clear ();
	selectedHoles  .clear ();
	selectedFaces  .clear ();
}

///  Select points, edges or faces depending on selection type.
void
X3DIndexedFaceSetSelectionObject::select (const std::vector <int32_t> & points, const SelectActionType selectAction)
{
	switch (getSelectionType ())
	{
		case SelectionType::POINTS:
			selectPoints (points, selectAction);
			break;

		case SelectionType::EDGES:
			selectEdges (points, selectAction);
			break;

		case SelectionType::FACES:
			selectFaces  (points, selectAction);
			break;
	}

	updateGeometries ();
}

///  Select points.
void
X3DIndexedFaceSetSelectionObject::selectPoints (const std::vector <int32_t> & points, const SelectActionType selectAction)
{
	switch (selectAction)
	{
		case SelectActionType::REPLACE:
		{
			clearSelection ();

		   // Proceed with next step:
		}
		case SelectActionType::ADD:
		{
			addSelectedPointsFunction (points);
			break;
		}
		case SelectActionType::REMOVE:
		{
			removeSelectedPointsFunction (points);
			break;
		}
	}
}

///  Select edges.
void
X3DIndexedFaceSetSelectionObject::selectEdges (const std::vector <int32_t> & points, const SelectActionType selectAction)
{
	switch (selectAction)
	{
		case SelectActionType::REPLACE:
		{
			clearSelection ();

		   // Proceed with next step:
		}
		case SelectActionType::ADD:
		{
			const std::set <int32_t> pointIndex (points .begin (), points .end ());

			for (const auto & point : points)
			{
				for (const auto & face : selection -> getAdjacentFaces (point))
				{
					const auto vertices = selection -> getFaceVertices (face .index);

					for (size_t i = 0, size = vertices .size (); i < size; ++ i)
					{
						const auto i0 = vertices [i];
						const auto i1 = vertices [(i + 1) % size];

						if (pointIndex .count (coordIndex () [i0]) and pointIndex .count (coordIndex () [i1]))
							addSelectedEdgesFunction ({ i0, i1 });
					}
				}
			}

			break;
		}
		case SelectActionType::REMOVE:
		{
			const std::set <int32_t> pointIndex (points .begin (), points .end ());

			for (const auto & point : points)
			{
				for (const auto & face : selection -> getAdjacentFaces (point))
				{
					const auto vertices = selection -> getFaceVertices (face .index);

					for (size_t i = 0, size = vertices .size (); i < size; ++ i)
					{
						const auto i0 = vertices [i];
						const auto i1 = vertices [(i + 1) % size];

						if (pointIndex .count (coordIndex () [i0]) and pointIndex .count (coordIndex () [i1]))
							removeSelectedEdgesFunction ({ i0, i1 });
					}
				}
			}

			break;
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

	selectHoles ();
}

///  Select one edge.
void
X3DIndexedFaceSetSelectionObject::selectEdge (const std::vector <size_t> & edge, const SelectActionType selectAction)
{
	switch (selectAction)
	{
		case SelectActionType::REPLACE:
		{
			clearSelection ();

		   // Proceed with next step:
		}
		case SelectActionType::ADD:
		{
			addSelectedEdgesFunction (edge);
			break;
		}
		case SelectActionType::REMOVE:
		{
			removeSelectedEdgesFunction (edge);
			break;
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

	selectHoles ();
}

///  Select line loops from selected edges.
void
X3DIndexedFaceSetSelectionObject::selectHoles ()
{
	std::set <std::pair <int32_t, int32_t>>               edges;
	std::multimap <int32_t, std::pair <int32_t, int32_t>> edgeIndex;

	for (const auto & edge : getSelectedEdges ())
	{
		if (edge .second .size () not_eq 1)
			continue;
		
		const auto & points1 = edge .first;
		const auto   points2 = std::make_pair (points1 .second, points1 .first);
		
		edges .emplace (points1);
		edges .emplace (points2);
		
		edgeIndex .emplace (points1 .first, points1);
		edgeIndex .emplace (points2 .first, points2);
	}

	selectedHoles .clear ();

	if (not edges .empty ())
		selectHoles (edges, edgeIndex, selectedHoles);

	selectedHoles_changed () = selectedHoles .size ();
}

void
X3DIndexedFaceSetSelectionObject::selectHoles (const std::set <std::pair <int32_t, int32_t>> & edges,
                                               const std::multimap <int32_t, std::pair <int32_t, int32_t>> & edgeIndex,
                                               std::vector <std::vector <int32_t>> & holes) const
{
	//__LOG__ << std::endl;
	//__LOG__ << "edges * 2: " << edges .size () << std::endl;

	std::vector <int32_t> hole;
	std::set <int32_t>    currentPoints;

	for (const auto & edge : edges)
	{
		const auto range = edgeIndex .equal_range (edge .first);

		if (std::distance (range .first, range .second) < 4)
		   continue;

		//__LOG__ << "distance: " << std::distance (range .first, range .second) << std::endl;
	
		// Test if edge is already in line loops.

		if (not currentPoints .emplace (edge .first) .second)
			continue;

		for (const auto & e : range)
		{
			hole = { edge .first };
			selectHole (currentPoints, edgeIndex, e .second, hole, holes);
		}
	}

	for (const auto & edge : edges)
	{
		// Test if edge is already in line loops.

		if (not currentPoints .emplace (edge .first) .second)
			continue;

		hole = { edge .first };

		selectHole (currentPoints, edgeIndex, edge, hole, holes);
	}

	//std::clog << std::endl;
	//for (const auto & hole : holes)
	//{
	//	for (const auto & h : hole)
	//		std::clog << h << " ";
	//	std::clog << std::endl;
	//}
}

void
X3DIndexedFaceSetSelectionObject::selectHole (std::set <int32_t> & currentPoints,
                                              const std::multimap <int32_t, std::pair <int32_t, int32_t>> & edgeIndex,
	                                           const std::pair <int32_t, int32_t> & current,
	                                           std::vector <int32_t> & hole,
	                                           std::vector <std::vector <int32_t>> & holes) const
{
	const auto last = current .second;

	if (currentPoints .emplace (last) .second)
	{
		hole .emplace_back (last);

		for (const auto & edge : edgeIndex .equal_range (last))
		{
			if (edge .second == std::make_pair (current .second, current .first))
				continue;
			
			selectHole (currentPoints, edgeIndex, edge .second, hole, holes);
		}

		hole .pop_back ();
	}
	else
	{
//for (const auto & h : hole)
//	std::clog << h << " ";
//std::clog << ": " << last << std::endl;

		const auto iter = std::find (hole .begin (), hole .end (), last);

		if (iter not_eq hole .end ())
		{
			if (hole .end () - iter >= 3)
				holes .emplace_back (iter, hole .end ());
		}
	}
}

///  Select faces.
void
X3DIndexedFaceSetSelectionObject::selectFaces (const std::vector <int32_t> & points, const SelectActionType selectAction)
{
	switch (selectAction)
	{
		case SelectActionType::REPLACE:
		{
			clearSelection ();

		   // Proceed with next step:
		}
		case SelectActionType::ADD:
		{
			std::set <size_t> faces;

			for (const auto & point : points)
			{
				for (const auto & face : selection -> getAdjacentFaces (point))
					faces .emplace (face .index);
			}

			addSelectedFacesFunction (faces, std::set <int32_t> (points .begin (), points .end ()));
			break;
		}
		case SelectActionType::REMOVE:
		{
			std::set <size_t> faces;

			for (const auto & point : points)
			{
				for (const auto & face : selection -> getAdjacentFaces (point))
					faces .emplace (face .index);

			}
	
			removeSelectedFacesFunction (faces, std::set <int32_t> (points .begin (), points .end ()));
			break;
		}
	}

	selectedPoints .clear ();

	for (const auto & face : selectedFaces)
		for (const auto & vertex : selection -> getFaceVertices (face))
		   selectedPoints .emplace (coordIndex () [vertex], getCoord () -> get1Point (coordIndex () [vertex]));
}

///  Add @a points to selection of points.
void
X3DIndexedFaceSetSelectionObject::addSelectedPointsFunction (const std::vector <int32_t> & points)
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
X3DIndexedFaceSetSelectionObject::removeSelectedPointsFunction (const std::vector <int32_t> & points)
{
	masterPoint = -1;

	for (const auto & point : points)
		selectedPoints .erase (point);
}

///  Update selected points geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedPoints ()
{
	try
	{
		size_t i = 0;

		for (auto & selectedPoint : selectedPoints)
			selectionCoord -> point () .set1Value (i ++, getCoord () -> get1Point (selectedPoint .first));

		selectionCoord -> point () .resize (i);

		selectedPoints_changed () = i;
	}
	catch (const X3DError &)
	{ }
}

///  Determine edges from @a vertices and add them to selected edges.
void
X3DIndexedFaceSetSelectionObject::addSelectedEdgesFunction (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = vertices [i];
		auto i1 = vertices [(i + 1) % size];

		auto index0 = coordIndex () [i0] .getValue ();
		auto index1 = coordIndex () [i1] .getValue ();

		if (i0 > i1)
			std::swap (i0, i1);

		if (index0 > index1)
			std::swap (index0, index1);

		selectedEdges [std::make_pair (index0, index1)] .emplace (std::make_pair (i0, i1)); 
	}
}

///  Determine edges from @a vertices and remove them from selected edges.
void
X3DIndexedFaceSetSelectionObject::removeSelectedEdgesFunction (const std::vector <size_t> & vertices)
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto i0 = vertices [i];
		auto i1 = vertices [(i + 1) % size];

		auto index0 = coordIndex () [i0] .getValue ();
		auto index1 = coordIndex () [i1] .getValue ();

		if (i0 > i1)
			std::swap (i0, i1);

		if (index0 > index1)
			std::swap (index0, index1);

		selectedEdges [std::make_pair (index0, index1)] .erase (std::make_pair (i0, i1)); 
	}
}

///  Update selected edges geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedEdges ()
{
	try
	{
		size_t i = 0;

		for (const auto & edge : selectedEdges)
		{
			if ((getSelectionType () == SelectionType::FACES and edge .second .size () not_eq 1) or edge .second .empty ())
				continue;

			selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .first);
			selectedEdgesGeometry -> coordIndex () .set1Value (i ++, edge .first .second);
			selectedEdgesGeometry -> coordIndex () .set1Value (i ++, -1);
		}

		selectedEdgesGeometry -> coordIndex () .resize (i);

		selectedEdges_changed () = i / 3;
		selectedHoles_changed () = selectedHoles .size ();
	}
	catch (const X3DError &)
	{ }
}

///  Add @a faces to selection of faces.
void
X3DIndexedFaceSetSelectionObject::addSelectedFacesFunction (const std::set <size_t> & faces, const std::set <int32_t> & points)
{
	for (const auto & face : faces)
	{
		const auto vertices = selection -> getFaceVertices (face);

		if (isInSelection (points, vertices))
		{
			addSelectedEdgesFunction (vertices);
			selectedFaces .emplace (face);
		}
	}
}

///  Remove @a faces from selection of faces.
void
X3DIndexedFaceSetSelectionObject::removeSelectedFacesFunction (const std::set <size_t> & faces, const std::set <int32_t> & points)
{
	for (const auto & face : faces)
	{
		const auto vertices = selection -> getFaceVertices (face);

		if (isInSelection (points, vertices))
		{
			removeSelectedEdgesFunction (vertices);
			selectedFaces .erase (face);
		}
	}
}

///  Update selected faces geometry.
void
X3DIndexedFaceSetSelectionObject::updateSelectedFaces ()
{
	try
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
	catch (const X3DError &)
	{ }
}

///  Update selection geometries.
void
X3DIndexedFaceSetSelectionObject::updateGeometries ()
{
	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
	updateMagicPoints ();
}

X3DIndexedFaceSetSelectionObject::SelectActionType
X3DIndexedFaceSetSelectionObject::getSelectActionType () const
{
	if (getBrowser () -> getControlKey ())
		return SelectActionType::REMOVE;

	const bool replace = not paintSelection () and not getBrowser () -> getShiftKey () and not getBrowser () -> getControlKey ();

	if (replace)
		return SelectActionType::REPLACE;
	
	return SelectActionType::ADD;
}

///  Returns true if all points destribed by @a vertices are in the set of selected of points, otherwise false. 
bool
X3DIndexedFaceSetSelectionObject::isInSelection (const std::set <int32_t> & points, const std::vector <size_t> & vertices) const
{
	for (const auto & vertex : vertices)
	{
		if (points .count (coordIndex () [vertex]))
			continue;

		return false;
	}

	return true;
}

///  Returns the sceen distance in pixels between @a point1 and @a point2.
double
X3DIndexedFaceSetSelectionObject::getDistance (const Vector3d & point1, const Vector3d & point2)
{
	try
	{
		const auto p1 = ViewVolume::projectPoint (point1, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
		const auto p2 = ViewVolume::projectPoint (point2, getModelViewMatrix (), getProjectionMatrix (), getViewport ());

		return abs (Vector2d (p1. x (), p1 .y ()) - Vector2d (p2. x (), p2 .y ()));
	}
	catch (const std::domain_error &)
	{
	   return 0;
	}
}

///  Returns the sceen area in pixels of @a polygon.
double
X3DIndexedFaceSetSelectionObject::getArea (const std::vector <size_t> & vertices)
{
	try
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
	catch (const std::domain_error &)
	{
	   return 0;
	}
}

void
X3DIndexedFaceSetSelectionObject::undoRestoreSelection (const UndoStepPtr & undoStep)
{
	std::vector <int32_t> points;

	for (const auto & selectedPoint : getSelectedPoints ())
		points .emplace_back (selectedPoint .first);

	undoStep -> addUndoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelection, SFNode (this), points);
}

void
X3DIndexedFaceSetSelectionObject::redoRestoreSelection (const std::vector <int32_t> & points, const UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelection, SFNode (this), points);
}

void
X3DIndexedFaceSetSelectionObject::redoRestoreSelectedEdges (const std::vector <int32_t> & points, const UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelectedEdges, SFNode (this), points);
}

void
X3DIndexedFaceSetSelectionObject::redoRestoreSelectedFaces (const std::vector <int32_t> & points, const UndoStepPtr & undoStep)
{
	undoStep -> addRedoFunction (&X3DIndexedFaceSetSelectionObject::restoreSelectedFaces, SFNode (this), points);
}

void
X3DIndexedFaceSetSelectionObject::restoreSelection (const SFNode & node, const std::vector <int32_t> & points)
{
	X3DPtr <X3DIndexedFaceSetSelectionObject> tool (node);

	if (tool)
		tool -> replaceSelection () .assign (points .begin (), points .end ());
}

void
X3DIndexedFaceSetSelectionObject::restoreSelectedEdges (const SFNode & node, const std::vector <int32_t> & points)
{
	X3DPtr <X3DIndexedFaceSetSelectionObject> tool (node);

	if (tool)
		tool -> replaceSelectedEdges () .assign (points .begin (), points .end ());
}

void
X3DIndexedFaceSetSelectionObject::restoreSelectedFaces (const SFNode & node, const std::vector <int32_t> & points)
{
	X3DPtr <X3DIndexedFaceSetSelectionObject> tool (node);

	if (tool)
		tool -> replaceSelectedFaces () .assign (points .begin (), points .end ());
}

X3DIndexedFaceSetSelectionObject::~X3DIndexedFaceSetSelectionObject ()
{ }

} // X3D
} // titania
