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

#include "Loader.h"

#include "../../X3D.h"
#include "../Browser/X3DBrowser.h"
#include "../Execution/X3DScene.h"
#include "../InputOutput/GoldenGate.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace X3D {

Loader::Loader (X3DExecutionContext* const executionContext) :
	Loader (executionContext, executionContext -> getWorldURL ())
{ }

Loader::Loader (X3DExecutionContext* const executionContext, const basic::uri & referer) :
	executionContext (executionContext),
	       userAgent (X3D::getBrowser () -> getUserAgent ()),
	         referer (referer),
	        worldURL (),
	        urlError ()
{ }

//  X3D Creation Handling

X3DScenePtr
Loader::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	basic::ifilestream istream (string);

	return createX3DFromStream (istream);
}

X3DScenePtr
Loader::createX3DFromStream (basic::ifilestream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return createX3DFromStream (referer, istream);
}

X3DScenePtr
Loader::createX3DFromStream (const basic::uri & worldURL, basic::ifilestream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	const X3DScenePtr scene = executionContext -> getBrowser () -> createScene ();

	golden_gate (scene, worldURL, std::move (istream));

	scene -> setExecutionContext (executionContext);
	scene -> isLive () = executionContext -> isLive ();
	scene -> isPrivate (executionContext -> getScene () -> isPrivate ());

	scene -> isCompressed (istream .is_compressed ());
	scene -> setup ();

	return scene;
}

X3DScenePtr
Loader::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	const X3DScenePtr scene = executionContext -> getBrowser () -> createScene ();

	parseIntoScene (scene, url);

	scene -> setExecutionContext (executionContext);
	scene -> isLive () = executionContext -> isLive ();
	scene -> isPrivate (executionContext -> getScene () -> isPrivate ());
	scene -> setup ();

	return scene;
}

void
Loader::loadURL (const MFString & url, const MFString & parameter)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (url .empty ())
		return;

	for (const auto & URL : url)
	{
		try
		{
			const basic::uri uri = URL .str ();

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
 */
void
Loader::parseIntoScene (const X3DScenePtr & scene, const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
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
			basic::ifilestream istream = loadStream (URL .str ());

			golden_gate (scene, worldURL, std::move (istream));

			scene -> isCompressed (istream .is_compressed ());
			return;
		}
		catch (const X3DError & error)
		{
			urlError .emplace_back (error .what ());
		}
	}

	std::ostringstream error;

	for (const auto & string : urlError)
		error << string .str ();

	throw Error <URL_UNAVAILABLE> (error .str () + "\nCouldn't load any URL of " + url .toString () + "\n");
}

//  Stream Handling

/***
 *
 *  thread save
 *
 */
std::string
Loader::loadDocument (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadDocument (URL .str ());
}

/***
 *
 *  thread save
 *
 */
std::string
Loader::loadDocument (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	basic::ifilestream istream = loadStream (uri);

	std::ostringstream ostringstream;

	ostringstream << istream .rdbuf ();

	return ostringstream .str ();
}

/***
 *
 *  thread save
 *
 */
basic::ifilestream
Loader::loadStream (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadStream (URL .str ());
}

/***
 *
 *  thread save
 *
 */
basic::ifilestream
Loader::loadStream (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (uri .empty ())
		throw Error <INVALID_URL> ("Couldn't load URL '" + uri + "'.");

	const bool data = uri .scheme () == "data";

	//if (not data)
	//	std::clog << "Trying to load URI '" << uri << "': " << std::flush;

	basic::ifilestream istream (data ? uri : referer .transform (uri), 16000);

	//if (not data)
	//	std::clog << "\tTransformed URL is '" << istream .url () << "'" << std::endl;

	if (istream)
	{
		istream .request_header ("User-Agent", userAgent);
		istream .request_header ("Referer",    referer .filename ());
		istream .send ();

		if (istream)
		{
			if (not data)
			{
				worldURL = istream .url ();

				//std::clog << "\tLoaded URL is '" << worldURL << "': " << std::endl;
				//std::clog << "Done." << std::endl;
			}
			else
				worldURL = referer;

			return istream;
		}
	}

	std::ostringstream osstream;

	osstream
		<< "Couldn't load URL '" << istream .url () << "'." << std::endl
		<< "Status: " << basic::to_string (istream .status ()) << ", " << istream .reason () << std::endl;

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
