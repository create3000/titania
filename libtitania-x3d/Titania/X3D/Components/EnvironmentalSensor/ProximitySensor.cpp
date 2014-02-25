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

#include "ProximitySensor.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Types/Geometry.h"

#include <iostream>

namespace titania {
namespace X3D {

const std::string ProximitySensor::componentName  = "EnvironmentalSensor";
const std::string ProximitySensor::typeName       = "ProximitySensor";
const std::string ProximitySensor::containerField = "children";

ProximitySensor::Fields::Fields () :
	centerOfRotation_changed (new SFVec3f ()),
	     orientation_changed (new SFRotation ()),
	        position_changed (new SFVec3f ())
{ }

ProximitySensor::ProximitySensor (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentalSensorNode (),
	                    fields (),
	                 viewpoint (NULL),
	           modelViewMatrix (),
	                    inside (false)
{
	addField (inputOutput, "metadata",                 metadata ());
	addField (inputOutput, "enabled",                  enabled ());
	addField (inputOutput, "size",                     size ());
	addField (inputOutput, "center",                   center ());
	addField (outputOnly,  "enterTime",                enterTime ());
	addField (outputOnly,  "exitTime",                 exitTime ());
	addField (outputOnly,  "isActive",                 isActive ());
	addField (outputOnly,  "position_changed",         position_changed ());
	addField (outputOnly,  "orientation_changed",      orientation_changed ());
	addField (outputOnly,  "centerOfRotation_changed", centerOfRotation_changed ());
}

X3DBaseNode*
ProximitySensor::create (X3DExecutionContext* const executionContext) const
{
	return new ProximitySensor (executionContext);
}

void
ProximitySensor::initialize ()
{
	X3DEnvironmentalSensorNode::initialize ();

	enabled () .addInterest (this, &ProximitySensor::set_enabled);

	set_enabled ();
}

void
ProximitySensor::set_enabled ()
{
	if (enabled ())
		getBrowser () -> sensors () .addInterest (this, &ProximitySensor::update);

	else
		getBrowser () -> sensors () .removeInterest (this, &ProximitySensor::update);
}

void
ProximitySensor::update ()
{
	try
	{
		if (inside)
		{
			modelViewMatrix *= viewpoint -> getInverseTransformationMatrix ();

			Vector3f   translation, scale;
			Rotation4f rotation;
			modelViewMatrix .get (translation, rotation, scale);

			const Vector3f   position    = inverse (modelViewMatrix) .origin ();
			const Rotation4f orientation = ~rotation;

			if (not isActive ())
			{
				isActive ()  = true;
				enterTime () = getCurrentTime ();

				position_changed ()    = position;
				orientation_changed () = orientation;
			}
			else
			{
				if (position_changed () not_eq position)
					position_changed () = position;

				if (orientation_changed () not_eq orientation)
					orientation_changed () = orientation;
			}
		}
		else
		{
			if (isActive ())
			{
				isActive () = false;
				exitTime () = getCurrentTime ();
			}
		}

		inside = false;
	}
	catch (const std::domain_error &)
	{ }
}

void
ProximitySensor::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		{
			viewpoint       = getCurrentViewpoint ();
			modelViewMatrix = getModelViewMatrix () .get ();
			break;
		}
		case TraverseType::COLLECT:
		{
			if (inside)
				break;

			if (size () == Vector3f (-1, -1, -1))
				inside = true;

			else
				inside = Box3f (size (), center ()) .intersect (inverse (getModelViewMatrix () .get ()) .origin ());

			break;
		}
		default:
			break;
	}
}

} // X3D
} // titania
