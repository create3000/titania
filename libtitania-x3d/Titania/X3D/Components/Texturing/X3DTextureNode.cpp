/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DTextureNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

const GLint X3DTextureNode::wrapTypes [2] = { GL_CLAMP, GL_REPEAT };

X3DTextureNode::X3DTextureNode () :
	X3DAppearanceChildNode (),
	             textureId (0)
{
	addNodeType (X3DConstants::X3DTextureNode);
}

void
X3DTextureNode::initialize ()
{
	X3DAppearanceChildNode::initialize ();

	glGenTextures (1, &textureId);
}

void
X3DTextureNode::updateTextureProperties (const GLenum target,
                                         const bool haveTextureProperties,
                                         const X3DPtr <TextureProperties> & textureProperties,
                                         const int32_t width, 
                                         const int32_t height, 
                                         const bool repeatS, 
                                         const bool repeatT, 
                                         const bool repeatR)
{
	glBindTexture (target, getTextureId ());

	if (std::max (width, height) < getBrowser () -> getBrowserOptions () -> minTextureSize ()
	    and textureProperties == x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties ()))
	{
		glTexParameteri (target, GL_GENERATE_MIPMAP, false);
		glTexParameteri (target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri (target, GL_TEXTURE_WRAP_S, wrapTypes [false]);
		glTexParameteri (target, GL_TEXTURE_WRAP_T, wrapTypes [false]);
		glTexParameteri (target, GL_TEXTURE_WRAP_R, wrapTypes [false]);
	}
	else
	{
		glTexParameteri (target, GL_GENERATE_MIPMAP,    textureProperties -> generateMipMaps ());
		glTexParameteri (target, GL_TEXTURE_MIN_FILTER, textureProperties -> getMinificationFilter ());
		glTexParameteri (target, GL_TEXTURE_MAG_FILTER, textureProperties -> getMagnificationFilter ());

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
	}

	glTexParameterfv (target, GL_TEXTURE_BORDER_COLOR,       textureProperties -> borderColor () .getValue () .data ());
	glTexParameterf  (target, GL_TEXTURE_MAX_ANISOTROPY_EXT, textureProperties -> anisotropicDegree ());
	glTexParameterf  (target, GL_TEXTURE_PRIORITY,           textureProperties -> texturePriority ());

	glBindTexture (target, 0);

	addEvent ();
}

void
X3DTextureNode::draw (const GLenum target, const size_t components)
{
	glEnable (target);
	glBindTexture (target, getTextureId ());

	if (glIsEnabled (GL_LIGHTING))
	{
		// Texture color modulates material diffuse color.
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
		switch (components)
		{
			case 1:
			case 2:
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				break;
			case 3:
			case 4:
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				break;
		}
	}
}

void
X3DTextureNode::dispose ()
{
	if (textureId)
		glDeleteTextures (1, &textureId);

	X3DAppearanceChildNode::dispose ();
}

} // X3D
} // titania
