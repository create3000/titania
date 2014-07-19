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

namespace titania {
namespace puck {

X3DTextureNodeEditor::X3DTextureNodeEditor () :
	          X3DBaseInterface (),
	 X3DTextureEditorInterface ("", ""),
	    X3DTexture2DNodeEditor (),
	X3DTexturePropertiesEditor (),
	              appearances (),
	               textureNode (),
	                  undoStep (),
	                  changing (false)
{
	getWidget () .signal_map () .connect (sigc::bind (sigc::mem_fun (getWindow (), &Gtk::Window::resize), 1, 1));
}

void
X3DTextureNodeEditor::initialize ()
{
	X3DTexturePropertiesEditor::initialize ();

	getBrowser () -> getSelection () -> getChildren () .addInterest (this, &X3DTextureNodeEditor::set_selection);

	set_selection ();
}

void
X3DTextureNodeEditor::set_selection ()
{
	for (const auto & appearance : appearances)
		appearance -> texture () .removeInterest (this, &X3DTextureNodeEditor::set_texture);

	undoStep .reset ();

	// Find Appearances.

	auto selection = getBrowser () -> getSelection () -> getChildren ();

	appearances .clear ();

	X3D::traverse (selection, [&] (X3D::SFNode & node)
	               {
	                  for (const auto & type: node -> getType ())
	                  {
	                     if (type == X3D::X3DConstants::Appearance)
	                     {
	                        appearances .emplace_back (node);
	                        return true;
								}
							}

	                  return true;
						});

	for (const auto & appearance : appearances)
		appearance -> texture () .addInterest (this, &X3DTextureNodeEditor::set_texture);

	set_texture ();
}

/***********************************************************************************************************************
 *
 *  texture
 *
 **********************************************************************************************************************/

void
X3DTextureNodeEditor::on_texture_changed ()
{
	getTextureNotebook () .set_sensitive (getTextureButton () .get_active_row_number () > 0);
	getTexture2DBox ()    .set_sensitive (getTextureButton () .get_active_row_number () > 0);

	getImageTextureBox () .set_visible (false);

	if (textureNode)
	{
		switch (getTextureButton () .get_active_row_number ())
		{
			case 1:
				//getImageTextureBox () .set_visible (true);
				//getTextureNotebook () .set_current_page (1);
				break;
			default:
				break;
		}
	}

	getWindow () .resize (1, 1);

	if (changing)
		return;

	if (getTextureButton () .get_active_row_number () > 0)
	{
		X3D::X3DPtr <X3D::X3DTextureNode> value;

		switch (getTextureButton () .get_active_row_number ())
		{
			case 1:
			{
				value = new X3D::ImageTexture (getExecutionContext ());
				setTexture2DNode (value);
				break;
			}
			default:
				break;
		}

		if (value)
		{
			textureNode = value;
			getExecutionContext () -> addUninitializedNode (textureNode);
			getExecutionContext () -> realize ();
		}
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

			if (getTextureButton () .get_active_row_number () > 0)
				getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, X3D::SFNode (textureNode), undoStep);
			else
				getBrowserWindow () -> replaceNode (X3D::SFNode (appearance), field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "texture", undoStep);

	X3DTexturePropertiesEditor::set_selection ();
}

void
X3DTextureNodeEditor::set_texture ()
{
	textureNode = nullptr;

	const bool hasField = not appearances .empty ();

	// Find last »geometry« field.

	int active = -1;

	for (const auto & appearance : basic::reverse_adapter (appearances))
	{
		try
		{
			const X3D::X3DPtr <X3D::X3DTextureNode> field (appearance -> texture ());

			if (active < 0)
			{
				textureNode = std::move (field);
				active      = bool (textureNode);
			}
			else if (field not_eq textureNode)
			{
				if (not textureNode)
					textureNode = std::move (field);

				active = -1;
				break;
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	if (not textureNode)
	{
		textureNode = new X3D::ImageTexture (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (textureNode);
		getExecutionContext () -> realize ();
	}

	setTexture2DNode (textureNode);

	changing = true;

	if (active > 0)
	{
		switch (textureNode -> getType () .back ())
		{
			case X3D::X3DConstants::ImageTexture:
				getTextureButton () .set_active (1);
				break;
			default:
				getTextureButton () .set_active (-1);
				break;
		}
	}
	else if (active == 0)
		getTextureButton () .set_active (0);
	else
		getTextureButton () .set_active (-1);

	getTextureButton () .set_sensitive (hasField);

	changing = false;

	X3DTexturePropertiesEditor::set_selection ();
}

void
X3DTextureNodeEditor::connectTexture (const X3D::SFNode & field)
{
	field .removeInterest (this, &X3DTextureNodeEditor::connectTexture);
	field .addInterest (this, &X3DTextureNodeEditor::set_texture);
}

} // puck
} // titania
