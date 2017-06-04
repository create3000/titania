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

#include "RenderingContext.h"

#include "../Bits/Error.h"

extern "C"
{
#include <gdk/gdkx.h>
}

namespace titania {
namespace X3D {

RenderingContext::RenderingContext (const Glib::RefPtr <Gdk::Display> & display,
                                    const std::shared_ptr <RenderingContext> & sharedContext) :
	sharedContext (sharedContext),
	      display (getDisplay (display)),
	       pixmap (createPixmap (getDisplay (display), 1, 1)),
	     drawable (createDrawable (getDisplay (display), pixmap)),
	   visualInfo (createVisualInfo (getDisplay (display))),
	      context (createContext (getDisplay (display), visualInfo, sharedContext, true))
{ }

Display*
RenderingContext::getDisplay (const Glib::RefPtr <Gdk::Display> & display) const
{
	return gdk_x11_display_get_xdisplay (display -> gobj ());
}

Pixmap
RenderingContext::createPixmap (Display* const display,
                                const size_t width,
                                const size_t height)
{
	const auto screen   = XDefaultScreenOfDisplay (display);
	const auto drawable = RootWindowOfScreen (screen);
	const auto depth    = DefaultDepthOfScreen (screen);
	const auto pixmap   = XCreatePixmap (display, drawable, width, height, depth);

	if (not pixmap)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createPixmap: Couldn't create pixmap.");

	return pixmap;
}

GLXPixmap
RenderingContext::createDrawable (Display* const display,
                                  const Pixmap pixmap)
{
	static int32_t fbAttributes [] = {
		GLX_DRAWABLE_TYPE,
		GLX_PIXMAP_BIT,
		GLX_X_RENDERABLE, true,
		GLX_RED_SIZE,     8,
		GLX_GREEN_SIZE,   8,
		GLX_BLUE_SIZE,    8,
		GLX_ALPHA_SIZE,   8,
		GLX_DEPTH_SIZE,   24, 
		None
	};

	int32_t count;

	const auto nscreen   = DefaultScreen (display);
	const auto fbConfigs = glXChooseFBConfig (display, nscreen, fbAttributes, &count);

	if (not fbConfigs)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createDrawable: No frame buffer configuration found.");

	const auto drawable = glXCreatePixmap (display, fbConfigs [0], pixmap, nullptr);

	//XFree (fbConfigs);

	if (not drawable)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createDrawable: Couldn't create GLX pixmap.");
	
	return drawable;
}

XVisualInfo*
RenderingContext::createVisualInfo (Display* const display)
{
	// Create visual attributes.

	static int32_t visualAttributes [] = {
		GLX_RGBA,
		GLX_RED_SIZE,   8,
		GLX_GREEN_SIZE, 8,
		GLX_BLUE_SIZE,  8,
		GLX_ALPHA_SIZE, 8,
		GLX_DEPTH_SIZE, 24,
		0
	};

	return glXChooseVisual (display, DefaultScreen (display), visualAttributes);
}

GLXContext
RenderingContext::createContext (Display* const display, XVisualInfo* const visualInfo, const std::shared_ptr <RenderingContext> & sharedContext, const bool direct)
{
	const auto context = glXCreateContext (display, visualInfo, sharedContext ? sharedContext -> context : NULL, direct);

	if (not context)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::RenderingContext: Couldn't create OpenGL context.");

	return context;
}

int32_t
RenderingContext::getConfig (const int32_t key) const
{
	int32_t value;

	glXGetConfig (display, visualInfo, key, &value);

	return value;
}

bool
RenderingContext::makeCurrent ()
{
	return glXMakeCurrent (display, drawable, context);
}

void
RenderingContext::dispose ()
{
	if (context)
	{
		if (glXGetCurrentContext () == context)
			glXMakeCurrent (display, None, nullptr);
	}
}

RenderingContext::~RenderingContext ()
{
	if (context)
	{
		if (glXGetCurrentContext () == context)
			glXMakeCurrent (display, None, nullptr);

		glXDestroyContext (display, context);
	}

	if (visualInfo)
	   XFree (visualInfo);

	if (drawable)
		glXDestroyPixmap (display, drawable);

	if (pixmap)
		XFreePixmap (display, pixmap);
}

} // X3D
} // titania
