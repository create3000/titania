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

#include "PixelTexture.h"

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
	       loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::PixelTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "image",             image ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());
	
	addChildObjects (loadState);
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

	image () .addInterest (&PixelTexture::update, this);

	update ();
}

void
PixelTexture::update ()
{
	const size_t width      = image () .getWidth ();
	const size_t height     = image () .getHeight ();
	const size_t components = image () .getComponents ();
	const auto & array      = image () .getArray ();
	const size_t size       = width * height;

	if (components < 1 or components > 4 or width < 1 or height < 1)
	{
		setTexture (TexturePtr ());
		return;
	}

	if (array .size () < size)
		const_cast <MFInt32 &> (array) .resize (size);

	MagickImageArrayPtr mimages (new MagickImageArray ());
	mimages -> emplace_back ();

	Magick::Image & mimage = mimages -> front ();
	mimage .depth (8);
	mimage .size (Magick::Geometry (width, height));

	switch (components)
	{
		case 1:
		{
			// Copy and flip image vertically.

			std::vector <uint8_t> pixels;
			pixels .reserve (size);

			for (size_t h = 0; h < height; ++ h)
			{
				const auto row = h * width;

				for (size_t w = 0; w < width; ++ w)
					pixels .emplace_back (array [row + w]);
			}

			Magick::Blob blob (pixels .data (), pixels .size ());
			mimage .magick ("GRAY");
			mimage .read (blob);
			break;
		}
		case 2:
		{
			// Copy and flip image vertically.
			
			constexpr auto components = 4;

			std::vector <uint8_t> pixels;
			pixels .reserve (size * components);

			for (size_t h = 0; h < height; ++ h)
			{
				const auto row = h * width;

				for (size_t w = 0; w < width; ++ w)
				{
					const uint32_t pixel = array [row + w];
					const uint8_t  color = pixel >> 8;

					pixels .emplace_back (color);
					pixels .emplace_back (color);
					pixels .emplace_back (color);
					pixels .emplace_back (pixel);
				}
			}

			Magick::Blob blob (pixels .data (), pixels .size ());
			mimage .magick ("RGBA");
			mimage .read (blob);
			break;
		}
		case 3:
		{
			// Copy and flip image vertically.

			std::vector <uint8_t> pixels;
			pixels .reserve (size * components);

			for (size_t h = 0; h < height; ++ h)
			{
				const auto row = h * width;

				for (size_t w = 0; w < width; ++ w)
				{
					const uint32_t pixel = array [row + w];
			
					pixels .emplace_back (pixel >> 16);
					pixels .emplace_back (pixel >> 8);
					pixels .emplace_back (pixel);
				}
			}

			Magick::Blob blob (pixels .data (), pixels .size ());
			mimage .magick ("RGB");
			mimage .read (blob);
			break;
		}
		case 4:
		{
			// Copy and flip image vertically.

			std::vector <uint8_t> pixels;
			pixels .reserve (size * components);

			for (size_t h = 0; h < height; ++ h)
			{
				const auto row = h * width;

				for (size_t w = 0; w < width; ++ w)
				{
					const uint32_t pixel = array [row + w];
			
					pixels .emplace_back (pixel >> 24);
					pixels .emplace_back (pixel >> 16);
					pixels .emplace_back (pixel >> 8);
					pixels .emplace_back (pixel);
				}
			}

			Magick::Blob blob (pixels .data (), pixels .size ());
			mimage .magick ("RGBA");
			mimage .read (blob);
			break;
		}
		default:
			break;
	}

	TexturePtr texture (new Texture (std::move (mimages)));

	texture -> process (getBrowser () -> getMinTextureSize (),
	                    getBrowser () -> getMaxTextureSize ());

	texture -> setComponents (image () .getComponents ());

	setTexture (texture);

	loadState = COMPLETE_STATE;
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
PixelTexture::setImage (const X3D::X3DPtr <X3D::X3DTexture2DNode> & texture2DNode)
{
	if (not texture2DNode)
		throw Error <INVALID_NODE> ("Node is NULL.");

	ContextLock lock (texture2DNode -> getBrowser ());

	const int32_t width      = texture2DNode -> getWidth ();
	const int32_t height     = texture2DNode -> getHeight ();
	const int32_t components = texture2DNode -> components ();
	X3D::MFInt32  array;

	switch (components)
	{
		case 1:
		{
			// Copy image to array.

			const int32_t stride    = 3;
			const int32_t rowStride = width * stride;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const uint8_t* first = image .data ();

			for (int32_t h = 0; h < height; ++ h)
			{
				const auto row = h * rowStride;

				for (int32_t w = 0; w < rowStride; w += stride)
				{
					const auto     p = first + (row + w);
					const uint32_t r = *p;

					array .emplace_back (r);
				}
			}

			break;
		}
		case 2:
		{
			// Copy image to array.

			const auto stride    = 4;
			const auto rowStride = width * stride;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const uint8_t* first = image .data ();

			for (int32_t h = 0; h < height; ++ h)
			{
				const auto row = h * rowStride;

				for (int32_t w = 0; w < rowStride; w += stride)
				{
					const auto     p = first + (row + w);
					const uint32_t r = *p;
					const uint32_t a = *(p + 3);

					array .emplace_back ((r << 8) | (a));
				}
			}

			break;
		}
		case 3:
		{
			// Copy image to array.

			const int32_t stride    = components;
			const int32_t rowStride = width * stride;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const uint8_t* first = image .data ();

			for (int32_t h = 0; h < height; ++ h)
			{
				const auto row = h * rowStride;

				for (int32_t w = 0; w < rowStride; w += stride)
				{
					auto           p = first + (row + w);
					const uint32_t r = *p ++;
					const uint32_t g = *p ++;
					const uint32_t b = *p;

					array .emplace_back ((r << 16) | (g << 8) | (b));
				}
			}

			break;
		}
		case 4:
		{
			// Copy image to array.

			const int32_t stride    = components;
			const int32_t rowStride = width * stride;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const uint8_t* first = image .data ();

			for (int32_t h = 0; h < height; ++ h)
			{
				const auto row = h * rowStride;

				for (int32_t w = 0; w < rowStride; w += stride)
				{
					auto           p = first + (row + w);
					const uint32_t r = *p ++;
					const uint32_t g = *p ++;
					const uint32_t b = *p ++;
					const uint32_t a = *p;

					array .emplace_back ((r << 24) | (g << 16) | (b << 8) | (a));
				}
			}

			break;
		}
		default:
			break;
	}

	image () = SFImage (width, height, components, std::move (array));
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
PixelTexture::setImage (const Cairo::RefPtr <Cairo::ImageSurface> & surface)
{
	auto & array = image () .getArray ();

	const auto width  = surface -> get_width ();
	const auto height = surface -> get_height ();

	image () .setWidth (width);
	image () .setHeight (height);
	image () .setComponents (4);

	bool     transparent = false;
	uint8_t* first       = surface -> get_data ();
	uint8_t* last        = first + 4 * width * height;
	size_t   index       = 0;

	while (first not_eq last)
	{
		auto r = (*first ++) / 255.0f;
		auto g = (*first ++) / 255.0f;
		auto b = (*first ++) / 255.0f;
		auto a = (*first ++) / 255.0f;

		r /= a;
		g /= a;
		b /= a;
		
		transparent |= a not_eq 1;

		uint32_t pixel = 0;

		pixel |= uint32_t (r * 255) << 8;
		pixel |= uint32_t (g * 255) << 16;
		pixel |= uint32_t (b * 255) << 24;
		pixel |= uint32_t (a * 255) << 0;

		const auto column = index % width;
		const auto row    = index / width;

		array [(height - 1 - row) * width + column] = pixel;

		++ index;
	}

	if (not transparent)
	{
		image () .setComponents (3);

		for (MFInt32::reference pixel : array)
			pixel = pixel .get () >> 8;
	}
}

} // X3D
} // titania
