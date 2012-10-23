/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
#include "../Texturing/TextureProperties.h"
#include <Titania/Math/Math.h>

namespace titania {
namespace X3D {

const int X3DTexture2DNode::wrapTypes [2] = { GL_CLAMP, GL_REPEAT };

X3DTexture2DNode::X3DTexture2DNode () :
	     X3DTextureNode (),     
	            repeatS (true), // SFBool [ ] repeatS            TRUE
	            repeatT (true), // SFBool [ ] repeatT            TRUE
	  textureProperties (),     // SFNode [ ] textureProperties  NULL        [TextureProperties]
	          textureId (0),    
	glFrontDiffuseColor (4, 1), 
	 glBackDiffuseColor (4, 1)
{
	addNodeType (X3DTexture2DNodeType);
}

void
X3DTexture2DNode::getImageFormat (Magick::Image & image,
                                  GLint & internalFormat,
                                  GLenum & format,
                                  size_t & channels,
                                  std::string & magick,
                                  const bool compressed)
{
	switch (image .type ())
	{
		case Magick::GrayscaleType:

			if (image .matte ())
			{
				image .colorSpace (Magick::RGBColorspace);
				image .type (Magick::TrueColorMatteType);
				internalFormat = compressed ? GL_COMPRESSED_LUMINANCE_ALPHA : GL_LUMINANCE_ALPHA;
				format         = GL_RGBA;
				channels       = 4;
				magick         = "RGBA";
			}
			else
			{
				image .colorSpace (Magick::GRAYColorspace);
				internalFormat = compressed ? GL_COMPRESSED_LUMINANCE : GL_LUMINANCE;
				format         = GL_LUMINANCE;
				channels       = 1;
				magick         = "GRAY";
			}

			break;

		case Magick::GrayscaleMatteType:
			image .colorSpace (Magick::RGBColorspace);
			internalFormat = compressed ? GL_COMPRESSED_LUMINANCE_ALPHA : GL_LUMINANCE_ALPHA;
			format         = GL_RGBA;
			channels       = 4;
			magick         = "RGBA";
			break;

		case Magick::TrueColorType:
			image .colorSpace (Magick::RGBColorspace);

			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
				format         = GL_RGBA;
				channels       = 4;
				magick         = "RGBA";
			}
			else
			{
				internalFormat = compressed ? GL_COMPRESSED_RGB : GL_RGB;
				format         = GL_RGB;
				channels       = 3;
				magick         = "RGB";
			}

			break;

		case Magick::TrueColorMatteType:
			image .colorSpace (Magick::RGBColorspace);
			internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
			format         = GL_RGBA;
			channels       = 4;
			magick         = "RGBA";
			break;

		default:
			image .colorSpace (Magick::RGBColorspace);

			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				internalFormat = compressed ? GL_COMPRESSED_RGBA : GL_RGBA;
				format         = GL_RGBA;
				channels       = 4;
				magick         = "RGBA";
			}
			else
			{
				image .type (Magick::TrueColorType);
				internalFormat = compressed ? GL_COMPRESSED_RGB : GL_RGB;
				format         = GL_RGB;
				channels       = 3;
				magick         = "RGB";
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
		std::clog << "Info: Texture needs scaling: scaling texture to " << new_width << " × " << new_height << " pixel." << std::endl;
	
		image .filterType (Magick::LanczosFilter);
		Magick::Geometry geometry (new_width, new_height);
		geometry .aspect (true);
		image .zoom (geometry);
	}
}

void
X3DTexture2DNode::bindImage (Magick::Image & image, const GLuint _textureId)
{
	textureId = _textureId;

	// TextureProperties

	GLint level      = 0;
	GLint border     = 0;
	bool  mipmap     = true;
	bool  compressed = true;

	SFNode <TextureProperties> _textureProperties = textureProperties;

	// scale image

	scaleImage (image);

	// get image properties

	GLenum      format;
	size_t      channels;
	std::string magick;
	getImageFormat (image, internalFormat, format, channels, magick, compressed);

	transparency = image .matte ();

	// convert to blob

	Magick::Blob blob;

	//	image .interlaceType(Magick::NoInterlace);
	//	image .endian(Magick::LSBEndian);
	image .depth (8);
	image .magick (magick);
	image .write (&blob);

	// transfer image

	glBindTexture (GL_TEXTURE_2D, textureId);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	if (mipmap) // mipmap
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [repeatS]);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [repeatT]);

	//glTexParameterfv(GL_TEXTURE_BORDER_COLOR, color);
	//glTexParameterf(GL_TEXTURE_PRIORITY, priority);

	glTexImage2D (GL_TEXTURE_2D, level, internalFormat, image .size () .width (), image .size () .height (), border,
	              format, GL_UNSIGNED_BYTE, blob .data ());

	// calculate texture size

	GLint red_size, green_size, blue_size, alpha_size, luminance_size, intensity_size;
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &red_size);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE, &green_size);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE, &blue_size);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE, &alpha_size);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_LUMINANCE_SIZE, &luminance_size);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_INTENSITY_SIZE, &intensity_size);
}

void
X3DTexture2DNode::rebindImage (Magick::Image & image, const GLuint _textureId)
{
	textureId    = _textureId;
	transparency = image .matte ();
	glBindTexture (GL_TEXTURE_2D, textureId);
	glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalFormat);
}

void
X3DTexture2DNode::draw ()
{
	if (not textureId)
		return;

	//	if (isTainted ())
	//		requestImmediateLoad ();

	if (glIsEnabled (GL_LIGHTING))
	{
		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		switch (internalFormat)
		{
			case GL_LUMINANCE:
			case GL_COMPRESSED_LUMINANCE:
			case GL_LUMINANCE_ALPHA:
			case GL_COMPRESSED_LUMINANCE_ALPHA:
				break;
			case GL_RGB:
			case GL_COMPRESSED_RGB:
			case GL_RGBA:
			case GL_COMPRESSED_RGBA:
				getBrowser () -> getObjectAlpha (glFrontDiffuseColor [3], glBackDiffuseColor [3]);
				glMaterialfv (GL_FRONT, GL_DIFFUSE, &glFrontDiffuseColor [0]);
				glMaterialfv (GL_BACK,  GL_DIFFUSE, &glBackDiffuseColor  [0]);
				break;
			default:
				break;
		}
	}
	else
	{
		switch (internalFormat)
		{
			case GL_LUMINANCE:
			case GL_COMPRESSED_LUMINANCE:
			case GL_LUMINANCE_ALPHA:
			case GL_COMPRESSED_LUMINANCE_ALPHA:
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				break;
			case GL_RGB:
			case GL_COMPRESSED_RGB:
			case GL_RGBA:
			case GL_COMPRESSED_RGBA:
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				break;
			default:
				glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				break;
		}
		
	}

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);
}

void
X3DTexture2DNode::dispose ()
{
	X3DTextureNode::dispose ();
}

} // X3D
} // titania
