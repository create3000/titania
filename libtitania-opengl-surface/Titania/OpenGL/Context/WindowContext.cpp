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

#include "WindowContext.h"

#include <stdexcept>

#include <Titania/LOG.h>

#include <vector>
#include <algorithm>

namespace titania {
namespace opengl {

WindowContext::WindowContext (Display* const display,
                              const GLXWindow xWindow,
                              const Context & sharingContext,
                              const bool direct,
                              const int32_t samples) :
	       Context (display),
	       xWindow (xWindow),
	visualInfoList (nullptr)
{
	setContext (create (sharingContext .getContext (), direct, samples));
	setDrawable (xWindow);
}

WindowContext::WindowContext (Display* const display,
                              const GLXWindow xWindow,
                              const bool direct,
                              const int32_t samples) :
	       Context (display),
	       xWindow (xWindow),
	visualInfoList (nullptr)
{
	setContext (create (nullptr, direct, samples));
	setDrawable (xWindow);
}

void
WindowContext::setSwapInterval (const size_t interval)
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

GLXContext
WindowContext::create (const GLXContext sharingContext, const bool direct, const int32_t samples)
{
	static
	const int32_t visualAttributes [ ] = {
		GLX_DOUBLEBUFFER,     true, 
		GLX_RED_SIZE,         8,
		GLX_GREEN_SIZE,       8,
		GLX_BLUE_SIZE,        8,
		GLX_ALPHA_SIZE,       8,
		GLX_DEPTH_SIZE,       24, 
		GLX_SAMPLE_BUFFERS,   samples ? 1 : 0, // Multisampling
		GLX_SAMPLES,          samples,         // Antialiasing
		GLX_ACCUM_RED_SIZE,   8,
		GLX_ACCUM_GREEN_SIZE, 8,
		GLX_ACCUM_BLUE_SIZE,  8,
		GLX_ACCUM_ALPHA_SIZE, 8,
		0
	};

	int count;
	visualInfoList = XGetVisualInfo (getDisplay (), 0, NULL, &count);

	if (visualInfoList)
	{
		const auto bestVisual = getBestVisual (visualInfoList, count, visualAttributes);
		const auto xContext   = glXCreateContext (getDisplay (), &visualInfoList [bestVisual], sharingContext, direct);

		if (xContext)
			return xContext;
	}

	throw std::runtime_error ("WindowContext::WindowContext: Couldn't create context.");
}

int32_t
WindowContext::getBestVisual (XVisualInfo* const visualInfoList, const int32_t count, const int32_t* const visualAttributes)
{
	// Make array of array of all visual attributes.

	std::vector <std::vector <int32_t>> configs;
	
	for (int32_t i = 0; i < count; ++ i)
	{
		std::vector <int32_t> config;

		for (int32_t a = 0; visualAttributes [a]; a += 2)
		{
			config .emplace_back ();
			glXGetConfig (getDisplay (), &visualInfoList [i], visualAttributes [a], &config .back ());
		}

		config .emplace_back (i);
		configs .emplace_back (std::move (config));
	}

	std::sort (configs .begin (), configs .end ());

	// Make array of visualAttributes values;

	std::vector <int32_t> config;

	for (int32_t a = 0; visualAttributes [a]; a += 2)
		config .emplace_back (visualAttributes [a + 1]);

	// Find lower bound.

	const auto iter = std::lower_bound (configs .begin (), configs .end (), config);

	if (iter not_eq configs .end ())
		return iter -> back ();

	return configs .back () .back ();
}

WindowContext::~WindowContext ()
{
	if (visualInfoList)
	   XFree (visualInfoList);
}
	  
} // opengl
} // titania
