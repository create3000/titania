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

#include "PixelTexture3D.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

constexpr size_t OFFSET     = 4;
constexpr size_t COMPONENTS = 0;
constexpr size_t WIDTH      = 1;
constexpr size_t HEIGHT     = 2;
constexpr size_t DEPTH      = 3;

const Component   PixelTexture3D::component      = Component ("Texturing3D", 1);
const std::string PixelTexture3D::typeName       = "PixelTexture3D";
const std::string PixelTexture3D::containerField = "texture";

PixelTexture3D::Fields::Fields () :
	image (new MFInt32 ({ 0, 0, 0, 0 }))
{ }

PixelTexture3D::PixelTexture3D (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture3DNode (),
	          fields (),
	       loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::PixelTexture3D);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "image",             image ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "repeatR",           repeatR ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (loadState);
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

	image () .addInterest (&PixelTexture3D::update, this);

	update ();
}

void
PixelTexture3D::update ()
{
	if (image () .size () < OFFSET)
	{
		setTexture (Texture3DPtr ());
		return;
	}

	const auto & image       = this -> image ();
	const size_t components  = std::max <int32_t> (0, image [COMPONENTS]);
	const size_t width       = std::max <int32_t> (0, image [WIDTH]);
	const size_t height      = std::max <int32_t> (0, image [HEIGHT]);
	const size_t depth       = std::max <int32_t> (0, image [DEPTH]);
	const size_t size        = width * height;
	const size_t size3D      = width * height * depth;

	if (components < 1 or components > 4 or width < 1 or height < 1 or depth < 1)
	{
		setTexture (Texture3DPtr ());
		return;
	}

	if (image .size () < OFFSET + size3D)
		const_cast <MFInt32 &> (image) .resize (OFFSET + size3D);

	std::vector <uint8_t> pixels;
	GLenum format = GL_LUMINANCE;

	for (size_t d = 0; d < depth; ++ d)
	{
		const size_t first = OFFSET + d * size;

		switch (components)
		{
			case 1:
			{
				// Copy and flip image vertically.

				format = GL_LUMINANCE;

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
						pixels .emplace_back (image [first + row + w]);
				}

				break;
			}
			case 2:
			{
				// Copy and flip image vertically.

				format = GL_LUMINANCE_ALPHA;

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto &  pixel = image [first + row + w];
						const uint8_t color = pixel >> 8;

						pixels .emplace_back (color);
						pixels .emplace_back (pixel);
					}
				}

				break;
			}
			case 3:
			{
				// Copy and flip image vertically.

				format = GL_RGB;

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto & pixel = image [first + row + w];

						pixels .emplace_back (pixel >> 16);
						pixels .emplace_back (pixel >> 8);
						pixels .emplace_back (pixel);
					}
				}

				break;
			}
			case 4:
			{
				// Copy and flip image vertically.

				format = GL_RGBA;

				for (size_t h = 0; h < height; ++ h)
				{
					const auto row = h * width;

					for (size_t w = 0; w < width; ++ w)
					{
						const auto & pixel = image [first + row + w];

						pixels .emplace_back (pixel >> 24);
						pixels .emplace_back (pixel >> 16);
						pixels .emplace_back (pixel >> 8);
						pixels .emplace_back (pixel);
					}
				}

				break;
			}
			default:
				break;
		}
	}

	setTexture (Texture3DPtr (new Texture3D (components, width, height, depth, format, std::move (pixels))));

	loadState = COMPLETE_STATE;
}

///  throws Error <INVALID_NODE>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
PixelTexture3D::setImage (const X3D::X3DPtr <X3D::X3DTexture3DNode> & texture3DNode)
{
	if (not texture3DNode)
		throw Error <INVALID_NODE> ("Node is NULL.");

	ContextLock lock (texture3DNode -> getBrowser ());

	auto &        image      = this -> image ();
	const int32_t width      = texture3DNode -> getWidth ();
	const int32_t height     = texture3DNode -> getHeight ();
	const int32_t depth      = texture3DNode -> depth ();
	const int32_t components = texture3DNode -> components ();


	image .resize (OFFSET);

	image [WIDTH]      = width;
	image [HEIGHT]     = height;
	image [DEPTH]      = depth;
	image [COMPONENTS] = components;

	switch (components)
	{
		case 1:
		{
			// Copy and flip image vertically.

			const int32_t stride    = 3;
			const int32_t rowStride = width * stride;
			const int32_t size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (int32_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;

				for (int32_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;

					for (int32_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);

						image .emplace_back (*p);
					}
				}
			}

			break;
		}
		case 2:
		{
			// Copy and flip image vertically.

			const int32_t stride    = 4;
			const int32_t rowStride = width * stride;
			const int32_t size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (int32_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;

				for (int32_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;

					for (int32_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);

						uint32_t point = *p << 8; // The value is in the red channel.
						p     += 3;
						point |= *p;

						image .emplace_back (point);
					}
				}
			}

			break;
		}
		case 3:
		{
			// Copy and flip image vertically.

			const int32_t stride    = components;
			const int32_t rowStride = width * stride;
			const int32_t size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGB, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (int32_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;

				for (int32_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;

					for (int32_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);

						uint32_t point = *p ++ << 16;
						point |= *p ++ << 8;
						point |= *p;

						image .emplace_back (point);
					}
				}
			}

			break;
		}
		case 4:
		{
			// Copy and flip image vertically.

			const int32_t stride    = components;
			const int32_t rowStride = width * stride;
			const int32_t size      = width * height * stride;

			std::vector <uint8_t> images (size * depth);

			glBindTexture (GL_TEXTURE_3D, texture3DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_3D, 0, GL_RGBA, GL_UNSIGNED_BYTE, images .data ());
			glBindTexture (GL_TEXTURE_3D, 0);

			for (int32_t i = 0; i < depth; ++ i)
			{
				const uint8_t* first = images .data () + i * size;

				for (int32_t h = 0; h < height; ++ h)
				{
					const auto row = h * rowStride;

					for (int32_t w = 0; w < rowStride; w += stride)
					{
						auto p = first + (row + w);

						uint32_t point = *p ++ << 24;
						point |= *p ++ << 16;
						point |= *p ++ << 8;
						point |= *p;

						image .emplace_back (point);
					}
				}
			}

			break;
		}
		default:
			break;
	}
}

} // X3D
} // titania
