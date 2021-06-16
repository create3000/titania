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

#include "X3DTextureNodeEditor.h"

#include "../../Browser/BrowserSelection.h"
#include "../../ComposedWidgets/TexturePreview.h"

#include <Titania/X3D/Components/Shape/Appearance.h>

namespace titania {
namespace puck {

X3DTextureNodeEditor::X3DTextureNodeEditor () :
	               X3DBaseInterface (),
	      X3DTextureEditorInterface (),
	          X3DMultiTextureEditor (),
	         X3DTexture2DNodeEditor (),
	         X3DTexture3DNodeEditor (),
	X3DEnvironmentTextureNodeEditor (),
	     X3DTexturePropertiesEditor (),
	                       nodeName (this, getTextureNameEntry (), getTextureRenameButton ()),
	                        preview (new TexturePreview (this,
                                    getPreviewBox (),
                                    getTextureFormatLabel (),
                                    getTextureLoadStateLabel ())),
	                    appearances (),
	                  textureBuffer (),
	                    textureNode (),
	                       undoStep (),
	                       changing (false)
{
	addChildObjects (appearances, textureBuffer, textureNode);

	textureBuffer .addInterest (&X3DTextureNodeEditor::set_node, this);
}

void
X3DTextureNodeEditor::configure ()
{
	X3DEnvironmentTextureNodeEditor::configure ();
}

void
X3DTextureNodeEditor::set_selection (const X3D::MFNode & selection)
{
	X3DTexturePropertiesEditor::set_selection (selection);

	for (const auto & appearance : appearances)
		appearance -> texture () .removeInterest (&X3DTextureNodeEditor::set_texture, this);

	appearances = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> texture () .addInterest (&X3DTextureNodeEditor::set_texture, this);

	set_texture ();
}

/***********************************************************************************************************************
 *
 *  texture
 *
 **********************************************************************************************************************/

void
X3DTextureNodeEditor::on_texture_unlink_clicked ()
{
	unlinkClone (appearances, "texture", undoStep);
}

void
X3DTextureNodeEditor::on_texture_changed ()
{
	getTextureNotebook () .set_sensitive (getTextureComboBoxText () .get_active_row_number () > 0);

	if (changing)
		return;

	if (getTextureComboBoxText () .get_active_row_number () > 0)
	{
		switch (getTextureComboBoxText () .get_active_row_number ())
		{
			case 1:
				textureNode = getMultiTexture (textureNode);
				break;
			case 2:
				textureNode = getImageTexture (textureNode);
				break;
			case 3:
				textureNode = getPixelTexture (textureNode);
				break;
			case 4:
				textureNode = getMovieTexture (textureNode);
				break;
			case 5:
				textureNode = getComposedTexture3D (textureNode);
				break;
			case 6:
				textureNode = getImageTexture3D (textureNode);
				break;
			case 7:
				textureNode = getPixelTexture3D (textureNode);
				break;
			case 8:
				textureNode = getComposedCubeMapTexture (textureNode);
				break;
			case 9:
				textureNode = getGeneratedCubeMapTexture (textureNode);
				break;
			case 10:
				textureNode = getImageCubeMapTexture (textureNode);
				break;
			default:
				break;
		}

		getTextureNotebook () .set_current_page (getTextureComboBoxText () .get_active_row_number ());
	}
	else
	{
		// INCONSISTENT and NONE
		textureNode .addEvent ();
		getTextureFormatLabel () .set_text ("");
	}

	// Set field.

	const auto executionContext = X3D::MakePtr (getSelectionContext (appearances, true));

	addUndoFunction <X3D::SFNode> (appearances, "texture", undoStep);

	for (const auto & appearance : appearances)
	{
		try
		{
			auto & field = appearance -> texture ();

			field .removeInterest (&X3DTextureNodeEditor::set_texture, this);
			field .addInterest (&X3DTextureNodeEditor::connectTexture, this);

			if (getTextureComboBoxText () .get_active_row_number () > 0)
				X3D::X3DEditor::replaceNode (executionContext, appearance, field, textureNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, appearance, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "texture", undoStep);

	getTextureUnlinkButton () .set_sensitive (getTextureComboBoxText () .get_active_row_number () > 0 and textureNode -> getCloneCount () > 1);

	setMultiTexture           (executionContext, textureNode);
	setTexture2DNode          (executionContext, textureNode);
	setTexture3DNode          (executionContext, textureNode);
	setEnvironmentTextureNode (executionContext, textureNode);
	X3DTexturePropertiesEditor::set_selection (appearances);

	getTextureNameGrid () .set_sensitive (textureNode);

	nodeName .setNode (textureNode);

	preview -> setTexture (getTextureComboBoxText () .get_active_row_number () > 0 ? textureNode : nullptr);
}

void
X3DTextureNodeEditor::set_texture ()
{
	textureBuffer .addEvent ();
}

void
X3DTextureNodeEditor::set_node ()
{
	undoStep .reset ();

	enum {
		NULL_TEXTURE,
		MULTI_TEXTURE,
		IMAGE_TEXTURE,
		PIXEL_TEXTURE,
		MOVIE_TEXTURE,
		COMPOSED_TEXTURE_3D,
		IMAGE_TEXTURE_3D,
		PIXEL_TEXTURE_3D,
		COMPOSED_CUBE_MAP_TEXTURE,
		GENERATED_CUBE_MAP_TEXTURE,
		IMAGE_CUBE_MAP_TEXTURE
	};

	const auto executionContext = X3D::MakePtr (getSelectionContext (appearances, true));
	auto       tuple            = getSelection <X3D::X3DTextureNode> (appearances, "texture");
	const int  active           = std::get <1> (tuple);
	const bool hasParent        = std::get <2> (tuple);
	const bool hasField         = (active not_eq -2);

	textureNode = std::move (std::get <0> (tuple));

	setMultiTexture           (executionContext, textureNode);
	setTexture2DNode          (executionContext, textureNode);
	setTexture3DNode          (executionContext, textureNode);
	setEnvironmentTextureNode (executionContext, textureNode);

	getTextureNameGrid () .set_sensitive (textureNode);

	nodeName .setNode (textureNode);

	if (not textureNode)
		textureNode = getImageTexture (textureNode);

	changing = true;

	if (active > 0)
	{
		switch (textureNode -> getType () .back ())
		{
			case X3D::X3DConstants::MultiTexture:
				getTextureComboBoxText () .set_active (MULTI_TEXTURE);
				break;
			case X3D::X3DConstants::ImageTexture:
				getTextureComboBoxText () .set_active (IMAGE_TEXTURE);
				break;
			case X3D::X3DConstants::PixelTexture:
				getTextureComboBoxText () .set_active (PIXEL_TEXTURE);
				break;
			case X3D::X3DConstants::MovieTexture:
				getTextureComboBoxText () .set_active (MOVIE_TEXTURE);
				break;
			case X3D::X3DConstants::ComposedTexture3D:
				getTextureComboBoxText () .set_active (COMPOSED_TEXTURE_3D);
				break;
			case X3D::X3DConstants::ImageTexture3D:
				getTextureComboBoxText () .set_active (IMAGE_TEXTURE_3D);
				break;
			case X3D::X3DConstants::PixelTexture3D:
				getTextureComboBoxText () .set_active (PIXEL_TEXTURE_3D);
				break;
			case X3D::X3DConstants::ComposedCubeMapTexture:
				getTextureComboBoxText () .set_active (COMPOSED_CUBE_MAP_TEXTURE);
				break;
			case X3D::X3DConstants::GeneratedCubeMapTexture:
				getTextureComboBoxText () .set_active (GENERATED_CUBE_MAP_TEXTURE);
				break;
			case X3D::X3DConstants::ImageCubeMapTexture:
				getTextureComboBoxText () .set_active (IMAGE_CUBE_MAP_TEXTURE);
				break;
			default:
				getTextureComboBoxText () .set_active (-1);
				break;
		}
	}
	else if (active == 0)
	{
		getTextureComboBoxText () .set_active (NULL_TEXTURE);
		getTextureFormatLabel ()  .set_text ("");
	}
	else
	{
		getTextureComboBoxText () .set_active (-1);
		getTextureFormatLabel ()  .set_text ("");
	}

	getSelectTextureBox ()    .set_sensitive (hasParent);
	getTextureBox ()          .set_sensitive (hasField);
	getTextureUnlinkButton () .set_sensitive (active > 0 and textureNode -> getCloneCount () > 1);
	getTextureNotebook ()     .set_current_page (getTextureComboBoxText () .get_active_row_number ());

	preview -> setTexture (getTextureComboBoxText () .get_active_row_number () > 0 ? textureNode : nullptr);

	changing = false;

	X3DTexturePropertiesEditor::set_selection (getBrowserWindow () -> getSelection () -> getNodes ());
}

void
X3DTextureNodeEditor::connectTexture (const X3D::SFNode & field)
{
	field .removeInterest (&X3DTextureNodeEditor::connectTexture, this);
	field .addInterest (&X3DTextureNodeEditor::set_texture, this);
}

void
X3DTextureNodeEditor::store ()
{
	X3DEnvironmentTextureNodeEditor::store ();
}

X3DTextureNodeEditor::~X3DTextureNodeEditor ()
{ }

} // puck
} // titania
