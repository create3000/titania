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

#include "ExternalToolsEditor.h"

#include "../../Browser/X3DBrowserWindow.h"
#include "../../Configuration/config.h"

#include <Titania/OS.h>

namespace titania {
namespace puck {

class ExternalToolsEditor::Columns {
public:

	static constexpr size_t ID   = 0;
	static constexpr size_t NAME = 1;

};

ExternalToolsEditor::ExternalToolsEditor (X3DBrowserWindow* const browserWindow) :
	               X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DExternalToolsEditorInterface (get_ui ("Editors/ExternalToolsEditor.glade"))
{
	getSourceView () .get_source_buffer () -> signal_changed () .connect (sigc::mem_fun (this, &ExternalToolsEditor::on_text_changed));
	getSourceView () .get_source_buffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme ("x_ite"));

	setup ();
}

void
ExternalToolsEditor::initialize ()
{
	X3DExternalToolsEditorInterface::initialize ();
}

void
ExternalToolsEditor::configure ()
{
	X3DExternalToolsEditorInterface::configure ();

	restoreTree ();
}

void
ExternalToolsEditor::on_add_tool_clicked ()
{
	try
	{
		const auto id   = getNewId ();
		const auto iter = getTreeStore () -> append ();
	
		iter -> set_value (Columns::ID,   id);
		iter -> set_value (Columns::NAME, _ ("New Tool"));
	
		saveTree ();
		setText (id, "#!/bin/sh\n");
	
		getTreeSelection () -> select (iter);
	}
	catch (const Glib::Error & error)
	{ }
}

void
ExternalToolsEditor::on_remove_tool_clicked ()
{
}

void
ExternalToolsEditor::on_tree_selection_changed ()
{
	try
	{
		if (getTreeSelection () -> get_selected_rows () .empty ())
		{
			getToolBox () .set_sensitive (false);
			getSourceView () .get_source_buffer () -> set_text ("");
		}
		else
		{
			const auto iter = getTreeSelection () -> get_selected ();
			const auto id   = getId (iter);
			const auto text = getText (id);
		
			getToolBox () .set_sensitive (true);
			getSourceView () .get_source_buffer () -> set_text (text);
		
			setLanguage (text);
		}
	}
	catch (const Glib::Error & error)
	{ }
}

void
ExternalToolsEditor::on_text_changed ()
{
	try
	{
		const auto iter = getTreeSelection () -> get_selected ();
		const auto id   = getId (iter);
		const auto text = getSourceView () .get_source_buffer () -> get_text ();
	
		setText (id, text);
		//setLanguage (text);
	}
	catch (const Glib::Error & error)
	{ }
}

Glib::RefPtr <Gio::File>
ExternalToolsEditor::getToolFolder () const
{
	const auto folder = Gio::File::create_for_path (config_dir ("Tools"));

	if (not folder -> query_exists ())
		folder -> make_directory_with_parents ();

	return folder;
}

std::string
ExternalToolsEditor::getNewId () const
{
	const auto  folder   = getToolFolder ();
	std::string filename = folder -> get_path () + "/XXXXXX.txt";
	auto        ofstream = os::mkstemps (filename, 4);
	const auto  id       = filename .substr (filename .size () - 10, 6);

	return id;
}

void
ExternalToolsEditor::restoreTree ()
{

}

void
ExternalToolsEditor::saveTree ()
{

}

void
ExternalToolsEditor::setText (const std::string & id, const std::string & text)
{
	const auto folder   = getToolFolder ();
	const auto file     = folder -> get_child (id + ".txt");
	auto       ofstream = std::ofstream (file -> get_path ());

	ofstream << text;
}

std::string
ExternalToolsEditor::getText (const std::string & id) const
{
	const auto folder = getToolFolder ();
	const auto file   = folder -> get_child (id + ".txt");

	return os::load_file (file -> get_path ());
}

std::string
ExternalToolsEditor::getContentType (const std::string & data) const
{
	bool        result_uncertain;
	std::string contentType = Gio::content_type_guess ("", (guchar*) &data [0], data .size (), result_uncertain);

	return contentType;
}

void
ExternalToolsEditor::setLanguage (const std::string & text) const
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

std::string
ExternalToolsEditor::getId (const Gtk::TreeIter & iter) const
{
	auto id = std::string ();

	iter -> get_value (Columns::ID, id);

	return id;
}

void
ExternalToolsEditor::store ()
{
	X3DExternalToolsEditorInterface::store ();
}

ExternalToolsEditor::~ExternalToolsEditor ()
{
	dispose ();
}

} // puck
} // titania
