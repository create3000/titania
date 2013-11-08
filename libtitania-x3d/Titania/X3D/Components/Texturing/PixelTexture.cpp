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

#include "PixelTexture.h"

#include "../../Miscellaneous/Texture.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string PixelTexture::componentName  = "Texturing";
const std::string PixelTexture::typeName       = "PixelTexture";
const std::string PixelTexture::containerField = "texture";

PixelTexture::Fields::Fields () :
	image (new SFImage (0, 0, 0, MFInt32 ()))
{ }

PixelTexture::PixelTexture (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture2DNode (),
	          fields (),
	       loadState (COMPLETE_STATE)
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "image",             image ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());
}

X3DBaseNode*
PixelTexture::create (X3DExecutionContext* const executionContext) const
{
	return new PixelTexture (executionContext);
}

void
PixelTexture::initialize ()
{
	X3DTexture2DNode::initialize ();

	image () .addInterest (this, &PixelTexture::update);

	update ();
}

void
PixelTexture::update ()
{
	if (image () .getComponents () < 1 or image () .getComponents () > 4 or image () .getWidth () <= 0 or image () .getHeight () <= 0)
	{
		setTexture (TexturePtr ());
		return;
	}

	size_t pixels = image () .getWidth () * image () .getHeight ();

	Magick::Image mimage;
	mimage .depth (8);
	mimage .size (Magick::Geometry (image () .getWidth (), image () .getHeight ()));

	switch (image () .getComponents ())
	{
		case 1:
		{
			std::vector <uint8_t> array;
			array .reserve (pixels);

			array .assign (image () .getArray () .begin (),
			               image () .getArray () .end ());

			Magick::Blob blob (array .data (), pixels);
			mimage .magick ("GRAY");
			mimage .read (blob);

			break;
		}
		case 2:
		{
			std::vector <uint8_t> array;
			array .reserve (pixels * 2);

			for (const auto & pixel : image () .getArray ())
			{
				uint8_t color = pixel >> 8;
				array .push_back (color);
				array .push_back (color);
				array .push_back (color);
				array .push_back (pixel);
			}

			Magick::Blob blob (array .data (), pixels * 4);
			mimage .magick ("RGBA");
			mimage .read (blob);

			mimage .type (Magick::GrayscaleMatteType);

			break;
		}
		case 3:
		{
			std::vector <uint8_t> array;
			array .reserve (pixels * 3);

			for (const auto & pixel : image () .getArray ())
			{
				array .push_back (pixel >> 16);
				array .push_back (pixel >> 8);
				array .push_back (pixel);
			}

			Magick::Blob blob (array .data (), pixels * 3);
			mimage .magick ("RGB");
			mimage .read (blob);

			break;
		}
		case 4:
		{
			std::vector <uint8_t> array;
			array .reserve (pixels * 4);

			for (const auto & pixel : image () .getArray ())
			{
				array .push_back (pixel >> 24);
				array .push_back (pixel >> 16);
				array .push_back (pixel >> 8);
				array .push_back (pixel);
			}

			Magick::Blob blob (array .data (), pixels * 4);
			mimage .magick ("RGBA");
			mimage .read (blob);

			break;
		}
		default:
			break;
	}

	mimage .flip ();

	TexturePtr texture (new Texture (mimage));

	texture -> process (getTextureProperties () -> borderColor (),
	                    getTextureProperties () -> borderWidth (),
	                    getBrowser () -> getBrowserOptions () -> minTextureSize (),
	                    getBrowser () -> getRenderingProperties () -> maxTextureSize ());

	setTexture (texture);
}

} // X3D
} // titania
