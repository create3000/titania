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

#include "X3DFontStyleNodeEditor.h"

#include <Titania/String.h>

namespace titania {
namespace puck {

enum FamilyEditorColumns
{
	FAMILY_NAME

};

X3DFontStyleNodeEditor::X3DFontStyleNodeEditor () :
	X3DTextEditorInterface ("", ""),
	                 texts (),
	         fontStyleNode (),
	             fontStyle (),
	       screenFontStyle (),
	              undoStep (),
	              changing (false)
{
	//  Drag & Drop

	getFamilyTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	getFamilyTreeView () .enable_model_drag_dest ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_MOVE);
}

void
X3DFontStyleNodeEditor::initialize ()
{
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DFontStyleNodeEditor::set_selection);

	set_selection ();
}

void
X3DFontStyleNodeEditor::set_selection ()
{
	for (const auto & text : texts)
		text -> fontStyle () .removeInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);

	undoStep .reset ();

	// Find Appearances.

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	texts .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     if (type == X3D::X3DConstants::Text)
	                     {
	                        texts .emplace_back (node);
	                        return true;
								}
							}

	                  return true;
						});

	for (const auto & text : texts)
		text -> fontStyle () .addInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);

	set_fontStyle ();
}

/***********************************************************************************************************************
 *
 *  fontStyle
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_fontStyle_unlink_clicked ()
{
	unlinkClone (texts, "fontStyle", undoStep);
}

void
X3DFontStyleNodeEditor::on_fontStyle_changed ()
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
		const auto last = fontStyleNode;

		switch (getFontStyleButton () .get_active_row_number ())
		{
			case 1:
			{
				fontStyleNode = fontStyle;
				break;
			}
			case 2:
			{
				fontStyleNode = screenFontStyle;
				break;
			}
			default:
				break;
		}

		fontStyleNode -> language ()    = last -> language ();
		fontStyleNode -> family ()      = last -> family ();
		fontStyleNode -> style ()       = last -> style ();
		fontStyleNode -> size ()        = last -> size ();
		fontStyleNode -> spacing ()     = last -> spacing ();
		fontStyleNode -> horizontal ()  = last -> horizontal ();
		fontStyleNode -> leftToRight () = last -> leftToRight ();
		fontStyleNode -> topToBottom () = last -> topToBottom ();
		fontStyleNode -> justify ()     = last -> justify ();
	}

	// Set field.

	addUndoFunction <X3D::SFNode> (texts, "fontStyle", undoStep);

	for (const auto & text : texts)
	{
		try
		{
			auto & field = text -> fontStyle ();

			field .removeInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);
			field .addInterest (this, &X3DFontStyleNodeEditor::connectFontStyle);

			if (getFontStyleButton () .get_active_row_number () > 0)
				getBrowserWindow () -> replaceNode (X3D::SFNode (text), field, X3D::SFNode (fontStyleNode), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (text), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (texts, "fontStyle", undoStep);

	getFontStyleUnlinkButton () .set_sensitive (getFontStyleButton () .get_active () > 0 and fontStyleNode -> isCloned () > 1);
}

void
X3DFontStyleNodeEditor::set_fontStyle ()
{
	if (fontStyleNode)
	{
		fontStyleNode -> family ()      .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
		fontStyleNode -> style ()       .removeInterest (this, &X3DFontStyleNodeEditor::set_style);
		fontStyleNode -> size ()        .removeInterest (this, &X3DFontStyleNodeEditor::set_size);
		fontStyleNode -> spacing ()     .removeInterest (this, &X3DFontStyleNodeEditor::set_spacing);
		fontStyleNode -> horizontal ()  .removeInterest (this, &X3DFontStyleNodeEditor::set_horizontal);
		fontStyleNode -> leftToRight () .removeInterest (this, &X3DFontStyleNodeEditor::set_leftToRight);
		fontStyleNode -> topToBottom () .removeInterest (this, &X3DFontStyleNodeEditor::set_topToBottom);
		fontStyleNode -> justify ()     .removeInterest (this, &X3DFontStyleNodeEditor::set_justify);
	}

	auto       pair     = getNode <X3D::X3DFontStyleNode> (texts, "fontStyle");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	fontStyleNode = std::move (pair .first);

	if (not fontStyleNode)
	{
		fontStyleNode = new X3D::FontStyle (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (fontStyleNode);
		getExecutionContext () -> realize ();
	}

	fontStyle       = fontStyleNode;
	screenFontStyle = fontStyleNode;

	if (not fontStyle)
	{
		fontStyle = new X3D::FontStyle (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (fontStyle);
		getExecutionContext () -> realize ();
	}

	if (not screenFontStyle)
	{
		screenFontStyle = new X3D::ScreenFontStyle (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (screenFontStyle);
		getExecutionContext () -> realize ();
	}

	changing = true;

	if (active > 0)
	{
		switch (fontStyleNode -> getType () .back ())
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
				break;
		}
	}
	else if (active == 0)
		getFontStyleButton () .set_active (0);
	else
		getFontStyleButton () .set_active (-1);

	getFontStyleButton ()       .set_sensitive (hasField);
	getFontStyleUnlinkButton () .set_sensitive (active > 0 and fontStyleNode -> isCloned () > 1);

	changing = false;

	fontStyleNode -> family ()      .addInterest (this, &X3DFontStyleNodeEditor::set_family);
	fontStyleNode -> style ()       .addInterest (this, &X3DFontStyleNodeEditor::set_style);
	fontStyleNode -> size ()        .addInterest (this, &X3DFontStyleNodeEditor::set_size);
	fontStyleNode -> spacing ()     .addInterest (this, &X3DFontStyleNodeEditor::set_spacing);
	fontStyleNode -> horizontal ()  .addInterest (this, &X3DFontStyleNodeEditor::set_horizontal);
	fontStyleNode -> leftToRight () .addInterest (this, &X3DFontStyleNodeEditor::set_leftToRight);
	fontStyleNode -> topToBottom () .addInterest (this, &X3DFontStyleNodeEditor::set_topToBottom);
	fontStyleNode -> justify ()     .addInterest (this, &X3DFontStyleNodeEditor::set_justify);

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
X3DFontStyleNodeEditor::connectFontStyle (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectFontStyle);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);
}

/***********************************************************************************************************************
 *
 *  family
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_family_changed ()
{
	getRemoveFamilyButton () .set_sensitive (not getFamilySelection () -> get_selected_rows () .empty ());
}

void
X3DFontStyleNodeEditor::on_family_edited (const Glib::ustring & path, const Glib::ustring & familyName)
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

	field .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
	field .addInterest (this, &X3DFontStyleNodeEditor::connectFamily);

	field .set1Value (Gtk::TreePath (path) .front (), value);

	addRedoFunction (field, undoStep);
}

bool
X3DFontStyleNodeEditor::on_family_button_release_event (GdkEventButton* event)
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
X3DFontStyleNodeEditor::on_family_drag_data_received (const Glib::RefPtr <Gdk::DragContext> & context,
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

	field .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
	field .addInterest (this, &X3DFontStyleNodeEditor::connectFamily);

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

	addRedoFunction (field, undoStep);

	context -> drag_finish (false, false, time);
}

void
X3DFontStyleNodeEditor::on_add_family_clicked ()
{
	openFontChooserDialog (-1);
}

void
X3DFontStyleNodeEditor::on_remove_family_clicked ()
{
	const auto   selected = getFamilySelection () -> get_selected ();
	const size_t index    = getFamilyListStore () -> get_path (selected) .front ();
	auto &       field    = fontStyleNode -> family ();

	// Update list store.

	getFamilyListStore () -> erase (selected);

	// Remove value.

	undoStep .reset ();

	addUndoFunction (fontStyleNode, field, undoStep);

	field .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
	field .addInterest (this, &X3DFontStyleNodeEditor::connectFamily);

	field .erase (field .begin () + index);

	addRedoFunction (field, undoStep);
}

void
X3DFontStyleNodeEditor::openFontChooserDialog (const int index)
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

			field .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
			field .addInterest (this, &X3DFontStyleNodeEditor::connectFamily);

			field .emplace_back (value);

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

			field .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
			field .addInterest (this, &X3DFontStyleNodeEditor::connectFamily);

			field .set1Value (index, value);

			addRedoFunction (field, undoStep);
		}
	}

	delete familyChooserDialog;
	getWindow () .set_sensitive (true);
}

void
X3DFontStyleNodeEditor::set_family ()
{
	getFamilyListStore () -> clear ();

	for (const auto & familyName : fontStyleNode -> family ())
	{
		const auto iter = getFamilyListStore () -> append ();
		iter -> set_value (FAMILY_NAME, familyName .str ());
	}
}

void
X3DFontStyleNodeEditor::connectFamily (const X3D::MFString & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectFamily);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_family);
}

/***********************************************************************************************************************
 *
 *  style
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_style_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> style (), undoStep);

	fontStyleNode -> style () .removeInterest (this, &X3DFontStyleNodeEditor::set_style);
	fontStyleNode -> style () .addInterest (this, &X3DFontStyleNodeEditor::connectStyle);

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

	addRedoFunction (fontStyleNode -> style (), undoStep);
}

void
X3DFontStyleNodeEditor::set_style ()
{
	changing = true;

	getBoldToggleButton ()   .set_active (fontStyleNode -> isBold ());
	getItalicToggleButton () .set_active (fontStyleNode -> isItalic ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectStyle (const X3D::SFString & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectStyle);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_style);
}

/***********************************************************************************************************************
 *
 *  size or pointSize
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_size_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> size (), undoStep);

	fontStyleNode -> size () .removeInterest (this, &X3DFontStyleNodeEditor::set_size);
	fontStyleNode -> size () .addInterest (this, &X3DFontStyleNodeEditor::connectSize);

	fontStyleNode -> size () = getSizeSpinButton () .get_value ();

	addRedoFunction (fontStyleNode -> size (), undoStep);
}

void
X3DFontStyleNodeEditor::set_size ()
{
	changing = true;

	getSizeSpinButton () .set_value (fontStyleNode -> size ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectSize (const X3D::SFFloat & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectSize);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_size);
}

/***********************************************************************************************************************
 *
 *  spacing
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_spacing_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> spacing (), undoStep);

	fontStyleNode -> spacing () .removeInterest (this, &X3DFontStyleNodeEditor::set_spacing);
	fontStyleNode -> spacing () .addInterest (this, &X3DFontStyleNodeEditor::connectSpacing);

	fontStyleNode -> spacing () = getSpacingSpinButton () .get_value ();

	addRedoFunction (fontStyleNode -> spacing (), undoStep);
}

void
X3DFontStyleNodeEditor::set_spacing ()
{
	changing = true;

	getSpacingSpinButton () .set_value (fontStyleNode -> spacing ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectSpacing (const X3D::SFFloat & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectSpacing);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_spacing);
}

/***********************************************************************************************************************
 *
 *  horizontal
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_horizontal_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> horizontal (), undoStep);

	fontStyleNode -> horizontal () .removeInterest (this, &X3DFontStyleNodeEditor::set_horizontal);
	fontStyleNode -> horizontal () .addInterest (this, &X3DFontStyleNodeEditor::connectHorizontal);

	fontStyleNode -> horizontal () = getHorizontalCheckButton () .get_active ();

	addRedoFunction (fontStyleNode -> horizontal (), undoStep);
}

void
X3DFontStyleNodeEditor::set_horizontal ()
{
	changing = true;

	getHorizontalCheckButton () .set_active (fontStyleNode -> horizontal ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectHorizontal (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectHorizontal);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_horizontal);
}

/***********************************************************************************************************************
 *
 *  leftToRight
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_leftToRight_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> leftToRight (), undoStep);

	fontStyleNode -> leftToRight () .removeInterest (this, &X3DFontStyleNodeEditor::set_leftToRight);
	fontStyleNode -> leftToRight () .addInterest (this, &X3DFontStyleNodeEditor::connectLeftToRight);

	fontStyleNode -> leftToRight () = getLeftToRightCheckButton () .get_active ();

	addRedoFunction (fontStyleNode -> leftToRight (), undoStep);
}

void
X3DFontStyleNodeEditor::set_leftToRight ()
{
	changing = true;

	getLeftToRightCheckButton () .set_active (fontStyleNode -> leftToRight ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectLeftToRight (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectLeftToRight);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_leftToRight);
}

/***********************************************************************************************************************
 *
 *  topToBottom
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_topToBottom_toggled ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> topToBottom (), undoStep);

	fontStyleNode -> topToBottom () .removeInterest (this, &X3DFontStyleNodeEditor::set_topToBottom);
	fontStyleNode -> topToBottom () .addInterest (this, &X3DFontStyleNodeEditor::connectTopToBottom);

	fontStyleNode -> topToBottom () = getTopToBottomCheckButton () .get_active ();

	addRedoFunction (fontStyleNode -> topToBottom (), undoStep);
}

void
X3DFontStyleNodeEditor::set_topToBottom ()
{
	changing = true;

	getTopToBottomCheckButton () .set_active (fontStyleNode -> topToBottom ());

	changing = false;
}

void
X3DFontStyleNodeEditor::connectTopToBottom (const X3D::SFBool & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectTopToBottom);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_topToBottom);
}

/***********************************************************************************************************************
 *
 *  justify
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_justify_changed ()
{
	if (changing)
		return;

	addUndoFunction (fontStyleNode, fontStyleNode -> justify (), undoStep);

	fontStyleNode -> justify () .removeInterest (this, &X3DFontStyleNodeEditor::set_justify);
	fontStyleNode -> justify () .addInterest (this, &X3DFontStyleNodeEditor::connectJustify);

	fontStyleNode -> justify () .set1Value (0, getMajorAlignmentButton () .get_active_text ());
	fontStyleNode -> justify () .set1Value (1, getMinorAlignmentButton () .get_active_text ());

	addRedoFunction (fontStyleNode -> justify (), undoStep);
}

void
X3DFontStyleNodeEditor::set_justify ()
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
X3DFontStyleNodeEditor::connectJustify (const X3D::MFString & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectJustify);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_justify);
}

} // puck
} // titania
