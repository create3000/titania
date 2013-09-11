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

#include "../Browser/X3DBrowser.h"
#include "../Miscellaneous/GoldenGate.h"

namespace titania {
namespace X3D {

Loader::Loader (X3DExecutionContext* const executionContext) :
	Loader (executionContext, executionContext -> getWorldURL ())
{ }

Loader::Loader (X3DExecutionContext* const executionContext, const basic::uri & referer) :
	executionContext (executionContext),
	         referer (referer),
	        worldURL (),
	        urlError ()
{ }

//  X3D Creation Handling

X3DSFNode <Scene>
Loader::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::istringstream istringstream (string);

	return createX3DFromStream (istringstream);
}

X3DSFNode <Scene>
Loader::createX3DFromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return createX3DFromStream (getReferer (), istream);
}

X3DSFNode <Scene>
Loader::createX3DFromStream (const basic::uri & worldURL, std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DSFNode <Scene> scene = getBrowser () -> createScene ();

	basic::ifilestream goldenistream = golden_gate ("<stream>", std::move (istream));

	scene -> fromStream (worldURL, goldenistream);

	scene -> realize ();

	return scene;
}

X3DSFNode <Scene>
Loader::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	X3DSFNode <Scene> scene = getBrowser () -> createScene ();

	parseIntoScene (scene, url);

	scene -> realize ();

	return scene;
}

void
Loader::loadURL (const MFString & url, const MFString & parameter)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	for (const auto & URL : url)
	{
		try
		{
			basic::uri uri = URL .str ();

			if (uri .filename () .length ())
				getBrowser () -> loadURL ({ getReferer () .transform (uri) .str () }, parameter);

			else
				getExecutionContext () -> changeViewpoint (uri .fragment ());

			break;
		}
		catch (const X3DError &)
		{ }
	}
}

void
Loader::parseIntoScene (X3DScene* const scene, const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (url .empty ())
		throw Error <INVALID_URL> ("No URL given.");

	for (const auto & URL : url)
	{
		try
		{
			basic::uri uri = URL .str ();

			basic::ifilestream goldenistream = golden_gate (URL .str (), loadStream (uri));

			scene -> fromStream (worldURL, goldenistream);

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

	throw Error <URL_UNAVAILABLE> ("Couldn't load any URL of " + url .toString () + ":\n" + error .str ());
}

//  Stream Handling

std::string
Loader::loadDocument (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadDocument (URL .str ());
}

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

basic::ifilestream
Loader::loadStream (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadStream (URL .str ());
}

basic::ifilestream
Loader::loadStream (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (uri .empty ())
		throw Error <INVALID_URL> ("Couldn't load URL '" + uri + "'.");

	if (uri .scheme () not_eq "data")
		std::clog << "Trying to load URI '" << uri << "': " << std::endl;

	basic::uri transformedURL = getReferer () .transform (uri);

	if (transformedURL .scheme () not_eq "data")
		std::clog << "\tTransformed URL is '" << transformedURL << "'" << std::endl;

	basic::ifilestream istream (transformedURL, 15000);

	if (istream)
	{
		istream .request_header ("User-Agent", getBrowser () -> getUserAgent ());
		istream .request_header ("Referer",    getReferer () .filename ());
		istream .send ();

		if (istream)
		{
			worldURL = istream .url ();

			if (worldURL .scheme () not_eq "data")
			{
				std::clog << "\tLoaded URL is '" << worldURL << "': " << std::endl;
				std::clog << "Done." << std::endl;
			}

			return istream;
		}
	}

	std::clog << "Failed." << std::endl;

	throw Error <URL_UNAVAILABLE> ("Couldn't load URL '" + transformedURL + "'.\nStatus: " + std::to_string (istream .status ()) + ". " + istream .reason ());
}

} // X3D
} // titania
