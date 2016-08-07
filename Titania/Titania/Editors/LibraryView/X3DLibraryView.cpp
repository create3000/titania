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

#include "X3DLibraryView.h"

#include "../../Base/AdjustmentObject.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"

#include <Titania/String.h>
#include <Titania/OS/file_exists.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int ICON         = 0;
static constexpr int NAME         = 1;
static constexpr int TITANIA      = 2;
static constexpr int COBWEB       = 3;
static constexpr int EXPERIMENTAL = 4;

};

X3DLibraryView::X3DLibraryView () :
	X3DLibraryViewInterface (),
	            hadjustment (new AdjustmentObject ()),
	            vadjustment (new AdjustmentObject ())
{ }

void
X3DLibraryView::initialize ()
{
	try
	{
	   #ifndef TITANIA_DEBUG
	   getExperimantalColumn () -> set_visible (false);
	   #endif

		append (getRoot ());

		restoreExpanded ();
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

std::string
X3DLibraryView::getRoot () const
{
	return find_data_file ("Library");
}

std::string
X3DLibraryView::getFilename (Gtk::TreeModel::Path path) const
{
	std::string filename;

	while (path .size ())
	{
		const auto iter = getTreeStore () -> get_iter (path);

		std::string name;
		iter -> get_value (Columns::NAME, name);

		filename = name + '/' + filename;

		path .up ();
	}

	return getRoot () + '/' + filename .substr (0, filename .size () - 1);
}

std::vector <Glib::RefPtr <Gio::FileInfo>>
X3DLibraryView::getChildren (const Glib::RefPtr <Gio::File> & directory)
{
	std::vector <Glib::RefPtr <Gio::FileInfo>> fileInfos;

	try
	{
		const Glib::RefPtr <Gio::FileEnumerator> enumerator = directory -> enumerate_children ();
		Glib::RefPtr <Gio::FileInfo>             fileInfo   = enumerator -> next_file ();

		while (fileInfo)
		{
			if (not fileInfo -> is_hidden ())
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

bool
X3DLibraryView::containsFiles (const Glib::RefPtr <Gio::File> & directory)
{
	for (const auto & fileInfo : X3DLibraryView::getChildren (directory))
	{
		switch (fileInfo -> get_file_type ())
		{
			case Gio::FILE_TYPE_REGULAR       :
			case Gio::FILE_TYPE_SYMBOLIC_LINK :
				return true;
			default:
				continue;
		}
	}

	return false;
}

void
X3DLibraryView::append (const std::string & path) const
{
	static const std::string empty_string;
	static const std::string experimental_icon ("Experimental");
	static const std::string titania_icon ("Titania");
	static const std::string cobweb_icon ("Cobweb");

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
					iter -> set_value (Columns::ICON, std::string ("gtk-directory"));
					iter -> set_value (Columns::NAME, fileInfo -> get_name ());
					append (iter, directory -> get_child (fileInfo -> get_name ()));
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto basename     = basic::uri (fileInfo -> get_name ()) .basename (false);
					const bool experimental = os::file_exists (directory -> get_path () + "/.experimental/" + basename);

					#ifndef TITANIA_DEBUG
					if (experimental)
					   continue;
					#endif

					const bool titania  = os::file_exists (directory -> get_path () + "/.Titania/" + basename);
					const bool cobweb   = os::file_exists (directory -> get_path () + "/.Cobweb/" + basename);
					const auto iter     = getTreeStore () -> append ();

					iter -> set_value (Columns::ICON,         std::string ("gtk-file"));
					iter -> set_value (Columns::NAME,         fileInfo -> get_name ());
					iter -> set_value (Columns::EXPERIMENTAL, experimental ? experimental_icon : empty_string);
					iter -> set_value (Columns::TITANIA,      titania ? titania_icon : empty_string);
					iter -> set_value (Columns::COBWEB,       cobweb  ? cobweb_icon  : empty_string);
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
X3DLibraryView::append (Gtk::TreeModel::iterator & parent, const Glib::RefPtr <Gio::File> & directory) const
{
	static const std::string empty_string;
	static const std::string experimental_icon ("Experimental");
	static const std::string titania_icon ("Titania");
	static const std::string cobweb_icon ("Cobweb");

	try
	{
		for (const auto & fileInfo : getChildren (directory))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY :
					{
						auto iter = getTreeStore () -> append (parent -> children ());
						iter -> set_value (Columns::ICON, std::string ("gtk-directory"));
						iter -> set_value (Columns::NAME, fileInfo -> get_name ());
						append (iter, directory -> get_child (fileInfo -> get_name ()));
						continue;
					}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto basename     = basic::uri (fileInfo -> get_name ()) .basename (false);
					const bool experimental = os::file_exists (directory -> get_path () + "/.experimental/" + basename);

					#ifndef TITANIA_DEBUG
					if (experimental)
					   continue;
					#endif

					const bool titania  = os::file_exists (directory -> get_path () + "/.Titania/" + basename);
					const bool cobweb   = os::file_exists (directory -> get_path () + "/.Cobweb/" + basename);
					const auto iter     = getTreeStore () -> append (parent -> children ());

					iter -> set_value (Columns::ICON,         std::string ("gtk-file"));
					iter -> set_value (Columns::NAME,         fileInfo -> get_name ());
					iter -> set_value (Columns::EXPERIMENTAL, experimental ? experimental_icon : empty_string);
					iter -> set_value (Columns::TITANIA,      titania ? titania_icon : empty_string);
					iter -> set_value (Columns::COBWEB,       cobweb  ? cobweb_icon  : empty_string);
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
X3DLibraryView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn*)
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
				const auto nodes    = getBrowserWindow () -> import ({ file -> get_uri () }, undoStep);

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
X3DLibraryView::restoreExpanded ()
{
	const auto expanded = getConfig () -> getString ("expanded");
	const auto paths    = basic::split (expanded, ";");

	for (const auto & path : paths)
		getTreeView () .expand_row (Gtk::TreePath (path), false);

	hadjustment -> restore (getTreeView () .get_hadjustment (), getConfig () -> getDouble ("hadjustment"));
	vadjustment -> restore (getTreeView () .get_vadjustment (), getConfig () -> getDouble ("vadjustment"));
}

void
X3DLibraryView::saveExpanded ()
{
	std::deque <std::string> paths;

	getExpanded (getTreeStore () -> children (), paths);

	const auto expanded = basic::join (paths, ";");

	getConfig () -> setItem ("expanded", expanded);
	getConfig () -> setItem ("hadjustment", getTreeView () .get_hadjustment () -> get_value ());
	getConfig () -> setItem ("vadjustment", getTreeView () .get_vadjustment () -> get_value ());
}

void
X3DLibraryView::getExpanded (const Gtk::TreeModel::Children & children, std::deque <std::string> & paths) const
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

X3DLibraryView::~X3DLibraryView ()
{
	if (isInitialized ())
		saveExpanded ();
}

} // puck
} // titania
