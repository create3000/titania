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

#include "MFStringFamilyWidget.h"
#include <Titania/String.h>

namespace titania {
namespace puck {

X3DFontStyleNodeEditor::X3DFontStyleNodeEditor () :
	X3DTextEditorInterface (),
	                 texts (),
	   fontStyleNodeBuffer (),
	         fontStyleNode (),
	             fontStyle (),
	       screenFontStyle (),
	              undoStep (),
	         styleUndoStep (),
	              changing (false),
	                family (new MFStringFamilyWidget (this,
	                        getFontStyleFamilyTreeView (),
	                        getFontStyleFamilyCellrendererText (),
	                        getFontStyleFamilyAddButton (),
	                        getFontStyleFamilyRemoveButton (),
	                        getFontStyleFamilyChooserColumn (),
	                        "family")),
	                  size (getBrowserWindow (),
	                        getFontStyleSizeAdjustment (),
	                        getFontStyleSizeSpinButton (),
	                        "size"),
	             pointSize (getBrowserWindow (),
	                        getFontStylePointSizeAdjustment (),
	                        getFontStylePointSizeSpinButton (),
	                        "pointSize"),
	               spacing (getBrowserWindow (), getFontStyleSpacingAdjustment (), getFontStyleSpacingSpinButton (), "spacing"),
	            horizontal (getBrowserWindow (), getFontStyleHorizontalCheckButton (),  "horizontal"),
	           leftToRight (getBrowserWindow (), getFontStyleLeftToRightCheckButton (), "leftToRight"),
	           topToBottom (getBrowserWindow (), getFontStyleTopToBottomCheckButton (), "topToBottom"),
	        majorAlignment (getBrowserWindow (), getFontStyleMajorAlignmentComboBoxText (), "justify", 0, "BEGIN"),
	        minorAlignment (getBrowserWindow (), getFontStyleMinorAlignmentComboBoxText (), "justify", 1, "FIRST")
{
	addChildren (fontStyleNodeBuffer);
	fontStyleNodeBuffer .addInterest (this, &X3DFontStyleNodeEditor::set_node);
	
	getFontStyleSizeSpinButton ()      .property_sensitive () .signal_changed () .connect (sigc::mem_fun (*this, &X3DFontStyleNodeEditor::on_size_sensitive_changed));
	getFontStylePointSizeSpinButton () .property_sensitive () .signal_changed () .connect (sigc::mem_fun (*this, &X3DFontStyleNodeEditor::on_point_size_sensitive_changed));
}

void
X3DFontStyleNodeEditor::initialize ()
{
	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DFontStyleNodeEditor::set_selection);

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
				fontStyle -> size () = screenFontStyle -> pointSize ();
				fontStyleNode = fontStyle;
				break;
			}
			case 2:
			{
				screenFontStyle -> pointSize () = fontStyle -> size ();
				fontStyleNode = screenFontStyle;
				break;
			}
			default:
				break;
		}

		fontStyleNode -> language ()    = last -> language ();
		fontStyleNode -> family ()      = last -> family ();
		fontStyleNode -> style ()       = last -> style ();
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

	set_widgets ();
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
		fontStyleNode -> style () .removeInterest (this, &X3DFontStyleNodeEditor::set_style);

	auto       pair     = getNode <X3D::X3DFontStyleNode> (texts, "fontStyle");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	fontStyleNode = std::move (pair .first);

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

	if (not fontStyleNode)
		fontStyleNode = fontStyle;

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

	fontStyleNode -> style () .addInterest (this, &X3DFontStyleNodeEditor::set_style);

	set_style ();
	set_widgets ();
}

void
X3DFontStyleNodeEditor::set_widgets ()
{
	const X3D::MFNode nodes = { fontStyleNode };

	family -> setNodes (nodes);
	size           .setNodes (nodes);
	pointSize      .setNodes (nodes);
	spacing        .setNodes (nodes);
	horizontal     .setNodes (nodes);
	leftToRight    .setNodes (nodes);
	topToBottom    .setNodes (nodes);
	majorAlignment .setNodes (nodes);
	minorAlignment .setNodes (nodes);
}

void
X3DFontStyleNodeEditor::connectFontStyle (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DFontStyleNodeEditor::connectFontStyle);
	field .addInterest (this, &X3DFontStyleNodeEditor::set_fontStyle);
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

	addUndoFunction (fontStyleNode, fontStyleNode -> style (), styleUndoStep);

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

	addRedoFunction (fontStyleNode -> style (), styleUndoStep);
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
 *  Size
 *
 **********************************************************************************************************************/

void
X3DFontStyleNodeEditor::on_size_sensitive_changed ()
{
	getFontStyleSizeLabel ()      .set_visible (getFontStyleSizeSpinButton () .get_sensitive ());
	getFontStyleSizeSpinButton () .set_visible (getFontStyleSizeSpinButton () .get_sensitive ());
}

void
X3DFontStyleNodeEditor::on_point_size_sensitive_changed ()
{
	getFontStylePointSizeLabel ()      .set_visible (getFontStylePointSizeSpinButton () .get_sensitive ());
	getFontStylePointSizeSpinButton () .set_visible (getFontStylePointSizeSpinButton () .get_sensitive ());
}

X3DFontStyleNodeEditor::~X3DFontStyleNodeEditor ()
{ }

} // puck
} // titania
