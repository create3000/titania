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

#include "X3DTexture.h"

#include "../../Browser/Networking/config.h"

#include <Titania/LOG.h>
#include <Titania/Utility/Range.h>

#include <fstream>

namespace titania {
namespace X3D {

X3DTexture::X3DTexture (MagickImageArrayPtr && images) :
	      images (std::move (images)),
	       flipY (false),
	      format (GL_RGB),
	       width (this -> images -> front () .size () .width ()),
	      height (this -> images -> front () .size () .height ()),
	       depth (this -> images -> size ()),
	transparency (false),
	  components (0),
	  imageWidth (width),
	 imageHeight (height),
	        blob ()
{ }

MagickImageArrayPtr
X3DTexture::readImages (const std::string & data)
{
	MagickImageArrayPtr images (new MagickImageArray ());

	Magick::readImages (images .get (), Magick::Blob (data .c_str (), data .length ()));

	return images;
}

void
X3DTexture::refineImageFormats ()
{
	Magick::Image & first = images -> front ();

	switch (first .type ())
	{
		case Magick::GrayscaleType:
		{
			if (not first .matte ())
			{
				first .magick ("GRAY");
				format     = GL_LUMINANCE;
				components = 1;
				break;
			}

			[[fallthrough]];
		}
		case Magick::GrayscaleMatteType:
		{
			first .type (Magick::TrueColorMatteType);
			first .magick ("RGBA");
			format     = GL_RGBA;
			components = 2;
			break;
		}
		case Magick::TrueColorType:
		{
			if (not first .matte ())
			{
				first .magick ("RGB");
				format     = GL_RGB;
				components = 3;
				break;
			}

			[[fallthrough]];
		}
		case Magick::TrueColorMatteType:
		{
			first .magick ("RGBA");
			format     = GL_RGBA;
			components = 4;
			break;
		}
		case Magick::BilevelType: // Does not work
		{
			if (first .matte ())
			{
				first .type (Magick::GrayscaleMatteType);
				refineImageFormats ();
				return;
			}
			
			first .type (Magick::GrayscaleType);
			refineImageFormats ();
			return;
		}
		case Magick::UndefinedType:
		case Magick::PaletteType:
		case Magick::PaletteMatteType:
		case Magick::ColorSeparationType:
		case Magick::ColorSeparationMatteType:
		case Magick::OptimizeType:
		default:
		{
			if (first .matte ())
			{
				first .type (Magick::TrueColorMatteType);
				refineImageFormats ();
				return;
			}

			first .type (Magick::TrueColorType);
			refineImageFormats ();
			return;
		}
	}

	for (auto & image : std::make_pair (++ images -> begin (), images -> end ()))
		image .magick (first .magick ());

	// Convert to sRGB profile

	static const Magick::Blob sRGBProfile = getProfile ("/icc/sRGB_IEC61966-2-1_black_scaled.icc");

	for (auto & image : *images)
		image .iccColorProfile (sRGBProfile);
}

void
X3DTexture::process (const size_type minTextureSize, const size_type maxTextureSize)
{
	refineImageFormats ();

	flip ();
	tryScaleImages (minTextureSize, maxTextureSize);

	writeImages ();
	
	setTransparency ();

	images .reset ();
}

void
X3DTexture::flip ()
{
	if (flipY)
	{
		for (auto & image : *images)
			image .flip ();
	}
}

void
X3DTexture::tryScaleImages (const size_type minTextureSize, const size_type maxTextureSize)
{
	if (maxTextureSize == 0)
		return;

	if (std::max (width, height) >= minTextureSize)
	{
		width  = std::min (math::next_power_of_two (width),  maxTextureSize);
		height = std::min (math::next_power_of_two (height), maxTextureSize);
	}

	scaleImages ();
}

void
X3DTexture::scaleImages ()
{
	Magick::Geometry geometry (width, height);

	geometry .aspect (true);

	for (Magick::Image & image : *images)
	{
		const size_type w = image .size () .width ();
		const size_type h = image .size () .height ();

		if (w not_eq width or h not_eq height)
		{
			image .filterType (Magick::LanczosFilter);
			image .zoom (geometry);
		}
	}
}

void
X3DTexture::writeImages ()
{
	for (Magick::Image & image : *images)
	{
		image .interlaceType (Magick::NoInterlace);
		image .endian (Magick::LSBEndian);
		image .depth (8);
	}

	Magick::writeImages (images -> begin (), images -> end (), &blob);
}

void
X3DTexture::setTransparency ()
{
	if (format == GL_RGBA)
	{
		auto       first = static_cast <const uint8_t*> (getData ()) + 3;
		const auto last  = first + getWidth () * getHeight () * getDepth () * 4;

		for (; first < last; first += 4)
		{
			if (*first == 255)
				continue;

			transparency = true;
			break;
		}
	}
}

Magick::Blob
X3DTexture::getProfile (const std::string & profile)
{
	std::ifstream stream (get_data ("/icc/sRGB_IEC61966-2-1_black_scaled.icc") .path (), std::ios::binary);
	
	const auto data = std::vector <char> (std::istreambuf_iterator <char> (stream), std::istreambuf_iterator <char> ());

	return Magick::Blob (data .data (), data .size ());
}

} // X3D
} // titania
