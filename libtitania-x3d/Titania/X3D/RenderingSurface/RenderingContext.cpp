
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

#ifndef __APPLE__
#include "RenderingContext.h"

#include "../Bits/Error.h"

extern "C"
{
#include <gdk/gdkx.h>
}

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

RenderingContext::RenderingContext (const std::shared_ptr <RenderingContext> & sharedContext) :
	sharedContext (sharedContext),
	     xDisplay (XOpenDisplay (nullptr)),
	      xPixmap (createPixmap (xDisplay, 1, 1)),
	    xDrawable (createDrawable (xDisplay, xPixmap)),
	  xVisualInfo (createVisualInfo (xDisplay)),
	     xContext (createContext (xDisplay, xVisualInfo, sharedContext, true))
{ }

Pixmap
RenderingContext::createPixmap (Display* const xDisplay,
                                const size_t width,
                                const size_t height)
{
	const auto xScreen   = XDefaultScreenOfDisplay (xDisplay);
	const auto xDrawable = RootWindowOfScreen (xScreen);
	const auto depth     = DefaultDepthOfScreen (xScreen);
	const auto xPixmap   = XCreatePixmap (xDisplay, xDrawable, width, height, depth);

	if (not xPixmap)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createPixmap: Couldn't create xPixmap.");

	return xPixmap;
}

GLXPixmap
RenderingContext::createDrawable (Display* const xDisplay,
                                  const Pixmap xPixmap)
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

	const auto nscreen   = DefaultScreen (xDisplay);
	const auto fbConfigs = glXChooseFBConfig (xDisplay, nscreen, fbAttributes, &count);

	if (not fbConfigs)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createDrawable: No frame buffer configuration found.");

	const auto xDrawable = glXCreatePixmap (xDisplay, fbConfigs [0], xPixmap, nullptr);

	XFree (fbConfigs);

	if (not xDrawable)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::createDrawable: Couldn't create GLX xPixmap.");

	return xDrawable;
}

XVisualInfo*
RenderingContext::createVisualInfo (Display* const xDisplay)
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

	return glXChooseVisual (xDisplay, DefaultScreen (xDisplay), visualAttributes);
}

GLXContext
RenderingContext::createContext (Display* const xDisplay,
                                 XVisualInfo* const xVisualInfo,
                                 const std::shared_ptr <RenderingContext> & sharedContext,
                                 const bool direct)
{
	const auto xContext = glXCreateContext (xDisplay, xVisualInfo, sharedContext ? sharedContext -> xContext : NULL, direct);

	if (not xContext)
		throw Error <INSUFFICIENT_CAPABILITIES> ("RenderingContext::RenderingContext: Couldn't create OpenGL xContext.");

	return xContext;
}

//int32_t
//RenderingContext::getConfig (const int32_t key) const
//{
//	int32_t value;
//
//	glXGetConfig (xDisplay, xVisualInfo, key, &value);
//
//	return value;
//}

bool
RenderingContext::makeCurrent ()
{
	return glXMakeCurrent (xDisplay, xDrawable, xContext);
}

void
RenderingContext::clearCurrent ()
{
	// Or use XOpenDisplay (NULL); to get a display;
	// Or use Gtk default display
	const auto xCurrentDisplay = glXGetCurrentDisplay ();

	if (xCurrentDisplay)
		glXMakeCurrent (xCurrentDisplay, None, nullptr);
}

RenderingContext::~RenderingContext ()
{
	if (xContext)
	{
		if (glXGetCurrentContext () == xContext)
			glXMakeCurrent (xDisplay, None, nullptr);

		glXDestroyContext (xDisplay, xContext);
	}

	if (xVisualInfo)
	   XFree (xVisualInfo);

	if (xDrawable)
		glXDestroyPixmap (xDisplay, xDrawable);

	if (xPixmap)
		XFreePixmap (xDisplay, xPixmap);

	XCloseDisplay (xDisplay);
}

} // X3D
} // titania
#endif
