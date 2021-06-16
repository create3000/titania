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

#include "X3DTexturePaletteEditor.h"

#include "../../Browser/MagicImport.h"

#include <Titania/X3D/Components/Geometry2D/Rectangle2D.h>
#include <Titania/X3D/Components/Grouping/Transform.h>
#include <Titania/X3D/Components/Shape/Appearance.h>
#include <Titania/X3D/Components/Shape/Shape.h>
#include <Titania/X3D/Components/Texturing/ImageTexture.h>

namespace titania {
namespace puck {

X3DTexturePaletteEditor::X3DTexturePaletteEditor () :
	X3DPaletteEditor <X3DTextureEditorInterface> ("Textures")
{
	setBoxTransparency (1);
}

void
X3DTexturePaletteEditor::initialize ()
{
	X3DPaletteEditor <X3DTextureEditorInterface>::initialize ();
}

void
X3DTexturePaletteEditor::configure ()
{
	X3DPaletteEditor <X3DTextureEditorInterface>::configure ();
}

X3D::SFNode
X3DTexturePaletteEditor::getObject (const basic::uri & URL)
{
	try
	{
		const auto undoStep    = std::make_shared <X3D::UndoStep> (_ ("Import"));
		const auto scene       = getPreview () -> createX3DFromURL ({ URL .str () });
		const auto appearance  = getPreview () -> getExecutionContext () -> createNode <X3D::Appearance> ();
		const auto rectangle   = getPreview () -> getExecutionContext () -> createNode <X3D::Rectangle2D> ();
		const auto shape       = getPreview () -> getExecutionContext () -> createNode <X3D::Shape> ();

		shape -> appearance () = appearance;
		shape -> geometry ()   = rectangle;

		MagicImport (getBrowserWindow ()) .import (getPreview () -> getExecutionContext (), { shape }, scene, undoStep);

		return shape;
	}
	catch (...)
	{
		return nullptr;
	}
}

void
X3DTexturePaletteEditor::setTouchTime (const basic::uri & URL)
{
	try
	{
		auto selection              = getBrowserWindow () -> getSelection () -> getNodes ();
		const auto executionContext = X3D::MakePtr (getSelectionContext (selection, true));

		if (selection .empty ())
			return;

		if (not executionContext)
			return;

		const auto undoStep = std::make_shared <X3D::UndoStep> (_ ("Apply Texture From Palette"));
		const auto scene    = getCurrentBrowser () -> createX3DFromURL ({ URL .str () });

		if (MagicImport (getBrowserWindow ()) .import (executionContext, selection, scene, undoStep))
			getBrowserWindow () -> addUndoStep (undoStep);
	}
	catch (const X3D::X3DError &)
	{ }
}

bool
X3DTexturePaletteEditor::createScene (const X3D::X3DScenePtr & scene, const std::string & name, const size_t position)
{
	try
	{
		const X3D::SFNode texture (getTexture () -> copy (scene, X3D::FLAT_COPY));

		updateNamedNode (scene, name, position, texture);

		// Create scene.

		const auto transform  = scene -> createNode <X3D::Transform> ();
		const auto shape      = scene -> createNode <X3D::Shape> ();
		const auto appearance = scene -> createNode <X3D::Appearance> ();
		const auto rectangle  = scene -> createNode <X3D::Rectangle2D> ();

		scene -> getRootNodes () = X3D::MFNode ({ transform });
		transform -> children () = X3D::MFNode ({ shape });
		shape -> appearance ()   = appearance;
		shape -> geometry ()     = rectangle;
		appearance -> texture () = texture;

		// Setup scene.

		scene -> setMetaData ("titania magic", "Texture");

		return true;
	}
	catch (...)
	{
		return false;
	}
}

void
X3DTexturePaletteEditor::store ()
{
	X3DPaletteEditor <X3DTextureEditorInterface>::store ();
}

X3DTexturePaletteEditor::~X3DTexturePaletteEditor ()
{ }

} // puck
} // titania
