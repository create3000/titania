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

#include "X3DTextureEditor.h"

#include "../../ComposedWidgets/TexturePreview.h"

#include <Titania/X3D/Components/ProjectiveTextureMapping/X3DTextureProjectorNode.h>
#include <Titania/X3D/Components/Texturing/ImageTexture.h>
#include <Titania/X3D/Components/Texturing/X3DTexture2DNode.h>

namespace titania {
namespace puck {

X3DTextureEditor::X3DTextureEditor () :
	X3DPrecisionPlacementPanelInterface (),
	                            preview (new TexturePreview (this, getTexturePreviewBox (),  getTextureFormatLabel (),  getTextureLoadStateLabel ())),
	                      projectorNode (),
								      changing (false)
{
	addChildObjects (projectorNode);
}

void
X3DTextureEditor::initialize ()
{ }

void
X3DTextureEditor::set_selection (const X3D::MFNode & selection)
{
	// Get Texture

	if (projectorNode)
		projectorNode -> texture () .removeInterest (&X3DTextureEditor::set_texture, this);

	projectorNode = X3D::X3DPtr <X3D::X3DTextureProjectorNode> (selection .empty () ? nullptr : selection .back ());

	getTextureExpander () .set_visible (projectorNode);

	if (projectorNode)
	{
		projectorNode -> texture () .addInterest (&X3DTextureEditor::set_texture, this);

		set_texture ();
	}
}

void
X3DTextureEditor::on_texture_toggled ()
{
	if (changing)
		return;

	const auto undoStep         = std::make_shared <X3D::UndoStep> ("Toggle Texture Field Of Texture Projector Node");
	const auto executionContext = X3D::X3DExecutionContextPtr (projectorNode -> getExecutionContext ());

	if (getTextureCheckButton () .get_active ())
	{
		const auto texture = executionContext -> createNode <X3D::ImageTexture> ();

		X3D::X3DEditor::replaceNode (executionContext, projectorNode, projectorNode -> texture (), texture, undoStep);
	}
	else
	{
		X3D::X3DEditor::replaceNode (executionContext, projectorNode, projectorNode -> texture (), nullptr, undoStep);
	}

	getBrowserWindow () -> addUndoStep (undoStep);
}

void
X3DTextureEditor::set_texture ()
{
	changing = true;

	const auto texture = X3D::X3DPtr <X3D::X3DTexture2DNode> (projectorNode -> texture ());

	getTextureCheckButton () .set_active (texture);

	preview -> setTexture (texture);

	changing = false;
}

X3DTextureEditor::~X3DTextureEditor ()
{ }

} // puck
} // titania
