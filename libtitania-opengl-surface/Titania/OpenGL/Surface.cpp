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

#include "Surface.h"

extern "C"
{
#include <gdk/gdkx.h>
}

#include <gtkmm/container.h>
#include <gtkmm/main.h>

#include "Context/OffScreenContext.h"
#include "Context/Context.h"

#include <Titania/LOG.h>

namespace titania {
namespace OpenGL {

Surface::Surface () :
	Surface (nullptr)
{ }

Surface::Surface (const Surface & sharingSurface) :
	Surface (sharingSurface .sharingContext)
{ }

Surface::Surface (const std::shared_ptr <Context> & sharingContext) :
	   Gtk::DrawingArea (),
	            treadId (std::this_thread::get_id ()),
	            context (),
	     sharingContext (sharingContext),
	   visualAttributes ()
{
	// Enable map_event.
	add_events (Gdk::STRUCTURE_MASK);
	set_double_buffered (false);
	set_app_paintable (true);

	// Create off-screen context.
	setAttributes (0, false);

	if (not sharingContext)
		this -> sharingContext = context;
}

void
Surface::setAttributes (const int32_t antialiasing, const bool accumBuffer)
{
	visualAttributes = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER,     true, 
		GLX_RED_SIZE,         8,
		GLX_GREEN_SIZE,       8,
		GLX_BLUE_SIZE,        8,
		GLX_ALPHA_SIZE,       8,
		GLX_DEPTH_SIZE,       24, 
		GLX_SAMPLE_BUFFERS,   antialiasing ? 1 : 0,
		GLX_SAMPLES,          antialiasing,
	};

	if (accumBuffer)
	{
		visualAttributes .emplace_back (GLX_ACCUM_RED_SIZE);
		visualAttributes .emplace_back (0);

		visualAttributes .emplace_back (GLX_ACCUM_GREEN_SIZE);
		visualAttributes .emplace_back (0);

		visualAttributes .emplace_back (GLX_ACCUM_BLUE_SIZE);
		visualAttributes .emplace_back (0);

		visualAttributes .emplace_back (GLX_ACCUM_ALPHA_SIZE);
		visualAttributes .emplace_back (0);
	}

	visualAttributes .emplace_back (0);

	createContext ();
}

void
Surface::createContext ()
{
	if (get_mapped ())
	{
		context .reset (new Context (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
		                             gdk_x11_window_get_xid (get_window () -> gobj ()),
		                             sharingContext -> getContext (),
		                             true,
		                             visualAttributes));
	}
	else
	{
		context .reset (new OffScreenContext (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
		                                      sharingContext ? sharingContext -> getContext () : None,
		                                      true,
		                                      visualAttributes,
	                                         8,
	                                         8));
	}
}

void
Surface::setSwapInterval (const size_t interval)
{
	context -> setSwapInterval (interval);
}

bool
Surface::makeCurrent () const
{
	return std::this_thread::get_id () == treadId and context and context -> makeCurrent ();
}

void
Surface::swapBuffers () const
{
	context -> swapBuffers ();
}

void
Surface::initialize ()
{
	glewInit ();
}

bool
Surface::on_configure_event (GdkEventConfigure* const event)
{
	Gtk::DrawingArea::on_configure_event (event);

	reshape (math::vector4 <int32_t> (0, 0, get_width (), get_height ()));

	queue_draw ();

	return false;
}

void
Surface::on_map ()
{
	Gtk::DrawingArea::on_map ();

	if (not std::dynamic_pointer_cast <OffScreenContext> (context) and context)
		return;

	createContext ();

	setup ();

	reshape (math::vector4 <int32_t> (0, 0, get_width (), get_height ()));
}

bool
Surface::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	Gtk::DrawingArea::on_draw (cairo);

	update ();

	return false;
}

void
Surface::on_unmap ()
{
	Gtk::DrawingArea::on_unmap ();
}

void
Surface::on_unrealize ()
{
	// Dispose context.

	context        .reset ();
	sharingContext .reset ();

	Gtk::DrawingArea::on_unrealize ();
}

void
Surface::dispose ()
{
	notify_callbacks ();

	// Don't use widget unparent!
	const auto container = get_parent ();

	if (container)
		container -> remove (*this);

	// Dispose context.

	context        .reset ();
	sharingContext .reset ();
}

Surface::~Surface ()
{ }

} // opengl
} // titania

