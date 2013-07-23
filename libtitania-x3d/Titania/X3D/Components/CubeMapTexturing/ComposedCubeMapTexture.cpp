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

#include "ComposedCubeMapTexture.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

ComposedCubeMapTexture::Fields::Fields () :
	front (new SFNode ()),
	back (new SFNode ()),
	left (new SFNode ()),
	right (new SFNode ()),
	bottom (new SFNode ()),
	top (new SFNode ())
{ }

ComposedCubeMapTexture::ComposedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	              transparent (false)
{
	setComponent ("CubeMapTexturing");
	setTypeName ("ComposedCubeMapTexture");

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "front",    front ());
	addField (inputOutput, "back",     back ());
	addField (inputOutput, "left",     left ());
	addField (inputOutput, "right",    right ());
	addField (inputOutput, "bottom",   bottom ());
	addField (inputOutput, "top",      top ());
}

X3DBaseNode*
ComposedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedCubeMapTexture (executionContext);
}

void
ComposedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	if (glXGetCurrentContext ())
		set_children ();
}

void
ComposedCubeMapTexture::set_children ()
{
	glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());

	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	auto _front  = x3d_cast <X3DTexture2DNode*> (front ());
	auto _back   = x3d_cast <X3DTexture2DNode*> (back ());
	auto _left   = x3d_cast <X3DTexture2DNode*> (left ());
	auto _right  = x3d_cast <X3DTexture2DNode*> (right ());
	auto _bottom = x3d_cast <X3DTexture2DNode*> (bottom ());
	auto _top    = x3d_cast <X3DTexture2DNode*> (top ());

	size_t width = 0, height = 0;

	if (_front)
		setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, _back, width, height, true);

	if (_back)
		setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_Z, _front, width, height, false);

	if (_left)
		setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_X, _right, width, height, false);

	if (_right)
		setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_X, _left, width, height, false);

	if (_bottom)
		setTexture (GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, _bottom, width, height, false);

	if (_top)
		setTexture (GL_TEXTURE_CUBE_MAP_POSITIVE_Y, _top, width, height, false);

	glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
}

void
ComposedCubeMapTexture::setTexture (GLenum target, const X3DTexture2DNode* const texture, size_t & w, size_t & h, bool store)
{
	// Get texture 2d data

	GLint width = 0, height = 0;

	glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());

	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,  &width);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	std::vector <char> image (width * height * 4);

	glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
	glBindTexture (GL_TEXTURE_2D, 0);

	// Flip image verticaly and scale if needed

	Magick::Blob blob (image .data (), image .size ());
	{
		Magick::Image mimage;
		mimage .magick ("RGBA");
		mimage .depth (8);
		mimage .size (Magick::Geometry (width, height));
		mimage .read (blob);

		if (store)
		{
			w = width;
			h = height;
		}
		else
		{
			scaleImage (mimage, w, h);
		}

		//mimage .flip ();
		//mimage .flop ();
		mimage .write (&blob);
	}

	// Transfer image

	//applyTextureProperties (textureOptions);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D (target, 0, GL_RGBA, w, h, false, GL_RGBA, GL_UNSIGNED_BYTE, blob .data ());
}

//void
//ComposedCubeMapTexture::applyTextureProperties (const TextureProperties* textureOptions) const
//{
//	glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    textureOptions -> generateMipMaps);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureOptions -> getMinificationFilter ());
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureOptions -> getMagnificationFilter ());
//
//	if (this -> textureOptions)
//	{
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureOptions -> getBoundaryModeS ());
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureOptions -> getBoundaryModeT ());
//	}
//	else
//	{
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [repeatS]);
//		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [repeatT]);
//	}
//
//	glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureOptions -> borderColor .getValue () .data ());
//	glTexParameterf  (GL_TEXTURE_2D, GL_TEXTURE_PRIORITY,     textureOptions -> texturePriority);
//}

void
ComposedCubeMapTexture::draw ()
{
	glMatrixMode (GL_TEXTURE);
	glScalef (-1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glEnable (GL_TEXTURE_CUBE_MAP);
	glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());

	if (glIsEnabled (GL_LIGHTING))
	{
		// Texture color modulates material diffuse color.
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	else
	{
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}
}

} // X3D
} // titania
