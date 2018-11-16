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

#include "SphereSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType SphereSensor::component      = ComponentType::POINTING_DEVICE_SENSOR;
const std::string   SphereSensor::typeName       = "SphereSensor";
const std::string   SphereSensor::containerField = "children";

SphereSensor::Fields::Fields () :
	          offset (new SFRotation (0, 1, 0, 0)),
	rotation_changed (new SFRotation ())
{ }

SphereSensor::SphereSensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DDragSensorNode (),
	                fields (),
	                sphere (),
	                zPlane (),
	                behind (false),
	            fromVector (),
	            startPoint (),
	           startOffset (),
	inverseModelViewMatrix ()
{
	addType (X3DConstants::SphereSensor);

	addField (inputOutput, "metadata",           metadata ());
	addField (inputOutput, "enabled",            enabled ());
	addField (inputOutput, "description",        description ());
	addField (inputOutput, "autoOffset",         autoOffset ());
	addField (inputOutput, "offset",             offset ());
	addField (outputOnly,  "trackPoint_changed", trackPoint_changed ());
	addField (outputOnly,  "rotation_changed",   rotation_changed ());
	addField (outputOnly,  "isOver",             isOver ());
	addField (outputOnly,  "isActive",           isActive ());
}

X3DBaseNode*
SphereSensor::create (X3DExecutionContext* const executionContext) const
{
	return new SphereSensor (executionContext);
}

std::pair <Vector3d, bool>
SphereSensor::getTrackPoint (const Line3d & hitRay, const bool behind) const
{
	const auto & [enter, exit, intersected] = sphere .intersects (hitRay);

	if (not intersected)
		return std::pair (Vector3d (), false);

	if ((abs (hitRay .point () - exit) < abs (hitRay .point () - enter)) - behind)
		return std::pair (exit, true);

	return std::pair (enter, true);
}

void
SphereSensor::set_active (const bool active,
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

			const auto hitPoint = Vector3d (hit -> getIntersection () -> getPoint ()) * inverseModelViewMatrix;
			const auto center   = Vector3d ();

			zPlane = Plane3d (center, normalize (inverseModelViewMatrix .mult_dir_matrix (Vector3d (0, 0, 1)))); // Screen aligned Z-plane
			sphere = Sphere3d (abs (hitPoint - center), center);
			behind = zPlane .distance (hitPoint) < 0;

			fromVector  = hitPoint - sphere .center ();
			startPoint  = hit -> getIntersection () -> getPoint ();
			startOffset = offset () .getValue ();

			trackPoint_changed () .setValue (hitPoint);
			rotation_changed ()   .setValue (offset ());
		}
		else
		{
			if (autoOffset ())
				offset () = rotation_changed ();
		}
	}
	catch (const std::exception &)
	{ }
}

void
SphereSensor::set_motion (const HitPtr & hit,
                          const Matrix4d & modelViewMatrix,
                          const Matrix4d & projectionMatrix,
                          const Vector4i & viewport)
{
	try
	{
		X3DDragSensorNode::set_motion (hit, modelViewMatrix, projectionMatrix, viewport);

		auto         hitRay                      = hit -> getHitRay () * inverseModelViewMatrix;
		const auto   startPoint                  = this -> startPoint * inverseModelViewMatrix;
		const auto & [intersection, intersected] = getTrackPoint (hitRay, behind);

		Vector3d trackPoint;

		if (intersected)
		{
			const auto zAxis = normalize (inverseModelViewMatrix .mult_dir_matrix (Vector3d (0, 0, 1))); // Camera direction

			trackPoint = intersection;
			zPlane     = Plane3d (trackPoint, zAxis); // Screen aligned Z-plane
		}
		else
		{
			// Find trackPoint on the plane with sphere

			const auto tangentPoint = zPlane .intersects (hitRay) .first;

			hitRay = Line3d (tangentPoint, sphere .center (), points_type ());

			const auto & [trackPoint1, intersected] = getTrackPoint (hitRay);

			// Find trackPoint behind sphere

			const auto triNormal     = Triangle3d (sphere .center (), trackPoint1, startPoint) .normal ();
			const auto dirFromCenter = normalize (trackPoint1 - sphere .center ());
			const auto normal        = normalize (cross (triNormal, dirFromCenter));

			hitRay     = Line3d (trackPoint1 - normal * abs (tangentPoint - trackPoint1), sphere .center (), points_type ());
			trackPoint = getTrackPoint (hitRay) .first;
		}

		const auto toVector = trackPoint - sphere .center ();
		auto       rotation = Rotation4d (fromVector, toVector);

		if (behind)
			rotation .inverse ();

		trackPoint_changed () = trackPoint;
		rotation_changed ()   = startOffset * rotation;
	}
	catch (const std::domain_error &)
	{ }
}

} // X3D
} // titania
