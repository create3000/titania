/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "HistoryEditor.h"

#include "../Configuration/config.h"
#include <iostream>

namespace titania {
namespace puck {

static constexpr int ICON_COLUMN      = 0;
static constexpr int TITLE_COLUMN     = 1;
static constexpr int WORLD_URL_COLUMN = 2;

HistoryEditor::HistoryEditor (const std::string & sessionKey, X3DBrowserInterface* const browserWidget) :
	X3DHistoryEditorUI (get_ui ("HistoryEditor.ui"), sessionKey),
	        history ()
{
	setBrowserWidget (browserWidget);
	setBrowser (browserWidget -> getBrowser ());
}

void
HistoryEditor::initialize ()
{
	X3DHistoryEditorUI::initialize ();

	getBrowser () -> initialized .addInterest (this, &HistoryEditor::set_initialized);
}

void
HistoryEditor::set_initialized ()
{
	getBrowser () -> initialized .removeInterest (this, &HistoryEditor::set_initialized);
	getBrowser () -> initialized .addInterest    (this, &HistoryEditor::set_world);
}

const History &
HistoryEditor::getHistory ()
{
	return history;
}

std::string
HistoryEditor::getTitle (const basic::uri & worldURL)
{
	std::string title;

	try
	{
		title = getBrowser () -> getExecutionContext () -> getMetaData ("title");
	}
	catch (const X3D::Error <X3D::INVALID_NAME> &)
	{
		title = worldURL .filename ();
	}

	return title;
}

void
HistoryEditor::on_map ()
{
	getListStore () -> clear ();
	
	for (const auto & item : history .getItems ())
	{
		auto row = getListStore () -> append ();
		row -> set_value (ICON_COLUMN,      std::string ("BlankIcon"));
		row -> set_value (TITLE_COLUMN,     item .at ("title"));
		row -> set_value (WORLD_URL_COLUMN, item .at ("worldURL"));
	}

	getScrolledWindow () .queue_draw ();
}

void
HistoryEditor::set_world ()
{
	const basic::uri & worldURL = getBrowser () -> getExecutionContext () -> getWorldURL ();
	std::string        title    = getTitle (worldURL);

	try
	{
		getListStore () -> erase (getListStore () -> get_iter (history .getIndex (worldURL)));
	}
	catch (const std::out_of_range &)
	{ }

	auto row = getListStore () -> prepend ();
	row -> set_value (ICON_COLUMN,      worldURL .str ());
	row -> set_value (TITLE_COLUMN,     title);
	row -> set_value (WORLD_URL_COLUMN, worldURL .str ());

	history .setItem (title, worldURL);
	
	getScrolledWindow () .queue_draw ();
}

void
HistoryEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	// Open worldURL.

	getBrowserWidget () -> loadURL ({ history .getItemFromIndex (path .to_string ()) .at ("worldURL") });
}

HistoryEditor::~HistoryEditor ()
{ }

} // puck
} // titania
