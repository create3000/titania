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
#include "../../Rendering/ViewVolume.h"

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

bool
PlaneSensor::getLineTrackPoint (const HitPtr & hit, const Line3d & line, Vector3d & trackPoint)
throw (std::domain_error)
{
	const auto screenLine     = ViewVolume::projectLine (line, modelViewMatrix, getLastProjectionMatrix (), getLastViewport ());
	auto       trackPoint1    = screenLine .closest_point (Vector3d (hit -> x, hit -> y, 0));
	const auto trackPointLine = ViewVolume::unProjectLine (trackPoint1 .x (), trackPoint1 .y (), modelViewMatrix, getLastProjectionMatrix (), getLastViewport ());

	return line .closest_point (trackPointLine, trackPoint);
}

void
PlaneSensor::set_active (const HitPtr & hit, const bool active)
{
	X3DDragSensorNode::set_active (hit, active);

	try
	{
		if (isActive ())
		{
			modelViewMatrix        = getLastModelViewMatrix ();
			inverseModelViewMatrix = ~getLastModelViewMatrix ();

			const auto hitRay   = hit -> ray * inverseModelViewMatrix;
			const auto hitPoint = hit -> point * inverseModelViewMatrix;

			const Rotation4d axisRotation (this -> axisRotation () .getValue ());

			if (minPosition () .getX () == maxPosition () .getX ())
			{
				planeSensor = false;

				const auto direction = axisRotation * Vector3d (0, std::abs (maxPosition () .getY () - minPosition () .getY ()), 0);

				line = Line3d (hitPoint, normalize (direction));
			}
			else if (minPosition () .getY () == maxPosition () .getY ())
			{
				planeSensor = false;

				const auto direction = axisRotation * Vector3d (std::abs (maxPosition () .getX () - minPosition () .getX ()), 0, 0);

				line = Line3d (hitPoint, normalize (direction));
			}
			else
			{
				planeSensor = true;
				plane       = Plane3d (hitPoint, axisRotation * Vector3d (0, 0, 1));
			}

			if (planeSensor)
			{
				if (plane .intersect (hitRay, startPoint))
				{
					Vector3d trackPoint;

					Plane3d (Vector3d (), plane .normal ()) .intersect (hitRay, trackPoint);

					trackStart (trackPoint);
				}
			}
			else
			{
				if (getLineTrackPoint (hit, line, startPoint))
				{
					Vector3d trackPoint;

					try
					{
						getLineTrackPoint (hit, Line3d (Vector3d (), line .direction ()), trackPoint);
					}
					catch (const std::domain_error &)
					{
						trackPoint = startPoint;
					}

					trackStart (trackPoint);
				}
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

void
PlaneSensor::trackStart (const Vector3d & trackPoint)
{
	startOffset            = offset () .getValue ();
	trackPoint_changed ()  = trackPoint;
	translation_changed () = offset ();
}

void
PlaneSensor::set_motion (const HitPtr & hit)
{
	try
	{
		if (planeSensor)
		{
			const auto hitRay = hit -> ray * inverseModelViewMatrix;

			Vector3d endPoint, trackPoint;

			if (plane .intersect (hitRay, endPoint))
			{
				Plane3d (Vector3d (), plane .normal ()) .intersect (hitRay, trackPoint);

				track (endPoint, trackPoint);
			}
			else
				throw std::domain_error ("Plane and line are parallel.");
		}
		else
		{
			Vector3d endPoint, trackPoint;

			if (getLineTrackPoint (hit, line, endPoint))
			{
				try
				{
					getLineTrackPoint (hit, Line3d (Vector3d (), line .direction ()), trackPoint);
				}
				catch (const std::domain_error &)
				{
					trackPoint = endPoint;
				}
			
				track (endPoint, trackPoint);
			}
			else
				throw std::domain_error ("Lines are parallel.");
		}
	}
	catch (const std::domain_error &)
	{
		trackPoint_changed ()  .addEvent ();
		translation_changed () .addEvent ();
	}
}

void
PlaneSensor::track (const Vector3d & endPoint, const Vector3d & trackPoint)
{
	auto translation = Rotation4d (~axisRotation ()) * (startOffset + endPoint - startPoint);

	// X component

	if (not (minPosition () .getX () > maxPosition () .getX ()))
		translation .x (math::clamp <float> (translation .x (), minPosition () .getX (), maxPosition () .getX ()));

	// Y component

	if (not (minPosition () .getY () > maxPosition () .getY ()))
		translation .y (math::clamp <float> (translation .y (), minPosition () .getY (), maxPosition () .getY ()));

	trackPoint_changed ()  = trackPoint;
	translation_changed () = Rotation4d (axisRotation () .getValue ()) * translation;
}

} // X3D
} // titania
