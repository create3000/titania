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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "LibraryView.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

static constexpr int ICON_COLUMN = 0;
static constexpr int NAME_COLUMN = 1;

LibraryView::LibraryView (X3DBrowserWindow* const browserWindow) :
	       X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	X3DLibraryViewInterface (get_ui ("LibraryView.xml"), gconf_dir ())
{
	setup ();
}

void
LibraryView::on_map ()
{
	getBrowserWindow () -> getSideBarLabel () .set_text (_ ("Library"));
}

void
LibraryView::initialize ()
{
	try
	{
		append (getRoot ());

		restoreExpanded ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

std::string
LibraryView::getRoot () const
{
	return find_data_file ("Library");
}

std::string
LibraryView::getFilename (Gtk::TreeModel::Path path) const
{
	std::string filename;

	while (path .size ())
	{
		const auto iter = getTreeStore () -> get_iter (path);

		std::string name;
		iter -> get_value (NAME_COLUMN, name);

		filename = name + '/' + filename;

		path .up ();
	}

	return getRoot () + '/' + filename .substr (0, filename .size () - 1);
}

std::vector <Glib::RefPtr <Gio::FileInfo>>
LibraryView::getChildren (const Glib::RefPtr <Gio::File> & directory)
{
	std::vector <Glib::RefPtr <Gio::FileInfo>> fileInfos;

	try
	{
		const Glib::RefPtr <Gio::FileEnumerator> enumerator = directory -> enumerate_children ();
		Glib::RefPtr <Gio::FileInfo>             fileInfo   = enumerator -> next_file ();

		while (fileInfo)
		{
			if (fileInfo -> is_hidden ())
				continue;

			fileInfos .emplace_back (fileInfo);
			fileInfo = enumerator -> next_file ();
		}
	}
	catch (...)
	{ }

	std::sort (fileInfos .begin (), fileInfos .end (), [ ] (const Glib::RefPtr <Gio::FileInfo> & lhs, const Glib::RefPtr <Gio::FileInfo> & rhs)
	           {
	              return basic::naturally_compare (lhs -> get_name (), rhs -> get_name ());
				  });

	std::stable_sort (fileInfos .begin (), fileInfos .end (), [ ] (const Glib::RefPtr <Gio::FileInfo> & lhs, const Glib::RefPtr <Gio::FileInfo> & rhs)
	                  {
	                     return (lhs -> get_file_type () == Gio::FILE_TYPE_DIRECTORY) > (rhs -> get_file_type () == Gio::FILE_TYPE_DIRECTORY);
							});

	return fileInfos;
}

void
LibraryView::append (const std::string & path) const
{
	try
	{
		const Glib::RefPtr <Gio::File> directory = Gio::File::create_for_path (path);

		for (const auto & fileInfo : getChildren (directory))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY :
					{
						auto iter = getTreeStore () -> append ();
						iter -> set_value (ICON_COLUMN, std::string ("gtk-directory"));
						iter -> set_value (NAME_COLUMN, fileInfo -> get_name ());
						append (iter, directory -> get_child (fileInfo -> get_name ()));
						break;
					}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto iter = getTreeStore () -> append ();
					iter -> set_value (ICON_COLUMN, std::string ("gtk-file"));
					iter -> set_value (NAME_COLUMN, fileInfo -> get_name ());
					break;
				}
				default:
					break;
			}
		}
	}
	catch (...)
	{ }
}

void
LibraryView::append (Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & directory) const
{
	try
	{
		for (const auto & fileInfo : getChildren (directory))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY :
					{
						auto iter = getTreeStore () -> append (parent -> children ());
						iter -> set_value (ICON_COLUMN, std::string ("gtk-directory"));
						iter -> set_value (NAME_COLUMN, fileInfo -> get_name ());
						append (iter, directory -> get_child (fileInfo -> get_name ()));
						break;
					}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto iter = getTreeStore () -> append (parent -> children ());
					iter -> set_value (ICON_COLUMN, std::string ("gtk-file"));
					iter -> set_value (NAME_COLUMN, fileInfo -> get_name ());
					break;
				}
				default:
					break;
			}
		}
	}
	catch (...)
	{ }
}

void
LibraryView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
{
	try
	{
		const Glib::RefPtr <Gio::File> file = Gio::File::create_for_path (getFilename (path));

		switch (file -> query_info () -> get_file_type ())
		{
			case Gio::FILE_TYPE_REGULAR:
			case Gio::FILE_TYPE_SYMBOLIC_LINK:
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import From Library"));
				const auto nodes    = getBrowserWindow () -> importURL ({ file -> get_uri () }, false, undoStep);

				getBrowserWindow () -> getSelection () -> setChildren (nodes, undoStep);
				getBrowserWindow () -> addUndoStep (undoStep);
				break;
			}
			default:
				break;
		}
	}
	catch (...)
	{ }
}

void
LibraryView::restoreExpanded ()
{
	const auto expanded = getConfig () .getString ("expanded");
	const auto paths    = basic::split (expanded, ";");

	for (const auto & path : paths)
		getTreeView () .expand_row (Gtk::TreePath (path), false);
}

void
LibraryView::saveExpanded ()
{
	std::deque <std::string> paths;

	getExpanded (getTreeStore () -> children (), paths);

	const auto expanded = basic::join (paths, ";");

	getConfig () .setItem ("expanded", expanded);
}

void
LibraryView::getExpanded (const Gtk::TreeModel::Children & children, std::deque <std::string> & paths) const
{
	for (const auto & child : children)
	{
		const auto path     = getTreeStore () -> get_path (child);
		const bool expanded = getTreeView () .row_expanded (path);

		if (expanded)
		{
			paths .emplace_back (path .to_string ());
			getExpanded (child -> children (), paths);
		}
	}
}

LibraryView::~LibraryView ()
{
	if (isInitialized ())
		saveExpanded ();

	dispose ();
}

} // puck
} // titania
