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

#include "X3DPixelTextureEditor.h"

#include "../../Dialogs/FileSaveDialog/FileSaveDialog.h"

#include <Titania/X3D/Browser/ContextLock.h>

namespace titania {
namespace puck {

X3DPixelTextureEditor::X3DPixelTextureEditor () :
	         X3DBaseInterface (),
	X3DTextureEditorInterface (),
	             pixelTexture ()
{
	addChildObjects (pixelTexture);
}

void
X3DPixelTextureEditor::setPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (pixelTexture)
	{
		//pixelTexture -> image () .removeInterest (this, &X3DPixelTextureEditor::set_image);
	}

	pixelTexture = value;

	getPixelTextureBox () .set_visible (pixelTexture);

	if (not pixelTexture)
		pixelTexture = getCurrentContext () -> createNode <X3D::PixelTexture> ();

	//pixelTexture -> image () .addInterest (this, &X3DPixelTextureEditor::set_image);

	//set_image ();
}

const X3D::X3DPtr <X3D::PixelTexture> &
X3DPixelTextureEditor::getPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	getPixelTextureBox () .set_visible (value);

	if (value)
	{
		switch (value -> getType () .back ())
		{
			case X3D::X3DConstants::ImageTexture:
			case X3D::X3DConstants::MovieTexture:
			{
				try
				{
					X3D::X3DPtr <X3D::X3DTexture2DNode> texture2DNode (value);

					if (texture2DNode -> getWidth () and texture2DNode -> getHeight () and texture2DNode -> getComponents ())
						pixelTexture -> setImage (texture2DNode);
				}
				catch (const X3D::X3DError &)
				{ }

				break;
			}
			default:
				break;
		}
	}

	return pixelTexture;
}

void
X3DPixelTextureEditor::on_pixel_texture_open_clicked ()
{

}

void
X3DPixelTextureEditor::on_pixel_texture_save_as_clicked ()
{
	saveTexture ();
}

void
X3DPixelTextureEditor::saveTexture ()
{
	try
	{
		const auto dialog = std::dynamic_pointer_cast <FileSaveDialog> (addDialog ("FileSaveDialog", false));
	
		dialog -> getWindow () .add_filter (dialog -> getFileFilterImage ());
		dialog -> getWindow () .set_filter (dialog -> getFileFilterImage ());
	
		dialog -> getWindow () .set_current_name (_ ("image.png"));
	
		if (dialog -> run ())
		{
			const auto image = getImage (X3D::X3DPtr <X3D::X3DTexture2DNode> (pixelTexture));
	
			image -> quality (100);
			image -> write (Glib::uri_unescape_string (dialog -> getWindow () .get_filename ()));
		}
	}
	catch (const X3D::X3DError &)
	{ }
}

std::shared_ptr <Magick::Image>
X3DPixelTextureEditor::getImage (const X3D::X3DPtr <X3D::X3DTexture2DNode> & texture2DNode) const
throw (X3D::Error <X3D::INVALID_NODE>,
       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
       X3D::Error <X3D::DISPOSED>)
{
	// Process image.
	const auto width     = texture2DNode -> getWidth ();
	const auto height    = texture2DNode -> getHeight ();
	const bool opaque    = texture2DNode -> getComponents () % 2;
	const auto imageData = getImageData (texture2DNode);
	const auto image     = std::make_shared <Magick::Image> (width, height, opaque ? "RGB" : "RGBA", Magick::CharPixel, imageData .data ());

	if (opaque)
	{
		image -> matte (false);
		image -> type (Magick::TrueColorType);
	}
	else
		image -> type (Magick::TrueColorMatteType);

	image -> flip ();
	image -> resolutionUnits (Magick::PixelsPerInchResolution);
	image -> density (Magick::Geometry (72, 72));

	return image;
}

std::vector <uint8_t>
X3DPixelTextureEditor::getImageData (const X3D::X3DPtr <X3D::X3DTexture2DNode> & texture2DNode) const
throw (X3D::Error <X3D::INVALID_NODE>,
       X3D::Error <X3D::INVALID_OPERATION_TIMING>,
       X3D::Error <X3D::DISPOSED>)
{
	if (not texture2DNode)
		throw X3D::Error <X3D::INVALID_NODE> ("Node is NULL.");

	X3D::ContextLock lock (texture2DNode -> getBrowser ());

	const auto   width      = texture2DNode -> getWidth ();
	const auto   height     = texture2DNode -> getHeight ();
	const auto   components = texture2DNode -> getComponents ();

	switch (components)
	{
		case 1:
		{
			// Copy image to array.

			const auto stride = 3;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			return image;
		}
		case 2:
		{
			// Copy image to array.

			const auto stride    = 4;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			return image;
		}
		case 3:
		{
			// Copy image to array.

			const auto stride    = components;
			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			return image;
		}
		case 4:
		{
			// Copy image to array.

			const auto stride = components;

			std::vector <uint8_t> image (width * height * stride);

			glBindTexture (GL_TEXTURE_2D, texture2DNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			return image;
		}
		default:
			break;
	}

	throw X3D::Error <X3D::INVALID_NODE> ("Unsupported comonent count.");
}

} // puck
} // titania
