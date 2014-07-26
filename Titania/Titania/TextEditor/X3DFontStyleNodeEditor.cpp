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
	   fontStyleNodeBuffer (),
	         fontStyleNode (),
	             fontStyle (),
	       screenFontStyle (),
	              undoStep (),
	              changing (false),
	               spacing (getBrowserWindow (), getFontStyleSpacingAdjustment (), getFontStyleSpacingSpinButton (), "spacing"),
	            horizontal (getBrowserWindow (), getFontStyleHorizontalCheckButton (),  "horizontal"),
	           leftToRight (getBrowserWindow (), getFontStyleLeftToRightCheckButton (), "leftToRight"),
	           topToBottom (getBrowserWindow (), getFontStyleTopToBottomCheckButton (), "topToBottom")
{
	fontStyleNodeBuffer .addParent (getBrowser ());
	fontStyleNodeBuffer .addInterest (this, &X3DFontStyleNodeEditor::set_node);

	//  Drag & Drop

	getFontStyleFamilyTreeView () .enable_model_drag_source ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::BUTTON1_MASK, Gdk::ACTION_MOVE);
	getFontStyleFamilyTreeView () .enable_model_drag_dest ({ Gtk::TargetEntry ("STRING", Gtk::TARGET_SAME_WIDGET) }, Gdk::ACTION_MOVE);
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
	undoStep .reset ();

	for (const auto & text : texts)
		text -> fontStyle () .removeInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);

	texts = getSelection <X3D::Text> ({ X3D::X3DConstants::Text });

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
	getFontStyleNodeBox () .set_sensitive (getFontStyleComboBoxText () .get_active_row_number () > 0);

	// Change size label.

	if (getFontStyleComboBoxText () .get_active_row_number () == 2)
		getSizeLabel () .set_text (_ ("Point Size:"));

	else
		getSizeLabel () .set_text (_ ("Size:"));

	setupGridLabels (getWidget ());

	if (changing)
		return;

	// Copy X3DFontStyleNode.

	if (getFontStyleComboBoxText () .get_active_row_number () > 0)
	{
		const auto last = fontStyleNode;

		switch (getFontStyleComboBoxText () .get_active_row_number ())
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

			if (getFontStyleComboBoxText () .get_active_row_number () > 0)
				getBrowserWindow () -> replaceNode (X3D::SFNode (text), field, X3D::SFNode (fontStyleNode), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (text), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (texts, "fontStyle", undoStep);

	getFontStyleUnlinkButton () .set_sensitive (getFontStyleComboBoxText () .get_active () > 0 and fontStyleNode -> getCloneCount () > 1);
}

void
X3DFontStyleNodeEditor::set_fontStyle ()
{
	fontStyleNodeBuffer .addEvent ();
}

void
X3DFontStyleNodeEditor::set_node ()
{
	if (fontStyleNode)
	{
		fontStyleNode -> family ()      .removeInterest (this, &X3DFontStyleNodeEditor::set_family);
		fontStyleNode -> style ()       .removeInterest (this, &X3DFontStyleNodeEditor::set_style);
		fontStyleNode -> size ()        .removeInterest (this, &X3DFontStyleNodeEditor::set_size);
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
				getFontStyleComboBoxText () .set_active (1);
				break;
			}
			case X3D::X3DConstants::ScreenFontStyle:
			{
				getFontStyleComboBoxText () .set_active (2);
				break;
			}
			default:
				break;
		}
	}
	else if (active == 0)
		getFontStyleComboBoxText () .set_active (0);
	else
		getFontStyleComboBoxText () .set_active (-1);

	getFontStyleComboBoxText () .set_sensitive (hasField);
	getFontStyleUnlinkButton () .set_sensitive (active > 0 and fontStyleNode -> getCloneCount () > 1);

	changing = false;

	fontStyleNode -> family ()      .addInterest (this, &X3DFontStyleNodeEditor::set_family);
	fontStyleNode -> style ()       .addInterest (this, &X3DFontStyleNodeEditor::set_style);
	fontStyleNode -> size ()        .addInterest (this, &X3DFontStyleNodeEditor::set_size);
	fontStyleNode -> justify ()     .addInterest (this, &X3DFontStyleNodeEditor::set_justify);

	set_family ();
	set_style ();
	set_size ();
	set_justify ();

	spacing     .setNodes ({ fontStyleNode });
	horizontal  .setNodes ({ fontStyleNode });
	leftToRight .setNodes ({ fontStyleNode });
	topToBottom .setNodes ({ fontStyleNode });
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
	getFontStyleRemoveFamilyButton () .set_sensitive (not getFontStyleFamilySelection () -> get_selected_rows () .empty ());
}

void
X3DFontStyleNodeEditor::on_family_edited (const Glib::ustring & path, const Glib::ustring & familyName)
{
	auto &     field = fontStyleNode -> family ();
	const auto value = familyName .empty () ? "SERIF" : familyName;

	if (field .get1Value (Gtk::TreePath (path) .front ()) == value)
		return;

	// Update list store.

	const auto iter = getFontStyleFamilyListStore () -> get_iter (Gtk::TreePath (path));
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

	getFontStyleFamilyTreeView () .get_path_at_pos (event -> x, event -> y, path, column, cell_x, cell_y);

	if (path .size ())
	{
		if (column == getFontStyleFamilyFontColumn () .operator -> ())
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
	const auto   selected = getFontStyleFamilySelection () -> get_selected ();
	const size_t index    = getFontStyleFamilyListStore () -> get_path (selected) .front ();

	// Update list store.

	Gtk::TreeModel::Path      destinationPath;
	Gtk::TreeViewDropPosition position;
	size_t                    dest = 0;

	if (getFontStyleFamilyTreeView () .get_dest_row_at_pos (x, y, destinationPath, position))
	{
		auto destination = getFontStyleFamilyListStore () -> get_iter (destinationPath);

		dest = getFontStyleFamilyListStore () -> get_path (destination) .front ();

		switch (position)
		{
			case Gtk::TREE_VIEW_DROP_BEFORE:
			case Gtk::TREE_VIEW_DROP_INTO_OR_BEFORE:
				getFontStyleFamilyListStore () -> move (selected, destination);
				break;
			case Gtk::TREE_VIEW_DROP_AFTER:
			case Gtk::TREE_VIEW_DROP_INTO_OR_AFTER:
				getFontStyleFamilyListStore () -> move (selected, ++ destination);
				++ dest;
				break;
		}
	}
	else
	{
		const auto children = getFontStyleFamilyListStore () -> children ();
		getFontStyleFamilyListStore () -> move (selected, children .end ());
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
	const auto   selected = getFontStyleFamilySelection () -> get_selected ();
	const size_t index    = getFontStyleFamilyListStore () -> get_path (selected) .front ();
	auto &       field    = fontStyleNode -> family ();

	// Update list store.

	getFontStyleFamilyListStore () -> erase (selected);

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
		const auto iter = getFontStyleFamilyListStore () -> get_iter (Gtk::TreePath (basic::to_string (index)));
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

			const auto iter = getFontStyleFamilyListStore () -> append ();
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

			const auto iter = getFontStyleFamilyListStore () -> get_iter (Gtk::TreePath (basic::to_string (index)));
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
	getFontStyleFamilyListStore () -> clear ();

	for (const auto & familyName : fontStyleNode -> family ())
	{
		const auto iter = getFontStyleFamilyListStore () -> append ();
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

	switch ((getFontStyleBoldToggleButton () .get_active () << 1) | getFontStyleItalicToggleButton () .get_active ())
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

	getFontStyleBoldToggleButton ()   .set_active (fontStyleNode -> isBold ());
	getFontStyleItalicToggleButton () .set_active (fontStyleNode -> isItalic ());

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

	fontStyleNode -> size () = getFontStyleSizeAdjustment () -> get_value ();

	addRedoFunction (fontStyleNode -> size (), undoStep);
}

void
X3DFontStyleNodeEditor::set_size ()
{
	changing = true;

	getFontStyleSizeAdjustment () -> set_value (fontStyleNode -> size ());

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

	fontStyleNode -> justify () .set1Value (0, getFontStyleMajorAlignmentComboBoxText () .get_active_text ());
	fontStyleNode -> justify () .set1Value (1, getFontStyleMinorAlignmentComboBoxText () .get_active_text ());

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
		getFontStyleMajorAlignmentComboBoxText () .set_active (alignments .at (fontStyleNode -> justify () .at (0)));
	}
	catch (const std::out_of_range &)
	{
		getFontStyleMajorAlignmentComboBoxText () .set_active (1);
	}

	try
	{
		getFontStyleMinorAlignmentComboBoxText () .set_active (alignments .at (fontStyleNode -> justify () .at (1)));
	}
	catch (const std::out_of_range &)
	{
		getFontStyleMinorAlignmentComboBoxText () .set_active (0);
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
