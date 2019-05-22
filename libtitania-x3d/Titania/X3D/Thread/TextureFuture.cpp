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

#include "TextureFuture.h"

#include "../InputOutput/FileLoader.h"

namespace titania {
namespace X3D {

const Component   TextureFuture::component      = Component ("Titania", 1);
const std::string TextureFuture::typeName       = "TextureFuture";
const std::string TextureFuture::containerField = "future";

TextureFuture::TextureFuture (X3DExecutionContext* const executionContext,
                              const MFString & url,
                              const Callback & callback) :
	TextureFuture (executionContext, url, 0, 0, false, callback)
{ }

TextureFuture::TextureFuture (X3DExecutionContext* const executionContext,
                              const MFString & url,
                              const size_t minTextureSize, const size_t maxTextureSize,
                              const Callback & callback) :
	TextureFuture (executionContext, url, minTextureSize, maxTextureSize, true, callback)
{ }

TextureFuture::TextureFuture (X3DExecutionContext* const executionContext,
                              const MFString & url,
                              const size_t minTextureSize, const size_t maxTextureSize,
                              const bool process,
                              const Callback & callback) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DFuture (),
	    browser (executionContext -> getBrowser ()),
	    process (process),
	   callback (callback),
	     loader (executionContext, executionContext -> getWorldURL ()),
	     future (getFuture (url, minTextureSize, maxTextureSize))
{
	getBrowser () -> prepareEvents () .addInterest (&TextureFuture::prepareEvents, this);
	getBrowser () -> addEvent ();
}

X3DBaseNode*
TextureFuture::create (X3DExecutionContext* const executionContext) const
{
	throw Error <NOT_SUPPORTED> ("TextureFuture::create");
}

std::future <TexturePtr>
TextureFuture::getFuture (const MFString & url,
                          const size_t minTextureSize, const size_t maxTextureSize)
{
	if (url .empty ())
		std::async (std::launch::deferred, [ ] (){ return nullptr; });

	return std::async (std::launch::async, std::mem_fn (&TextureFuture::loadAsync), this,
	                   url,
	                   minTextureSize, maxTextureSize);
}

void
TextureFuture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	const bool prepareEvents = getBrowser () -> prepareEvents () .hasInterest (&TextureFuture::prepareEvents, this);

	getBrowser () -> prepareEvents () .removeInterest (&TextureFuture::prepareEvents, this);

	X3DFuture::setExecutionContext (executionContext);

	if (prepareEvents)
	{
		getBrowser () -> prepareEvents () .addInterest (&TextureFuture::prepareEvents, this);

		getBrowser () -> addEvent ();
	}
}

TexturePtr
TextureFuture::loadAsync (const MFString & url,
                          const size_t minTextureSize, const size_t maxTextureSize)
{
	for (const auto & URL : url)
	{
		try
		{
			checkForInterrupt ();

			const auto mutex = getBrowser () -> getDownloadMutex ();

			checkForInterrupt ();

			std::lock_guard <std::mutex> lock (*mutex);

			checkForInterrupt ();

			TexturePtr texture (new Texture (loader .loadDocument (URL .raw ()), process));

			checkForInterrupt ();

			getBrowser () -> getConsole () -> log ("Done loading image '", loader .getWorldURL (), "'.\n");

			checkForInterrupt ();

			return texture;
		}
		catch (const InterruptThreadException &)
		{
			throw;
		}
		catch (const X3DError & error)
		{
			checkForInterrupt ();

			getBrowser () -> getConsole () -> error (error .what (), "\n");
		}
		catch (const std::exception & error)
		{
			checkForInterrupt ();

			getBrowser () -> getConsole () -> error ("Bad Image: ", error .what (), ", in URL '", loader .getReferer () .transform (URL .raw ()), "'.\n");
		}
	}

	return nullptr;
}

void
TextureFuture::prepareEvents ()
{
	try
	{
		checkForInterrupt ();

		getBrowser () -> addEvent ();

		if (not future .valid ())
			return;

		const auto status = future .wait_for (std::chrono::milliseconds (0));

		if (status not_eq std::future_status::ready)
			return;

		getBrowser () -> prepareEvents () .removeInterest (&TextureFuture::prepareEvents, this);

		callback (loader .getWorldURL (), future .get ());

		stop ();
	}
	catch (const InterruptThreadException &)
	{
		// Interrupt
	}
	catch (const std::exception &)
	{
		callback ("", nullptr);

		stop ();
	}
}

void
TextureFuture::stop ()
{
	X3DFuture::stop ();

	loader .stop ();

	callback = [ ] (const basic::uri &, const TexturePtr &) { };
}

void
TextureFuture::dispose ()
{
	stop ();

	X3DFuture::dispose ();
}

TextureFuture::~TextureFuture ()
{
	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
