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

#include "FileSaveDialog.h"

#include "../../Configuration/config.h"

#include <Titania/OS.h>

namespace titania {
namespace puck {

FileSaveDialog::FileSaveDialog (BrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DFileSaveDialogInterface (get_ui ("Dialogs/FileSaveDialog.xml"), gconf_dir ())
{
	getFileFilterX3D   () -> set_name (_ ("X3D"));
	getFileFilterImage () -> set_name (_ ("Images"));
	getFileFilterAudio () -> set_name (_ ("Audio"));
	getFileFilterVideo () -> set_name (_ ("Videos"));
	getFileFilterAll   () -> set_name (_ ("All Files"));

	getWindow () .add_filter (getFileFilterX3D ());
	getWindow () .set_filter (getFileFilterX3D ());

	const auto worldURL = getRootContext () -> getWorldURL ();

	if (not worldURL .empty () and worldURL .is_local ())
		getWindow () .set_uri (worldURL .filename () .str ());

	else
	{
		if (worldURL .basename () .empty ())
			getWindow () .set_filename (os::home () + _ ("scene.x3dv"));
		
		else
			getWindow () .set_current_name (worldURL .basename ());
	}

	setup ();
}

void
FileSaveDialog::run ()
{
	const auto saveCompressedButton = getWidget <Gtk::Switch> ("SaveCompressedButton");

	saveCompressedButton -> set_active (getRootContext () -> isCompressed ());

	const auto responseId = getWindow () .run ();

	if (responseId == Gtk::RESPONSE_OK)
	{
		getBrowserWindow () -> save (Glib::uri_unescape_string (getWindow () .get_uri ()), saveCompressedButton -> get_active ());
		getBrowserWindow () -> saveAs () .processInterests ();
	}

	getWindow () .hide ();
}

FileSaveDialog::~FileSaveDialog ()
{
	dispose ();
}

} // puck
} // titania
