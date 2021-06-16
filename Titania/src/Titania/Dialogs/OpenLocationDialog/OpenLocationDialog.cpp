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

#include "OpenLocationDialog.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <regex>

namespace titania {
namespace puck {

OpenLocationDialog::OpenLocationDialog (X3DBrowserWindow* const browserWindow) :
	              X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DOpenLocationDialogInterface (get_ui ("Dialogs/OpenLocationDialog.glade"))
{
	setTitleBar (getWindow (), getHeaderBar ());

	getWindow () .set_response_sensitive (Gtk::RESPONSE_OK, getLocationEntry () .get_text () .size ());

	const Glib::RefPtr <Gtk::Clipboard> clipboard = Gtk::Clipboard::get ();

	if (clipboard -> wait_is_text_available ())
	{
		static const std::regex scheme (R"/(file|http|https|ftp|smb)/");

		const basic::uri uri (clipboard -> wait_for_text ());

		if (std::regex_match (uri .scheme (), scheme))
			getLocationEntry () .set_text (uri .str ());
	}

	setup ();
}

void
OpenLocationDialog::run ()
{
	const auto responseId = getWindow () .run ();

	if (responseId == Gtk::RESPONSE_OK)
		getBrowserWindow () -> open (getLocationEntry () .get_text () .raw ());

	quit ();
}

// Dialog response handling

void
OpenLocationDialog::on_location_entry_changed ()
{
	getWindow () .set_response_sensitive (Gtk::RESPONSE_OK, getLocationEntry () .get_text () .size ());
}

bool
OpenLocationDialog::on_location_entry_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Return or event -> keyval == GDK_KEY_KP_Enter)
	{
		getWindow () .response (Gtk::RESPONSE_OK);
		return true;
	}

	return false;
}

OpenLocationDialog::~OpenLocationDialog ()
{
	dispose ();
}

} // puck
} // titania
