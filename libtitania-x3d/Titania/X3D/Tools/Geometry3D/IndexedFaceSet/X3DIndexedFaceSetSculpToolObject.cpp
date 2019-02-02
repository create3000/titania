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

#include "X3DIndexedFaceSetSculpToolObject.h"

#include "../../Rendering/CoordinateTool.h"

#include "../../../Browser/X3DBrowser.h"
#include "../../../Components/PointingDeviceSensor/TouchSensor.h"
#include "../../../Components/Rendering/X3DCoordinateNode.h"
#include "../../../Editing/Undo/UndoStepContainer.h"

#include <random>
#include <chrono>

namespace titania {
namespace X3D {

///  Brush
///  SFString type      ["CIRCULAR", "SQUARED"]
///  SFDouble radius    [0, inf]
///  SFDouble height    [-inf, inf]
///  SFDouble warp      [0, 1]
///  SFDouble hardness  [0, 1]
///  SFDouble sharpness [0, 1]
///  SFDouble spacing   [0, inf]

X3DIndexedFaceSetSculpToolObject::X3DIndexedFaceSetSculpToolObject () :
	              IndexedFaceSet (getExecutionContext ()),
	X3DIndexedFaceSetBrushObject (),
	                 touchSensor (),
	                   coordNode (),
	                lastHitPoint (),
	             pointerDistance (0),
	                  undoPoints (),
	                    undoStep ()
{
	addType (X3DConstants::X3DIndexedFaceSetSculpToolObject);

	addChildObjects (touchSensor, coordNode);
}

void
X3DIndexedFaceSetSculpToolObject::initialize ()
{
	getCoordinateTool () -> getInlineNode () -> checkLoadState () .addInterest (&X3DIndexedFaceSetSculpToolObject::set_loadState, this);

	getCoord () .addInterest (&X3DIndexedFaceSetSculpToolObject::set_coord, this);

	set_coord ();
}

void
X3DIndexedFaceSetSculpToolObject::set_loadState ()
{
	try
	{
		const auto & inlineNode = getCoordinateTool () -> getInlineNode ();

		touchSensor = inlineNode -> getExportedNode <TouchSensor> ("SculpBrushTouchSensor");

		touchSensor -> isActive ()         .addInterest (&X3DIndexedFaceSetSculpToolObject::set_touch_sensor_active,   this);
		touchSensor -> hitPoint_changed () .addInterest (&X3DIndexedFaceSetSculpToolObject::set_touch_sensor_hitPoint, this);
	}
	catch (const X3DError & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

void
X3DIndexedFaceSetSculpToolObject::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (&X3DIndexedFaceSetSculpToolObject::set_coord_points, this);

	coordNode = getCoord ();

	if (coordNode)
	{
		coordNode -> addInterest (&X3DIndexedFaceSetSculpToolObject::set_coord_points, this);

		undoPoints .clear ();

		set_coord_points ();
	}
}

void
X3DIndexedFaceSetSculpToolObject::set_coord_points ()
{
	if (getCoord () -> getSize () not_eq undoPoints .size ())
	{
		undoPoints .clear ();

		for (size_t i = 0, size = getCoord () -> getSize (); i < size; ++ i)
			undoPoints .emplace_back (getCoord () -> get1Point (i));
	}
}

void
X3DIndexedFaceSetSculpToolObject::set_touch_sensor_active ()
{
	try
	{
		if (touchSensor -> isActive ())
		{
			lastHitPoint    = touchSensor -> getHitPoint ();
			pointerDistance = std::numeric_limits <double>::infinity ();

			undoStep = std::make_shared <UndoStep> (_ (height () >= 0.0 ? "Pull Polygons" : "Push Polygons"));
	
			undoSetCoord (undoStep);
		}
		else
		{
			redoSetCoord (undoStep);
	
			undo_changed () = getExecutionContext () -> createNode <UndoStepContainer> (undoStep);
		}
	}
	catch (const X3DError & error)
	{ }
}

void
X3DIndexedFaceSetSculpToolObject::set_touch_sensor_hitPoint ()
{
	try
	{
		if (not touchSensor -> isActive ())
			return;
		
		if (not getCoord ())
			return;
	
		const auto & hitPoint = touchSensor -> getHitPoint ();

		pointerDistance += math::distance (hitPoint, lastHitPoint);

		if (pointerDistance > spacing ())
		{
			const auto & hitNormal = touchSensor -> getHitNormal ();
			const auto   r         = radius () * scale ();

			// Determine affected points.

			std::vector <std::pair <size_t, Vector3d>> points;

			for (size_t i = 0, size = getCoord () -> getSize (); i < size; ++ i)
			{
				const auto point    = getCoord () -> get1Point (i);
				const auto distance = math::distance (hitPoint, point);
		
				if (distance < r)
					points .emplace_back (i, point);
			}

			// Select tool.

			if (toolType () == "SCULP")
			{
				for (const auto & pair : points)
					getCoord () -> set1Point (pair .first, pair .second + (getSculpVector (hitNormal, hitPoint, pair .second) * height () .getValue ()));
			}
			else if (toolType () == "SCULP_ROUGHENING")
			{
				for (const auto & pair : points)
					getCoord () -> set1Point (pair .first, pair .second + (getRoughtVector (hitNormal, hitPoint, pair .second) * height () .getValue ()));
			}
			else if (toolType () == "SCULP_SMOOTHING")
			{
				// Calculate average point within radius.

				Vector3d averagePoint;

				for (const auto & pair : points)
					averagePoint += pair .second;

				averagePoint /= points .size ();

				// Apply smooth vector.

				for (const auto & pair : points)
					getCoord () -> set1Point (pair .first, pair .second + getSmoothVector (hitNormal, averagePoint, pair .second));
			}
			else if (toolType () == "SCULP_UNDO")
			{
				for (const auto & pair : points)
					getCoord () -> set1Point (pair .first, pair .second + getUndoVector (hitNormal, hitPoint, pair .second, undoPoints .at (pair .first)));
			}

			pointerDistance = 0;
		}

		lastHitPoint = hitPoint;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

double
X3DIndexedFaceSetSculpToolObject::getSculpHeight (const Vector3d & hitNormal,
                                                  const Vector3d & hitPoint,
                                                  const Vector3d & point)
{
	const auto p = (point - hitPoint) * Rotation4d (hitNormal, Vector3d (0, 0, 1));
	const auto r = radius () * scale ();
	const auto v = Vector2d (p .x (), p .y ()) / r;

	return getHeight (v);
}

Vector3d
X3DIndexedFaceSetSculpToolObject::getSculpVector (const Vector3d & hitNormal,
                                                  const Vector3d & hitPoint,
                                                  const Vector3d & point)
{
	return hitNormal * getSculpHeight (hitNormal, hitPoint, point);
}

Vector3d
X3DIndexedFaceSetSculpToolObject::getRoughtVector (const Vector3d & hitNormal,
                                                   const Vector3d & hitPoint,
                                                   const Vector3d & point)
{
	static std::uniform_real_distribution <double>
	uniform_real_distribution (-1, 1);

	static std::default_random_engine
	random_engine (std::chrono::system_clock::now () .time_since_epoch () .count ());

	const auto height = getSculpHeight (hitNormal, hitPoint, point);

	return uniform_real_distribution (random_engine) * height * hitNormal;
}

Vector3d
X3DIndexedFaceSetSculpToolObject::getSmoothVector (const Vector3d & hitNormal,
                                                   const Vector3d & hitPoint,
                                                   const Vector3d & point)
{
	const auto plane    = Plane3d (hitPoint, hitNormal);
	const auto distance = plane .distance (point);
	const auto height   = getSculpHeight (hitNormal, hitPoint, point);

	return -(distance * height) * hitNormal;
}

Vector3d
X3DIndexedFaceSetSculpToolObject::getUndoVector (const Vector3d & hitNormal,
                                                 const Vector3d & hitPoint,
                                                 const Vector3d & point,
                                                 const Vector3d & undoPoint)
{
	const auto height    = getSculpHeight (hitNormal, hitPoint, point);
	const auto direction = undoPoint - point;

	return (abs (direction) > 1 ? normalize (direction) : direction) * height;
}

X3DIndexedFaceSetSculpToolObject::~X3DIndexedFaceSetSculpToolObject ()
{ }

} // X3D
} // titania
