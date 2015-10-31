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

#include "Browser.h"

#include "../Browser/ContextLock.h"
#include "../Browser/KeyDeviceSensor/KeyDevice.h"
#include "../Browser/Navigation/ExamineViewer.h"
#include "../Browser/Navigation/FlyViewer.h"
#include "../Browser/Navigation/LookAtViewer.h"
#include "../Browser/Navigation/NoneViewer.h"
#include "../Browser/Navigation/PlaneViewer.h"
#include "../Browser/Navigation/WalkViewer.h"
#include "../Browser/Navigation/X3DViewer.h"
#include "../Browser/PointingDeviceSensor/PointingDevice.h"
#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Execution/World.h"

#include <glibmm/main.h>
#include <gtkmm/container.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

namespace titania {
namespace X3D {

Browser::Browser (const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (url, parameter),
	opengl::Surface (),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor (Gdk::X_CURSOR)
{
	addType (X3DConstants::Browser);

	addChildren (viewer,
	             keyDevice,
	             pointingDevice,
	             cursor);
}

Browser::Browser (const Browser & other, const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (url, parameter),
	opengl::Surface (other),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor (Gdk::X_CURSOR)
{
	addType (X3DConstants::Browser);

	addChildren (viewer,
	             keyDevice,
	             pointingDevice,
	             cursor);
}

Browser*
Browser::create (X3DExecutionContext* const) const
{
	return new Browser ({ }, { });
}

void
Browser::initialize ()
{
	X3DBrowser::initialize ();

	//swapInterval (0);

	viewer         -> setup ();
	keyDevice      -> setup ();
	pointingDevice -> setup ();

	getCursor ()        .addInterest (this, &Browser::set_cursor);
	getViewer ()        .addInterest (this, &Browser::set_viewer);
	getPrivateViewer () .addInterest (this, &Browser::set_viewer);
	changed ()          .addInterest (this, &Gtk::Widget::queue_draw);

	add_events (Gdk::BUTTON_PRESS_MASK |
	            Gdk::POINTER_MOTION_MASK |
	            Gdk::POINTER_MOTION_HINT_MASK |
	            Gdk::BUTTON_RELEASE_MASK |
	            Gdk::FOCUS_CHANGE_MASK |
	            Gdk::LEAVE_NOTIFY_MASK |
	            Gdk::SCROLL_MASK |
	            Gdk::KEY_PRESS_MASK |
	            Gdk::KEY_RELEASE_MASK);

	set_can_focus (true);
	grab_focus ();

	setCursor (Gdk::TOP_LEFT_ARROW);
}

void
Browser::on_map ()
{
	opengl::Surface::on_map ();

	if (isLive ())
		getExecutionContext () -> beginUpdate ();

	grab_focus ();
}

void
Browser::on_unmap ()
{
	if (isLive ())
	{
		getClock () -> advance ();

		getExecutionContext () -> endUpdate ();

		ContextLock lock (this);

		if (lock)
			processEvents ();
	}

	opengl::Surface::on_unmap ();
}

void
Browser::set_cursor (const Gdk::CursorType value)
{
	get_window () -> set_cursor (Gdk::Cursor::create (value));
}

void
Browser::set_viewer ()
{
	const auto type = getCurrentViewer ();

	setCursor (Gdk::TOP_LEFT_ARROW);

	if (type not_eq viewer -> getType () .back ())
	{
		switch (type)
		{
			case X3DConstants::NoneViewer:
			{
				viewer .setValue (new NoneViewer (this));
				break;
			}
			case X3DConstants::ExamineViewer:
			{
				viewer .setValue (new ExamineViewer (this));
				break;
			}
			case X3DConstants::WalkViewer:
			{
				viewer .setValue (new WalkViewer (this));
				break;
			}
			case X3DConstants::FlyViewer:
			{
				viewer .setValue (new FlyViewer (this));
				break;
			}
			case X3DConstants::PlaneViewer:
			{
				viewer .setValue (new PlaneViewer (this));
				break;
			}
			case X3DConstants::LookAtViewer:
			{
				viewer .setValue (new LookAtViewer (this));
				break;
			}
			default:
				break;
		}

		viewer -> setup ();
	}
}

void
Browser::reshape ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		X3DBrowser::reshape ();
	}
	catch (const X3D::X3DError &)
	{
		// Send error message, via signal?
	}
}

void
Browser::update ()
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		X3DBrowser::update ();
	}
	catch (const X3D::X3DError &)
	{
		// Send error message, via signal?
	}
}

void
Browser::dispose ()
{
	notify_callbacks ();

	X3DBrowser::dispose ();
	opengl::Surface::dispose ();

	const auto container = get_parent ();

	if (container)
		container -> remove (*this);
}

Browser::~Browser ()
{ }

} // X3D
} // titania
