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

#include "PointingDevice.h"

#include "../Browser/Browser.h"
#include "../Components/Networking/Anchor.h"
#include "../Components/PointingDeviceSensor/X3DPointingDeviceSensorNode.h"

#define SELECT_BUFFER_SIZE 1024

namespace titania {
namespace X3D {

PointingDevice::PointingDevice (Browser* const browser) :
	X3DWidget (browser), 
	   button (0),       
	   isOver (false)    
{
	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_press_event),   false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &PointingDevice::on_motion_notify_event),  false);
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &PointingDevice::on_button_release_event), false);
}

bool
PointingDevice::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	if (button == 1)
	{
		if (pick (event -> x, getBrowser () -> get_height () - event -> y))
		{
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND1));
			return true;
		}
		else
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::FLEUR));
	}

	else if (button == 2)
	{
		getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::FLEUR));
	}

	return false;
}

bool
PointingDevice::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 0 or button == 1)
	{
		if (pick (event -> x, getBrowser () -> get_height () - event -> y))
		{
			if (not isOver)
			{
				getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));
				isOver = true;
			}

			//return true;
		}
		else
		{
			if (isOver)
			{
				getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
				isOver = false;
			}
		}
	}

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
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));

			if (pick (event -> x, getBrowser () -> get_height () - event -> y))
			{
				try
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
				}
				catch (const X3DError & error)
				{
					std::clog << error .what () << std::endl;
				}

				//return true;
			}
		}
		else
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	}

	else if (event -> button == 2)
	{
		if (isOver)
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::HAND2));
		else
			getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::ARROW));
	}

	return false;
}

bool
PointingDevice::pick (const double x, const double y)
{
	if (not getBrowser () -> makeCurrent ())
		return false;

	getBrowser () -> pick (x, y);

	return getBrowser () -> getHits () .size ();
}

} // X3D
} // titania
