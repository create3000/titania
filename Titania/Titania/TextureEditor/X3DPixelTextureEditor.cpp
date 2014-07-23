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

#include "X3DPixelTextureEditor.h"

namespace titania {
namespace puck {

X3DPixelTextureEditor::X3DPixelTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface ("", ""),
	             pixelTexture ()
{ }

void
X3DPixelTextureEditor::setPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (pixelTexture)
	{
		//pixelTexture -> url () .removeInterest (this, &X3DPixelTextureEditor::set_url);
	}

	pixelTexture = value;

	getPixelTextureBox () .set_visible (pixelTexture);

	if (not pixelTexture)
	{
		pixelTexture = new X3D::PixelTexture (getExecutionContext ());
		getExecutionContext () -> addUninitializedNode (pixelTexture);
		getExecutionContext () -> realize ();
	}

	//pixelTexture -> url () .addInterest (this, &X3DPixelTextureEditor::set_url);

	//set_url ();
}
	
const X3D::X3DPtr <X3D::PixelTexture> &
X3DPixelTextureEditor::getPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getPixelTextureBox () .set_visible (value);

	if (not value)
		return pixelTexture;

	switch (value -> getType () .back ()) 
	{
		case X3D::X3DConstants::ImageTexture:
		{
			assign (X3D::X3DPtr <X3D::ImageTexture> (value));
			break;
		}
		case X3D::X3DConstants::PixelTexture:
		{
			const X3D::X3DPtr <X3D::PixelTexture> last (value);

			pixelTexture -> image () = last -> image ();
			break;
		}
		default:
			break;
	}

	return pixelTexture;
}

void
X3DPixelTextureEditor::assign (const X3D::X3DPtr <X3D::ImageTexture> & imageTexture)
{	
	if (not imageTexture -> getBrowser () -> makeCurrent ())
		return;

	const auto   width      = imageTexture -> getWidth ();
	const auto   height     = imageTexture -> getHeight ();
	const auto   components = imageTexture -> getComponents ();
	X3D::MFInt32 array;

	switch (components)
	{
		case 1:
		{
//				const auto     stride = 4;
//				const uint8_t* first  = static_cast <uint8_t*> (image .data ());
//				const uint8_t* last   = first + imageTexture -> getWidth () * imageTexture -> getHeight () * stride;
//
//				while (first not_eq last)
//					array .emplace_back (*first ++);
//
			break;
		}
		case 2:
		{
//				const auto      stride = 4;
//				const uint32_t* first  = static_cast <uint32_t*> ((void*) image .data ());
//				const uint32_t* last   = first + imageTexture -> getWidth () * imageTexture -> getHeight () * stride;
//
//				while (first not_eq last)
//					array .emplace_back (*first ++ & 0x0000ffff);
//
			break;
		}
		case 3:
		{
			// Copy and flip image vertically.

			const auto stride    = components;
			const auto rowStride = width * stride;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, imageTexture -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const uint8_t* first = static_cast <uint8_t*> (image .data ());

			for (size_t h = 0; h < height; ++ h)
			{
				const auto row = (height - 1 - h) * rowStride;

				for (size_t w = 0; w < rowStride; w += stride)
				{
					auto p = first + (row + w);

					uint32_t point = 0;

					point |= *p ++ << 16;
					point |= *p ++ << 8;
					point |= *p;

					array .emplace_back (point);
				}
			}

//			while (first not_eq last)
//			{
//				int32_t point = 0;
//
//				point |= *first ++ << 16;
//				point |= *first ++ << 8;
//				point |= *first ++;
//		
//				array .emplace_back (point);
//			}

			break;
		}
		case 4:
		{
//				const auto      stride = 4;
//				const uint32_t* first  = static_cast <uint32_t*> ((void*) image .data ());
//				const uint32_t* last   = first + imageTexture -> getWidth () * imageTexture -> getHeight () * stride;
//
//				while (first not_eq last)
//					array .emplace_back (*first ++);
//				
			break;
		}
		default:
			break;
	}

	pixelTexture -> image () .setValue (width, height, components, array);
}

} // puck
} // titania
