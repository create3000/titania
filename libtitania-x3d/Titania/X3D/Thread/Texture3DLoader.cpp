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

#include "Texture3DLoader.h"

#include "../InputOutput/Loader.h"

namespace titania {
namespace X3D {

Texture3DLoader::Texture3DLoader (X3DExecutionContext* const executionContext,
                                  const MFString & url,
                                  const size_t minTextureSize, const size_t maxTextureSize,
                                  const Callback & callback) :
	        X3DInput (),
	         browser (executionContext -> getBrowser ()),
	         referer (executionContext -> getWorldURL ()),
	        callback (callback),
	         running (true),
	           mutex (),
	          future (getFuture (url, minTextureSize, maxTextureSize))
{
	browser -> prepareEvents () .addInterest (this, &Texture3DLoader::prepareEvents);
	browser -> addEvent ();
}

std::future <Texture3DPtr>
Texture3DLoader::getFuture (const MFString & url,
                            const size_t minTextureSize, const size_t maxTextureSize)
{
	if (url .empty ())
		std::async (std::launch::deferred, [ ] (){ return nullptr; });

	return std::async (std::launch::async, std::mem_fn (&Texture3DLoader::loadAsync), this,
	                   url,
	                   minTextureSize, maxTextureSize);
}

Texture3DPtr
Texture3DLoader::loadAsync (const MFString & url,
                            const size_t minTextureSize, const size_t maxTextureSize)
{
	for (const auto & URL : url)
	{
		try
		{
			if (running)
			{
				std::lock_guard <std::mutex> lock (browser -> getDownloadMutex ());

				Texture3DPtr texture;

				if (running)
					texture .reset (new Texture3D (Loader (nullptr, referer) .loadDocument (URL)));

				if (running)
					texture -> process (minTextureSize, maxTextureSize);

				return texture;
			}

			return nullptr;
		}
		catch (const X3DError & error)
		{
			std::clog << "ImageTexture: " << error .what () << std::endl;
		}
		catch (const std::exception & error)
		{
			std::clog
				<< "Bad Image: " << error .what () << ", "
				<< "in URL '" << referer .transform (URL .str ()) << "'"
				<< std::endl;
		}
	}

	return nullptr;
}

void
Texture3DLoader::prepareEvents ()
{
	if (running)
	{
		browser -> addEvent ();

		if (future .valid ())
		{
			const auto status = future .wait_for (std::chrono::milliseconds (0));

			if (status == std::future_status::ready)
			{
				callback (future .get ());
				
				dispose ();
			}
		}
	}
}

void
Texture3DLoader::dispose ()
{
	if (running)
	{
		running = false;

		X3DInput::dispose ();
		
		// Clear the bound callback arguments.

		callback = [ ] (const Texture3DPtr &) { };
	}
}

Texture3DLoader::~Texture3DLoader ()
{
	if (future .valid ())
		future .wait ();
}

} // X3D
} // titania
