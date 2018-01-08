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

static constexpr int ICON = 0;
static constexpr int NAME = 1;
static constexpr int PATH  = 2;

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
	             scrollFreezer (new ScrollFreezer (getTreeView ()))
{
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

bool
ProjectsEditor::on_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

		if (selectedRows .empty ())
			return true;

		if (selectedRows .size () == 1)
		{
			const auto path     = selectedRows .front ();
			const auto file     = Gio::File::create_for_path (getPath (getTreeStore () -> get_iter (path)));
			const auto fileInfo = file -> query_info ();

			createOpenWithMenu (file);

			// Show hide menu items.
	
			getAddMenuItem ()        .set_visible (fileInfo -> get_file_type () == Gio::FILE_TYPE_DIRECTORY);
			getRenameItemMenuItem () .set_visible (true);
		}
		else
		{
			getOpenWithMenuItem ()      .set_visible (false);
			getAddMenuItem ()           .set_visible (false);
			getFileSeparatorMenuItem () .set_visible (false);
			getRenameItemMenuItem ()    .set_visible (false);
		}

		// Show context menu.
	
		getContextMenu () .popup (event -> button, event -> time);
		return true;
	}

	return false;
}

void
ProjectsEditor::on_open_with_activate (const Glib::RefPtr <Gio::AppInfo> & appInfo, const Glib::RefPtr <Gio::File> & file)
{
	appInfo -> launch (file);
}

void
ProjectsEditor::on_add_new_file_activate ()
{
	getCreateFileEntry ()      .set_text ("");
	getCreateFileTypeButton () .set_active (getConfig () -> getItem <int32_t> ("fileType", 0));
	getCreateFilePopover ()    .set_pointing_to (getRectangle (getTreeViewSelection () -> get_selected_rows () .front ()));
	getCreateFilePopover ()    .popup ();
}

void
ProjectsEditor::on_create_file_clicked ()
{
	try
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
		const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
		const auto suffix       = getSuffix (getCreateFileTypeButton () .get_active_row_number ());
		const auto parent       = Gio::File::create_for_path (getPath (iter));
		const auto file         = parent -> get_child (getFileName (getCreateFileEntry () .get_text () .raw (), suffix));

		if (getCreateFileEntry () .get_text () .empty ())
		{
			getWidget () .error_bell ();
			return;
		}
	
		if (file -> query_exists ())
		{
			getWidget () .error_bell ();
			return;
		}

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
	catch (...)
	{ }
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

void
ProjectsEditor::on_add_new_folder_activate ()
{
	getCreateFolderEntry ()   .set_text ("");
	getCreateFolderPopover () .set_pointing_to (getRectangle (getTreeViewSelection () -> get_selected_rows () .front ()));
	getCreateFolderPopover () .popup ();
}

void
ProjectsEditor::on_create_folder_clicked ()
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
	const auto parent       = Gio::File::create_for_path (getPath (iter));
	const auto folder       = parent -> get_child (getCreateFolderEntry () .get_text ());

	if (getCreateFolderEntry () .get_text () .empty ())
	{
		getWidget () .error_bell ();
		return;
	}

	if (folder -> query_exists ())
	{
		getWidget () .error_bell ();
		return;
	}

	// Create folder.

	getCreateFolderPopover () .popdown ();

	folder -> make_directory_with_parents ();

	if (not basic::uri (folder -> get_uri ()) .is_local ())
		on_file_changed (folder, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_CREATED);
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

void
ProjectsEditor::on_rename_item_activate ()
{
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
	getRenameItemPopover () .set_pointing_to (getRectangle (path));
	getRenameItemPopover () .popup ();

	// Workaround, Gtk::Entry::select_region does not work.
	gtk_editable_select_region (GTK_EDITABLE (getRenameItemEntry () .gobj ()), 0, basename .size ());
}

void
ProjectsEditor::on_rename_item_clicked ()
{
	try
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
		const auto iter         = getTreeStore () -> get_iter (selectedRows .front ());
		const auto item         = Gio::File::create_for_path (getPath (iter));
		const auto parent       = item -> get_parent ();
		const auto destination  = parent -> get_child (getRenameItemEntry () .get_text ());

		if (getRenameItemEntry () .get_text () .empty ())
		{
			getWidget () .error_bell ();
			return;
		}
	
		if (destination -> query_exists ())
		{
			getWidget () .error_bell ();
			return;
		}

		// Create X3D file.

		getRenameItemPopover () .popdown ();

		item -> move (destination);

		iter -> set_value (Columns::PATH, destination -> get_path ());

		if (not basic::uri (item -> get_uri ()) .is_local ())
			on_file_changed (item, destination, Gio::FILE_MONITOR_EVENT_RENAMED);
	}
	catch (const Gio::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (...)
	{ }
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

void
ProjectsEditor::on_move_to_trash_activate ()
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

	for (const auto & path : selectedRows)
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
		file -> trash ();
	}
	catch (const Gio::Error & error)
	{
		__LOG__ << error .what () << std::endl;

		if (error .code () == Gio::Error::NOT_SUPPORTED)
			on_remove_file_activate (file);
	}
	catch (...)
	{ }
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

		file -> remove ();

		if (not basic::uri (file -> get_uri ()) .is_local ())
			on_file_changed (file, Glib::RefPtr <Gio::File> (), Gio::FILE_MONITOR_EVENT_DELETED);
	}
	catch (const Gio::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (...)
	{ }
}

void
ProjectsEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	launchFile (getPath (getTreeStore () -> get_iter (path)));
}

void
ProjectsEditor::on_selection_changed ()
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

	switch (selectedRows .size ())
	{
		case 1:
		{
			getRemoveProjectButton () .set_sensitive (selectedRows .front () .size () == 1);
			break;
		}
		case 0:
		default:
		{
			getRemoveProjectButton () .set_sensitive (false);
			break;
		}
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
	catch (const Gio::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
	catch (...)
	{ }
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
	catch (...)
	{ }
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
	catch (...)
	{ }
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
	catch (...)
	{ }
}

void
ProjectsEditor::addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon)
{
	const auto url = basic::uri (file -> get_uri ());

	iter -> set_value (Columns::ICON, File::getIconName (file -> query_info (), "gtk-file"));
	iter -> set_value (Columns::PATH, file -> get_path ());

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
		if (pair .first .find (path) == 0)
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
