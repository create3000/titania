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
 ******************************************************************************/

#include "Surface.h"

#include <gdk/gdkx.h>
#include <gtkmm/container.h>

#include "Context/PixelBufferContext.h"
#include "Context/WindowContext.h"

#include <Titania/LOG.h>
#include <stdexcept>

namespace titania {
namespace opengl {

Surface::Surface () :
	Gtk::DrawingArea (),    
	        context  (NULL) 
{
	set_double_buffered (false);
	set_app_paintable (true);

	// Enable map_event.
	add_events (Gdk::STRUCTURE_MASK);

	// Connect to map_event.
	initialized_connection = signal_map_event () .connect (sigc::mem_fun (*this, &Surface::on_map));
}

const std::shared_ptr <Context> &
Surface::getContext ()
{
	return context;
}

//void
//Surface::createPixmapContext ()
//{
//	GLXContext sharingContext = NULL;
//
//	display = gdk_x11_display_get_xdisplay (get_display () -> gobj ());
//
//	static
//	int32_t visualAttributes [ ] = {
//		GLX_RGBA,
//		GLX_RED_SIZE, 1,
//		GLX_GREEN_SIZE, 1,
//		GLX_BLUE_SIZE, 1,
//		GLX_ALPHA_SIZE, 1,
//		0
//	};
//
//	XVisualInfo* visualInfo = glXChooseVisual (display, DefaultScreen (display), visualAttributes);
//
//	if (not visualInfo)
//		return;
//
//	context = glXCreateContext (display, visualInfo, sharingContext, true);
//
//	if (not context)
//	{
//		XFree (visualInfo);
//		return;
//	}
//
//	pixmap = XCreatePixmap (display, DefaultRootWindow (display), 1, 1, visualInfo -> depth);
//
//	if (not pixmap)
//	{
//		XFree (visualInfo);
//		return;
//	}
//
//	glxPixmap = glXCreateGLXPixmap (display, visualInfo, pixmap);
//
//	if (not glxPixmap)
//	{
//		XFreePixmap (display, pixmap);
//		XFree (visualInfo);
//		return;
//	}
//
//	XFree (visualInfo);
//}

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
Surface::on_map (GdkEventAny* event)
{
	initialized_connection .disconnect ();

	context .reset (new WindowContext (get_window (), get_display ()));

	if (makeCurrent ())
	{
		signal_configure_event () .connect (sigc::mem_fun (*this, &Surface::set_configure_event));
		signal_draw ()            .connect (sigc::mem_fun (*this, &Surface::set_draw));

		glewInit ();

		glViewport (0, 0, get_width (), get_height ());

		construct ();
	}

	return false; // Propagate the event further.
}

bool
Surface::set_configure_event (GdkEventConfigure* event)
{
	if (makeCurrent ())
	{
		glViewport (0, 0, get_width (), get_height ());

		reshape ();
	}

	return false; // Propagate the event further.
}

bool
Surface::set_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	if (makeCurrent ())
	{
		update (cairo);
	}

	return false; // Propagate the event further.
}

bool
Surface::makeCurrent ()
{
	return context -> makeCurrent ();
}

void
Surface::swapBuffers ()
{
	glFinish ();

	context -> swapBuffers ();
}

void
Surface::dispose ()
{
	context = std::shared_ptr <Context> ();
}

Surface::~Surface ()
{
}

} // opengl
} // titania
