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

#include "X3DPointingDevice.h"

#include "../X3DBrowserSurface.h"

namespace titania {
namespace X3D {

X3DPointingDevice::X3DPointingDevice (X3DBrowserSurface* const browser) :
	sigc::trackable (),
	        browser (browser),
	         button (0),
	         isOver (false)
{ }

void
X3DPointingDevice::connect ()
{
	button_press_conncection   = browser -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_press_event),   false);
	button_release_conncection = browser -> signal_button_release_event () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_release_event), false);
	motion_notify_conncection  = browser -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &PointingDevice::on_motion_notify_event));
	leave_notify_conncection   = browser -> signal_leave_notify_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_leave_notify_event));

	isOver = false;
}

void
X3DPointingDevice::disconnect ()
{
	button_press_conncection   .disconnect ();
	button_release_conncection .disconnect ();
	motion_notify_conncection  .disconnect ();
	leave_notify_conncection   .disconnect ();
}

bool
X3DPointingDevice::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 0 or button == 1)
	{
		bool picked = true;

		if ((picked = pick (event -> x, event -> y)))
		{
			if (haveSensor ())
			{
				if (not isOver)
				{
					browser -> setCursor (Gdk::HAND2);
					isOver = true;
				}

				browser -> motionNotifyEvent ();
				
				return false;
			}
		}
		
		if (isOver)
		{
			browser -> setCursor (Gdk::ARROW);
			isOver = false;
		}

		browser -> motionNotifyEvent ();
		motionNotifyEvent (picked);
	}

	return false;
}

bool
X3DPointingDevice::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	browser -> grab_focus ();

	if (button == 1)
	{
		bool picked = true;
		bool cursor = true;
	
		if ((picked = pick (event -> x, event -> y)))
		{
			if (haveSensor ())
			{
				browser -> buttonPressEvent ();

				browser -> setCursor (Gdk::HAND1);

				return true;
			}
		}

		cursor = buttonPressEvent (picked);

		if (cursor)
			browser -> setCursor (Gdk::FLEUR);
	}

	else if (button == 2)
	{
		browser -> setCursor (Gdk::FLEUR);
	}

	return false;
}

bool
X3DPointingDevice::on_button_release_event (GdkEventButton* event)
{
	button = 0;

	if (event -> button == 1)
	{
		browser -> buttonReleaseEvent ();

		if (not browser -> getHits () .empty ())
		{
			if (not haveSensor ())
				buttonReleaseEvent (true);
		}
		else
			buttonReleaseEvent (false);
			
		// Set cursor

		if (isOver)
			browser -> setCursor (Gdk::HAND2);

		else
			browser -> setCursor (Gdk::ARROW);
	}

	else if (event -> button == 2)
	{
		if (isOver)
			browser -> setCursor (Gdk::HAND2);

		else
			browser -> setCursor (Gdk::ARROW);
	}

	return false;
}

bool
X3DPointingDevice::on_leave_notify_event (GdkEventCrossing*)
{
	browser -> leaveNotifyEvent ();
	leaveNotifyEvent ();

	browser -> setCursor (Gdk::ARROW);
	isOver = false;

	return false;
}

bool
X3DPointingDevice::pick (const double x, const double y)
{
	browser -> pick (x, browser -> get_height () - y);

	return not browser -> getHits () .empty ();
}

bool
X3DPointingDevice::haveSensor ()
{
	return not browser -> getHits () .front () -> sensors .empty ()
	       and trackSensors ();
}

} // X3D
} // titania
