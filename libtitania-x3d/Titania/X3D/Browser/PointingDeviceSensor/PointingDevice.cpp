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

#include "../Browser.h"

#include "PointingDevice.h"

#include "../Selection.h"

namespace titania {
namespace X3D {

PointingDevice::PointingDevice (Browser* const browser) :
	          X3DBrowserObject (browser),
	  button_press_conncection (),
	button_release_conncection (),
	 motion_notify_conncection (),
	  leave_notify_conncection (),
	                    button (0),
	                    isOver (false)
{ }

void
PointingDevice::initialize ()
{
	X3DBrowserObject::initialize ();

	getBrowser () -> isSensitive () .addInterest (this, &PointingDevice::set_sensitive);

	set_sensitive ();
}

void
PointingDevice::set_sensitive ()
{
	if (getBrowser () -> isSensitive ())
	{
		button_press_conncection   = getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_press_event),   false);
		button_release_conncection = getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_release_event), false);
		motion_notify_conncection  = getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &PointingDevice::on_motion_notify_event));
		leave_notify_conncection   = getBrowser () -> signal_leave_notify_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_leave_notify_event));
	}
	else
	{
		button_press_conncection   .disconnect ();
		button_release_conncection .disconnect ();
		motion_notify_conncection  .disconnect ();
		leave_notify_conncection   .disconnect ();

		getBrowser () -> buttonReleaseEvent ();
		getBrowser () -> leaveNotifyEvent ();
		getBrowser () -> setCursor (Gdk::ARROW);
	}
}

bool
PointingDevice::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 0 or button == 1)
		set_motion (event -> x, event -> y);

	return false;
}

void
PointingDevice::set_motion (const double x, const double y)
{
	if (getBrowser () -> motionNotifyEvent (x, getBrowser () -> get_height () - y))
	{
		if (not isOver)
		{
			getBrowser () -> setCursor (Gdk::HAND2);
			isOver = true;
		}
	}
	else
	{
		if (isOver)
		{
			getBrowser () -> setCursor (Gdk::ARROW);
			isOver = false;
		}
	}
}

void
PointingDevice::set_verify_motion (const double x, const double y)
{
	// Veryfy isOver state. This is neccessay if an Switch changes on buttonReleaseEvent
	// and the new child has a sensor node inside. This sensor node must be update to
	// reflect the correct isOver state.

	getBrowser () -> finished () .removeInterest (this, &PointingDevice::set_verify_motion);
	set_motion (x, y);
}

bool
PointingDevice::on_button_press_event (GdkEventButton* event)
{
	getBrowser () -> grab_focus ();

	if (event -> type not_eq GDK_BUTTON_PRESS)
		return false;

	if (getBrowser () -> hasShiftKey () and getBrowser () -> hasControlKey ())
		return false;

	button = event -> button;

	switch (button)
	{
		case 1:
		{
			if (getBrowser () -> buttonPressEvent (event -> x, getBrowser () -> get_height () - event -> y))
			{
				getBrowser () -> setCursor (Gdk::HAND1);
				getBrowser () -> finished () .addInterest (this, &PointingDevice::set_verify_motion, event -> x, event -> y);

				return true;
			}

			getBrowser () -> setCursor (Gdk::FLEUR);
			break;
		}
		case 2:
		{
			//const bool clicked = click (event -> x, event -> y);

			getBrowser () -> setCursor (Gdk::FLEUR);
			break;
		}
		default:
			break;
	}

	return false;
}

bool
PointingDevice::on_button_release_event (GdkEventButton* event)
{
	if (event -> button not_eq button)
		return false;

	switch (button)
	{
		case 1:
		{
			getBrowser () -> buttonReleaseEvent ();
			getBrowser () -> finished () .addInterest (this, &PointingDevice::set_verify_motion, event -> x, event -> y);

			// Set cursor

			if (isOver)
				getBrowser () -> setCursor (Gdk::HAND2);

			else
				getBrowser () -> setCursor (Gdk::ARROW);

			break;
		}
		case 2:
		{
			if (isOver)
				getBrowser () -> setCursor (Gdk::HAND2);

			else
				getBrowser () -> setCursor (Gdk::ARROW);

			break;
		}
		default:
			break;
	}

	button = 0;
	return false;
}

bool
PointingDevice::on_leave_notify_event (GdkEventCrossing*)
{
	getBrowser () -> leaveNotifyEvent ();
	getBrowser () -> setCursor (Gdk::ARROW);

	isOver = false;
	return false;
}

} // X3D
} // titania
