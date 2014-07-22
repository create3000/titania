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

#include "X3DTexturePropertiesEditor.h"

namespace titania {
namespace puck {

X3DTexturePropertiesEditor::X3DTexturePropertiesEditor () :
	X3DTextureEditorInterface ("", ""),
	             textureNodes (),
	        textureProperties (),
	                 undoStep (),
	                 changing (false),
	              borderWidth (getBrowserWindow (), getTexturePropertiesBorderWidthAdjustment (), getTexturePropertiesBorderWidthSpinButton (), "borderWidth"),
	        anisotropicDegree (getBrowserWindow (), getTexturePropertiesAnisotropicDegreeAdjustment (), getTexturePropertiesAnisotropicDegreeSpinButton (), "anisotropicDegree"),
	          generateMipMaps (getBrowserWindow (), getTexturePropertiesGenerateMipMapsCheckButton (), "generateMipMaps"),
	       minificationFilter (getBrowserWindow (), getTexturePropertiesMinificationFilterComboBoxText (), "minificationFilter"),
	      magnificationFilter (getBrowserWindow (), getTexturePropertiesMagnificationFilterComboBoxText (), "magnificationFilter"),
	            boundaryModeS (getBrowserWindow (), getTexturePropertiesBoundaryModeSComboBoxText (), "boundaryModeS"),
	            boundaryModeT (getBrowserWindow (), getTexturePropertiesBoundaryModeTComboBoxText (), "boundaryModeT"),
	            boundaryModeR (getBrowserWindow (), getTexturePropertiesBoundaryModeRComboBoxText (), "boundaryModeR"),
	       textureCompression (getBrowserWindow (), getTexturePropertiesTextureCompressionComboBoxText (), "textureCompression"),
	          texturePriority (getBrowserWindow (), getTexturePropertiesTexturePriorityAdjustment (), getTexturePropertiesTexturePrioritySpinButton (), "texturePriority")
{ }

void
X3DTexturePropertiesEditor::initialize ()
{
	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DTexturePropertiesEditor::set_selection);

	set_selection ();
}

void
X3DTexturePropertiesEditor::set_selection ()
{
	undoStep .reset ();

	for (const auto & textureNode : textureNodes)
	{
		try
		{
			textureNode -> getField <X3D::SFNode> ("textureProperties") .removeInterest (this, &X3DTexturePropertiesEditor::set_textureProperties);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	textureNodes = getSelection <X3D::X3DTextureNode> ({
	                                                      X3D::X3DConstants::ImageTexture,
	                                                      X3D::X3DConstants::MovieTexture,
	                                                      X3D::X3DConstants::PixelTexture,
	                                                      X3D::X3DConstants::ComposedTexture3D,
	                                                      X3D::X3DConstants::ImageTexture3D,
	                                                      X3D::X3DConstants::PixelTexture3D
																		});

	for (const auto & textureNode : textureNodes)
	{
		try
		{
			textureNode -> getField <X3D::SFNode> ("textureProperties") .addInterest (this, &X3DTexturePropertiesEditor::set_textureProperties);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_textureProperties ();
}

/***********************************************************************************************************************
 *
 *  textureProperties
 *
 **********************************************************************************************************************/

void
X3DTexturePropertiesEditor::on_textureProperties_unlink_clicked ()
{
	unlinkClone (textureNodes, "textureProperties", undoStep);
}

void
X3DTexturePropertiesEditor::on_textureProperties_toggled ()
{
	if (changing)
		return;

	getTexturePropertiesCheckButton () .set_inconsistent (false);
	getTexturePropertiesBox ()         .set_sensitive (getTexturePropertiesCheckButton () .get_active ());

	// Set field.

	addUndoFunction <X3D::SFNode> (textureNodes, "textureProperties", undoStep);

	for (const auto & textureNode : textureNodes)
	{
		try
		{
			auto & field = textureNode -> getField <X3D::SFNode> ("textureProperties");

			field .removeInterest (this, &X3DTexturePropertiesEditor::set_textureProperties);
			field .addInterest (this, &X3DTexturePropertiesEditor::connectTextureProperties);

			if (getTexturePropertiesCheckButton () .get_active ())
				getBrowserWindow () -> replaceNode (X3D::SFNode (textureNode), field, X3D::SFNode (textureProperties), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (textureNode), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (textureNodes, "textureProperties", undoStep);

	getTexturePropertiesUnlinkButton () .set_sensitive (getTexturePropertiesCheckButton () .get_active () and textureProperties -> isCloned () > 1);
}

void
X3DTexturePropertiesEditor::set_textureProperties ()
{
	auto       pair     = getNode <X3D::TextureProperties> (textureNodes, "textureProperties");
	const int  active   = pair .second;
	const bool hasField = (active not_eq -2);

	textureProperties = std::move (pair .first);

	if (not textureProperties)
	{
		textureProperties = new X3D::TextureProperties (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (textureProperties);
		getExecutionContext () -> realize ();
	}

	changing = true;

	getTexturePropertiesCheckButton () .set_sensitive (hasField);
	getTexturePropertiesCheckButton () .set_active (active > 0);
	getTexturePropertiesCheckButton () .set_inconsistent (active < 0);

	getTexturePropertiesUnlinkButton () .set_sensitive (active > 0 and textureProperties -> isCloned () > 1);
	getTexturePropertiesBox ()          .set_sensitive (active > 0);

	changing = false;

	borderWidth         .setNodes ({ textureProperties });
	anisotropicDegree   .setNodes ({ textureProperties });
	generateMipMaps     .setNodes ({ textureProperties });
	minificationFilter  .setNodes ({ textureProperties });
	magnificationFilter .setNodes ({ textureProperties });
	boundaryModeS       .setNodes ({ textureProperties });
	boundaryModeT       .setNodes ({ textureProperties });
	boundaryModeR       .setNodes ({ textureProperties });
	textureCompression  .setNodes ({ textureProperties });
	texturePriority     .setNodes ({ textureProperties });
}

void
X3DTexturePropertiesEditor::connectTextureProperties (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DTexturePropertiesEditor::connectTextureProperties);
	field .addInterest (this, &X3DTexturePropertiesEditor::set_textureProperties);
}

/***********************************************************************************************************************
 *
 *  backSpecularColor
 *
 **********************************************************************************************************************/

//bool
//X3DTexturePropertiesEditor::on_draw (const Cairo::RefPtr <Cairo::Context> & context)
//{
//	const auto color = dialog .get_color_selection () -> get_current_rgba ();
//
//	context -> set_source_rgba (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
//	context -> rectangle (0, 0, drawingArea .get_width (), drawingArea .get_height ());
//	context -> fill ();
//
//	return true;
//}
//
//void
//X3DTexturePropertiesEditor::on_clicked ()
//{
//	dialog () .present ();
//}
//
//void
//X3DTexturePropertiesEditor::on_color_changed ()
//{
//	drawingArea .queue_draw ();
//
//	if (changing)
//		return;
//
//	const auto color = dialog .get_color_selection () -> get_current_rgba ();
//
//	// Update field
//
//	addUndoFunction <X3D::SFColorRGBA> (nodes, name, undoStep);
//
//	for (const auto & node : nodes)
//	{
//		try
//		{
//			auto & field = node -> getField <X3D::SFColorRGBA> (name);
//
//			field .removeInterest (this, &X3DTexturePropertiesEditor::set_color);
//			field .addInterest (this, &X3DTexturePropertiesEditor::connect);
//
//			field = X3D::Color4f (color .get_red (), color .get_green (), color .get_blue (), color .get_alpha ());
//		}
//		catch (const X3D::X3DError &)
//		{ }
//	}
//
//	addRedoFunction <X3D::SFColorRGBA> (nodes, name, undoStep);
//}
//
//void
//X3DTexturePropertiesEditor::set_field ()
//{
//	changing = true;
//
//	const int  active   = getColorRGBA (nodes, name);
//	const bool hasField = (active not_eq -2);
//
//	toggleButton .set_sensitive (hasField);
//	toggleButton .set_active (active > 0);
//	toggleButton .set_inconsistent (active < 0);
//
//	changing = false;
//}
//
//void
//X3DTexturePropertiesEditor::connect (const X3D::SFColor & field)
//{
//	field .removeInterest (this, &X3DTexturePropertiesEditor::connect);
//	field .addInterest (this, &X3DTexturePropertiesEditor::set_field);
//}

} // puck
} // titania
