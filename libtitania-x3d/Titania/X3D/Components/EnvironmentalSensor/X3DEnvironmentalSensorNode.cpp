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

#include "X3DEnvironmentalSensorNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

X3DEnvironmentalSensorNode::Fields::Fields () :
	     size (new SFVec3f ()),
	   center (new SFVec3f ()),
	enterTime (new SFTime ()),
	 exitTime (new SFTime ())
{ }

X3DEnvironmentalSensorNode::X3DEnvironmentalSensorNode () :
	X3DSensorNode (),
	       fields (),
	    traversed (true)
{
	addType (X3DConstants::X3DEnvironmentalSensorNode);

	size ()   .setUnit (UnitCategory::LENGTH);
	center () .setUnit (UnitCategory::LENGTH);
}

void
X3DEnvironmentalSensorNode::initialize ()
{
	X3DSensorNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);
	isLive () .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);

	if (isCameraObject ())
	{
		enabled ()        .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);
		size ()           .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);
		isCameraObject () .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);

		set_enabled ();
	}
}

void
X3DEnvironmentalSensorNode::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
	{
		getBrowser () -> sensorEvents ()    .removeInterest (&X3DEnvironmentalSensorNode::update, this);
		getExecutionContext () -> isLive () .removeInterest (&X3DEnvironmentalSensorNode::set_enabled, this);
	}

	X3DSensorNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&X3DEnvironmentalSensorNode::set_enabled, this);

		set_enabled ();
	}
}

void
X3DEnvironmentalSensorNode::setTraversed (const bool value)
{
   if (value)
		setCameraObject (true);
	else
		setCameraObject (traversed);

   traversed = value;
}

void
X3DEnvironmentalSensorNode::set_enabled ()
{
	if (isCameraObject () and enabled () and size () not_eq Vector3f () and isLive () and getExecutionContext () -> isLive ())
	{
		getBrowser () -> sensorEvents () .addInterest (&X3DEnvironmentalSensorNode::update, this);
	}
	else
	{
		getBrowser () -> sensorEvents () .removeInterest (&X3DEnvironmentalSensorNode::update, this);

		if (isActive ())
		{
			isActive () = false;
			exitTime () = getCurrentTime ();
		}
	}
}

} // X3D
} // titania
