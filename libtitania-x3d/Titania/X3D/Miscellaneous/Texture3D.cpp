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

#include "Texture3D.h"

#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

Texture3D::Texture3D (const MagickImageArrayPtr & images) :
	     width (images -> front () .size () .width ()),
	    height (images -> front () .size () .height ()),
	components (0),
	     depth (images -> size ())
{
	refineImageFormats (images);
	setData (images);
}

void
Texture3D::refineImageFormats (const MagickImageArrayPtr & images)
{
	Magick::Image & image = images -> front ();

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
				break;
			}
		}
		case Magick::GrayscaleMatteType:
		{
			image .colorSpace (Magick::GRAYColorspace);
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
				image .colorSpace (Magick::RGBColorspace);
				image .magick ("RGB");
				format     = GL_RGB;
				components = 3;
				break;
			}
		}
		case Magick::TrueColorMatteType:
		{
			image .colorSpace (Magick::RGBColorspace);
			image .magick ("RGBA");
			format     = GL_RGBA;
			components = 4;
			break;
		}
		default:
		{
			if (image .matte ())
			{
				image .type (Magick::TrueColorMatteType);
				refineImageFormats (images);
				return;
			}
			else
			{
				image .type (Magick::TrueColorType);
				refineImageFormats (images);
				return;
			}
		}
	}
	
	for (Magick::Image & next : basic::adapter (++ images -> begin (), images -> end ()))
	{
		next .colorSpace (image .colorSpace ());
		next .type (image .type ());
		next .magick (image .magick ());
	}
}


void
Texture3D::setData (const MagickImageArrayPtr & images)
{
	size_t bytes = width * height;

	switch (format)
	{
		case GL_RGB:
			bytes *= 3;
			break;
		case GL_RGBA:
			bytes *= 4;
			break;
		default:
			break;
	}

	data .reserve (images -> size () * bytes);

	for (Magick::Image & image : *images)
		addImage (image, bytes);
}

void
Texture3D::addImage (Magick::Image & image, size_t bytes)
{
	Magick::Blob blob;

	image .interlaceType (Magick::NoInterlace);
	image .endian (Magick::LSBEndian);
	image .depth (8);
	image .write (&blob);

	data .insert (data .end (), (char*) blob .data (), ((char*) blob .data ()) + bytes);
}

} // X3D
} // titania
