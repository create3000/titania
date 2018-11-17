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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#include <Titania/Basic/URI.h>
//#include <iostream>
//
//namespace titania {
//
//class TestURI
//{
//public:
//
//	TestURI ()
//	{
//		std::clog << std::endl;
//		test_uri (basic::uri ());
//		test_uri ("");
//
//		std::clog << std::endl;
//		test_uri ("//");
//		test_uri ("//test.create3000.de:80/path/to/file");
//		test_uri ("test.create3000.de:80/path/to/file");
//		test_uri (":80/path/to/file");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de");
//		test_uri ("http://test.create3000.de?query=true");
//		test_uri ("http://test.create3000.de#fragment");
//		test_uri ("http://test.create3000.de?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de/");
//		test_uri ("http://test.create3000.de/?query=true");
//		test_uri ("http://test.create3000.de/#fragment");
//		test_uri ("http://test.create3000.de/?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de/path/to/file.html");
//		test_uri ("http://test.create3000.de/path/to/file.html?query=true");
//		test_uri ("http://test.create3000.de/path/to/file.html#fragment");
//		test_uri ("http://test.create3000.de/path/to/file.html?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de/path/to/file");
//		test_uri ("http://test.create3000.de/path/to/file?query=true");
//		test_uri ("http://test.create3000.de/path/to/file#fragment");
//		test_uri ("http://test.create3000.de/path/to/file?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de/path/to/file.html?");
//		test_uri ("http://test.create3000.de/path/to/file.html#");
//		test_uri ("http://test.create3000.de/path/to/file.html?#");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de:80");
//		test_uri ("http://test.create3000.de:80?query=true");
//		test_uri ("http://test.create3000.de:80#fragment");
//		test_uri ("http://test.create3000.de:80?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de:80/");
//		test_uri ("http://test.create3000.de:80/?query=true");
//		test_uri ("http://test.create3000.de:80/#fragment");
//		test_uri ("http://test.create3000.de:80/?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("http://test.create3000.de:80/path/to/file.html");
//		test_uri ("http://test.create3000.de:80/path/to/file.html?query=true");
//		test_uri ("http://test.create3000.de:80/path/to/file.html#fragment");
//		test_uri ("http://test.create3000.de:080/path/to/file.html?query=true#fragment");
//
//		std::clog << std::endl;
//		test_uri ("/");
//		test_uri ("/path/to/file.html");
//		test_uri ("/path/to/");
//
//		test_uri ("file://");
//		test_uri ("file:///");
//		test_uri ("file:///path/to/file.html");
//		test_uri ("file:///path/to/");
//
//		std::clog << std::endl;
//		test_uri ("path/to/file.html");
//		test_uri ("path/to/");
//
//		test_uri ("http://www.web3d.org/x3d/content/examples/Basic/Vrml97Specification/ChopperRotor.wrl");
//		test_uri ("file:///home/holger/Projekte/Titania/Library/Tests/Basic/EmptyWorld.wrl");
//
//		std::clog << std::endl;
//		test_uri ("about:blank");
//		test_uri ("urn:fantasy:123:345");
//		test_uri ("urn:iso:std:iso:9999:-1:ed-2:en:amd:1");
//
//		// assign
//		basic::uri uri;
//		uri = uri;
//		uri = "http://test.create3000.de:80/path/to/file.html?query=true#fragment";
//		uri = std::string ("http://test.create3000.de:80/path/to/file.html?query=true#fragment");
//	}
//
//	void
//	test_uri (const basic::uri & uri)
//	{
//		print_uri (uri);
//
//		std::clog << "Transform:  " << uri .transform ("http://test.create3000.de/path/to/child.html") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("http://www.web3d.org/x3d/content/examples/Basic/Vrml97Specification/ChopperRotor.wrl") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("file:///home/holger/Projekte/Titania/Library/Tests/Basic/EmptyWorld.wrl") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("/from/basename.suffix") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("../from/basename.suffix") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("//") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("//test.create3000.de/path/to/file") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("test.create3000.de/path/to/file") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("/path/to/file") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("//test.create3000.de:80/path/to/file") << std::endl;
//		std::clog << "Transform:  " << uri .transform ("test.create3000.de:80/path/to/file") << std::endl;
//		std::clog << "Transform:  " << uri .transform (":80/path/to/file") << std::endl;
//	}
//
//	void
//	print_uri (const basic::uri & uri)
//	{
//		std::clog << std::endl;
//
//		std::clog << "string: '" << uri .str () << "'" << std::endl;
//		std::clog << "uri:    '" << uri << "'" << std::endl;
//		std::clog << "copy:   '" << basic::uri (uri) << "'" << std::endl;
//
//		std::clog << "is equal:     " << std::boolalpha << (uri == uri) << std::endl;
//		std::clog << "is absolute:  " << uri .is_absolute () << std::endl;
//		std::clog << "is relative:  " << uri .is_relative () << std::endl;
//		std::clog << "is local:     " << uri .is_local () << std::endl;
//		std::clog << "is directory: " << uri .is_directory () << std::endl;
//		std::clog << "is file:      " << uri .is_file () << std::endl;
//
//		std::clog << "Scheme:        " << uri .scheme () << std::endl;
//		std::clog << "Authority:     " << uri .authority () << std::endl;
//		std::clog << "Hierarchy:     " << uri .hierarchy () << std::endl;
//		std::clog << "Host:          " << uri .host () << std::endl;
//		std::clog << "Port:          " << uri .port () << std::endl;
//		std::clog << "WellKnownPort: " << uri .well_known_port () << std::endl;
//		std::clog << "Path:          " << uri .path () << std::endl;
//		std::clog << "Query:         " << uri .query () << std::endl;
//		std::clog << "Fragment:      " << uri .fragment () << std::endl;
//
//		std::clog << "Root:     " << uri .root () << std::endl;
//		std::clog << "Base:     " << uri .base () << std::endl;
//		std::clog << "Parent:   " << uri .parent () << std::endl;
//
//		std::clog << "Filename: " << uri .filename () << std::endl;
//		std::clog << "Basename: " << uri .basename () << std::endl;
//		std::clog << "Basename: " << uri .basename (".html") << std::endl;
//		std::clog << "Basename: " << uri .basename ({ ".wrl", ".html" }) << std::endl;
//		std::clog << "Suffix:   " << uri .extension () << std::endl;
//	}
//
//};
//
//} // titania
