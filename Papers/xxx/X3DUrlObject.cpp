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

#include "X3DUrlObject.h"

#include "../../Browser/X3DBrowser.h"
#include <Titania/Basic/URI.h>

#include <atomic>
#include <glibmm/dispatcher.h>
#include <glibmm/thread.h>

namespace titania {
namespace X3D {

//class X3DUrlObject::Thread
//{
//public:
//
//	Thread () :
//		thread (NULL),
//		load (true)
//	{ }
//
//	Glib::Dispatcher &
//	getLoadTime ()
//	{ return loadTime; }
//
//	basic::ifilestream
//	getStream ()
//	{
//		Glib::Mutex::Lock lock (mutex);
//		return std::move (stream);
//	}
//
//	std::string
//	getUrlError ()
//	{
//		Glib::Mutex::Lock lock (mutex);
//		return std::move (urlError);
//	}
//
//	void
//	loadAsync (const std::string & ua, const basic::uri & ref, const SFString & URL)
//	{
//		load = false;
//
//		if (thread)
//			thread -> join ();
//
//		load      = true;
//		userAgent = ua;
//		referer   = ref;
//		uri       = URL .str ();
//		urlError .clear ();
//
//		thread = Glib::Thread::create (sigc::mem_fun (this, &Thread::run), true);
//	}
//
//	~Thread ()
//	{
//		load = false;
//
//		if (thread)
//			thread -> join ();
//	}
//
//private:
//
//	void
//	run ()
//	{
//		try
//		{
//			Glib::Mutex::Lock lock (mutex);
//			stream = X3DUrlObject::loadStream (userAgent, referer, uri, worldURL);
//
//			if (load)
//				loadTime ();
//		}
//		catch (const X3DError & error)
//		{
//			Glib::Mutex::Lock lock (mutex);
//			urlError = error .what ();
//		}
//	}
//
//	Glib::Thread*       thread;
//	Glib::Mutex         mutex;
//	Glib::Dispatcher    loadTime;
//	std::string         userAgent;
//	basic::uri          referer;
//	basic::uri          uri;
//	basic::uri          worldURL;
//	basic::ifilestream  stream;
//	std::string         urlError;
//	std::atomic <bool>  load;
//};

URNIndex X3DUrlObject::URNCache;

X3DUrlObject::Fields::Fields () :
	url (new MFString ()),
	//	document_changed (new SFString ()),
	loadTime (new SFTime ()),
	urlError (new MFString ())
{ }

X3DUrlObject::X3DUrlObject () :
	X3DBaseNode (),
	     fields (),
	  loadState (NOT_STARTED_STATE),
	  userAgent (),
	   worldURL ()
	//,thread (new Thread ())
{
	addNodeType (X3DConstants::X3DUrlObject);

	addChildren (/*document_changed (),*/ loadTime (), urlError ());
}

void
X3DUrlObject::initialize ()
{
	userAgent = getBrowser () -> getName () + "/" + getBrowser () -> getVersion () + " (X3D Browser; +http://titania.create3000.de)";

	//	thread -> getLoadTime () .connect (sigc::mem_fun (this, &X3DUrlObject::set_loadTime));
}

//  Element Access

void
X3DUrlObject::setLoadState (LoadState value)
{
	loadState = value;

	if (loadState == COMPLETE_STATE)
	{
		//		document_changed () .set ("");

		loadTime () = getCurrentTime ();
	}
}

//  X3D Creation Handling

SFNode <Scene>
X3DUrlObject::createX3DFromString (const std::string & string)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::istringstream istringstream (string);

	return createX3DFromStream (istringstream);
}

SFNode <Scene>
X3DUrlObject::createX3DFromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <Scene> scene = getBrowser () -> createScene ();

	scene -> fromStream (getReferer (), istream);

	return scene;
}

void
X3DUrlObject::createX3DFromURL (const MFString & url, const SFNode <X3DBaseNode> & node, const std::string & fieldName)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{ }

SFNode <Scene>
X3DUrlObject::createX3DFromURL (const MFString & url)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	SFNode <Scene> scene = getBrowser () -> createScene ();

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
		urlError () .set ({ });

		for (const auto & URL : url)
		{
			try
			{
				basic::ifilestream stream = loadStream (URL .str ());

				scene -> fromStream (worldURL, stream);

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

//void
//X3DUrlObject::loadDocumentAsync (const SFString & URL)
//{
//	thread -> loadAsync (userAgent, getReferer (), URL);
//}
//
//void
//X3DUrlObject::set_loadTime ()
//{
//	std::string error = thread -> getUrlError ();
//
//	if (error .empty ())
//	{
//		std::ostringstream ostringstream;
//		ostringstream << thread -> getStream () .rdbuf ();
//
//		document_changed () = ostringstream .str ();
//	}
//	else
//		urlError () = { error };
//}

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
	return loadStream (userAgent, getReferer (), uri, worldURL);
}

basic::ifilestream
X3DUrlObject::loadStream (const std::string & userAgent, const basic::uri & referer, const basic::uri & uri, basic::uri & worldURL)
throw (Error <INVALID_URL>,
       Error <URL_UNAVAILABLE>)
{
	if (uri .empty ())
		throw Error <INVALID_URL> ("Couldn't load URL '" + uri + "'");

	std::clog << "Trying to load URI '" << uri << "': " << std::endl;

	basic::uri transformedURL = transformURI (referer, uri);
	std::clog << "\tResolved URL is '" << transformedURL << "'" << std::endl;

	basic::ifilestream stream (basic::http::GET, transformedURL);

	if (stream)
	{
		stream .request_header ("User-Agent", userAgent);
		stream .request_header ("Referer",    referer);
		stream .send ();

		if (uri .is_network ())
		{
			worldURL = uri;
		}
		else
			worldURL = transformedURL;

		std::clog << "\tLoaded URL is '" << worldURL << "': " << std::endl;
		std::clog << "Done." << std::endl;

		return stream;
	}

	std::clog << "Failed." << std::endl;

	throw Error <URL_UNAVAILABLE> ("Couldn't load URL '" + transformedURL + "'");
}

//  URI Handling

MFString
X3DUrlObject::transformURI (const MFString & uri)
{
	return transformURI (getReferer (), uri);
}

MFString
X3DUrlObject::transformURI (const basic::uri & base, const MFString & uri)
{
	MFString url;

	for (const auto & URI : uri)
		url .emplace_back (transformURI (base, URI .str ()) .str ());

	return url;
}

basic::uri
X3DUrlObject::transformURI (const basic::uri & uri)
{
	return transformURI (getReferer (), uri);
}

basic::uri
X3DUrlObject::transformURI (const basic::uri & _base, const basic::uri & uri)
{
	if (uri .is_absolute () and uri .is_network ())
		return getURL (uri);

	const basic::uri & base = getURL (_base);

	std::clog << "\t\tWorld URL is '" << base << "'" << std::endl;

	auto transformedURL = base .transform (uri);

	return transformedURL;
}

//  URN Handling

void
X3DUrlObject::addURN (const basic::uri & urn, const basic::uri & uri)
{
	URNCache .insert (std::pair (urn .filename (), uri));
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
{
	removeChildren (/*document_changed (),*/ loadTime (), urlError ());
}

} // X3D
} // titania
