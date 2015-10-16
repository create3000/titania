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

#include "X3DTextureNodeEditor.h"

#include "../../Configuration/config.h"

#include <Titania/X3D/Components/Shape/Appearance.h>

namespace titania {
namespace puck {

X3DTextureNodeEditor::X3DTextureNodeEditor (const X3D::BrowserPtr & preview) :
	          X3DBaseInterface (),
	 X3DTextureEditorInterface (),
	    X3DTexture2DNodeEditor (preview),
	X3DTexturePropertiesEditor (),
	                   preview (preview),
	               appearances (),
	             textureBuffer (),
	               textureNode (),
	                  undoStep (),
	                  changing (false)
{
	addChildren (textureBuffer, textureNode);
	textureBuffer .addInterest (this, &X3DTextureNodeEditor::set_node);
	textureNode   .addInterest (this, &X3DTextureNodeEditor::set_preview);

	preview -> setAntialiasing (4);
}

void
X3DTextureNodeEditor::initialize ()
{
	X3DTexturePropertiesEditor::initialize ();

	getPreviewBox () .pack_start (*preview, true, true, 0);

	preview -> initialized () .addInterest (this, &X3DTextureNodeEditor::set_browser);
	preview -> set_opacity (0);
	preview -> show ();

	getBrowserWindow () -> getSelection () -> getChildren () .addInterest (this, &X3DTextureNodeEditor::set_selection);

	set_selection ();
}

void
X3DTextureNodeEditor::set_browser ()
{
	preview -> initialized () .removeInterest (this, &X3DTextureNodeEditor::set_browser);
	preview -> initialized () .addInterest (this, &X3DTextureNodeEditor::set_initialized);

	try
	{
		preview -> loadURL ({ get_ui ("Editors/TextureEditorPreview.x3dv") }, { });
	}
	catch (const X3D::X3DError &)
	{ }
}

void
X3DTextureNodeEditor::set_initialized ()
{
	preview -> initialized () .removeInterest (this, &X3DTextureNodeEditor::set_initialized);
	preview -> set_opacity (1);

	try
	{
		preview -> getExecutionContext () -> getNamedNode ("Appearance") -> isPrivate (true);
	}
	catch (const X3D::X3DError &)
	{ }

	set_preview ();
	X3DTexture2DNodeEditor::set_preview ();
}

void
X3DTextureNodeEditor::set_selection ()
{
	for (const auto & appearance : appearances)
		appearance -> texture () .removeInterest (this, &X3DTextureNodeEditor::set_texture);

	appearances = getSelection <X3D::Appearance> ({ X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> texture () .addInterest (this, &X3DTextureNodeEditor::set_texture);

	set_texture ();
}

/***********************************************************************************************************************
 *
 *  Preview
 *
 **********************************************************************************************************************/

void
X3DTextureNodeEditor::set_preview ()
{
	try
	{
		const X3D::AppearancePtr appearance (preview -> getExecutionContext () -> getNamedNode ("Appearance"));

		if (appearance)
		{
			if (appearance -> texture ())
				appearance -> texture () -> removeInterest (*preview, &X3D::Browser::addEvent);

			if (getTextureComboBoxText () .get_active_row_number () > 0)
				appearance -> texture () = textureNode;

			else
				appearance -> texture () = nullptr;

			if (appearance -> texture ())
				appearance -> texture () -> addInterest (*preview, &X3D::Browser::addEvent);
		}
	}
	catch (const X3D::X3DError &)
	{ }
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
	getTextureNotebook () .set_visible (getTextureComboBoxText () .get_active_row_number () > 0 and
	                                    getTextureComboBoxText () .get_active_row_number () not_eq 2);

	if (changing)
		return;

	if (getTextureComboBoxText () .get_active_row_number () > 0)
	{
		//getTextureNotebook () .set_visible (true);

		switch (getTextureComboBoxText () .get_active_row_number ())
		{
			case 1:
				textureNode = getImageTexture (textureNode);
				break;
			case 2:
				textureNode = getPixelTexture (textureNode);
				break;
			case 3:
				textureNode = getMovieTexture (textureNode);
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

	addUndoFunction <X3D::SFNode> (appearances, "texture", undoStep);

	for (const auto & appearance : appearances)
	{
		try
		{
			auto & field = appearance -> texture ();

			field .removeInterest (this, &X3DTextureNodeEditor::set_texture);
			field .addInterest (this, &X3DTextureNodeEditor::connectTexture);

			if (getTextureComboBoxText () .get_active_row_number () > 0)
				getBrowserWindow () -> replaceNode (getExecutionContext (), X3D::SFNode (appearance), field, X3D::SFNode (textureNode), undoStep);
			else
				getBrowserWindow () -> replaceNode (getExecutionContext (), X3D::SFNode (appearance), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "texture", undoStep);

	getTextureUnlinkButton () .set_sensitive (getTextureComboBoxText () .get_active_row_number () > 0 and textureNode -> getCloneCount () > 1);

	X3DTexturePropertiesEditor::set_selection ();
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
		IMAGE_TEXTURE,
		PIXEL_TEXTURE,
		MOVIE_TEXTURE
	};

	auto       tuple     = getSelection <X3D::X3DTextureNode> (appearances, "texture");
	const int  active    = std::get <1> (tuple);
	const bool hasParent = std::get <2> (tuple);
	const bool hasField  = (active not_eq -2);

	textureNode = std::move (std::get <0> (tuple));

	setTexture2DNode (textureNode);
	//setTexture3DNode (textureNode);

	if (not textureNode)
		textureNode = getImageTexture (textureNode);

	changing = true;

	if (active > 0)
	{
		switch (textureNode -> getType () .back ())
		{
			case X3D::X3DConstants::ImageTexture:
				getTextureComboBoxText () .set_active (IMAGE_TEXTURE);
				break;
			case X3D::X3DConstants::PixelTexture:
				getTextureComboBoxText () .set_active (PIXEL_TEXTURE);
				break;
			case X3D::X3DConstants::MovieTexture:
				getTextureComboBoxText () .set_active (MOVIE_TEXTURE);
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

	changing = false;

	X3DTexturePropertiesEditor::set_selection ();
}

void
X3DTextureNodeEditor::connectTexture (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DTextureNodeEditor::connectTexture);
	field .addInterest (this, &X3DTextureNodeEditor::set_texture);
}

X3DTextureNodeEditor::~X3DTextureNodeEditor ()
{ }

} // puck
} // titania
