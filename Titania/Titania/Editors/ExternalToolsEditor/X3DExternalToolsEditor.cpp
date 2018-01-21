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

#include "X3DExternalToolsEditor.h"

#include "../../Configuration/config.h"

#include <Titania/OS.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

class X3DExternalToolsEditor::Columns {
public:

	static constexpr size_t ID   = 0;
	static constexpr size_t NAME = 1;

};

X3DExternalToolsEditor::X3DExternalToolsEditor () :
	X3DExternalToolsEditorInterface ()
{ }

Glib::RefPtr <Gio::File>
X3DExternalToolsEditor::getToolFolder ()
{
	const auto folder = Gio::File::create_for_path (config_dir ("Tools"));

	if (not folder -> query_exists ())
		folder -> make_directory_with_parents ();

	return folder;
}

std::string
X3DExternalToolsEditor::createTool ()
{
	const auto  folder   = getToolFolder ();
	std::string filename = folder -> get_path () + "/XXXXXX.txt";
	auto        ofstream = os::mkstemps (filename, 4);
	const auto  id       = filename .substr (filename .size () - 10, 6);

	return id;
}

std::string
X3DExternalToolsEditor::getId (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::ID, value);

	return value;
}

void
X3DExternalToolsEditor::setId (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::ID, value);
}

std::string
X3DExternalToolsEditor::getName (const Gtk::TreeIter & iter) const
{
	auto value = std::string ();

	iter -> get_value (Columns::NAME, value);

	return value;
}

void
X3DExternalToolsEditor::setName (const Gtk::TreeIter & iter, const std::string & value) const
{
	iter -> set_value (Columns::NAME, value);
}

void
X3DExternalToolsEditor::setText (const std::string & id, const std::string & text)
{
	const auto folder   = getToolFolder ();
	const auto file     = folder -> get_child (id + ".txt");
	auto       ofstream = std::ofstream (file -> get_path ());

	ofstream << text;
}

std::string
X3DExternalToolsEditor::getText (const std::string & id)
{
	const auto folder = getToolFolder ();
	const auto file   = folder -> get_child (id + ".txt");

	return os::load_file (file -> get_path ());
}

std::string
X3DExternalToolsEditor::getContentType (const std::string & data) const
{
	bool        result_uncertain;
	std::string contentType = Gio::content_type_guess ("", (guchar*) &data [0], data .size (), result_uncertain);

	return contentType;
}

void
X3DExternalToolsEditor::setLanguage (const std::string & text) const
{
	try
	{
		const auto contextType = getContentType (text);
		const auto language    = Gsv::LanguageManager::get_default () -> guess_language ("", contextType);

		getSourceView () .get_source_buffer () -> set_language (language);
	}
	catch (const Glib::Error & error)
	{ }
}

void
X3DExternalToolsEditor::restoreTree ()
{
	const auto string  = getConfig () -> getItem <std::string> ("tree");
	auto       strings = std::vector <std::string> ();

	basic::split (std::back_inserter (strings), string, "\n");

	std::vector <Gtk::TreePath> expand;

	for (const auto & value : strings)
	{
		try
		{
			auto array = std::vector <std::string> ();
	
			basic::split (std::back_inserter (array), value, "\t");
	
			auto         path     = Gtk::TreePath (array .at (0));
			const auto   expanded = array .at (1) == "true";
			const auto & id       = array .at (2);
			const auto & name     = array .at (3);

			if (expanded)
				expand .emplace_back (path);

			auto iter = Gtk::TreeIter ();

			switch (path .size ())
			{
				case 0:
				{
					continue;
				}
				case 1:
				{
					iter = getTreeStore () -> append ();
					break;
				}
				default:
				{
					if (not path .up ())
						continue;

					const auto parent = getTreeStore () -> get_iter (path);

					iter = getTreeStore () -> append (parent -> children ());
					break;
				}
			}

			setId   (iter, id);
			setName (iter, name);
		}
		catch (const std::out_of_range & error)
		{ }
	}

	for (const auto & path : expand)
		getTreeView () .expand_row (path, false);
}

void
X3DExternalToolsEditor::saveTree ()
{
	std::vector <std::vector <std::string>> tree;
	std::vector <std::string>               strings;
	std::string                             string;

	saveTree (getTreeStore () -> children (), tree);

	for (const auto & array : tree)
		strings .emplace_back (basic::join (array, "\t"));

	string = basic::join (strings, "\n");

	getConfig () -> setItem <std::string> ("tree", string);
}

void
X3DExternalToolsEditor::saveTree (const Gtk::TreeNodeChildren & children, std::vector <std::vector <std::string>> & tree) const
{
	for (const auto & child : children)
	{
		const auto path = getTreeStore () -> get_path (child);

		std::vector <std::string> array;

		array .emplace_back (path .to_string ());
		array .emplace_back (getTreeView () .row_expanded (path) ? "true" : "false");
		array .emplace_back (getId (child));
		array .emplace_back (getName (child));

		tree .emplace_back (std::move (array));

		saveTree (child -> children (), tree);
	}
}

X3DExternalToolsEditor::~X3DExternalToolsEditor ()
{ }

} // puck
} // titania
