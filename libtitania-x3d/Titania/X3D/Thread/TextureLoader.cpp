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

#include "TextureLoader.h"

#include "../InputOutput/Loader.h"

namespace titania {
namespace X3D {

TextureLoader::TextureLoader (X3DExecutionContext* const executionContext,
                              const MFString & url,
                              const size_t minTextureSize, const size_t maxTextureSize,
                              const Callback & callback) :
	X3DInterruptibleThread (),
	              X3DInput (),
	               browser (executionContext -> getBrowser ()),
	               referer (executionContext -> getWorldURL ()),
	              callback (callback),
	                 mutex (),
	               future (getFuture (url, minTextureSize, maxTextureSize))
{
	getBrowser () -> prepareEvents () .addInterest (this, &TextureLoader::prepareEvents);
	getBrowser () -> addEvent ();
}

void
TextureLoader::setExecutionContext (X3DExecutionContext* const executionContext)
{
	getBrowser () -> prepareEvents () .removeInterest (this, &TextureLoader::prepareEvents);

	browser = executionContext -> getBrowser ();
	getBrowser () -> prepareEvents () .addInterest (this, &TextureLoader::prepareEvents);
	getBrowser () -> addEvent ();
}

std::future <TexturePtr>
TextureLoader::getFuture (const MFString & url,
                          const size_t minTextureSize, const size_t maxTextureSize)
{
	if (url .empty ())
		std::async (std::launch::deferred, [ ] (){ return nullptr; });

	return std::async (std::launch::async, std::mem_fn (&TextureLoader::loadAsync), this,
	                   url,
	                   minTextureSize, maxTextureSize);
}

TexturePtr
TextureLoader::loadAsync (const MFString & url,
                          const size_t minTextureSize, const size_t maxTextureSize)
{
	for (const auto & URL : url)
	{
		try
		{
			checkForInterrupt ();

			const auto mutex = getBrowser () -> getDownloadMutex ();

			std::lock_guard <std::mutex> lock (*mutex);

			checkForInterrupt ();

			Loader loader (nullptr, referer);

			TexturePtr texture (new Texture (loader .loadDocument (URL)));

			checkForInterrupt ();

			texture -> process (minTextureSize, maxTextureSize);

			checkForInterrupt ();

			getBrowser () -> println ("Done loading image '", loader .getWorldURL (), "'.");

			checkForInterrupt ();

			return texture;
		}
		catch (const X3DError & error)
		{
			getBrowser () -> println (error .what ());
		}
		catch (const InterruptFutureException &)
		{
			throw;
		}
		catch (const std::exception & error)
		{
			getBrowser () -> println ("Bad Image: ", error .what (), ", in URL '", referer .transform (URL .str ()), "'.");
		}
	}

	return nullptr;
}

void
TextureLoader::prepareEvents ()
{
	if (isStopping ())
		return;

	getBrowser () -> addEvent ();

	if (not future .valid ())
	   return;

	const auto status = future .wait_for (std::chrono::milliseconds (0));

	if (status not_eq std::future_status::ready)
	   return;

	callback (future .get ());

	X3DInput::dispose ();
}

void
TextureLoader::dispose ()
{
	if (isStopping ())
		return;

	stop ();

	X3DInput::dispose ();

	// Clear the bound callback arguments.

	callback = [ ] (const TexturePtr &) { };
}

TextureLoader::~TextureLoader ()
{
	dispose ();

	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
