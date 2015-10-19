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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "WindowContext.h"

#include <stdexcept>

#include <Titania/LOG.h>

namespace titania {
namespace opengl {

WindowContext::WindowContext (Display* const display,
                              const GLXWindow xWindow,
                              const Context & sharingContext,
                              const bool direct) :
	       Context (display),
	       xWindow (xWindow),
	visualInfoList (nullptr)
{
	setContext (create (sharingContext .getContext (), direct));
	setDrawable (xWindow);
}

WindowContext::WindowContext (Display* const display,
                              const GLXWindow xWindow,
                              const bool direct) :
	       Context (display),
	       xWindow (xWindow),
	visualInfoList (nullptr)
{
	setContext (create (nullptr, direct));
	setDrawable (xWindow);
}

void
WindowContext::setSwapInterval (const size_t interval)
{
	if (makeCurrent ())
	{
		auto glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress ((GLubyte*) "glXSwapIntervalEXT");
		
		if (glXSwapIntervalEXT)
		{
			glXSwapIntervalEXT (getDisplay (), xWindow, interval);
			return;
		}

		auto glXSwapIntervalSGI = (PFNGLXSWAPINTERVALSGIPROC) glXGetProcAddress ((GLubyte*) "glXSwapIntervalSGI");

		if (glXSwapIntervalSGI)
		{
			glXSwapIntervalSGI (interval);
			return;
		}
		
		auto glXSwapIntervalMESA = (PFNGLXSWAPINTERVALMESAPROC) glXGetProcAddress ((GLubyte*) "glXSwapIntervalMESA");

		if (glXSwapIntervalMESA)
		{
			glXSwapIntervalMESA (interval);
			return;
		}
	}
}

GLXContext
WindowContext::create (const GLXContext sharingContext, const bool direct)
{
//	XWindowAttributes attributes;
//
//	if (not XGetWindowAttributes (getDisplay (), xWindow, &attributes))
//		throw std::runtime_error ("WindowContext::WindowContext: Couldn't get window attributes.");
//
//	XVisualInfo visualInfo;
//	visualInfo .visualid = XVisualIDFromVisual (attributes .visual);
//
//	int numReturned = 0;
//
//	visualInfoList = XGetVisualInfo (getDisplay (), VisualIDMask, &visualInfo, &numReturned);
//
//	GLXContext xContext = glXCreateContext (getDisplay (), visualInfoList, sharingContext, direct);
//
//	if (not xContext)
//		throw std::runtime_error ("WindowContext::WindowContext: Couldn't create context.");
//
//	return xContext;

	static
	int32_t xVisualAttributes [ ] = {
		GLX_RGBA,
		GLX_RED_SIZE,         1,
		GLX_GREEN_SIZE,       1,
		GLX_BLUE_SIZE,        1,
		GLX_ALPHA_SIZE,       1,
		GLX_ACCUM_RED_SIZE,   1,
		GLX_ACCUM_GREEN_SIZE, 1,
		GLX_ACCUM_BLUE_SIZE,  1,
		GLX_ACCUM_ALPHA_SIZE, 1,
		GLX_DOUBLEBUFFER,     true, 
		GLX_DEPTH_SIZE,       24, 
		0
	};

	visualInfoList = glXChooseVisual (getDisplay (), DefaultScreen (getDisplay ()), xVisualAttributes);

	GLXContext xContext = glXCreateContext (getDisplay (), visualInfoList, sharingContext, direct);

	if (not xContext)
		throw std::runtime_error ("WindowContext::WindowContext: Couldn't create context.");

	return xContext;
}

WindowContext::~WindowContext ()
{
	if (visualInfoList)
	   XFree (visualInfoList);
}
	  
} // opengl
} // titania
