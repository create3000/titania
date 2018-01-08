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

#include "X3DLibraryView.h"

#include "../../Base/ScrollFreezer.h"
#include "../../Bits/File.h"
#include "../../Browser/X3DBrowserWindow.h"
#include "../../Browser/BrowserSelection.h"
#include "../../Configuration/config.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

namespace Columns {

static constexpr int ICON         = 0;
static constexpr int NAME         = 1;
static constexpr int TITANIA      = 2;
static constexpr int X_ITE        = 3;
static constexpr int EXPERIMENTAL = 4;
static constexpr int PATH         = 5;

};

X3DLibraryView::X3DLibraryView () :
	X3DLibraryViewInterface (),
	          scrollFreezer (new ScrollFreezer (getTreeView ()))
{
   #ifndef TITANIA_DEBUG
   getExperimantalColumn () -> set_visible (false);
   #endif
}

void
X3DLibraryView::initialize ()
{ }

void
X3DLibraryView::configure ()
{
	setRootFolder (Gio::File::create_for_path (find_data_file ("Library")));

	restoreExpanded ();
}

void
X3DLibraryView::on_row_activated (const Gtk::TreeModel::Path & path, Gtk::TreeViewColumn* column)
{
	try
	{
		const auto iter = getTreeStore () -> get_iter (path);
		const auto file = Gio::File::create_for_path (getPath (iter));

		switch (file -> query_info () -> get_file_type ())
		{
			case Gio::FILE_TYPE_REGULAR:
			case Gio::FILE_TYPE_SYMBOLIC_LINK:
			{
				const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Import From Library"));
				const auto nodes    = getBrowserWindow () -> import ({ file -> get_uri () }, undoStep);

				getBrowserWindow () -> getSelection () -> setNodes (nodes, undoStep);
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
X3DLibraryView::setRootFolder (const Glib::RefPtr <Gio::File> & folder)
{
	try
	{
		getTreeStore () -> clear ();

		for (const auto & fileInfo : File::getChildren (folder))
		{
			switch (fileInfo -> get_file_type ())
			{
				case Gio::FILE_TYPE_DIRECTORY:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getTreeStore () -> append ();
	
					addFolder (iter, child);
					continue;
				}
				case Gio::FILE_TYPE_REGULAR:
				case Gio::FILE_TYPE_SYMBOLIC_LINK:
				{
					const auto child = folder -> get_child (fileInfo -> get_name ());
					const auto iter  = getTreeStore () -> append ();
	
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
X3DLibraryView::addFolder (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & folder)
{
	addChild (iter, folder, "gtk-directory");
	addChildren (iter, folder);
}

void
X3DLibraryView::addChildren (const Gtk::TreeIter & parentIter, const Glib::RefPtr <Gio::File> & folder)
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
X3DLibraryView::addChild (const Gtk::TreeIter & iter, const Glib::RefPtr <Gio::File> & file, const std::string & defaultIcon)
{
	const auto url = basic::uri (file -> get_uri ());

	iter -> set_value (Columns::ICON, File::getIconName (file -> query_info (), "gtk-file"));
	iter -> set_value (Columns::PATH, file -> get_path ());

	if (url .is_local ())
		iter -> set_value (Columns::NAME, file -> get_basename ());
	else
		iter -> set_value (Columns::NAME, file -> get_basename () + " (" + url .authority () + ")");

	const auto directory    = file -> get_parent ();
	const auto basename     = basic::uri (file -> get_path ()) .basename (false);
	const bool titania      = Gio::File::create_for_path (directory -> get_path () + "/.Titania/"      + basename) -> query_exists ();
	const bool x_ite        = Gio::File::create_for_path (directory -> get_path () + "/.X_ITE/"        + basename) -> query_exists ();
	const bool experimental = Gio::File::create_for_path (directory -> get_path () + "/.experimental/" + basename) -> query_exists ();

	#ifndef TITANIA_FEATURE
	if (experimental and not (x_ite or titania))
	{
		getTreeStore () -> erase (iter);
	   return;
	}
	#endif

	iter -> set_value (Columns::EXPERIMENTAL, std::string (experimental ? "Experimental" : ""));
	iter -> set_value (Columns::TITANIA,      std::string (titania      ? "Titania"      : ""));
	iter -> set_value (Columns::X_ITE,        std::string (x_ite        ? "X_ITE"        : ""));
}

Gtk::TreeIter
X3DLibraryView::getIter (const std::string & path) const
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
X3DLibraryView::getIter (const Gtk::TreeIter & iter, const std::string & path, Gtk::TreeIter & result) const
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
X3DLibraryView::getPath (const Gtk::TreeIter & iter) const
{
	std::string path;

	iter -> get_value (Columns::PATH, path);

	return path;
}

void
X3DLibraryView::restoreExpanded ()
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
X3DLibraryView::saveExpanded ()
{
	X3D::MFString folders;

	getExpanded (getTreeStore () -> children (), folders);

	getConfig () -> setItem <X3D::MFString> ("expanded", folders);
	getConfig () -> setItem <double> ("hadjustment", getTreeView () .get_hadjustment () -> get_value ());
	getConfig () -> setItem <double> ("vadjustment", getTreeView () .get_vadjustment () -> get_value ());
}

void
X3DLibraryView::getExpanded (const Gtk::TreeModel::Children & children, X3D::MFString & folders)
{
	for (const auto & child : children)
	{
		if (getTreeView () .row_expanded (getTreeStore () -> get_path (child)))
			folders .emplace_back (getPath (child));

		getExpanded (child -> children (), folders);
	}
}

void
X3DLibraryView::store ()
{
	saveExpanded ();
}

X3DLibraryView::~X3DLibraryView ()
{ }

} // puck
} // titania
