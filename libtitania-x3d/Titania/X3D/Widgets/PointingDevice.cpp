/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "PointingDevice.h"

#include "../Components/Networking/Anchor.h"
#include "../Components/PointingDeviceSensor/X3DPointingDeviceSensorNode.h"
#include "Surface.h"

#define SELECT_BUFFER_SIZE 1024

namespace titania {
namespace X3D {

PointingDevice::PointingDevice (Surface & surface) :
	surface (surface), 
	     button (0),           
	     isOver (false)
{
	surface .signal_button_press_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_press_event),   false);
	surface .signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &PointingDevice::on_motion_notify_event),  false);
	surface .signal_button_release_event () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_release_event), false);
}

const SFNode <X3DBrowser> &
PointingDevice::getBrowser ()
{
	return surface .getBrowser ();
}

bool
PointingDevice::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	if (button == 1)
	{
		surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND1));

		if (pick (event -> x, surface .get_height () - event -> y))
		{
			//return true;
		}
	}

	else if (button == 2)
	{
		surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::FLEUR));
	}

	return false;
}

bool
PointingDevice::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 0)
	{
		if (pick (event -> x, surface .get_height () - event -> y))
		{
			if (not isOver)
			{
				surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));
				isOver = true;
			}

			//return true;
		}
		else
		{
			if (isOver)
			{
				surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
				isOver = false;
			}
		}
	}

	else if (button == 0 or  button == 1)
	{ }

	return false;
}

bool
PointingDevice::on_button_release_event (GdkEventButton* event)
{
	button = 0;

	if (event -> button == 1)
	{
		if (isOver)
		{
			surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));

			if (pick (event -> x, surface .get_height () - event -> y))
			{
				Hit* hit = getBrowser () -> getHits () .front ();

				for (const auto & node : hit -> nodes)
				{
					Anchor* anchor = dynamic_cast <Anchor*> (node);

					if (anchor)
					{
						anchor -> activate ();
						break;
					}

					X3DPointingDeviceSensorNode* pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

					if (pointingDeviceSensorNode)
					{
						pointingDeviceSensorNode -> update ();
						continue;
					}
				}

				//return true;
			}
		}
		else
			surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	}

	else if (event -> button == 2)
	{
		if (isOver)
			surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));
		else
			surface .get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	}

	return false;
}

bool
PointingDevice::pick (const size_t x, const size_t y)
{
	if (not surface .makeCurrent ())
		return false;

	getBrowser () -> pick (x, y);

	return getBrowser () -> getHits () .size ();
}

} // X3D
} // titania
