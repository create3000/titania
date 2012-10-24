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

#include "PixelTexture.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

PixelTexture::PixelTexture (X3DExecutionContext* const executionContext) :
	    X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DTexture2DNode (),                                 
	           image (0, 0, 0, MFInt32 ())              // SFImage [in,out] image  0 0 0
{
	setComponent ("Texturing");
	setTypeName ("PixelTexture");

	appendField (inputOutput,    "metadata",          metadata);
	appendField (initializeOnly, "repeatS",           repeatS);
	appendField (initializeOnly, "repeatT",           repeatT);
	appendField (initializeOnly, "textureProperties", textureProperties);
	appendField (inputOutput,    "image",             image);
}

X3DBasicNode*
PixelTexture::create (X3DExecutionContext* const executionContext) const
{
	return new PixelTexture (executionContext);
}

void
PixelTexture::initialize ()
{
	X3DTexture2DNode::initialize ();

	requestImmediateLoad ();
}

void
PixelTexture::requestImmediateLoad ()
{
	if (not getTexture () or image .getComp () < 1 or image .getComp () > 4 or image .getWidth () <= 0 or image .getHeight () <= 0)
		return;

	size_t pixels = image .getWidth () * image .getHeight ();

	std::vector <uint8_t> array;
	Magick::Image         mimage;
	mimage .depth (8);
	mimage .size (Magick::Geometry (image .getWidth (), image .getHeight ()));

	if (image .getComp () == 1)
	{
		array .reserve (pixels);

		for (int h = image .getHeight () - 1; h >= 0; -- h)
		{
			for (SFImage::size_type w = 0; w < image .getWidth (); ++ w)
			{
				size_t value = image .getArray () [(h * image .getWidth ()) + w];
				array .push_back (value);
			}
		}

		Magick::Blob blob (&array [0], array .size ());
		mimage .magick ("GRAY");
		mimage .read (blob);
	}
	else if (image .getComp () == 2)
	{
		array .reserve (pixels * 4);

		for (int h = image .getHeight () - 1; h >= 0; -- h)
		{
			for (SFImage::size_type w = 0; w < image .getWidth (); ++ w)
			{
				size_t value = image .getArray () [(h * image .getWidth ()) + w];
				array .push_back (value >> 8);
				array .push_back (value >> 8);
				array .push_back (value >> 8);
				array .push_back (value); // alpha
			}
		}

		Magick::Blob blob (&array [0], array .size ());
		mimage .magick ("RGBA");
		mimage .read (blob);
		mimage .type (Magick::GrayscaleMatteType);
	}
	else if (image .getComp () == 3)
	{
		array .reserve (pixels * 3);

		for (int h = image .getHeight () - 1; h >= 0; -- h)
		{
			for (SFImage::size_type w = 0; w < image .getWidth (); ++ w)
			{
				size_t value = image .getArray () [(h * image .getWidth ()) + w];
				array .push_back (value >> 16);
				array .push_back (value >> 8);
				array .push_back (value);
			}
		}

		Magick::Blob blob (&array [0], array .size ());
		mimage .magick ("RGB");
		mimage .read (blob);
	}
	else if (image .getComp () == 4)
	{
		array .reserve (pixels * 4);

		for (int h = image .getHeight () - 1; h >= 0; -- h)
		{
			for (SFImage::size_type w = 0; w < image .getWidth (); ++ w)
			{
				size_t value = image .getArray () [(h * image .getWidth ()) + w];
				array .push_back (value >> 24);
				array .push_back (value >> 16);
				array .push_back (value >> 8);
				array .push_back (value); // alpha
			}
		}

		Magick::Blob blob (&array [0], array .size ());
		mimage .magick ("RGBA");
		mimage .read (blob);
	}

	setImage (mimage);
}

void
PixelTexture::dispose ()
{
	deleteTexture ();

	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
