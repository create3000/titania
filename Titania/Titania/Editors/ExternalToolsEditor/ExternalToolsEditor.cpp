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

#include <Titania/Utility/Map.h>

namespace titania {
namespace puck {

const std::map <std::string, int32_t> ExternalToolsEditor::saveTypes = {
	std::make_pair ("NOTHING",       0),
	std::make_pair ("CURRENT_SCENE", 1),
	std::make_pair ("ALL_SCENES",    2),
};

const std::map <std::string, int32_t> ExternalToolsEditor::inputTypes = {
	std::make_pair ("NOTHING",          0),
	std::make_pair ("CURRENT_SCENE",    1),
	std::make_pair ("MASTER_SELECTION", 2),
};

const std::map <std::string, int32_t> ExternalToolsEditor::outputTypes = {
	std::make_pair ("NOTHING",                  0),
	std::make_pair ("DISPLAY_IN_CONSOLE",       1),
	std::make_pair ("CREATE_NEW_SCENE",         2),
	std::make_pair ("APPEND_TO_CURRENT_SCENE",  3),
	std::make_pair ("REPLACE_CURRENT_SCENE",    4),
	std::make_pair ("APPEND_TO_CURRENT_LAYER",  5),
	std::make_pair ("REPLACE_MASTER_SELECTION", 6),
};

const std::map <std::string, int32_t> ExternalToolsEditor::applicabilityTypes = {
	std::make_pair ("ALWAYS_AVAILABLE",                0),
	std::make_pair ("ALL_SCENES_EXCEPT_UNTITLED_ONES", 1),
	std::make_pair ("LOCAL_FILES_ONLY",                2),
	std::make_pair ("REMOTE_FILES_ONLY",               3),
	std::make_pair ("UNTITLED_SCENS_ONLY",             4),
};

ExternalToolsEditor::ExternalToolsEditor (X3DBrowserWindow* const browserWindow) :
	               X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DExternalToolsEditorInterface (get_ui ("Editors/ExternalToolsEditor.glade")),
	         X3DExternalToolsEditor (),
	                       changing ()
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
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ExternalToolsEditor::on_remove_tool_clicked ()
{
	const auto iter = getTreeSelection () -> get_selected ();

	removeTool (iter);

	getTreeStore () -> erase (iter);

	saveTree ();
}

void
ExternalToolsEditor::on_tree_selection_changed ()
{
	try
	{
		changing = true;

		if (getTreeSelection () -> get_selected_rows () .empty ())
		{
			getRemoveToolButton () .set_sensitive (false);
			getToolBox () .set_sensitive (false);
			getSourceView () .get_source_buffer () -> set_text ("");
		}
		else
		{
			const auto iter              = getTreeSelection () -> get_selected ();
			const auto id                = getId (iter);
			const auto text              = getText (id);
			const auto saveType          = getSaveType (iter);
			const auto inputType         = getInputType (iter);
			const auto outputType        = getOutputType (iter);
			const auto applicabilityType = getApplicabilityType (iter);
		
			getRemoveToolButton () .set_sensitive (true);
			getToolBox () .set_sensitive (true);
			getSourceView () .get_source_buffer () -> set_text (text);

			try { getSaveTypeButton          () .set_active (saveTypes          .at (saveType));          } catch (...) { getSaveTypeButton          () .set_active (0); };
			try { getInputTypeButton         () .set_active (inputTypes         .at (inputType));         } catch (...) { getInputTypeButton         () .set_active (0); };
			try { getOutputTypeButton        () .set_active (outputTypes        .at (outputType));        } catch (...) { getOutputTypeButton        () .set_active (0); };
			try { getApplicabilityTypeButton () .set_active (applicabilityTypes .at (applicabilityType)); } catch (...) { getApplicabilityTypeButton () .set_active (0); };

			Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &ExternalToolsEditor::setLanguage), text));
		}

		changing = false;
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ExternalToolsEditor::on_name_edited (const Glib::ustring & path, const Glib::ustring & new_text)
{
	if (changing)
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
		if (changing)
			return;

		const auto iter = getTreeSelection () -> get_selected ();
		const auto id   = getId (iter);
		const auto text = getSourceView () .get_source_buffer () -> get_text ();
	
		setText (id, text);

		Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &ExternalToolsEditor::setLanguage), text));
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ExternalToolsEditor::on_save_type_changed ()
{
	if (changing)
		return;

	const auto iter = getTreeSelection () -> get_selected ();

	try
	{
		static const auto saveTypes = basic::reverse (this -> saveTypes);

		setSaveType (iter, saveTypes .at (getSaveTypeButton () .get_active_row_number ()));
	}
	catch (const std::out_of_range & error)
	{
		setSaveType (iter, "NOTHING");
	}

	saveTree ();
}

void
ExternalToolsEditor::on_input_type_changed ()
{
	if (changing)
		return;

	const auto iter = getTreeSelection () -> get_selected ();

	try
	{
		static const auto inputTypes = basic::reverse (this -> inputTypes);

		setInputType (iter, inputTypes .at (getInputTypeButton () .get_active_row_number ()));
	}
	catch (const std::out_of_range & error)
	{
		setInputType (iter, "NOTHING");
	}

	saveTree ();
}

void
ExternalToolsEditor::on_output_type_changed ()
{
	if (changing)
		return;

	const auto iter = getTreeSelection () -> get_selected ();

	try
	{
		static const auto outputTypes = basic::reverse (this -> outputTypes);

		setOutputType (iter, outputTypes .at (getOutputTypeButton () .get_active_row_number ()));
	}
	catch (const std::out_of_range & error)
	{
		setOutputType (iter, "NOTHING");
	}

	saveTree ();
}

void
ExternalToolsEditor::on_applicability_type_changed ()
{
	if (changing)
		return;

	const auto iter = getTreeSelection () -> get_selected ();

	try
	{
		static const auto applicabilityTypes = basic::reverse (this -> applicabilityTypes);

		setApplicabilityType (iter, applicabilityTypes .at (getApplicabilityTypeButton () .get_active_row_number ()));
	}
	catch (const std::out_of_range & error)
	{
		setApplicabilityType (iter, "ALWAYS_AVAILABLE");
	}

	saveTree ();
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
