/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Core/MetadataString.h>
#include <Titania/X3D/Components/Shape/X3DShapeNode.h>
#include <Titania/String.h>

namespace titania {
namespace puck {

TextEditor::TextEditor (X3DBrowserWindow* const browserWindow) :
	      X3DBaseInterface (browserWindow, browserWindow -> getCurrentBrowser ()),
	X3DTextEditorInterface (get_ui ("Editors/TextEditor.glade")),
	X3DFontStyleNodeEditor (),
	                string (this, getTextStringTextView (), "string"),
	             maxExtent (this, getTextMaxExtentAdjustment (), getTextMaxExtentSpinButton (), "maxExtent"),
	            shapeNodes (),
	    geometryNodeBuffer (),
	                  text (),
	               measure (),
	              undoStep (),
	        lengthUndoStep (),
	              changing (false)
{
	addChildren (shapeNodes, geometryNodeBuffer, text, measure);

	geometryNodeBuffer .addInterest (this, &TextEditor::set_node);

	getTextCharSpacingAdjustment () -> set_step_increment (1e-3);
	getTextMaxExtentAdjustment ()   -> set_step_increment (1e-3);

	getTextCharSpacingSpinButton () .property_climb_rate () = 1e-2;
	getTextMaxExtentSpinButton ()   .property_climb_rate () = 1e-2;

	setup ();
}

void
TextEditor::configure ()
{
	X3DTextEditorInterface::configure ();

	getTextNotebook () .set_current_page (getConfig () -> getInteger ("currentPage"));
}

void
TextEditor::initialize ()
{
	X3DTextEditorInterface::initialize ();
	X3DFontStyleNodeEditor::initialize ();
}

void
TextEditor::set_selection (const X3D::MFNode & selection)
{
	X3DTextEditorInterface::set_selection (selection);
	X3DFontStyleNodeEditor::set_selection (selection);

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .removeInterest (this, &TextEditor::set_geometry);

	shapeNodes = getNodes <X3D::X3DShapeNode> (selection, { X3D::X3DConstants::X3DShapeNode });

	for (const auto & shapeNode : shapeNodes)
		shapeNode -> geometry () .addInterest (this, &TextEditor::set_geometry);

	set_geometry ();
}

/***********************************************************************************************************************
 *
 *  Text
 *
 **********************************************************************************************************************/

void
TextEditor::on_text_unlink_clicked ()
{
	unlinkClone (shapeNodes, "geometry", undoStep);
}

void
TextEditor::on_text_toggled ()
{
	if (changing)
		return;

	getTextCheckButton () .set_inconsistent (false);
	getTextBox ()         .set_sensitive (getTextCheckButton () .get_active ());

	// Set field.

	addUndoFunction <X3D::SFNode> (shapeNodes, "geometry", undoStep);

	for (const auto & shapeNode : shapeNodes)
	{
		try
		{
			auto & field = shapeNode -> geometry ();

			field .removeInterest (this, &TextEditor::set_geometry);
			field .addInterest (this, &TextEditor::connectGeometry);

			if (getTextCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (shapeNode), field, X3D::SFNode (text), undoStep);
			else
				getBrowserWindow () -> replaceNode (getCurrentContext (), X3D::SFNode (shapeNode), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (shapeNodes, "geometry", undoStep);

	X3DFontStyleNodeEditor::set_selection (getBrowserWindow () -> getSelection () -> getChildren ());

	getTextUnlinkButton () .set_sensitive (getTextCheckButton () .get_active () and text -> getCloneCount () > 1);
}

void
TextEditor::set_geometry ()
{
	geometryNodeBuffer .addEvent ();
}

void
TextEditor::connectGeometry (const X3D::SFNode & field)
{
	field .removeInterest (this, &TextEditor::connectGeometry);
	field .addInterest (this, &TextEditor::set_geometry);
}

void
TextEditor::set_node ()
{
	undoStep .reset ();

	auto  tuple             = getSelection <X3D::Text> (shapeNodes, "geometry");
	const int32_t active    = std::get <1> (tuple);
	const bool    hasParent = std::get <2> (tuple);
	const bool    hasField  = (active not_eq -2);

	text = std::move (std::get <0> (tuple));

	if (text)
	{
		measure = getCurrentContext () -> createNode <X3D::Text> ();
		measure -> lineBounds () .addInterest (this, &TextEditor::set_lineBounds);

		text -> length ()    .addInterest (this, &TextEditor::set_length);
		text -> string ()    .addInterest (measure -> string ());
		text -> fontStyle () .addInterest (measure -> fontStyle ());

		measure -> string ()    = text -> string ();
		measure -> fontStyle () = text -> fontStyle ();
		getCurrentContext () -> realize ();

		set_length ();
	}
	else
	{
		text = new X3D::Text (getCurrentContext ());
		getCurrentContext () -> addUninitializedNode (text);
		getCurrentContext () -> realize ();
	}

	changing = true;

	getSelectTextBox ()   .set_sensitive (hasParent);
	getTextCheckButton () .set_sensitive (hasField);
	getTextCheckButton () .set_active (active > 0);
	getTextCheckButton () .set_inconsistent (active < 0);

	getTextUnlinkButton () .set_sensitive (active > 0 and text -> getCloneCount () > 1);
	getTextBox ()          .set_sensitive (active > 0);

	changing = false;

	string    .setNodes ({ text });
	maxExtent .setNodes ({ text });

	X3DFontStyleNodeEditor::set_selection (getBrowserWindow () -> getSelection () -> getChildren ());
}

void
TextEditor::set_lineBounds ()
{
	set_char_spacing (getTextCharSpacingAdjustment () -> get_value ());
}

void
TextEditor::on_char_spacing_changed ()
{
	if (changing)
		return;

	if (not text)
		return;

	// Set text length

	addUndoFunction <X3D::MFFloat> (X3D::MFNode ({ text }), "length", lengthUndoStep);

	set_char_spacing (getTextCharSpacingAdjustment () -> get_value ());

	addRedoFunction <X3D::MFFloat> (X3D::MFNode ({ text }), "length", lengthUndoStep);
}

void
TextEditor::set_char_spacing (const double kerning)
{
	if (not text)
		return;

	// Set text length

	text -> length () .removeInterest (this, &TextEditor::set_length);
	text -> length () .addInterest (this, &TextEditor::connectLength);
	text -> length () .addEvent ();

	if (kerning)
	{
		for (size_t i = 0, size = text -> string () .size (); i < size; ++ i)
		{
			const auto numChars   = text -> string () .get1Value (i) .length ();
			const auto lineLength = text -> getFontStyle () -> horizontal ()
			                        ? measure -> lineBounds () .get1Value (i) .getX ()
			                        : measure -> lineBounds () .get1Value (i) .getY ();

			if (numChars)
				text -> length () .set1Value (i, (numChars - 1) * kerning + lineLength);
		}
	}
	else
		text -> length () .clear ();
}

void
TextEditor::set_length ()
{
	lengthUndoStep .reset ();

	changing = true;

	double kerning = 0;

	for (size_t i = 0, size = std::min (text -> length () .size (), text -> string () .size ()); i < size; ++ i)
	{
		const auto numChars   = text -> string () .get1Value (i) .length ();
		const auto lineLength = text -> getFontStyle () -> horizontal ()
		                        ? measure -> lineBounds () .get1Value (i) .getX ()
		                        : measure -> lineBounds () .get1Value (i) .getY ();

		kerning += (text -> length () .get1Value (i) - lineLength) / (numChars - 1);
	}

	if (text -> lineBounds () .size ())
		kerning /= text -> lineBounds () .size ();

	getTextCharSpacingAdjustment () -> set_value (kerning);

	changing = false;
}

void
TextEditor::connectLength (const X3D::MFFloat & field)
{
	field .removeInterest (this, &TextEditor::connectLength);
	field .addInterest (this, &TextEditor::set_length);
}

void
TextEditor::store ()
{
	getConfig () -> setItem ("currentPage", getTextNotebook () .get_current_page ());

	X3DTextEditorInterface::store ();
}

TextEditor::~TextEditor ()
{
	dispose ();
}

} // puck
} // titania
