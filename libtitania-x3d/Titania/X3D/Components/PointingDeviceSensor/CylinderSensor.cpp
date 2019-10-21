/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, ScheffelstraÃÂÃÂe 31a, Leipzig, Germany 2011.
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

#include "CylinderSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   CylinderSensor::component      = Component ("PointingDeviceSensor", 1);
const std::string CylinderSensor::typeName       = "CylinderSensor";
const std::string CylinderSensor::containerField = "children";

CylinderSensor::Fields::Fields () :
	    axisRotation (new SFRotation (0, 1, 0, 0)),
	       diskAngle (new SFFloat (0.261792)),      // PI <float> / 12
	        minAngle (new SFFloat ()),
	        maxAngle (new SFFloat (-1)),
	          offset (new SFFloat ()),
	rotation_changed (new SFRotation ())
{ }

CylinderSensor::CylinderSensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DDragSensorNode (),
	                fields (),
	              cylinder (),
	                  disk (false),
	                yPlane (),
	                zPlane (),
	                behind (1),
	            fromVector (),
	           startOffset (),
	                 angle (0),
	           startVector (),
	inverseModelViewMatrix ()
{
	addType (X3DConstants::CylinderSensor);

	addField (inputOutput, "metadata",           metadata ());
	addField (inputOutput, "enabled",            enabled ());
	addField (inputOutput, "description",        description ());
	addField (inputOutput, "axisRotation",       axisRotation ());
	addField (inputOutput, "diskAngle",          diskAngle ());
	addField (inputOutput, "minAngle",           minAngle ());
	addField (inputOutput, "maxAngle",           maxAngle ());
	addField (inputOutput, "offset",             offset ());
	addField (inputOutput, "autoOffset",         autoOffset ());
	addField (outputOnly,  "trackPoint_changed", trackPoint_changed ());
	addField (outputOnly,  "rotation_changed",   rotation_changed ());
	addField (outputOnly,  "isOver",             isOver ());
	addField (outputOnly,  "isActive",           isActive ());

	diskAngle () .setUnit (UnitCategory::ANGLE);
	minAngle ()  .setUnit (UnitCategory::ANGLE);
	maxAngle ()  .setUnit (UnitCategory::ANGLE);
	offset ()    .setUnit (UnitCategory::ANGLE);
}

X3DBaseNode*
CylinderSensor::create (X3DExecutionContext* const executionContext) const
{
	return new CylinderSensor (executionContext);
}

bool
CylinderSensor::isBehind (const Line3d & hitRay, const Vector3d & hitPoint) const
{
	const auto & [intersection1, intersection2, intersected] = cylinder .intersects (hitRay);

	return abs (hitPoint - intersection1) > abs (hitPoint - intersection2);
}

Vector3d
CylinderSensor::getTrackPoint (const Line3d & hitRay) const
{
	const auto zPoint    = zPlane .intersects (hitRay) .first;
	const auto axisPoint = zPoint + cylinder .axis () .perpendicular_vector (zPoint);
	const auto distance  = sxPlane .distance (zPoint) / cylinder .radius ();
	const auto section   = std::floor ((distance + 1) / 2);

	// Use asin on the cylinder and outside linear angle.
	const auto sinp       = interval (distance, -1.0, 1.0);
	const auto phi        = section == 0 ? std::asin (sinp) : sinp * pi_2 <double>;
	const auto angle      = phi + section * pi <double>;
	const auto rotation   = Rotation4d  (cylinder .axis () .direction (), angle);
	const auto trackPoint = szNormal * cylinder .radius () * rotation + axisPoint;

	return trackPoint;
}

// Returns an angle in the interval [-2pi,2pi]

double
CylinderSensor::getAngle (const Rotation4d & rotation) const
{
	if (dot (rotation .axis (), cylinder .axis () .direction ()) > 0)
		return rotation .angle ();

	else
		return -rotation .angle ();
}

void
CylinderSensor::set_active (const bool active,
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
			inverseModelViewMatrix = inverse (modelViewMatrix);

			const auto hitRay   = hit -> getHitRay () * inverseModelViewMatrix;
			const auto hitPoint = Vector3d (hit -> getIntersection () -> getPoint ()) * inverseModelViewMatrix;

			const auto     yAxis      = Vector3d (0, 1, 0) * Rotation4d (axisRotation () .getValue ());
			const Vector3d cameraBack = normalize (inverseModelViewMatrix .mult_dir_matrix (Vector3d (0, 0, 1)));

			const auto axis   = Line3d (Vector3d (), yAxis);
			const auto radius = abs (axis .perpendicular_vector (hitPoint));

			cylinder = Cylinder3d (axis, radius);

			disk   = std::abs (dot (cameraBack, yAxis)) > std::cos (diskAngle ());
			behind = isBehind (hitRay, hitPoint);

			yPlane = Plane3d (hitPoint, yAxis);             // Sensor aligned y-plane
			zPlane = Plane3d (hitPoint, cameraBack);        // Screen aligned z-plane

			// Compute normal as in Billboard with yAxis as axis of rotation.
			const Vector3d billboardToViewer = inverseModelViewMatrix .origin ();
			const Vector3d sxNormal          = normalize (cross (yAxis, billboardToViewer));

			sxPlane  = Plane3d (Vector3d (), sxNormal);     // Billboarded special x-plane made parallel to sensors axis.
			szNormal = normalize (cross (sxNormal, yAxis)); // Billboarded special z-normal made parallel to sensors axis.

			Vector3d trackPoint;

			if (disk)
				trackPoint = yPlane .intersects (hitRay) .first;
			else
				trackPoint = getTrackPoint (hitRay);

			fromVector  = -cylinder .axis () .perpendicular_vector (trackPoint);
			startOffset = Rotation4d (yAxis, offset ());

			trackPoint_changed () .setValue (trackPoint);
			rotation_changed ()   .setValue (startOffset);

			// For min/max angle.

			angle       = offset ();
			startVector = Vector3d (0, 0, 1) * axisRotation () * rotation_changed ();
		}
		else
		{
			if (autoOffset ())
				offset () = getAngle (rotation_changed () .getValue ());
		}
	}
	catch (const std::exception &)
	{ }
}

void
CylinderSensor::set_motion (const HitPtr & hit,
                            const Matrix4d & modelViewMatrix,
                            const Matrix4d & projectionMatrix,
                            const Vector4i & viewport)
{
	try
	{
		X3DDragSensorNode::set_motion (hit, modelViewMatrix, projectionMatrix, viewport);

		const auto hitRay = hit -> getHitRay () * inverseModelViewMatrix;

		Vector3d trackPoint;

		if (disk)
			trackPoint = yPlane .intersects (hitRay) .first;
		else
			trackPoint = getTrackPoint (hitRay);

		trackPoint_changed () = trackPoint;

		const auto toVector = -cylinder .axis () .perpendicular_vector (trackPoint);
		auto       rotation = Rotation4d (fromVector, toVector);

		if (disk)
		{
			// The trackpoint can swap behind the viewpoint if viewpoint is a Viewpoint node
			// as the viewing volume is not a cube where the picking ray goes straight up.
			// This phenomenon is very clear on the viewport corners.

			const auto trackPoint_ = trackPoint * inverse (inverseModelViewMatrix);

			if (trackPoint_ .z () > 0)
				rotation *= Rotation4d (yPlane .normal (), pi <double>);
		}
		else
		{
			if (behind)
				rotation .inverse ();
		}

		rotation = startOffset * rotation;

		if (minAngle () > maxAngle ())
		{
			rotation_changed () = rotation;
		}
		else
		{
			const auto   endVector     = Vector3d (0, 0, 1) * Rotation4d (axisRotation ()) * rotation;
			const auto   deltaRotation = Rotation4d (startVector, endVector);
			const auto   axis          = Vector3d (0, 1, 0) * Rotation4d (axisRotation ());
			const auto   sign          = dot (axis, deltaRotation .axis ()) > 0 ? 1 : -1;
			const double min           = minAngle ();
			const double max           = maxAngle ();

			angle += sign * deltaRotation .angle ();

			startVector = endVector;

			if (angle < min)
				rotation = Rotation4d (cylinder .axis () .direction (), min);
			else if (angle > max)
				rotation = Rotation4d (cylinder .axis () .direction (), max);
			else
				rotation = Rotation4d (cylinder .axis () .direction (), angle);

			if (rotation_changed () != rotation)
				rotation_changed () = rotation;
		}
	}
	catch (const std::domain_error &)
	{ }
}

} // X3D
} // titania
