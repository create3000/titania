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

#include "ImageTexture.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

//

#include <future>
#include <glibmm/dispatcher.h>

namespace titania {
namespace X3D {

class Texture
{
public:

	typedef size_t size_type;

	Texture (const std::string & data) :
		image (getImage (data)),
		format (GL_RGB),
		components (3)
	{ }

	GLenum
	getFormat ()
	{ return format; }
	
	size_type
	getWidth () const
	{ return image .size () .width (); }

	size_type
	getHeight () const
	{ return image .size () .height (); }

	size_type
	getComponents () const
	{ return components; }
	
	const void*
	getData ()
	{ return blob .data (); }

	void
	process (const Color4f & borderColor, size_type borderWidth,
	         size_type minTextureSize, size_type maxTextureSize)
	{
		addBorder (borderColor, borderWidth);

		tryScaleImage (minTextureSize, maxTextureSize);

		refineImageFormat ();

		writeBlob ();
	}

	virtual
	~Texture ()
	{ }


private:

	static
	Magick::Image
	getImage (const std::string & data)
	{
		std::list <Magick::Image> images;
		Magick::readImages (&images, Magick::Blob (data .c_str (), data .length ()));

		switch (images .size ())
		{
			case 0:  // I have no idea what to do now.
				throw std::domain_error ("Image contains nothing.");

			case 1:
			{
				// Image with one layer image.
				return images .back ();
			}
			default:
			{
				// Flatten image with more than one layer.
				Magick::Image image;
				Magick::flattenImages (&image, images .begin (), images .end ());
				return image;
			}
		}
	}

	void
	addBorder (const Color4f & borderColor, size_type borderWidth)
	{
		if (borderWidth > 0)
		{
			std::ostringstream color;

			color
				<< std::hex
				<< '#'
				<< std::setfill ('0')
				<< std::setw (2) << (int) (uint8_t) (borderColor .r () * 255)
				<< std::setw (2) << (int) (uint8_t) (borderColor .g () * 255)
				<< std::setw (2) << (int) (uint8_t) (borderColor .b () * 255)
				<< std::setw (2) << (int) (uint8_t) (borderColor .a () * 255);

			image .borderColor (Magick::Color (color .str ()));

			image .border (Magick::Geometry (borderWidth, borderWidth));
		}
	}

	void
	tryScaleImage (size_type minTextureSize, size_type maxTextureSize)
	{
		size_t width  = getWidth ();
		size_t height = getHeight ();

		if (std::max (width, height) < minTextureSize)
			return;

		bool needsScaling = false;

		if (not math::is_power_of_two (width))
		{
			width        = std::min (math::next_power_of_two (width), maxTextureSize);
			needsScaling = true;
		}

		if (not math::is_power_of_two (height))
		{
			height       = std::min (math::next_power_of_two (height), maxTextureSize);
			needsScaling = true;
		}

		if (needsScaling)
		{
			std::clog
				<< "Warning: Texture needs scaling: scaling texture from "
				<< getWidth () << " x " << getHeight ()
				<< " to " << width << " x " << height << " pixel."
				<< std::endl;

			scaleImage (width, height);
		}
	}

	void
	scaleImage (size_type width, size_type height)
	{
		Magick::Geometry geometry (width, height);

		geometry .aspect (true);
		image .filterType (Magick::LanczosFilter);
		image .zoom (geometry);
	}

	void
	refineImageFormat ()
	{
		switch (image .type ())
		{
			case Magick::GrayscaleType:
			{
				if (not image .matte ())
				{
					image .colorSpace (Magick::GRAYColorspace);
					image .magick ("GRAY");
					format      = GL_LUMINANCE;
					components  = 1;
					return;
				}
			}
			case Magick::GrayscaleMatteType:
			{
				image .colorSpace (Magick::GRAYColorspace);
				image .type (Magick::TrueColorMatteType);
				image .magick ("RGBA");
				format      = GL_RGBA;
				components  = 2;
				return;
			}
			case Magick::TrueColorType:
			{
				if (not image .matte ())
				{
					image .colorSpace (Magick::RGBColorspace);
					image .magick ("RGB");
					format      = GL_RGB;
					components  = 3;
					return;
				}
			}
			case Magick::TrueColorMatteType:
			{
				image .colorSpace (Magick::RGBColorspace);
				image .magick ("RGBA");
				format      = GL_RGBA;
				components  = 4;
				return;
			}
			default:
			{
				if (image .matte ())
				{
					image .type (Magick::TrueColorMatteType);
					refineImageFormat ();
					return;
				}
				else
				{
					image .type (Magick::TrueColorType);
					refineImageFormat ();
					return;
				}
			}
		}
	}
	
	void
	writeBlob ()
	{
		image .interlaceType (Magick::NoInterlace);
		image .endian (Magick::LSBEndian);
		image .depth (8);
		image .write (&blob);
	}

	///  @name Properties

	Magick::Image image;
	GLenum        format;
	size_type     components;
	Magick::Blob  blob;

};

class ImageTexture::Thread :
	public X3DInput
{
public:

	typedef Texture::size_type size_type;

	Thread (ImageTexture* const imageTexture) :
		imageTexture (imageTexture),
		future (std::async (std::launch::async, std::mem_fn (&Thread::loadAsync), this,
		                    imageTexture -> url (),
		                    imageTexture -> getTextureProperties () -> borderColor (),
		                    imageTexture -> getTextureProperties () -> borderWidth (),
		                    8,
		                    imageTexture -> getBrowser () -> getRenderingProperties () -> maxTextureSize ()))
	{
		imageTexture -> getBrowser () -> prepareEvents .addInterest (this, &Thread::prepareEvents);
		imageTexture -> getBrowser () -> notify ();
	}

	virtual
	~Thread ()
	{
		if (future .valid ())
			future .wait ();
	}

private:

	void
	prepareEvents ()
	{
		imageTexture -> getBrowser () -> notify ();

		if (future .valid ())
		{
			auto status = future .wait_for (std::chrono::milliseconds (0));

			if (status == std::future_status::ready)
			{
				imageTexture -> getBrowser () -> prepareEvents .removeInterest (this, &Thread::prepareEvents);
				imageTexture -> set_image (future .get ());
			}
		}
	}

	TexturePtr
	loadAsync (const MFString & url,
	           const Color4f & borderColor, size_type borderWidth,
	           size_type minTextureSize, size_type maxTextureSize)
	{
		std::lock_guard <std::mutex> lock (getThread ());

		for (const auto & URL : url)
		{
			try
			{
				TexturePtr texture (new Texture (imageTexture -> loadDocument (URL)));

				texture -> process (borderColor, borderWidth, minTextureSize, maxTextureSize);

				return texture;
			}
			catch (const X3DError & error)
			{
				std::clog << "ImageTexture: " << error .what () << std::endl;
			}
			catch (const std::exception & error)
			{
				std::clog << "Bad Image: " << error .what () << ", in URL '" << imageTexture -> getWorldURL () << "'" << std::endl;
			}
		}

		return nullptr;
	}
	
	std::mutex &
	getThread ()
	{
		std::lock_guard <std::mutex> lock (mutex);
		threadIndex = (threadIndex + 1) % threads .size ();
		return threads [threadIndex];
	}
	//
	
	static size_t                   threadIndex;
	static std::deque <std::mutex>  threads;
	std::mutex                      mutex;

	ImageTexture* const      imageTexture;
	std::future <TexturePtr> future;

};

size_t                   ImageTexture::Thread::threadIndex = 0;
std::deque <std::mutex>  ImageTexture::Thread::threads (4);

ImageTexture::ImageTexture (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture2DNode (),
	    X3DUrlObject (),
	          thread ()
{
	setComponent ("Texturing");
	setTypeName ("ImageTexture");

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "url",               url ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());
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

	url () .addInterest (this, &ImageTexture::update);

	requestImmediateLoad ();
}

void
ImageTexture::set_image (const TexturePtr & texture)
{
	__LOG__ << texture .get () << " : " << url () << std::endl;

	if (texture)
	{
		setImage (texture -> getComponents (),
		          texture -> getFormat (),
		          texture -> getWidth (), texture -> getHeight (),
		          texture -> getData ());

		setLoadState (COMPLETE_STATE);
	}
	else
		setLoadState (FAILED_STATE);
}

void
ImageTexture::update ()
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

	thread .reset (new Thread (this));

//	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
//		return;
//
//	setLoadState (IN_PROGRESS_STATE);
//
//	// Load image.
//
//	for (const auto & URL : url ())
//	{
//		try
//		{
//			setImage (loadDocument (URL));
//
//			setLoadState (COMPLETE_STATE);
//			return;
//		}
//		catch (const X3DError & error)
//		{
//			std::clog << "ImageTexture: " << error .what () << std::endl;
//		}
//		catch (const std::exception & error)
//		{
//			std::clog << "Bad Image: " << error .what () << ", in URL '" << getWorldURL () << "'" << std::endl;
//		}
//	}
//
//	setLoadState (FAILED_STATE);
}

void
ImageTexture::setImage (const std::string & data)
{
	std::list <Magick::Image> images;
	Magick::readImages (&images, Magick::Blob (data .c_str (), data .length ()));

	switch (images .size ())
	{
		case 0:  // I have no idea what to do now.
			throw std::domain_error ("Image contains nothing.");

		case 1:
		{
			// Image with one layer image.
			setImage (images .back ());
			return;
		}
		default:
		{
			// Flatten image with more than one layer.
			Magick::Image image;
			Magick::flattenImages (&image, images .begin (), images .end ());
			setImage (image);
			return;
		}
	}
}

void
ImageTexture::dispose ()
{
	thread .reset ();

	X3DUrlObject::dispose ();
	X3DTexture2DNode::dispose ();
}

} // X3D
} // titania
