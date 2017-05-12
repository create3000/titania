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

#include "OffScreenContext.h"

#include <stdexcept>

namespace titania {
namespace OpenGL {

OffScreenContext::OffScreenContext (Display* const display,
                                    const GLXContext sharingContext,
                                    const bool direct,
                                    const std::vector <int32_t> & visualAttributes,
                                    unsigned int width,
                                    unsigned int height) :
	   Context (display, createPixmap (display, width, height), sharingContext, direct, visualAttributes)
{ }

//Pixmap
//OffScreenContext::createPixmap (Display* display,
//                                unsigned int width,
//                                unsigned int height)
//{
//	const auto screen   = XDefaultScreenOfDisplay (display);
//	const auto drawable = RootWindowOfScreen (screen);
//	const auto depth    = DefaultDepthOfScreen (screen);
//	const auto pixmap   = XCreatePixmap (display, drawable, width, height, depth);
//
//	if (not pixmap)
//		throw std::runtime_error ("OffScreenContext::createPixmap: Couldn't create pixmap.");
//
//	return pixmap;
//}

GLXPixmap
OffScreenContext::createPixmap (Display* display,
                                unsigned int width,
                                unsigned int height)
{
	const std::vector <int32_t> fbAttributes = {
		GLX_DRAWABLE_TYPE,
		GLX_PIXMAP_BIT,
		GLX_X_RENDERABLE, true,
		GLX_DOUBLEBUFFER, true, 
		GLX_RED_SIZE,     8,
		GLX_GREEN_SIZE,   8,
		GLX_BLUE_SIZE,    8,
		GLX_ALPHA_SIZE,   8,
		GLX_DEPTH_SIZE,   24, 
		None
	};

	const auto screen   = XDefaultScreenOfDisplay (display);
	const auto nscreen  = DefaultScreen (display);
	const auto drawable = RootWindowOfScreen (screen);
	const auto depth    = DefaultDepthOfScreen (screen);

	pixmap = XCreatePixmap (display, drawable, width, height, depth);

	if (not pixmap)
		throw std::runtime_error ("OffScreenContext::createPixmap: Couldn't create pixmap.");

	int32_t count;

	const auto fbConfigs = glXChooseFBConfig (display, nscreen, fbAttributes .data (), &count);

	if (not fbConfigs)
		throw std::runtime_error ("OffScreenContext::createPixmap: No frame buffer configuration found.");

	const auto glXPixmap = glXCreatePixmap (display, fbConfigs [0], pixmap, nullptr);

	if (not glXPixmap)
		throw std::runtime_error ("OffScreenContext::createPixmap: Couldn't create GLX pixmap.");
	
	return glXPixmap;
}

OffScreenContext::~OffScreenContext ()
{
	glXDestroyPixmap (getDisplay (), getDrawable ());
	XFreePixmap (getDisplay (), pixmap);
}
	  
} // OpenGL
} // titania
