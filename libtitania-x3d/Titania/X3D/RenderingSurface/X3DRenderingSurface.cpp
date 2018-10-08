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

#include <glibmm/main.h>
#include <gtkmm/container.h>
#include <gdkmm/screen.h>

#include "../RenderingSurface/ContextLock.h"
#include "../RenderingSurface/RenderingContext.h"
#include "../Rendering/FrameBuffer.h"

#include <Titania/String.h>
#include <Titania/LOG.h>

#include <atomic>
#include <thread>

namespace titania {
namespace X3D {

/***
 *  A widget used for drawing with OpenGL.
 *
 *  @param  other  Sharing surface.
 *
 *  A X3DRenderingSurface object must be instantiated and disposed in the main thread.
 */

static const std::atomic <std::thread::id> mainTreadId (std::this_thread::get_id ());

X3DRenderingSurface::X3DRenderingSurface () :
	X3DRenderingSurface (nullptr)
{ }

X3DRenderingSurface::X3DRenderingSurface (X3DRenderingSurface* const other) :
	  Gtk::DrawingArea (),
	        initialized (false),
	            context (new RenderingContext (other ? other -> context : nullptr)),
	         extensions (),
	       antialiasing (0),
	          frameRate (60),
	processRenderEvents (true),
	        frameBuffer (new FrameBuffer (this, 1, 1, 0, true)),
	        setupSignal (),
	      reshapeSignal (),
	       renderSignal (),
	  timeoutDispatcher (new Glib::Dispatcher ()),
	  timeoutConnection (),
	              mutex ()
{
	ContextLock lock (this);

	// Enable map event.
	add_events (Gdk::STRUCTURE_MASK);
	set_app_paintable (true);
	get_style_context () -> add_class ("titania-surface");

	glewInit ();

	basic::split (std::inserter (extensions, extensions .end ()), (const char*) glGetString (GL_EXTENSIONS), " ");

	//for (const auto & extension : extensions)
	//	__LOG__ << extension << std::endl;

	frameBuffer -> setup ();

	timeoutDispatcher -> connect (sigc::mem_fun (this, &X3DRenderingSurface::on_dispatch));
}

bool
X3DRenderingSurface::getExtension (const std::string & name) const
{
	return extensions .count (name);
}

void
X3DRenderingSurface::lock ()
{
	mutex .lock ();
}

void
X3DRenderingSurface::unlock ()
{
	mutex .unlock ();
}

void
X3DRenderingSurface::queue_render ()
{
	if (processRenderEvents)
	{
		if (std::this_thread::get_id () == mainTreadId)
		{
			on_dispatch ();
		}
		else
		{
			if (timeoutDispatcher)
				timeoutDispatcher -> emit ();
		}
	}
}

void
X3DRenderingSurface::on_realize ()
{
	try
	{
		Gtk::DrawingArea::on_realize ();

		if (not initialized)
		{
			initialized = true;

			ContextLock lock (this);

			on_setup ();
			setupSignal .emit ();
		}
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

void
X3DRenderingSurface::on_style_updated ()
{
	Gtk::DrawingArea::on_style_updated ();

	queue_resize ();
}

void
X3DRenderingSurface::on_size_allocate (Gtk::Allocation & allocation)
{
	try
	{
		Gtk::DrawingArea::on_size_allocate (allocation);

		if (not get_mapped ())
			return;

		ContextLock lock (this);

		frameBuffer .reset (new FrameBuffer (this, allocation .get_width (), allocation .get_height (), getAntialiasing (), true)),
		frameBuffer -> setup ();
		frameBuffer -> bind ();

		const auto state   = get_style_context () -> get_state ();
		const auto margin  = get_style_context () -> get_margin (state);
		const auto border  = get_style_context () -> get_border (state);
		const auto padding = get_style_context () -> get_padding (state);

		const auto x = margin .get_left ()   + border .get_left ()   + padding .get_left ();
		const auto y = margin .get_bottom () + border .get_bottom () + padding .get_bottom ();
		const auto w = allocation .get_width ()  - x - (margin .get_right () + border .get_right () + padding .get_right ());
		const auto h = allocation .get_height () - y - (margin .get_top ()   + border .get_top ()   + padding .get_top ());

		glViewport (0, 0, allocation .get_width (), allocation .get_height ());
		glScissor  (0, 0, allocation .get_width (), allocation .get_height ());

		glClearColor (0, 0, 0, 0);
		glClear (GL_COLOR_BUFFER_BIT);

		on_reshape (x, y, w, h);
		reshapeSignal .emit (x, y, w, h);

		frameBuffer -> unbind ();

		on_timeout ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
X3DRenderingSurface::on_reshape (const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{ }

void
X3DRenderingSurface::on_dispatch ()
{
	if (timeoutConnection .connected ())
		return;

	// Timout with Gtk default drawing priority.
	timeoutConnection = Glib::signal_timeout () .connect (sigc::mem_fun (this, &X3DRenderingSurface::on_timeout), frameRate ? 1000 / frameRate : 0, Glib::PRIORITY_DEFAULT_IDLE);
}

bool
X3DRenderingSurface::on_timeout ()
{
	try
	{
		ContextLock lock (this);
	
		timeoutConnection .disconnect ();

		frameBuffer -> bind ();
	
		on_render ();
		renderSignal .emit ();

		queue_draw ();

		frameBuffer -> unbind ();
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

		const auto state   = get_style_context () -> get_state ();
		const auto margin  = get_style_context () -> get_margin (state);
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

		get_style_context () -> render_frame (cairo,
		                                      margin .get_left (),
		                                      margin .get_top (),
		                                      get_width ()  - margin .get_left () - margin .get_right (),
		                                      get_height () - margin .get_top ()  - margin .get_bottom ());

		frameBuffer -> unbind ();
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
	Gtk::DrawingArea::on_unrealize ();
}

void
X3DRenderingSurface::dispose ()
{
	// The Dispatcher object must be instantiated and deleted by the receiver thread.
	timeoutDispatcher .reset ();
	timeoutConnection .disconnect ();

	notify_callbacks ();

	// Don't use widget's unparent!
	const auto container = get_parent ();

	if (container)
		container -> remove (*this);
}

X3DRenderingSurface::~X3DRenderingSurface ()
{
	// Dispose context.

	frameBuffer .reset ();
	context     .reset ();
}

} // X3D
} // titania

