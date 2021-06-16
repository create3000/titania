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

#include "X3DCoreContext.h"

#include "BrowserOptions.h"
#include "BrowserProperties.h"
#include "RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/OpenGL.h"

#include <Titania/String.h>

#include <glibtop.h>
#include <glibtop/procmem.h>
#include <unistd.h>

namespace titania {
namespace X3D {

X3DCoreContext::X3DCoreContext () :
	        X3DBaseNode (),
	             strict (true),
	             vendor (),
	           renderer (),
	            version (),
	     browserOptions (new BrowserOptions (getBrowser ())),
	  browserProperties (new BrowserProperties (getBrowser ())),
	renderingProperties (new RenderingProperties (getBrowser ()))
{
	addChildObjects (browserOptions,
	                 browserProperties,
	                 renderingProperties);

	glibtop_init ();
}

void
X3DCoreContext::initialize ()
{
	vendor   = (const char*) glGetString (GL_VENDOR);
	renderer = (const char*) glGetString (GL_RENDERER);
	version  = (const char*) glGetString (GL_VERSION);

	browserOptions      -> setup ();
	browserProperties   -> setup ();
	renderingProperties -> setup ();
}

size_t
X3DCoreContext::getMaxRenderBufferSize () const
{
	int32_t maxRenderBufferSize = 0;

	try
	{
		ContextLock lock (getBrowser ());

		glGetIntegerv (GL_MAX_RENDERBUFFER_SIZE, &maxRenderBufferSize);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	return maxRenderBufferSize;
}

size_t
X3DCoreContext::getMaxSamples () const
{
	int32_t maxSamples = 0;

	try
	{
		ContextLock lock (getBrowser ());

		glGetIntegerv (GL_MAX_SAMPLES, &maxSamples);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	return maxSamples;
}

size_t
X3DCoreContext::getMemoryUsage ()
{
	glibtop_proc_mem memory;

	glibtop_get_proc_mem (&memory, getpid ());

	return memory .rss;
}

X3DCoreContext::~X3DCoreContext ()
{ }

} // X3D
} // titania
