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

#include <Titania/X3D.h>
#include <gtkmm.h>

using namespace titania;
using namespace titania::X3D;

namespace Test
{
class DrawingArea :
	public Gtk::DrawingArea
{
public:

	DrawingArea ();

	bool
	on_draw (const Cairo::RefPtr <Cairo::Context> & cairo);

};

DrawingArea::DrawingArea () :
	Gtk::DrawingArea ()
{ }

bool
DrawingArea::on_draw (const Cairo::RefPtr <Cairo::Context> & cairo)
{
	cairo -> set_source_rgb (1, 0, 0);
  	cairo -> move_to (0, 10);
  	cairo -> line_to (100, 10);
  	cairo -> stroke ();

	return false;
}

class Window :
	public Gtk::Window
{
public:

	Window ();

private:

	Gtk::Box box;

	X3D::BrowserPtr browser1;
	X3D::BrowserPtr browser2;
};

// file:///Users/holger/Desktop/X3D/beethoven/beethoven-no-normals.x3dv

Window::Window () :
	Gtk::Window (),
	   browser1 (X3D::createBrowser ({ "file:///Users/holger/Desktop/test.x3dv" })),
	   browser2 (X3D::createBrowser ({ "file:///Users/holger/Desktop/X3D/beethoven/beethoven-no-normals.x3dv" }))
{
	set_title ("Test GLArea");
	set_default_size (400, 200);
	set_size_request (400, 200);

	box .pack_start (*browser1);
	box .pack_start (*browser2);

	add (box);
	show_all ();
}

class Application :
	public Gtk::Application
{
public:

	Application (int & argc, char** & argv);

	virtual
	void
	on_activate () final override;

};

Application::Application (int & argc, char** & argv) :
	     Gtk::Application (argc, argv, "de.create3000.titania", Gio::APPLICATION_HANDLES_OPEN)
{
	Glib::set_application_name ("Titania");
}

void
Application::on_activate ()
{
	static Window* window = nullptr;

	if (!window)
	{
		window = new Window ();

		add_window (*window);
	}

	window -> present ();
}

} // namespace Test

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;
	std::cout << std::boolalpha;
	std::clog << std::boolalpha;
	std::cerr << std::boolalpha;
	std::cout << std::setprecision (std::numeric_limits <float>::digits10);
	std::clog << std::setprecision (std::numeric_limits <float>::digits10);
	std::cerr << std::setprecision (std::numeric_limits <float>::digits10);
	//std::cout << std::setprecision (std::numeric_limits <double>::digits10);
	//std::clog << std::setprecision (std::numeric_limits <double>::digits10);
	//std::cerr << std::setprecision (std::numeric_limits <double>::digits10);

	//std::cout .imbue (std::locale (""));
	//std::clog .imbue (std::locale (""));
	//std::cerr .imbue (std::locale (""));

	//std::locale::global (std::locale (""));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	X3D::Init (argc, argv);

	Test::Application (argc, argv) .run ();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
