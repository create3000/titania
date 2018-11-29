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

#include "ProjectEditor.h"

#include "../../Bits/File.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/FileOpenDialog.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"

#include <Titania/X3D/InputOutput/GoldenGate.h>
#include <Titania/String.h>
#include <regex>

namespace titania {
namespace puck {

ProjectEditor::ProjectEditor (X3DBrowserWindow* const browserWindow, const basic::uri & folder) :
	                          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	                 X3DProjectEditorInterface (get_ui ("Editors/ProjectEditor.glade")),
	X3DFileBrowser <X3DProjectEditorInterface> (),
	                                    folder (folder),
	                        dragMoveConnection (),
	                        dragCopyConnection (),
	                        dragLinkConnection (),
	                                   removed (false),
	                                  changing (false)
{
	setName (getWidget () .get_name () + "." + basic::to_string (std::hash <std::string> () (folder), std::locale::classic ()));

	getFileView () .signal_display_menu () .connect (sigc::mem_fun (this, &ProjectEditor::on_display_menu));

	getFileView () .enable_model_drag_source ({
		Gtk::TargetEntry ("text/uri-list", Gtk::TARGET_SAME_APP)
	},
	Gdk::BUTTON1_MASK, Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK | Gdk::ACTION_ASK);

	getFileView () .enable_model_drag_dest ({
		Gtk::TargetEntry ("text/uri-list", Gtk::TARGET_SAME_WIDGET),
		Gtk::TargetEntry ("text/uri-list", Gtk::TARGET_SAME_APP),
		Gtk::TargetEntry ("STRING", Gtk::TARGET_OTHER_APP)
	},
	Gdk::ACTION_COPY | Gdk::ACTION_MOVE | Gdk::ACTION_LINK | Gdk::ACTION_ASK);

	getCutItemMenuItem ()         .add_accelerator ("activate", getAccelGroup (), GDK_KEY_X, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getCopyItemMenuItem ()        .add_accelerator ("activate", getAccelGroup (), GDK_KEY_C, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getPasteIntoFolderMenuItem () .add_accelerator ("activate", getAccelGroup (), GDK_KEY_V, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	setTitleBar (getAddFilesDialog (),  getAddFilesHeaderBar ());
	setTitleBar (getAddFolderDialog (), getAddFolderHeaderBar ());

	setup ();
}

void
ProjectEditor::initialize ()
{
	X3DProjectEditorInterface::initialize ();
	X3DFileBrowser <X3DProjectEditorInterface>::initialize ();
}

void
ProjectEditor::configure ()
{
	X3DProjectEditorInterface::configure ();
	X3DFileBrowser <X3DProjectEditorInterface>::configure ();

	getTitle () .set_text (folder .basename () + " (" + _ ("Project") + ")");
	getTitle () .set_tooltip_text (folder .str ());

	addRootFolder (Gio::File::create_for_path (folder));
	restoreExpanded ();

	set_execution_context ();
}

void
ProjectEditor::on_remove_project_clicked ()
{
	removed = true;

	getConfig () -> remove ();
}

void
ProjectEditor::on_map ()
{
	getCurrentContext () .addInterest (&ProjectEditor::set_execution_context, this);

	set_execution_context ();
}

void
ProjectEditor::on_unmap ()
{
	getCurrentContext () .removeInterest (&ProjectEditor::set_execution_context, this);
}

bool
ProjectEditor::on_focus_in_event (GdkEventFocus* event)
{
	getBrowserWindow () -> setAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	return false;
}

bool
ProjectEditor::on_focus_out_event (GdkEventFocus* event)
{
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

void
ProjectEditor::on_display_menu (GdkEventButton* event)
{
	const auto selectedFiles = getSelectedFiles ();

	if (selectedFiles .empty ())
		return;

	if (selectedFiles .size () == 1)
	{
		const auto file      = selectedFiles .front ();
		const auto fileInfo  = file -> query_info ();
		const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;

		createOpenWithMenu (file);

		// Show hide menu items.

		getImportMenuItem ()     .set_visible (canOpenFile (file));
		getAddItemMenuItem ()    .set_visible (directory);
		getRenameItemMenuItem () .set_visible (true);
	}
	else
	{
		getImportMenuItem ()        .set_visible (false);
		getOpenWithMenuItem ()      .set_visible (false);
		getAddItemMenuItem ()       .set_visible (false);
		getFileSeparatorMenuItem () .set_visible (false);
		getRenameItemMenuItem ()    .set_visible (false);
	}

	// Show context menu.

	getContextMenu () .popup (event -> button, event -> time);
}

void
ProjectEditor::on_open_with_activate (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file)
{
	appInfo -> launch (file);
}

void
ProjectEditor::on_import_activate ()
{
	try
	{
		const auto file     = getSelectedFiles () .front ();
		const auto undoStep = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Import »%s« From Project"), file -> get_basename () .c_str ()));
		const auto nodes    = getBrowserWindow () -> import ({ file -> get_uri () }, undoStep);

		getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectEditor::on_add_new_file_activate ()
{
	changing = true;

	getCreateFileTypeButton () .set_active (getConfig () -> getItem <int32_t> ("fileType", 0));
	getCreateFileEntry ()      .set_text ("");
	getCreateFileButton ()     .set_sensitive (false);
	getCreateFilePopover ()    .set_pointing_to (getRectangle (getFileSelection () -> get_selected_rows () .front ()));
	getCreateFilePopover ()    .popup ();
	getCreateFileEntry ()      .grab_focus ();

	changing = false;
}

void
ProjectEditor::on_create_file_changed ()
{
	try
	{
		if (changing)
			return;

		getCreateFileButton () .set_sensitive (bool (getNewFile ()));
	}
	catch (...)
	{
		getWidget () .error_bell ();
		getCreateFileButton () .set_sensitive (false);
	}
}

void
ProjectEditor::on_create_file_clicked ()
{
	try
	{
		const auto file = getNewFile ();

		getConfig () -> setItem <int32_t> ("fileType", getCreateFileTypeButton () .get_active_row_number ());

		// Create X3D file.

		getCreateFilePopover () .popdown ();
	
		const auto command = std::vector <std::string> ({ Glib::find_program_in_path ("titania"), get_page ("about/new.x3dv"), "-e", file -> get_path () });

		Glib::spawn_sync (Glib::get_current_dir (), command);
	
		on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);

		unselectAll ();
		selectFile (file);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const std::invalid_argument & error)
	{
		getWidget () .error_bell ();
	}
}

bool
ProjectEditor::on_create_file_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			on_create_file_clicked ();
			return true;
		}
		case GDK_KEY_Escape:
		{
			getCreateFilePopover () .popdown ();
			return true;
		}
		default:
			break;
	}

	return false;
}

Glib::RefPtr <Gio::File>
ProjectEditor::getNewFile () const
{
	const auto extension = getExtension (getCreateFileTypeButton () .get_active_row_number ());
	const auto parent    = getSelectedFiles () .front ();
	const auto file      = parent -> get_child (getFileName (getCreateFileEntry () .get_text () .raw (), extension));

	if (getCreateFileEntry () .get_text () .empty ())
		throw std::invalid_argument ("getNewFile");

	if (file -> query_exists ())
		throw std::invalid_argument ("getNewFile");

	return file;
}

void
ProjectEditor::on_add_files_activate ()
{
	try
	{
		const auto dialog = createDialog <FileOpenDialog> ("FileOpenDialog");

		dialog -> getWindow () .set_select_multiple (true);

		if (not dialog -> run ())
			return;

		const auto folder = getSelectedFiles () .front ();

		if (dialog -> getWindow () .get_current_folder () == folder -> get_path ())
			return;

		getCopyFilesButton () .set_active ();

		const auto response = getAddFilesDialog () .run ();

		getAddFilesDialog () .hide ();

		if (response not_eq Gtk::RESPONSE_OK)
			return;
	
		std::vector <Glib::RefPtr <Gio::File>> selection;

		for (const auto & source : dialog -> getWindow () .get_files ())
		{
			const auto destination = folder -> get_child (source -> get_basename ());
			auto       action      = TransferAction::COPY;

			if (getCopyFilesButton () .get_active ())
				action = TransferAction::COPY;
			else if (getMoveFilesButton () .get_active ())
				action = TransferAction::MOVE;
			else if (getLinkFilesButton () .get_active ())
				action = TransferAction::LINK;

			transferFile (action, source, destination);

			selection .emplace_back (destination);
		}

		unselectAll ();

		for (const auto & file : selection)
			selectFile (file);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectEditor::on_add_new_folder_activate ()
{
	changing = true;

	getCreateFolderEntry ()   .set_text ("");
	getCreateFolderButton ()  .set_sensitive (false);
	getCreateFolderPopover () .set_pointing_to (getRectangle (getFileSelection () -> get_selected_rows () .front ()));
	getCreateFolderPopover () .popup ();

	changing = false;
}

void
ProjectEditor::on_create_folder_changed ()
{
	try
	{
		if (changing)
			return;

		getCreateFolderButton () .set_sensitive (bool (getNewFolder ()));
	}
	catch (...)
	{
		getWidget () .error_bell ();
		getCreateFolderButton () .set_sensitive (false);
	}
}

void
ProjectEditor::on_create_folder_clicked ()
{
	try
	{
		const auto folder = getNewFolder ();
	
		// Create folder.
	
		getCreateFolderPopover () .popdown ();
	
		folder -> make_directory_with_parents ();
	
		on_file_changed (folder, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);

		unselectAll ();
		selectFile (folder);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const std::invalid_argument & error)
	{
		getWidget () .error_bell ();
	}
}

bool
ProjectEditor::on_create_folder_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			on_create_folder_clicked ();
			return true;
		}
		case GDK_KEY_Escape:
		{
			getCreateFolderPopover () .popdown ();
			return true;
		}
		default:
			break;
	}

	return false;
}

Glib::RefPtr <Gio::File>
ProjectEditor::getNewFolder () const
{
	const auto parent = getSelectedFiles () .front ();
	const auto folder = parent -> get_child (getCreateFolderEntry () .get_text ());

	if (getCreateFolderEntry () .get_text () .empty ())
		throw std::invalid_argument ("getNewFolder");

	if (folder -> query_exists ())
		throw std::invalid_argument ("getNewFolder");

	return folder;
}

void
ProjectEditor::on_add_existing_folder_activate ()
{
	try
	{
		const auto dialog = createDialog <OpenFolderDialog> ("OpenFolderDialog");
	
		if (not dialog -> run ())
			return;
	
		const auto source = dialog -> getWindow () .get_file ();
		const auto folder = getSelectedFiles () .front ();

		if (source -> has_parent () and source -> get_parent () -> get_path () == folder -> get_path ())
			return;

		getCopyFolderLabel () .set_markup (basic::sprintf (_ ("The folder <b>»%s«</b> is outside the target folder. What would you like to do?"), folder -> get_path () .c_str ()));
		getCopyFilesButton () .set_active ();

		const auto response = getAddFilesDialog () .run ();
	
		getAddFilesDialog () .hide ();
	
		if (response not_eq Gtk::RESPONSE_OK)
			return;
	
		const auto destination = folder -> get_child (source -> get_basename ());
		auto       action      = TransferAction::COPY;

		if (getCopyFilesButton () .get_active ())
			action = TransferAction::COPY;
		else if (getMoveFilesButton () .get_active ())
			action = TransferAction::MOVE;
		else if (getLinkFilesButton () .get_active ())
			action = TransferAction::LINK;

		transferFile (action, source, destination);

		unselectAll ();
		selectFile (destination);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectEditor::on_rename_item_activate ()
{
	changing = true;

	const auto file      = getSelectedFiles () .front ();
	const auto fileInfo  = file -> query_info ();
	const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
	const auto basename  = basic::uri (file -> get_basename ());
	const auto name      = Glib::ustring (directory ? basename .basename () : basename .stem ());

	getRenameItemLabel () .set_text (directory ? _ ("Folder Name") : _ ("File Name"));
	getRenameItemEntry () .set_placeholder_text (directory ? _ ("Folder Name") : _ ("File Name"));

	getRenameItemEntry ()   .set_text (file -> get_basename ());
	getRenameItemEntry ()   .select_region (0, name .size ());
	getRenameItemButton ()  .set_sensitive (false);
	getRenameItemPopover () .set_pointing_to (getRectangle (getFileSelection () -> get_selected_rows () .front ()));
	getRenameItemPopover () .popup ();

	// Workaround, Gtk::Entry::select_region does not work.
	gtk_editable_select_region (GTK_EDITABLE (getRenameItemEntry () .gobj ()), 0, name .size ());

	changing = false;
}

void
ProjectEditor::on_rename_item_changed ()
{
	try
	{
		if (changing)
			return;

		getRenameItemButton () .set_sensitive (bool (getRenameDestination ()));
	}
	catch (...)
	{
		getWidget () .error_bell ();
		getRenameItemButton () .set_sensitive (false);
	}
}

void
ProjectEditor::on_rename_item_clicked ()
{
	try
	{
		const auto source      = getSelectedFiles () .front ();
		const auto destination = getRenameDestination ();

		// Create X3D file.

		getRenameItemPopover () .popdown ();

		source -> move (destination);

		on_file_changed (source, destination, Gio::FILE_MONITOR_EVENT_RENAMED);

		unselectAll ();
		selectFile (destination);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (const std::invalid_argument & error)
	{
		getWidget () .error_bell ();
	}
}

bool
ProjectEditor::on_rename_item_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Return:
		case GDK_KEY_KP_Enter:
		{
			on_rename_item_clicked ();
			return true;
		}
		case GDK_KEY_Escape:
		{
			getRenameItemPopover () .popdown ();
			return true;
		}
		default:
			break;
	}

	return false;
}

Glib::RefPtr <Gio::File>
ProjectEditor::getRenameDestination () const
{
	const auto file        = getSelectedFiles () .front ();
	const auto parent      = file -> get_parent ();
	const auto destination = parent -> get_child (getRenameItemEntry () .get_text ());

	if (getRenameItemEntry () .get_text () .empty ())
		throw std::invalid_argument ("getRenameItem");

	if (destination -> query_exists ())
		throw std::invalid_argument ("getRenameItem");

	return destination;
}

void
ProjectEditor::on_cut_item_activate ()
{
	cutItems (getSelectedFiles ());
}

void
ProjectEditor::on_copy_item_activate ()
{
	copyItems (getSelectedFiles ());
}

void
ProjectEditor::on_paste_into_folder_activate ()
{
	pasteIntoFolder (getSelectedFiles () .front ());
}

void
ProjectEditor::on_move_to_trash_activate ()
{
	for (const auto & file : getSelectedFiles ())
		on_move_to_trash_activate (file);

	for (const auto & file : getSelectedFiles ())
		on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_DELETED);

	unselectAll ();
}

void
ProjectEditor::on_move_to_trash_activate (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		if (getRootFolders () .count (file -> get_path ()))
		{
			const auto dialog = createDialog <MessageDialog> ("MessageDialog");
	
			dialog -> setType (Gtk::MESSAGE_QUESTION);
			dialog -> setMessage ("You are about to remove a project folder to trash!");
			dialog -> setText (basic::sprintf (_ ("Do you realy want to move project »%s« to trash?"), file -> get_basename () .c_str ()));

			if (dialog -> run () not_eq Gtk::RESPONSE_OK)
				return;

			removeRootFolder (file);
		}

		file -> trash ();
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;

		if (error .code () == Gio::Error::NOT_SUPPORTED)
			on_remove_file_activate (file);
	}
}

void
ProjectEditor::on_remove_file_activate (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		const auto dialog = createDialog <MessageDialog> ("MessageDialog");

		dialog -> setType (Gtk::MESSAGE_QUESTION);
		dialog -> setMessage (basic::sprintf (_ ("Are you sure you want to permanently delete »%s«?"), file -> get_basename () .c_str ()));
		dialog -> setText (_ ("If you delete an item, it is permanently lost."));
		dialog -> getOkButton () .set_label ("gtk-delete");

		if (dialog -> run () not_eq Gtk::RESPONSE_OK)
			return;

		File::removeFile (file);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	launchFile (getFile (getFileStore () -> get_iter (path)));
}

void
ProjectEditor::on_drag_data_get (const Glib::RefPtr <Gdk::DragContext> & context,
                                  Gtk::SelectionData & selection_data,
                                  guint info,
                                  guint time)
{
	std::vector <Glib::ustring> uris;

	for (const auto & file : getSelectedFiles ())
		uris .emplace_back (file -> get_uri ());

	selection_data .set_uris (uris);
}

void
ProjectEditor::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                       int x, int y,
                                       const Gtk::SelectionData & selection_data,
                                       guint info,
                                       guint time)
{
	// Update list store.

	Gtk::TreePath             destinationPath;
	Gtk::TreeViewDropPosition position;

	if (getFileView () .get_dest_row_at_pos (x, y, destinationPath, position))
	{
		const auto destination = getFile (getFileStore () -> get_iter (destinationPath));

		std::vector <Glib::RefPtr <Gio::File>> files;

		if (selection_data .get_data_type () == "text/uri-list")
		{
			const auto strings = selection_data .get_uris ();

			for (const auto & string : strings)
				files .emplace_back (Gio::File::create_for_uri (string));
		}

		if (selection_data .get_data_type () == "STRING")
		{
			auto strings = std::vector <std::string> ();

			basic::split (std::back_inserter (strings), basic::trim (selection_data .get_data_as_string ()), "\r\n");

			for (const auto & string : strings)
				files .emplace_back (Gio::File::create_for_uri (Glib::uri_unescape_string (string)));
		}

		if (not files .empty ())
		{
			const auto folder = File::getContainingFolder (destination);

			if (folder)
			{
				if (context -> get_selected_action () == Gdk::ACTION_ASK)
				{
					dragMoveConnection = getDragMoveMenuItem () .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &ProjectEditor::on_drag_action_activate), TransferAction::MOVE, files, folder));
					dragCopyConnection = getDragCopyMenuItem () .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &ProjectEditor::on_drag_action_activate), TransferAction::COPY, files, folder));
					dragLinkConnection = getDragLinkMenuItem () .signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &ProjectEditor::on_drag_action_activate), TransferAction::LINK, files, folder));

					getDragActionMenu () .popup (x, y);
				}
				else
				{
					transferFiles (getTransferAction (context -> get_selected_action ()), files, folder);
				}

				context -> drag_finish (true, false, time);
				return;
			}
		}
	}

	context -> drag_finish (false, false, time);
}

void
ProjectEditor::on_drag_action_activate (const TransferAction action,
                                         const std::vector <Glib::RefPtr <Gio::File>> & files,
                                         const Glib::RefPtr <Gio::File> & folder)
{
	dragMoveConnection .disconnect ();
	dragCopyConnection .disconnect ();
	dragLinkConnection .disconnect ();

	transferFiles (action, files, folder);
 }

ProjectEditor::TransferAction
ProjectEditor::getTransferAction (const Gdk::DragAction action)
{
	switch (action)
	{
		case Gdk::ACTION_DEFAULT:
		case Gdk::ACTION_MOVE:
			return TransferAction::MOVE;
		case Gdk::ACTION_COPY:
			return TransferAction::COPY;
		case Gdk::ACTION_LINK:
			return TransferAction::LINK;
		case Gdk::ACTION_ASK:
		case Gdk::ACTION_PRIVATE:
			return TransferAction::COPY;
	}

	return TransferAction::COPY;
}

void
ProjectEditor::on_selection_changed ()
{
	try
	{
		const auto selectedFiles = getSelectedFiles ();

		getCutItemMenuItem ()     .set_sensitive (selectedFiles .size ());
		getCopyItemMenuItem ()    .set_sensitive (selectedFiles .size ());
		getMoveToTrashMenuItem () .set_sensitive (selectedFiles .size ());

		switch (selectedFiles .size ())
		{
			case 1:
			{
				const auto file      = selectedFiles .front ();
				const auto fileInfo  = file -> query_info ();
				const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;

				getOpenWithMenuItem ()        .set_sensitive (true);
				getImportMenuItem ()          .set_sensitive (true);
				getAddItemMenuItem ()         .set_sensitive (directory);
				getRenameItemMenuItem ()      .set_sensitive (true);
				getPasteIntoFolderMenuItem () .set_sensitive (directory and getClipboard () .size ());
				break;
			}
			case 0:
			default:
			{
				getOpenWithMenuItem ()        .set_sensitive (false);
				getImportMenuItem ()          .set_sensitive (false);
				getAddItemMenuItem ()         .set_sensitive (false);
				getRenameItemMenuItem ()      .set_sensitive (false);
				getPasteIntoFolderMenuItem () .set_sensitive (false);
				break;
			}
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectEditor::set_execution_context ()
{
	if (getCurrentContext () -> getWorldURL () .is_local ())
	{
		unselectAll ();
		selectFile (Gio::File::create_for_path (getCurrentContext () -> getWorldURL () .path ()));
	}
}

void
ProjectEditor::createOpenWithMenu (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		// Clear »Open With ...« menu.
	
		for (const auto & widget : getOpenWithMenu () .get_children ())
			getOpenWithMenu () .remove (*widget);
	
		// Populate »Open With ...« menu.
	
		const auto contentType  = file -> query_info () -> get_content_type ();
		const auto appInfo      = file -> query_default_handler ();
		const auto appInfos     = Gio::AppInfo::get_all_for_type (contentType);
		auto       appInfoIndex = std::map <std::string, Glib::RefPtr <Gio::AppInfo>> ();
	
		// Create app info index.
	
		for (const auto & appInfo : appInfos)
			appInfoIndex .emplace (appInfo -> get_display_name (), appInfo);
	
		appInfoIndex .erase (appInfo -> get_display_name ());
	
		// Create menu items.
	
		createOpenWithMenuItem (appInfo, file);
	
		if (not appInfoIndex .empty ())
			getOpenWithMenu () .append (*Gtk::manage (new Gtk::SeparatorMenuItem ()));
	
		for (const auto & pair : appInfoIndex)
			createOpenWithMenuItem (pair .second, file);
	
		// Show menu items.
	
		getOpenWithMenu ()     .show_all ();
		getOpenWithMenuItem () .set_visible (appInfos .size ());
	}
	catch (const Glib::Error & error)
	{
		getOpenWithMenuItem () .set_visible (false);
	}
}

void
ProjectEditor::createOpenWithMenuItem (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file)
{
	const auto image    = Gtk::manage (new Gtk::Image ());
	const auto menuItem = Gtk::manage (new Gtk::ImageMenuItem ());

	menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &ProjectEditor::on_open_with_activate), appInfo, file));

	image -> set (Glib::RefPtr <const Gio::Icon> (appInfo -> get_icon ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	menuItem -> set_image (*image);
	menuItem -> set_always_show_image (true);
	menuItem -> set_label (appInfo -> get_display_name ());

	getOpenWithMenu () .append (*menuItem);
}

void
ProjectEditor::launchFile (const Glib::RefPtr <Gio::File> & file)
{
	if (canOpenFile (file))
	{
		getBrowserWindow () -> open ("file://" + file -> get_path ());
	}
	else
	{
		const auto appInfo = file -> query_default_handler ();
	
		if (appInfo)
			appInfo -> launch (file);
	}
}

bool
ProjectEditor::canOpenFile (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		const auto contentType = file -> query_info () -> get_content_type ();
		const auto appInfos    = Gio::AppInfo::get_all_for_type (contentType);
	
		for (const auto & appInfo : appInfos)
		{
			if (appInfo -> get_executable () == "titania")
				return true;
		}
	
		for (const auto & goldenType : X3D::GoldenGate::getContentTypes ())
		{
			if (Gio::content_type_is_a (contentType, goldenType))
				return true;
		}
	
		return false;
	}
	catch (const Glib::Error & error)
	{
		return false;
	}
}

std::string
ProjectEditor::getFileName (const std::string & filename, const std::string & extension) const
{
	static const std::regex pattern (R"/(\.(?:x3d|x3dv|wrl|x3dj|x3dz|x3dvz|wrz)$)/");

	return std::regex_replace (filename, pattern, "") + extension;
}

std::string
ProjectEditor::getExtension (const int32_t type) const
{
	switch (type)
	{
		default:
		case 0:
			return ".x3d";
		case 1:
			return ".x3dv";
		case 2:
			return ".x3dj";
		case 3:
			return ".wrl";
		case 4:
			return ".x3dz";
		case 5:
			return ".x3dvz";
		case 6:
			return ".wrz";
	}
}

Gdk::Rectangle
ProjectEditor::getRectangle (const Gtk::TreePath & path) const
{
	Gdk::Rectangle rectangle;

	getFileView () .get_cell_area (path, *getFileColumn () .operator -> (), rectangle);

	return rectangle;
}

void
ProjectEditor::store ()
{
	if (not removed)
		saveExpanded ();

	X3DFileBrowser <X3DProjectEditorInterface>::store ();
	X3DProjectEditorInterface::store ();
}

ProjectEditor::~ProjectEditor ()
{
	dispose ();
}

} // puck
} // titania
