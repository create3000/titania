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

#include "ProjectsEditor.h"

#include "../../Bits/File.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"
#include "../../Dialogs/FileOpenDialog/OpenFolderDialog.h"

#include <Titania/X3D/InputOutput/GoldenGate.h>
#include <regex>

namespace titania {
namespace puck {

ProjectsEditor::ProjectsEditor (X3DBrowserWindow* const browserWindow) :
	                           X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	                 X3DProjectsEditorInterface (get_ui ("Editors/ProjectsEditor.glade")),
	X3DFileBrowser <X3DProjectsEditorInterface> (),
	                                  clipboard (),
	                                   changing (false)
{
	getTreeView () .signal_display_menu () .connect (sigc::mem_fun (this, &ProjectsEditor::on_display_menu));

	getCutItemMenuItem ()         .add_accelerator ("activate", getAccelGroup (), GDK_KEY_X, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getCopyItemMenuItem ()        .add_accelerator ("activate", getAccelGroup (), GDK_KEY_C, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getPasteIntoFolderMenuItem () .add_accelerator ("activate", getAccelGroup (), GDK_KEY_V, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	setTitleBar (getAddFilesDialog (), getAddFilesHeaderBar ());

	setup ();
}

void
ProjectsEditor::initialize ()
{
	X3DProjectsEditorInterface::initialize ();
	X3DFileBrowser <X3DProjectsEditorInterface>::initialize ();
}

void
ProjectsEditor::configure ()
{
	X3DProjectsEditorInterface::configure ();
	X3DFileBrowser <X3DProjectsEditorInterface>::configure ();

	const auto projects = getConfig () -> getItem <X3D::MFString> ("projects");

	for (const auto & folder : projects)
		addRootFolder (Gio::File::create_for_path (folder));

	restoreExpanded ();

	set_execution_context ();
}

void
ProjectsEditor::on_map ()
{
	getCurrentContext () .addInterest (&ProjectsEditor::set_execution_context, this);

	set_execution_context ();
}

void
ProjectsEditor::on_unmap ()
{
	getCurrentContext () .removeInterest (&ProjectsEditor::set_execution_context, this);
}

bool
ProjectsEditor::on_focus_in_event (GdkEventFocus* focus_event)
{
	getBrowserWindow () -> setAccelerators (false);
	getBrowserWindow () -> getWindow () .add_accel_group (getAccelGroup ());
	return false;
}

bool
ProjectsEditor::on_focus_out_event (GdkEventFocus* focus_event)
{
	getBrowserWindow () -> getWindow () .remove_accel_group (getAccelGroup ());
	getBrowserWindow () -> setAccelerators (true);
	return false;
}

void
ProjectsEditor::on_add_project_clicked ()
{
	const auto openDirectoryDialog = std::dynamic_pointer_cast <OpenFolderDialog> (createDialog ("OpenFolderDialog"));

	if (not openDirectoryDialog -> run ())
		return;

	// Create root folder.

	addRootFolder (Gio::File::create_for_path (openDirectoryDialog -> getUrl () .path ()));
}

void
ProjectsEditor::on_remove_project_clicked ()
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());

	removeRootFolder (iter);
}

void
ProjectsEditor::on_display_menu (GdkEventButton* event)
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

	if (selectedRows .empty ())
		return;

	if (selectedRows .size () == 1)
	{
		const auto path      = selectedRows .front ();
		const auto file      = Gio::File::create_for_path (getPath (getTreeStore () -> get_iter (path)));
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
ProjectsEditor::on_open_with_activate (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file)
{
	appInfo -> launch (file);
}

void
ProjectsEditor::on_import_activate ()
{
	try
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
		const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
		const auto file         = Gio::File::create_for_path (getPath (iter));
		const auto undoStep     = std::make_shared <X3D::UndoStep> (basic::sprintf (_ ("Import »%s« From Project"), file -> get_basename () .c_str ()));
		const auto nodes        = getBrowserWindow () -> import ({ file -> get_uri () }, undoStep);

		getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
		getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectsEditor::on_add_new_file_activate ()
{
	changing = true;

	getCreateFileTypeButton () .set_active (getConfig () -> getItem <int32_t> ("fileType", 0));
	getCreateFileEntry ()      .set_text ("");
	getCreateFileButton ()     .set_sensitive (false);
	getCreateFilePopover ()    .set_pointing_to (getRectangle (getTreeViewSelection () -> get_selected_rows () .front ()));
	getCreateFilePopover ()    .popup ();
	getCreateFileEntry ()      .grab_focus ();

	changing = false;
}

void
ProjectsEditor::on_create_file_changed ()
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
ProjectsEditor::on_create_file_clicked ()
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
ProjectsEditor::on_create_file_key_press_event (GdkEventKey* event)
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
ProjectsEditor::getNewFile () const
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
	const auto suffix       = getSuffix (getCreateFileTypeButton () .get_active_row_number ());
	const auto parent       = Gio::File::create_for_path (getPath (iter));
	const auto file         = parent -> get_child (getFileName (getCreateFileEntry () .get_text () .raw (), suffix));

	if (getCreateFileEntry () .get_text () .empty ())
		throw std::invalid_argument ("getNewFile");

	if (file -> query_exists ())
		throw std::invalid_argument ("getNewFile");

	return file;
}

void
ProjectsEditor::on_add_new_folder_activate ()
{
	changing = true;

	getCreateFolderEntry ()   .set_text ("");
	getCreateFolderButton ()  .set_sensitive (false);
	getCreateFolderPopover () .set_pointing_to (getRectangle (getTreeViewSelection () -> get_selected_rows () .front ()));
	getCreateFolderPopover () .popup ();

	changing = false;
}

void
ProjectsEditor::on_create_folder_changed ()
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
ProjectsEditor::on_create_folder_clicked ()
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
ProjectsEditor::on_create_folder_key_press_event (GdkEventKey* event)
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
ProjectsEditor::getNewFolder () const
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
	const auto parent       = Gio::File::create_for_path (getPath (iter));
	const auto folder       = parent -> get_child (getCreateFolderEntry () .get_text ());

	if (getCreateFolderEntry () .get_text () .empty ())
		throw std::invalid_argument ("getNewFolder");

	if (folder -> query_exists ())
		throw std::invalid_argument ("getNewFolder");

	return folder;
}

void
ProjectsEditor::on_add_existing_folder_activate ()
{
	try
	{
		const auto dialog = std::dynamic_pointer_cast <OpenFolderDialog> (createDialog ("OpenFolderDialog"));
	
		if (not dialog -> run ())
			return;
	
		const auto response = getAddFilesDialog () .run ();
	
		getAddFilesDialog () .hide ();
	
		if (response not_eq Gtk::RESPONSE_OK)
			return;
	
		const auto selectedRows   = getTreeViewSelection () -> get_selected_rows ();
		const auto iter           = getTreeStore () -> get_iter (selectedRows .front ());
		const auto folder         = Gio::File::create_for_path (getPath (iter));
		const auto source         = dialog -> getWindow () .get_file ();
		const auto destination    = folder -> get_child (source -> get_basename ());
		const auto sourceUri      = basic::uri (source -> get_uri ());
		const auto destinationUri = basic::uri (destination -> get_uri ());
	
		if (destination -> query_exists ())
		{
			const auto dialog = std::dynamic_pointer_cast <MessageDialog> (createDialog ("MessageDialog"));
	
			dialog -> setType (Gtk::MESSAGE_QUESTION);
			dialog -> setMessage (basic::sprintf (_ ("Replace directory »%s«?"), destination -> get_basename () .c_str ()));
			dialog -> setText (basic::sprintf (_ ("A directory with the same name already exists in »%s«. Replacing it will overwrite its content."), folder -> get_basename () .c_str ()));
	
			if (dialog -> run () not_eq Gtk::RESPONSE_OK)
				return;
	
			File::removeFile (destination);
		}
	
		if (getCopyFilesButton () .get_active ())
		{
			auto flags = Gio::FILE_COPY_OVERWRITE;
		
			if (sourceUri .is_local () and destinationUri .is_local ())
				flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;
	
			File::copyFile (source, destination, flags);
	
			on_file_changed (destination, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
		}
		else if (getMoveFilesButton () .get_active ())
		{
			try
			{
				source -> move (destination, Gio::FILE_COPY_OVERWRITE | Gio::FILE_COPY_NOFOLLOW_SYMLINKS);
			}
			catch (const Gio::Error & error)
			{
				auto flags = Gio::FILE_COPY_OVERWRITE;
	
				if (sourceUri .is_local () and destinationUri .is_local ())
					flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;
	
				File::copyFile (source, destination, flags);
				File::removeFile (source);
			}
	
			on_file_changed (source, destination, Gio::FILE_MONITOR_EVENT_MOVED);
		}
		else if (getLinkFilesButton () .get_active ())
		{
			destination -> make_symbolic_link (source -> get_path ());
		}

		unselectAll ();
		selectFile (destination);
	}
	catch (const Gio::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectsEditor::on_rename_item_activate ()
{
	changing = true;

	const auto path      = getTreeViewSelection () -> get_selected_rows () .front ();
	const auto iter      = getTreeStore () -> get_iter (path);
	const auto file      = Gio::File::create_for_path (getPath (iter));
	const auto fileInfo  = file -> query_info ();
	const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
	const auto basename  = basic::uri (file -> get_basename ());
	const auto name      = Glib::ustring (directory ? basename .basename () : basename .name ());

	getRenameItemLabel () .set_text (directory ? _ ("Folder Name") : _ ("File Name"));
	getRenameItemEntry () .set_placeholder_text (directory ? _ ("Folder Name") : _ ("File Name"));

	getRenameItemEntry ()   .set_text (file -> get_basename ());
	getRenameItemEntry ()   .select_region (0, name .size ());
	getRenameItemButton ()  .set_sensitive (false);
	getRenameItemPopover () .set_pointing_to (getRectangle (path));
	getRenameItemPopover () .popup ();

	// Workaround, Gtk::Entry::select_region does not work.
	gtk_editable_select_region (GTK_EDITABLE (getRenameItemEntry () .gobj ()), 0, name .size ());

	changing = false;
}

void
ProjectsEditor::on_rename_item_changed ()
{
	try
	{
		if (changing)
			return;

		getRenameItem ();

		getRenameItemButton () .set_sensitive (true);
	}
	catch (...)
	{
		getWidget () .error_bell ();
		getRenameItemButton () .set_sensitive (false);
	}
}

void
ProjectsEditor::on_rename_item_clicked ()
{
	try
	{
		const auto tuple       = getRenameItem ();
		const auto iter        = std::get <0> (tuple);
		const auto item        = std::get <1> (tuple);
		const auto destination = std::get <2> (tuple);

		// Create X3D file.

		getRenameItemPopover () .popdown ();

		item -> move (destination);

		setPath (iter, destination -> get_path ());

		on_file_changed (item, destination, Gio::FILE_MONITOR_EVENT_RENAMED);

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
ProjectsEditor::on_rename_item_key_press_event (GdkEventKey* event)
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

std::tuple <Gtk::TreeIter, Glib::RefPtr <Gio::File>, Glib::RefPtr <Gio::File>>
ProjectsEditor::getRenameItem () const
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
	const auto item         = Gio::File::create_for_path (getPath (iter));
	const auto parent       = item -> get_parent ();
	const auto destination  = parent -> get_child (getRenameItemEntry () .get_text ());

	if (getRenameItemEntry () .get_text () .empty ())
		throw std::invalid_argument ("getRenameItem");

	if (destination -> query_exists ())
		throw std::invalid_argument ("getRenameItem");

	return std::make_tuple (iter, item, destination);
}

void
ProjectsEditor::on_cut_item_activate ()
{
	cutItems (getTreeViewSelection () -> get_selected_rows ());
}

void
ProjectsEditor::on_copy_item_activate ()
{
	copyItems (getTreeViewSelection () -> get_selected_rows ());
}

void
ProjectsEditor::on_paste_into_folder_activate ()
{
	pasteIntoFolder (getTreeViewSelection () -> get_selected_rows () .front ());
}

void
ProjectsEditor::clearClipboard ()
{
	for (const auto & path : clipboard)
	{
		const auto & iter = getIter (path);

		if (not getTreeStore () -> iter_is_valid (iter))
			continue;

		iter -> set_value (Columns::SENSITIVE, true);
	}

	clipboard .clear ();
}

void
ProjectsEditor::cutItems (const std::vector <Gtk::TreePath> & rows)
{
	clearClipboard ();

	for (const auto & path : rows)
	{
		const auto iter = getTreeStore () -> get_iter (path);

		iter -> set_value (Columns::SENSITIVE, false);

		clipboard .emplace_back (getPath (iter));
	}
}

void
ProjectsEditor::copyItems (const std::vector <Gtk::TreePath> & rows)
{
	clearClipboard ();

	for (const auto & path : getTreeViewSelection () -> get_selected_rows ())
	{
		const auto iter = getTreeStore () -> get_iter (path);

		clipboard .emplace_back (getPath (iter));
	}
}

void
ProjectsEditor::pasteIntoFolder (const Gtk::TreePath & row)
{
	const auto iter       = getTreeStore () -> get_iter (row);
	const auto folder     = Gio::File::create_for_path (getPath (iter));
	const auto folderInfo = folder -> query_info ();

	if (folderInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
		return;

	auto selection = std::vector <Glib::RefPtr <Gio::File>> ();

	bool copy = false;

	for (const auto & path : clipboard)
	{
		try
		{
			const auto iter = getIter (path);
	
			if (not getTreeStore () -> iter_is_valid (iter))
				continue;

			iter -> get_value (Columns::SENSITIVE, copy);
	
			const auto source         = Gio::File::create_for_path (path);
			const auto sourceInfo     = source -> query_info ();
			const auto directory      = sourceInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
			const auto destination    = getPasteDestination (copy, source, folder);
			const auto sourceUri      = basic::uri (source -> get_uri ());
			const auto destinationUri = basic::uri (destination -> get_uri ());

			if (destination -> query_exists ())
			{
				const auto dialog = std::dynamic_pointer_cast <MessageDialog> (createDialog ("MessageDialog"));
		
				dialog -> setType (Gtk::MESSAGE_QUESTION);

				if (directory)
				{
					dialog -> setMessage (basic::sprintf (_ ("Replace directory »%s«?"), destination -> get_basename () .c_str ()));
					dialog -> setText (basic::sprintf (_ ("A directory with the same name already exists in »%s«. Replacing it will overwrite its content."), folder -> get_basename () .c_str ()));
				}
				else
				{
					dialog -> setMessage (basic::sprintf (_ ("Replace file »%s«?"), destination -> get_basename () .c_str ()));
					dialog -> setText (basic::sprintf (_ ("Another file with the same name already exists in »%s«. Replacing it will overwrite its content."), folder -> get_basename () .c_str ()));
				}

				if (dialog -> run () not_eq Gtk::RESPONSE_OK)
					continue;

				File::removeFile (destination);
			}

			if (not copy)
			{
				if (source -> get_parent () -> get_uri () == destination -> get_parent () -> get_uri ())
					return;
			}

			if (File::isSubfolder (destination, source))
			{
				const auto dialog = std::dynamic_pointer_cast <MessageDialog> (createDialog ("MessageDialog"));
		
				dialog -> setType (Gtk::MESSAGE_ERROR);
				dialog -> setMessage (_ ("You cannot copy a folder into itself!"));
				dialog -> setText (_ ("The destination folder is inside the source folder."));
				dialog -> run ();

				continue;
			}

			if (copy)
			{
				auto flags = Gio::FILE_COPY_OVERWRITE;

				if (sourceUri .is_local () and destinationUri .is_local ())
					flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;

				File::copyFile (source, destination, flags);

				on_file_changed (destination, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
			}
			else
			{
				try
				{
					source -> move (destination, Gio::FILE_COPY_OVERWRITE | Gio::FILE_COPY_NOFOLLOW_SYMLINKS);
				}
				catch (const Gio::Error & error)
				{
					auto flags = Gio::FILE_COPY_OVERWRITE;

					if (sourceUri .is_local () and destinationUri .is_local ())
						flags |= Gio::FILE_COPY_NOFOLLOW_SYMLINKS;

					File::copyFile (source, destination, flags);
					File::removeFile (source);
				}

				on_file_changed (source, destination, Gio::FILE_MONITOR_EVENT_MOVED);
			}

			selection .emplace_back (destination);
		}
		catch (const Glib::Error & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	if (not copy)
		clearClipboard ();

	// Select destinations.

	unselectAll ();

	for (const auto & file : selection)
		selectFile (file);
}

Glib::RefPtr <Gio::File>
ProjectsEditor::getPasteDestination (const bool copy, const Glib::RefPtr <Gio::File> & source, const Glib::RefPtr <Gio::File> & folder) const
{
	static const std::regex pattern (_ ("\\s*\\((?:copy|another copy|\\d+\\.\\s+copy)\\)\\s*$"));

	if (copy and source -> get_parent () -> get_uri () == folder -> get_uri ())
	{
		auto    basename = basic::uri (source -> get_basename ());
		auto    name     = std::regex_replace (basename .name (), pattern, "");
		auto    suffix   = basename .suffix ();
		int32_t copy     = 0;
		auto    child    = Glib::RefPtr <Gio::File> ();
	
		do
		{
			child = folder -> get_child (name + getPasteCopyString (copy) + suffix);
			copy += 1;
		}
		while (child -> query_exists ());
	
		return child;
	}
	else
	{
		return folder -> get_child (source -> get_basename ());
	}
}

std::string
ProjectsEditor::getPasteCopyString (const int32_t count) const
{
	switch (count)
	{
		case 0:
			return "";
		case 1:
			return _ (" (copy)");
		case 2:
			return _ (" (another copy)");
		default:
			return basic::sprintf (_(" (%d. copy)"), count);
	}
}

void
ProjectsEditor::on_move_to_trash_activate ()
{
	for (const auto & path : getTreeViewSelection () -> get_selected_rows ())
	{
		const auto iter = getTreeStore () -> get_iter (path);
		const auto file = Gio::File::create_for_path (getPath (iter));

		on_move_to_trash_activate (file);
	}

	for (const auto & path : getTreeViewSelection () -> get_selected_rows ())
	{
		const auto iter = getTreeStore () -> get_iter (path);
		const auto file = Gio::File::create_for_path (getPath (iter));

		on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_DELETED);
	}

	unselectAll ();
}

void
ProjectsEditor::on_move_to_trash_activate (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		if (getRootFolders () .count (file -> get_path ()))
		{
			const auto dialog = std::dynamic_pointer_cast <MessageDialog> (createDialog ("MessageDialog"));
	
			dialog -> setType (Gtk::MESSAGE_QUESTION);
			dialog -> setMessage ("You are about to remove a project folder to trash!");
			dialog -> setText (basic::sprintf (_ ("Do you realy want to move project »%s« to trash?"), file -> get_basename () .c_str ()));

			if (dialog -> run () not_eq Gtk::RESPONSE_OK)
				return;

			removeRootFolder (getIter (file -> get_path ()));
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
ProjectsEditor::on_remove_file_activate (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		const auto dialog = std::dynamic_pointer_cast <MessageDialog> (createDialog ("MessageDialog"));

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
ProjectsEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	launchFile (getPath (getTreeStore () -> get_iter (path)));
}

void
ProjectsEditor::on_selection_changed ()
{
	try
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

		getCutItemMenuItem ()     .set_sensitive (selectedRows .size ());
		getCopyItemMenuItem ()    .set_sensitive (selectedRows .size ());
		getMoveToTrashMenuItem () .set_sensitive (selectedRows .size ());

		switch (selectedRows .size ())
		{
			case 1:
			{
				const auto iter      = getTreeStore () -> get_iter (selectedRows .front ());
				const auto file      = Gio::File::create_for_path (getPath (iter));
				const auto fileInfo  = file -> query_info ();
				const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
	
				getTreeView () .set_button3_select (true);

				getRemoveProjectButton ()     .set_sensitive (selectedRows .front () .size () == 1);
				getOpenWithMenuItem ()        .set_sensitive (true);
				getImportMenuItem ()          .set_sensitive (true);
				getAddItemMenuItem ()         .set_sensitive (directory);
				getRenameItemMenuItem ()      .set_sensitive (true);
				getPasteIntoFolderMenuItem () .set_sensitive (directory and clipboard .size ());
				break;
			}
			case 0:
			default:
			{
				getTreeView () .set_button3_select (selectedRows .empty ());
	
				getRemoveProjectButton ()     .set_sensitive (false);
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
ProjectsEditor::set_execution_context ()
{
	if (getCurrentContext () -> getWorldURL () .is_local ())
	{
		unselectAll ();
		selectFile (Gio::File::create_for_path (getCurrentContext () -> getWorldURL () .path ()), true);
	}
}

void
ProjectsEditor::addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file)
{
	X3DFileBrowser <X3DProjectsEditorInterface>::addChild (iter, file);

	iter -> set_value (Columns::SENSITIVE, true);
}

void
ProjectsEditor::createOpenWithMenu (const Glib::RefPtr <Gio::File> & file)
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

void
ProjectsEditor::createOpenWithMenuItem (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file)
{
	const auto image    = Gtk::manage (new Gtk::Image ());
	const auto menuItem = Gtk::manage (new Gtk::ImageMenuItem ());

	menuItem -> signal_activate () .connect (sigc::bind (sigc::mem_fun (this, &ProjectsEditor::on_open_with_activate), appInfo, file));

	image -> set (Glib::RefPtr <const Gio::Icon> (appInfo -> get_icon ()), Gtk::IconSize (Gtk::ICON_SIZE_MENU));
	menuItem -> set_image (*image);
	menuItem -> set_always_show_image (true);
	menuItem -> set_label (appInfo -> get_display_name ());

	getOpenWithMenu () .append (*menuItem);
}

void
ProjectsEditor::launchFile (const std::string & path)
{
	const auto file = Gio::File::create_for_path (path);

	if (canOpenFile (file))
	{
		getBrowserWindow () -> open ("file://" + path);
	}
	else
	{
		const auto appInfo = file -> query_default_handler ();
	
		if (appInfo)
			appInfo -> launch (file);
	}
}

bool
ProjectsEditor::canOpenFile (const Glib::RefPtr <Gio::File> & file)
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

std::string
ProjectsEditor::getFileName (const std::string & filename, const std::string & suffix) const
{
	static const std::regex pattern (R"/(\.(?:x3d|x3dv|wrl|json|x3dz|x3dvz|wrz)$)/");

	return std::regex_replace (filename, pattern, "") + suffix;
}

std::string
ProjectsEditor::getSuffix (const int32_t type) const
{
	switch (type)
	{
		default:
		case 0:
			return ".x3d";
		case 1:
			return ".x3dv";
		case 2:
			return ".wrl";
		case 3:
			return ".json";
		case 4:
			return ".x3dz";
		case 5:
			return ".x3dvz";
		case 6:
			return ".wrz";
	}
}

Gdk::Rectangle
ProjectsEditor::getRectangle (const Gtk::TreePath & path) const
{
	Gdk::Rectangle rectangle;

	getTreeView () .get_cell_area (path, *getFileColumn () .operator -> (), rectangle);

	return rectangle;
}

void
ProjectsEditor::store ()
{
	saveExpanded ();

	getConfig () -> setItem <X3D::MFString> ("projects", X3D::MFString (getRootFolders () .begin (), getRootFolders () .end ()));

	X3DFileBrowser <X3DProjectsEditorInterface>::store ();
	X3DProjectsEditorInterface::store ();
}

ProjectsEditor::~ProjectsEditor ()
{
	dispose ();
}

} // puck
} // titania
