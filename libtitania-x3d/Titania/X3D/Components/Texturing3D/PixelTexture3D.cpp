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

#include "PixelTexture3D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string PixelTexture3D::componentName  = "Texturing3D";
const std::string PixelTexture3D::typeName       = "PixelTexture3D";
const std::string PixelTexture3D::containerField = "texture";

PixelTexture3D::Fields::Fields () :
	image (new MFInt32 ({ 0, 0, 0, 0 }))
{ }

PixelTexture3D::PixelTexture3D (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture3DNode (),
	          fields (),
	       loadState (COMPLETE_STATE)
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "repeatR",           repeatR ());
	addField (inputOutput,    "image",             image ());
	addField (initializeOnly, "textureProperties", textureProperties ());
}

X3DBaseNode*
PixelTexture3D::create (X3DExecutionContext* const executionContext) const
{
	return new PixelTexture3D (executionContext);
}

void
PixelTexture3D::initialize ()
{
	X3DTexture3DNode::initialize ();

	image () .addInterest (this, &PixelTexture3D::update);

	update ();
}

void
PixelTexture3D::update ()
{
	constexpr size_t COMPONENTS = 0;
	constexpr size_t WIDTH      = 1;
	constexpr size_t HEIGHT     = 2;
	constexpr size_t DEPTH      = 3;

	if (image () .size () < 4 or image () [COMPONENTS] < 1 or image () [COMPONENTS] > 4 or image () [WIDTH] <= 0 or image () [HEIGHT] <= 0 or image () [DEPTH] < 1)
	{
		setTexture (Texture3DPtr ());
		return;
	}

	size_t components  = image () [COMPONENTS];
	size_t width       = image () [WIDTH];
	size_t height      = image () [HEIGHT];
	size_t depth       = image () [DEPTH];
	size_t pixels      = width * height;
	size_t pixels3D    = width * height * depth;

	if (image () .size () < 4 + pixels3D)
		image () .resize (4 + pixels3D);

	MagickImageArrayPtr mimages (new MagickImageArray ());

	for (size_t d = 0; d < depth; ++ d)
	{
		size_t first = 4 + d * pixels;
		size_t last  = first + pixels;
	
		mimages -> emplace_back ();
		mimages -> back () .depth (8);
		mimages -> back () .size (Magick::Geometry (width, height));

		switch (components)
		{
			case 1:
			{
				std::vector <uint8_t> array;
				array .reserve (pixels);

				array .assign (image () .begin () + first, image () .begin () + last);

				Magick::Blob blob (array .data (), pixels);
				mimages -> back () .magick ("GRAY");
				mimages -> back () .read (blob);

				break;
			}
			case 2:
			{
				std::vector <uint8_t> array;
				array .reserve (pixels * 4);

				for (const auto & pixel : basic::adapter (image () .begin () + first, image () .begin () + last))
				{
					uint8_t color = pixel >> 8;
					array .emplace_back (color);
					array .emplace_back (color);
					array .emplace_back (color);
					array .emplace_back (pixel);
				}

				Magick::Blob blob (array .data (), pixels * 4);
				mimages -> back () .magick ("RGBA");
				mimages -> back () .read (blob);

				mimages -> back () .type (Magick::GrayscaleMatteType);

				break;
			}
			case 3:
			{
				std::vector <uint8_t> array;
				array .reserve (pixels * 3);

				for (const auto & pixel : basic::adapter (image () .begin () + first, image () .begin () + last))
				{
					array .emplace_back (pixel >> 16);
					array .emplace_back (pixel >> 8);
					array .emplace_back (pixel);
				}

				Magick::Blob blob (array .data (), pixels * 3);
				mimages -> back () .magick ("RGB");
				mimages -> back () .read (blob);

				break;
			}
			case 4:
			{
				std::vector <uint8_t> array;
				array .reserve (pixels * 4);

				for (const auto & pixel : basic::adapter (image () .begin () + first, image () .begin () + last))
				{
					array .emplace_back (pixel >> 24);
					array .emplace_back (pixel >> 16);
					array .emplace_back (pixel >> 8);
					array .emplace_back (pixel);
				}

				Magick::Blob blob (array .data (), pixels * 4);
				mimages -> back () .magick ("RGBA");
				mimages -> back () .read (blob);

				break;
			}
			default:
				break;
		}

		mimages -> back () .flip ();
	}

	Texture3DPtr texture (new Texture3D (std::move (mimages)));

	texture -> process (getBrowser () -> getMinTextureSize (),
	                    getBrowser () -> getMaxTextureSize ());

	texture -> setComponents (components);

	setTexture (texture);
}

} // X3D
} // titania
