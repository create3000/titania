/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "PixelBufferContext.h"

#include <gdk/gdkx.h>
#include <gdkmm/window.h>
#include <stdexcept>

namespace titania {
namespace opengl {

PixelBufferContext::PixelBufferContext (const Glib::RefPtr <Gdk::Display> & display,
                                        const Context & sharingContext,
                                        bool direct) :
	Context (display), 
	 xPBuffer (0)        
{
	setValue    (create (sharingContext .getValue (), direct));
	setDrawable (xPBuffer);
}

PixelBufferContext::PixelBufferContext (const Glib::RefPtr <Gdk::Display> & display,
                                        bool direct) :
	Context (display), 
	 xPBuffer (0)        
{
	setValue    (create (NULL, direct));
	setDrawable (xPBuffer);
}

GLXContext
PixelBufferContext::create (GLXContext sharingContext, bool direct)
{
	static
	int32_t fbConfigAttributes [ ] = {
		GLX_DRAWABLE_TYPE, GLX_PBUFFER_BIT,
		GLX_RENDER_TYPE,   GLX_RGBA_BIT,
		GLX_RED_SIZE,      1,
		GLX_GREEN_SIZE,    1,
		GLX_BLUE_SIZE,     1,
		GLX_ALPHA_SIZE,    1,
		GLX_DOUBLEBUFFER,  GL_FALSE,
		0
	};

	int          returnedElements;
	GLXFBConfig* configs = glXChooseFBConfig (getDisplay (), 0, fbConfigAttributes, &returnedElements);

	if (not returnedElements)
	{
		XFree (configs);
		throw std::runtime_error ("PixelBufferContext::PixelBufferContext: Couldn't get frame buffer config.");
	}

	// We will be rendering to a texture, so our pbuffer does not need to be large.
	static const int pbufferAttributes [ ] = { GLX_PBUFFER_WIDTH, 1, GLX_PBUFFER_HEIGHT, 1, 0 };

	xPBuffer = glXCreatePbuffer (getDisplay (), configs [0], pbufferAttributes);

	if (not xPBuffer)
	{
		XFree (configs);
		throw std::runtime_error ("PixelBufferContext::PixelBufferContext: Couldn't create pixel buffer.");
	}

	GLXContext xContext = glXCreateNewContext (getDisplay (), configs [0], GLX_RGBA_TYPE, sharingContext, direct);
	XFree (configs);

	if (not xContext)
	{
		glXDestroyPbuffer (getDisplay (), xPBuffer);
		throw std::runtime_error ("PixelBufferContext::PixelBufferContext: Couldn't create context.");
	}

	return xContext;
}

PixelBufferContext::~PixelBufferContext ()
{
	glXDestroyPbuffer (getDisplay (), xPBuffer);
}

} // opengl
} // titania
