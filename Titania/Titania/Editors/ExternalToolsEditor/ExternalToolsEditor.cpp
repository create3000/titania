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

// Implement icon for menu items, related functions:
// const auto iconTheme = Gtk::IconTheme::get_default ();
// const auto contexts  = iconTheme -> list_contexts ();
// const auto iconNames = iconTheme -> list_icons (contexts [i]);

const std::map <std::string, int32_t> ExternalToolsEditor::saveTypes = {
	std::make_pair ("NOTHING",       0),
	std::make_pair ("CURRENT_SCENE", 1),
	std::make_pair ("ALL_SCENES",    2),
};

const std::map <std::string, int32_t> ExternalToolsEditor::inputTypes = {
	std::make_pair ("NOTHING",       0),
	std::make_pair ("CURRENT_SCENE", 1),
	std::make_pair ("SELECTION",     2),
};

const std::map <std::string, int32_t> ExternalToolsEditor::inputEncodings = {
	std::make_pair ("XML",  0),
	std::make_pair ("VRML", 1),
	std::make_pair ("JSON", 2),
};

const std::map <std::string, int32_t> ExternalToolsEditor::outputTypes = {
	std::make_pair ("NOTHING",                  0),
	std::make_pair ("DISPLAY_IN_CONSOLE",       1),
	std::make_pair ("CREATE_NEW_SCENE",         2),
	std::make_pair ("REPLACE_CURRENT_SCENE",    3),
	std::make_pair ("APPEND_TO_CURRENT_SCENE",  4),
	std::make_pair ("APPEND_TO_CURRENT_LAYER",  5),
	std::make_pair ("REPLACE_SELECTION",        6),
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
	                           keys (),
	                       changing ()
{
	getSourceView () .get_source_buffer () -> signal_changed () .connect (sigc::mem_fun (this, &ExternalToolsEditor::on_text_changed));
	getSourceView () .get_source_buffer () -> set_style_scheme (Gsv::StyleSchemeManager::get_default () -> get_scheme ("x_ite"));

	getNameRenderer () -> signal_edited () .connect (sigc::mem_fun (this, &ExternalToolsEditor::on_name_edited));
	getNameRenderer () -> property_editable () = true;

	getTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_COPY);
	getTreeView () .enable_model_drag_dest   ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_COPY);

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
	
		setId (iter, id);
		setName (iter, _ ("New Tool"));
		setOutputType (iter, "DISPLAY_IN_CONSOLE");
		setText (id, "#!/bin/sh\n");
		saveTree ();

		// First grab focus to release any previous selected field.
		getTreeView () .grab_focus ();
		getTreeView () .set_cursor (getTreeStore () -> get_path (iter), *getNameColumn () .operator -> (), true);
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
ExternalToolsEditor::on_row_expanded (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	setExpanded (iter, true);

	// Auto expand children.

	for (const auto & child : iter -> children ())
	{
		if (getExpanded (child))
			getTreeView () .expand_row (getTreeStore () -> get_path (child), false);
	}

	saveTree ();
}

void
ExternalToolsEditor::on_row_collapsed (const Gtk::TreeIter & iter, const Gtk::TreePath & path)
{
	setExpanded (iter, false);
	saveTree ();
}

void
ExternalToolsEditor::on_tree_selection_changed ()
{
	try
	{
		changing = true;

		getSourceView () .get_source_buffer () -> begin_not_undoable_action ();

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
			const auto shortcutKey       = getShortcutKey (iter);
			const auto saveType          = getSaveType (iter);
			const auto inputType         = getInputType (iter);
			const auto inputEncoding     = getInputEncoding (iter);
			const auto outputType        = getOutputType (iter);
			const auto applicabilityType = getApplicabilityType (iter);
		
			getRemoveToolButton () .set_sensitive (true);
			getToolBox () .set_sensitive (true);
			getSourceView () .get_source_buffer () -> set_text (text);

			// Shortcut Key

			guint key;
			Gdk::ModifierType modifiers;

			Gtk::AccelGroup::parse (shortcutKey, key, modifiers);

			if (key)
				getShortcutKeyEntry () .set_text (Gtk::AccelGroup::get_label (key, modifiers));
			else
				getShortcutKeyEntry () .set_text ("");

			// //

			try { getSaveTypeButton          () .set_active (saveTypes          .at (saveType));          } catch (...) { getSaveTypeButton          () .set_active (0); };
			try { getInputTypeButton         () .set_active (inputTypes         .at (inputType));         } catch (...) { getInputTypeButton         () .set_active (0); };
			try { getInputEncodingButton     () .set_active (inputEncodings     .at (inputEncoding));     } catch (...) { getInputEncodingButton     () .set_active (0); };
			try { getOutputTypeButton        () .set_active (outputTypes        .at (outputType));        } catch (...) { getOutputTypeButton        () .set_active (0); };
			try { getApplicabilityTypeButton () .set_active (applicabilityTypes .at (applicabilityType)); } catch (...) { getApplicabilityTypeButton () .set_active (0); };

			Glib::signal_idle () .connect_once (sigc::bind (sigc::mem_fun (this, &ExternalToolsEditor::setLanguage), text));
		}

		getSourceView () .get_source_buffer () -> end_not_undoable_action ();

		changing = false;
	}
	catch (const Glib::Error & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
ExternalToolsEditor::on_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                            int x, int y,
                                            const Gtk::SelectionData & selection_data,
                                            guint info,
                                            guint time)
{
	const auto selected = getTreeSelection () -> get_selected ();
	const auto expanded = getTreeView () .row_expanded (getTreeStore () -> get_path (selected));

	// Update list store.

	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;

	if (getTreeView () .get_dest_row_at_pos (x, y, destinationPath, position))
	{
		auto destination = getTreeStore () -> get_iter (destinationPath);
		auto child       = Gtk::TreeIter ();

		// Create new iter at end of level.

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_AFTER:
			{
				if (getTreeStore () -> iter_depth (destination))
					child = getTreeStore () -> append (destination -> parent () -> children ());
				else
					child = getTreeStore () -> append ();
				break;
			}
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
			{
				child = getTreeStore () -> append (destination -> children ());
				break;
			}
		}

		// Assign selected iter to newly created iter.

		assignIter (child, selected);

		// Move newly created iter to desired position.

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
				getTreeStore () -> move (child, destination);
				break;
			case Gtk::TREE_VIEW_DROP_AFTER:
				getTreeStore () -> move (child, ++ destination);
				break;
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				getTreeView () .expand_row (destinationPath, false);
				break;
		}

		getTreeStore () -> erase (selected);

		if (expanded)
			getTreeView () .expand_row (getTreeStore () -> get_path (child), false);

		getTreeSelection () -> select (child);
	}
	else
	{
		const auto child = getTreeStore () -> append ();

		assignIter (child, selected);

		getTreeStore () -> erase (selected);

		if (expanded)
			getTreeView () .expand_row (getTreeStore () -> get_path (child), false);
	
		getTreeSelection () -> select (child);
	}

	saveTree ();

	context -> drag_finish (true, false, time);
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

bool
ExternalToolsEditor::on_shortcut_key_press_event (GdkEventKey* event)
{
	if (changing)
		return true;

	const auto iter = getTreeSelection () -> get_selected ();

	keys .press (event);

	Gdk::ModifierType modifiers = Gdk::ModifierType (0);
	std::string shortcutKey;

	if (keys .control ())
		modifiers |= Gdk::CONTROL_MASK;

	if (keys .shift ())
		modifiers |= Gdk::SHIFT_MASK;

	if (keys .alt ())
		modifiers |= Gdk::MOD1_MASK;

	if (modifiers == Gdk::ModifierType (0) and event -> keyval == GDK_KEY_Delete)
	{
		if (getShortcutKeyEntry () .get_text () == "")
			return true;

		getShortcutKeyEntry () .set_text ("");
		setShortcutKey (iter, "");
		saveTree ();
		return true;
	}

	const auto key = gdk_keyval_to_unicode (event -> keyval);

	if (key)
		shortcutKey = Gtk::AccelGroup::name (event -> keyval, modifiers);

	if (shortcutKey .empty ())
		return true;

	const auto label = Gtk::AccelGroup::get_label (event -> keyval, modifiers);

	if (getShortcutKeyEntry () .get_text () == label)
		return true;

	getShortcutKeyEntry () .set_text (label);
	setShortcutKey (iter, shortcutKey);
	saveTree ();
	return true;
}

bool
ExternalToolsEditor::on_shortcut_key_release_event (GdkEventKey* event)
{
	keys .release (event);
	return true;
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
	getInputEncodingButton () .set_sensitive (getInputTypeButton () .get_active_row_number () not_eq 0);

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
ExternalToolsEditor::on_input_format_changed ()
{
	if (changing)
		return;

	const auto iter = getTreeSelection () -> get_selected ();

	try
	{
		static const auto inputEncodings = basic::reverse (this -> inputEncodings);

		setInputEncoding (iter, inputEncodings .at (getInputEncodingButton () .get_active_row_number ()));
	}
	catch (const std::out_of_range & error)
	{
		setInputEncoding (iter, "XML");
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
