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

#include "../Browser.h"

#include "PointingDevice.h"

namespace titania {
namespace X3D {

const std::string PointingDevice::componentName  = "Titania";
const std::string PointingDevice::typeName       = "PointingDevice";
const std::string PointingDevice::containerField = "pointingDevice";

PointingDevice::PointingDevice (X3DExecutionContext* const executionContext) :
		            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	           sigc::trackable (),
	  button_press_conncection (),
	button_release_conncection (),
	 motion_notify_conncection (),
	  leave_notify_conncection (),
                       cursor ("ARROW"),
	                    button (0),
	                    isOver (false)
{ }

X3DBaseNode*
PointingDevice::create (X3DExecutionContext* const executionContext) const
{
	return new PointingDevice (executionContext);
}

void
PointingDevice::initialize ()
{
	X3DBaseNode::initialize ();

	getBrowser () -> initialized () .addInterest (&PointingDevice::set_initialized, this);
	getBrowser () -> getPickable () .addInterest (&PointingDevice::set_pickable, this);

	set_pickable ();
}

void
PointingDevice::set_initialized ()
{
	button = 0;
	isOver = false;
}

void
PointingDevice::set_pickable ()
{
	// Always disconnect.

	{
		isOver = false;

		button_press_conncection   .disconnect ();
		button_release_conncection .disconnect ();
		motion_notify_conncection  .disconnect ();
		leave_notify_conncection   .disconnect ();

		getBrowser () -> setButtonReleaseEvent (-1000, -1000);
		getBrowser () -> setLeaveNotifyEvent (-1000, -1000);
		getBrowser () -> setCursor ("ARROW");
	}

	// Connect.

	if (getBrowser () -> getPickable ())
	{
		button_press_conncection   = getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (this, &PointingDevice::on_button_press_event), false);
		button_release_conncection = getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (this, &PointingDevice::on_button_release_event));
		motion_notify_conncection  = getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (this, &PointingDevice::on_motion_notify_event));
		leave_notify_conncection   = getBrowser () -> signal_leave_notify_event   () .connect (sigc::mem_fun (this, &PointingDevice::on_leave_notify_event));
	}
}

bool
PointingDevice::on_motion_notify_event (GdkEventMotion* event)
{
	set_motion (event -> x, event -> y);
	return false;
}

void
PointingDevice::set_motion (const double x, const double y)
{
	if (getBrowser () -> setMotionNotifyEvent (x, getBrowser () -> get_height () - y))
	{
		if (not isOver)
		{
			isOver = true;
			cursor = getBrowser () -> getCursor ();
			getBrowser () -> setCursor (button == 1 ? "GRABBING" : "GRAB");
		}
	}
	else
	{
		if (isOver)
		{
			isOver = false;
			getBrowser () -> setCursor (button == 1 ? "GRABBING" : cursor);
		}
	}
}

void
PointingDevice::set_verify_motion (const double x, const double y)
{
	// Veryfy isOver state. This is neccessay if an Switch changes on buttonReleaseEvent
	// and the new child has a sensor node inside. This sensor node must be update to
	// reflect the correct isOver state.

	getBrowser () -> finished () .removeInterest (&PointingDevice::set_verify_motion, this);

	set_motion (x, y);
}

bool
PointingDevice::on_button_press_event (GdkEventButton* event)
{
	getBrowser () -> grab_focus (); // remove if gtkmm 3.20 and see Browser initialize

	// Dont't handle double click events.
	// Prevents jump issue if click come heavily.
	if (event -> type not_eq GDK_BUTTON_PRESS)
		return true;

	if (getBrowser () -> getShiftKey () and getBrowser () -> getControlKey ())
		return false;

	button = event -> button;

	if (button == 1)
	{
		if (getBrowser () -> setButtonPressEvent (event -> x, getBrowser () -> get_height () - event -> y))
		{
			getBrowser () -> setCursor ("GRABBING");
			getBrowser () -> finished () .addInterest (&PointingDevice::set_verify_motion, this, event -> x, event -> y);
			return true;
		}
	}

	return false;
}

bool
PointingDevice::on_button_release_event (GdkEventButton* event)
{
	if (button == 1)
		getBrowser () -> setButtonReleaseEvent (event -> x, getBrowser () -> get_height () - event -> y);

	if (isOver)
		getBrowser () -> setCursor ("GRAB");
	else
		getBrowser () -> setCursor ("ARROW");

	getBrowser () -> finished () .addInterest (&PointingDevice::set_verify_motion, this, event -> x, event -> y);
	getBrowser () -> addEvent ();

	button = 0;
	cursor = "ARROW";
	return false;
}

bool
PointingDevice::on_leave_notify_event (GdkEventCrossing* event)
{
	getBrowser () -> setLeaveNotifyEvent (event -> x, getBrowser () -> get_height () - event -> y);
	return false;
}

void
PointingDevice::dispose ()
{
	notify_callbacks ();

	X3DBaseNode::dispose ();
}

PointingDevice::~PointingDevice ()
{ }

} // X3D
} // titania
