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

#include "BrowserApplication.h"

#include "../../Bits/File.h"
#include "../../Browser/BrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

BrowserApplication::BrowserApplication (int & argc, char** & argv) :
	     Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN),
	X3DBrowserApplication (),
	     X3DDBusInterface ()
{
	Glib::set_application_name ("Titania");
}

int
BrowserApplication::main (int argc, char** argv)
{
	#ifdef TITANIA_DEBUG
	std::clog
		<< std::boolalpha
		<< "Titania started ..." << std::endl
		<< " Compiled at " << __DATE__ << " " << __TIME__ << std::endl
		<< std::endl;
	#endif

	{ BrowserApplication (argc, argv) .run (); }

	#ifdef TITANIA_DEBUG
	std::clog
		<< std::endl
		<< "Titania finished." << std::endl;
	#endif

	return 0;
}

void
BrowserApplication::realize ()
{
	if (getRealized ())
		return;

	X3DBrowserApplication::realize ();
	X3DDBusInterface::realize ();
}

void
BrowserApplication::on_open (const Gio::Application::type_vec_files & files, const Glib::ustring & hint)
{
	for (const auto & file : files)
		getBrowserWindow () -> open (File::getUri (file));

	getBrowserWindow () -> present ();

	//Call the base class's implementation:
	Gtk::Application::on_open (files, hint);
}

void
BrowserApplication::on_window_removed (Gtk::Window* window)
{
	quit ();
}

BrowserApplication::~BrowserApplication ()
{ }

} // puck
} // titania
