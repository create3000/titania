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

#include "../../Dialogs/FileSaveDialog/FileExportImageDialog.h"

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
X3DPixelTextureEditor::setPixelTexture (const X3D::X3DExecutionContextPtr & executionContext, const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (pixelTexture)
	{
		//pixelTexture -> image () .removeInterest (&X3DPixelTextureEditor::set_image, this);
	}

	pixelTexture = value;

	getPixelTextureBox () .set_visible (pixelTexture);

	if (not pixelTexture)
		pixelTexture = executionContext -> createNode <X3D::PixelTexture> ();

	//pixelTexture -> image () .addInterest (&X3DPixelTextureEditor::set_image, this);

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

					if (texture2DNode -> getWidth () and texture2DNode -> getHeight () and texture2DNode -> components ())
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
	try
	{
		const auto dialog = addDialog <FileExportImageDialog> ("FileExportImageDialog", false);
		auto       image  = pixelTexture -> getImage ();

		dialog -> save (image, _ ("image.png"));
	}
	catch (const X3D::X3DError &)
	{ }
}

} // puck
} // titania
