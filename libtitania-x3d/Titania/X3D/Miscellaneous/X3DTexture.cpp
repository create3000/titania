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

#include "X3DTexture.h"

#include <Titania/LOG.h>
#include <Titania/Utility/Range.h>

namespace titania {
namespace X3D {

X3DTexture::X3DTexture (MagickImageArrayPtr && images) :
	     images (std::move (images)),
	     format (GL_RGB),
	      width (this -> images -> front () .size () .width ()),
	     height (this -> images -> front () .size () .height ()),
	      depth (this -> images -> size ()),
	 components (0),
	 imageWidth (width),
	imageHeight (height),
	       blob ()
{ }

MagickImageArrayPtr
X3DTexture::readImages (const std::string & data)
{
	MagickImageArrayPtr images (new MagickImageArray);

	Magick::readImages (images .get (), Magick::Blob (data .c_str (), data .length ()));

	return images;
}

void
X3DTexture::refineImageFormats ()
{
	Magick::Image & image = images -> front ();

	switch (image .type ())
	{
		case Magick::GrayscaleType:
		{
			if (not image .matte ())
			{
				image .magick ("GRAY");
				format     = GL_LUMINANCE;
				components = 1;
				break;
			}
		}
		case Magick::GrayscaleMatteType:
		{
			image .type (Magick::TrueColorMatteType);
			image .magick ("RGBA");
			format     = GL_RGBA;
			components = 2;
			break;
		}
		case Magick::TrueColorType:
		{
			if (not image .matte ())
			{
				image .magick ("RGB");
				format     = GL_RGB;
				components = 3;
				break;
			}
		}
		case Magick::TrueColorMatteType:
		{
			image .magick ("RGBA");
			format     = GL_RGBA;
			components = 4;
			break;
		}
		case Magick::BilevelType: // Does not work
		{
			if (image .matte ())
			{
				image .type (Magick::GrayscaleMatteType);
				refineImageFormats ();
				return;
			}
			else
			{
				image .type (Magick::GrayscaleType);
				refineImageFormats ();
				return;
			}
		}
		case Magick::UndefinedType:
		case Magick::PaletteType:
		case Magick::PaletteMatteType:
		case Magick::ColorSeparationType:
		case Magick::ColorSeparationMatteType:
		case Magick::OptimizeType:
		default:
		{
			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				refineImageFormats ();
				return;
			}
			else
			{
				image .type (Magick::TrueColorType);
				refineImageFormats ();
				return;
			}
		}
	}

	for (Magick::Image & next : std::make_pair (++ images -> begin (), images -> end ()))
	{
		next .type (image .type ());
		next .magick (image .magick ());
	}
}

void
X3DTexture::process (const size_type minTextureSize, const size_type maxTextureSize)
{
	refineImageFormats ();

	tryScaleImages (minTextureSize, maxTextureSize);

	writeImages ();

	images .reset ();
}

void
X3DTexture::tryScaleImages (const size_type minTextureSize, const size_type maxTextureSize)
{
	if (minTextureSize == 0)
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

} // X3D
} // titania
