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

#include "Console.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../Footer/Footer.h"

namespace titania {
namespace puck {

Console::Console (X3DBrowserWindow* const browserWindow) :
	   X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DConsoleInterface (get_ui ("Console.glade"))
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
	if (browser == getBrowserWindow () -> getMasterBrowser ())
	   return;
	
	browser -> getConsole () -> getString () .addInterest (this, &Console::set_string);	
}

void
Console::set_enabled ()
{
	if (isEnabled ())
	{
		getCurrentBrowser () .addInterest (this, &Console::set_browser);

		set_browser (getCurrentBrowser ());
	}
	else
	{
		getCurrentBrowser () .removeInterest (this, &Console::set_browser);

		for (const auto & browser : getBrowserWindow () -> getBrowsers ())
			browser -> getConsole () -> getString () .removeInterest (this, &Console::set_string);
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

	// Place cursor.

	getTextBuffer () -> place_cursor (getTextBuffer () -> end ());

	getTextView () .scroll_to (getTextBuffer () -> get_insert ());

	// Refresh.

	getTextView () .queue_draw ();
}

Console::~Console ()
{
	dispose ();
}

} // puck
} // titania
