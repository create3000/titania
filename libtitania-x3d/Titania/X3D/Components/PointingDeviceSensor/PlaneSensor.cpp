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

#include "PlaneSensor.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

const ComponentType PlaneSensor::component      = ComponentType::POINTING_DEVICE_SENSOR;
const std::string   PlaneSensor::typeName       = "PlaneSensor";
const std::string   PlaneSensor::containerField = "children";

PlaneSensor::Fields::Fields () :
	       axisRotation (new SFRotation ()),
	             offset (new SFVec3f ()),
	        minPosition (new SFVec2f ()),
	        maxPosition (new SFVec2f (-1, -1)),
	translation_changed (new SFVec3f ())
{ }

std::map <X3DBaseNode*, Vector3d> currentOffset;

PlaneSensor::PlaneSensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DDragSensorNode (),
	                fields (),
	           planeSensor (true),
	                  line (),
	                 plane (),
	            startPoint (),
	           startOffset (),
	       modelViewMatrix (),
	      projectionMatrix (),
	              viewport (),
	inverseModelViewMatrix ()
{
	addType (X3DConstants::PlaneSensor);

	addField (inputOutput, "metadata",            metadata ());
	addField (inputOutput, "enabled",             enabled ());
	addField (inputOutput, "description",         description ());
	addField (inputOutput, "axisRotation",        axisRotation ());
	addField (inputOutput, "autoOffset",          autoOffset ());
	addField (inputOutput, "offset",              offset ());
	addField (inputOutput, "minPosition",         minPosition ());
	addField (inputOutput, "maxPosition",         maxPosition ());
	addField (outputOnly,  "trackPoint_changed",  trackPoint_changed ());
	addField (outputOnly,  "translation_changed", translation_changed ());
	addField (outputOnly,  "isOver",              isOver ());
	addField (outputOnly,  "isActive",            isActive ());

	offset ()              .setUnit (UnitCategory::LENGTH);
	minPosition ()         .setUnit (UnitCategory::LENGTH);
	maxPosition ()         .setUnit (UnitCategory::LENGTH);
	translation_changed () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
PlaneSensor::create (X3DExecutionContext* const executionContext) const
{
	return new PlaneSensor (executionContext);
}

///  throws std::domain_error
std::pair <Vector3d, bool>
PlaneSensor::getLineTrackPoint (const HitPtr & hit, const Line3d & line)
{
	const auto screenLine     = ViewVolume::projectLine (line, modelViewMatrix, projectionMatrix, viewport);
	const auto trackPoint1    = screenLine .closest_point (hit -> getPointer ());
	const auto trackPointLine = ViewVolume::unProjectRay (trackPoint1, modelViewMatrix, projectionMatrix, viewport);
	const auto intersection   = line .closest_point (trackPointLine);

	return intersection;
}

void
PlaneSensor::set_active (const bool active,
                         const HitPtr & hit,
                         const Matrix4d & modelViewMatrix,
                         const Matrix4d & projectionMatrix,
                         const Vector4i & viewport)
{
	X3DDragSensorNode::set_active (active, hit, modelViewMatrix, projectionMatrix, viewport);

	try
	{
		if (isActive ())
		{
			this -> modelViewMatrix        = modelViewMatrix;
			this -> projectionMatrix       = projectionMatrix;
			this -> viewport               = viewport;
			this -> inverseModelViewMatrix = inverse (modelViewMatrix);

			const auto hitRay   = hit -> getHitRay () * inverseModelViewMatrix;
			const auto hitPoint = Vector3d (hit -> getIntersection () -> getPoint ()) * inverseModelViewMatrix;

			const Rotation4d axisRotation (this -> axisRotation () .getValue ());

			if (minPosition () .getX () == maxPosition () .getX ())
			{
				const auto direction = Vector3d (0, std::abs (maxPosition () .getY () - minPosition () .getY ()), 0) * axisRotation;

				planeSensor = false;
				line        = Line3d (hitPoint, normalize (direction));
			}
			else if (minPosition () .getY () == maxPosition () .getY ())
			{
				const auto direction = Vector3d (std::abs (maxPosition () .getX () - minPosition () .getX ()), 0, 0) * axisRotation;

				planeSensor = false;
				line        = Line3d (hitPoint, normalize (direction));
			}
			else
			{
				planeSensor = true;
				plane       = Plane3d (hitPoint, Vector3d (0, 0, 1) * axisRotation);
			}

			if (planeSensor)
			{
				const auto & [intersection, intersected] = plane .intersects (hitRay);

				if (intersected)
				{
					startPoint = intersection;

					trackStart (startPoint);
				}
			}
			else
			{
				const auto & [intersection, intersected] = getLineTrackPoint (hit, line);

				if (intersected)
				{
					startPoint = intersection;

					try
					{
						trackStart (getLineTrackPoint (hit, Line3d (line .direction (), line .direction ())) .first);
					}
					catch (const std::domain_error &)
					{
						trackStart (startPoint);
					}
				}
			}
		}
		else
		{
			if (autoOffset ())
				offset () = translation_changed ();
		}
	}
	catch (const std::exception &)
	{ }
}

void
PlaneSensor::trackStart (const Vector3d & trackPoint)
{
	startOffset = offset () .getValue ();

	trackPoint_changed ()  .setValue (trackPoint);
	translation_changed () .setValue (offset ());
}

void
PlaneSensor::set_motion (const HitPtr & hit,
                         const Matrix4d & modelViewMatrix,
                         const Matrix4d & projectionMatrix,
                         const Vector4i & viewport)
{
	try
	{
		X3DDragSensorNode::set_motion (hit, modelViewMatrix, projectionMatrix, viewport);

		if (planeSensor)
		{
			const auto hitRay = hit -> getHitRay () * inverseModelViewMatrix;
			const auto & [intersection, intersected] = plane .intersects (hitRay);

			if (intersected)
			{
				track (intersection, intersection);
			}
			else
				throw std::domain_error ("Plane and line are parallel.");
		}
		else
		{
			const auto & [intersection, intersected] = getLineTrackPoint (hit, line);

			Vector3d trackPoint;

			if (intersected)
			{
				try
				{
					trackPoint = getLineTrackPoint (hit, Line3d (Vector3d (), line .direction ())) .first;
				}
				catch (const std::domain_error &)
				{
					trackPoint = intersection;
				}

				track (intersection, trackPoint);
			}
			else
				throw std::domain_error ("Lines are parallel.");
		}
	}
	catch (const std::domain_error &)
	{ }
}

void
PlaneSensor::track (const Vector3d & endPoint, const Vector3d & trackPoint)
{
	Vector3d translation = (startOffset + endPoint - startPoint) * ~axisRotation ();

	// X component

	if (not (minPosition () .getX () > maxPosition () .getX ()))
		translation .x (std::clamp <float> (translation .x (), minPosition () .getX (), maxPosition () .getX ()));

	// Y component

	if (not (minPosition () .getY () > maxPosition () .getY ()))
		translation .y (std::clamp <float> (translation .y (), minPosition () .getY (), maxPosition () .getY ()));

	translation = translation * Rotation4d (axisRotation () .getValue ());

	if (trackPoint != trackPoint_changed ())
		trackPoint_changed () = trackPoint;

	if (translation != translation_changed ())
		translation_changed () = translation;
}

} // X3D
} // titania
