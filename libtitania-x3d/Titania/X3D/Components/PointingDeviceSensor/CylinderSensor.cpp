/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "CylinderSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

CylinderSensor::Fields::Fields () :
	axisRotation (new SFRotation (0, 1, 0, 0)),
	diskAngle (new SFFloat (M_PI / 12)),
	minAngle (new SFFloat ()),
	maxAngle (new SFFloat (-1)),
	offset (new SFFloat ()),
	rotation_changed (new SFRotation ())
{ }

CylinderSensor::CylinderSensor (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DDragSensorNode (),
	           fields ()
{
	setComponent ("PointingDeviceSensor");
	setTypeName ("CylinderSensor");

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
}

X3DBaseNode*
CylinderSensor::create (X3DExecutionContext* const executionContext) const
{
	return new CylinderSensor (executionContext);
}

bool
CylinderSensor::isBehind (const Line3f & hitRay, const Vector3f & hitPoint) const
{
	Vector3f enter, exit;

	cylinder .intersect (hitRay, enter, exit);

	return abs (hitPoint - enter) > abs (hitPoint - exit);
}

Vector3f
CylinderSensor::getVector (const Line3f & hitRay, const Vector3f & trackPoint) const
{
	Vector3f intersection;

	if (disk and yPlane .intersect (hitRay, intersection))
		return normalize (intersection);

	return normalize (-cylinder .axis () .perpendicular_vector (trackPoint));
}

bool
CylinderSensor::getTrackPoint (const Line3f & hitRay, Vector3f & trackPoint, bool behind) const
{
	Vector3f exit;

	if (cylinder .intersect (hitRay, trackPoint, exit))
	{
		if ((abs (hitRay .origin () - exit) < abs (hitRay .origin () - trackPoint)) - behind)
			trackPoint = exit;

		return true;
	}

	return false;
}

float
CylinderSensor::getAngle (const Rotation4f & rotation) const
{
	if (dot (rotation .axis (), cylinder .axis () .direction ()) > 0)
		return rotation .angle ();

	else
		return -rotation .angle ();
}

void
CylinderSensor::set_active (const std::shared_ptr <Hit> & hit, bool active)
{
	X3DDragSensorNode::set_active (hit, active);

	if (isActive ())
	{
		inverseTransformationMatrix = ~getTransformationMatrix ();

		auto hitRay   = hit -> ray * inverseTransformationMatrix;
		auto hitPoint = hit -> point * inverseTransformationMatrix;

		auto yAxis = axisRotation () * Vector3f (0, 1, 0);
		auto zAxis = inverseTransformationMatrix .multDirMatrix (axisRotation () * Vector3f (0, 0, 1));

		auto axis    = Line3f (Vector3f (), yAxis);
		auto pvector = axis .perpendicular_vector (hitPoint);

		cylinder = Cylinder3f (axis, abs (pvector));

		yPlane = Plane3f (Vector3f (), yAxis);  // Sensor aligned Y-plane
		zPlane = Plane3f (hitPoint, zAxis);     // Screen aligned Z-plane.

		disk   = std::abs (dot (hitRay .direction (), yAxis)) > std::cos (diskAngle ());
		behind = isBehind (hitRay, hitPoint);

		Vector3f trackPoint;
		getTrackPoint (hitRay, trackPoint, behind);

		fromVector = getVector (hitRay, trackPoint);

		trackPoint_changed () = trackPoint;
		rotation_changed ()   = Rotation4f (yAxis, offset ());
		startOffset           = offset ();
	}
	else
	{
		if (autoOffset ())
			offset () = getAngle (rotation_changed ());
	}
}

void
CylinderSensor::set_motion (const std::shared_ptr <Hit> & hit)
{
	auto hitRay = hit -> ray * inverseTransformationMatrix;

	Vector3f trackPoint;

	if (getTrackPoint (hitRay, trackPoint, behind))
	{
		zPlane = Plane3f (trackPoint, zPlane .normal ());
	}
	else
	{
		// Find trackPoint on the plane with cylinder

		Vector3f p1;
		zPlane .intersect (hitRay, p1);

		auto axisPoint = p1 + cylinder .axis () .perpendicular_vector (p1);

		auto hitRay = Line3f (p1, axisPoint, true);

		getTrackPoint (hitRay, trackPoint);

		// Find trackPoint behind sphere

		auto normal = zPlane .normal ();

		if (behind)
			normal .negate ();

		hitRay = Line3f (trackPoint - normal * abs (p1 - trackPoint), axisPoint, true);

		getTrackPoint (hitRay, trackPoint);
	}

	trackPoint_changed () = trackPoint;

	auto toVector = getVector (hitRay, trackPoint);
	auto offset   = Rotation4f (cylinder .axis () .direction (), startOffset);
	auto rotation = Rotation4f (fromVector, toVector);

	if (behind and not disk)
		rotation .inverse ();

	rotation = offset * rotation;

	if (minAngle () > maxAngle ())
		rotation_changed () = rotation;

	else
	{
		auto angle = getAngle (rotation);

		if (angle > minAngle () and angle < maxAngle ())
			rotation_changed () = rotation;
	}
}

} // X3D
} // titania
