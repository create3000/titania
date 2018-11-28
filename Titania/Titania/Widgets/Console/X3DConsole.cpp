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

#include <Titania/String.h>

namespace titania {
namespace puck {

static constexpr int32_t CONSOLE_LIMIT      = 10'000'000; // 10 MB
static constexpr int32_t INSERT_MARK_COUNT  = 2;
static constexpr int32_t SCROLL_COUNT       = 1;
static constexpr int32_t SCROLL_LINES_COUNT = 42;
static constexpr int32_t LAST_LINES_COUNT   = 2;

X3DConsole::X3DConsole () :
	X3DConsoleInterface (),
	        scrollToEnd (true)
{
	getTextView () .get_vadjustment () -> signal_value_changed () .connect (sigc::mem_fun (this, &Console::on_vadjustment_value_changed));
}

void
X3DConsole::setup ()
{
	const auto redTag    = getTextBuffer () -> create_tag ("red");
	const auto yellowTag = getTextBuffer () -> create_tag ("yellow");
	const auto blueTag   = getTextBuffer () -> create_tag ("blue");
	const auto greenTag  = getTextBuffer () -> create_tag ("green");

	redTag    -> property_foreground_set () = true;
	yellowTag -> property_foreground_set () = true;
	blueTag   -> property_foreground_set () = true;
	greenTag  -> property_foreground_set () = true;

	redTag    -> property_foreground_gdk () = Gdk::Color ("#e06c75");
	yellowTag -> property_foreground_gdk () = Gdk::Color ("#e5c07b");
	blueTag   -> property_foreground_gdk () = Gdk::Color ("#61afef");
	greenTag  -> property_foreground_gdk () = Gdk::Color ("#98c379");
}

void
X3DConsole::on_size_allocate (Gtk::Allocation & allocation)
{
	on_scroll_to_end ();
}

void
X3DConsole::on_scroll_to_end ()
{
	if (not scrollToEnd)
		return;

	const auto upper = getScrolledWindow () .get_vadjustment () -> get_upper ();

	getScrolledWindow () .get_vadjustment () -> set_value (upper);
}

void
X3DConsole::on_vadjustment_value_changed ()
{
	int           buffer_x = 0;
	int           buffer_y = 0;
	int           line_top = 0;
	Gtk::TextIter lineY;

	getTextView () .window_to_buffer_coords (Gtk::TEXT_WINDOW_WIDGET, 0, getTextView () .get_height (), buffer_x, buffer_y); 
	getTextView () .get_line_at_y (lineY, buffer_y, line_top);

	const auto lastLines = getTextBuffer () -> get_line_count () - lineY .get_line ();

	if (lastLines <= LAST_LINES_COUNT)
	{
		scrollToEnd = true;
	}
	else if (lastLines > SCROLL_LINES_COUNT)
	{
		scrollToEnd = false;
	}
}

void
X3DConsole::push (const std::vector <Glib::ustring> & tags, const Glib::ustring & string)
{
	// Append.

	getTextBuffer () -> insert_with_tags_by_name (getTextBuffer () -> end (), string, tags);

	// Erase.

	if (getTextBuffer () -> size () > CONSOLE_LIMIT)
	{
		const int charOffset = getTextBuffer () -> size () - CONSOLE_LIMIT;

		getTextBuffer () -> erase (getTextBuffer () -> begin (), getTextBuffer () -> get_iter_at_offset (charOffset));
	}

	on_scroll_to_end ();
}

X3DConsole::~X3DConsole ()
{ }

} // puck
} // titania
