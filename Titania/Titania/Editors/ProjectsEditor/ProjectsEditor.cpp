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

#include "../../Base/ScrollFreezer.h"
#include "../../Bits/File.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/OpenDirectoryDialog.h"
#include "../../Dialogs/MessageDialog/MessageDialog.h"

#include <regex>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int ICON      = 0;
static constexpr int NAME      = 1;
static constexpr int PATH      = 2;
static constexpr int SENSITIVE = 3;

};

class ProjectsEditor::FolderElement
{
public:

	Glib::RefPtr <Gio::FileMonitor> fileMonitor;
};

ProjectsEditor::ProjectsEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DProjectsEditorInterface (get_ui ("Editors/ProjectsEditor.glade")),
	                  projects (),
	                   folders (),
	                 clipboard (),
	             scrollFreezer (new ScrollFreezer (getTreeView ())),
	                  changing (false)
{
	getTreeView () .signal_display_menu () .connect (sigc::mem_fun (this, &ProjectsEditor::on_display_menu));

	getCutItemMenuItem ()         .add_accelerator ("activate", getAccelGroup (), GDK_KEY_X, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getCopyItemMenuItem ()        .add_accelerator ("activate", getAccelGroup (), GDK_KEY_C, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);
	getPasteIntoFolderMenuItem () .add_accelerator ("activate", getAccelGroup (), GDK_KEY_V, Gdk::CONTROL_MASK, (Gtk::AccelFlags) 0);

	setup ();
}

void
ProjectsEditor::initialize ()
{
	X3DProjectsEditorInterface::initialize ();
}

void
ProjectsEditor::configure ()
{
	X3DProjectsEditorInterface::configure ();

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
	const auto openDirectoryDialog = std::dynamic_pointer_cast <OpenDirectoryDialog> (createDialog ("OpenDirectoryDialog"));

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

		getAddItemMenuItem ()    .set_visible (directory);
		getRenameItemMenuItem () .set_visible (true);
	}
	else
	{
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
	
		if (not basic::uri (file -> get_uri ()) .is_local ())
			on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
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
	
		if (not basic::uri (folder -> get_uri ()) .is_local ())
			on_file_changed (folder, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
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
ProjectsEditor::on_rename_item_activate ()
{
	changing = true;

	const auto path      = getTreeViewSelection () -> get_selected_rows () .front ();
	const auto iter      = getTreeStore () -> get_iter (path);
	const auto file      = Gio::File::create_for_path (getPath (iter));
	const auto fileInfo  = file -> query_info ();
	const auto directory = fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
	const auto basename  = Glib::ustring (basic::uri (file -> get_basename ()) .basename (directory));

	getRenameItemLabel () .set_text (directory ? _ ("Folder Name") : _ ("File Name"));
	getRenameItemEntry () .set_placeholder_text (directory ? _ ("Folder Name") : _ ("File Name"));

	getRenameItemEntry ()   .set_text (file -> get_basename ());
	getRenameItemEntry ()   .select_region (0, basename .size ());
	getRenameItemButton ()  .set_sensitive (false);
	getRenameItemPopover () .set_pointing_to (getRectangle (path));
	getRenameItemPopover () .popup ();

	// Workaround, Gtk::Entry::select_region does not work.
	gtk_editable_select_region (GTK_EDITABLE (getRenameItemEntry () .gobj ()), 0, basename .size ());

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

		iter -> set_value (Columns::PATH, destination -> get_path ());

		if (not basic::uri (item -> get_uri ()) .is_local ())
			on_file_changed (item, destination, Gio::FILE_MONITOR_EVENT_RENAMED);
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

	bool copy = false;

	for (const auto & path : clipboard)
	{
		try
		{
			const auto iter = getIter (path);
	
			if (not getTreeStore () -> iter_is_valid (iter))
				continue;

			iter -> get_value (Columns::SENSITIVE, copy);
	
			const auto source      = Gio::File::create_for_path (path);
			const auto sourceInfo  = source -> query_info ();
			const auto directory   = sourceInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY;
			const auto destination = copy ? getPasteDestination (folder, source -> get_basename ()) : folder -> get_child (source -> get_basename ());

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

			if (File::isSubfolder (source, destination))
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
				if (directory)
					File::copyFolder (source, destination);
				else
					source -> copy (destination);
			}
			else
			{
				source -> move (destination, Gio::FILE_COPY_OVERWRITE);
			}
		}
		catch (const Glib::Error & error)
		{
			__LOG__ << error .what () << std::endl;
		}
	}

	if (not copy)
		clearClipboard ();
}

Glib::RefPtr <Gio::File>
ProjectsEditor::getPasteDestination (const Glib::RefPtr <Gio::File> & folder, const basic::uri & basename) const
{
	static const std::regex pattern (_ ("\\s*\\((?:copy|another copy|\\d+\\.\\s+copy)\\)\\s*$"));

	auto    name   = std::regex_replace (basename .basename (false), pattern, "");
	auto    suffix = basename .suffix ();
	int32_t copy   = 0;
	auto    child  = Glib::RefPtr <Gio::File> ();

	do
	{
		child = folder -> get_child (name + getPasteCopyString (copy) + suffix);
		copy += 1;
	}
	while (child -> query_exists ());

	return child;
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
}

void
ProjectsEditor::on_move_to_trash_activate (const Glib::RefPtr <Gio::File> & file)
{
	try
	{
		if (projects .count (file -> get_path ()))
			removeRootFolder (getIter (file -> get_path ()));

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

		if (not basic::uri (file -> get_uri ()) .is_local ())
			on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_DELETED);
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

bool
ProjectsEditor::on_test_expand_row (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	// Refresh children.

	removeChildren (iter);
	addChildren (iter, Gio::File::create_for_path (getPath (iter)));

	// Return false to allow expansion, true to reject.
	return false;
}

void
ProjectsEditor::on_file_changed (const Glib::RefPtr <Gio::File> & file,
                                 const Glib::RefPtr <Gio::File> & other_file,
                                 Gio::FileMonitorEvent event)
{
	try
	{
		switch (event)
		{
			case Gio::FILE_MONITOR_EVENT_DELETED:
			case Gio::FILE_MONITOR_EVENT_CREATED:
			case Gio::FILE_MONITOR_EVENT_MOVED:
			case Gio::FILE_MONITOR_EVENT_RENAMED:
			case Gio::FILE_MONITOR_EVENT_MOVED_IN:
			case Gio::FILE_MONITOR_EVENT_MOVED_OUT:
			{
				// Update tree view
				break;
			}
			case Gio::FILE_MONITOR_EVENT_CHANGED:
			case Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
			case Gio::FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
			case Gio::FILE_MONITOR_EVENT_PRE_UNMOUNT:
			case Gio::FILE_MONITOR_EVENT_UNMOUNTED:
			{
				// Do nothing.
				return;
			}
		}

		if (not file -> has_parent ())
			return;

		const auto folder = file -> get_parent ();
		const auto iter   = getIter (folder -> get_path ());
	
		if (not getTreeStore () -> iter_is_valid (iter))
			return;

		// Work with path as iter don't remain valid after append and erase.
		const auto path     = getTreeStore () -> get_path (iter);
		const auto selected = other_file and isSelected (other_file);

		saveExpanded ();
		getTreeStore () -> insert_after (iter);
		removeChild (iter);
		addFolder (getTreeStore () -> get_iter (path), folder);
		restoreExpanded ();

		if (selected)
		{
			unselectAll ();
			selectFile (other_file);
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
ProjectsEditor::unselectAll ()
{
	getTreeViewSelection () -> unselect_all ();
}

bool
ProjectsEditor::selectFile (const Glib::RefPtr <Gio::File> & file, const bool scroll)
{
	using ScrollToRow = void (Gtk::TreeView::*) (const Gtk::TreePath &, float);

	if (not expandTo (file))
		return false;

	const auto iter = getIter (file -> get_path ());
	const auto path = getTreeStore () -> get_path (iter);

	getTreeViewSelection () -> select (iter);
	getTreeView () .set_cursor (path);

	if (scroll)
		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (getTreeView (), (ScrollToRow) &Gtk::TreeView::scroll_to_row), path, 2 - math::phi <double>));

	return true;
}

bool
ProjectsEditor::isSelected (const Glib::RefPtr <Gio::File> & file) const
{
	for (const auto & path : getTreeViewSelection () -> get_selected_rows ())
	{
		const auto iter = getTreeStore () -> get_iter (path);

		if (getPath (iter) == file -> get_path ())
			return true;
	}

	return false;
}

bool
ProjectsEditor::expandTo (const Glib::RefPtr <Gio::File> & file)
{
	if (projects .count (file -> get_path ()))
		return true;

	if (not file -> has_parent ())
		return false;

	const auto folder = file -> get_parent ();

	if (expandTo (folder))
	{
		getTreeView () .expand_row (getTreeStore () -> get_path (getIter (folder -> get_path ())), false);
		return true;
	}

	return false;
}

void
ProjectsEditor::addRootFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		const auto fileInfo = folder -> query_info ();

		if (fileInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
			return;

		if (not projects .emplace (folder -> get_path ()) .second)
			return;

		addFolder (getTreeStore () -> append (), folder);
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectsEditor::addFolder (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & folder)
{
	addFolder (folder);
	addChild (iter, folder, "gtk-directory");

	if (File::hasChildren (folder))
		getTreeStore () -> append (iter -> children ());
}

void
ProjectsEditor::addFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		const auto fileElement = std::make_shared <FolderElement> ();
	
		folders .emplace (folder -> get_path (), fileElement);
	
		fileElement -> fileMonitor = folder -> monitor_directory (Gio::FILE_MONITOR_WATCH_MOVES);
	
		fileElement -> fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &ProjectsEditor::on_file_changed));
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectsEditor::addChildren (const Gtk::TreeIter & parentIter, const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		for (const auto & fileInfo : File::getChildren (folder))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getTreeStore () -> append (parentIter -> children ());

					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getTreeStore () -> append (parentIter -> children ());

					addChild (iter, child, "gtk-file");
					continue;
				}
				default:
					continue;
			}
		}
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ProjectsEditor::addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon)
{
	const auto url = basic::uri (file -> get_uri ());

	iter -> set_value (Columns::ICON,      File::getIconName (file -> query_info (), "gtk-file"));
	iter -> set_value (Columns::PATH,      file -> get_path ());
	iter -> set_value (Columns::SENSITIVE, true);

	if (url .is_local () or not projects .count (file -> get_path ()))
		iter -> set_value (Columns::NAME, file -> get_basename ());
	else
		iter -> set_value (Columns::NAME, file -> get_basename () + " (" + url .authority () + ")");
}

void
ProjectsEditor::removeRootFolder (const Gtk::TreeIter & iter)
{
	// Remove project.

	projects .erase (getPath (iter));

	removeChild (iter);
}

void
ProjectsEditor::removeChildren (const Gtk::TreeIter & iter)
{
	std::vector <Gtk::TreePath> children;

	for (const auto & child : iter -> children ())
		children .emplace_back (getTreeStore () -> get_path (child));

	for (const auto & child : basic::make_reverse_range (children))
		removeChild (getTreeStore () -> get_iter (child));
}

void
ProjectsEditor::removeChild (const Gtk::TreeIter & iter)
{
	const auto path = getPath (iter);

	getTreeStore () -> erase (iter);

	if (path .empty ())
		return;

	// Remove project and subfolders if path is a folder.

	const auto child    = Gio::File::create_for_path (path);
	const auto fileInfo = child -> query_info ();

	if (fileInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
		return;

	// Remove subfolders.

	std::vector <std::string> subfolders;

	for (const auto & pair : folders)
	{
		if (File::isSubfolder (Gio::File::create_for_path (path), Gio::File::create_for_path (pair .first)))
			subfolders .emplace_back (pair .first);
	}

	for (const auto & subfolder : subfolders)
		folders .erase (subfolder);
}

Gtk::TreeIter
ProjectsEditor::getIter (const std::string & path) const
{
	Gtk::TreeIter result;

	for (const auto & childIter : getTreeStore () -> children ())
	{
		if (getIter (childIter, path, result))
			break;
	}

	return result;
}

bool
ProjectsEditor::getIter (const Gtk::TreeIter & iter, const std::string & path, Gtk::TreeIter & result) const
{
	const auto parent = getPath (iter);

	if (parent == path)
	{
		result = iter;
		return true;
	}

	if (path .find (parent) not_eq 0)
		return false;

	for (const auto & childIter : iter -> children ())
	{
		if (getIter (childIter, path, result))
			return true;
	}

	return false;
}

std::string
ProjectsEditor::getPath (const Gtk::TreeIter & iter) const
{
	std::string path;

	iter -> get_value (Columns::PATH, path);

	return path;
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
	const auto file        = Gio::File::create_for_path (path);
	const auto contentType = file -> query_info () -> get_content_type ();
	const auto appInfos    = Gio::AppInfo::get_all_for_type (contentType);

	for (const auto & appInfo : appInfos)
	{
		if (appInfo -> get_executable () == "titania")
		{
			getBrowserWindow () -> open ("file://" + path);
			return;
		}
	}

	const auto appInfo = file -> query_default_handler ();

	if (appInfo)
		appInfo -> launch (file);
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
ProjectsEditor::restoreExpanded ()
{
	const auto folders = getConfig () -> getItem <X3D::MFString> ("expanded");

	for (const auto & folder : folders)
	{
		const auto iter = getIter (folder);

		if (not getTreeStore () -> iter_is_valid (iter))
			continue;

		getTreeView () .expand_row (getTreeStore () -> get_path (iter), false);
	}

	scrollFreezer -> restore (getConfig () -> getItem <double> ("hadjustment"), getConfig () -> getItem <double> ("vadjustment"));
}

void
ProjectsEditor::saveExpanded ()
{
	X3D::MFString folders;

	getExpanded (getTreeStore () -> children (), folders);

	getConfig () -> setItem <X3D::MFString> ("expanded", folders);
	getConfig () -> setItem <double> ("hadjustment", getTreeView () .get_hadjustment () -> get_value ());
	getConfig () -> setItem <double> ("vadjustment", getTreeView () .get_vadjustment () -> get_value ());
}

void
ProjectsEditor::getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders)
{
	for (const auto & child : children)
	{
		if (getTreeView () .row_expanded (getTreeStore () -> get_path (child)))
			folders .emplace_back (getPath (child));

		getExpanded (child -> children (), folders);
	}
}

void
ProjectsEditor::store ()
{
	saveExpanded ();

	getConfig () -> setItem <X3D::MFString> ("projects", X3D::MFString (projects .begin (), projects .end ()));

	X3DProjectsEditorInterface::store ();
}

ProjectsEditor::~ProjectsEditor ()
{
	dispose ();
}

} // puck
} // titania
