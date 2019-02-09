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
#include "../../Configuration/config.h"
#include "../../Widgets/Footer/Footer.h"

namespace titania {
namespace puck {

Console::Console (X3DBrowserWindow* const browserWindow) :
	   X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DConsoleInterface (get_ui ("Widgets/Console.glade")),
	         X3DConsole ()
{
	X3DConsoleInterface::setup ();
	X3DConsole::setup ();
}

void
Console::initialize ()
{
	X3DConsoleInterface::initialize ();

	set_enabled ();
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
Console::set_enabled ()
{
	if (getSuspendButton () .get_active ())
		getCurrentBrowser () -> getConsole () -> getMessages () .removeInterest (&Console::set_messages, this);	
	else
		getCurrentBrowser () -> getConsole () -> getMessages () .addInterest (&Console::set_messages, this);	
}

void
Console::set_messages (const X3D::MFString & messages)
{
	for (size_t i = 0; i < messages .size (); i += 2)
	{
		const auto & type    = messages [i];
		const auto & message = messages [i + 1];

		if (type == "error")
			error (message);

		else if (type == "warn")
			warn (message);

		else if (type == "log")
			log (message);

		else
			print (message);
	}
}

Console::~Console ()
{
	dispose ();
}

} // puck
} // titania
