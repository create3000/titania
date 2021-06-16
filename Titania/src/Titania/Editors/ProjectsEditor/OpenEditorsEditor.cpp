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

#include "OpenEditorsEditor.h"

#include "../../Bits/File.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../BrowserNotebook/NotebookPage/NotebookPage.h"
#include "../../Configuration/config.h"
#include "../../Editors/ProjectsEditor/ProjectsEditor.h"

#include <regex>

namespace titania {
namespace puck {

struct OpenEditorsEditor::Columns
{
	static constexpr int32_t ICON   = 0;
	static constexpr int32_t NAME   = 1;
	static constexpr int32_t FOLDER = 2;
	static constexpr int32_t CLOSE  = 3;
};

OpenEditorsEditor::OpenEditorsEditor (X3DBrowserWindow* const browserWindow, ProjectsEditor* const projectsEditor) :
	             X3DBaseInterface (browserWindow, browserWindow -> getMasterBrowser ()),
	X3DOpenEditorsEditorInterface (get_ui ("Editors/OpenEditorsEditor.glade")),
	               projectsEditor (projectsEditor)
{
	setup ();
}

void
OpenEditorsEditor::initialize ()
{
	X3DOpenEditorsEditorInterface::initialize ();

	getBrowserWindow () -> worldURL_changed () .addInterest (this, &OpenEditorsEditor::set_pages);

	projectsEditor -> rootFolders_changed () .addInterest (this, &OpenEditorsEditor::set_pages);

	getBrowserWindow () -> getBrowserNotebook () .signal_page_reordered () .connect (sigc::mem_fun (this, &OpenEditorsEditor::on_notebook_changed));
	getBrowserWindow () -> getBrowserNotebook () .signal_page_removed ()   .connect (sigc::mem_fun (this, &OpenEditorsEditor::on_notebook_changed));
	getBrowserWindow () -> getBrowserNotebook () .signal_page_added ()     .connect (sigc::mem_fun (this, &OpenEditorsEditor::on_notebook_changed));
	getBrowserWindow () -> getBrowserNotebook () .signal_switch_page ()    .connect (sigc::mem_fun (this, &OpenEditorsEditor::on_switch_page));
}

void
OpenEditorsEditor::on_map ()
{
	set_pages ();
}

void
OpenEditorsEditor::on_notebook_changed (Gtk::Widget* widget, guint pageNumber)
{
	set_pages ();
}

void
OpenEditorsEditor::set_pages ()
{
	if (not getWidget () .get_mapped ())
		return;

	getListStore () -> clear ();

	for (const auto & page : getBrowserWindow () -> getPages ())
	{
		const auto & browser  = page -> getMainBrowser ();
		const auto & worldURL = page -> getWorldURL ();
		const auto   row      = getListStore () -> append ();

		browser -> initialized () .addInterest (this, &OpenEditorsEditor::set_pages);

		row -> set_value (Columns::ICON, page -> getMasterSceneURL () .filename () .str ());
		row -> set_value (Columns::NAME, worldURL .is_local () ? worldURL .basename () : worldURL .escape (" ") .basename ());

		// Folder

		if (worldURL .is_local ())
		{
			const auto path   = worldURL .parent () .path ();
			const auto folder = Gio::File::create_for_path (path);
			auto       found  = false;

			for (const basic::uri rootFolder : projectsEditor -> getRootFolders ())
			{
				const auto projectFolder = Gio::File::create_for_path (rootFolder .path ());

				if (File::isSubfolder (folder, projectFolder))
				{
					found = true;

					const auto releativePath = rootFolder .relative_path (path);
					auto       string        = rootFolder .basename ();

					if (releativePath not_eq "./")
					{
						string += " • ";
						string += std::regex_replace (releativePath .str (), std::regex ("/$"), "");
					}

					row -> set_value (Columns::FOLDER, string);
					break;
				}
			}

			if (not found)
			{
				const auto homeFolder = Gio::File::create_for_path (Glib::get_home_dir ());

				if (File::isSubfolder (folder, homeFolder))
				{
					const auto  home          = basic::uri (basic::path (Glib::get_home_dir ()) .escape () + "/");
					const auto  releativePath = home .relative_path (path);
					std::string string        = "~";

					if (releativePath not_eq "./")
					{
						string += "/";
						string += std::regex_replace (releativePath .str (), std::regex ("/$"), "");
					}

					row -> set_value (Columns::FOLDER, string);
				}
				else
				{
					row -> set_value (Columns::FOLDER, std::regex_replace (path, std::regex ("/$"), ""));
				}
			}
		}
		else
		{
			row -> set_value (Columns::FOLDER, std::regex_replace (worldURL .parent () .escape () .str (), std::regex ("/$"), ""));
		}
	}

	on_switch_page (nullptr, getBrowserWindow () -> getBrowserNotebook () .get_current_page ());
}

void
OpenEditorsEditor::on_switch_page (Gtk::Widget*, guint pageNumber)
{
	const Gtk::TreePath path (basic::to_string (pageNumber, std::locale::classic ()));

	getTreeView () .get_selection () -> select (path);
}

void
OpenEditorsEditor::on_selection_changed ()
{
	for (const auto & row : getListStore () -> children ())
		row -> set_value (Columns::CLOSE, std::string ());

	if (getTreeView () .get_selection () -> count_selected_rows ())
	{
		const auto selected = getTreeView () .get_selection () -> get_selected ();

		selected -> set_value (Columns::CLOSE, std::string ("gtk-close"));
	}
}

void
OpenEditorsEditor::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	try
	{
		const auto page = getBrowserWindow () -> getPages () .at (path .front ());

		getBrowserWindow () -> getBrowserNotebook () .set_current_page (page -> getPageNumber ());
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

bool
OpenEditorsEditor::on_button_release_event (GdkEventButton* event)
{
	try
	{
		Gtk::TreePath        path;
		Gtk::TreeViewColumn* column = nullptr;
		int                  cell_x = 0;
		int                  cell_y = 0;

		getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

		if (not path .size ())
			return false;

		if (column not_eq getCloseColumn () .operator -> ())
			return false;

		const auto page = getBrowserWindow () -> getPages () .at (path .front ());

		getBrowserWindow () -> close (page);

		return true;
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
		return true;
	}
}

bool
OpenEditorsEditor::on_motion_notify_event (GdkEventMotion* event)
{
	Gtk::TreePath        path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (not path .size ())
		return false;

	const auto row = getListStore () -> get_iter (path);

	on_selection_changed ();

	row -> set_value (Columns::CLOSE, std::string ("gtk-close"));

	return true;
}

bool
OpenEditorsEditor::on_leave_notify_event (GdkEventCrossing* event)
{
	on_selection_changed ();
	return false;
}

OpenEditorsEditor::~OpenEditorsEditor ()
{
	dispose ();
}

} // puck
} // titania
