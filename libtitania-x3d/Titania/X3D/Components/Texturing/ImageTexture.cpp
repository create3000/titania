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
 ******************************************************************************/

#include "ImageTexture.h"

#include "../../Browser/Browser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

namespace titania {
namespace X3D {

ImageTexture::ImageTexture (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DTexture2DNode (),                                                    
	    X3DUrlObject ()                                                    
{
	setComponent ("Texturing");
	setTypeName ("ImageTexture");

	appendField (inputOutput,    "metadata",          metadata);
	appendField (inputOutput,    "url",               url);
	appendField (initializeOnly, "repeatS",           repeatS);
	appendField (initializeOnly, "repeatT",           repeatT);
	appendField (initializeOnly, "textureProperties", textureProperties);
}

X3DBaseNode*
ImageTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ImageTexture (executionContext);
}

void
ImageTexture::initialize ()
{
	X3DTexture2DNode::initialize ();
	X3DUrlObject::initialize ();

	url .addInterest (this, &ImageTexture::set_url);

	set_url ();
}

void
ImageTexture::set_url ()
{
	setLoadState (NOT_STARTED_STATE);
	requestImmediateLoad ();
}

void
ImageTexture::requestImmediateLoad ()
{
	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	// Delete previous Texture.

	if (getTexture ())
		getBrowser () -> removeTexture (getWorldURL (), getTexture ());

	// Get cached Texture.

	std::string cache_control;

	try
	{
		cache_control = getBrowser () -> getExecutionContext () -> getMetaData ("cache-control");
	}
	catch (...)
	{ }

	if (cache_control not_eq "no-cache")
	{
		for (const auto & URL : transformURI (url))
		{
			GLint textureId = getBrowser () -> getTexture (URL);

			if (textureId)
			{
				setWorldURL (URL .str ());
				setTexture (textureId);
				setLoadState (COMPLETE_STATE);
				return;
			}
		}
	}

	// Load image.

	Magick::Image image;

	if (not loadImage (image))
	{
		setLoadState (FAILED_STATE);
		return;
	}

	// Set image.

	setImage (image);

	// Add texture to cache.

	getBrowser () -> addTexture (getWorldURL (), getTexture ());

	setLoadState (COMPLETE_STATE);
}

bool
ImageTexture::loadImage (Magick::Image & image)
{
	std::clog
		<< std::endl
		<< "Info: Loading image for ImageTexture '" << (void*) this << '\'' << std::endl;

	for (const auto & URL : url)
	{
		try
		{
			std::string data = loadDocument (URL);

			std::list <Magick::Image> images;
			Magick::readImages (&images, Magick::Blob (data .c_str (), data .length ()));

			switch (images .size ())
			{
				case 0:  // I have no idea what to do now.
					continue;

				case 1:  // Image with one layer image.
					image = images .back ();
					return true;

				default: // Flatten image with more than one layer.
					Magick::flattenImages (&image, images .begin (), images .end ());
					return true;
			}
		}
		catch (const X3DError & error)
		{
			std::clog << "ImageTexture: " << error .what () << std::endl;
		}
		catch (const std::exception & error)
		{
			std::clog << "Bad Image: " << error .what () << ", in URL '" << getWorldURL () << "'" << std::endl;
		}
	}

	return false;
}

void
ImageTexture::dispose ()
{
	if (getBrowser () -> removeTexture (getWorldURL (), getTexture ()))
		deleteTexture ();

	X3DUrlObject::dispose ();
	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
