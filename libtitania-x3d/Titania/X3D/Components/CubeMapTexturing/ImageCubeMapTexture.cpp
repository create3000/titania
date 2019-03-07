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

#include "ImageCubeMapTexture.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/ImageTexture.h"

namespace titania {
namespace X3D {

const Component   ImageCubeMapTexture::component      = Component ("CubeMapTexturing", 2);
const std::string ImageCubeMapTexture::typeName       = "ImageCubeMapTexture";
const std::string ImageCubeMapTexture::containerField = "texture";

ImageCubeMapTexture::ImageCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	             X3DUrlObject (),
	              textureNode (new ImageTexture (executionContext))
{
	addType (X3DConstants::ImageCubeMapTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "url",               url ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (textureNode);
}

X3DBaseNode*
ImageCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ImageCubeMapTexture (executionContext);
}

void
ImageCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();
	X3DUrlObject::initialize ();

	url () .addInterest (textureNode -> url ());

	textureProperties () .addInterest (textureNode -> textureProperties ());

	textureNode -> checkLoadState () .addInterest (&ImageCubeMapTexture::set_loadState, this);
	textureNode -> setPrivate (true);
	textureNode -> setResizeToPowerOfTwo (false);
	textureNode -> url () = url ();
	textureNode -> textureProperties () = textureProperties ();
	textureNode -> setup ();
}

void
ImageCubeMapTexture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	textureNode -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DEnvironmentTextureNode::setExecutionContext (executionContext);
}

bool
ImageCubeMapTexture::isTransparent () const
{
	return textureNode -> isTransparent ();
}

size_t
ImageCubeMapTexture::getWidth () const
{
	return textureNode -> getWidth () / 4;
}

size_t
ImageCubeMapTexture::getHeight () const
{
	return textureNode -> getHeight () / 3;
}

size_t
ImageCubeMapTexture::getComponents () const
{
	return textureNode -> components ();
}

void
ImageCubeMapTexture::requestImmediateLoad ()
{
	textureNode -> requestImmediateLoad ();
}

void
ImageCubeMapTexture::set_loadState ()
{
	static const std::vector <std::pair <size_t, size_t>> offsets = {
		std::pair (1, 1), // Front
		std::pair (3, 1), // Back
		std::pair (0, 1), // Left
		std::pair (2, 1), // Right
		std::pair (1, 2), // Top
		std::pair (1, 0), // Bottom
	
	};

	setLoadState (textureNode -> checkLoadState ());

	switch (checkLoadState ())
	{
		case COMPLETE_STATE:
		{
			// Get texture 2d data as four component RGBA
	
			const auto width         = getWidth ();
			const auto height        = getHeight ();
			const auto width4        = width * 4;
			const auto textureWidth4 = textureNode -> getWidth () * 4;

			if (width != height)
			{
				getBrowser () -> getConsole () -> error ("Error: ImageCubeMapTexture: width and height must be equal, and all images must be of the same size!\n");

				for (size_t i = 0; i < 6; ++ i)
					setImage (getTargets () [i], GL_RGBA, GL_RGBA, nullptr, true);

				setLoadState (FAILED_STATE);
				return;
			}

			std::vector <uint8_t> texture (textureWidth4 * textureNode -> getHeight ());
			std::vector <uint8_t> image (width4 * height);
		
			glBindTexture (GL_TEXTURE_2D, textureNode -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture .data ());
			glBindTexture (GL_TEXTURE_2D, 0);

			const size_t height_1 = height - 1;

			for (size_t i = 0; i < 6; ++ i)
			{
				const auto x = offsets [i] .first  * width4;
				const auto y = offsets [i] .second * height;
	
				// Flip image vertically
		
				for (size_t r = 0; r < height; ++ r)
				{
					for (size_t c = 0; c < width4; ++ c)
					{
						image [r * width4 + c] = texture [(height_1 - r + y) * textureWidth4 + c + x];
					}
				}
		
				// Transfer image
				// Important: width and height must be equal, and all images must be of the same size!!!
	
				setImage (getTargets () [i], GL_RGBA, GL_RGBA, image .data (), true);
			}
	
			break;
		}
		case FAILED_STATE:
		{
			for (size_t i = 0; i < 6; ++ i)
				setImage (getTargets () [i], GL_RGBA, GL_RGBA, nullptr, true);

			break;
		}
		default:
			break;
	}
}


void
ImageCubeMapTexture::dispose ()
{
	X3DUrlObject::dispose ();
	X3DEnvironmentTextureNode::dispose ();
}

} // X3D
} // titania
