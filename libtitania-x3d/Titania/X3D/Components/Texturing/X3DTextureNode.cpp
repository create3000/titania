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

#include "X3DTextureNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../Shaders/X3DProgrammableShaderObject.h"
#include "../Texturing/MultiTexture.h"

namespace titania {
namespace X3D {

const GLint X3DTextureNode::wrapTypes [2] = { GL_CLAMP, GL_REPEAT };

X3DTextureNode::X3DTextureNode () :
	X3DAppearanceChildNode (),
	           transparent (false),
	             textureId (0)
{
	addType (X3DConstants::X3DTextureNode);

	addChildObjects (transparent);

	transparent .setAccessType (outputOnly);
}

void
X3DTextureNode::setup ()
{
	X3DAppearanceChildNode::setup ();
}

void
X3DTextureNode::initialize ()
{
	X3DAppearanceChildNode::initialize ();

	glGenTextures (1, &textureId);
}

void
X3DTextureNode::setTransparent (const bool value)
{
	if (value not_eq transparent)
		transparent = value;
}

void
X3DTextureNode::updateTextureProperties (const GLenum target,
                                         const bool haveTextureProperties,
                                         const X3DPtr <TextureProperties> & textureProperties,
                                         const size_t width, 
                                         const size_t height, 
                                         const bool repeatS, 
                                         const bool repeatT, 
                                         const bool repeatR)
{
	glBindTexture (target, getTextureId ());

	if (std::max (width, height) < getBrowser () -> getMinTextureSize () and not haveTextureProperties)
	{
		glTexParameteri (target, GL_GENERATE_MIPMAP,    false);
		glTexParameteri (target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri (target, GL_GENERATE_MIPMAP,    textureProperties -> generateMipMaps ());
		glTexParameteri (target, GL_TEXTURE_MIN_FILTER, textureProperties -> getMinificationFilter ());
		glTexParameteri (target, GL_TEXTURE_MAG_FILTER, textureProperties -> getMagnificationFilter ());
	}

	if (haveTextureProperties)
	{
		glTexParameteri (target, GL_TEXTURE_WRAP_S, textureProperties -> getBoundaryModeS ());
		glTexParameteri (target, GL_TEXTURE_WRAP_T, textureProperties -> getBoundaryModeT ());
		glTexParameteri (target, GL_TEXTURE_WRAP_R, textureProperties -> getBoundaryModeR ());
	}
	else
	{
		glTexParameteri (target, GL_TEXTURE_WRAP_S, wrapTypes [repeatS]);
		glTexParameteri (target, GL_TEXTURE_WRAP_T, wrapTypes [repeatT]);
		glTexParameteri (target, GL_TEXTURE_WRAP_R, wrapTypes [repeatR]);
	}

	glTexParameterfv (target, GL_TEXTURE_BORDER_COLOR,       textureProperties -> borderColor () .getValue () .data ());
	glTexParameterf  (target, GL_TEXTURE_MAX_ANISOTROPY_EXT, textureProperties -> anisotropicDegree ());
	glTexParameterf  (target, GL_TEXTURE_PRIORITY,           textureProperties -> texturePriority ());

	glBindTexture (target, 0);
}

void
X3DTextureNode::setShaderUniforms (X3DProgrammableShaderObject* const shaderObject) const
{
	setShaderUniforms (shaderObject, 0);

	glUniform1i (shaderObject -> getNumTexturesUniformLocation (),               1);
	glUniform1i (shaderObject -> getMultiTextureModeUniformLocation () [0],      int (MultiTexture::ModeType::SELECTARG2));
	glUniform1i (shaderObject -> getMultiTextureAlphaModeUniformLocation () [0], int (MultiTexture::ModeType::SELECTARG2));
	glUniform1i (shaderObject -> getMultiTextureSourceUniformLocation () [0],    int (MultiTexture::SourceType::DEFAULT));
	glUniform1i (shaderObject -> getMultiTextureFunctionUniformLocation () [0],  int (MultiTexture::FunctionType::DEFAULT));
}

void
X3DTextureNode::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (textureId)
			glDeleteTextures (1, &textureId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DAppearanceChildNode::dispose ();
}

} // X3D
} // titania
