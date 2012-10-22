/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3D.h"

#include <iostream>

namespace titania {
namespace X3D {

///  6.2.2 The getBrowser service returns a reference to an instance of an X3D browser through which other service
///  requests may be processed.  This is a unique identifier per application instance.
const SFNode <Browser> &
getBrowser (/* parameter */)
throw (Error <BROWSER_UNAVAILABLE>)
{
	static SFNode <Browser> browser = createBrowser ();
	return browser;
}

///  6.2.3 The createBrowser service creates a new instance of a browser application.
SFNode <Browser>
createBrowser (/* parameter */)
throw (Error <BROWSER_UNAVAILABLE>)
{
	std::clog << "Creating Browser ..." << std::endl;
	SFNode <Browser> browser = new Browser ();

	//browser -> setup ();

	// XXX
	//browser -> replaceWorld (browser -> createScene ());

	std::clog << "\tDone creating Browser." << std::endl;
	return browser;
}

///  6.2.3 The createBrowser service creates a new instance of a browser application.
SFNode <Browser>
createBrowser (const MFString & url)
throw (Error <BROWSER_UNAVAILABLE>)
{
	SFNode <Browser> browser = createBrowser ();

	browser -> url = url;

	return browser;
}

} // X3D
} // Titania
