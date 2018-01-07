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
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/OpenDirectoryDialog.h"
#include "../../Editors/LibraryView/X3DLibraryView.h"

namespace titania {
namespace puck {

namespace Columns {

static constexpr int ICON = 0;
static constexpr int NAME = 1;
static constexpr int URL  = 2;

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
	             scrollFreezer (new ScrollFreezer (getProjectsView ()))
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
		addRootFolder (folder .str ());

	restoreExpanded ();
}

void
ProjectsEditor::on_add_project_clicked ()
{
	const auto openDirectoryDialog = std::dynamic_pointer_cast <OpenDirectoryDialog> (createDialog ("OpenDirectoryDialog"));

	if (not openDirectoryDialog -> run ())
		return;

	// Create root folder.

	addRootFolder (openDirectoryDialog -> getUrl ());
}

bool
ProjectsEditor::on_button_press_event (GdkEventButton* event)
{
	if (event -> button == 3)
	{
		const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();

		if (selectedRows .empty ())
			return false;
	
		if (selectedRows .size () == 1)
		{
			const auto path = selectedRows .front ();
			const auto file = Gio::File::create_for_uri (getUrl (getProjectsStore () -> get_iter (path)));

			// Clear »Open With ...« menu.

			for (const auto & widget : getOpenWithMenu () .get_children ())
				getOpenWithMenu () .remove (*widget);

			// Populate »Open With ...« menu.
	
			const auto contentType = file -> query_info () -> get_content_type ();
			const auto appInfos    = Gio::AppInfo::get_all_for_type (contentType);

			for (const auto & appInfo : appInfos)
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

			getOpenWithMenu ()     .show_all ();
			getOpenWithMenuItem () .set_visible (appInfos .size ());
		}
		else
		{
			getOpenWithMenuItem () .set_visible (false);
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
}

void
ProjectsEditor::on_add_new_folder_activate ()
{
}

void
ProjectsEditor::on_remove_project_clicked ()
{
	const auto selectedRows = getTreeViewSelection () -> get_selected_rows ();
	const auto iter         = getProjectsStore () -> get_iter (selectedRows .front ());

	removeChild (iter);
}

void
ProjectsEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	launchUrl (getUrl (getProjectsStore () -> get_iter (path)));
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
	
		const auto directory = file -> get_parent ();
	
		if (not directory)
			return;
	
		const auto iter = getIter (directory -> get_uri ());
	
		if (not getProjectsStore () -> iter_is_valid (iter))
			return;

		const auto newIter = getProjectsStore () -> insert_after (iter);

		saveExpanded ();
		addFolder (newIter, directory);
		removeChild (iter);
		restoreExpanded ();
	}
	catch (...)
	{ }
}

void
ProjectsEditor::addRootFolder (const basic::uri & URL)
{
	try
	{
		const auto directory = Gio::File::create_for_uri (URL);
		const auto fileInfo  = directory -> query_info ();

		if (fileInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
			return;

		if (not projects .emplace (URL) .second)
			return;

		addFolder (getProjectsStore () -> append (), directory);
	}
	catch (...)
	{ }
}

void
ProjectsEditor::addFolder (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & directory)
{
	addFolder (directory);
	addChild (iter, directory, "gtk-directory");
	addChildren (iter, directory);
}

void
ProjectsEditor::addFolder (const Glib::RefPtr <Gio::File> & directory)
{
	const auto fileElement = std::make_shared <FolderElement> ();

	folders .emplace (directory -> get_uri (), fileElement);

	fileElement -> fileMonitor = directory -> monitor_directory (Gio::FILE_MONITOR_WATCH_MOVES);

	fileElement -> fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &ProjectsEditor::on_file_changed));
}

void
ProjectsEditor::addChildren (const Gtk::TreeIter & parentIter, const Glib::RefPtr <Gio::File> & directory)
{
	try
	{
		for (const auto & fileInfo : X3DLibraryView::getChildren (directory))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY:
				{
					const auto child = directory -> get_child (fileInfo -> get_name ());
					const auto iter  = getProjectsStore () -> append (parentIter -> children ());

					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto child = directory -> get_child (fileInfo -> get_name ());
					const auto iter  = getProjectsStore () -> append (parentIter -> children ());

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
	iter -> set_value (Columns::ICON, X3DLibraryView::getIconName (file -> query_info (), "gtk-file"));
	iter -> set_value (Columns::NAME, file -> get_basename ());
	iter -> set_value (Columns::URL,  file -> get_uri ());
}

void
ProjectsEditor::removeChild (const Gtk::TreeIter & iter)
{
	const auto URL = getUrl (iter);

	getProjectsStore () -> erase (iter);

	// Remove project and subfolders if URL is a directory.

	const auto child    = Gio::File::create_for_uri (URL);
	const auto fileInfo = child -> query_info ();

	if (fileInfo -> get_file_type () not_eq Gio::FILE_TYPE_DIRECTORY)
		return;

	// Remove project.

	projects .erase (URL);

	// Remove subfolders.

	std::vector <std::string> subfolders;

	for (const auto & pair : folders)
	{
		if (pair .first .find (URL) == 0)
			subfolders .emplace_back (pair .first);
	}

	for (const auto & subfolder : subfolders)
		folders .erase (subfolder);
}

Gtk::TreeIter
ProjectsEditor::getIter (const std::string & URL) const
{
	Gtk::TreeIter result;

	for (const auto & childIter : getProjectsStore () -> children ())
	{
		if (getIter (childIter, URL, result))
			break;
	}

	return result;
}

bool
ProjectsEditor::getIter (const Gtk::TreeIter & iter, const std::string & URL, Gtk::TreeIter & result) const
{
	const auto parentURL = getUrl (iter);

	if (parentURL == URL)
	{
		result = iter;
		return true;
	}

	if (URL .find (parentURL) not_eq 0)
		return false;

	for (const auto & childIter : iter -> children ())
	{
		if (getIter (childIter, URL, result))
			return true;
	}

	return false;
}

std::string
ProjectsEditor::getUrl (const Gtk::TreeIter & iter) const
{
	std::string URL;

	iter -> get_value (Columns::URL, URL);

	return URL;
}

void
ProjectsEditor::launchUrl (const std::string & URL)
{
	const auto file        = Gio::File::create_for_uri (URL);
	const auto contentType = file -> query_info () -> get_content_type ();
	const auto appInfos    = Gio::AppInfo::get_all_for_type (contentType);

	for (const auto & appInfo : appInfos)
	{
		if (appInfo -> get_executable () == "titania")
		{
			getBrowserWindow () -> open (URL);
			return;
		}
	}

	const auto appInfo = file -> query_default_handler ();

	if (appInfo)
		appInfo -> launch (file);
}

void
ProjectsEditor::restoreExpanded ()
{
	const auto URLs = getConfig () -> getItem <X3D::MFString> ("expanded");

	for (const auto & URL : URLs)
	{
		const auto iter = getIter (URL);

		if (not getProjectsStore () -> iter_is_valid (iter))
			continue;

		getProjectsView () .expand_row (getProjectsStore () -> get_path (iter), false);
	}

	scrollFreezer -> restore (getConfig () -> getItem <double> ("hadjustment"), getConfig () -> getItem <double> ("vadjustment"));
}

void
ProjectsEditor::saveExpanded ()
{
	X3D::MFString URLs;

	getExpanded (getProjectsStore () -> children (), URLs);

	getConfig () -> setItem <X3D::MFString> ("expanded", URLs);
	getConfig () -> setItem <double> ("hadjustment", getProjectsView () .get_hadjustment () -> get_value ());
	getConfig () -> setItem <double> ("vadjustment", getProjectsView () .get_vadjustment () -> get_value ());
}

void
ProjectsEditor::getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & URLs)
{
	for (const auto & child : children)
	{
		if (getProjectsView () .row_expanded (getProjectsStore () -> get_path (child)))
			URLs .emplace_back (getUrl (child));

		getExpanded (child -> children (), URLs);
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
