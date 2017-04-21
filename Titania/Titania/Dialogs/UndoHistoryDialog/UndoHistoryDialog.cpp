/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "UndoHistoryDialog.h"

#include "../../Base/AdjustmentObject.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

namespace Columns {

static constexpr int NUMBER      = 0;
static constexpr int DESCRIPTION = 1;
static constexpr int SENSITIVE   = 2;

}

UndoHistoryDialog::UndoHistoryDialog (X3DBrowserWindow* const browserWindow) :
	             X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DUndoHistoryDialogInterface (get_ui ("Dialogs/UndoHistoryDialog.glade")),
	                      browser (browserWindow -> getCurrentBrowser ()),
	                  hadjustment (new AdjustmentObject ()),
	                  vadjustment (new AdjustmentObject ()),
	                   undoBuffer ()
{
	addChildObjects (browser, undoBuffer);

	setup ();
}

void
UndoHistoryDialog::initialize ()
{
	X3DUndoHistoryDialogInterface::initialize ();

	getCurrentBrowser () .addInterest (&UndoHistoryDialog::set_browser, this);

	undoBuffer .addInterest (&UndoHistoryDialog::set_undoHistory, this);

	getTreeView () .set_headers_clickable (false);

	set_browser ();
}

void
UndoHistoryDialog::set_browser ()
{
	getBrowserWindow () -> getUndoHistory (getCurrentBrowser ()) .removeInterest (&X3D::SFTime::setValue, undoBuffer);

	browser = getCurrentBrowser ();

	getBrowserWindow () -> getUndoHistory (getCurrentBrowser ()) .addInterest (&X3D::SFTime::setValue, undoBuffer, 1);

	set_undoHistory ();
}

void
UndoHistoryDialog::set_undoHistory ()
{
	getTreeView () .queue_draw ();

	hadjustment -> preserve (getTreeView () .get_hadjustment ());
	vadjustment -> preserve (getTreeView () .get_vadjustment ());

	getTreeView () .unset_model ();
	getListStore () -> clear ();

	const auto & undoHistory = getBrowserWindow () -> getUndoHistory (getCurrentBrowser ());

	size_t number = 1;

	for (const auto & undoStep : undoHistory .getUndoList ())
	{
		const auto row = getListStore () -> append ();

		row -> set_value (Columns::NUMBER,      number);
		row -> set_value (Columns::DESCRIPTION, undoStep -> getDescription ());
		row -> set_value (Columns::SENSITIVE,   true);

		++ number;
	}

	for (const auto & undoStep : undoHistory .getRedoList ())
	{
		const auto row = getListStore () -> append ();

		row -> set_value (Columns::NUMBER,      number);
		row -> set_value (Columns::DESCRIPTION, undoStep -> getDescription ());
		row -> set_value (Columns::SENSITIVE,   false);

		++ number;
	}

	getTreeView () .set_model (getListStore ());
	getTreeView () .set_search_column (Columns::DESCRIPTION);

	getUndoButton () .set_sensitive (undoHistory .hasUndo ());
	getRedoButton () .set_sensitive (undoHistory .hasRedo ());
}

void
UndoHistoryDialog::on_undo_clicked ()
{
	getBrowserWindow () -> undo ();
}

void
UndoHistoryDialog::on_redo_clicked ()
{
	getBrowserWindow () -> redo ();
}

void
UndoHistoryDialog::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	const size_t index       = path .front ();
	const auto & undoHistory = getBrowserWindow () -> getUndoHistory (getCurrentBrowser ());

	if (index < undoHistory .getUndoList () .size ())
	{
		// Undo

		const auto size = undoHistory .getUndoList () .size () - index;

		for (size_t i = 0; i < size; ++ i)
			getBrowserWindow () -> undo ();
	}
	else
	{
		// Redo

		const auto size = index - undoHistory .getUndoList () .size () + 1;

		for (size_t i = 0; i < size; ++ i)
			getBrowserWindow () -> redo ();
	}
}

UndoHistoryDialog::~UndoHistoryDialog ()
{
	dispose ();
}

} // puck
} // titania
