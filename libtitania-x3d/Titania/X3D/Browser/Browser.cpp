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
#include "../Browser/Rendering/BackgroundTexture.h"
#include "../Browser/Tools/LightSaber.h"
#include "../Browser/Tools/LassoSelection.h"
#include "../Browser/Tools/RectangleSelection.h"
#include "../Execution/World.h"

#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Layering/X3DLayerNode.h"
#include "../Components/Navigation/NavigationInfo.h"

#include <glibmm/main.h>
#include <gtkmm/container.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>

#include "../Fields/X3DWeakPtrArray.h"

namespace titania {
namespace X3D {

Browser::Browser (const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (nullptr, url, parameter),
	OpenGL::Surface (),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor ("default"),
	     background (new BackgroundTexture (this))
{
	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor,
	                 background);

	setAntialiasing (0);
}

Browser::Browser (const BrowserPtr & sharedBrowser, const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (sharedBrowser, url, parameter),
	OpenGL::Surface (*sharedBrowser),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor ("default"),
	     background (new BackgroundTexture (this))
{
	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor,
	                 background);

	setAntialiasing (0);
}

Browser*
Browser::create (X3DExecutionContext* const executionContext) const
{
	return new Browser ({ }, { });
}

void
Browser::initialize ()
{
	try
	{
		ContextLock lock (this);

		X3DBrowser::initialize ();

		get_style_context () -> add_class ("background");
		get_style_context () -> add_class ("titania-surface");

		property_opacity () .signal_changed () .connect (sigc::mem_fun (this, &Browser::on_opacity));

		//swapInterval (0);

		background -> setOpacity (get_opacity ());
		background -> setSize (get_width (), get_height ());
		background -> setStyleContext (get_style_context ());

		viewer         -> setup ();
		keyDevice      -> setup ();
		pointingDevice -> setup ();
		background     -> setup ();

		getCursor ()        .addInterest (&Browser::set_cursor, this);
		getViewerType ()    .addInterest (&Browser::set_viewer, this);
		getPrivateViewer () .addInterest (&Browser::set_viewer, this);

		add_events (Gdk::BUTTON_PRESS_MASK |
		            Gdk::POINTER_MOTION_MASK |
		            Gdk::POINTER_MOTION_HINT_MASK |
		            Gdk::BUTTON_RELEASE_MASK |
		            Gdk::FOCUS_CHANGE_MASK |
		            Gdk::LEAVE_NOTIFY_MASK |
		            Gdk::SCROLL_MASK |
		            Gdk::KEY_PRESS_MASK |
		            Gdk::KEY_RELEASE_MASK);

		//set_focus_on_click (true);
		set_can_focus (true);
		setCursor ("default");

		if (get_mapped ())
		{
			changed () .addInterest (&Browser::set_idle, this);
		}
		else
		{
			changed () .addInterest (&Browser::set_timeout, this);
			update ();
		}
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Browser::setAntialiasing (const int32_t samples)
noexcept (true)
{
	OpenGL::Surface::setAttributes (samples, true);
}

bool
Browser::makeCurrent ()
noexcept (true)
{
	return OpenGL::Surface::makeCurrent ();
}

void
Browser::swapBuffers ()
noexcept (true)
{
	OpenGL::Surface::swapBuffers ();
}

void
Browser::reshape (const Vector4i & viewport)
noexcept (true)
{
	X3DBrowser::reshape (viewport);

	if (not isInitialized ())
		return;

	background -> setSize (viewport [2], viewport [3]);
}

void
Browser::renderBackground ()
{
	if (getAlphaChannel () .top ())
		X3DRenderingContext::renderBackground ();
	else
		background -> renderBackground ();
}

void
Browser::renderForeground ()
{
	if (get_opacity () >= 1)
		return;

	if (getAlphaChannel () .top ())
		return;

	background -> renderForeground ();
}

void
Browser::on_style_updated ()
{
	OpenGL::Surface::on_style_updated ();

	if (not isInitialized ())
		return;

	background -> setStyleContext (get_style_context ());
}

void
Browser::on_opacity ()
{
	background -> setOpacity (get_opacity ());
}

void
Browser::on_map ()
{
	OpenGL::Surface::on_map ();

	queue_draw ();
	set_cursor (cursor);

	if (not isInitialized ())
		setup ();

	reshape (Vector4i (0, 0, get_width (), get_height ()));

	getExecutionContext () -> beginUpdate ();
}

bool
Browser::on_configure_event (GdkEventConfigure* const event)
{
	OpenGL::Surface::on_configure_event (event);

	reshape (Vector4i (0, 0, get_width (), get_height ()));

	queue_draw ();

	return false;
}

bool
Browser::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	OpenGL::Surface::on_draw (cairo);

	update ();

	return false;
}

void
Browser::on_unmap ()
{
	OpenGL::Surface::on_unmap ();

	if (not isLive ())
		return;

	getExecutionContext () -> endUpdate ();

	queue_draw ();
}

void
Browser::set_idle ()
{
	Glib::signal_idle () .connect_once (sigc::mem_fun (this, &Gtk::Widget::queue_draw), Glib::PRIORITY_DEFAULT_IDLE);
}

void
Browser::set_timeout ()
{
	Glib::signal_timeout () .connect_once (sigc::mem_fun (this, &Browser::update), 1000 / 60, Glib::PRIORITY_DEFAULT_IDLE);
}

void
Browser::set_cursor (const String & value)
{
	if (not get_mapped ())
		return;

	get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), value));
}

void
Browser::set_viewer ()
{
	const auto type = getCurrentViewer ();

	setCursor ("default");

	viewer .addEvent ();

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
			case X3DConstants::RectangleSelection:
			{
				viewer .setValue (new RectangleSelection (this));
				break;
			}
			case X3DConstants::LassoSelection:
			{
				viewer .setValue (new LassoSelection (this));
				break;
			}
			case X3DConstants::LightSaber:
			{
				viewer .setValue (new LightSaber (this));
				break;
			}
			default:
				viewer .setValue (new NoneViewer (this));
				break;
		}

		viewer -> setup ();
	}
}

void
Browser::dispose ()
{
	X3DBrowser::dispose ();
	OpenGL::Surface::dispose ();
}

Browser::~Browser ()
{ }

} // X3D
} // titania
