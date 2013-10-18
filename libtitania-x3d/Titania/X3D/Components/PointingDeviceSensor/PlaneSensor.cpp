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

#include "PlaneSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string PlaneSensor::componentName  = "PointingDeviceSensor";
const std::string PlaneSensor::typeName       = "PlaneSensor";
const std::string PlaneSensor::containerField = "children";

PlaneSensor::Fields::Fields () :
	       axisRotation (new SFRotation ()),
	             offset (new SFVec3f ()),
	        minPosition (new SFVec2f ()),
	        maxPosition (new SFVec2f (-1, -1)),
	translation_changed (new SFVec3f ())
{ }

PlaneSensor::PlaneSensor (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DDragSensorNode (),
	                fields (),
	                 plane (),
	           startOffset (),
	            startPoint (),
	inverseModelViewMatrix ()
{
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
}

X3DBaseNode*
PlaneSensor::create (X3DExecutionContext* const executionContext) const
{
	return new PlaneSensor (executionContext);
}

void
PlaneSensor::set_active (const HitPtr & hit, bool active)
{
	try
	{
		X3DDragSensorNode::set_active (hit, active);

		if (isActive ())
		{
			inverseModelViewMatrix = ~getModelViewMatrix ();

			const auto hitRay   = hit -> ray * inverseModelViewMatrix;
			const auto hitPoint = hit -> point * inverseModelViewMatrix;

			plane  = Plane3f (hitPoint, axisRotation () * Vector3f (0, 0, 1));

			Vector3f trackPoint;

			if (plane .intersect (hitRay, trackPoint))
			{
				startPoint             = trackPoint;
				trackPoint_changed ()  = trackPoint;
				translation_changed () = offset ();
				startOffset            = offset ();
			}
		}
		else
		{
			if (autoOffset ())
				offset () = translation_changed ();
		}
	}
	catch (const std::domain_error &)
	{ }
}

Vector3f
PlaneSensor::getTrackPoint (const Line3f hitRay) const
{
	Vector3f trackPoint;
	Vector3f zTrackPoint;

//	float angle = std::abs (dot (plane .normal (), hitRay .direction ()));
//	float alpha = 1 / (1 + std::pow (100, (-12 * angle + 1.5)));
//
//	__LOG__ << angle << " : " << alpha << std::endl;
//
//	Plane3f sPlane (Vector3f (), hitRay .direction ());
//	sPlane .intersect (hitRay, zTrackPoint);
//
//	if (plane .intersect (hitRay, trackPoint))
//		return math::lerp (zTrackPoint, trackPoint, angle);

	return zTrackPoint;
}

void
PlaneSensor::set_motion (const HitPtr & hit)
{
	auto hitRay = hit -> ray * inverseModelViewMatrix;

	Vector3f trackPoint;

	if (plane .intersect (hitRay, trackPoint))
	{
		//auto trackPoint  = getTrackPoint (hitRay);
		auto translation = ~axisRotation () * (startOffset + (trackPoint - startPoint));

		// X component

		if (not (minPosition () .getX () > maxPosition () .getX ()))
			translation .x (math::clamp <float> (translation .x (), minPosition () .getX (), maxPosition () .getX ()));

		// Y component

		if (not (minPosition () .getY () > maxPosition () .getY ()))
			translation .y (math::clamp <float> (translation .y (), minPosition () .getY (), maxPosition () .getY ()));

		// Z component

		translation .z (0);

		trackPoint_changed ()  = trackPoint;
		translation_changed () = axisRotation () * translation;
	}
}

} // X3D
} // titania
