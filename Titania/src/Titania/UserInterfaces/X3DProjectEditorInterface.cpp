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

#include "X3DProjectEditorInterface.h"

namespace titania {
namespace puck {

void
X3DProjectEditorInterface::create (const std::string & filename)
{
	// Create Builder.
	m_builder = Gtk::Builder::create_from_file (filename);

	create ();
}

void
X3DProjectEditorInterface::create (std::initializer_list <std::string> filenames)
{
	// Create Builder.
	m_builder = Gtk::Builder::create ();

	for (const auto & filename : filenames)
		m_builder -> add_from_file (filename);

	create ();
}

void
X3DProjectEditorInterface::create ()
{
	// Get objects.
	m_AccelGroup    = Glib::RefPtr <Gtk::AccelGroup>::cast_dynamic (m_builder -> get_object ("AccelGroup"));
	m_FileStore     = Glib::RefPtr <Gtk::TreeStore>::cast_dynamic (m_builder -> get_object ("FileStore"));
	m_FileSelection = Glib::RefPtr <Gtk::TreeSelection>::cast_dynamic (m_builder -> get_object ("FileSelection"));
	m_FileColumn    = Glib::RefPtr <Gtk::TreeViewColumn>::cast_dynamic (m_builder -> get_object ("FileColumn"));
	m_IconRenderer  = Glib::RefPtr <Gtk::CellRendererPixbuf>::cast_dynamic (m_builder -> get_object ("IconRenderer"));
	m_NameRenderer  = Glib::RefPtr <Gtk::CellRendererText>::cast_dynamic (m_builder -> get_object ("NameRenderer"));

	// Get widgets.
	m_builder -> get_widget ("AddFilesDialog", m_AddFilesDialog);
	m_builder -> get_widget ("CancelButton1", m_CancelButton1);
	m_builder -> get_widget ("OkButton1", m_OkButton1);
	m_builder -> get_widget ("AddFilesHeaderBar", m_AddFilesHeaderBar);
	m_builder -> get_widget ("CopyFilesLabel", m_CopyFilesLabel);
	m_builder -> get_widget ("CopyFilesButton", m_CopyFilesButton);
	m_builder -> get_widget ("MoveFilesButton", m_MoveFilesButton);
	m_builder -> get_widget ("LinkFilesButton", m_LinkFilesButton);
	m_builder -> get_widget ("AddFolderDialog", m_AddFolderDialog);
	m_builder -> get_widget ("CancelButton", m_CancelButton);
	m_builder -> get_widget ("OkButton", m_OkButton);
	m_builder -> get_widget ("AddFolderHeaderBar", m_AddFolderHeaderBar);
	m_builder -> get_widget ("CopyFolderLabel", m_CopyFolderLabel);
	m_builder -> get_widget ("CopyFolderButton", m_CopyFolderButton);
	m_builder -> get_widget ("MoveFolderButton", m_MoveFolderButton);
	m_builder -> get_widget ("LinkFolderButton", m_LinkFolderButton);
	m_builder -> get_widget ("DragActionMenu", m_DragActionMenu);
	m_builder -> get_widget ("DragMoveMenuItem", m_DragMoveMenuItem);
	m_builder -> get_widget ("DragCopyMenuItem", m_DragCopyMenuItem);
	m_builder -> get_widget ("DragLinkMenuItem", m_DragLinkMenuItem);
	m_builder -> get_widget ("DragCancelMenuItem", m_DragCancelMenuItem);
	m_builder -> get_widget ("Window", m_Window);
	m_builder -> get_widget ("Widget", m_Widget);
	m_builder -> get_widget ("HeaderBar", m_HeaderBar);
	m_builder -> get_widget ("Title", m_Title);
	m_builder -> get_widget ("RemoveProjectButton", m_RemoveProjectButton);
	m_builder -> get_widget_derived ("FileView.FileView", m_FileView);
	m_builder -> get_widget ("CreateFilePopover", m_CreateFilePopover);
	m_builder -> get_widget ("CreateFileTypeButton", m_CreateFileTypeButton);
	m_builder -> get_widget ("CreateFileEntry", m_CreateFileEntry);
	m_builder -> get_widget ("CreateFileButton", m_CreateFileButton);
	m_builder -> get_widget ("CreateFolderPopover", m_CreateFolderPopover);
	m_builder -> get_widget ("CreateFolderEntry", m_CreateFolderEntry);
	m_builder -> get_widget ("CreateFolderButton", m_CreateFolderButton);
	m_builder -> get_widget ("RenameItemPopover", m_RenameItemPopover);
	m_builder -> get_widget ("RenameItemLabel", m_RenameItemLabel);
	m_builder -> get_widget ("RenameItemEntry", m_RenameItemEntry);
	m_builder -> get_widget ("RenameItemButton", m_RenameItemButton);
	m_builder -> get_widget ("ContextMenu", m_ContextMenu);
	m_builder -> get_widget ("OpenWithMenuItem", m_OpenWithMenuItem);
	m_builder -> get_widget ("OpenWithMenu", m_OpenWithMenu);
	m_builder -> get_widget ("ImportMenuItem", m_ImportMenuItem);
	m_builder -> get_widget ("AddItemMenuItem", m_AddItemMenuItem);
	m_builder -> get_widget ("AddNewFileMenuItem", m_AddNewFileMenuItem);
	m_builder -> get_widget ("AddFilesMenuItem", m_AddFilesMenuItem);
	m_builder -> get_widget ("AddNewFolderMenuItem", m_AddNewFolderMenuItem);
	m_builder -> get_widget ("AddExistingFolderMenuItem", m_AddExistingFolderMenuItem);
	m_builder -> get_widget ("FileSeparatorMenuItem", m_FileSeparatorMenuItem);
	m_builder -> get_widget ("RenameItemMenuItem", m_RenameItemMenuItem);
	m_builder -> get_widget ("CutItemMenuItem", m_CutItemMenuItem);
	m_builder -> get_widget ("CopyItemMenuItem", m_CopyItemMenuItem);
	m_builder -> get_widget ("PasteIntoFolderMenuItem", m_PasteIntoFolderMenuItem);
	m_builder -> get_widget ("MoveToTrashMenuItem", m_MoveToTrashMenuItem);

	// Connect object Gtk::Box with id 'Widget'.
	m_Widget -> signal_map () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_map));
	m_Widget -> signal_unmap () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_unmap));

	// Connect object Gtk::Button with id 'RemoveProjectButton'.
	m_RemoveProjectButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_remove_project_clicked));

	// Connect object Gtk::TreeView with id 'FileView'.
	m_FileView -> signal_drag_data_get () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_drag_data_get));
	m_FileView -> signal_drag_data_received () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_drag_data_received));
	m_FileView -> signal_focus_in_event () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_focus_in_event));
	m_FileView -> signal_focus_out_event () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_focus_out_event));
	m_FileView -> signal_row_activated () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_row_activated));
	m_FileView -> signal_test_expand_row () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_test_expand_row));

	// Connect object Gtk::TreeSelection with id 'FileSelection'.
	m_FileSelection -> signal_changed () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_selection_changed));

	// Connect object Gtk::Entry with id 'CreateFileEntry'.
	m_CreateFileEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_file_changed));
	m_CreateFileEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_file_key_press_event), false);

	// Connect object Gtk::Button with id 'CreateFileButton'.
	m_CreateFileButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_file_clicked));

	// Connect object Gtk::Entry with id 'CreateFolderEntry'.
	m_CreateFolderEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_folder_changed));
	m_CreateFolderEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_folder_key_press_event), false);

	// Connect object Gtk::Button with id 'CreateFolderButton'.
	m_CreateFolderButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_create_folder_clicked));

	// Connect object Gtk::Entry with id 'RenameItemEntry'.
	m_RenameItemEntry -> signal_changed () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_rename_item_changed));
	m_RenameItemEntry -> signal_key_press_event () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_rename_item_key_press_event), false);

	// Connect object Gtk::Button with id 'RenameItemButton'.
	m_RenameItemButton -> signal_clicked () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_rename_item_clicked));

	// Connect object Gtk::ImageMenuItem with id 'ImportMenuItem'.
	m_ImportMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_import_activate));
	m_AddNewFileMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_add_new_file_activate));

	// Connect object Gtk::MenuItem with id 'AddFilesMenuItem'.
	m_AddFilesMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_add_files_activate));

	// Connect object Gtk::ImageMenuItem with id 'AddNewFolderMenuItem'.
	m_AddNewFolderMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_add_new_folder_activate));

	// Connect object Gtk::MenuItem with id 'AddExistingFolderMenuItem'.
	m_AddExistingFolderMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_add_existing_folder_activate));

	// Connect object Gtk::ImageMenuItem with id 'RenameItemMenuItem'.
	m_RenameItemMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_rename_item_activate));
	m_CutItemMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_cut_item_activate));
	m_CopyItemMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_copy_item_activate));
	m_PasteIntoFolderMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_paste_into_folder_activate));
	m_MoveToTrashMenuItem -> signal_activate () .connect (sigc::mem_fun (this, &X3DProjectEditorInterface::on_move_to_trash_activate));
}

X3DProjectEditorInterface::~X3DProjectEditorInterface ()
{
	delete m_AddFilesDialog;
	delete m_AddFolderDialog;
	delete m_Window;
}

} // puck
} // titania
