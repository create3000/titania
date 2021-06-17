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

#ifdef __APPLE__
#include "ContextLock.h"

#include "../Browser/X3DBrowser.h"
#include "../RenderingSurface/X3DRenderingSurface.h"

#include <thread>

namespace titania {
namespace X3D {

class ContextLock::Implementation
{
public:

	///  @name Construction

	Implementation (X3DRenderingSurface* const renderingSurface);

	///  @name Destruction

	~Implementation ();


private:

	///  @name Static members

	static std::map <std::thread::id, Glib::RefPtr <Gdk::GLContext>> currentContexts;

	static std::mutex mutex;

	///  @name Members

	Glib::RefPtr <Gdk::GLContext> previousContext;

};

std::map <std::thread::id, Glib::RefPtr <Gdk::GLContext>> ContextLock::Implementation::currentContexts;
std::mutex ContextLock::Implementation::mutex;

ContextLock::Implementation::Implementation (X3DRenderingSurface* const renderingSurface) :
	previousContext ()
{
	std::lock_guard <std::mutex> lock (mutex);

	const auto currentContext = renderingSurface -> getContext ();

	if (not currentContext)
		throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");

	previousContext = currentContexts [std::this_thread::get_id ()];

	currentContext -> make_current ();

	currentContexts [std::this_thread::get_id ()] = currentContext;
}

ContextLock::Implementation::~Implementation ()
{
	std::lock_guard <std::mutex> lock (mutex);

	if (previousContext)
	{
		previousContext -> make_current ();

		currentContexts [std::this_thread::get_id ()] = previousContext;
	}
	else
	{
		currentContexts .erase (std::this_thread::get_id ());

		Gdk::GLContext::clear_current ();
	}
}

ContextLock::ContextLock (X3DBrowser* const browser) :
	ContextLock (static_cast <X3DRenderingSurface*> (browser))
{ }

/**
 *  When a ContextLock object is created, it attempts to aquire the OpenGL context of the browser instance, otherwise
 *  an exception of type INVALID_OPERATION_TIMING is thrown.  On destruction the previous OpenGL context is restored.
 *
 *  @param  renderingSurface  A valid X3DRenderingSurface instance.
 *
 *  throws Error <INVALID_OPERATION_TIMING>
 */
ContextLock::ContextLock (X3DRenderingSurface* const renderingSurface) :
	implementation ()
{
	implementation .reset (new Implementation (renderingSurface));
}

ContextLock::~ContextLock ()
{ }

} // X3D
} // titania
#else
#include "ContextLock.h"

#include "../RenderingSurface/RenderingContext.h"
#include "../RenderingSurface/X3DRenderingSurface.h"

#include <thread>

namespace titania {
namespace X3D {

class ContextLock::Implementation
{
public:

	///  @name Construction

	Implementation (X3DRenderingSurface* const renderingSurface);

	///  @name Destruction

	~Implementation ();


private:

	///  @name Static members

	static std::map <std::thread::id, std::shared_ptr <RenderingContext>> currentContexts;

	static std::mutex mutex;

	///  @name Members

	std::lock_guard <X3DRenderingSurface> surfaceLock;
	std::shared_ptr <RenderingContext>    previousContext;

};

std::map <std::thread::id, std::shared_ptr <RenderingContext>> ContextLock::Implementation::currentContexts;
std::mutex ContextLock::Implementation::mutex;

ContextLock::Implementation::Implementation (X3DRenderingSurface* const renderingSurface) :
	    surfaceLock (*renderingSurface),
	previousContext ()
{
	std::lock_guard <std::mutex> lock (mutex);

	const auto currentContext = renderingSurface -> getContext ();

	previousContext = currentContexts [std::this_thread::get_id ()];

	if (currentContext)
	{
		if (currentContext -> makeCurrent ())
		{
			currentContexts [std::this_thread::get_id ()] = currentContext;
		   return;
		}
	}

	// Throw an exception if it cannot make current!  The destructor is then not called.
	throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");
}

ContextLock::Implementation::~Implementation ()
{
	std::lock_guard <std::mutex> lock (mutex);

	if (previousContext)
	{
		if (previousContext -> makeCurrent ())
		{
			currentContexts [std::this_thread::get_id ()] = previousContext;
			return;
		}
	}

	currentContexts .erase (std::this_thread::get_id ());

	RenderingContext::clearCurrent ();
}

/**
 *  When a ContextLock object is created, it attempts to aquire the OpenGL context of the browser instance, otherwise
 *  an exception of type INVALID_OPERATION_TIMING is thrown.  On destruction the previous OpenGL context is restored.
 *
 *  @param  renderingSurface  A valid X3DRenderingSurface instance.
 *
 *  throws Error <INVALID_OPERATION_TIMING>
 */
ContextLock::ContextLock (X3DRenderingSurface* const renderingSurface) :
	implementation ()
{
	if (not renderingSurface)
		throw Error <INVALID_OPERATION_TIMING> ("Invalid operation timing.");

	implementation .reset (new Implementation (renderingSurface));
}

ContextLock::~ContextLock ()
{ }

} // X3D
} // titania
#endif
