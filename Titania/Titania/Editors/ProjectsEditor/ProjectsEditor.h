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

#ifndef __TITANIA_EDITORS_PROJECTS_EDITOR_PROJECTS_EDITOR_H__
#define __TITANIA_EDITORS_PROJECTS_EDITOR_PROJECTS_EDITOR_H__

#include "../../UserInterfaces/X3DProjectsEditorInterface.h"

namespace titania {
namespace puck {

class BrowserWindow;
class ScrollFreezer;

class ProjectsEditor :
	virtual public X3DProjectsEditorInterface
{
public:

	///  @name Construction

	ProjectsEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~ProjectsEditor () final override;


private:

	///  Member types

	class FolderElement;

	using FolderElementPtr = std::shared_ptr <FolderElement>;

	///  @name Construction

	virtual
	void
	initialize () final override;

	virtual
	void
	configure () final override;

	///  @name Event handlers

	virtual
	void
	on_map () final override;

	virtual
	void
	on_unmap () final override;

	virtual
	bool
	on_focus_in_event (GdkEventFocus* focus_event);

	virtual
	bool
	on_focus_out_event (GdkEventFocus* gdk_event);

	///  @name Project folders handling

	virtual
	void
	on_add_project_clicked () final override;

	virtual
	void
	on_remove_project_clicked () final override;

	///  @name Menu handling

	void
	on_display_menu (GdkEventButton* event);

	void
	on_open_with_activate (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file);

	///  @name Add file handling

	virtual
	void
	on_import_activate () final override;

	virtual
	void
	on_add_new_file_activate () final override;

	virtual
	void
	on_create_file_changed () final override;

	virtual
	void
	on_create_file_clicked () final override;

	virtual
	bool
	on_create_file_key_press_event (GdkEventKey* event) final override;

	Glib::RefPtr <Gio::File>
	getNewFile () const;

	///  @name Add folder handling

	virtual
	void
	on_add_new_folder_activate () final override;

	virtual
	void
	on_create_folder_changed () final override;

	virtual
	void
	on_create_folder_clicked () final override;

	virtual
	bool
	on_create_folder_key_press_event (GdkEventKey* event) final override;

	Glib::RefPtr <Gio::File>
	getNewFolder () const;

	///  @name Rename item handling

	virtual
	void
	on_rename_item_activate () final override;

	virtual
	void
	on_rename_item_changed () final override;

	virtual
	void
	on_rename_item_clicked () final override;

	virtual
	bool
	on_rename_item_key_press_event (GdkEventKey* event) final override;

	std::tuple <Gtk::TreeIter, Glib::RefPtr <Gio::File>, Glib::RefPtr <Gio::File>>
	getRenameItem () const;

	///  @name Clipboard handling

	virtual
	void
	on_cut_item_activate () final override;

	virtual
	void
	on_copy_item_activate () final override;

	virtual
	void
	on_paste_into_folder_activate () final override;

	void
	clearClipboard ();
	
	void
	cutItems (const std::vector <Gtk::TreePath> & rows);
	
	void
	copyItems (const std::vector <Gtk::TreePath> & rows);

	void
	pasteIntoFolder (const Gtk::TreePath & row);

	Glib::RefPtr <Gio::File>
	getPasteDestination (const bool copy, const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & folder) const;

	std::string
	getPasteCopyString (const int32_t count) const;

	///  @name Move to trash handling

	virtual
	void
	on_move_to_trash_activate () final override;

	void
	on_move_to_trash_activate (const Glib::RefPtr <Gio::File> & file);

	void
	on_remove_file_activate (const Glib::RefPtr <Gio::File> & file);

	///  @name Selection handling

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) final override;

	virtual
	void
	on_selection_changed () final override;

	virtual
	bool
	on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path) final override;

	void
	on_file_changed (const Glib::RefPtr <Gio::File> & file,
	                 const Glib::RefPtr <Gio::File> & other_file,
	                 Gio::FileMonitorEvent event);

	void
	on_file_changed_update_tree_view (const Glib::RefPtr <Gio::File> & file);

	void
	set_execution_context ();

	///  @name Selection handling

	void
	unselectAll ();

	bool
	selectFile (const Glib::RefPtr <Gio::File> & file, const bool scroll = false);

	bool
	isSelected (const Glib::RefPtr <Gio::File> & file) const;

	bool
	expandTo (const Glib::RefPtr <Gio::File> & file);

	///  @name Folder handling handling

	void
	addRootFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addFolder (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & folder);

	void
	addFolder (const Glib::RefPtr <Gio::File> & folder);

	void
	addChildren (const Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & folder);

	void
	addChild (const Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon);

	void
	removeRootFolder (const Gtk::TreeModel::iterator & iter);

	void
	removeChildren (const Gtk::TreeIter & iter);

	void
	removeChild (const Gtk::TreeModel::iterator & iter);

	Gtk::TreeIter
	getIter (const std::string & path) const;
	
	bool
	getIter (const Gtk::TreeIter & iter, const std::string & path, Gtk::TreeIter & result) const;

	std::string
	getPath (const Gtk::TreeIter & iter) const;

	///  @name Misc operations

	void
	createOpenWithMenu (const Glib::RefPtr <Gio::File> & file);

	void
	createOpenWithMenuItem (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file);

	void
	launchFile (const std::string & path);

	bool
	canOpenFile (const Glib::RefPtr <Gio::File> & file);

	std::string
	getFileName (const std::string & filename, const std::string & suffix) const;
	
	std::string
	getSuffix (const int32_t type) const;

	Gdk::Rectangle
	getRectangle (const Gtk::TreePath & path) const;

	///  @name Expanded handling

	void
	restoreExpanded ();

	void
	saveExpanded ();

	void
	getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders);

	///  @name Destruction

	virtual
	void
	store () final override;

	///  @name Members

	std::set <std::string>                   projects;
	std::map <std::string, FolderElementPtr> folders;
	std::vector <std::string>                clipboard;
	std::unique_ptr <ScrollFreezer>          scrollFreezer;
	bool                                     changing;

};

} // puck
} // titania

#endif
