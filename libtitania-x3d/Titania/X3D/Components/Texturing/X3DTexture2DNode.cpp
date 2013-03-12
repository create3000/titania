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

X3DTexture2DNode::X3DTexture2DNode () :
	   X3DTextureNode (),     
	          repeatS (true), // SFBool [ ] repeatS            TRUE
	          repeatT (true), // SFBool [ ] repeatT            TRUE
	textureProperties (),     // SFNode [ ] textureProperties  NULL        [TextureProperties]
	        textureId (0),    
	       components (0),    
	      transparent (false) 
{
	addNodeType (X3DConstants::X3DTexture2DNode);
}

void
X3DTexture2DNode::initialize ()
{
	X3DTextureNode::initialize ();

	glGenTextures (1, &textureId);
}

const TextureProperties*
X3DTexture2DNode::getTextureProperties ()
{
	auto _textureProperties = x3d_cast <TextureProperties*> (textureProperties .getValue ());

	return _textureProperties
	       ? _textureProperties
			 : x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties .getValue ());
}

void
X3DTexture2DNode::getImageFormat (Magick::Image & image,
                                  GLenum & format,
                                  const bool compressed)
{
	switch (image .type ())
	{
		case Magick::GrayscaleType:
		{
			if (not image .matte ())
			{
				image .colorSpace (Magick::GRAYColorspace);
				image .magick ("GRAY");
				format      = GL_LUMINANCE;
				components  = 1;
				transparent = false;
				break;
			}
		}
		case Magick::GrayscaleMatteType:
		{
			image .colorSpace (Magick::GRAYColorspace);
			image .type (Magick::TrueColorMatteType);
			image .magick ("RGBA");
			format      = GL_RGBA;
			components  = 2;
			transparent = true;
			break;
		}
		case Magick::TrueColorType:
		{
			if (not image .matte ())
			{
				image .colorSpace (Magick::RGBColorspace);
				image .magick ("RGB");
				format      = GL_RGB;
				components  = 3;
				transparent = false;
				break;
			}
		}
		case Magick::TrueColorMatteType:
		{
			image .colorSpace (Magick::RGBColorspace);
			image .magick ("RGBA");
			format      = GL_RGBA;
			components  = 4;
			transparent = true;
			break;
		}
		default:
		{
			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				return getImageFormat (image, format, compressed);
			}
			else
			{
				image .type (Magick::TrueColorType);
				return getImageFormat (image, format, compressed);
			}
		}
	}
}

void
X3DTexture2DNode::scaleImage (Magick::Image & image)
{
	bool needs_scaling = false;

	size_t width      = image .size () .width ();
	size_t height     = image .size () .height ();
	size_t new_width  = width;
	size_t new_height = height;

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
		std::clog
			<< "Warning: Texture needs scaling: scaling texture from "
			<< width << " × " << height
			<< " to " << new_width << " × " << new_height << " pixel."
			<< std::endl;

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

	bool compressed = false;

	// scale image

	scaleImage (image);

	// get image properties

	GLenum      format;
	std::string magick;
	getImageFormat (image, format, compressed);

	// convert to blob

	Magick::Blob blob;

	image .interlaceType (Magick::NoInterlace);
	image .endian (Magick::LSBEndian);
	image .depth (8);
	image .write (&blob);

	// transfer image
	              
	setImage (components, format, image .size () .width (), image .size () .height (), blob .data ());
}

void
X3DTexture2DNode::setImage (size_t components, GLenum format, GLint width, GLint height, const void* data)
{
	// transfer image

	auto textureProperties = getTextureProperties ();
	
	GLint level = 0; // This texture is level 0 in mimpap generation.

	glBindTexture (GL_TEXTURE_2D, textureId);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	applyTextureProperties (textureProperties);

	glTexImage2D (GL_TEXTURE_2D, level, textureProperties -> getInternalFormat (components),
	              width, height,
	              false, // border
	              format, GL_UNSIGNED_BYTE,
	              data);
}
	              
void
X3DTexture2DNode::updateImage (GLenum format, GLint width, GLint height, const void* data)
{
	// update image

	glBindTexture (GL_TEXTURE_2D, textureId);

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
}

void
X3DTexture2DNode::applyTextureProperties (const TextureProperties* textureProperties) const
{
	glTexParameteri (GL_TEXTURE_2D, GL_GENERATE_MIPMAP,    textureProperties -> generateMipMaps);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureProperties -> getMinificationFilter ());
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureProperties -> getMagnificationFilter ());

	if (this -> textureProperties)
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureProperties -> getBoundaryModeS ());
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureProperties -> getBoundaryModeT ());
	}
	else
	{
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapTypes [repeatS]);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapTypes [repeatT]);
	}

	glTexParameterfv (GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, textureProperties -> borderColor .getValue () .data ());
	glTexParameterf  (GL_TEXTURE_2D, GL_TEXTURE_PRIORITY,     textureProperties -> texturePriority);
}

void
X3DTexture2DNode::draw ()
{
	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);

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
X3DTexture2DNode::dispose ()
{
	if (textureId)
		glDeleteTextures (1, &textureId);

	X3DTextureNode::dispose ();
}

} // X3D
} // titania
