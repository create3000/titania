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

#include "X3DComposedCubeMapTextureEditor.h"

#include "../../ComposedWidgets/TexturePreview.h"

namespace titania {
namespace puck {

X3DComposedCubeMapTextureEditor::X3DComposedCubeMapTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	             frontPreview (new TexturePreview (this, getComposedCubeMapFrontTexturePreviewBox (),  getComposedCubeMapFrontTextureFormatLabel (),  getComposedCubeMapFrontTextureLoadStateLabel ())),
	              backPreview (new TexturePreview (this, getComposedCubeMapBackTexturePreviewBox (),   getComposedCubeMapBackTextureFormatLabel (),   getComposedCubeMapBackTextureLoadStateLabel ())),
	              leftPreview (new TexturePreview (this, getComposedCubeMapLeftTexturePreviewBox (),   getComposedCubeMapLeftTextureFormatLabel (),   getComposedCubeMapLeftTextureLoadStateLabel ())),
	             rightPreview (new TexturePreview (this, getComposedCubeMapRightTexturePreviewBox (),  getComposedCubeMapRightTextureFormatLabel (),  getComposedCubeMapRightTextureLoadStateLabel ())),
	               topPreview (new TexturePreview (this, getComposedCubeMapTopTexturePreviewBox (),    getComposedCubeMapTopTextureFormatLabel (),    getComposedCubeMapTopTextureLoadStateLabel ())),
	            bottomPreview (new TexturePreview (this, getComposedCubeMapBottomTexturePreviewBox (), getComposedCubeMapBottomTextureFormatLabel (), getComposedCubeMapBottomTextureLoadStateLabel ())),
	           cubeMapTexture ()
{
	addChildObjects (cubeMapTexture);
}

void
X3DComposedCubeMapTextureEditor::configure ()
{
	getComposedCubeMapNotebook () .set_current_page (getConfig () -> getItem <int32_t> ("composedCubeMapPage"));
}

void
X3DComposedCubeMapTextureEditor::setComposedCubeMapTexture (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (cubeMapTexture)
	{
		cubeMapTexture -> frontTexture ()  .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
		cubeMapTexture -> backTexture ()   .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
		cubeMapTexture -> leftTexture ()   .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
		cubeMapTexture -> rightTexture ()  .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
		cubeMapTexture -> topTexture ()    .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
		cubeMapTexture -> bottomTexture () .removeInterest (&X3DComposedCubeMapTextureEditor::set_texture, this);
	}

	cubeMapTexture = value;

	getComposedCubeMapTextureBox () .set_visible (cubeMapTexture);

	if (not cubeMapTexture)
		cubeMapTexture = executionContext -> createNode <X3D::ComposedCubeMapTexture> ();

	cubeMapTexture -> frontTexture ()  .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, frontPreview,  std::cref (cubeMapTexture -> frontTexture ()));
	cubeMapTexture -> backTexture ()   .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, backPreview,   std::cref (cubeMapTexture -> backTexture ()));
	cubeMapTexture -> leftTexture ()   .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, leftPreview,   std::cref (cubeMapTexture -> leftTexture ()));
	cubeMapTexture -> righTexturet ()  .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, rightPreview,  std::cref (cubeMapTexture -> rightTexture ()));
	cubeMapTexture -> topTexture ()    .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, topPreview,    std::cref (cubeMapTexture -> topTexture ()));
	cubeMapTexture -> bottomTexture () .addInterest (&X3DComposedCubeMapTextureEditor::set_texture, this, bottomPreview, std::cref (cubeMapTexture -> bottomTexture ()));

	set_texture (frontPreview,  cubeMapTexture -> Texture ());
	set_texture (backPreview,   cubeMapTexture -> backTexture ());
	set_texture (leftPreview,   cubeMapTexture -> leftTexture ());
	set_texture (rightPreview , cubeMapTexture -> rightTexture ());
	set_texture (topPreview,    cubeMapTexture -> topTexture ());
	set_texture (bottomPreview, cubeMapTexture -> bottomTexture ());
}

const X3D::X3DPtr <X3D::ComposedCubeMapTexture> &
X3DComposedCubeMapTextureEditor::getComposedCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getComposedCubeMapTextureBox () .set_visible (value);

	if (value)
	{
		switch (value -> getType () .back ())
		{
			default:
				break;
		}
	}

	return cubeMapTexture;
}

void
X3DComposedCubeMapTextureEditor::set_texture (const std::shared_ptr <TexturePreview> & preview, const X3D::SFNode & texture)
{
	preview -> setTexture (X3D::X3DPtr <X3D::X3DTextureNode> (X3D::X3DPtr <X3D::X3DTexture2DNode> (texture)));
}

void
X3DComposedCubeMapTextureEditor::store ()
{
	getConfig () -> setItem ("composedCubeMapPage", getComposedCubeMapNotebook () .get_current_page ());
}

X3DComposedCubeMapTextureEditor::~X3DComposedCubeMapTextureEditor ()
{
}

} // puck
} // titania
