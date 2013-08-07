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

#include "X3DTexture2DNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Bits/Texture.h"

#include <Titania/Math/Functional.h>
#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

//#define RED_SATURATION_WEIGHT   0.30
//#define GREEN_SATURATION_WEIGHT 0.59
//#define BLUE_SATURATION_WEIGHT  0.11
//
//GLfloat constant[4];
//constant[0] = 0.5f + 0.5f * RED_SATURATION_WEIGHT;
//constant[1] = 0.5f + 0.5f * GREEN_SATURATION_WEIGHT;
//constant[2] = 0.5f + 0.5f * BLUE_SATURATION_WEIGHT;
//constant[3] = 1.0;

const GLint X3DTexture2DNode::wrapTypes [2] = { GL_CLAMP, GL_REPEAT };

X3DTexture2DNode::Fields::Fields () :
	repeatS (new SFBool (true)),
	repeatT (new SFBool (true)),
	textureProperties (new SFNode ())
{ }

X3DTexture2DNode::X3DTexture2DNode () :
	X3DTextureNode (),
	        fields (),
	         width (0),
	        height (0),
	    components (0),
	   transparent (false)
{
	addNodeType (X3DConstants::X3DTexture2DNode);
}

void
X3DTexture2DNode::initialize ()
{
	X3DTextureNode::initialize ();

	repeatS ()           .addInterest (this, &X3DTexture2DNode::updateTextureProperties);
	repeatT ()           .addInterest (this, &X3DTexture2DNode::updateTextureProperties);
	textureProperties () .addInterest (this, &X3DTexture2DNode::update);
}

const TextureProperties*
X3DTexture2DNode::getTextureProperties () const
{
	auto _textureProperties = x3d_cast <TextureProperties*> (textureProperties ());

	if (_textureProperties)
		return _textureProperties;

	return x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties ());
}

void
X3DTexture2DNode::setTexture (const TexturePtr & texture)
{
	if (texture)
	{
		setImage (texture -> getComponents (),
		          texture -> getFormat (),
		          texture -> getWidth (), texture -> getHeight (),
		          texture -> getData ());

		setLoadState (COMPLETE_STATE);
	}
	else
	{
		setImage (3, GL_RGB, 0, 0, nullptr);

		setLoadState (FAILED_STATE);
	}
}

void
X3DTexture2DNode::setImage (size_t comp, GLenum format, GLint w, GLint h, const void* data)
{
	// transfer image

	width       = w;
	height      = h;
	components  = comp;
	transparent = math::is_even (comp);

	glBindTexture (GL_TEXTURE_2D, getTextureId ());

	updateTextureProperties ();

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D (GL_TEXTURE_2D,
	              0,     // This texture is level 0 in mimpap generation.
	              getInternalFormat (),
	              width, height,
	              false, // border
	              format, GL_UNSIGNED_BYTE,
	              data);

	glBindTexture (GL_TEXTURE_2D, 0);
}

void
X3DTexture2DNode::updateImage (GLenum format, GLint width, GLint height, const void* data)
{
	// update image

	glBindTexture (GL_TEXTURE_2D, getTextureId ());
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
	glBindTexture (GL_TEXTURE_2D, 0);
}

void
X3DTexture2DNode::updateTextureProperties () const
{
	auto textureProperties = getTextureProperties ();

	glBindTexture (GL_TEXTURE_2D, getTextureId ());

	if (std::max (width, height) < getBrowser () -> getBrowserOptions () -> minTextureSize ()
	    and textureProperties == x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties ()))
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, false);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [false]);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [false]);
	}
	else
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    textureProperties -> generateMipMaps ());
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureProperties -> getMinificationFilter ());
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureProperties -> getMagnificationFilter ());

		if (this -> textureProperties ())
		{
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureProperties -> getBoundaryModeS ());
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureProperties -> getBoundaryModeT ());
		}
		else
		{
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [repeatS ()]);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [repeatT ()]);
		}
	}

	glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,       textureProperties -> borderColor () .getValue () .data ());
	glTexParameterf  (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, textureProperties -> anisotropicDegree ());
	glTexParameterf  (GL_TEXTURE_2D, GL_TEXTURE_PRIORITY,           textureProperties -> texturePriority ());
}

void
X3DTexture2DNode::notify ()
{
	update ();
}

void
X3DTexture2DNode::draw ()
{
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, getTextureId ());

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

} // X3D
} // titania
