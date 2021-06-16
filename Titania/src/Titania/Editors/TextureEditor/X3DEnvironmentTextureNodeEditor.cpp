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

#include "X3DEnvironmentTextureNodeEditor.h"

namespace titania {
namespace puck {

X3DEnvironmentTextureNodeEditor::X3DEnvironmentTextureNodeEditor () :
	                X3DBaseInterface (),
	       X3DTextureEditorInterface (),
	 X3DComposedCubeMapTextureEditor (),
	X3DGeneratedCubeMapTextureEditor (),
	    X3DImageCubeMapTextureEditor (),
	          environmentTextureNode ()
{
	addChildObjects (environmentTextureNode);
}

void
X3DEnvironmentTextureNodeEditor::configure ()
{
	X3DComposedCubeMapTextureEditor::configure ();
}

void
X3DEnvironmentTextureNodeEditor::setEnvironmentTextureNode (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	setComposedCubeMapTexture  (executionContext, value);
	setGeneratedCubeMapTexture (executionContext, value);
	setImageCubeMapTexture     (executionContext, value);

	setEnvironmentTextureNode (X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> (value), value);
}

const X3D::X3DPtr <X3D::ComposedCubeMapTexture> &
X3DEnvironmentTextureNodeEditor::getComposedCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DComposedCubeMapTextureEditor::getComposedCubeMapTexture (value);

	setEnvironmentTextureNode (X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> (texture), value);

	return texture;
}

const X3D::X3DPtr <X3D::GeneratedCubeMapTexture> &
X3DEnvironmentTextureNodeEditor::getGeneratedCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DGeneratedCubeMapTextureEditor::getGeneratedCubeMapTexture (value);

	setEnvironmentTextureNode (X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> (texture), value);

	return texture;
}

const X3D::X3DPtr <X3D::ImageCubeMapTexture> &
X3DEnvironmentTextureNodeEditor::getImageCubeMapTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	const auto & texture = X3DImageCubeMapTextureEditor::getImageCubeMapTexture (value);

	setEnvironmentTextureNode (X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> (texture), value);

	return texture;
}

void
X3DEnvironmentTextureNodeEditor::setEnvironmentTextureNode (const X3D::X3DPtr <X3D::X3DEnvironmentTextureNode> & texture, 
                                                            const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	environmentTextureNode = texture;

	if (environmentTextureNode == value)
		return;

//	for (const auto & type : value -> getType ())
//	{
//		switch (type)
//		{
//			case X3D::X3DConstants::X3DTexture3DNode :
//				{
//					const X3D::X3DPtr <X3D::X3DTexture3DNode> last (value);
//
//					texture3DNode -> repeatS ()           = last -> repeatS ();
//					texture3DNode -> repeatT ()           = last -> repeatT ();
//					texture3DNode -> repeatR ()           = last -> repeatR ();
//					texture3DNode -> textureProperties () = last -> textureProperties ();
//					break;
//				}
//			default:
//				break;
//		}
//	}
}

void
X3DEnvironmentTextureNodeEditor::store ()
{
	X3DComposedCubeMapTextureEditor::store ();
}

X3DEnvironmentTextureNodeEditor::~X3DEnvironmentTextureNodeEditor ()
{ }

} // puck
} // titania
