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

TextEditor::TextEditor (BrowserWindow* const browserWindow) :
	          X3DBaseInterface (browserWindow, browserWindow -> getBrowser ()),
	    X3DTextEditorInterface (get_ui ("Dialogs/TextEditor.xml"), gconf_dir ()),
	                shapeNodes (),
	                      text (),
	             fontStyleNode (),
	               familyIndex (-1),
	             familyEntries (),
	                  undoStep (),
	                  changing (false)
{ }

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

// Text

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

// string

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

// fontStyle

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
		fontStyleNode -> family () .removeInterest (this, &TextEditor::set_family);
		fontStyleNode -> size ()   .removeInterest (this, &TextEditor::set_size);
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
	
	fontStyleNode -> family () .addInterest (this, &TextEditor::set_family);
	fontStyleNode -> size ()   .addInterest (this, &TextEditor::set_size);
	
	set_family ();
	set_size ();
}

void
TextEditor::connectFontStyle (const X3D::SFNode & field)
{
	field .removeInterest (this, &TextEditor::connectFontStyle);
	field .addInterest (this, &TextEditor::set_fontStyle);
}

// family

void
TextEditor::on_family_changed (const Gtk::Entry* const entry)
{
	const auto iter = std::find (familyEntries .begin (), familyEntries .end (), entry);

	if (iter == familyEntries .end ())
		return;

	const auto index = iter - familyEntries .begin ();
	
	addUndoFunction (fontStyleNode, fontStyleNode -> family (), undoStep);

	fontStyleNode -> family () [index] = entry -> get_text ();

	fontStyleNode -> family () .removeInterest (this, &TextEditor::set_family);
	fontStyleNode -> family () .addInterest (this, &TextEditor::connectFamily);

	addRedoFunction (fontStyleNode -> family (), undoStep);
}

void
TextEditor::set_family ()
{
	familyEntries .clear ();

	for (const auto & widget : getFamilyBox () .get_children ())
		getFamilyBox () .remove (*widget);

	for (const auto & value : fontStyleNode -> family ())
	{
		updateWidget ("FamilyWidget");
		
		const auto familyWidget = getWidget <Gtk::Box> ("FamilyWidget");
		const auto entry        = getWidget <Gtk::Entry> ("FamilyEntryX");

		entry -> signal_changed () .connect (sigc::bind (sigc::mem_fun (this, &TextEditor::on_family_changed), entry));

		entry -> set_text (value);
		familyEntries .emplace_back (entry);

		getWidget <Gtk::Button> ("FontChooserButtonX")  -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &TextEditor::on_fontChooserButtonX_clicked), entry));
		getWidget <Gtk::Button> ("RemoveFamilyButtonX") -> signal_clicked () .connect (sigc::bind (sigc::mem_fun (this, &TextEditor::on_removeFamilyX_clicked), entry));

		familyWidget -> show ();
		getFamilyBox () .pack_start (*familyWidget, true, false);
	}
}

void
TextEditor::connectFamily (const X3D::MFString & field)
{
	field .removeInterest (this, &TextEditor::connectFamily);
	field .addInterest (this, &TextEditor::set_family);
}

void
TextEditor::on_fontChooserButtonX_clicked (const Gtk::Entry* const entry)
{
	const auto iter = std::find (familyEntries .begin (), familyEntries .end (), entry);

	if (iter == familyEntries .end ())
		return;

	openFontChooserDialog (iter - familyEntries .begin ());
}

void
TextEditor::on_removeFamilyX_clicked (const Gtk::Entry* const entry)
{
	const auto iter = std::find (familyEntries .begin (), familyEntries .end (), entry);

	if (iter == familyEntries .end ())
		return

	undoStep .reset ();

	const auto index = iter - familyEntries .begin ();

	addUndoFunction (fontStyleNode, fontStyleNode -> family (), undoStep);

	fontStyleNode -> family () .erase (fontStyleNode -> family () .begin () + index);

	addRedoFunction (fontStyleNode -> family (), undoStep);
}

void
TextEditor::on_fontChooserButton_clicked ()
{
	openFontChooserDialog (-1);
}

void
TextEditor::openFontChooserDialog (const int value)
{
	familyIndex = value;

	std::string family = familyIndex == -1
	                     ? getFamilyEntry () .get_text ()
	                     : familyEntries [familyIndex] -> get_text ();

	Pango::FontDescription fontDescription;

	fontDescription .set_family (family .empty () ? "SERIF" : family);

	getFontChooserDialog () .set_font_desc (fontDescription);
	getFontChooserDialog () .present ();
}

void
TextEditor::on_font_response (int response_id)
{
	getFontChooserDialog () .hide ();

	if (response_id == Gtk::RESPONSE_OK)
	{
		const auto fontDescription = getFontChooserDialog () .get_font_desc ();
	
		if (familyIndex == -1)
			getFamilyEntry () .set_text (fontDescription .get_family ());

		else
		{
			undoStep .reset ();

			familyEntries [familyIndex] -> set_text (fontDescription .get_family ());
		}
	}
}

void
TextEditor::on_add_family_clicked ()
{
	undoStep .reset ();

	addUndoFunction (fontStyleNode, fontStyleNode -> family (), undoStep);

	fontStyleNode -> family () .emplace_back (getFamilyEntry () .get_text ());

	addRedoFunction (fontStyleNode -> family (), undoStep);

	getFamilyEntry () .set_text ("");
}

// size or pointSize

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

TextEditor::~TextEditor ()
{ }

} // puck
} // titania
