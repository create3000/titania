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
 ******************************************************************************/

#include "X3DTexture2DNode.h"

#include "../../Browser/Browser.h"
#include <Titania/Math/Math.h>

namespace titania {
namespace X3D {

#define RED_SATURATION_WEIGHT   0.30
#define GREEN_SATURATION_WEIGHT 0.59
#define BLUE_SATURATION_WEIGHT  0.11

const int X3DTexture2DNode::wrapTypes [2] = { GL_CLAMP, GL_REPEAT };

X3DTexture2DNode::X3DTexture2DNode () :
	   X3DTextureNode (),      
	          repeatS (true),  // SFBool [ ] repeatS            TRUE
	          repeatT (true),  // SFBool [ ] repeatT            TRUE
	textureProperties (),      // SFNode [ ] textureProperties  NULL        [TextureProperties]
	        textureId (0),     
	      transparent (false)
{
	addNodeType (X3DTexture2DNodeType);
}

void
X3DTexture2DNode::initialize ()
{
	X3DTextureNode::initialize ();

	glGenTextures (1, &textureId);
}

bool
X3DTexture2DNode::isTransparent ()
{
	return transparent;
}

void
X3DTexture2DNode::setTexture (const GLuint value)
{
	textureId = value;

	GLint alpha;
	glBindTexture (GL_TEXTURE_2D, textureId);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &alpha);

	transparent = alpha;
}

GLuint
X3DTexture2DNode::getTexture ()
{
	return textureId;
}

void
X3DTexture2DNode::getImageFormat (Magick::Image & image,
                                  GLint & internalFormat,
                                  GLenum & format,
                                  const bool compressed)
{
	switch (image .type ())
	{
		case Magick::GrayscaleType:
			image .colorSpace (Magick::GRAYColorspace);

			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				image .magick ("RGBA");
				internalFormat = compressed ? GL_COMPRESSED_LUMINANCE_ALPHA : GL_LUMINANCE_ALPHA;
				format         = GL_RGBA;
				transparent    = true;
			}
			else
			{
				image .magick ("GRAY");
				internalFormat = compressed ? GL_COMPRESSED_LUMINANCE : GL_LUMINANCE;
				format         = GL_LUMINANCE;
				transparent    = false;
			}

			break;

		case Magick::GrayscaleMatteType:
			image .colorSpace (Magick::GRAYColorspace);
			image .type (Magick::TrueColorMatteType);
			image .magick ("RGBA");
			internalFormat = compressed ? GL_COMPRESSED_LUMINANCE_ALPHA : GL_LUMINANCE_ALPHA;
			format         = GL_RGBA;
			transparent    = true;
			break;

		case Magick::TrueColorType:
			image .colorSpace (Magick::RGBColorspace);

			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				image .magick ("RGBA");
				internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
				format         = GL_RGBA;
				transparent    = true;
			}
			else
			{
				image .magick ("RGB");
				internalFormat = compressed ? GL_COMPRESSED_RGB : GL_RGB;
				format         = GL_RGB;
				transparent    = false;
			}

			break;

		case Magick::TrueColorMatteType:
			image .colorSpace (Magick::RGBColorspace);
			image .magick ("RGBA");
			internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
			format         = GL_RGBA;
			transparent    = true;
			break;

		default:
			image .colorSpace (Magick::RGBColorspace);

			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				image .magick ("RGBA");
				internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
				format         = GL_RGBA;
				transparent    = true;
			}
			else
			{
				image .type (Magick::TrueColorType);
				image .magick ("RGB");
				internalFormat = compressed ? GL_COMPRESSED_RGB : GL_RGB;
				format         = GL_RGB;
				transparent    = false;
			}

			break;
	}
}

void
X3DTexture2DNode::scaleImage (Magick::Image & image)
{
	bool   needs_scaling = false;
	size_t new_width     = image .size () .width ();
	size_t new_height    = image .size () .height ();

	GLint max_texture_size;

	glGetIntegerv (GL_MAX_TEXTURE_SIZE, &max_texture_size);

	if (not math::is_power_of_two (new_width))
	{
		new_width     = std::min (math::next_power_of_two (new_width), (size_t) max_texture_size);
		needs_scaling = true;
	}

	if (not math::is_power_of_two (new_height))
	{
		new_height    = std::min (math::next_power_of_two (new_height), (size_t) max_texture_size);
		needs_scaling = true;
	}

	if (needs_scaling)
	{
		std::clog << "Warning: Texture needs scaling: scaling texture to " << new_width << " × " << new_height << " pixel." << std::endl;

		image .filterType (Magick::LanczosFilter);
		Magick::Geometry geometry (new_width, new_height);
		geometry .aspect (true);
		image .zoom (geometry);
	}
}

void
X3DTexture2DNode::setImage (Magick::Image & image)
{
	// TextureProperties

	GLint level      = 0;
	GLint border     = 0;
	bool  mipmap     = true;
	bool  compressed = true;

	SFNode <TextureProperties> _textureProperties = textureProperties;

	// scale image

	scaleImage (image);

	// get image properties

	GLint       internalFormat;
	GLenum      format;
	std::string magick;
	getImageFormat (image, internalFormat, format, compressed);

	transparent = image .matte ();

	// convert to blob

	Magick::Blob blob;

	//	image .interlaceType(Magick::NoInterlace);
	//	image .endian(Magick::LSBEndian);
	image .depth (8);
	image .write (&blob);

	// transfer image

	glBindTexture (GL_TEXTURE_2D, textureId);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	if (mipmap) // mipmap
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    GL_TRUE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    GL_FALSE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [repeatS]);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [repeatT]);

	//glTexParameterfv(GL_TEXTURE_BORDER_COLOR, color);
	//glTexParameterf(GL_TEXTURE_PRIORITY, priority);

	glTexImage2D (GL_TEXTURE_2D, level, internalFormat,
	              image .size () .width (), image .size () .height (),
	              border,
	              format, GL_UNSIGNED_BYTE,
	              blob .data ());
}

void
X3DTexture2DNode::draw ()
{
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);

	if (glIsEnabled (GL_LIGHTING))
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	else
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void
X3DTexture2DNode::deleteTexture ()
{
	if (textureId)
	{
		glDeleteTextures (1, &textureId);
		textureId = 0;
	}
}

} // X3D
} // titania
