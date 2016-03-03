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
	                        cutPoint (-1),
	                         cutFace (0),
	                        cutFaces ()
{
	cutPolygons () .isHidden (true);

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
		setActionType (ActionType::CUT);
		getHotSwitch () -> whichChoice () = true;
	}
	else
		setActionType (ActionType::SELECT);

	knifeSwitch -> whichChoice () = cutPolygons ();
}

void
X3DIndexedFaceSetKnifeObject::set_touch_sensor_hitPoint  ()
{
	if (not cutPolygons ())
	   return;

	// Set hot points.

	if (not setMagicSelection ())
	   return;
	
	if (planeSensor -> isActive ())
	   ;
	else
	{
	   // Set start point

	   if (getHotEdge () .empty ())
	      return;

		if (getHotPoints () .size () == 1)
			knifeLineCoordinate -> point () [0] = getCoord () -> get1Point (getHotPoints () .front ());

		else
		{
			const Line3d edgeLine (getCoord () -> get1Point (coordIndex () [getHotEdge () .front ()]),
			                       getCoord () -> get1Point (coordIndex () [getHotEdge () .back ()]),
			                       math::point_type ());

			knifeLineCoordinate -> point () [0] = edgeLine .closest_point (touchSensor -> getHitPoint ());
		}  

		const auto normal       = getPolygonNormal (getFaceSelection () -> getFaceVertices (getHotFace ()));		               
		const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), Vector3d (normal));

		planeSensor -> enabled ()      = true;
		planeSensor -> axisRotation () = axisRotation;
		planeSensor -> offset ()       = touchSensor -> getHitPoint ();
		planeSensor -> maxPosition ()  = Vector2d (-1, -1);

		knifeLineCoordinate -> point () [1] = knifeLineCoordinate -> point () [0];
		knifeStartPoint -> translation ()   = knifeLineCoordinate -> point () [0] .getValue ();
		knifeEndPoint -> translation ()     = touchSensor -> getHitPoint ();
	}
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor_active ()
{
	if (not cutPolygons ())
	   return;

	knifeLineSwitch -> whichChoice () = planeSensor -> isActive ();	 
  
	if (planeSensor -> isActive ())
	{
	}
}

void
X3DIndexedFaceSetKnifeObject::set_plane_sensor_translation ()
{
	if (not cutPolygons ())
	   return;

	if (getHotPoints () .size () == 1)
		knifeLineCoordinate -> point () [1] = getCoord () -> get1Point (getHotPoints () .front ());

	else
		knifeLineCoordinate -> point () [1] = planeSensor -> translation_changed () .getValue ();

	const Line3d edgeLine (getCoord () -> get1Point (coordIndex () [getHotEdge () .front ()]),
	                       getCoord () -> get1Point (coordIndex () [getHotEdge () .back ()]),
	                       math::point_type ());
	                      
	const Line3d cutLine (knifeLineCoordinate -> point () [0],
	                      knifeLineCoordinate -> point () [1],
	                      math::point_type ());

	Vector3d closestPoint;
                     
	if (edgeLine .closest_point (cutLine, closestPoint))
	{
		knifeLineSwitch -> whichChoice () = true;	
		knifeEndPoint -> translation ()   = closestPoint;
	}
	else
	{
		knifeLineSwitch -> whichChoice () = false;	
	}
}

///  Determine and update hot and active points, edges and face
bool
X3DIndexedFaceSetKnifeObject::setMagicSelection ()
{
	// Find closest points.

	const auto coincidentPoints = getFaceSelection () -> getCoincidentPoints (touchSensor -> getClosestPoint ());

	if (coincidentPoints .empty ())
		return false;
			
	const auto & hitPoint = touchSensor -> getHitPoint ();
	auto         index    = coincidentPoints [0];
	const auto   faces    = getFaceSelection () -> getAdjacentFaces (coincidentPoints);
	auto         face     = getFaceSelection () -> getNearestFace (hitPoint, faces) .index;

	if (planeSensor -> isActive ())
	{
		if (cutFaces .count (face))
		{
			cutPoint = index;
			cutFace  = face;				   
		}
		else
		{
			face  = getFaceSelection () -> getNearestFace (hitPoint, std::vector <size_t> (cutFaces .begin (), cutFaces .end ())) .index;
			index = cutPoint;
		}
	}
	else
	{
		cutPoint = index;
		cutFace  = face;
		cutFaces = getHotFaces ();
	}

	const auto point    = getCoord () -> get1Point (index);
	const auto vertices = getFaceSelection () -> getFaceVertices (face);

	setHotFace (face);
	setHotFaces ({ });
	setHotPoints ({ });

	if (vertices .size () >= 3)
	{
		const auto edge          = getFaceSelection () -> getEdge (hitPoint, vertices);
		const auto pointDistance = getDistance (hitPoint, point);

		// Hot edge and points for near point or face

		setHotEdge ({ edge .index0, edge .index1 });
	
		if (edge .isEdge and pointDistance > SELECTION_DISTANCE)
		{
			// Edge
			std::set <size_t> hotFaces;

			setHotPoints ({ coordIndex () [edge .index0], coordIndex () [edge .index1] });

			for (const auto & face : getFaceSelection () -> getAdjacentFaces (edge))
				hotFaces .emplace (face);
			
			setHotFaces (hotFaces);
		}
		else
		{
			// Point
			std::set <size_t> hotFaces;

			setHotPoints ({ index });

			for (const auto & face : faces)
				hotFaces .emplace (face .index);
			
			setHotFaces (hotFaces);
		}
	}

	updateMagicSelection ();
	return true;
}

X3DIndexedFaceSetKnifeObject::~X3DIndexedFaceSetKnifeObject ()
{ }

} // X3D
} // titania
