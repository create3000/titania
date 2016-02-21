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

#include "../../Editing/Selection/FaceSelection.h"
#include "../../Rendering/FrameBuffer.h"

#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/NURBS/CoordinateDouble.h"
#include "../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../Components/PointingDeviceSensor/PlaneSensor.h"
#include "../../Components/Rendering/IndexedLineSet.h"

namespace titania {
namespace X3D {

constexpr double SELECTION_DISTANCE = 8;

X3DIndexedFaceSetSelectionObject::X3DIndexedFaceSetSelectionObject () :
	X3DComposedGeometryNodeTool (),
	                planeSensor (),
	                touchSensor (),
	           activePointCoord (),
	        activeEdgesGeometry (),
	             selectionCoord (),
	      selectedEdgesGeometry (),
	      selectedFacesGeometry (),
	                  coordNode (),
	               activePoints (),
	                 activeFace (),
	                  selection (new FaceSelection (getExecutionContext ())),
	                masterPoint (-1),
	             selectedPoints (),
	              selectedEdges (),
	              selectedFaces (),
	                translation ()
{
	//addType (X3DConstants::X3DIndexedFaceSetSelectionObject);

	addChildren (planeSensor,
	             touchSensor,
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

		planeSensor           = inlineNode -> getExportedNode <PlaneSensor> ("PlaneSensor");
		touchSensor           = inlineNode -> getExportedNode <TouchSensor> ("TouchSensor");
		activePointCoord      = inlineNode -> getExportedNode <CoordinateDouble> ("ActivePointCoord");
		activeEdgesGeometry   = inlineNode -> getExportedNode <IndexedLineSet> ("ActiveEdgesGeometry");
		selectionCoord        = inlineNode -> getExportedNode <CoordinateDouble> ("SelectionCoord");
		selectedEdgesGeometry = inlineNode -> getExportedNode <IndexedLineSet> ("SelectedEdgesGeometry");
		selectedFacesGeometry = inlineNode -> getExportedNode <IndexedFaceSet> ("SelectedFacesGeometry");

		touchSensor -> isOver ()           .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_over);
		touchSensor -> isActive ()         .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_active);
		touchSensor -> hitPoint_changed () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint);
		touchSensor -> touchTime ()        .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime);

		planeSensor -> isActive ()            .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_plane_sensor_active);
		planeSensor -> translation_changed () .addInterest (this, &X3DIndexedFaceSetSelectionObject::set_plane_sensor_translation);

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
		// Update active point

		for (auto & activePoint : activePoints)
			activePoint .second = getCoord () -> get1Point (activePoint .first);

		setActivePoints ();
	
		// Update selected points
	
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
X3DIndexedFaceSetSelectionObject::set_touch_sensor_active (const bool active)
{
	if (active)
		translation = Vector3d ();
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_hitPoint (const X3D::Vector3f & hitPoint)
{
	if (planeSensor -> isActive ())
		return;

	// Determine face and faces

	selection -> findCoincidentPoints (hitPoint);

	if (selection -> getCoincidentPoints () .empty ())
		return;

	// Set selected point

	selection -> setAdjacentFaces (hitPoint);

	setActiveSelection (hitPoint);

	if (touchSensor -> isActive () and paintSelection ())
		set_touch_sensor_touchTime ();

	// Setup PlaneSensor

	if (activePoints .size () == 0)
	{
		planeSensor -> enabled () = false;
	}
	else if (activePoints .size () == 1)
	{
		// Translate over screen plane

		const auto vector       = inverse (getModelViewMatrix ()) .mult_dir_matrix (Vector3d (0, 0, 1));
		const auto axisRotation = Rotation4d (Vector3d (0, 0, 1), vector);

		planeSensor -> enabled ()      = not paintSelection ();
		planeSensor -> axisRotation () = axisRotation;
		planeSensor -> maxPosition ()  = Vector2f (-1, -1);
	}
	else if (activePoints .size () == 2)
	{
		// Translate along edge

		const auto vector       = getCoord () -> get1Point (activeEdgesGeometry -> coordIndex () [0]) - getCoord () -> get1Point (activeEdgesGeometry -> coordIndex () [1]);
		const auto axisRotation = Rotation4d (Vector3d (1, 0, 0), vector);

		planeSensor -> enabled ()      = not paintSelection ();
		planeSensor -> axisRotation () = axisRotation;
		planeSensor -> maxPosition ()  = Vector2f (-1, 0);
	}
	else
	{
		// Translate along face normal
		planeSensor -> enabled () = false;
	}
}

void
X3DIndexedFaceSetSelectionObject::set_selection_ (const MFVec3d & vertices)
{
	FrameBuffer depthBuffer (getBrowser (), getBrowser () -> getRectangle () [2], getBrowser () -> getRectangle () [3], 0, false);

	depthBuffer .setup ();
	depthBuffer .bind ();
	getBrowser () -> getActiveLayer () -> traverse (TraverseType::DEPTH);
	depthBuffer .readDepth ();
	depthBuffer .unbind ();

	for (const Vector3d & vertex : vertices)
	{
		// In this order.
		selection -> setCoincidentPoints (vertex);
		selection -> setAdjacentFaces (vertex);
		selectPoints (vertex, paintSelection (), true);
		selectFaces (vertex, paintSelection (), true, true);
	}

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
}

void
X3DIndexedFaceSetSelectionObject::set_touch_sensor_touchTime ()
{
	if (translation not_eq Vector3d ())
		return;

	bool first = true;

	for (const auto & activePoint : activePoints)
	{
		// In this order.
		selection -> setCoincidentPoints (activePoint .second);
		selectPoints (activePoint .second, not first or paintSelection (), false);
		selectFaces  (activePoint .second, not first or paintSelection (), false, false);
		first = false;
	}

	// In this order.
	updateSelectedFaces ();
	updateSelectedEdges ();
	updateSelectedPoints ();
}

void
X3DIndexedFaceSetSelectionObject::setActiveSelection (const Vector3f & hitPoint)
{
	const auto vertices = selection -> getVertices (selection -> getFace () .first);
	const auto index    = selection -> getCoincidentPoints () [0];
	const auto point    = getCoord () -> get1Point (index);

	// Active line

	activeFace   .clear ();
	activePoints .clear ();

	if (vertices .size () >= 3)
	{
		// Active points for near point or face
	
		if (getDistance (hitPoint, point) > SELECTION_DISTANCE)
		{
			const auto edge     = selection -> getEdge (vertices, hitPoint);
			const auto distance = getDistance (hitPoint, edge .line .closest_point (hitPoint));

			if (distance > SELECTION_DISTANCE)
			{
				// Face

				activeFace  = vertices;

				for (const auto & i : vertices)
					activePoints .emplace (coordIndex () [i], getCoord () -> get1Point (coordIndex () [i]));
			}
			else
			{
				// Edge

				activePoints .emplace (coordIndex () [edge .index0], edge .point0);
				activePoints .emplace (coordIndex () [edge .index1], edge .point1);
			}
		}
		else
		{
			// Point
			activePoints = { std::make_pair (index, point) };
		}
	}

	setActivePoints ();
}

void
X3DIndexedFaceSetSelectionObject::setActivePoints ()
{
	activeEdgesGeometry -> coordIndex () .clear ();

	if (activePoints .size () == 2)
	{
		for (const auto activePoint : activePoints)
			activeEdgesGeometry -> coordIndex () .emplace_back (activePoint .first);
	}
	else if (activePoints .size () > 2)
	{
		const auto vertices = selection -> getVertices (selection -> getFace () .first);

		for (const auto vertex : vertices)
			activeEdgesGeometry -> coordIndex () .emplace_back (coordIndex () [vertex]);

		activeEdgesGeometry -> coordIndex () .emplace_back (coordIndex () [vertices [0]]);
	}

	activePointCoord -> point () .clear ();

	for (const auto activePoint : activePoints)
		activePointCoord -> point () .emplace_back (getCoord () -> get1Point (activePoint .first));
}

void
X3DIndexedFaceSetSelectionObject::selectPoints (const Vector3d & hitPoint, const bool paint, const bool coincidentPoints)
{
	struct PointComp
	{
		bool
		operator () (const SFVec3f & value, const Vector3f & point)
		{ return value .getValue () < point; }

	};

	for (const auto & index : selection -> getCoincidentPoints ())
	{
		const auto point = getCoord () -> get1Point (index);

		if (getDistance (hitPoint, point) > SELECTION_DISTANCE)
			return;
	
		if (not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey () and not paint)
		{
			selectionCoord -> point () .clear ();
			selectedPoints .clear ();
		}
	
		if (getBrowser () -> hasControlKey ())
			removeSelectedPoint (index);

		else
			addSelectedPoint (index);

		if (not coincidentPoints)
			break;
	}
}

void
X3DIndexedFaceSetSelectionObject::selectFaces (const Vector3d & hitPoint, const bool paint, const bool coincidentPoints, const bool adjacentFaces)
{
	struct PointComp
	{
		bool
		operator () (const SFVec3f & value, const Vector3f & point)
		{ return value .getValue () < point; }

	};

	for (const auto & index : selection -> getCoincidentPoints ())
	{
		const auto point = getCoord () -> get1Point (index);
	
		if (getDistance (hitPoint, point) > SELECTION_DISTANCE)
			return;
	
		if (not getBrowser () -> hasShiftKey () and not getBrowser () -> hasControlKey () and not paint)
		{
			selectedFacesGeometry -> coordIndex () .clear ();
			selectedFaces .clear ();
			selectedEdges .clear ();
		}
	
		if (getBrowser () -> hasControlKey ())
		{
			removeSelectedFace (selection -> getFace () .first);

			if (adjacentFaces)
			{
				for (const auto & face : selection -> getAdjacentFaces ())
					removeSelectedFace (face .first);
			}
		}
		else
		{
			addSelectedFace (selection -> getFace () .first);

			if (adjacentFaces)
			{
				for (const auto & face : selection -> getAdjacentFaces ())
					addSelectedFace (face .first);
			}
		}

		if (not coincidentPoints)
			break;
	}
}

void
X3DIndexedFaceSetSelectionObject::addSelectedPoint (const int32_t index)
{
	const auto point = getCoord () -> get1Point (index);

	selectedPoints .emplace (index, point);

	masterPoint = index;
}

void
X3DIndexedFaceSetSelectionObject::removeSelectedPoint (const int32_t index)
{
	selectedPoints .erase (index);
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

		auto index0 = coordIndex () [vertices [i0]];
		auto index1 = coordIndex () [vertices [i1]];

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

		auto index0 = coordIndex () [vertices [i0]];
		auto index1 = coordIndex () [vertices [i1]];

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
X3DIndexedFaceSetSelectionObject::addSelectedFace (const size_t index)
{
	const auto vertices = selection -> getVertices (index);

	if (isInSelection (vertices))
	{
		addSelectedEdges (vertices);
		selectedFaces .emplace (index);
	}
}

void
X3DIndexedFaceSetSelectionObject::removeSelectedFace (const size_t index)
{
	const auto vertices = selection -> getVertices (index);

	if (not isInSelection (vertices))
	{
		removeSelectedEdges (vertices);
		selectedFaces .erase (index);
	}
}

void
X3DIndexedFaceSetSelectionObject::updateSelectedFaces ()
{
	for (const auto index : selectedFaces)
	{
		for (const auto v : selection -> getVertices (index))
			addSelectedPoint (coordIndex () [v]);
	}

	size_t i = 0;

	for (const auto index : selectedFaces)
	{
		const auto vertices = selection -> getVertices (index);
	
		for (const auto v : vertices)
			selectedFacesGeometry -> coordIndex () .set1Value (i ++, coordIndex () [v]);
	
		selectedFacesGeometry -> coordIndex () .set1Value (i ++, -1);
	}

	selectedFacesGeometry -> coordIndex () .resize (i);
}

bool
X3DIndexedFaceSetSelectionObject::isInSelection (const std::vector <size_t> & vertices) const
{
	for (const auto & v : vertices)
	{
		if (not selectedPoints .count (coordIndex () [v]))
			return false;
	}

	return true;
}

void
X3DIndexedFaceSetSelectionObject::set_plane_sensor_active (const bool active)
{
	if (active)
		return;

	set_coord_point ();
}

void
X3DIndexedFaceSetSelectionObject::set_plane_sensor_translation (const Vector3f & value)
{
	translation = value;

	for (const auto & pair : selectedPoints)
		getCoord () -> set1Point (pair .first, pair .second + Vector3d (translation));
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
