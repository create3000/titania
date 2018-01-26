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

X3DConsole::X3DConsole () :
	X3DConsoleInterface (),
	        scrollToEnd (true)
{ }

void
X3DConsole::setup ()
{
	const auto redTag    = getTextBuffer () -> create_tag ("red");
	const auto yellowTag = getTextBuffer () -> create_tag ("yellow");
	const auto blueTag   = getTextBuffer () -> create_tag ("blue");
	const auto greenTag  = getTextBuffer () -> create_tag ("green");

	redTag    -> property_foreground_set () = true;
	redTag    -> property_foreground_gdk () = getColor ("#e06c75");
	yellowTag -> property_foreground_set () = true;
	yellowTag -> property_foreground_gdk () = getColor ("#e5c07b");
	blueTag   -> property_foreground_set () = true;
	blueTag   -> property_foreground_gdk () = getColor ("#61afef");
	greenTag  -> property_foreground_set () = true;
	greenTag  -> property_foreground_gdk () = getColor ("#98c379");
}

void
X3DConsole::on_scoll_to_end ()
{
	auto iter = getTextBuffer () -> end ();
	auto mark = getTextBuffer () -> get_mark ("scroll");

	iter .set_line_offset (0);
	getTextBuffer () -> move_mark (mark, iter);
	getTextView () .scroll_to (mark);
}

void
X3DConsole::push (const std::vector <Glib::ustring> & tags, const Glib::ustring & string)
{
	// Append.

	getTextBuffer () -> insert_with_tags_by_name (getTextBuffer () -> end (), string, tags);

	std::clog << string << std::flush;

	// Erase.

	static constexpr int32_t CONSOLE_LIMIT = 20'000'000; // 20 MB

	if (getTextBuffer () -> size () > CONSOLE_LIMIT)
	{
		const int charOffset = getTextBuffer () -> size () - CONSOLE_LIMIT;

		getTextBuffer () -> erase (getTextBuffer () -> begin (), getTextBuffer () -> get_iter_at_offset (charOffset));
	}

	// Update TextView and scoll to end.

	if (scrollToEnd)
		Glib::signal_idle () .connect_once (sigc::mem_fun (this, &Console::on_scoll_to_end));
}

Gdk::Color
X3DConsole::getColor (const std::string & value) const
{
	auto color = Gdk::Color ();

	color .set (value);

	return color;
}

X3DConsole::~X3DConsole ()
{ }

} // puck
} // titania
