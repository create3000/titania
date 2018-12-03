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

#include "HistoryEditor.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Browser/IconFactory.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

struct HistoryEditor::Columns
{
	static constexpr int ICON             = 0;
	static constexpr int TITLE            = 1;
	static constexpr int WORLD_URL        = 2;
	static constexpr int LAST_ACCESS      = 3;
	static constexpr int LAST_ACCESS_TIME = 4;
	static constexpr int ID               = 5;
};

HistoryEditor::HistoryEditor (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DHistoryEditorInterface (get_ui ("Editors/HistoryEditor.glade")),
	          scrollFreezer (new ScrollFreezer (getTreeView ()))
{
	setup ();
}

void
HistoryEditor::initialize ()
{
	X3DHistoryEditorInterface::initialize ();
}

void
HistoryEditor::configure ()
{
	X3DHistoryEditorInterface::configure ();

	if (getConfig () -> hasItem ("sortColumn"))
	{
		const auto sortColumn = getConfig () -> getItem <int32_t> ("sortColumn");
		const auto sortOrder  = getConfig () -> getItem <int32_t> ("sortOrder");

		getTreeModelSort () -> set_sort_column (sortColumn, Gtk::SortType (sortOrder));
	}
	else
	{
		getTreeModelSort () -> set_sort_column (Columns::LAST_ACCESS_TIME, Gtk::SORT_DESCENDING);
	}

	const auto rememberHistory = getBrowserWindow () -> getConfig () -> getItem <int32_t> ("rememberHistory");

	switch (rememberHistory)
	{
		case 0:
			getNeverMenuItem () .set_active (true);
			break;
		case 1:
			getOneMonthMenuItem () .set_active (true);
			break;
		case 6:
			getHalfYearMenuItem () .set_active (true);
			break;
		case 12:
			getOneYearMenuItem () .set_active (true);
			break;
		case -1:
			getAlwaysMenuItem () .set_active (true);
			break;
		default:
			break;
	}

	for (const auto & item : getBrowserWindow () -> getHistory () -> getItems (0, 0))
		getBrowserWindow () -> getIconFactory () -> createIcon (basic::uri (item .at ("worldURL")), getBrowserWindow () -> getHistory () -> getPreview (item .at ("id")));

	if (getConfig () -> hasItem ("titleColumnSize"))
		getTitleColumn () -> set_fixed_width (getConfig () -> getItem <int32_t> ("titleColumnSize"));

	if (getConfig () -> hasItem ("worldURLColumnSize"))
		getWorlURLColumn () -> set_fixed_width (getConfig () -> getItem <int32_t> ("worldURLColumnSize"));

	if (getConfig () -> hasItem ("lastAccessColumnSize"))
		getLastAccessColumn () -> set_fixed_width (getConfig () -> getItem <int32_t> ("lastAccessColumnSize"));
}

void
HistoryEditor::on_map ()
{
	getBrowserWindow () -> getHistory () -> addInterest (&HistoryEditor::set_history, this);

	set_history ();
}

void
HistoryEditor::on_unmap ()
{
	getBrowserWindow () -> getHistory () -> removeInterest (&HistoryEditor::set_history, this);
}

void
HistoryEditor::set_history ()
{
	// Fill model.

	scrollFreezer -> freeze ();

	const auto rows   = getTreeView () .get_selection () -> get_selected_rows ();
	const auto search = getSearchEntry () .get_text ();

	getTreeView () .unset_model ();
	getListStore () -> clear ();

	for (const auto & item : getBrowserWindow () -> getHistory () -> getItems (0, 10000, search))
	{
		const auto   worldURL   = basic::uri (item .at ("worldURL"));
		const time_t lastAccess = std::strtoul (item .at ("lastAccess") .c_str (), nullptr, 0);
		const auto   iter       = getListStore () -> append ();

		std::ostringstream osstream;

		osstream << std::put_time (std::localtime (&lastAccess), "%c"); // "%c %Z"

		iter -> set_value (Columns::ID,               item .at ("id"));
		iter -> set_value (Columns::ICON,             worldURL .filename () .str ());
		iter -> set_value (Columns::TITLE,            item .at ("title"));
		iter -> set_value (Columns::WORLD_URL,        worldURL .escape (" ") .str ());
		iter -> set_value (Columns::LAST_ACCESS,      osstream .str ());
		iter -> set_value (Columns::LAST_ACCESS_TIME, std::strtod (item .at ("lastAccess") .c_str (), nullptr));
	}

	getTreeView () .set_model (getTreeModelSort ());
	getTreeView () .set_search_column (Columns::TITLE);

	for (const auto & path : rows)
		getTreeView () .get_selection () -> select (path);
}

void
HistoryEditor::on_search_changed ()
{
	set_history ();
}

void
HistoryEditor::on_column_clicked ()
{
	int  sortColumn = 0;
	auto sortOrder  = Gtk::SortType ();

	getTreeModelSort () -> get_sort_column_id (sortColumn, sortOrder);

	getConfig () -> setItem ("sortOrder",  int32_t (sortOrder));
	getConfig () -> setItem ("sortColumn", sortColumn);
}

void
HistoryEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	try
	{
		// Open worldURL.

		const auto search = getSearchEntry () .get_text ();
		const auto iter   = getTreeModelSort () -> get_iter (path);

		std::string id;

		iter -> get_value (Columns::ID, id);

		getBrowserWindow () -> open (getBrowserWindow () -> getHistory () -> getItem (id) .at ("worldURL"));
	}
	catch (const std::exception & error)
	{ }
}

bool
HistoryEditor::on_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
		getMenu () .popup (event -> button, event -> time);
		return true;
	}

	return false;
}

void
HistoryEditor::on_never_toggled ()
{
	getBrowserWindow () -> getConfig () -> setItem ("rememberHistory", 0);
}

void
HistoryEditor::on_on_month_toggled ()
{
	getBrowserWindow () -> getConfig () -> setItem ("rememberHistory", 1);
}

void
HistoryEditor::on_half_year_toggled ()
{
	getBrowserWindow () -> getConfig () -> setItem ("rememberHistory", 6);
}

void
HistoryEditor::on_one_year_toggled ()
{
	getBrowserWindow () -> getConfig () -> setItem ("rememberHistory", 12);
}

void
HistoryEditor::on_always_toggled ()
{
	getBrowserWindow () -> getConfig () -> setItem ("rememberHistory", -1);
}

void
HistoryEditor::store ()
{
	getConfig () -> setItem ("titleColumnSize",      getTitleColumn ()      -> get_fixed_width ());
	getConfig () -> setItem ("worldURLColumnSize",   getWorlURLColumn ()    -> get_fixed_width ());
	getConfig () -> setItem ("lastAccessColumnSize", getLastAccessColumn () -> get_fixed_width ());

	X3DHistoryEditorInterface::store ();
}

HistoryEditor::~HistoryEditor ()
{
	dispose ();
}

} // puck
} // titania
