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

#include "X3DRenderingSurface.h"

extern "C"
{
#include <gdk/gdkx.h>
}

#include <glibmm/main.h>
#include <gtkmm/container.h>

#include "../Browser/ContextLock.h"
#include "../RenderingSurface/OffScreenContext.h"
#include "../Rendering/FrameBuffer.h"

#include <Titania/String.h>
#include <Titania/LOG.h>

namespace titania {
namespace X3D {

X3DRenderingSurface::X3DRenderingSurface () :
	X3DRenderingSurface (nullptr)
{ }

X3DRenderingSurface::X3DRenderingSurface (X3DRenderingSurface* const other) :
	X3DRenderingSurface (other ? other -> sharingContext : nullptr)
{ }

X3DRenderingSurface::X3DRenderingSurface (const std::shared_ptr <RenderingContext> & sharingContext) :
	Gtk::DrawingArea (),
	         treadId (std::this_thread::get_id ()),
	         context (),
	  sharingContext (sharingContext),
	      extensions (),
	    antialiasing (0),
	       frameRate (60),
	     frameBuffer (new FrameBuffer (this, 1, 1, 0, true)),
	     setupSignal (),
	   reshapeSignal (),
	    renderSignal (),
	      connection ()
{
	try
	{
		// Enable map event.
		add_events (Gdk::STRUCTURE_MASK);
		set_app_paintable (true);
	
		createContext ();
	
		if (not sharingContext)
			this -> sharingContext = context;
	
		ContextLock lock (this);

		glewInit ();

		basic::split (std::inserter (extensions, extensions .end ()), (const char*) glGetString (GL_EXTENSIONS), " ");
	
		frameBuffer -> setup ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DRenderingSurface::createContext ()
{
	// Create visual attributes.

	const std::vector <int32_t> visualAttributes = {
		GLX_RGBA,
		GLX_DOUBLEBUFFER, true, 
		GLX_RED_SIZE,     8,
		GLX_GREEN_SIZE,   8,
		GLX_BLUE_SIZE,    8,
		GLX_ALPHA_SIZE,   8,
		GLX_DEPTH_SIZE,   24,
		0
	};

	// Create OpenGL context.

	context .reset (new OffScreenContext (gdk_x11_display_get_xdisplay (get_display () -> gobj ()),
	                                      sharingContext ? sharingContext -> getContext () : None,
	                                      true,
	                                      visualAttributes,
	                                      8,
	                                      8));
}

bool
X3DRenderingSurface::isExtensionAvailable (const std::string & name) const
{
	return extensions .count (name);
}

bool
X3DRenderingSurface::makeCurrent ()
{
	return std::this_thread::get_id () == treadId and context and context -> makeCurrent ();
}

void
X3DRenderingSurface::queue_render ()
{
	connection .disconnect ();

	connection = Glib::signal_timeout () .connect (sigc::mem_fun (this, &X3DRenderingSurface::on_timeout), 1000 / frameRate, Glib::PRIORITY_DEFAULT_IDLE);
}

void
X3DRenderingSurface::on_style_updated ()
{
	Gtk::DrawingArea::on_style_updated ();

	queue_resize ();
}

void
X3DRenderingSurface::on_realize ()
{
	try
	{
		Gtk::DrawingArea::on_realize ();
		
		ContextLock lock (this);

		on_setup ();
		setupSignal .emit ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DRenderingSurface::on_setup ()
{ }

void
X3DRenderingSurface::on_map ()
{
	Gtk::DrawingArea::on_map ();

	queue_resize ();
}

bool
X3DRenderingSurface::on_configure_event (GdkEventConfigure* const event)
{
	try
	{
		Gtk::DrawingArea::on_configure_event (event);

		if (not get_mapped ())
			return false;

		ContextLock lock (this);

		frameBuffer .reset (new FrameBuffer (this, get_width (), get_height (), getAntialiasing (), true)),
		frameBuffer -> setup ();
		frameBuffer -> bind ();

		on_reshape (0, 0, get_width (), get_height ());
		reshapeSignal .emit (0, 0, get_width (), get_height ());
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
X3DRenderingSurface::on_reshape (const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{ }

bool
X3DRenderingSurface::on_timeout ()
{
	try
	{
		ContextLock lock (this);

		frameBuffer -> bind ();

		on_render ();
		renderSignal .emit ();

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
X3DRenderingSurface::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	try
	{
		Gtk::DrawingArea::on_draw (cairo);

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

bool
X3DRenderingSurface::on_render ()
{
	return false;
}

void
X3DRenderingSurface::on_unmap ()
{
	Gtk::DrawingArea::on_unmap ();
}

void
X3DRenderingSurface::on_unrealize ()
{
	// Dispose context.

	context        .reset ();
	sharingContext .reset ();

	Gtk::DrawingArea::on_unrealize ();
}

void
X3DRenderingSurface::dispose ()
{
	connection .disconnect ();

	notify_callbacks ();

	// Don't use widget's unparent!
	const auto container = get_parent ();

	if (container)
		container -> remove (*this);

	// Dispose context.

	context        .reset ();
	sharingContext .reset ();
}

X3DRenderingSurface::~X3DRenderingSurface ()
{ }

} // X3D
} // titania

