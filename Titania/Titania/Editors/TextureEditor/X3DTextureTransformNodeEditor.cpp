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

#include "X3DTextureTransformNodeEditor.h"

#include <Titania/X3D/Components/Shape/Appearance.h>

namespace titania {
namespace puck {

X3DTextureTransformNodeEditor::X3DTextureTransformNodeEditor () :
	                 X3DBaseInterface (),
	        X3DTextureEditorInterface (),
	        X3DTextureTransformEditor (),
	      X3DTextureTransform3DEditor (),
	X3DTextureTransformMatrix3DEditor (),
	                         nodeName (this, getTextureTransformNameEntry (), getTextureTransformRenameButton ()),
	                      appearances (),
	           textureTransformBuffer (),
	             textureTransformNode (),
	                         undoStep (),
	                        changing (false)
{
	addChildObjects (appearances, textureTransformBuffer, textureTransformNode);

	textureTransformBuffer .addInterest (&X3DTextureTransformNodeEditor::set_node, this);
}

void
X3DTextureTransformNodeEditor::configure ()
{
	X3DTextureTransformEditor::configure ();
	X3DTextureTransform3DEditor::configure ();
	X3DTextureTransformMatrix3DEditor::configure ();
}

void
X3DTextureTransformNodeEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & appearance : appearances)
		appearance -> textureTransform () .removeInterest (&X3DTextureTransformNodeEditor::set_textureTransform, this);

	appearances = getNodes <X3D::Appearance> (selection, { X3D::X3DConstants::Appearance });

	for (const auto & appearance : appearances)
		appearance -> textureTransform () .addInterest (&X3DTextureTransformNodeEditor::set_textureTransform, this);

	set_textureTransform ();
}

/***********************************************************************************************************************
 *
 *  textureTransform
 *
 **********************************************************************************************************************/

void
X3DTextureTransformNodeEditor::on_textureTransform_unlink_clicked ()
{
	unlinkClone (appearances, "textureTransform", undoStep);
}

void
X3DTextureTransformNodeEditor::on_textureTransform_changed ()
{
	getTextureTransformNotebook () .set_sensitive (getTextureTransformComboBoxText () .get_active_row_number () > 0);

	getTextureTransformBox ()         .set_visible (false);
	getTextureTransform3DBox ()       .set_visible (false);
	getTextureTransformMatrix3DBox () .set_visible (false);

	if (textureTransformNode)
	{
		switch (getTextureTransformComboBoxText () .get_active_row_number ())
		{
			case 1:
				getTextureTransformBox () .set_visible (true);
				getTextureTransformNotebook () .set_current_page (1);
				break;
			case 2:
				getTextureTransform3DBox () .set_visible (true);
				getTextureTransformNotebook () .set_current_page (2);
				break;
			case 3:
				getTextureTransformMatrix3DBox () .set_visible (true);
				getTextureTransformNotebook () .set_current_page (3);
				break;
			default:
				break;
		}
	}

	if (changing)
		return;

	if (getTextureTransformComboBoxText () .get_active_row_number () > 0)
	{
		switch (getTextureTransformComboBoxText () .get_active_row_number ())
		{
			case 1:
				textureTransformNode = getTextureTransform (textureTransformNode);
				break;
			case 2:
				textureTransformNode = getTextureTransform3D (textureTransformNode);
				break;
			case 3:
				textureTransformNode = getTextureTransformMatrix3D (textureTransformNode);
				break;
			default:
				break;
		}
	}

	// Set field.

	const auto executionContext = X3D::MakePtr (getSelectionContext (appearances, true));

	addUndoFunction <X3D::SFNode> (appearances, "textureTransform", undoStep);

	for (const auto & appearance : appearances)
	{
		try
		{
			auto & field = appearance -> textureTransform ();

			field .removeInterest (&X3DTextureTransformNodeEditor::set_textureTransform, this);
			field .addInterest (&X3DTextureTransformNodeEditor::connectTextureTransform, this);

			if (getTextureTransformComboBoxText () .get_active_row_number () > 0)
				X3D::X3DEditor::replaceNode (executionContext, appearance, field, textureTransformNode, undoStep);
			else
				X3D::X3DEditor::replaceNode (executionContext, appearance, field, nullptr, undoStep);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (appearances, "textureTransform", undoStep);

	getTextureTransformUnlinkButton () .set_sensitive (getTextureTransformComboBoxText () .get_active_row_number () > 0 and textureTransformNode -> getCloneCount () > 1);

	getTextureTransformNameGrid () .set_sensitive (textureTransformNode);

	nodeName .setNode (textureTransformNode);
}

void
X3DTextureTransformNodeEditor::set_textureTransform ()
{
	textureTransformBuffer .addEvent ();
}

void
X3DTextureTransformNodeEditor::set_node ()
{
	undoStep .reset ();

	const auto    executionContext = X3D::MakePtr (getSelectionContext (appearances, true));
	auto          tuple            = getSelection <X3D::X3DTextureTransformNode> (appearances, "textureTransform");
	const int32_t active           = std::get <1> (tuple);
	const bool    hasParent        = std::get <2> (tuple);
	const bool    hasField         = (active not_eq -2);

	textureTransformNode = std::move (std::get <0> (tuple));

	setTextureTransform         (executionContext, textureTransformNode);
	setTextureTransform3D       (executionContext, textureTransformNode);
	setTextureTransformMatrix3D (executionContext, textureTransformNode);

	getTextureTransformNameGrid () .set_sensitive (textureTransformNode);

	nodeName .setNode (textureTransformNode);

	if (not textureTransformNode)
		textureTransformNode = getTextureTransform (textureTransformNode);

	changing = true;

	if (active > 0)
	{
		switch (textureTransformNode -> getType () .back ())
		{
			case X3D::X3DConstants::TextureTransform:
				getTextureTransformComboBoxText () .set_active (1);
				break;
			case X3D::X3DConstants::TextureTransform3D:
				getTextureTransformComboBoxText () .set_active (2);
				break;
			case X3D::X3DConstants::TextureTransformMatrix3D:
				getTextureTransformComboBoxText () .set_active (3);
				break;
			default:
				getTextureTransformComboBoxText () .set_active (-1);
				break;
		}
	}
	else if (active == 0)
		getTextureTransformComboBoxText () .set_active (0);
	else
		getTextureTransformComboBoxText () .set_active (-1);

	getSelectTextureTransformBox ()    .set_sensitive (hasParent);
	getTextureTransformNodeBox ()      .set_sensitive (hasField);
	getTextureTransformUnlinkButton () .set_sensitive (active > 0 and textureTransformNode -> getCloneCount () > 1);

	changing = false;
}

void
X3DTextureTransformNodeEditor::connectTextureTransform (const X3D::SFNode & field)
{
	field .removeInterest (&X3DTextureTransformNodeEditor::connectTextureTransform, this);
	field .addInterest (&X3DTextureTransformNodeEditor::set_textureTransform, this);
}

void
X3DTextureTransformNodeEditor::store ()
{
	X3DTextureTransformMatrix3DEditor::store ();
	X3DTextureTransform3DEditor::store ();
	X3DTextureTransformEditor::store ();
}

} // puck
} // titania
