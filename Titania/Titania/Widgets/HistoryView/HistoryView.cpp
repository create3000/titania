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

#include "HistoryView.h"

#include "../../Base/AdjustmentObject.h"
#include "../../Browser/BrowserUserData.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

static constexpr int ICON_COLUMN      = 0;
static constexpr int TITLE_COLUMN     = 1;
static constexpr int WORLD_URL_COLUMN = 2;

HistoryView::HistoryView (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DHistoryViewInterface (get_ui ("HistoryView.xml"), gconf_dir ()),
	            hadjustment (new AdjustmentObject ()),
	            vadjustment (new AdjustmentObject ())
{
	getScene () .addInterest (this, &HistoryView::set_scene);
	setup ();
}

void
HistoryView::initialize ()
{
	X3DHistoryViewInterface::initialize ();

	getBrowserWindow () -> getHistory () .constrainSize (getConfig () .getInteger ("rememberHistory"));
}

void
HistoryView::restoreSession ()
{
	X3DHistoryViewInterface::restoreSession ();
			
	if (not getConfig () .hasItem ("rememberHistory"))
		getConfig () .setItem ("rememberHistory", 12);

	const auto rememberHistory = getConfig () .getInteger ("rememberHistory");

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

	for (const auto & item : getBrowserWindow () -> getHistory () .getItems (0, 0))
		getBrowserWindow () -> loadIcon (item .at ("worldURL"), getBrowserWindow () -> getHistory () .getIcon (item .at ("id")));
}

void
HistoryView::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("History"));

	getBrowserWindow () -> getHistory () .addInterest (this, &HistoryView::set_history);
	getBrowserWindow () -> worldURL_changed () .addInterest (this, &HistoryView::set_scene);

	set_history ();
}

void
HistoryView::on_unmap ()
{
	getBrowserWindow () -> getHistory () .removeInterest (this, &HistoryView::set_history);
	getBrowserWindow () -> worldURL_changed () .removeInterest (this, &HistoryView::set_scene);
}

void
HistoryView::set_history ()
{
	// Fill model.

	getTreeView () .unset_model ();
	getListStore () -> clear ();

	for (const auto & item : getBrowserWindow () -> getHistory () .getItems (0, 2000))
	{
		const auto & worldURL = item .at ("worldURL");
		const auto   iter     = getListStore () -> append ();

		iter -> set_value (ICON_COLUMN,      basic::uri (worldURL) .filename () .str ());
		iter -> set_value (TITLE_COLUMN,     item .at ("title"));
		iter -> set_value (WORLD_URL_COLUMN, worldURL);
	}

	getTreeView () .set_model (getListStore ());
	getTreeView () .set_search_column (TITLE_COLUMN);
	getTreeView () .get_selection () -> select (Gtk::TreePath ("0"));

	hadjustment -> restore (getTreeView () .get_hadjustment (), getConfig () .getDouble ("hadjustment"));
	vadjustment -> restore (getTreeView () .get_vadjustment (), getConfig () .getDouble ("vadjustment"));
}

void
HistoryView::set_scene ()
{
	const std::string title    = getScene () -> getTitle ();
	const basic::uri  worldURL = getScene () -> getWorldURL ();

	if (worldURL .empty ())
		return;

	try
	{
		if (getScene () -> getMetaData ("titania-history") == "FALSE")
			return;
	}
	catch (const X3D::X3DError &)
	{ }

	// Update history.

	getBrowserWindow () -> getHistory () .setItem (title, worldURL, getBrowserWindow () -> getIcon (worldURL, Gtk::IconSize (Gtk::ICON_SIZE_MENU)));

	// Move row.

	if (not getWidget () .get_mapped ())
		return;

	try
	{
		getListStore () -> erase (getListStore () -> get_iter (getBrowserWindow () -> getHistory () .getIndex (worldURL)));
	}
	catch (const std::out_of_range &)
	{ }

	const auto iter = getListStore () -> prepend ();
	iter -> set_value (ICON_COLUMN,      worldURL .filename () .str ());
	iter -> set_value (TITLE_COLUMN,     title);
	iter -> set_value (WORLD_URL_COLUMN, worldURL .str ());

	getListStore () -> row_changed (getListStore () -> get_path (iter), iter);
}

void
HistoryView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	// Open worldURL.

	const std::string URL = getBrowserWindow () -> getHistory () .getItemFromIndex (path .to_string ()) .at ("worldURL");

	getBrowserWindow () -> open (URL);
}

bool
HistoryView::on_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
		getMenu () .popup (event -> button, event -> time);
		return true;
	}

	return false;
}

void
HistoryView::on_never_toggled ()
{
	getConfig () .setItem ("rememberHistory", 0);
}

void
HistoryView::on_on_month_toggled ()
{
	getConfig () .setItem ("rememberHistory", 1);
}

void
HistoryView::on_half_year_toggled ()
{
	getConfig () .setItem ("rememberHistory", 6);
}

void
HistoryView::on_one_year_toggled ()
{
	getConfig () .setItem ("rememberHistory", 12);
}

void
HistoryView::on_always_toggled ()
{
	getConfig () .setItem ("rememberHistory", -1);
}

HistoryView::~HistoryView ()
{
	getConfig () .setItem ("hadjustment", getTreeView () .get_hadjustment () -> get_value ());
	getConfig () .setItem ("vadjustment", getTreeView () .get_vadjustment () -> get_value ());

	dispose ();
}

} // puck
} // titania
