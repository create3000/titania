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

#include "MFStringURLWidget.h"

#include "../Dialogs/FileOpenDialog/FileOpenDialog.h"

namespace titania {
namespace puck {

MFStringURLWidget::MFStringURLWidget (X3DUserInterface* const userInterface,
                                      Gtk::TreeView & treeView,
                                      const Glib::RefPtr <Gtk::CellRendererText> & cellRenderer,
                                      Gtk::Button & addButton,
                                      Gtk::Button & removeButton,
                                      Gtk::Button & reloadButton,
                                      const Glib::RefPtr <Gtk::TreeViewColumn> & URLChooserColumn,
                                      const std::string & name) :
	 X3DBaseInterface (userInterface -> getBrowserWindow (), userInterface -> getCurrentBrowser ()),
	X3DMFStringWidget (userInterface, treeView, cellRenderer, addButton, removeButton, name, ""),
	    userInterface (userInterface),
	     reloadButton (reloadButton),
	 URLChooserColumn (URLChooserColumn),
	   fileOpenDialog (),
	         fragment ()
{
	treeView     .signal_button_release_event () .connect (sigc::mem_fun (this, &MFStringURLWidget::on_button_release_event));
	reloadButton .signal_clicked ()              .connect (sigc::mem_fun (this, &MFStringURLWidget::on_reload_clicked));

	setup ();
}

void
MFStringURLWidget::on_add_clicked ()
{
	fileOpenDialog .reset (new FileOpenDialog (getBrowserWindow ()));

	fileOpenDialog -> getWindow () .set_transient_for (userInterface -> getBrowserWindow () -> getWindow ());
	fileOpenDialog -> getWindow () .set_modal (true);
	fileOpenDialog -> getRelativePathBox () .set_visible (true);

	if (fileOpenDialog -> run ())
	{
		auto URL = fileOpenDialog -> getUrl ();

		if (fileOpenDialog -> getRelativePathSwitch () .get_active ())
			URL = getCurrentContext () -> getWorldURL () .relative_path (URL);

		append (URL .str ());
		reloadButton .set_sensitive (true);
	}

	fileOpenDialog -> quit ();
	fileOpenDialog .reset ();
}

bool
MFStringURLWidget::on_button_release_event (GdkEventButton* event)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (not path .size ())
		return false;

	if (column not_eq URLChooserColumn .operator -> ())
		return false;

	// Choose new URL

	fileOpenDialog .reset (new FileOpenDialog (getBrowserWindow ()));

	fileOpenDialog -> getWindow () .set_transient_for (userInterface -> getBrowserWindow () -> getWindow ());
	fileOpenDialog -> getWindow () .set_modal (true);
	fileOpenDialog -> getRelativePathBox () .set_visible (true);

	// Set URL

	basic::uri URL = getCurrentContext () -> getWorldURL () .transform (get1Value (path .front ()) .raw ());

	// Run

	if (fileOpenDialog -> run ())
	{
		URL = fileOpenDialog -> getUrl ();

		if (fileOpenDialog -> getRelativePathSwitch () .get_active ())
			URL = getCurrentContext () -> getWorldURL () .relative_path (URL);

		URL .fragment (fragment);

		set1Value (path .front (), URL .str ());
	}

	fileOpenDialog -> quit ();
	fileOpenDialog .reset ();
	return true;
}

void
MFStringURLWidget::set_buffer ()
{
	if (fileOpenDialog)
		fileOpenDialog -> quit ();

	X3DMFStringWidget::set_buffer ();
}

void
MFStringURLWidget::set_string (const X3D::MFString & string)
{
	reloadButton .set_sensitive (not string .empty ());
}

void
MFStringURLWidget::on_reload_clicked ()
{
	for (const auto & node : getNodes ())
	{
		try
		{
			node -> getField <X3D::MFString> (getName ()) .addEvent ();
		}
		catch (const X3D::X3DError &)
		{ }
	}
}

MFStringURLWidget::~MFStringURLWidget ()
{
	dispose ();
}

} // puck
} // titania
