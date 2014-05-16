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

#include "HistoryEditor.h"

#include "../Browser/BrowserWindow.h"
#include "../Configuration/config.h"

namespace titania {
namespace puck {

static constexpr int ICON_COLUMN      = 0;
static constexpr int TITLE_COLUMN     = 1;
static constexpr int WORLD_URL_COLUMN = 2;

HistoryEditor::HistoryEditor (BrowserWindow* const browserWindow) :
	         X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DHistoryEditorInterface (get_ui ("HistoryEditor.xml"), gconf_dir ()),
	                  history ()
{
	// Fill model

	//auto model = getTreeView () .get_model ();
	//getTreeView () .unset_model ();

	for (const auto & item : history .getItems ())
	{
		const auto row = getListStore () -> append ();
		row -> set_value (ICON_COLUMN,      std::string ("BlankIcon"));
		row -> set_value (TITLE_COLUMN,     item .at ("title"));
		row -> set_value (WORLD_URL_COLUMN, item .at ("worldURL"));

		getListStore () -> row_changed (getListStore () -> get_path (row), row);
	}
	
	//getTreeView () .set_model (model);

	// Add browser interest

	getWorld () .addInterest (this, &HistoryEditor::set_splashScreen);
}

void
HistoryEditor::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("History"));
}

void
HistoryEditor::set_splashScreen ()
{
	getWorld () .removeInterest (this, &HistoryEditor::set_splashScreen);
	getWorld () .addInterest    (this, &HistoryEditor::set_world);
}

void
HistoryEditor::set_world ()
{
	const std::string title    = getWorld () -> getTitle ();
	const basic::uri  worldURL = getWorld () -> getWorldURL ();

	if (worldURL .empty ())
		return;
		
	// Move row

	try
	{
		getListStore () -> erase (getListStore () -> get_iter (history .getIndex (worldURL)));
	}
	catch (const std::out_of_range &)
	{
		__LOG__ << std::endl;
	}

	const auto iter = getListStore () -> prepend ();
	iter -> set_value (ICON_COLUMN,      worldURL .str ());
	iter -> set_value (TITLE_COLUMN,     title);
	iter -> set_value (WORLD_URL_COLUMN, worldURL .str ());

	getListStore () -> row_changed (getListStore () -> get_path (iter), iter);

	// Update history

	history .setItem (title, worldURL);
}

void
HistoryEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	// Open worldURL.

	if (getBrowserWindow () -> isSaved ())
		getBrowserWindow () -> open (history .getItemFromIndex (path .to_string ()) .at ("worldURL"));
}

HistoryEditor::~HistoryEditor ()
{ }

} // puck
} // titania
