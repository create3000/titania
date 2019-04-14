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

#include "FileLoader.h"

#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"
#include "../InputOutput/GoldenGate.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace X3D {

FileLoader::FileLoader (X3DExecutionContext* const executionContext) :
	FileLoader (executionContext, executionContext -> getWorldURL ())
{ }

FileLoader::FileLoader (X3DExecutionContext* const executionContext, const basic::uri & referer) :
	executionContext (executionContext),
	       userAgent (executionContext -> getBrowser () -> getUserAgent ()),
	         referer (referer),
	        worldURL (),
	        urlError (),
	         istream ()
{ }

//  X3D Creation Handling

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
FileLoader::createX3DFromString (const std::string & string)
{
	basic::ifilestream istream (string);

	return createX3DFromStream (istream);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
FileLoader::createX3DFromStream (basic::ifilestream & istream)
{
	return createX3DFromStream (referer, istream);
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
X3DScenePtr
FileLoader::createX3DFromStream (const basic::uri & worldURL, basic::ifilestream & istream)
{
	const X3DScenePtr scene = executionContext -> getBrowser () -> createScene (false);

	GoldenGate::read (istream, scene, worldURL);

	if (executionContext not_eq executionContext -> getBrowser ())
	{
		scene -> setExecutionContext (executionContext);
		scene -> setPrivate (executionContext -> getPrivate ());
	}

	scene -> isLive () = executionContext -> isLive ();
	scene -> setup ();

	return scene;
}

///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
X3DScenePtr
FileLoader::createX3DFromURL (const MFString & url)
{
	const auto scene = executionContext -> getBrowser () -> createScene (false);

	parseIntoScene (scene, url);

	if (executionContext not_eq executionContext -> getBrowser ())
	{
		scene -> setExecutionContext (executionContext);
		scene -> setPrivate (executionContext -> getPrivate ());
	}

	scene -> isLive () = executionContext -> isLive ();
	scene -> setup ();

	return scene;
}

///  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
void
FileLoader::loadURL (const MFString & url, const MFString & parameter)
{
	if (url .empty ())
		return;

	for (const auto & URL : url)
	{
		try
		{
			const basic::uri uri (URL);

			if (uri .filename (true) .empty ())
				executionContext -> changeViewpoint (uri .fragment ());

			else
				executionContext -> getBrowser () -> loadURL ({ referer .transform (uri) .str () }, parameter);

			return;
		}
		catch (const X3DError &)
		{ }
	}

	if (url .size () == 1)
		throw Error <URL_UNAVAILABLE> ("Couldn't load URL " + url .toString ());

	throw Error <URL_UNAVAILABLE> ("Couldn't load any URL of " + url .toString ());
}

/***
 *
 *  thread save
 *
 *  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
 */
void
FileLoader::parseIntoScene (const X3DScenePtr & scene, const MFString & url)
{
	if (url .empty ())
	{
		urlError .emplace_back ("No URL given.");

		throw Error <INVALID_URL> ("Invalid url.");
	}

	for (const auto & URL : url)
	{
		try
		{
			loadStream (URL .raw (), istream);

			GoldenGate::read (istream, scene, worldURL);

			istream .close ();
			return;
		}
		catch (const X3DError & error)
		{
			executionContext -> getBrowser () -> getConsole () -> error (error .what ());

			urlError .emplace_back (error .what ());

			istream .close ();
		}
	}

	throw Error <URL_UNAVAILABLE> ("Couldn't load any URL of " + url .toString () + "\n");
}

/***
 *
 *  thread save if calling parseIntoScene
 *
 */
void
FileLoader::stop ()
{
	istream .stop ();
}

//  Stream Handling

/***
 *
 *  thread save
 *
 *  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
 */
std::string
FileLoader::loadDocument (const basic::uri & uri)
{
	loadStream (uri, istream);

	std::ostringstream ostringstream;

	ostringstream << istream .rdbuf ();

	istream .close ();

	return ostringstream .str ();
}

/***
 *
 *  thread save
 *
 *  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
 */
basic::ifilestream
FileLoader::loadStream (const basic::uri & uri)
{
	loadStream (uri, istream);

	return std::move (istream);
}

/***
 *
 *  thread save
 *
 *  throws Error <INVALID_URL>, Error <URL_UNAVAILABLE>
 */
void
FileLoader::loadStream (const basic::uri & uri, basic::ifilestream & istream)
{
	if (uri .empty ())
		throw Error <INVALID_URL> ("Couldn't load URL '" + uri + "'.");

	if (istream .stopping ())
		return;

	const auto transformed = referer .transform (uri);

	istream .open (transformed, 30'000);

	if (istream)
	{
		istream .request_header ("User-Agent", userAgent);
		istream .request_header ("Referer",    referer .filename ());
		istream .send ();

		if (istream)
		{
			if (transformed .scheme () not_eq "data")
			{
				worldURL = transformed;

				//std::clog << "\tLoaded URL is '" << worldURL << "': " << std::endl;
				//std::clog << "Done." << std::endl;
			}
			else
			{
				worldURL = referer;
			}

			return;
		}
	}

	std::ostringstream osstream;

	osstream
		<< "Couldn't load URL '" << istream .url () << "'." << std::endl
		<< "Status: " << basic::to_string (istream .status (), std::locale::classic ()) << ", " << istream .reason () << std::endl;

	switch (istream .status ())
	{
		case 300: // Multiple Choices
		case 301: // Moved Permanently
		case 302: // Found
		case 303: // See Other
		//case 304: // Not Modified
		case 305: // Use Proxy
		//case 306: // (reserviert)
		case 307: // Temporary Redirect
		case 308: // Permanent Redirect
		{
			try
			{
				std::string location = "Location: '" + istream .response_headers () .at ("Location") + "'\n";
				osstream << location;
			}
			catch (...)
			{ }
		}
	}

	#ifdef TITANIA_DEBUG
	std::clog << osstream .str ();
	#endif

	throw Error <URL_UNAVAILABLE> (osstream .str ());
}

} // X3D
} // titania
