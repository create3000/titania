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

#include "X3D.h"

#include <gtkmm/main.h>
#include <js/Initialization.h>

namespace titania {
namespace X3D {

/***
 *  Init function must be the first X3D function call in a multi-threaded program, and it must complete before any other X3D call is made.
 */
void
Init (int argc, char** argv)
{
	#ifndef __APPLE__
	// XInitThreads function must be the first Xlib function a multi-threaded program calls, and it must complete before any other Xlib call is made.
	XInitThreads (); // GStreamer
	#endif

	// Replace the C++ global locale as well as the C locale with the user-preferred locale.
	std::locale::global (std::locale (""));

	// Initialize SpiderMonkey
	JS_Init ();
}

///  6.2.2 The getBrowser service returns a reference to an instance of an X3D browser through which other service
///  requests may be processed.  This is a unique identifier per application instance.
///  throws Error <BROWSER_UNAVAILABLE>
const BrowserApplicationPtr &
getBrowser (/* parameter */)
{
	try
	{
		static Gtk::Main kit (0, nullptr);

		static BrowserApplicationPtr browserApplication;

		if (not browserApplication)
		{
			browserApplication = new BrowserApplication ({ }, { });

			browserApplication -> setLoadUrlObjects (false);
			browserApplication -> setup ();
		}

		return browserApplication;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
		throw;
	}
}

///
///  The createBrowser service creates a new instance of a browser application.
///
///  throws Error <BROWSER_UNAVAILABLE>
BrowserPtr
createBrowser (const MFString & url, const MFString & parameter)
{
	try
	{
		return BrowserPtr (new Browser (url, parameter));
	}
	catch (const std::exception & error)
	{
		throw Error <BROWSER_UNAVAILABLE> (error .what ());
	}
}

///
///  The createBrowser service creates a new instance of a browser application.
///
///  throws Error <INVALID_NODE>, Error <BROWSER_UNAVAILABLE>
BrowserPtr
createBrowser (const BrowserPtr & sharedBrowser, const MFString & url, const MFString & parameter)
{
	try
	{
		if (not sharedBrowser)
			throw Error <INVALID_NODE> ("createBrowser: Shared browser is NULL.");

		return BrowserPtr (new Browser (sharedBrowser, url, parameter));
	}
	catch (const std::exception & error)
	{
		throw Error <BROWSER_UNAVAILABLE> (error .what ());
	}
}

} // X3D
} // titania
