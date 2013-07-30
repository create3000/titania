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

#include "Browser/BrowserWindow.h"
#include "Configuration/config.h"
#include <Titania/OS/Env.h>
#include <iostream>
#include <libintl.h>

using namespace titania;
using namespace titania::puck;

class BrowserApplication :
	public Gtk::Application
{
public:

	using Gtk::Application::add_window;

	BrowserApplication (int & argc, char** & argv) :
		Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN)
	{ }

	virtual
	void
	on_activate () final
	{
		__LOG__ << std::endl;

		if (browserWindows .empty ())
			add_window ("");

		else
			add_window (get_page ("about/blank.wrl"));
	}

	virtual
	void
	on_open (const Gio::Application::type_vec_files & files, const Glib::ustring & hint) final
	{
		__LOG__ << std::endl;

		for (const auto & file : files)
			add_window (Glib::uri_unescape_string (file -> get_uri ()));
	}

	virtual
	void
	on_window_removed (Gtk::Window* window) final
	{
		__LOG__ << std::endl;

		auto browserWindow = browserWindows .find (window);

		if (browserWindow not_eq browserWindows .end ())
			browserWindows .erase (browserWindow);

		if (browserWindows .empty ())
			quit ();

		__LOG__ << std::endl;
	}

	void
	add_window (const basic::uri & uri)
	{
		auto browserWindow = std::make_shared <BrowserWindow> (uri);

		browserWindow -> getWindow () .show ();

		add_window (browserWindow -> getWindow ());

		browserWindows .insert (std::make_pair (&browserWindow -> getWindow (), browserWindow));

		__LOG__ << browserWindow << std::endl;
	}

private:

	std::map <Gtk::Window*, std::shared_ptr <BrowserWindow>> browserWindows;

};

int
main (int argc, char** argv)
{
	std::clog
		<< "Titania started ..." << std::endl
		<< " Compiled at " << __DATE__ << " " << __TIME__ << std::endl
		<< std::endl;

	std::setlocale (LC_ALL, os::env ("LANG") .c_str ());

	//std::locale::global (std::locale (os::env ("LANG") .c_str ()));

	try
	{
		BrowserApplication browserWindow (argc, argv);

		browserWindow .run ();
	}
	catch (const std::exception & error)
	{
		std::clog << error .what () << std::endl;
	}
	catch (const Glib::Error & error)
	{
		std::clog << error .what () << std::endl;
	}

	std::clog
		<< std::endl
		<< "Titania finished." << std::endl;

	return 0;
}
