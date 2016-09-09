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

#include "Context/WindowContext.h"
#include "Context/Background.h"

#include <Titania/LOG.h>
#include <stdexcept>


namespace titania {
namespace opengl {

Surface::Surface (const std::shared_ptr <WindowContext> & sharingContext) :
	   Gtk::DrawingArea (),
	            treadId (std::this_thread::get_id ()),
	            context (),
	     sharingContext (sharingContext),
	         background (new Background ()),
	      mapConnection (),
	constructConnection (),
	     drawConnection ()
{
	get_style_context () -> add_class ("background");
	get_style_context () -> add_class ("titania-surface");
	set_double_buffered (false);
	set_app_paintable (true);

	// Enable map_event.
	add_events (Gdk::STRUCTURE_MASK);

	// Connect to map_event.
	mapConnection = signal_map_event () .connect (sigc::mem_fun (*this, &Surface::set_map_event));
	signal_unrealize () .connect (sigc::mem_fun (*this, &Surface::dispose));
}

Surface::Surface (const Surface & other) :
	Surface (other .sharingContext ? other .sharingContext : other .context)
{ }

void
Surface::setAntialiasing (const int32_t samples)
{
	static
	int32_t visualAttributes [ ] = {
		//		GLX_X_RENDERABLE,     true,
		//		GLX_DRAWABLE_TYPE,    GLX_WINDOW_BIT,
		GLX_X_VISUAL_TYPE,    GLX_TRUE_COLOR,
		//GLX_TRANSPARENT_TYPE, GLX_TRANSPARENT_RGB,
		//GLX_RENDER_TYPE,      GLX_RGBA_BIT,
		GLX_RGBA,
		GLX_RED_SIZE,         8,
		GLX_GREEN_SIZE,       8,
		GLX_BLUE_SIZE,        8,
		GLX_ALPHA_SIZE,       8,       // zero
		GLX_ACCUM_RED_SIZE,   8,
		GLX_ACCUM_GREEN_SIZE, 8,
		GLX_ACCUM_BLUE_SIZE,  8,
		GLX_ACCUM_ALPHA_SIZE, 8,
		GLX_DOUBLEBUFFER,     true,
		GLX_DEPTH_SIZE,       24,
		GLX_SAMPLE_BUFFERS,   1,       // Multisampling
		GLX_SAMPLES,          samples, // 4 x Antialiasing
		0
	};

	GdkScreen* screen  = gdk_screen_get_default ();
	gint       nscreen = GDK_SCREEN_XNUMBER (screen);

	auto display = gdk_x11_get_default_xdisplay ();

	XVisualInfo* visualInfo = glXChooseVisual (display, nscreen, visualAttributes);

	if (visualInfo)
	{
		GdkVisual* visual = gdk_x11_screen_lookup_visual (screen, visualInfo -> visualid);

		if (visual)
			gtk_widget_set_visual (GTK_WIDGET (gobj ()), visual);

		XFree (visualInfo);
	}
}

void
Surface::renderBackground ()
{
	background -> draw ();
}

bool
Surface::glew ()
{
	// Initilaize GLEW.

	GLenum glewErrorNum = glewInit ();

	if (glewErrorNum not_eq GLEW_OK)
		throw std::runtime_error (std::string ("Error in glew init: ") + (char*) glewGetErrorString (glewErrorNum));

	if (not GLEW_ARB_vertex_buffer_object)
		throw std::runtime_error ("The glew vertex buffer objects are not supported.");

	return true;
}

bool
Surface::makeCurrent () const
{
	return std::this_thread::get_id () == treadId and context and context -> makeCurrent ();
}

void
Surface::setSwapInterval (const size_t interval)
{
	context -> setSwapInterval (interval);
}

void
Surface::swapBuffers () const
{
	context -> swapBuffers ();
}

bool
Surface::set_map_event (GdkEventAny* const event)
{
	mapConnection .disconnect ();

	if (sharingContext)
	{
		context .reset (new WindowContext (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
		                                   gdk_x11_window_get_xid (get_window () -> gobj ()),
		                                   *sharingContext));
	}
	else
	{
		context .reset (new WindowContext (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
		                                   gdk_x11_window_get_xid (get_window () -> gobj ())));
	}

	if (makeCurrent ())
	{
		signal_configure_event () .connect (sigc::mem_fun (*this, &Surface::set_configure_event));

		constructConnection = signal_draw () .connect (sigc::mem_fun (*this, &Surface::set_construct));

		glewInit ();
	}

	return false; // Propagate the event further.
}

void
Surface::on_style_updated ()
{
	Gtk::DrawingArea::on_style_updated ();

	if (makeCurrent ())
	{
		if (background)
		   background -> configure (get_style_context (), get_width (), get_height ());
	}
}

bool
Surface::set_configure_event (GdkEventConfigure* const event)
{
	if (makeCurrent ())
	{
		background -> configure (get_style_context (), get_width (), get_height ());

		reshape (math::vector4 <int32_t> (0, 0, get_width (), get_height ()));
	}

	queue_draw ();

	return false; // Propagate the event further.
}

bool
Surface::set_construct (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	constructConnection .disconnect ();
	
	if (drawConnection .connected ())
		return false;

	if (makeCurrent ())
	{
		drawConnection = signal_draw () .connect (sigc::mem_fun (*this, &Surface::set_draw));

		background -> setup ();

		construct ();
	}

	return false; // Propagate the event further.
}

bool
Surface::set_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	//Gtk::Main::iteration (false);

	update ();

	return false; // Propagate the event further.
}

void
Surface::on_unrealize ()
{
	if (context)
		context -> dispose ();

	context        .reset ();
	sharingContext .reset ();

	Gtk::DrawingArea::on_unrealize ();
}

void
Surface::dispose ()
{
	drawConnection .disconnect ();

	notify_callbacks ();

	// Don't use widget unparent!
	const auto container = get_parent ();

	if (container)
		container -> remove (*this);
	//

	if (context)
		context -> dispose ();

	context        .reset ();
	sharingContext .reset ();
}

Surface::~Surface ()
{ }

} // opengl
} // titania

