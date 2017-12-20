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

#include "X3DTexturePropertiesEditor.h"

namespace titania {
namespace puck {

X3DTexturePropertiesEditor::X3DTexturePropertiesEditor () :
	X3DTextureEditorInterface (),
	                 nodeName (this, getTexturePropertiesNameEntry (), getTexturePropertiesRenameButton ()),
	              borderColor (this, getTexturePropertiesBorderColorButton (), getTexturePropertiesBorderColorAdjustment (), getTexturePropertiesBorderColorBox (), "borderColor"),
	              borderWidth (this, getTexturePropertiesBorderWidthAdjustment (), getTexturePropertiesBorderWidthSpinButton (), "borderWidth"),
	        anisotropicDegree (this, getTexturePropertiesAnisotropicDegreeAdjustment (), getTexturePropertiesAnisotropicDegreeSpinButton (), "anisotropicDegree"),
	          generateMipMaps (this, getTexturePropertiesGenerateMipMapsCheckButton (), "generateMipMaps"),
	       minificationFilter (this, getTexturePropertiesMinificationFilterComboBoxText (), "minificationFilter"),
	      magnificationFilter (this, getTexturePropertiesMagnificationFilterComboBoxText (), "magnificationFilter"),
	            boundaryModeS (this, getTexturePropertiesBoundaryModeSComboBoxText (), "boundaryModeS"),
	            boundaryModeT (this, getTexturePropertiesBoundaryModeTComboBoxText (), "boundaryModeT"),
	            boundaryModeR (this, getTexturePropertiesBoundaryModeRComboBoxText (), "boundaryModeR"),
	       textureCompression (this, getTexturePropertiesTextureCompressionComboBoxText (), "textureCompression"),
	          texturePriority (this, getTexturePropertiesTexturePriorityAdjustment (), getTexturePropertiesTexturePrioritySpinButton (), "texturePriority"),
	             textureNodes (),
	  texturePropertiesBuffer (),
	        textureProperties (),
	                 undoStep (),
	                 changing (false)
{
	addChildObjects (textureNodes, texturePropertiesBuffer, textureProperties);

	texturePropertiesBuffer .addInterest (&X3DTexturePropertiesEditor::set_node, this);
}

void
X3DTexturePropertiesEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & textureNode : textureNodes)
	{
		try
		{
			textureNode -> getField <X3D::SFNode> ("textureProperties") .removeInterest (&X3DTexturePropertiesEditor::set_textureProperties, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	textureNodes = getNodes <X3D::X3DTextureNode> (selection, {
	                                                            X3D::X3DConstants::ImageTexture,
	                                                            X3D::X3DConstants::MovieTexture,
	                                                            X3D::X3DConstants::PixelTexture,
	                                                            X3D::X3DConstants::ComposedTexture3D,
	                                                            X3D::X3DConstants::ImageTexture3D,
	                                                            X3D::X3DConstants::PixelTexture3D,
	                                                            X3D::X3DConstants::ComposedCubeMapTexture,
	                                                            X3D::X3DConstants::GeneratedCubeMapTexture,
	                                                            X3D::X3DConstants::ImageCubeMapTexture
																		      });

	for (const auto & textureNode : textureNodes)
	{
		try
		{
			textureNode -> getField <X3D::SFNode> ("textureProperties") .addInterest (&X3DTexturePropertiesEditor::set_textureProperties, this);
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

	const auto executionContext = X3D::MakePtr (getSelectionContext (textureNodes, true));

	addUndoFunction <X3D::SFNode> (textureNodes, "textureProperties", undoStep);

	for (const auto & textureNode : textureNodes)
	{
		try
		{
			auto & field = textureNode -> getField <X3D::SFNode> ("textureProperties");

			field .removeInterest (&X3DTexturePropertiesEditor::set_textureProperties, this);
			field .addInterest (&X3DTexturePropertiesEditor::connectTextureProperties, this);

			if (getTexturePropertiesCheckButton () .get_active ())
				X3D::X3DEditor::replaceNode (executionContext, textureNode, field, textureProperties, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, textureNode, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (textureNodes, "textureProperties", undoStep);

	getTexturePropertiesUnlinkButton () .set_sensitive (getTexturePropertiesCheckButton () .get_active () and textureProperties -> getCloneCount () > 1);
}

void
X3DTexturePropertiesEditor::set_textureProperties ()
{
	texturePropertiesBuffer .addEvent ();
}

void
X3DTexturePropertiesEditor::set_node ()
{
	undoStep .reset ();

	const auto    executionContext = getSelectionContext (textureNodes, true);
	auto          tuple            = getSelection <X3D::TextureProperties> (textureNodes, "textureProperties");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	textureProperties = std::move (std::get <0> (tuple));

	if (not textureProperties)
		textureProperties = executionContext -> createNode <X3D::TextureProperties> ();

	changing = true;

	getSelectTexturePropertiesBox ()   .set_sensitive (hasParent);
	getTexturePropertiesCheckButton () .set_sensitive (hasField);
	getTexturePropertiesCheckButton () .set_active (active > 0);
	getTexturePropertiesCheckButton () .set_inconsistent (active < 0);

	getTexturePropertiesUnlinkButton () .set_sensitive (active > 0 and textureProperties -> getCloneCount () > 1);
	getTexturePropertiesBox ()          .set_sensitive (active > 0);

	changing = false;

	// Widgets

	const X3D::MFNode nodes = { textureProperties };

	nodeName .setNode (nodes .back ());

	borderColor         .setNodes (nodes);
	borderWidth         .setNodes (nodes);
	anisotropicDegree   .setNodes (nodes);
	generateMipMaps     .setNodes (nodes);
	minificationFilter  .setNodes (nodes);
	magnificationFilter .setNodes (nodes);
	boundaryModeS       .setNodes (nodes);
	boundaryModeT       .setNodes (nodes);
	boundaryModeR       .setNodes (nodes);
	textureCompression  .setNodes (nodes);
	texturePriority     .setNodes (nodes);
}

void
X3DTexturePropertiesEditor::connectTextureProperties (const X3D::SFNode & field)
{
	field .removeInterest (&X3DTexturePropertiesEditor::connectTextureProperties, this);
	field .addInterest (&X3DTexturePropertiesEditor::set_textureProperties, this);
}

} // puck
} // titania
