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

#include "ProximitySensor.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../../Types/Geometry.h"

namespace titania {
namespace X3D {

const Component   ProximitySensor::component      = Component ("EnvironmentalSensor", 2);
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
	             viewpointNode (),
	               modelMatrix (),
	                  position (),
	                    inside (false),
	                 traversed (true)
{
	addType (X3DConstants::ProximitySensor);

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

	centerOfRotation_changed () .setUnit (UnitCategory::LENGTH);
	position_changed ()         .setUnit (UnitCategory::LENGTH);

	setCameraObject (true);
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

	getExecutionContext () -> isLive () .addInterest (&ProximitySensor::set_enabled, this);
	isLive () .addInterest (&ProximitySensor::set_enabled, this);

	enabled ()        .addInterest (&ProximitySensor::set_enabled, this);
	size ()           .addInterest (&ProximitySensor::set_enabled, this);
	isCameraObject () .addInterest (&ProximitySensor::set_enabled, this);

	set_enabled ();
}

void
ProximitySensor::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
	{
		getBrowser () -> sensorEvents ()    .removeInterest (&ProximitySensor::update,      this);
		getExecutionContext () -> isLive () .removeInterest (&ProximitySensor::set_enabled, this);
	}

	X3DEnvironmentalSensorNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&ProximitySensor::set_enabled, this);

		set_enabled ();
	}
}

void
ProximitySensor::setTraversed (const bool value)
{
   if (value)
		setCameraObject (true);
	else
		setCameraObject (traversed);

   traversed = value;
}

void
ProximitySensor::set_enabled ()
{
	if (isCameraObject () and enabled () and size () not_eq Vector3f () and isLive () and getExecutionContext () -> isLive ())
	{
		getBrowser () -> sensorEvents () .addInterest (&ProximitySensor::update, this);
	}
	else
	{
		getBrowser () -> sensorEvents () .removeInterest (&ProximitySensor::update, this);

		if (isActive ())
		{
			isActive () = false;
			exitTime () = getCurrentTime ();
		}
	}
}

void
ProximitySensor::update ()
{
	try
	{
		if (not viewpointNode)
			return;

		if (inside and getTraversed ())
		{
			Matrix4d centerOfRotationMatrix = viewpointNode -> getModelMatrix ();
			centerOfRotationMatrix .translate (viewpointNode -> getUserCenterOfRotation ());
			centerOfRotationMatrix *= inverse (modelMatrix);

			modelMatrix *= viewpointNode -> getInverseCameraSpaceMatrix ();

			Vector3d   translation, scale;
			Rotation4d rotation;
			modelMatrix .get (translation, rotation, scale);

			position = inverse (modelMatrix) .origin ();

			const Rotation4d orientation      = ~rotation;
			const Vector3d   centerOfRotation = centerOfRotationMatrix .origin ();

			if (isActive ())
			{
				if (position_changed () not_eq position)
					position_changed () = position;

				if (orientation_changed () not_eq orientation)
					orientation_changed () = orientation;

				if (centerOfRotation_changed () not_eq centerOfRotation)
					centerOfRotation_changed () = centerOfRotation;
			}
			else
			{
				isActive ()  = true;
				enterTime () = getCurrentTime ();

				position_changed ()         = position;
				orientation_changed ()      = orientation;
				centerOfRotation_changed () = centerOfRotation;
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

		viewpointNode = nullptr;
	}
	catch (const std::domain_error &)
	{ }

	inside = false;

	setTraversed (false);
}

void
ProximitySensor::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	try
	{
		if (not enabled ())
			return;

		if (renderObject -> getBrowser () not_eq getBrowser ())
			return;

		switch (type)
		{
			case TraverseType::CAMERA:
			{
				viewpointNode = renderObject -> getViewpoint ();
				modelMatrix   = renderObject -> getModelViewMatrix () .get ();
				return;
			}
			case TraverseType::DISPLAY:
			{
				setTraversed (true);

				if (inside)
					return;

				if (size () == Vector3f (-1, -1, -1))
				{
					inside = true;
				}
				else
				{
					const auto bbox = Box3d (size () .getValue (), center () .getValue ());

					inside = bbox .intersects (inverse (renderObject -> getModelViewMatrix () .get ()) .origin ());
				}

				return;
			}
			default:
				return;
		}
	}
	catch (const std::domain_error &)
	{ }
}

} // X3D
} // titania
