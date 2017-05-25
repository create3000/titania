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

#include "Console.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"
#include "../../Widgets/Footer/Footer.h"

namespace titania {
namespace puck {

Console::Console (X3DBrowserWindow* const browserWindow) :
	   X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DConsoleInterface (get_ui ("Widgets/Console.glade")),
	        scrollToEnd (true)
{
	setup ();
}

void
Console::on_map ()
{
	set_enabled ();
}

void
Console::on_unmap ()
{
	set_enabled ();
}

bool
Console::isEnabled () const
{
	return not getSuspendButton () .get_active () and getWidget () .get_mapped ();
}

void
Console::on_suspend_button_toggled ()
{
	set_enabled ();
}

void
Console::on_clear_button_clicked ()
{
	getTextBuffer () -> set_text ("");
}

void
Console::set_browser (const X3D::BrowserPtr & browser)
{
	browser -> getConsole () -> getString () .addInterest (&Console::set_string, this);	
}

void
Console::set_enabled ()
{
	if (isEnabled ())
	{
		getCurrentBrowser () .addInterest (&Console::set_browser, this);

		set_browser (getCurrentBrowser ());
	}
	else
	{
		getCurrentBrowser () .removeInterest (&Console::set_browser, this);

		for (const auto & page : getBrowserWindow () -> getPages ())
			page -> getBrowser () -> getConsole () -> getString () .removeInterest (&Console::set_string, this);
	}
}

void
Console::set_string (const X3D::MFString & value)
{
	// Insert.

	for (const auto & string : value)
		getTextBuffer () -> insert (getTextBuffer () -> end (), string .getValue ());

	// Erase.

	static constexpr int CONSOLE_LIMIT = 1 << 20; // 2 MB

	if (getTextBuffer () -> size () > CONSOLE_LIMIT)
	{
		const int charOffset = getTextBuffer () -> size () - CONSOLE_LIMIT;

		getTextBuffer () -> erase (getTextBuffer () -> begin (), getTextBuffer () -> get_iter_at_offset (charOffset));
	}

	// Update TextView and scoll to end.

	int           buffer_x = 0;
	int           buffer_y = 0;
	int           line_top = 0;
	Gtk::TextIter lineY;

	getTextView () .window_to_buffer_coords (Gtk::TEXT_WINDOW_WIDGET, 0, getTextView () .get_height (), buffer_x, buffer_y); 
	getTextView () .get_line_at_y (lineY, buffer_y, line_top);

	if (lineY .get_line () > getTextBuffer () -> get_line_count () - 4)
		scrollToEnd = true;

	if (scrollToEnd)
	{
		// Update TextView and thus we can scoll to iter.
		while (Gtk::Main::events_pending ())
			Gtk::Main::iteration ();

		auto iter = getTextBuffer () -> end ();

		getTextView () .scroll_to (iter, 0, 0, 0);
	}
}

void
Console::on_mark_set (const Gtk::TextBuffer::iterator & location, const Glib::RefPtr <Gtk::TextBuffer::Mark> & mark)
{
	if (mark == getTextBuffer () -> get_insert ())
		scrollToEnd = false;
}

Console::~Console ()
{
	dispose ();
}

} // puck
} // titania
