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

#include "X3DIndexedFaceSetKnifeObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/Geometry2D/Arc2D.h"
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Editing/Selection/FaceSelection.h"
#include "../../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8; // in pixel

X3DIndexedFaceSetKnifeObject::Fields::Fields () :
	 cutPolygons (new SFBool ())
{ }

X3DIndexedFaceSetKnifeObject::X3DIndexedFaceSetKnifeObject () :
	            IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetCutObject (),
	                    fields (),
	       knifeSelectionGroup (),
	          knifeTouchSensor (),
	              planeSensors (),
	               knifeSwitch (),
	           knifeStartPoint (),
	             knifeEndPoint (),
	           knifeLineSwitch (),
	       knifeLineCoordinate (),
	            knifeArcSwitch (),
	                  knifeArc (),
	          knifeArcGeometry (),
	                  cutFaces (),
	              cutFaceIndex (),
	                   cutFace (0),
	                   cutEdge (),
	                 cutPoints (),
	               startPoints (),
	                 endPoints (),
	                 startEdge (),
	                   endEdge (),
	                    active (false)
{
	//addType (X3DConstants::X3DIndexedFaceSetKnifeObject);

	addChildren (knifeSelectionGroup,
	             knifeTouchSensor,
	             planeSensors,
	             knifeSwitch,
	             knifeStartPoint,
	             knifeEndPoint,
	             knifeLineSwitch,
	             knifeLineCoordinate,
	             knifeArcSwitch,
	             knifeArc,
	             knifeArcGeometry);
}

void
X3DIndexedFaceSetKnifeObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_loadState);

	cutPolygons () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_cutPolygons);
}

void
X3DIndexedFaceSetKnifeObject::set_loadState ()
{
	try
	{
		const auto & inlineNode         = getCoordinateTool () -> getInlineNode ();
		const auto   activeFaceGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("ActiveFaceGeometry");

		knifeSelectionGroup = inlineNode -> getExportedNode <Group>            ("KnifeSelectionGroup");
		knifeTouchSensor    = inlineNode -> getExportedNode <TouchSensor>      ("KnifeTouchSensor");
		knifeSwitch         = inlineNode -> getExportedNode <Switch>           ("KnifeSwitch");
		knifeStartPoint     = inlineNode -> getExportedNode <Transform>        ("KnifeStartPoint");
		knifeEndPoint       = inlineNode -> getExportedNode <Transform>        ("KnifeEndPoint");
		knifeLineSwitch     = inlineNode -> getExportedNode <Switch>           ("KnifeLineSwitch");
		knifeLineCoordinate = inlineNode -> getExportedNode <CoordinateDouble> ("KnifeLineCoordinate");
		knifeArcSwitch      = inlineNode -> getExportedNode <Switch>           ("KnifeArcSwitch");
		knifeArc            = inlineNode -> getExportedNode <Transform>        ("KnifeArc");
		knifeArcGeometry    = inlineNode -> getExportedNode <Arc2D>            ("KnifeArcGeometry");

		knifeTouchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_touch_sensor_hitPoint);
		knifeTouchSensor -> isActive ()         .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_touch_sensor_active);

		set_cutPolygons ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetKnifeObject::set_cutPolygons ()
{
	try
	{
		if (cutPolygons ())
		{
			select () = false;
			getHotSwitch () -> whichChoice () = true;
		}
		else
			select () = true;

		knifeSwitch -> whichChoice () = cutPolygons ();
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetKnifeObject::set_touch_sensor_hitPoint  ()
{
	// Set hot points.

	if (active)
		return;

	if (not setStartMagicSelection ())
	   return;

   // Set start point

   if (getHotEdges () .empty ())
      return;

	// Determine first cut point.

	if (getHotPoints () .size () == 1)
	{
		// Snap to vertex.
		cutPoints .first = getCoord () -> get1Point (getHotPoints () .front ());

		snapToVertex (cutFace, startPoints, cutPoints .first);
	}
	else
	{
		const auto point1       = getCoord () -> get1Point (coordIndex () [startEdge .first]);
		const auto point2       = getCoord () -> get1Point (coordIndex () [startEdge .second]);
		const auto edgeLine     = Line3d (point1, point2, math::points_type ());
		const auto closestPoint = edgeLine .closest_point (knifeTouchSensor -> getHitPoint ());

		cutPoints .first = closestPoint;

		if (snapToVertex (cutFace, startPoints, cutPoints .first))
			;
		else
		{
			if (snapToCenter (startEdge, cutPoints .first))
				;
			else
				knifeArcSwitch -> whichChoice () = false;
		}
	}  

	// Configure plane sensors.

	size_t i = 0;

	for (const size_t size = std::min (cutFaces .size (), planeSensors .size ()); i < size; ++ i)
		set_plane_sensor (planeSensors [i], cutFaces [i]);
	
	if (cutFaces .size () > planeSensors .size ())
	{
		const auto & scene = getCoordinateTool () -> getInlineNode () -> getInternalScene ();

		for (const size_t size = cutFaces .size (); i < size; ++ i)
		{
			const auto planeSensor = scene -> createNode <PlaneSensor> ();

			planeSensors .emplace_back (planeSensor);
			knifeSelectionGroup -> children () .emplace_back (planeSensor);

			set_plane_sensor (planeSensor, cutFace);
		}
	}
	else
	{
	   for (const size_t size = planeSensors .size (); i < size; ++ i)
	      planeSensors [i] -> enabled () = false;
	}

	// Set up visual cut ray.

	cutEdge                             = cutPoints;
	cutPoints .second                   = cutPoints .first;
	knifeLineCoordinate -> point () [0] = cutPoints .first;
	knifeLineCoordinate -> point () [1] = cutPoints .first;
	knifeStartPoint -> translation ()   = cutPoints .first;
	knifeEndPoint -> translation ()     = knifeTouchSensor -> getHitPoint ();
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor (const X3DPtr <PlaneSensor> & planeSensor, size_t cutFace)
{
	const auto normal       = getPolygonNormal (getFaceSelection () -> getFaceVertices (cutFace));		               
	const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

	planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_plane_sensor_translation, planeSensor .getValue ());

	planeSensor -> enabled ()      = true;
	planeSensor -> axisRotation () = axisRotation;
	planeSensor -> autoOffset ()   = false;
	planeSensor -> offset ()       = knifeTouchSensor -> getHitPoint ();
}

void
X3DIndexedFaceSetKnifeObject::set_touch_sensor_active ()
{
	active = knifeTouchSensor -> isActive ();	 
  
	knifeLineSwitch -> whichChoice () = active;	 
  
	if (active)
		return;

	knifeArcSwitch -> whichChoice () = false;

	// Cut

	const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Cut Polygons"));

	undoRestoreSelection (undoStep);
	undoSetColorIndex    (undoStep);
	undoSetTexCoordIndex (undoStep);
	undoSetNormalIndex   (undoStep);
	undoSetCoordIndex    (undoStep);
	undoSetColorColor    (undoStep);
	undoSetTexCoordPoint (undoStep);
	undoSetNormalVector  (undoStep);
	undoSetCoordPoint    (undoStep);

	const auto selection = cut (cutFace, cutEdge, startPoints, endPoints);

	startPoints .clear ();
	endPoints   .clear ();

	if (selection .empty ())
		return;

	replaceSelectedEdges () .assign (selection .begin (), selection .end ());

	// Remove degenerated edges and faces.
	rebuildIndices  ();
	rebuildColor    ();
	rebuildTexCoord ();
	rebuildNormal   ();
	rebuildCoord    ();

	redoSetCoordPoint    (undoStep);
	redoSetNormalVector  (undoStep);
	redoSetTexCoordPoint (undoStep);
	redoSetColorColor    (undoStep);
	redoSetCoordIndex    (undoStep);
	redoSetNormalIndex   (undoStep);
	redoSetTexCoordIndex (undoStep);
	redoSetColorIndex    (undoStep);
	redoRestoreSelection ({ }, undoStep);

	undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor_translation (PlaneSensor* const planeSensor)
{
	if (not setEndMagicSelection (planeSensor))
	   return;

	Vector3d closestPoint;

 	if (getHotPoints () .size () == 1)
	{
		// Snap to vertex.
		cutPoints .second = getCoord () -> get1Point (getHotPoints () .front ());
		closestPoint      = cutPoints .second;

		snapToVertex (cutFace, endPoints, closestPoint);
	}
	else
	{
		cutPoints .second = planeSensors [cutFaceIndex] -> translation_changed () .getValue ();

		const auto point1     = getCoord () -> get1Point (coordIndex () [endEdge .first]);
		const auto point2     = getCoord () -> get1Point (coordIndex () [endEdge .second]);
		const auto edgeLine   = Line3d (point1, point2, math::points_type ());
		const auto orthoPoint = edgeLine .closest_point (cutPoints .first);

		closestPoint = getClosestPoint (endEdge, cutPoints);	                   

		if (snapToVertex (cutFace, endPoints, closestPoint))
			;
		else
		{
			if (getDistance (orthoPoint, closestPoint) <= SELECTION_DISTANCE)
			{
			   // Snap to ortho point of edge.
				closestPoint = orthoPoint;

				knifeArcSwitch   -> whichChoice () = true;
				knifeArc         -> translation () = orthoPoint;
				knifeArcGeometry -> startAngle ()  = 0;
				knifeArcGeometry -> endAngle ()    = M_PI / 2;
			}
			else
			{
				if (snapToCenter (endEdge, closestPoint))
					;
				else
					knifeArcSwitch -> whichChoice () = false;
			}
		}
	}

	knifeLineCoordinate -> point () [1] = cutPoints .second;
	knifeEndPoint -> translation ()     = closestPoint;
	cutEdge .second                     = closestPoint;

	// Ugly fallback if the track point lies behind the screen.

	const auto trackPoint = Vector3d (planeSensors [cutFaceIndex] -> trackPoint_changed () .getValue ()) * getModelViewMatrix ();

	if (trackPoint .z () > 0)
		knifeLineCoordinate -> point () [1] = getBrowser () -> getHitRay (getModelViewMatrix (), getProjectionMatrix (), getViewport ()) .point ();
}

Vector3d
X3DIndexedFaceSetKnifeObject::getClosestPoint (const std::pair <size_t, size_t> & edge,
                                               const std::pair <Vector3d, Vector3d> & cutPoints) const
{
	try
	{
		Vector3d closestPoint;
	
		const auto point1     = getCoord () -> get1Point (coordIndex () [edge .first]);
		const auto point2     = getCoord () -> get1Point (coordIndex () [edge .second]);
		const auto edgeLine   = Line3d (point1, point2, math::points_type ());	                   
		const auto cutRay     = Line3d (cutPoints .first, cutPoints .second, math::points_type ());
		const auto edgeScreen = ViewVolume::projectLine (edgeLine, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
		const auto cutScreen  = ViewVolume::projectLine (cutRay,   getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	
		// Determine closeset point.
		edgeScreen .closest_point (cutScreen, closestPoint);
		const auto hitRay = ViewVolume::unProjectLine (closestPoint .x (), closestPoint .y (), getModelViewMatrix (), getProjectionMatrix (), getViewport ());
		edgeLine .closest_point (hitRay, closestPoint);
	
		return closestPoint;
	}
	catch (const std::domain_error &)
	{
		return Vector3d ();
	}
}

///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetKnifeObject::setStartMagicSelection ()
{
	// Find closest points.

	const auto coincidentPoints = getFaceSelection () -> getCoincidentPoints (knifeTouchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return false;
			
	const auto  hitPoint = knifeTouchSensor -> getHitPoint ();
	const auto  index    = coincidentPoints [0];
	const auto  faces    = getFaceSelection () -> getAdjacentFaces (coincidentPoints);
	const auto  face     = getFaceSelection () -> getClosestFace (hitPoint, faces) .index;
	const auto  vertices = getFaceSelection () -> getFaceVertices (face);

	setHotPoints ({ });
	setHotEdges ({ });
	setHotFaces ({ });

	cutFace = face;

	if (vertices .size () >= 3)
	{
		const auto point         = getCoord () -> get1Point (index);
		const auto edge          = getFaceSelection () -> getClosestEdge (hitPoint, vertices);
		const auto pointDistance = getDistance (hitPoint, point);
	
		// Hot edge and points for near point or face

		setHotEdges ({ std::make_pair (edge .index0, edge .index1) });
	
		if (edge .isEdge and pointDistance > SELECTION_DISTANCE)
		{
			// Edge
			setHotPoints ({ coordIndex () [edge .index0], coordIndex () [edge .index1] });

			cutFaces = getFaceSelection () -> getAdjacentFaces (edge);
		}
		else
		{
			// Point
			setHotPoints ({ index });

			// Adjacent faces
			cutFaces .clear ();

			for (const auto face : faces)
				cutFaces .emplace_back (face .index); 
		}

		startPoints = getHotPoints ();
		startEdge   = std::make_pair (edge .index0, edge .index1);
	}

	updateMagicSelection ();
	return true;
}


///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetKnifeObject::setEndMagicSelection (PlaneSensor* const planeSensor)
{
	// Find closest points.

	try
	{
		const auto coincidentPoints = getFaceSelection () -> getCoincidentPoints (knifeTouchSensor -> getClosestPoint ());

		if (coincidentPoints .empty ())
			return false;

		const auto hitPoint = knifeTouchSensor -> isOver ()
		                      ? knifeTouchSensor -> getHitPoint ()
		                      : Vector3d (planeSensor -> translation_changed () .getValue ());

		const auto cutSegment  = LineSegment3d (cutEdge .first, hitPoint);
		const auto closestEdge = getFaceSelection () -> getClosestEdge (cutSegment, cutFaces);
		const auto edge        = closestEdge .first;
		const auto face        = closestEdge .second;
		const auto index       = std::find (cutFaces .begin (), cutFaces .end (), face) - cutFaces .begin ();

		if (planeSensor not_eq planeSensors [index])
		   return false;

		setHotPoints ({ });
	
		cutFace      = face;
		cutFaceIndex = index;

		const auto vertices = getFaceSelection () -> getFaceVertices (cutFace);

		if (vertices .size () >= 3)
		{
			const auto cutVertex     = getFaceSelection () -> getClosestVertex (hitPoint, vertices);
			const auto cutPoint      = coordIndex () [cutVertex];
			const auto point         = getCoord () -> get1Point (cutPoint);
			const auto pointDistance = getDistance (hitPoint, point);

			// Hot edge and points for near point or face

			setHotEdges ({ std::make_pair (edge .index0, edge .index1) });
		
			if (edge .isEdge and pointDistance > SELECTION_DISTANCE)
			{
				// Edge
				setHotPoints ({ coordIndex () [edge .index0], coordIndex () [edge .index1] });
			}
			else
			{
				// Point
				setHotPoints ({ cutPoint });
			}

			endPoints = getHotPoints ();
			endEdge   = std::make_pair (edge .index0, edge .index1);
		}

		updateMagicSelection ();
		return true;
	}
	catch (const std::domain_error &)
	{
	   return false;
	}
}

bool
X3DIndexedFaceSetKnifeObject::snapToCenter (const std::pair <size_t, size_t> & edge, Vector3d & point)
{
	if (not cutSnapping ())
		return false;

	const auto point1   = getCoord () -> get1Point (coordIndex () [edge .first]);
	const auto point2   = getCoord () -> get1Point (coordIndex () [edge .second]);
	const auto center   = (point1 + point2) / 2.0;
	const auto distance = getDistance (center, point);

	// Snap to center of edge.
	if (distance > SELECTION_DISTANCE)
	   return false;

	point = center;

	knifeArcSwitch   -> whichChoice () = true;
	knifeArc         -> translation () = center;
	knifeArcGeometry -> startAngle ()  = M_PI;
	knifeArcGeometry -> endAngle ()    = M_PI * 2;
	return true;
}

bool
X3DIndexedFaceSetKnifeObject::snapToVertex (const size_t face, std::vector <int32_t> & points, Vector3d & vertex)
{
	if (not cutSnapping ())
		return false;

	const auto vertices      = getFaceSelection () -> getFaceVertices (face);
	const auto closestPoint  = coordIndex () [getFaceSelection () -> getClosestVertex (vertex, vertices)];
	const auto closestVertex = getCoord () -> get1Point (closestPoint);

	if (getDistance (closestVertex, vertex) > SELECTION_DISTANCE)
	   return false;

	setHotPoints (points = { closestPoint });
	vertex = closestVertex;

   // Snap to vertex
	knifeArcSwitch   -> whichChoice () = true;
	knifeArc         -> translation () = closestVertex;
	knifeArcGeometry -> startAngle ()  = 0;
	knifeArcGeometry -> endAngle ()    = 0;
	return true;
}

X3DIndexedFaceSetKnifeObject::~X3DIndexedFaceSetKnifeObject ()
{ }

} // X3D
} // titania
