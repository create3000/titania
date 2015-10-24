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

#include <Titania/X3D.h>
#include <Titania/OS/env.h>

namespace titania {
namespace puck {

class BrowserApplication :
	public Gtk::Application
{
public:

	///  @name Construction
	
	BrowserApplication (int & argc, char** & argv) :
		Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN),
		   browserWindow ()
	{ }


private:

	///  @name Operations
	
	void
	realize ()
	{
		browserWindow .reset (new BrowserWindow (X3D::createBrowser ()));

		add_window (browserWindow -> getWindow ());

		browserWindow -> getWindow () .present ();	
	}

	///  @name Event handlers

	virtual
	void
	on_activate () final override
	{
		if (browserWindow)
		{
			browserWindow -> blank ();
			browserWindow -> getWindow () .present ();
		}
		else
			realize ();
	}

	virtual
	void
	on_open (const Gio::Application::type_vec_files & files, const Glib::ustring & hint) final override
	{
		if (not browserWindow)
			realize ();

		for (const auto & file : files)
			browserWindow -> open (Glib::uri_unescape_string (file -> get_uri ()));

		browserWindow -> getWindow () .present ();
	}

	virtual
	void
	on_window_removed (Gtk::Window* window) final override
	{
		quit ();
	}	

	///  @name Members

	std::unique_ptr <BrowserWindow> browserWindow;

};

} // puck
} // titania

int
main (int argc, char** argv)
{
	using namespace titania;
	using namespace titania::puck;

	#ifdef TITANIA_DEBUG
	std::clog
		<< std::boolalpha
		<< "Titania started ..." << std::endl
		<< " Compiled at " << __DATE__ << " " << __TIME__ << std::endl
		<< std::endl;
	#endif

	std::locale::global (std::locale (""));

	os::env ("LIBOVERLAY_SCROLLBAR", "0"); // XXX: This fixes the bug with modal windows.
	os::env ("UBUNTU_MENUPROXY",     "0"); // XXX: This fixes the bug with check button menu items.

	{
		BrowserApplication browserApplication (argc, argv);

		browserApplication .run ();
	}

	#ifdef TITANIA_DEBUG
	std::clog
		<< std::endl
		<< "Titania finished." << std::endl;
	#endif

	return 0;
}
