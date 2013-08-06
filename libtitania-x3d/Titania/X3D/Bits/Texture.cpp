/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "Texture.h"

#include <iomanip>
#include <iostream>

namespace titania {
namespace X3D {

Texture::Texture (const Magick::Image & image) :
	     image (image),
	    format (GL_RGB),
	components (3),
	      blob ()
{ }

Texture::Texture (const std::string & data) :
	     image (getImage (data)),
	    format (GL_RGB),
	components (3),
	      blob ()
{ }

void
Texture::process (const Color4f & borderColor, size_type borderWidth,
                  size_type minTextureSize, size_type maxTextureSize)
{
	addBorder (borderColor, borderWidth);

	tryScaleImage (minTextureSize, maxTextureSize);

	refineImageFormat ();

	writeBlob ();
}

Magick::Image
Texture::getImage (const std::string & data)
{
	std::list <Magick::Image> images;
	Magick::readImages (&images, Magick::Blob (data .c_str (), data .length ()));

	switch (images .size ())
	{
		case 0:  // I have no idea what to do now.
			throw std::domain_error ("Image contains nothing.");

		case 1:
		{
			// Image with one layer image.
			return images .back ();
		}
		default:
		{
			// Flatten image with more than one layer.
			Magick::Image image;
			Magick::flattenImages (&image, images .begin (), images .end ());
			return image;
		}
	}
}

void
Texture::addBorder (const Color4f & borderColor, size_type borderWidth)
{
	if (borderWidth > 0)
	{
		std::ostringstream color;

		color
			<< std::hex
			<< '#'
			<< std::setfill ('0')
			<< std::setw (2) << (int) (uint8_t) (borderColor .r () * 255)
			<< std::setw (2) << (int) (uint8_t) (borderColor .g () * 255)
			<< std::setw (2) << (int) (uint8_t) (borderColor .b () * 255)
			<< std::setw (2) << (int) (uint8_t) (borderColor .a () * 255);

		image .borderColor (Magick::Color (color .str ()));
		
		if (borderColor .a () < 1)
			image .matte (true);

		image .border (Magick::Geometry (borderWidth, borderWidth));
	}
}

void
Texture::tryScaleImage (size_type minTextureSize, size_type maxTextureSize)
{
	size_type width  = getWidth ();
	size_type height = getHeight ();

	if (std::max (width, height) < minTextureSize)
		return;

	bool needsScaling = false;

	if (not math::is_power_of_two (width))
	{
		width        = std::min (math::next_power_of_two (width), maxTextureSize);
		needsScaling = true;
	}

	if (not math::is_power_of_two (height))
	{
		height       = std::min (math::next_power_of_two (height), maxTextureSize);
		needsScaling = true;
	}

	if (needsScaling)
	{
//		std::clog
//			<< "Warning: Texture needs scaling: scaling texture from "
//			<< getWidth () << " x " << getHeight ()
//			<< " to " << width << " x " << height << " pixel."
//			<< std::endl;

		scaleImage (width, height);
	}
}

void
Texture::scaleImage (size_type width, size_type height)
{
	Magick::Geometry geometry (width, height);

	geometry .aspect (true);
	image .filterType (Magick::LanczosFilter);
	image .zoom (geometry);
}

void
Texture::refineImageFormat ()
{
	switch (image .type ())
	{
		case Magick::GrayscaleType:
		{
			if (not image .matte ())
			{
				image .colorSpace (Magick::GRAYColorspace);
				image .magick ("GRAY");
				format     = GL_LUMINANCE;
				components = 1;
				return;
			}
		}
		case Magick::GrayscaleMatteType:
		{
			image .colorSpace (Magick::GRAYColorspace);
			image .type (Magick::TrueColorMatteType);
			image .magick ("RGBA");
			format     = GL_RGBA;
			components = 2;
			return;
		}
		case Magick::TrueColorType:
		{
			if (not image .matte ())
			{
				image .colorSpace (Magick::RGBColorspace);
				image .magick ("RGB");
				format     = GL_RGB;
				components = 3;
				return;
			}
		}
		case Magick::TrueColorMatteType:
		{
			image .colorSpace (Magick::RGBColorspace);
			image .magick ("RGBA");
			format     = GL_RGBA;
			components = 4;
			return;
		}
		default:
		{
			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				refineImageFormat ();
				return;
			}
			else
			{
				image .type (Magick::TrueColorType);
				refineImageFormat ();
				return;
			}
		}
	}
}

void
Texture::writeBlob ()
{
	image .interlaceType (Magick::NoInterlace);
	image .endian (Magick::LSBEndian);
	image .depth (8);
	image .write (&blob);
}

} // X3D
} // titania
