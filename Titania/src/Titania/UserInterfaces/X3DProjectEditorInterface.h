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

#ifndef __TMP_GLAD2CPP_PROJECT_EDITOR_H__
#define __TMP_GLAD2CPP_PROJECT_EDITOR_H__

#include "../Base/X3DEditorInterface.h"

#include "../Editors/ProjectsEditor/FileView.h"

#include <gtkmm.h>
#include <string>

namespace titania {
namespace puck {

/**
 *  Gtk Interface for ProjectEditor.
 */
class X3DProjectEditorInterface :
	public X3DEditorInterface
{
public:

	///  @name Construction

	X3DProjectEditorInterface () :
		X3DEditorInterface ()
	{ }

	template <class ... Arguments>
	X3DProjectEditorInterface (const std::string & filename, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filename); }

	template <class ... Arguments>
	X3DProjectEditorInterface (std::initializer_list <std::string> filenames, const Arguments & ... arguments) :
		X3DEditorInterface (arguments ...)
	{ create (filenames); }

	///  @name Member access

	const Glib::RefPtr <Gtk::Builder> &
	getBuilder () const
	{ return m_builder; }

	const Glib::RefPtr <Gtk::AccelGroup> &
	getAccelGroup () const
	{ return m_AccelGroup; }

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

	Gtk::Dialog &
	getAddFilesDialog () const
	{ return *m_AddFilesDialog; }

	Gtk::Button &
	getCancelButton1 () const
	{ return *m_CancelButton1; }

	Gtk::Button &
	getOkButton1 () const
	{ return *m_OkButton1; }

	Gtk::HeaderBar &
	getAddFilesHeaderBar () const
	{ return *m_AddFilesHeaderBar; }

	Gtk::Label &
	getCopyFilesLabel () const
	{ return *m_CopyFilesLabel; }

	Gtk::RadioButton &
	getCopyFilesButton () const
	{ return *m_CopyFilesButton; }

	Gtk::RadioButton &
	getMoveFilesButton () const
	{ return *m_MoveFilesButton; }

	Gtk::RadioButton &
	getLinkFilesButton () const
	{ return *m_LinkFilesButton; }

	Gtk::Dialog &
	getAddFolderDialog () const
	{ return *m_AddFolderDialog; }

	Gtk::Button &
	getCancelButton () const
	{ return *m_CancelButton; }

	Gtk::Button &
	getOkButton () const
	{ return *m_OkButton; }

	Gtk::HeaderBar &
	getAddFolderHeaderBar () const
	{ return *m_AddFolderHeaderBar; }

	Gtk::Label &
	getCopyFolderLabel () const
	{ return *m_CopyFolderLabel; }

	Gtk::RadioButton &
	getCopyFolderButton () const
	{ return *m_CopyFolderButton; }

	Gtk::RadioButton &
	getMoveFolderButton () const
	{ return *m_MoveFolderButton; }

	Gtk::RadioButton &
	getLinkFolderButton () const
	{ return *m_LinkFolderButton; }

	Gtk::Menu &
	getDragActionMenu () const
	{ return *m_DragActionMenu; }

	Gtk::MenuItem &
	getDragMoveMenuItem () const
	{ return *m_DragMoveMenuItem; }

	Gtk::MenuItem &
	getDragCopyMenuItem () const
	{ return *m_DragCopyMenuItem; }

	Gtk::MenuItem &
	getDragLinkMenuItem () const
	{ return *m_DragLinkMenuItem; }

	Gtk::MenuItem &
	getDragCancelMenuItem () const
	{ return *m_DragCancelMenuItem; }

	Gtk::Window &
	getWindow () const final override
	{ return *m_Window; }

	Gtk::Box &
	getWidget () const final override
	{ return *m_Widget; }

	Gtk::HeaderBar &
	getHeaderBar () const
	{ return *m_HeaderBar; }

	Gtk::Label &
	getTitle () const
	{ return *m_Title; }

	Gtk::Button &
	getRemoveProjectButton () const
	{ return *m_RemoveProjectButton; }

	FileView &
	getFileView () const
	{ return *m_FileView; }

	Gtk::Popover &
	getCreateFilePopover () const
	{ return *m_CreateFilePopover; }

	Gtk::ComboBoxText &
	getCreateFileTypeButton () const
	{ return *m_CreateFileTypeButton; }

	Gtk::Entry &
	getCreateFileEntry () const
	{ return *m_CreateFileEntry; }

	Gtk::Button &
	getCreateFileButton () const
	{ return *m_CreateFileButton; }

	Gtk::Popover &
	getCreateFolderPopover () const
	{ return *m_CreateFolderPopover; }

	Gtk::Entry &
	getCreateFolderEntry () const
	{ return *m_CreateFolderEntry; }

	Gtk::Button &
	getCreateFolderButton () const
	{ return *m_CreateFolderButton; }

	Gtk::Popover &
	getRenameItemPopover () const
	{ return *m_RenameItemPopover; }

	Gtk::Label &
	getRenameItemLabel () const
	{ return *m_RenameItemLabel; }

	Gtk::Entry &
	getRenameItemEntry () const
	{ return *m_RenameItemEntry; }

	Gtk::Button &
	getRenameItemButton () const
	{ return *m_RenameItemButton; }

	Gtk::Menu &
	getContextMenu () const
	{ return *m_ContextMenu; }

	Gtk::ImageMenuItem &
	getOpenWithMenuItem () const
	{ return *m_OpenWithMenuItem; }

	Gtk::Menu &
	getOpenWithMenu () const
	{ return *m_OpenWithMenu; }

	Gtk::ImageMenuItem &
	getImportMenuItem () const
	{ return *m_ImportMenuItem; }

	Gtk::ImageMenuItem &
	getAddItemMenuItem () const
	{ return *m_AddItemMenuItem; }

	Gtk::ImageMenuItem &
	getAddNewFileMenuItem () const
	{ return *m_AddNewFileMenuItem; }

	Gtk::MenuItem &
	getAddFilesMenuItem () const
	{ return *m_AddFilesMenuItem; }

	Gtk::ImageMenuItem &
	getAddNewFolderMenuItem () const
	{ return *m_AddNewFolderMenuItem; }

	Gtk::MenuItem &
	getAddExistingFolderMenuItem () const
	{ return *m_AddExistingFolderMenuItem; }

	Gtk::SeparatorMenuItem &
	getFileSeparatorMenuItem () const
	{ return *m_FileSeparatorMenuItem; }

	Gtk::ImageMenuItem &
	getRenameItemMenuItem () const
	{ return *m_RenameItemMenuItem; }

	Gtk::ImageMenuItem &
	getCutItemMenuItem () const
	{ return *m_CutItemMenuItem; }

	Gtk::ImageMenuItem &
	getCopyItemMenuItem () const
	{ return *m_CopyItemMenuItem; }

	Gtk::ImageMenuItem &
	getPasteIntoFolderMenuItem () const
	{ return *m_PasteIntoFolderMenuItem; }

	Gtk::ImageMenuItem &
	getMoveToTrashMenuItem () const
	{ return *m_MoveToTrashMenuItem; }

	///  @name Signal handlers

	virtual
	void
	on_map () = 0;

	virtual
	void
	on_unmap () = 0;

	virtual
	void
	on_remove_project_clicked () = 0;

	virtual
	void
	on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context, Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	void
	on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context, int x, int y, const Gtk::SelectionData & selection_data, guint info, guint time) = 0;

	virtual
	bool
	on_focus_in_event (GdkEventFocus* focus_event) = 0;

	virtual
	bool
	on_focus_out_event (GdkEventFocus* gdk_event) = 0;

	virtual
	void
	on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column) = 0;

	virtual
	bool
	on_test_expand_row (const Gtk::TreeModel::iterator & iter, const Gtk::TreeModel::Path & path) = 0;

	virtual
	void
	on_selection_changed () = 0;

	virtual
	void
	on_create_file_changed () = 0;

	virtual
	bool
	on_create_file_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_create_file_clicked () = 0;

	virtual
	void
	on_create_folder_changed () = 0;

	virtual
	bool
	on_create_folder_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_create_folder_clicked () = 0;

	virtual
	void
	on_rename_item_changed () = 0;

	virtual
	bool
	on_rename_item_key_press_event (GdkEventKey* key_event) = 0;

	virtual
	void
	on_rename_item_clicked () = 0;

	virtual
	void
	on_import_activate () = 0;

	virtual
	void
	on_add_new_file_activate () = 0;

	virtual
	void
	on_add_files_activate () = 0;

	virtual
	void
	on_add_new_folder_activate () = 0;

	virtual
	void
	on_add_existing_folder_activate () = 0;

	virtual
	void
	on_rename_item_activate () = 0;

	virtual
	void
	on_cut_item_activate () = 0;

	virtual
	void
	on_copy_item_activate () = 0;

	virtual
	void
	on_paste_into_folder_activate () = 0;

	virtual
	void
	on_move_to_trash_activate () = 0;

	///  @name Destruction

	virtual
	~X3DProjectEditorInterface () override;


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
	Glib::RefPtr <Gtk::AccelGroup> m_AccelGroup;
	Glib::RefPtr <Gtk::TreeStore> m_FileStore;
	Glib::RefPtr <Gtk::TreeSelection> m_FileSelection;
	Glib::RefPtr <Gtk::TreeViewColumn> m_FileColumn;
	Glib::RefPtr <Gtk::CellRendererPixbuf> m_IconRenderer;
	Glib::RefPtr <Gtk::CellRendererText> m_NameRenderer;
	Gtk::Dialog* m_AddFilesDialog;
	Gtk::Button* m_CancelButton1;
	Gtk::Button* m_OkButton1;
	Gtk::HeaderBar* m_AddFilesHeaderBar;
	Gtk::Label* m_CopyFilesLabel;
	Gtk::RadioButton* m_CopyFilesButton;
	Gtk::RadioButton* m_MoveFilesButton;
	Gtk::RadioButton* m_LinkFilesButton;
	Gtk::Dialog* m_AddFolderDialog;
	Gtk::Button* m_CancelButton;
	Gtk::Button* m_OkButton;
	Gtk::HeaderBar* m_AddFolderHeaderBar;
	Gtk::Label* m_CopyFolderLabel;
	Gtk::RadioButton* m_CopyFolderButton;
	Gtk::RadioButton* m_MoveFolderButton;
	Gtk::RadioButton* m_LinkFolderButton;
	Gtk::Menu* m_DragActionMenu;
	Gtk::MenuItem* m_DragMoveMenuItem;
	Gtk::MenuItem* m_DragCopyMenuItem;
	Gtk::MenuItem* m_DragLinkMenuItem;
	Gtk::MenuItem* m_DragCancelMenuItem;
	Gtk::Window* m_Window;
	Gtk::Box* m_Widget;
	Gtk::HeaderBar* m_HeaderBar;
	Gtk::Label* m_Title;
	Gtk::Button* m_RemoveProjectButton;
	FileView* m_FileView;
	Gtk::Popover* m_CreateFilePopover;
	Gtk::ComboBoxText* m_CreateFileTypeButton;
	Gtk::Entry* m_CreateFileEntry;
	Gtk::Button* m_CreateFileButton;
	Gtk::Popover* m_CreateFolderPopover;
	Gtk::Entry* m_CreateFolderEntry;
	Gtk::Button* m_CreateFolderButton;
	Gtk::Popover* m_RenameItemPopover;
	Gtk::Label* m_RenameItemLabel;
	Gtk::Entry* m_RenameItemEntry;
	Gtk::Button* m_RenameItemButton;
	Gtk::Menu* m_ContextMenu;
	Gtk::ImageMenuItem* m_OpenWithMenuItem;
	Gtk::Menu* m_OpenWithMenu;
	Gtk::ImageMenuItem* m_ImportMenuItem;
	Gtk::ImageMenuItem* m_AddItemMenuItem;
	Gtk::ImageMenuItem* m_AddNewFileMenuItem;
	Gtk::MenuItem* m_AddFilesMenuItem;
	Gtk::ImageMenuItem* m_AddNewFolderMenuItem;
	Gtk::MenuItem* m_AddExistingFolderMenuItem;
	Gtk::SeparatorMenuItem* m_FileSeparatorMenuItem;
	Gtk::ImageMenuItem* m_RenameItemMenuItem;
	Gtk::ImageMenuItem* m_CutItemMenuItem;
	Gtk::ImageMenuItem* m_CopyItemMenuItem;
	Gtk::ImageMenuItem* m_PasteIntoFolderMenuItem;
	Gtk::ImageMenuItem* m_MoveToTrashMenuItem;

};

} // puck
} // titania

#endif
