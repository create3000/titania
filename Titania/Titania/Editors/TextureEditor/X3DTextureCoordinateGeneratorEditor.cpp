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

#include "X3DTextureCoordinateGeneratorEditor.h"

#include <Titania/X3D/Components/Texturing/TextureCoordinateGenerator.h>

namespace titania {
namespace puck {

X3DTextureCoordinateGeneratorEditor::X3DTextureCoordinateGeneratorEditor () :
	       X3DTextureEditorInterface (),
	                   geometryNodes (),
	textureCoordinateGeneratorBuffer (),
	      textureCoordinateGenerator (),
	                        undoStep (),
	                        changing (false),
	                        nodeName (this, getTextureCoordinateGeneratorNameEntry (), getTextureCoordinateGeneratorRenameButton ()),
	                            mode (this, getTextureCoordinateGeneratorModeComboBoxText (), "mode"),
	                      parameter0 (this, getTextureCoordinateGeneratorParameter0Adjustment (), getTextureCoordinateGeneratorParameter0SpinButton (), "parameter"),
	                      parameter1 (this, getTextureCoordinateGeneratorParameter1Adjustment (), getTextureCoordinateGeneratorParameter1SpinButton (), "parameter"),
	                      parameter2 (this, getTextureCoordinateGeneratorParameter2Adjustment (), getTextureCoordinateGeneratorParameter2SpinButton (), "parameter"),
	                      parameter3 (this, getTextureCoordinateGeneratorParameter3Adjustment (), getTextureCoordinateGeneratorParameter3SpinButton (), "parameter"),
	                      parameter4 (this, getTextureCoordinateGeneratorParameter4Adjustment (), getTextureCoordinateGeneratorParameter4SpinButton (), "parameter"),
	                      parameter5 (this, getTextureCoordinateGeneratorParameter5Adjustment (), getTextureCoordinateGeneratorParameter5SpinButton (), "parameter")
{
	addChildObjects (geometryNodes, textureCoordinateGeneratorBuffer, textureCoordinateGenerator);

	textureCoordinateGeneratorBuffer .addInterest (&X3DTextureCoordinateGeneratorEditor::set_node, this);

	parameter0 .setIndex (0);
	parameter1 .setIndex (1);
	parameter2 .setIndex (2);
	parameter3 .setIndex (3);
	parameter4 .setIndex (4);
	parameter5 .setIndex (5);
}

void
X3DTextureCoordinateGeneratorEditor::initialize ()
{ }

void
X3DTextureCoordinateGeneratorEditor::set_selection (const X3D::MFNode & selection)
{
	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .removeInterest (&X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	geometryNodes = getNodes <X3D::X3DGeometryNode> (selection, { X3D::X3DConstants::X3DGeometryNode });

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			geometry -> getField <X3D::SFFloat> ("texCoord") .addInterest (&X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator, this);
		}
		catch (const X3D::X3DError &)
		{ }
	}

	set_textureCoordinateGenerator ();
}

/***********************************************************************************************************************
 *
 *  textureCoordinateGenerator
 *
 **********************************************************************************************************************/

void
X3DTextureCoordinateGeneratorEditor::on_textureCoordinateGenerator_unlink_clicked ()
{
	unlinkClone (geometryNodes, "texCoord", undoStep);
}

void
X3DTextureCoordinateGeneratorEditor::on_textureCoordinateGenerator_toggled ()
{
	if (changing)
		return;

	const auto executionContext = X3D::MakePtr (getSelectionContext (geometryNodes, true));

	addUndoFunction <X3D::SFNode> (geometryNodes, "texCoord", undoStep);

	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (false);
	getTextureCoordinateGeneratorBox ()         .set_sensitive (getTextureCoordinateGeneratorCheckButton () .get_active ());

	for (const auto & geometry : geometryNodes)
	{
		try
		{
			auto & field = geometry -> getField <X3D::SFNode> ("texCoord");

			field .removeInterest (&X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator, this);
			field .addInterest (&X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator, this);

			if (getTextureCoordinateGeneratorCheckButton () .get_active ())
			{
				try
				{
					X3D::X3DEditor::setValue (geometry, geometry -> getField <X3D::MFInt32> ("texCoordIndex"), X3D::MFInt32 (), undoStep);
				}
				catch (const X3D::X3DError & error)
				{ }

				X3D::X3DEditor::replaceNode (executionContext, geometry, field, textureCoordinateGenerator, undoStep);
			}
			else
			{
				X3D::X3DEditor::replaceNode (executionContext, geometry, field, nullptr, undoStep);
			}
		}
		catch (const X3D::X3DError &)
		{ }
	}

	addRedoFunction <X3D::SFNode> (geometryNodes, "texCoord", undoStep);

	getTextureCoordinateGeneratorUnlinkButton () .set_sensitive (getTextureCoordinateGeneratorCheckButton () .get_active () and textureCoordinateGenerator -> getCloneCount () > 1);
}

void
X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator ()
{
	textureCoordinateGeneratorBuffer .addEvent ();
}

void
X3DTextureCoordinateGeneratorEditor::set_node ()
{
	undoStep .reset ();

	const auto executionContext = getSelectionContext (geometryNodes, true);
	auto       tuple            = getSelection <X3D::TextureCoordinateGenerator> (geometryNodes, "texCoord");
	const int  active           = std::get <1> (tuple);
	const bool hasParent        = std::get <2> (tuple);
	const bool hasField         = (active not_eq -2);

	textureCoordinateGenerator = std::move (std::get <0> (tuple));

	if (not textureCoordinateGenerator)
		textureCoordinateGenerator = executionContext -> createNode <X3D::TextureCoordinateGenerator> ();

	changing = true;

	getSelectTextureCoordinateGeneratorBox ()   .set_sensitive (hasParent);
	getTextureCoordinateGeneratorMainBox ()     .set_sensitive (hasField);
	getTextureCoordinateGeneratorCheckButton () .set_active (active > 0);
	getTextureCoordinateGeneratorCheckButton () .set_inconsistent (active < 0);

	getTextureCoordinateGeneratorUnlinkButton () .set_sensitive (active > 0 and textureCoordinateGenerator -> getCloneCount () > 1);
	getTextureCoordinateGeneratorBox ()          .set_sensitive (active > 0);

	changing = false;

	const X3D::MFNode nodes = { textureCoordinateGenerator };

	nodeName .setNode (nodes .back ());

	mode       .setNodes (nodes);
	parameter0 .setNodes (nodes);
	parameter1 .setNodes (nodes);
	parameter2 .setNodes (nodes);
	parameter3 .setNodes (nodes);
	parameter4 .setNodes (nodes);
	parameter5 .setNodes (nodes);
}

void
X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator (const X3D::SFNode & field)
{
	field .removeInterest (&X3DTextureCoordinateGeneratorEditor::connectTextureCoordinateGenerator, this);
	field .addInterest (&X3DTextureCoordinateGeneratorEditor::set_textureCoordinateGenerator, this);
}

X3DTextureCoordinateGeneratorEditor::~X3DTextureCoordinateGeneratorEditor ()
{ }

} // puck
} // titania
