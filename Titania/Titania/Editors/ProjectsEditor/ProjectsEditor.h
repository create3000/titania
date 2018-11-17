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
#include "../../Editors/ProjectsEditor/X3DFileBrowser.h"

namespace titania {
namespace puck {

class BrowserWindow;
class ScrollFreezer;

class ProjectsEditor :
	virtual public X3DProjectsEditorInterface,
	public X3DFileBrowser <X3DProjectsEditorInterface>
{
public:

	///  @name Construction

	ProjectsEditor (X3DBrowserWindow* const browserWindow);

	///  @name Destruction

	virtual
	~ProjectsEditor () final override;


private:

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
	on_focus_in_event (GdkEventFocus* event) final override;

	virtual
	bool
	on_focus_out_event (GdkEventFocus* event) final override;

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

	virtual
	void
	on_add_files_activate () final override;

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

	virtual
	void
	on_add_existing_folder_activate () final override;

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

	Glib::RefPtr <Gio::File>
	getRenameDestination () const;

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
	on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context,
                     Gtk::SelectionData & selection_data,
                     guint info,
                     guint time) final override;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
	                       int x, int y,
	                       const Gtk::SelectionData & selection_data,
	                       guint info,
	                       guint time) final override;

	void
	on_drag_action_activate (const TransferAction action,
	                         const std::vector <Glib::RefPtr <Gio::File>> & files,
	                         const Glib::RefPtr <Gio::File> & folder);

	TransferAction
	getTransferAction (const Gdk::DragAction action);

	virtual
	void
	on_selection_changed () final override;

	void
	set_execution_context ();

	///  @name Misc operations

	void
	createOpenWithMenu (const Glib::RefPtr <Gio::File> & file);

	void
	createOpenWithMenuItem (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file);

	void
	launchFile (const Glib::RefPtr <Gio::File> & file);

	bool
	canOpenFile (const Glib::RefPtr <Gio::File> & file);

	std::string
	getFileName (const std::string & filename, const std::string & extension) const;
	
	std::string
	getExtension (const int32_t type) const;

	Gdk::Rectangle
	getRectangle (const Gtk::TreePath & path) const;

	///  @name Destruction

	virtual
	void
	store () final override;

	///  @name Members

	sigc::connection dragMoveConnection;
	sigc::connection dragCopyConnection;
	sigc::connection dragLinkConnection;

	int32_t button;
	bool    changing;

};

} // puck
} // titania

#endif
