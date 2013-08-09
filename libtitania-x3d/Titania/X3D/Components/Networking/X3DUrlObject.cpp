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

#include "X3DUrlObject.h"

#include "../../Miscellaneous/GoldenGate.h"
#include "../../Browser/X3DBrowser.h"
#include <Titania/Basic/URI.h>

#include <atomic>
#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>

namespace titania {
namespace X3D {

X3DUrlObject::Fields::Fields () :
	url (new MFString ()),
	loadState (NOT_STARTED_STATE),
	urlError ()
{ }

X3DUrlObject::X3DUrlObject () :
  	X3DBaseNode (),
	       fields (),
	    userAgent (),
	     worldURL (),
	        mutex ()
{
	addNodeType (X3DConstants::X3DUrlObject);

	addChildren (loadState (), urlError ());
}

void
X3DUrlObject::initialize ()
{
	userAgent = getBrowser () -> getName () + "/" + getBrowser () -> getVersion () + " (X3D Browser; +http://titania.create3000.de)";
}

//  X3D Creation Handling

X3DSFNode <Scene>
X3DUrlObject::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::istringstream istringstream (string);

	return createX3DFromStream (istringstream);
}

X3DSFNode <Scene>
X3DUrlObject::createX3DFromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DSFNode <Scene> scene = getBrowser () -> createScene ();

	scene -> fromStream (getReferer (), istream);

	return scene;
}

X3DSFNode <Scene>
X3DUrlObject::createX3DFromStream (const basic::uri & worldURL, std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DSFNode <Scene> scene = getBrowser () -> createScene ();

	scene -> fromStream (worldURL, istream);

	return scene;
}

void
X3DUrlObject::createX3DFromURL (const MFString & url, const SFNode & node, const std::string & fieldName)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{ }

X3DSFNode <Scene>
X3DUrlObject::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	X3DSFNode <Scene> scene = getBrowser () -> createScene ();

	parseIntoScene (scene, url);

	return scene;
}

void
X3DUrlObject::loadURL (const MFString & url, const MFString & parameter)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	for (const auto & URL : url)
	{
		try
		{
			basic::uri uri = URL .str ();

			if (uri .filename () .length ())
				getBrowser () -> loadURL ({ transformUri (uri) .str () }, parameter);

			else
				getExecutionContext () -> changeViewpoint (uri .fragment ());

			break;
		}
		catch (const X3DError &)
		{ }
	}
}

void
X3DUrlObject::parseIntoScene (X3DScene* const scene, const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (url .size ())
	{
		urlError () .set ({ });

		for (const auto & URL : url)
		{
			try
			{
				basic::uri uri = URL .str ();
			
				basic::ifilestream istream  = golden_gate (URL .str (), loadStream (uri));

				scene -> fromStream (worldURL, istream);

				return;
			}
			catch (const X3DError & error)
			{
				urlError () .emplace_back (error .what ());
			}
		}

		throw Error <URL_UNAVAILABLE> ("Couldn't load any URL of " + url .toString () + ".");
	}
	else
		throw Error <INVALID_URL> ("No URL given.");
}

//  Stream Handling

std::string
X3DUrlObject::loadDocument (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadDocument (URL .str ());
}

std::string
X3DUrlObject::loadDocument (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	basic::ifilestream stream = loadStream (uri);

	std::ostringstream ostringstream;

	ostringstream << stream .rdbuf ();

	return ostringstream .str ();
}

basic::ifilestream
X3DUrlObject::loadStream (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	std::lock_guard <std::mutex> lock (mutex);

	if (uri .empty ())
		throw Error <INVALID_URL> ("Couldn't load URL '" + uri + "'");

	std::clog << "Trying to load URI '" << uri << "': " << std::endl;

	basic::uri transformedURL = transformUri (uri);
	std::clog << "\tTransformed URL is '" << transformedURL << "'" << std::endl;

	basic::ifilestream stream (basic::http::GET, transformedURL, 15000);

	if (stream)
	{
		stream .request_header ("User-Agent", userAgent);
		stream .request_header ("Referer",    getReferer () .filename ());
		stream .send ();

		if (stream)
		{
			setWorldURL (stream .url ());

			std::clog << "\tLoaded URL is '" << getWorldURL () << "': " << std::endl;
			std::clog << "Done." << std::endl;

			return stream;
		}
	}

	std::clog << "Failed." << std::endl;

	throw Error <URL_UNAVAILABLE> ("Couldn't load URL '" + transformedURL + "'\nStatus: " + std::to_string (stream .status ()) + " " + stream .reason ());
}

//  URI Handling

MFString
X3DUrlObject::transformUri (const MFString & uri)
{
	return transformUri (getReferer (), uri);
}

MFString
X3DUrlObject::transformUri (const basic::uri & base, const MFString & uri)
{
	MFString url;

	for (const auto & URI : uri)
		url .emplace_back (transformUri (base, URI .str ()) .str ());

	return url;
}

basic::uri
X3DUrlObject::transformUri (const basic::uri & uri)
{
	return transformUri (getReferer (), uri);
}

basic::uri
X3DUrlObject::transformUri (const basic::uri & base, const basic::uri & uri)
{
	std::clog << "\t\tWorld URL is '" << base << "'" << std::endl;

	return base .transform (uri);
}

void
X3DUrlObject::dispose ()
{ }

} // X3D
} // titania
