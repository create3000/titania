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

namespace titania {
namespace puck {

ExternalToolsEditor::ExternalToolsEditor (X3DBrowserWindow* const browserWindow) :
	               X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DExternalToolsEditorInterface (get_ui ("Editors/ExternalToolsEditor.glade")),
	         X3DExternalToolsEditor ()
{
	getSourceView () .get_source_buffer () -> signal_changed () .connect (sigc::mem_fun (this, &ExternalToolsEditor::on_text_changed));
	getSourceView () .get_source_buffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme ("x_ite"));

	getNameRenderer () -> signal_edited () .connect (sigc::mem_fun (this, &ExternalToolsEditor::on_name_edited));
	getNameRenderer () -> property_editable () = true;

	setup ();
}

void
ExternalToolsEditor::initialize ()
{
	X3DExternalToolsEditorInterface::initialize ();
	X3DExternalToolsEditor::initialize ();
}

void
ExternalToolsEditor::configure ()
{
	X3DExternalToolsEditorInterface::configure ();
	X3DExternalToolsEditor::configure ();

	restoreTree ();
}

void
ExternalToolsEditor::on_add_tool_clicked ()
{
	try
	{
		const auto id   = createTool ();
		const auto iter = getTreeStore () -> append ();
	
		setId   (iter, id);
		setName (iter, _ ("New Tool"));
		setText (id, "#!/bin/sh\n");
		saveTree ();

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
ExternalToolsEditor::on_name_edited (const Glib::ustring & path, const Glib::ustring & new_text)
{
	if (new_text .empty ())
		return;

	const auto iter = getTreeStore () -> get_iter (path);

	setName (iter, new_text);
	saveTree ();
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

void
ExternalToolsEditor::store ()
{
	X3DExternalToolsEditor::store ();
	X3DExternalToolsEditorInterface::store ();
}

ExternalToolsEditor::~ExternalToolsEditor ()
{
	dispose ();
}

} // puck
} // titania
