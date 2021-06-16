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

#ifndef __TMP_GLAD2CPP_LIBRARY_VIEW_H__
#define __TMP_GLAD2CPP_LIBRARY_VIEW_H__

#include "../Base/X3DEditorInterface.h"

#include "../Editors/ProjectsEditor/FileView.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for LibraryView.
 */
class X3DLibraryViewInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DLibraryViewInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DLibraryViewInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DLibraryViewInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::TreeStore> &
	getFileStore () const
	{ return m_FileStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getFileSelection () const
	{ return m_FileSelection; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getFileColumn () const
	{ return m_FileColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getIconRenderer () const
	{ return m_IconRenderer; }

	const Glib::RefPtr <Gtk::CellRendererText> &
	getNameRenderer () const
	{ return m_NameRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getExperimantalColumn () const
	{ return m_ExperimantalColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getExperimantalRenderer () const
	{ return m_ExperimantalRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getTitaniaColumn () const
	{ return m_TitaniaColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getTitaniaRenderer () const
	{ return m_TitaniaRenderer; }

	const Glib::RefPtr <Gtk::TreeViewColumn> &
	getX_ITEColumn () const
	{ return m_X_ITEColumn; }

	const Glib::RefPtr <Gtk::CellRendererPixbuf> &
	getX_ITERenderer () const
	{ return m_X_ITERenderer; }

	const Glib::RefPtr <Gtk::ListStore> &
	getFoldersListStore () const
	{ return m_FoldersListStore; }

	const Glib::RefPtr <Gtk::TreeSelection> &
	getFoldersSelection () const
	{ return m_FoldersSelection; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Button &
	getFolderButton () const
	{ return *m_FolderButton; }

	Gtk::Label &
	getFolderLabel () const
	{ return *m_FolderLabel; }

	Gtk::Box &
	getFilesBox () const
	{ return *m_FilesBox; }

	Gtk::ScrolledWindow &
	getScrolledWindow () const
	{ return *m_ScrolledWindow; }

	FileView &
	getFileView () const
	{ return *m_FileView; }

	Gtk::Popover &
	getFoldersPopover () const
	{ return *m_FoldersPopover; }

	Gtk::TreeView &
	getFoldersTreeView () const
	{ return *m_FoldersTreeView; }

	///  @name Signal handlers

	virtual
	void
	on_folder_clicked () = 0;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	void
	on_row_expanded (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) = 0;

	virtual
	bool
	on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) = 0;

	virtual
	void
	on_folder_selection_changed () = 0;

	///  @name Destruction

	virtual
	~X3DLibraryViewInterface () override;


private:

	///  @name Construction

	void
	create (const std::string &);

	void
	create (std::initializer_list <std::string>);

	void
	create ();

	///  @name Static members

	///  @name Members

	Glib::RefPtr <Gtk::Builder> m_builder;
	Glib::RefPtr <Gtk::TreeStore> m_FileStore;
	Glib::RefPtr <Gtk::TreeSelection> m_FileSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FileColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconRenderer;
	Glib::RefPtr <Gtk::CellRendererText> m_NameRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_ExperimantalColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_ExperimantalRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_TitaniaColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_TitaniaRenderer;
	Glib::RefPtr <Gtk::TreeViewColumn> m_X_ITEColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_X_ITERenderer;
	Glib::RefPtr <Gtk::ListStore> m_FoldersListStore;
	Glib::RefPtr <Gtk::TreeSelection> m_FoldersSelection;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Button* m_FolderButton;
	Gtk::Label* m_FolderLabel;
	Gtk::Box* m_FilesBox;
	Gtk::ScrolledWindow* m_ScrolledWindow;
	FileView* m_FileView;
	Gtk::Popover* m_FoldersPopover;
	Gtk::TreeView* m_FoldersTreeView;

};

} // puck
} // titania

#endif
