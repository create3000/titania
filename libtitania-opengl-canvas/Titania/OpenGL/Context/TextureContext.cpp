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

#include "TextureContext.h"

#include <cairomm/cairomm.h>
#include <gdkmm/window.h>
#include <stdexcept>

#include "PixelBufferContext.h"
#include "WindowContext.h"

namespace titania {
namespace OpenGL {

TextureContext::TextureContext (const Glib::RefPtr <Gdk::Window> & window,
                                const Glib::RefPtr <Gdk::Display> & display,
                                const GLContext & sharingContext,
                                bool direct) :
	  GLContext (display), 
	     window (window),  
	    context (NULL),    
	frameBuffer (0),       
	    texture (0),       
	depthBuffer (0),       
	      array ()

{
	setValue (create (window, display, sharingContext .getValue (), direct));
}

TextureContext::TextureContext (const Glib::RefPtr <Gdk::Window> & window,
                                const Glib::RefPtr <Gdk::Display> & display,
                                bool direct) :
	  GLContext (display), 
	     window (window),  
	    context (NULL),    
	frameBuffer (0),       
	    texture (0),       
	depthBuffer (0),       
	      array ()
{
	setValue (create (window, display, NULL, direct));
}

GLXContext
TextureContext::create (const Glib::RefPtr <Gdk::Window> & window,
                        const Glib::RefPtr <Gdk::Display> & display,
                        GLXContext sharingContext, bool direct)
{
	context = std::shared_ptr <GLContext> (new PixelBufferContext (display));

	if (context -> makeCurrent ())
	{
		glViewport (0, 0, 1, 1);

		glGenFramebuffers  (1, &frameBuffer);
		glGenTextures      (1, &texture);
		glGenRenderbuffers (1, &depthBuffer);
	}

	return context -> getValue ();
}

bool
TextureContext::makeCurrent ()
{
	if (context -> makeCurrent ())
	{
		int height = window -> get_height ();
		int width  = window -> get_width  ();

		// Bind frame buffer.
		glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer);

		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture (GL_TEXTURE_2D, texture);

		// Give an empty image to OpenGL ( the last "0" )
		glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// Poor filtering. Needed !
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// The depth buffer
		glBindRenderbuffer (GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		// Set "texture" as our colour attachement #0
		glFramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

		// Set the list of draw buffers.
		GLenum DrawBuffers [2] = { GL_COLOR_ATTACHMENT0, 0 };
		glDrawBuffers (1, DrawBuffers);                 // "1" is the size of DrawBuffers

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus (GL_FRAMEBUFFER) not_eq GL_FRAMEBUFFER_COMPLETE)
			return false;

		return true;
	}

	return false;
}

void
TextureContext::swapBuffers ()
{
	int height = window -> get_height ();
	int width  = window -> get_width  ();

	array .resize (4 * width * height, 127);

	glBindTexture (GL_TEXTURE_2D, texture);
	glGetTexImage (GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, array .data ());

	Cairo::RefPtr <Cairo::ImageSurface> image = Cairo::ImageSurface::create (array .data (),
	                                                                         Cairo::FORMAT_ARGB32,
	                                                                         width, height,
	                                                                         Cairo::ImageSurface::format_stride_for_width (Cairo::FORMAT_ARGB32, width));

	//	cairo -> set_source (image, 0, 0);
	//	cairo -> paint ();

}

TextureContext::~TextureContext ()
{ }

} // OpenGL
} // titania
