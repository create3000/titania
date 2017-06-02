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
#include "../Browser/Tools/LightSaber.h"
#include "../Browser/Tools/LassoSelection.h"
#include "../Browser/Tools/RectangleSelection.h"
#include "../Execution/World.h"
#include "../Rendering/FrameBuffer.h"

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
	    frameBuffer (new FrameBuffer (this, 1, 1, 0, true)),
	  render_signal (),
	     connection ()
{
	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor);

	setAccumBuffer (true);
}

Browser::Browser (const BrowserPtr & sharedBrowser, const MFString & url, const MFString & parameter) :
	    X3DBaseNode (this, this),
	     X3DBrowser (sharedBrowser, url, parameter),
	OpenGL::Surface (*sharedBrowser),
	        viewer  (new NoneViewer (this)),
	      keyDevice (new KeyDevice (this)),
	pointingDevice  (new PointingDevice (this)),
	         cursor ("default"),
	    frameBuffer (new FrameBuffer (this, 1, 1, 0, true)),
	  render_signal (),
	     connection ()
{
	addType (X3DConstants::Browser);

	addChildObjects (viewer,
	                 keyDevice,
	                 pointingDevice,
	                 cursor);

	setAccumBuffer (true);
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

		//swapInterval (0);

		viewer         -> setup ();
		keyDevice      -> setup ();
		pointingDevice -> setup ();
		frameBuffer    -> setup ();

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

		set_focus_on_click (true);
		set_can_focus (true);
		setCursor ("default");

		// As last command connect.
		changed () .addInterest (&Browser::queue_render, this);
		queue_render ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
Browser::makeCurrent ()
noexcept (true)
{
	return OpenGL::Surface::makeCurrent ();
}

void
Browser::queue_render ()
{
	connection .disconnect ();

	connection = Glib::signal_timeout () .connect (sigc::mem_fun (this, &Browser::on_timeout), 1000 / 60, Glib::PRIORITY_DEFAULT_IDLE);
}

void
Browser::on_realize ()
{
	try
	{
		OpenGL::Surface::on_realize ();
		
		ContextLock lock (this);

		setup ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Browser::on_map ()
{
	try
	{
		OpenGL::Surface::on_map ();

		set_cursor (cursor);

		ContextLock lock (this);

		frameBuffer -> bind ();

		on_reshape (Vector4i (0, 0, get_width (), get_height ()));
		on_timeout ();

		frameBuffer -> unbind ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
Browser::on_configure_event (GdkEventConfigure* const event)
{
	try
	{
		OpenGL::Surface::on_configure_event (event);

		if (not get_mapped ())
			return false;

		ContextLock lock (this);

		frameBuffer .reset (new FrameBuffer (this, get_width (), get_height (), getAntialiasing (), true)),
		frameBuffer -> setup ();
		frameBuffer -> bind ();

		on_reshape (Vector4i (0, 0, get_width (), get_height ()));
		on_timeout ();

		frameBuffer -> unbind ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	return false;
}

void
Browser::on_reshape (const Vector4i & viewport)
{
	reshape (viewport);
}

bool
Browser::on_timeout ()
{
	try
	{
		ContextLock lock (this);

		frameBuffer -> bind ();

		on_render ();
		render_signal .emit ();

		frameBuffer -> unbind ();

		queue_draw ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	return false;
}

bool
Browser::on_render ()
{
	update ();
	return false;
}

bool
Browser::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	try
	{
		OpenGL::Surface::on_draw (cairo);

		ContextLock lock (this);

		frameBuffer -> bind ();
		frameBuffer -> readPixels (GL_BGRA);
		frameBuffer -> unbind ();

		const auto surface = Cairo::ImageSurface::create (const_cast <uint8_t*> (frameBuffer -> getPixels () .data ()),
		                                                  Cairo::FORMAT_ARGB32,
		                                                  frameBuffer -> getWidth (),
		                                                  frameBuffer -> getHeight (),
		                                                  cairo_format_stride_for_width (CAIRO_FORMAT_ARGB32, frameBuffer -> getWidth ()));

		get_style_context () -> render_background (cairo, 0, 0, get_width (), get_height ());

		cairo -> save ();
		cairo -> set_operator (Cairo::OPERATOR_OVER);
		cairo -> set_source (surface, 0, 0);
		cairo -> get_source () -> set_matrix (Cairo::Matrix (1, 0, 0, -1, 0, frameBuffer -> getHeight ()));
		cairo -> paint_with_alpha (get_opacity ());
		cairo -> restore ();

		get_style_context () -> render_frame (cairo, 0, 0, get_width (), get_height ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}

	return false;
}

void
Browser::on_unmap ()
{
	OpenGL::Surface::on_unmap ();
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
				viewer .setValue (new PlaneViewer (this, X3DConstants::PlaneViewer));
				break;
			}
			case X3DConstants::PlaneViewer3D:
			{
				viewer .setValue (new PlaneViewer (this, X3DConstants::PlaneViewer3D));
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
	connection .disconnect ();

	X3DBrowser::dispose ();
	OpenGL::Surface::dispose ();
}

Browser::~Browser ()
{ }

} // X3D
} // titania
