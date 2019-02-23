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

#include "TransformSensor.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Grouping/X3DBoundedObject.h"

namespace titania {
namespace X3D {

const Component   TransformSensor::component      = Component ("EnvironmentalSensor", 3);
const std::string TransformSensor::typeName       = "TransformSensor";
const std::string TransformSensor::containerField = "children";

TransformSensor::Fields::Fields () :
	   position_changed (new SFVec3f ()),
	orientation_changed (new SFRotation ()),
	       targetObject (new SFNode ())
{ }

TransformSensor::TransformSensor (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentalSensorNode (),
	                    fields (),
	          targetObjectNode ()
{
	addType (X3DConstants::TransformSensor);

	addField (inputOutput, "metadata",            metadata ());
	addField (inputOutput, "enabled",             enabled ());
	addField (inputOutput, "size",                size ());
	addField (inputOutput, "center",              center ());
	addField (outputOnly,  "enterTime",           enterTime ());
	addField (outputOnly,  "exitTime",            exitTime ());
	addField (outputOnly,  "isActive",            isActive ());
	addField (outputOnly,  "position_changed",    position_changed ());
	addField (outputOnly,  "orientation_changed", orientation_changed ());
	addField (inputOutput, "targetObject",        targetObject ());

	addChildObjects (targetObjectNode);

	position_changed () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
TransformSensor::create (X3DExecutionContext* const executionContext) const
{
	return new TransformSensor (executionContext);
}

void
TransformSensor::initialize ()
{
	X3DEnvironmentalSensorNode::initialize ();

	enabled ()      .addInterest (&TransformSensor::set_enabled,      this);
	size ()         .addInterest (&TransformSensor::set_enabled,      this);
	targetObject () .addInterest (&TransformSensor::set_targetObject, this);

	set_targetObject ();
}

void
TransformSensor::set_enabled ()
{
	if (enabled () and size () not_eq Vector3f () and targetObjectNode and isLive () and getExecutionContext () -> isLive ())
	{
		getBrowser () -> sensorEvents () .addInterest (&TransformSensor::update, this);
	}
	else
	{
		getBrowser () -> sensorEvents () .removeInterest (&TransformSensor::update, this);
			
		if (isActive ())
		{
			isActive () = false;
			exitTime () = getCurrentTime ();
		}
	}
}

void
TransformSensor::set_targetObject ()
{
	targetObjectNode .set (x3d_cast <X3DBoundedObject*> (targetObject ()));

	set_enabled ();
}

void
TransformSensor::update ()
{
	const auto sourceBox = Box3d (size () .getValue (), center () .getValue ());
	const auto targetBox = targetObjectNode -> getBBox ();

	if (size () == Vector3f (-1, -1, -1) or sourceBox .intersects (targetBox))
	{
		Vector3d   translation;
		Rotation4d rotation;

		targetBox .matrix () .get (translation, rotation);

		if (isActive ())
		{
			if (translation not_eq position_changed ())
				position_changed () = translation;

			if (rotation not_eq orientation_changed ())
				orientation_changed () = rotation;
		}
		else
		{
			isActive ()  = true;
			enterTime () = getCurrentTime ();

			position_changed ()    = translation;
			orientation_changed () = rotation;
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
}

} // X3D
} // titania
