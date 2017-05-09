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

#include "Context.h"

#include <Titania/LOG.h>
#include <stdexcept>
#include <thread>

namespace titania {
namespace OpenGL {

Context::Context (Display* const display,
                  const GLXDrawable drawable,
                  const GLXContext sharingContext,
                  const bool direct,
                  const std::vector <int32_t> & visualAttributes) :
	   display (display),
	  drawable (drawable),
	   context (create (sharingContext, direct, visualAttributes)),
	visualInfo (nullptr)
{ }

GLXContext
Context::create (const GLXContext sharingContext, const bool direct, const std::vector <int32_t> & visualAttributes)
{
	visualInfo = glXChooseVisual (getDisplay (), DefaultScreen (getDisplay ()), const_cast <int32_t*> (visualAttributes .data ()));

	const auto context = glXCreateContext (getDisplay (), visualInfo, sharingContext, direct);

	if (not context)
		throw std::runtime_error ("WindowContext::WindowContext: Couldn't create context.");

	return context;
}

void
Context::setSwapInterval (const size_t interval)
{
	auto glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC) glXGetProcAddress ((GLubyte*) "glXSwapIntervalEXT");
	
	if (glXSwapIntervalEXT)
	{
		glXSwapIntervalEXT (display, drawable, interval);
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

int32_t
Context::getConfig (const int32_t key) const
{
	int32_t value;

	glXGetConfig (display, visualInfo, key, &value);

	return value;
}

bool
Context::makeCurrent () const
{
	return glXMakeCurrent (display, drawable, context);
}

void
Context::swapBuffers () const
{
	glXSwapBuffers (display, drawable);
}

void
Context::dispose ()
{
	if (context)
	{
		if (glXGetCurrentContext () == context)
			glXMakeCurrent (display, None, nullptr);
	}
}

Context::~Context ()
{
	if (context)
	{
		if (glXGetCurrentContext () == context)
			glXMakeCurrent (display, None, nullptr);

		glXDestroyContext (display, context);
	}

	if (visualInfo)
	   XFree (visualInfo);
}

} // OpenGL
} // titania
