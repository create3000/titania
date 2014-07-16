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

#include "TextEditor.h"

#include "../Configuration/config.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

enum FamilyEditorColumns
{
	FAMILY_NAME

};

TextEditor::TextEditor (BrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	    X3DTextEditorInterface (get_ui ("Dialogs/TextEditor.xml"), gconf_dir ()),
	                shapeNodes (),
	                      text (),
	             fontStyleNode (),
	                  undoStep (),
	                  changing (false)
{
	//  Drag & Drop

	getFamilyTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	getFamilyTreeView () .enable_model_drag_dest ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_MOVE);
}

void
TextEditor::initialize ()
{
	X3DTextEditorInterface::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &TextEditor::set_selection);

	set_selection ();
}

void
TextEditor::set_selection ()
{
	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .removeInterest (this, &TextEditor::set_text);

	undoStep .reset ();

	// Find Appearances.

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	shapeNodes .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     if (type == X3D::X3DConstants::X3DShapeNode)
	                     {
	                        shapeNodes .emplace_back (node);
	                        return true;
								}
							}

	                  return true;
						});

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .addInterest (this, &TextEditor::set_text);

	set_text ();
}

/***********************************************************************************************************************
 *
 *  Text
 *
 **********************************************************************************************************************/

void
TextEditor::on_text_toggled ()
{
	if (changing)
		return;
		
	getTextCheckButton () .set_inconsistent (false);
	getTextBox ()         .set_sensitive (getTextCheckButton () .get_active ());
	getFontStyleBox ()    .set_sensitive (getTextCheckButton () .get_active ());

	if (not getTextCheckButton () .get_active ())
	{
		text = text -> copy (text -> getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (text);
		getExecutionContext () -> realize ();
	}

	// Set field.

	addUndoFunction <X3D::SFNode> (shapeNodes, "geometry", undoStep);

	for (const auto & shapeNode : shapeNodes)
	{
		try
		{
			auto & field = shapeNode -> geometry ();

			if (getTextCheckButton () .get_active ())
				field = text;
			else
				field = nullptr;

			field .removeInterest (this, &TextEditor::set_text);
			field .addInterest (this, &TextEditor::connectText);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (shapeNodes, "geometry", undoStep);

	set_fontStyle ();
}

void
TextEditor::set_text ()
{
	if (text)
	{
		text -> string ()    .removeInterest (this, &TextEditor::set_string);
		text -> maxExtent () .removeInterest (this, &TextEditor::set_maxExtent);
		text -> fontStyle () .removeInterest (this, &TextEditor::set_fontStyle);
	}

	text = nullptr;

	const bool hasField = not shapeNodes .empty ();

	// Find last »geometry« field.

	int active = -1;

	for (const auto & shapeNode : basic::reverse_adapter (shapeNodes))
	{
		try
		{
			const X3D::X3DPtr <X3D::Text> field (shapeNode -> geometry ());
	
			if (active < 0)
			{
				text   = std::move (field);
				active = bool (text);
			}
			else if (field not_eq text)
			{
				if (not text)
					text = std::move (field);

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not text)
	{
		text = new X3D::Text (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (text);
		getExecutionContext () -> realize ();
	}

	changing = true;

	getTextCheckButton () .set_sensitive (hasField);
	getTextCheckButton () .set_active (active > 0);
	getTextCheckButton () .set_inconsistent (active < 0);

	getTextBox ()      .set_sensitive (active > 0);
	getFontStyleBox () .set_sensitive (active > 0);

	changing = false;

	text -> string ()    .addInterest (this, &TextEditor::set_string);
	text -> maxExtent () .addInterest (this, &TextEditor::set_maxExtent);
	text -> fontStyle () .addInterest (this, &TextEditor::set_fontStyle);

	set_string ();
	set_maxExtent ();
	set_fontStyle ();
}

void
TextEditor::connectText (const X3D::SFNode & field)
{
	field .removeInterest (this, &TextEditor::connectText);
	field .addInterest (this, &TextEditor::set_text);
}

/***********************************************************************************************************************
 *
 *  string
 *
 **********************************************************************************************************************/

void
TextEditor::on_string_changed ()
{
	if (changing)
		return;

	addUndoFunction (text, text -> string (), undoStep);

	text -> string () .clear ();

	const auto string = basic::split (getStringTextBuffer () -> get_text (), "\n");

	for (auto & value : string)
		text -> string () .emplace_back (std::move (value));

	text -> string () .removeInterest (this, &TextEditor::set_string);
	text -> string () .addInterest (this, &TextEditor::connectString);

	addRedoFunction (text -> string (), undoStep);
}

void
TextEditor::set_string ()
{
	changing = true;

	getStringTextBuffer () -> set_text ("");

	if (not text -> string () .empty ())
	{
		for (const auto & value : text -> string ())
		{
			getStringTextBuffer () -> insert (getStringTextBuffer () -> end (), value);
			getStringTextBuffer () -> insert (getStringTextBuffer () -> end (), "\n");
		}

		getStringTextBuffer () -> erase (-- getStringTextBuffer () -> end (), getStringTextBuffer () -> end ());
	}

	changing = false;
}

void
TextEditor::connectString (const X3D::MFString & field)
{
	field .removeInterest (this, &TextEditor::connectString);
	field .addInterest (this, &TextEditor::set_string);
}

// maxExtent

void
TextEditor::on_maxExtent_changed ()
{
	if (changing)
		return;

	addUndoFunction (text, text -> maxExtent (), undoStep);

	text -> maxExtent () = getMaxExtentSpinButton () .get_value ();

	text -> maxExtent () .removeInterest (this, &TextEditor::set_maxExtent);
	text -> maxExtent () .addInterest (this, &TextEditor::connectMaxExtent);

	addRedoFunction (text -> maxExtent (), undoStep);
}

void
TextEditor::set_maxExtent ()
{
	changing = true;

	getMaxExtentSpinButton () .set_value (text -> maxExtent ());

	changing = false;
}

void
TextEditor::connectMaxExtent (const X3D::SFFloat & field)
{
	field .removeInterest (this, &TextEditor::connectMaxExtent);
	field .addInterest (this, &TextEditor::set_maxExtent);
}

/***********************************************************************************************************************
 *
 *  fontStyle
 *
 **********************************************************************************************************************/

void
TextEditor::on_fontStyle_changed ()
{
	getFontStyleNodeBox () .set_sensitive (getFontStyleButton () .get_active_row_number () > 0);

	// Change size label.

	if (getFontStyleButton () .get_active_row_number () == 2)
		getSizeLabel () .set_text (_ ("Point Size:"));

	else
		getSizeLabel () .set_text (_ ("Size:"));

	setupGridLabels (getWidget ());

	if (changing)
		return;

	// Copy X3DFontStyleNode.

	if (getFontStyleButton () .get_active_row_number () > 0)
	{
		const auto tmp = fontStyleNode;

		switch (getFontStyleButton () .get_active_row_number ())
		{
			case 1:
			{
				fontStyleNode = new X3D::FontStyle (getExecutionContext ());
				break;
			}
			case 2:
			{
				fontStyleNode = new X3D::ScreenFontStyle (getExecutionContext ());
				break;
			}
			default:
				break;
		}

		fontStyleNode -> language ()    = tmp -> language ();
		fontStyleNode -> family ()      = tmp -> family ();
		fontStyleNode -> style ()       = tmp -> style ();
		fontStyleNode -> size ()        = tmp -> size ();
		fontStyleNode -> spacing ()     = tmp -> spacing ();
		fontStyleNode -> horizontal ()  = tmp -> horizontal ();
		fontStyleNode -> leftToRight () = tmp -> leftToRight ();
		fontStyleNode -> topToBottom () = tmp -> topToBottom ();
		fontStyleNode -> justify ()     = tmp -> justify ();

		getExecutionContext () -> addUninitializedNode (fontStyleNode);
		getExecutionContext () -> realize ();
	}

	// Set field.

	auto & field = text -> fontStyle ();
	
	addUndoFunction (text, field, undoStep);
	
	switch (getFontStyleButton () .get_active_row_number ())
	{
		case -1:
			break;
		case 0:
		{
			field = nullptr;
			break;
		}
		default:
		{
			field = fontStyleNode;
			break;
		}
	}

	field .removeInterest (this, &TextEditor::set_fontStyle);
	field .addInterest (this, &TextEditor::connectFontStyle);

	addRedoFunction (field, undoStep);
}

void
TextEditor::set_fontStyle ()
{
	if (fontStyleNode)
	{
		fontStyleNode -> family ()      .removeInterest (this, &TextEditor::set_family);
		fontStyleNode -> style ()       .removeInterest (this, &TextEditor::set_style);
		fontStyleNode -> size ()        .removeInterest (this, &TextEditor::set_size);
		fontStyleNode -> spacing ()     .removeInterest (this, &TextEditor::set_spacing);
		fontStyleNode -> horizontal ()  .removeInterest (this, &TextEditor::set_horizontal);
		fontStyleNode -> leftToRight () .removeInterest (this, &TextEditor::set_leftToRight);
		fontStyleNode -> topToBottom () .removeInterest (this, &TextEditor::set_topToBottom);
		fontStyleNode -> justify ()     .removeInterest (this, &TextEditor::set_justify);
	}

	changing = true;

	if (text -> fontStyle ())
	{
		switch (text -> fontStyle () -> getType () .back ())
		{
			case X3D::X3DConstants::FontStyle:
			{
				getFontStyleButton () .set_active (1);
				break;
			}
			case X3D::X3DConstants::ScreenFontStyle:
			{
				getFontStyleButton () .set_active (2);
				break;
			}
			default:
			{
				getFontStyleButton () .set_active (-1);
				break;
			}	
		}
	}
	else
		getFontStyleButton () .set_active (0);
	
	fontStyleNode = text -> fontStyle ();

	if (not fontStyleNode)
	{
		fontStyleNode = new X3D::FontStyle (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (fontStyleNode);
		getExecutionContext () -> realize ();
	}

	changing = false;
	
	fontStyleNode -> family ()      .addInterest (this, &TextEditor::set_family);
	fontStyleNode -> style ()       .addInterest (this, &TextEditor::set_style);
	fontStyleNode -> size ()        .addInterest (this, &TextEditor::set_size);
	fontStyleNode -> spacing ()     .addInterest (this, &TextEditor::set_spacing);
	fontStyleNode -> horizontal ()  .addInterest (this, &TextEditor::set_horizontal);
	fontStyleNode -> leftToRight () .addInterest (this, &TextEditor::set_leftToRight);
	fontStyleNode -> topToBottom () .addInterest (this, &TextEditor::set_topToBottom);
	fontStyleNode -> justify ()     .addInterest (this, &TextEditor::set_justify);
	
	set_family ();
	set_style ();
	set_size ();
	set_spacing ();
	set_horizontal ();
	set_leftToRight ();
	set_topToBottom ();
	set_justify ();
}

void
TextEditor::connectFontStyle (const X3D::SFNode & field)
{
	field .removeInterest (this, &TextEditor::connectFontStyle);
	field .addInterest (this, &TextEditor::set_fontStyle);
}

/***********************************************************************************************************************
 *
 *  family
 *
 **********************************************************************************************************************/

void
TextEditor::on_family_changed ()
{
	getRemoveFamilyButton () .set_sensitive (not getFamilySelection () -> get_selected_rows () .empty ());
}

void
TextEditor::on_family_edited (const Glib::ustring & path, const Glib::ustring & familyName)
{
	auto &     field = fontStyleNode -> family ();
	const auto value = familyName .empty () ? "SERIF" : familyName;

	if (*field .get1Value (Gtk::TreePath (path) .front ()) == value)
		return;

	// Update list store.
	
	const auto iter = getFamilyListStore () -> get_iter (Gtk::TreePath (path));
	iter -> set_value (FAMILY_NAME, value);

	// Change value.

	undoStep .reset ();

	addUndoFunction (fontStyleNode, field, undoStep);

	field .set1Value (Gtk::TreePath (path) .front (), value);

	field .removeInterest (this, &TextEditor::set_family);
	field .addInterest (this, &TextEditor::connectFamily);

	addRedoFunction (field, undoStep);
}

bool
TextEditor::on_family_button_release_event (GdkEventButton* event)
{
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn* column = nullptr;
	int                  cell_x = 0;
	int                  cell_y = 0;

	getFamilyTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (path .size ())
	{
		if (column == getFamilyFontColumn () .operator -> ())
		{
			openFontChooserDialog (path .front ());
			return true;
		}
	}

	return false;
}

void
TextEditor::on_family_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
                                          int x, int y,
                                          const Gtk::SelectionData & selection_data,
                                          guint info,
                                          guint time)
{
	const auto   selected = getFamilySelection () -> get_selected ();
	const size_t index    = getFamilyListStore () -> get_path (selected) .front ();

	// Update list store.

	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;
	size_t                    dest = 0;

	if (getFamilyTreeView () .get_dest_row_at_pos (x, y, destinationPath, position))
	{	
		auto destination = getFamilyListStore () -> get_iter (destinationPath);

		dest = getFamilyListStore () -> get_path (destination) .front ();

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
				getFamilyListStore () -> move (selected, destination);
				break;
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				getFamilyListStore () -> move (selected, ++ destination);
				++ dest;
				break;
		}
	}
	else
	{
		const auto children = getFamilyListStore () -> children ();
		getFamilyListStore () -> move (selected, children .end ());
		dest = children .size ();
	}

	// Move value.

	auto &     field = fontStyleNode -> family ();
	const auto value = field [index];

	undoStep .reset ();

	addUndoFunction (fontStyleNode, field, undoStep);

	if (index < dest)
	{
		field .insert (field .begin () + dest, value);
		field .erase (field .begin () + index);
	}
	else
	{
		field .erase (field .begin () + index);
		field .insert (field .begin () + dest, value);
	}

	field .removeInterest (this, &TextEditor::set_family);
	field .addInterest (this, &TextEditor::connectFamily);

	addRedoFunction (field, undoStep);

	context -> drag_finish (false, false, time);
}

void
TextEditor::on_add_family_clicked ()
{
	openFontChooserDialog (-1);
}

void
TextEditor::on_remove_family_clicked ()
{
	const auto selected = getFamilySelection () -> get_selected ();
	const size_t index  = getFamilyListStore () -> get_path (selected) .front ();
	auto &       field  = fontStyleNode -> family ();

	// Update list store.

	getFamilyListStore () -> erase (selected);
	
	// Remove value.

	undoStep .reset ();

	addUndoFunction (fontStyleNode, field, undoStep);

	field .erase (field .begin () + index);

	field .removeInterest (this, &TextEditor::set_family);
	field .addInterest (this, &TextEditor::connectFamily);

	addRedoFunction (field, undoStep);
}

void
TextEditor::openFontChooserDialog (const int index)
{
	// Create font description.

	std::string family;

	if (index == -1)
		family = "SERIF";

	else
	{
		const auto iter = getFamilyListStore () -> get_iter (Gtk::TreePath (basic::to_string (index)));
		iter -> get_value (FAMILY_NAME, family);
	}

	if (family .empty ())
		family = "SERIF";

	Pango::FontDescription fontDescription;

	fontDescription .set_family (family);
	
	// Run Dialog
	

	updateWidget ("FamilyChooserDialog");
	const auto familyChooserDialog = getWidget <Gtk::FontChooserDialog> ("FamilyChooserDialog");

	familyChooserDialog -> set_font_desc (fontDescription);

	getWindow () .set_sensitive (false);
	const auto response_id = familyChooserDialog -> run ();

	if (response_id == Gtk::RESPONSE_OK)
	{
		auto &     field           = fontStyleNode -> family ();
		const auto fontDescription = familyChooserDialog -> get_font_desc ();
		const auto value           = fontDescription .get_family ();
	
		if (index == -1)
		{
			// Update list store.

			const auto iter = getFamilyListStore () -> append ();
			iter -> set_value (FAMILY_NAME, value);

			// Append value.

			undoStep .reset ();
		
			addUndoFunction (fontStyleNode, field, undoStep);

			field .emplace_back (value);

			field .removeInterest (this, &TextEditor::set_family);
			field .addInterest (this, &TextEditor::connectFamily);

			addRedoFunction (field, undoStep);
		}
		else
		{
			// Update list store.

			const auto iter = getFamilyListStore () -> get_iter (Gtk::TreePath (basic::to_string (index)));
			iter -> set_value (FAMILY_NAME, value);

			// Change value.

			undoStep .reset ();

			addUndoFunction (fontStyleNode, field, undoStep);

			field .set1Value (index, value);

			field .removeInterest (this, &TextEditor::set_family);
			field .addInterest (this, &TextEditor::connectFamily);

			addRedoFunction (field, undoStep);
		}
	}

	delete familyChooserDialog;
	getWindow () .set_sensitive (true);
}

void
TextEditor::set_family ()
{
	getFamilyListStore () -> clear ();

	for (const auto & familyName : fontStyleNode -> family ())
	{
		const auto iter = getFamilyListStore () -> append ();
		iter -> set_value (FAMILY_NAME, familyName .str ());
	}
}

void
TextEditor::connectFamily (const X3D::MFString & field)
{
	field .removeInterest (this, &TextEditor::connectFamily);
	field .addInterest (this, &TextEditor::set_family);
}

/***********************************************************************************************************************
 *
 *  style
 *
 **********************************************************************************************************************/

void
TextEditor::on_style_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> style (), undoStep);

	switch ((getBoldToggleButton () .get_active () << 1) | getItalicToggleButton () .get_active ())
	{
		case 0:
		{
			fontStyleNode -> style () = "PLAIN";
			break;
		}
		case 1:
		{
			fontStyleNode -> style () = "ITALIC";
			break;
		}
		case 2:
		{
			fontStyleNode -> style () = "BOLD";
			break;
		}
		case 3:
		{
			fontStyleNode -> style () = "BOLDITALIC";
			break;
		}
		default:
			break;
	}

	fontStyleNode -> style () .removeInterest (this, &TextEditor::set_style);
	fontStyleNode -> style () .addInterest (this, &TextEditor::connectStyle);

	addRedoFunction (fontStyleNode -> style (), undoStep);
}

void
TextEditor::set_style ()
{
	changing = true;

	getBoldToggleButton ()   .set_active (fontStyleNode -> isBold ());
	getItalicToggleButton () .set_active (fontStyleNode -> isItalic ());

	changing = false;
}

void
TextEditor::connectStyle (const X3D::SFString & field)
{
	field .removeInterest (this, &TextEditor::connectStyle);
	field .addInterest (this, &TextEditor::set_style);
}

/***********************************************************************************************************************
 *
 *  size or pointSize
 *
 **********************************************************************************************************************/

void
TextEditor::on_size_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> size (), undoStep);

	fontStyleNode -> size () = getSizeSpinButton () .get_value ();

	fontStyleNode -> size () .removeInterest (this, &TextEditor::set_size);
	fontStyleNode -> size () .addInterest (this, &TextEditor::connectSize);

	addRedoFunction (fontStyleNode -> size (), undoStep);
}

void
TextEditor::set_size ()
{
	changing = true;

	getSizeSpinButton () .set_value (fontStyleNode -> size ());

	changing = false;
}

void
TextEditor::connectSize (const X3D::SFFloat & field)
{
	field .removeInterest (this, &TextEditor::connectSize);
	field .addInterest (this, &TextEditor::set_size);
}

/***********************************************************************************************************************
 *
 *  spacing
 *
 **********************************************************************************************************************/

void
TextEditor::on_spacing_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> spacing (), undoStep);

	fontStyleNode -> spacing () = getSpacingSpinButton () .get_value ();

	fontStyleNode -> spacing () .removeInterest (this, &TextEditor::set_spacing);
	fontStyleNode -> spacing () .addInterest (this, &TextEditor::connectSpacing);

	addRedoFunction (fontStyleNode -> spacing (), undoStep);
}

void
TextEditor::set_spacing ()
{
	changing = true;

	getSpacingSpinButton () .set_value (fontStyleNode -> spacing ());

	changing = false;
}

void
TextEditor::connectSpacing (const X3D::SFFloat & field)
{
	field .removeInterest (this, &TextEditor::connectSpacing);
	field .addInterest (this, &TextEditor::set_spacing);
}

/***********************************************************************************************************************
 *
 *  horizontal
 *
 **********************************************************************************************************************/

void
TextEditor::on_horizontal_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> horizontal (), undoStep);

	fontStyleNode -> horizontal () = getHorizontalCheckButton () .get_active ();

	fontStyleNode -> horizontal () .removeInterest (this, &TextEditor::set_horizontal);
	fontStyleNode -> horizontal () .addInterest (this, &TextEditor::connectHorizontal);

	addRedoFunction (fontStyleNode -> horizontal (), undoStep);
}

void
TextEditor::set_horizontal ()
{
	changing = true;

	getHorizontalCheckButton () .set_active (fontStyleNode -> horizontal ());

	changing = false;
}

void
TextEditor::connectHorizontal (const X3D::SFBool & field)
{
	field .removeInterest (this, &TextEditor::connectHorizontal);
	field .addInterest (this, &TextEditor::set_horizontal);
}

/***********************************************************************************************************************
 *
 *  leftToRight
 *
 **********************************************************************************************************************/

void
TextEditor::on_leftToRight_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> leftToRight (), undoStep);

	fontStyleNode -> leftToRight () = getLeftToRightCheckButton () .get_active ();

	fontStyleNode -> leftToRight () .removeInterest (this, &TextEditor::set_leftToRight);
	fontStyleNode -> leftToRight () .addInterest (this, &TextEditor::connectLeftToRight);

	addRedoFunction (fontStyleNode -> leftToRight (), undoStep);
}

void
TextEditor::set_leftToRight ()
{
	changing = true;

	getLeftToRightCheckButton () .set_active (fontStyleNode -> leftToRight ());

	changing = false;
}

void
TextEditor::connectLeftToRight (const X3D::SFBool & field)
{
	field .removeInterest (this, &TextEditor::connectLeftToRight);
	field .addInterest (this, &TextEditor::set_leftToRight);
}

/***********************************************************************************************************************
 *
 *  topToBottom
 *
 **********************************************************************************************************************/

void
TextEditor::on_topToBottom_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> topToBottom (), undoStep);

	fontStyleNode -> topToBottom () = getTopToBottomCheckButton () .get_active ();

	fontStyleNode -> topToBottom () .removeInterest (this, &TextEditor::set_topToBottom);
	fontStyleNode -> topToBottom () .addInterest (this, &TextEditor::connectTopToBottom);

	addRedoFunction (fontStyleNode -> topToBottom (), undoStep);
}

void
TextEditor::set_topToBottom ()
{
	changing = true;

	getTopToBottomCheckButton () .set_active (fontStyleNode -> topToBottom ());

	changing = false;
}

void
TextEditor::connectTopToBottom (const X3D::SFBool & field)
{
	field .removeInterest (this, &TextEditor::connectTopToBottom);
	field .addInterest (this, &TextEditor::set_topToBottom);
}

/***********************************************************************************************************************
 *
 *  justify
 *
 **********************************************************************************************************************/

void
TextEditor::on_justify_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> justify (), undoStep);

	fontStyleNode -> justify () .set1Value (0, getMajorAlignmentButton () .get_active_text ());
	fontStyleNode -> justify () .set1Value (1, getMinorAlignmentButton () .get_active_text ());

	fontStyleNode -> justify () .removeInterest (this, &TextEditor::set_justify);
	fontStyleNode -> justify () .addInterest (this, &TextEditor::connectJustify);

	addRedoFunction (fontStyleNode -> justify (), undoStep);
}

void
TextEditor::set_justify ()
{
	static const std::map <std::string, int> alignments = {
		std::make_pair ("FIRST",  0),
		std::make_pair ("BEGIN",  1),
		std::make_pair ("MIDDLE", 2),
		std::make_pair ("END",    3)
	};

	changing = true;

	try
	{
		getMajorAlignmentButton () .set_active (alignments .at (fontStyleNode -> justify () .at (0)));
	}
	catch (const std::out_of_range &)
	{
		getMajorAlignmentButton () .set_active (1);
	}

	try
	{
		getMinorAlignmentButton () .set_active (alignments .at (fontStyleNode -> justify () .at (1)));
	}
	catch (const std::out_of_range &)
	{
		getMinorAlignmentButton () .set_active (0);
	}

	changing = false;
}

void
TextEditor::connectJustify (const X3D::MFString & field)
{
	field .removeInterest (this, &TextEditor::connectJustify);
	field .addInterest (this, &TextEditor::set_justify);
}

TextEditor::~TextEditor ()
{ }

} // puck
} // titania
