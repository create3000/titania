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

#include "../../Browser/X3DBrowser.h"

#include <Titania/Basic/URI.h>

#include <fstream>
#include <iostream>
#include <sstream>

namespace titania {
namespace X3D {

URNIndex X3DUrlObject::URNCache;

X3DUrlObject::X3DUrlObject () :
	X3DBaseNode (),                  
	        url (),                  // MFString [in,out] url               [ ]       [URL]
	   urlError (),                  // MFString [out]    urlError                    [BrowserEvent]
	  loadState (NOT_STARTED_STATE), 
	  userAgent (),                  
	   worldURL ()                   
{
	addNodeType (X3DConstants::X3DUrlObject);

	setChildren (urlError);
}

void
X3DUrlObject::initialize ()
{
	userAgent = getBrowser () -> getName () + "/" + getBrowser () -> getVersion () + " (X3D Browser; +http://titania.create3000.de)";
}

//  Element Access

LoadState
X3DUrlObject::checkLoadState ()
{
	return loadState;
}

void
X3DUrlObject::setLoadState (LoadState value)
{
	loadState = value;
}

void
X3DUrlObject::setWorldURL (const basic::uri & value)
{
	worldURL = value;
}

const basic::uri &
X3DUrlObject::getWorldURL ()
{
	return worldURL;
}

//  Load Operations

void
X3DUrlObject::requestImmediateLoad ()
{ }

//  X3D Creation Handling

SFNode <Scene>
X3DUrlObject::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::istringstream istringstream (string);

	return getBrowser () -> getExecutionContext () -> create (worldURL, istringstream);
}

SFNode <Scene>
X3DUrlObject::createX3DFromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getBrowser () -> getExecutionContext () -> create (worldURL, istream);
}

void
X3DUrlObject::createX3DFromURL (const MFString & url, const SFNode <X3DBaseNode> & node, const basic::id & fieldName)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{ }

SFNode <Scene>
X3DUrlObject::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	SFNode <Scene> scene = getBrowser () -> createScene ();

	parseIntoScene (*scene, url);

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
				getBrowser () -> loadURL ({ transformURI (uri) .str () }, parameter);

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
		urlError .set ({ });

		for (const auto & URL : url)
		{
			try
			{
				basic::ifilestream stream = loadStream (URL);

				scene -> fromStream (worldURL, stream);

				return;
			}
			catch (const X3DError & error)
			{
				urlError .push_back (error .what ());
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

static
void
print_uri (const basic::uri &);

basic::ifilestream
X3DUrlObject::loadStream (const SFString & URL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	return loadStream (URL .str ());
}

basic::ifilestream
X3DUrlObject::loadStream (const basic::uri & uri)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	std::clog << "\tTrying to load URI '" << uri << "': " << std::endl;

	basic::uri transformedURL = transformURI (uri);
	std::clog << "\t\tResolved URL is '" << transformedURL << "'" << std::endl;

	basic::ifilestream stream (basic::http::GET, transformedURL);

	if (stream)
	{
		stream .request_header ("User-Agent", userAgent);
		stream .request_header ("Referer",    getExecutionContext () -> getWorldURL ());
		stream .send ();

		if (uri .is_network ())
		{
			worldURL = uri;
		}
		else
			worldURL = transformedURL;

		std::clog << "\t\tLoaded URL is '" << worldURL << "': " << std::endl;
		std::clog << "\tDone." << std::endl;

		return stream;
	}

	std::clog << "\tFailed." << std::endl;

	throw Error <URL_UNAVAILABLE> ("Couldn't load URL '" + transformedURL + "'");
}

//  URI Handling

MFString
X3DUrlObject::transformURI (const MFString & uri)
{
	MFString url;

	for (const auto & URI : uri)
		url .push_back (transformURI (URI .str ()) .str ());

	return url;
}

basic::uri
X3DUrlObject::transformURI (const basic::uri & uri)
{
	if (uri .is_absolute () and uri .is_network ())
		return getURL (uri);

	const basic::uri & base = getURL (getExecutionContext () -> getWorldURL ());

	std::clog << "\t\tWorld URL is '" << base << "'" << std::endl;

	auto transformedURL = base .transform (uri);

	//	print_uri (uri);
	//	print_uri (base);
	//	print_uri (transformedURL);

	return transformedURL;
}

//  URN Handling

void
X3DUrlObject::addURN (const basic::uri & urn, const basic::uri & uri)
{
	URNCache .insert (std::make_pair (urn .filename (), uri));
}

void
X3DUrlObject::removeURN (const basic::uri & urn)
{
	URNCache .erase (urn .filename ());
}

basic::uri
X3DUrlObject::getURL (const basic::uri & uri)
{
	const auto urn = URNCache .find (uri .filename ());

	if (urn not_eq URNCache .end ())
	{
		basic::uri url = urn -> second;
		url .query    (uri .query ());
		url .fragment (uri .fragment ());
		return url;
	}

	return uri;
}

const URNIndex &
X3DUrlObject::getURNs ()
{
	return URNCache;
}

void
X3DUrlObject::dispose ()
{ }

static
void
print_uri (const basic::uri & uri)
{
	std::clog << std::endl;
	std::clog << "uri:    '" << uri << "'" << std::endl;
	std::clog << "copy:   '" << basic::uri (uri) << "'" << std::endl;

	std::clog << "is equal:     " << std::boolalpha << (uri == uri) << std::endl;
	std::clog << "is absolute:  " << uri .is_absolute () << std::endl;
	std::clog << "is relative:  " << uri .is_relative () << std::endl;
	std::clog << "is local:     " << uri .is_local () << std::endl;
	std::clog << "is directory: " << uri .is_directory () << std::endl;
	std::clog << "is file:      " << uri .is_file () << std::endl;

	std::clog << "Scheme:        " << uri .scheme () << std::endl;
	std::clog << "Authority:     " << uri .authority () << std::endl;
	std::clog << "HierarchyPart: " << uri .hierarchy () << std::endl;
	std::clog << "Host:          " << uri .host () << std::endl;
	std::clog << "Port:          " << uri .port () << std::endl;
	std::clog << "WellKnownPort: " << uri .well_known_port () << std::endl;
	std::clog << "Path:          " << uri .path () << std::endl;
	std::clog << "Query:         " << uri .query () << std::endl;
	std::clog << "Fragment:      " << uri .fragment () << std::endl;

	std::clog << "Root:     " << uri .root () << std::endl;
	std::clog << "Base:     " << uri .base () << std::endl;
	std::clog << "Parent:   " << uri .parent () << std::endl;

	std::clog << "Filename: " << uri .filename () << std::endl;
	std::clog << "Basename: " << uri .basename () << std::endl;
	std::clog << "Suffix:   " << uri .suffix () << std::endl;
}

} // X3D
} // titania
