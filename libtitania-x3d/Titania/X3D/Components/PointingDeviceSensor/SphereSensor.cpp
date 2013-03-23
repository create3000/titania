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

#include "SphereSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

SphereSensor::SphereSensor (X3DExecutionContext* const executionContext) :
	      X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DDragSensorNode (),                                                    
	           offset (0, 1, 0, 0),                                          // SFRotation [in,out] offset            0 1 0 0        [-1,1],(-∞,∞)
	 rotation_changed ()                                                     // SFRotation [out]    rotation_changed
{
	setComponent ("PointingDeviceSensor");
	setTypeName ("SphereSensor");

	addField (inputOutput, "metadata",           metadata);
	addField (inputOutput, "enabled",            enabled);
	addField (inputOutput, "description",        description);
	addField (inputOutput, "autoOffset",         autoOffset);
	addField (inputOutput, "offset",             offset);
	addField (outputOnly,  "trackPoint_changed", trackPoint_changed);
	addField (outputOnly,  "rotation_changed",   rotation_changed);
	addField (outputOnly,  "isActive",           isActive);
	addField (outputOnly,  "isOver",             isOver);
}

X3DBaseNode*
SphereSensor::create (X3DExecutionContext* const executionContext) const
{
	return new SphereSensor (executionContext);
}

/*
 *
 */

void
SphereSensor::set_active (const std::shared_ptr <Hit> & hit, bool active)
{
	X3DDragSensorNode::set_active (hit, active);

	if (isActive)
	{
		inverseTransformationMatrix = ~getTransformationMatrix ();

		auto hitPoint  = hit -> point * inverseTransformationMatrix;
		auto hitCenter = Vector3f ();

		plane  = Plane3f (hitCenter, inverseTransformationMatrix .multDirMatrix (Vector3f (0, 0, 1)));
		sphere = Sphere3f (abs (hitPoint - hitCenter), hitCenter);

		behind = plane .distance (hitPoint) < 0;

		Vector3f trackPoint, intersection2;
		{
			// Calculate trackPoint
			auto hitRay = Line3f (hitPoint, sphere .center ());
			sphere .intersect (hitRay, trackPoint, intersection2);

			if (abs (hitRay .origin () - intersection2) < abs (hitRay .origin () - trackPoint))
				trackPoint = intersection2;
		}

		fromVector         = hitPoint - hitCenter;
		trackPoint_changed = trackPoint;
		rotation_changed   = offset;
		startOffset        = offset;
		startPoint         = hitPoint;
	}
	else
	{
		if (autoOffset)
			offset = rotation_changed;
	}
}

void
SphereSensor::set_motion (const std::shared_ptr <Hit> & hit)
{
	auto hitRay = hit -> ray * inverseTransformationMatrix;

	Vector3f trackPoint, intersection2;

	if (sphere .intersect (hitRay, trackPoint, intersection2))
	{
		if ((abs (hitRay .origin () - intersection2) < abs (hitRay .origin () - trackPoint)) - behind)
			trackPoint = intersection2;

		plane = Plane3f (trackPoint, inverseTransformationMatrix .multDirMatrix (Vector3f (0, 0, 1)));
	}
	else
	{
		// Find trackPoint on the plane with sphere
		
		Vector3f p1;
		plane .intersect (hitRay, p1);

		auto hitRay = Line3f (p1, sphere .center ());
		sphere .intersect (hitRay, trackPoint, intersection2);

		if (abs (hitRay .origin () - intersection2) < abs (hitRay .origin () - trackPoint))
			trackPoint = intersection2;
			
		// Find trackPoint behind sphere

		auto normal = cross (math::normal (sphere .center (), trackPoint, startPoint), normalize (trackPoint - sphere .center ()));
	
		hitRay = Line3f (trackPoint - normal * abs (p1 - trackPoint), sphere .center ());
		
		sphere .intersect (hitRay, trackPoint, intersection2);

		if (abs (hitRay .origin () - intersection2) < abs (hitRay .origin () - trackPoint))
			trackPoint = intersection2;	
	}

	{
		trackPoint_changed = trackPoint;

		auto toVector = trackPoint - sphere .center ();

		rotation_changed = startOffset * Rotation4f (fromVector, toVector);
	}
}

} // X3D
} // titania
