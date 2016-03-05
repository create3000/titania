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
#include "../../../Components/Grouping/Transform.h"
#include "../../../Components/Grouping/Switch.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../../Components/NURBS/CoordinateDouble.h"
#include "../../../Editing/Selection/FaceSelection.h"

namespace titania {
namespace X3D {

static constexpr double SELECTION_DISTANCE = 8;

X3DIndexedFaceSetKnifeObject::Fields::Fields () :
	 cutPolygons (new SFBool ())
{ }

X3DIndexedFaceSetKnifeObject::X3DIndexedFaceSetKnifeObject () :
	                  IndexedFaceSet (getExecutionContext ()),
	           X3DIndexedFaceSetTool (),
	                          fields (),
	                     touchSensor (),
	                     planeSensor (),
	                 knifeStartPoint (),
	                   knifeEndPoint (),
	                 knifeLineSwitch (),
	             knifeLineCoordinate (),
	                         cutFace (0),
	                         cutEdge (),
	                     startPoints (),
	                       endPoints (),
	                       startEdge (),
	                         endEdge ()
{
	//addType (X3DConstants::X3DIndexedFaceSetKnifeObject);

	addChildren (touchSensor,
	             planeSensor,
	             knifeSwitch,
	             knifeStartPoint,
	             knifeEndPoint,
	             knifeLineSwitch,
	             knifeLineCoordinate);
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

		planeSensor         = inlineNode -> getExportedNode <PlaneSensor>      ("PlaneSensor");
		touchSensor         = inlineNode -> getExportedNode <TouchSensor>      ("TouchSensor");
		knifeSwitch         = inlineNode -> getExportedNode <Switch>           ("KnifeSwitch");
		knifeStartPoint     = inlineNode -> getExportedNode <Transform>        ("KnifeStartPoint");
		knifeEndPoint       = inlineNode -> getExportedNode <Transform>        ("KnifeEndPoint");
		knifeLineSwitch     = inlineNode -> getExportedNode <Switch>           ("KnifeLineSwitch");
		knifeLineCoordinate = inlineNode -> getExportedNode <CoordinateDouble> ("KnifeLineCoordinate");

		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_touch_sensor_hitPoint);

		planeSensor -> isActive ()            .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetKnifeObject::set_plane_sensor_translation);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetKnifeObject::set_cutPolygons ()
{
	__LOG__ <<std::endl;

	if (cutPolygons ())
	{
		select () = false;
		getHotSwitch () -> whichChoice () = true;
	}
	else
		select () = true;

	knifeSwitch -> whichChoice () = cutPolygons ();
}

void
X3DIndexedFaceSetKnifeObject::set_touch_sensor_hitPoint  ()
{
	if (not cutPolygons ())
	   return;

	// Set hot points.

	if (planeSensor -> isActive ())
	   return;

	if (not setStartMagicSelection ())
	   return;

   // Set start point

   if (getHotEdge () .empty ())
      return;

	if (getHotPoints () .size () == 1)
		cutPoints .first = getCoord () -> get1Point (getHotPoints () .front ());

	else
	{
		const Line3d edgeLine (getCoord () -> get1Point (coordIndex () [getHotEdge () .front ()]),
		                       getCoord () -> get1Point (coordIndex () [getHotEdge () .back ()]),
		                       math::point_type ());

		cutPoints .first = edgeLine .closest_point (touchSensor -> getHitPoint ());
	}  

	const auto normal       = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));		               
	const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

	planeSensor -> enabled ()      = true;
	planeSensor -> axisRotation () = axisRotation;
	planeSensor -> offset ()       = touchSensor -> getHitPoint ();
	planeSensor -> maxPosition ()  = Vector2d (-1, -1);

	cutEdge                             = cutPoints;
	cutPoints .second                   = cutPoints .first;
	knifeLineCoordinate -> point () [0] = cutPoints .first;
	knifeLineCoordinate -> point () [1] = cutPoints .first;
	knifeStartPoint -> translation ()   = cutPoints .first;
	knifeEndPoint -> translation ()     = touchSensor -> getHitPoint ();
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor_active ()
{
	if (not cutPolygons ())
	   return;

	knifeLineSwitch -> whichChoice () = planeSensor -> isActive ();	 
  
	if (planeSensor -> isActive ())
		return;

	cut ();
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor_translation ()
{
	if (not cutPolygons ())
	   return;

	if (not setEndMagicSelection ())
	   return;

 	if (getHotPoints () .size () == 1)
		cutPoints .second = getCoord () -> get1Point (getHotPoints () .front ());

	else
		cutPoints .second = planeSensor -> translation_changed () .getValue ();

	const auto edgeLine = Line3d (getCoord () -> get1Point (coordIndex () [getHotEdge () .front ()]), getCoord () -> get1Point (coordIndex () [getHotEdge () .back ()]), math::point_type ());	                   
	const auto cutRay   = Line3d (cutPoints .first, cutPoints .second, math::point_type ());

	const auto edgeScreen = ViewVolume::projectLine (edgeLine, getModelViewMatrix (), getProjectionMatrix (), getViewport ());
	const auto cutScreen  = ViewVolume::projectLine (cutRay,   getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	Vector3d closestPoint;
                     
	edgeScreen .closest_point (cutScreen, closestPoint);

	const auto hitRay = ViewVolume::unProjectLine (closestPoint .x (), closestPoint .y (), getModelViewMatrix (), getProjectionMatrix (), getViewport ());

	edgeLine .closest_point (hitRay, closestPoint);

	knifeLineCoordinate -> point () [1] = cutPoints .second;
	knifeEndPoint -> translation ()     = closestPoint;
	cutEdge .second                     = closestPoint;

	// Ugly fallback if the track point lies behind the screen.

	const auto trackPoint = Vector3d (planeSensor -> trackPoint_changed () .getValue ()) * getModelViewMatrix ();

	if (trackPoint .z () > 0)
		knifeLineCoordinate -> point () [1] = getBrowser () -> getHitRay (getModelViewMatrix (), getProjectionMatrix (), getViewport ()) .point ();
}

///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetKnifeObject::setStartMagicSelection ()
{
	// Find closest points.

	const auto coincidentPoints = getFaceSelection () -> getCoincidentPoints (touchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return false;
			
	const auto  hitPoint = touchSensor -> getHitPoint ();
	auto        index    = coincidentPoints [0];
	const auto  faces    = getFaceSelection () -> getAdjacentFaces (coincidentPoints);
	auto        face     = getFaceSelection () -> getClosestFace (hitPoint, faces) .index;

	cutFace = face;

	setHotFace (face);
	setHotPoints ({ });

	const auto point    = getCoord () -> get1Point (index);
	const auto vertices = getFaceSelection () -> getFaceVertices (face);

	if (vertices .size () >= 3)
	{
		const auto edge          = getFaceSelection () -> getClosestEdge (hitPoint, vertices);
		const auto pointDistance = getDistance (hitPoint, point);

		// Hot edge and points for near point or face

		setHotEdge ({ edge .index0, edge .index1 });
	
		if (edge .isEdge and pointDistance > SELECTION_DISTANCE)
		{
			// Edge
			setHotPoints ({ coordIndex () [edge .index0], coordIndex () [edge .index1] });
		}
		else
		{
			// Point
			setHotPoints ({ index });
		}

		startPoints = getHotPoints ();
		startEdge   = std::make_pair (edge .index0, edge .index1);
	}


	updateMagicSelection ();
	return true;
}

///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetKnifeObject::setEndMagicSelection ()
{
	const auto hitPoint = Vector3d (planeSensor -> translation_changed () .getValue ());
	const auto cutRay   = Line3d (cutPoints .first, cutPoints .second, math::point_type ());

	setHotFace (cutFace);
	setHotPoints ({ });

	const auto vertices = getFaceSelection () -> getFaceVertices (cutFace);

	if (vertices .size () >= 3)
	{
		const auto edge          = getFaceSelection () -> getClosestEdge (cutRay, vertices);
		const auto cutVertex     = getFaceSelection () -> getClosestVertex (hitPoint, vertices);
		const auto cutPoint      = coordIndex () [cutVertex];
		const auto point         = getCoord () -> get1Point (cutPoint);
		const auto pointDistance = getDistance (hitPoint, point);

		// Hot edge and points for near point or face

		setHotEdge ({ edge .index0, edge .index1 });
	
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

bool
X3DIndexedFaceSetKnifeObject::cut ()
{
	const auto vertices     = getFaceSelection () -> getFaceVertices (cutFace);
	int32_t    startPoint   = -1;
	int32_t    endPoint     = -1;
	size_t     startVertex1 = -1;
	size_t     startVertex2 = -1;
	size_t     endVertex1   = -1;
	size_t     endVertex2   = -1;

	std::vector <int32_t> face1;
	std::vector <int32_t> face2;

	switch (startPoints .size ())
	{
		case 1:
		{
			if (cutEdge .first == getCoord () -> get1Point (startPoints [0]))
			{
				const auto iter = std::find_if (vertices .begin (), vertices .end (), [&] (const size_t v) { return coordIndex () [v] == startPoints [0]; });

				startVertex1 = *iter;
				endVertex2   = *iter;
			}

			break;
		}
		case 2:
		{
			startPoint = getCoord () -> getSize ();

			getCoord () -> set1Point (startPoint, cutEdge .first);

			startVertex1 = startEdge .second;
			endVertex2   = startEdge .first;
			break;
		}
		default:
			return false;
	}

	switch (endPoints .size ())
	{
		case 1:
		{
			if (cutEdge .first == getCoord () -> get1Point (endPoints [0]))
			{
				const auto iter = std::find_if (vertices .begin (), vertices .end (), [&] (const size_t v) { return coordIndex () [v] == endPoints [0]; });

				endVertex1   = *iter;
				startVertex2 = *iter;
			}

			break;
		}
		case 2:
		{
			endPoint = getCoord () -> getSize ();

			getCoord () -> set1Point (endPoint, cutEdge .second);

			endVertex1   = endEdge .first;
			startVertex2 = endEdge .second;
			break;
		}
		default:
			return false;
	}

	startVertex1 = std::find (vertices .begin (), vertices .end (), startVertex1) - vertices .begin ();
	startVertex2 = std::find (vertices .begin (), vertices .end (), startVertex2) - vertices .begin ();
	endVertex1   = std::find (vertices .begin (), vertices .end (), endVertex1)   - vertices .begin ();
	endVertex2   = std::find (vertices .begin (), vertices .end (), endVertex2)   - vertices .begin ();

	if (endVertex1 < startVertex1)
		endVertex1 += vertices .size ();

	if (endVertex2 < startVertex2)
		endVertex2 += vertices .size ();	

	//

	if (startPoints .size () == 2)
		face1 .emplace_back (startPoint);

	if (endPoints .size () == 2)
		face2 .emplace_back (endPoint);

	//

	for (size_t i = startVertex1; i <= endVertex1; ++ i)
		face1 .emplace_back (coordIndex () [vertices [i % vertices .size ()]]);

	for (size_t i = startVertex2; i <= endVertex2; ++ i)
		face2 .emplace_back (coordIndex () [vertices [i % vertices .size ()]]);

	//
	
	if (endPoints .size () == 2)
		face1 .emplace_back (endPoint);
	
	if (startPoints .size () == 2)
		face2 .emplace_back (startPoint);

	// add new faces

	for (const auto & point : face1)
		coordIndex () .emplace_back (point);

	coordIndex () .emplace_back (-1);

	for (const auto & point : face2)
		coordIndex () .emplace_back (point);

	coordIndex () .emplace_back (-1);

	// erase old face

	coordIndex () .erase (coordIndex () .begin () + vertices .front (), coordIndex () .begin () + vertices .back () + 1);

	return true;
}

X3DIndexedFaceSetKnifeObject::~X3DIndexedFaceSetKnifeObject ()
{ }

} // X3D
} // titania
