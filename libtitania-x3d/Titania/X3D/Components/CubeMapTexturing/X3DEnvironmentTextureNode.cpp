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

#include "X3DEnvironmentTextureNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../Shaders/X3DProgrammableShaderObject.h"

namespace titania {
namespace X3D {

const X3DEnvironmentTextureNode::TargetArray X3DEnvironmentTextureNode::targets = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z, // Front
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, // Back
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X, // Left
	GL_TEXTURE_CUBE_MAP_POSITIVE_X, // Right
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // Top
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, // Bottom

};

X3DEnvironmentTextureNode::Fields::Fields () :
	textureProperties (new SFNode ())
{ }

X3DEnvironmentTextureNode::X3DEnvironmentTextureNode () :
	       X3DTextureNode (),
	texturePropertiesNode ()
{
	addType (X3DConstants::X3DEnvironmentTextureNode);

	addChildObjects (texturePropertiesNode);
}

void
X3DEnvironmentTextureNode::initialize ()
{
	X3DTextureNode::initialize ();

	textureProperties () .addInterest (&X3DEnvironmentTextureNode::set_textureProperties, this);

	set_textureProperties ();
}

void
X3DEnvironmentTextureNode::setExecutionContext (X3DExecutionContext* const executionContext)
{
	X3DTextureNode::setExecutionContext (executionContext);

	if (isInitialized ())
		set_textureProperties ();
}

void
X3DEnvironmentTextureNode::set_textureProperties ()
{
	if (texturePropertiesNode)
		texturePropertiesNode -> removeInterest (&X3DEnvironmentTextureNode::updateTextureProperties, this);

	texturePropertiesNode .set (x3d_cast <TextureProperties*> (textureProperties ()));

	if (not texturePropertiesNode)
		texturePropertiesNode .set (x3d_cast <TextureProperties*> (getBrowser () -> getDefaultCubeMapTextureProperties ()));

	texturePropertiesNode -> addInterest (&X3DEnvironmentTextureNode::updateTextureProperties, this);

	updateTextureProperties ();
}

void
X3DEnvironmentTextureNode::setImage (const GLenum target, const GLenum internalFormat, const GLenum format, const void* const data)
{
	updateTextureProperties ();

	glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());

	glTexImage2D (target,
	              0,     // This texture is level 0 in mimpap generation.
	              internalFormat,
	              getWidth (), getHeight (),
	              0, /* std::clamp <int32_t> (texturePropertiesNode -> borderWidth (), 0, 1), */ // This value must be 0.
	              format, GL_UNSIGNED_BYTE,
	              data);

	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
}

void
X3DEnvironmentTextureNode::updateTextureProperties ()
{
	X3DTextureNode::updateTextureProperties (GL_TEXTURE_CUBE_MAP, true, texturePropertiesNode, getWidth (), getHeight (), true, true, true);
}

void
X3DEnvironmentTextureNode::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t stage) const
{
	static const auto textureType = std::vector <GLint> ({ 4 }); 

	glActiveTexture (GL_TEXTURE4);
	glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());
	glUniform1iv (shaderObject -> getTextureTypeUniformLocation (), 1, textureType .data ());
}

} // X3D
} // titania
