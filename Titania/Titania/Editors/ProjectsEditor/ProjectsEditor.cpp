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

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"
#include "../../Dialogs/FileOpenDialog/OpenDirectoryDialog.h"
#include "../../Editors/LibraryView/X3DLibraryView.h"

namespace titania {
namespace puck {

namespace Columns {

static constexpr int ICON = 0;
static constexpr int NAME = 1;

};

class ProjectsEditor::FolderElement
{
public:

	Glib::RefPtr <Gio::FileMonitor> fileMonitor;
};

ProjectsEditor::ProjectsEditor (X3DBrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DProjectsEditorInterface (get_ui ("Editors/ProjectsEditor.glade")),
	                   folders (),
	               folderIndex ()
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

	const auto folders = getConfig () -> getItem <X3D::MFString> ("folders");

	for (const auto & folder : folders)
		addRootFolder (folder .str ());
}

void
ProjectsEditor::on_add_folder_clicked ()
{
	const auto openDirectoryDialog = std::dynamic_pointer_cast <OpenDirectoryDialog> (createDialog ("OpenDirectoryDialog"));

	if (not openDirectoryDialog -> run ())
		return;

	// Create root folder.

	addRootFolder (openDirectoryDialog -> getUrl ());
}

void
ProjectsEditor::on_file_changed (const Glib::RefPtr <Gio::File> & file,
                                 const Glib::RefPtr <Gio::File> & other_file,
                                 Gio::FileMonitorEvent event)
{
	if (file)
		__LOG__ << file -> get_uri () << std::endl;

	if (other_file)
		__LOG__ << other_file -> get_uri () << std::endl;

	switch (event)
	{
		case Gio::FILE_MONITOR_EVENT_CHANGED:
			__LOG__ << "FILE_MONITOR_EVENT_CHANGED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_CHANGES_DONE_HINT:
			__LOG__ << "FILE_MONITOR_EVENT_CHANGES_DONE_HINT" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_DELETED:
			__LOG__ << "FILE_MONITOR_EVENT_DELETED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_CREATED:
			__LOG__ << "FILE_MONITOR_EVENT_CREATED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED:
			__LOG__ << "FILE_MONITOR_EVENT_ATTRIBUTE_CHANGED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_PRE_UNMOUNT:
			__LOG__ << "FILE_MONITOR_EVENT_PRE_UNMOUNT" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_UNMOUNTED:
			__LOG__ << "FILE_MONITOR_EVENT_UNMOUNTED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_MOVED:
			__LOG__ << "FILE_MONITOR_EVENT_MOVED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_RENAMED:
			__LOG__ << "FILE_MONITOR_EVENT_RENAMED" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_MOVED_IN:
			__LOG__ << "FILE_MONITOR_EVENT_MOVED_IN" << std::endl;
			break;
		case Gio::FILE_MONITOR_EVENT_MOVED_OUT:
			__LOG__ << "FILE_MONITOR_EVENT_MOVED_OUT" << std::endl;
			break;
	}
}

void
ProjectsEditor::on_row_activated (const Gtk::TreeModel::Path &, Gtk::TreeViewColumn*)
{

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

		if (not folders .emplace (URL) .second)
			return;
	
		if (not folderIndex .emplace (directory -> get_uri (), std::make_shared <FolderElement> ()) .second)
			return;

		auto iter = getTreeStore () -> append ();

		addFolder (iter, directory);
	}
	catch (...)
	{ }
}

void
ProjectsEditor::addFolder (Gtk::TreeModel::iterator & iter, const Glib::RefPtr <Gio::File> & directory)
{
	const auto & fileElement = folderIndex [directory -> get_uri ()];

	fileElement -> fileMonitor = directory -> monitor_directory (Gio::FILE_MONITOR_WATCH_MOVES);

	fileElement -> fileMonitor -> signal_changed () .connect (sigc::mem_fun (this, &ProjectsEditor::on_file_changed));

	iter -> set_value (Columns::ICON, std::string ("gtk-directory"));
	iter -> set_value (Columns::NAME, directory -> get_basename ());

	addChildren (iter, directory);
}

void
ProjectsEditor::addChildren (Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & directory)
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

					if (not folderIndex .emplace (child -> get_uri (), std::make_shared <FolderElement> ()) .second)
						continue;

					auto iter = getTreeStore () -> append (parent -> children ());
		
					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto basename = basic::uri (fileInfo -> get_name ()) .basename (false);
					const auto iter     = getTreeStore () -> append (parent -> children ());

					iter -> set_value (Columns::ICON, std::string ("gtk-file"));
					iter -> set_value (Columns::NAME, fileInfo -> get_name ());
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
ProjectsEditor::store ()
{
	getConfig () -> setItem <X3D::MFString> ("folders", X3D::MFString (folders .begin (), folders .end ()));

	X3DProjectsEditorInterface::store ();
}

ProjectsEditor::~ProjectsEditor ()
{
	dispose ();
}

} // puck
} // titania
