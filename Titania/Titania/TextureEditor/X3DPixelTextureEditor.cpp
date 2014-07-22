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
	             pixelTexture (),
	                   future ()
{ }

void
X3DPixelTextureEditor::setPixelTexture (const X3D::X3DPtr <X3D::X3DTextureNode> & value)
{
	if (future)
		future -> dispose ();

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
			using namespace std::placeholders;

			const X3D::X3DPtr <X3D::ImageTexture> last (value);

			future .reset (new X3D::TextureLoader (last -> getExecutionContext (),
			                                       last -> url (),
			                                       0, 0,
			                                       std::bind (&X3DPixelTextureEditor::set_texture, this, _1)));
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
X3DPixelTextureEditor::set_texture (const X3D::TexturePtr & texture)
{
	if (texture)
	{
		__LOG__ << texture -> getWidth () << std::endl;
		__LOG__ << texture -> getHeight () << std::endl;
		__LOG__ << texture -> getComponents () << std::endl;
		__LOG__ << texture -> getData () << std::endl;
	}
}

X3DPixelTextureEditor::~X3DPixelTextureEditor ()
{
	if (future)
		future -> dispose ();
}

} // puck
} // titania
