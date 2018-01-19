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

#include "X3DLibraryViewInterface.h"

namespace titania {
namespace puck {

void
X3DLibraryViewInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DLibraryViewInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DLibraryViewInterface::create ()
{
	// Get objects.
	m_FileStore            = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("FileStore"));
	m_FileSelection        = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FileSelection"));
	m_FileColumn           = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FileColumn"));
	m_IconRenderer         = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconRenderer"));
	m_NameRenderer         = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameRenderer"));
	m_ExperimantalColumn   = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("ExperimantalColumn"));
	m_ExperimantalRenderer = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("ExperimantalRenderer"));
	m_TitaniaColumn        = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("TitaniaColumn"));
	m_TitaniaRenderer      = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("TitaniaRenderer"));
	m_X_ITEColumn          = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("X_ITEColumn"));
	m_X_ITERenderer        = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("X_ITERenderer"));
	m_FoldersListStore     = Glib::RefPtr <Gtk::ListStore>::cast_dynamic (m_builder -> get_object ("FoldersListStore"));
	m_FoldersSelection     = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FoldersSelection"));

	// Get widgets.
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("FolderButton", m_FolderButton);
	m_builder -> get_widget ("FolderLabel", m_FolderLabel);
	m_builder -> get_widget ("FilesBox", m_FilesBox);
	m_builder -> get_widget ("ScrolledWindow", m_ScrolledWindow);
	m_builder -> get_widget_derived ("FileView.FileView", m_FileView);
	m_builder -> get_widget ("FoldersPopover", m_FoldersPopover);
	m_builder -> get_widget ("FoldersTreeView", m_FoldersTreeView);
	m_FolderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_folder_clicked));

	// Connect object Gtk::TreeView with id 'FileView'.
	m_FileView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_row_activated));
	m_FileView -> signal_row_expanded () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_row_expanded));
	m_FileView -> signal_test_expand_row () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_test_expand_row));

	// Connect object Gtk::TreeSelection with id 'FoldersSelection'.
	m_FoldersSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DLibraryViewInterface::on_folder_selection_changed));
}

X3DLibraryViewInterface::~X3DLibraryViewInterface ()
{
	delete m_Window;
}

} // puck
} // titania
