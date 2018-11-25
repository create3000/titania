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

#include "LibraryView.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"

namespace titania {
namespace puck {

class LibraryView::Columns
{
public:

static constexpr int TITANIA      = 4;
static constexpr int X_ITE        = 5;
static constexpr int EXPERIMENTAL = 6;

};

LibraryView::LibraryView (X3DBrowserWindow* const browserWindow) :
	                        X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	                 X3DLibraryViewInterface (get_ui ("Editors/LibraryView.glade")),
	X3DFileBrowser <X3DLibraryViewInterface> ()
{
   #ifndef TITANIA_DEBUG
   getExperimantalColumn () -> set_visible (false);
   #endif

	setup ();
}

void
LibraryView::initialize ()
{
	X3DLibraryViewInterface::initialize ();
	X3DFileBrowser <X3DLibraryViewInterface>::initialize ();
}

void
LibraryView::configure ()
{
	try
	{
		X3DLibraryViewInterface::configure ();
		X3DFileBrowser <X3DLibraryViewInterface>::configure ();

		const auto folder = getConfig () -> getItem <std::string> ("folder", "1");
		const auto path   = Gtk::TreePath (folder);

		getFoldersSelection () -> select (path);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
LibraryView::on_folder_clicked ()
{
	getFoldersPopover () .popup ();
}

void
LibraryView::on_folder_selection_changed ()
{
	getFoldersPopover () .popdown ();

	const auto iter = getFoldersSelection () -> get_selected ();
	auto       name = std::string ();

	iter -> get_value (0, name);

	saveExpanded (getFolderLabel () .get_text ());
	getFolderLabel () .set_text (name);

	setRootFolder (Gio::File::create_for_path (find_data_file ("Library/" + name)));
	restoreExpanded (name);
}

void
LibraryView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	try
	{
		const auto iter = getFileStore () -> get_iter (path);
		const auto file = getFile (iter);

		switch (file -> query_info () -> get_file_type ())
		{
			case Gio::FILE_TYPE_REGULAR:
			case Gio::FILE_TYPE_SYMBOLIC_LINK:
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import From Library"));
				const auto nodes    = getBrowserWindow () -> import ({ file -> get_uri () }, undoStep);

				getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
				getBrowserWindow () -> addUndoStep (undoStep);
				break;
			}
			default:
				break;
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
LibraryView::on_row_expanded (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	for (const auto & child : iter -> children ())
	{
		const auto file         = getFile (child);
		const auto directory    = file -> get_parent ();
		const auto basename     = basic::uri (file -> get_path ()) .stem ();
		const bool titania      = Gio::File::create_for_path (directory -> get_path () + "/.Titania/"      + basename) -> query_exists ();
		const bool x_ite        = Gio::File::create_for_path (directory -> get_path () + "/.X_ITE/"        + basename) -> query_exists ();
		const bool experimental = Gio::File::create_for_path (directory -> get_path () + "/.experimental/" + basename) -> query_exists ();
	
		#ifndef TITANIA_FEATURE
		if (experimental and not (x_ite or titania))
		{
			getFileStore () -> erase (child);
		   return;
		}
		#endif

		child -> set_value (Columns::EXPERIMENTAL, std::string (experimental ? "Experimental" : ""));
		child -> set_value (Columns::TITANIA,      std::string (titania      ? "Titania"      : ""));
		child -> set_value (Columns::X_ITE,        std::string (x_ite        ? "X_ITE"        : ""));
	}
}

void
LibraryView::store ()
{
	const auto folder = getFoldersListStore () -> get_path (getFoldersSelection () -> get_selected ());

	getConfig () -> setItem <std::string> ("folder", folder .to_string ());

	saveExpanded (getFolderLabel () .get_text ());

	X3DFileBrowser <X3DLibraryViewInterface>::store ();
	X3DLibraryViewInterface::store ();
}

LibraryView::~LibraryView ()
{
	dispose ();
}

} // puck
} // titania
