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

#include "ImageTexture.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include <sigc++/bind.h>

namespace titania {
namespace X3D {

const Component   ImageTexture::component      = Component ("Texturing", 2);
const std::string ImageTexture::typeName       = "ImageTexture";
const std::string ImageTexture::containerField = "texture";

ImageTexture::ImageTexture (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture2DNode (),
	    X3DUrlObject (),
	          buffer (),
	          resize (true),
	          future ()
{
	addType (X3DConstants::ImageTexture);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "url",               url ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "textureProperties", textureProperties ());

	addChildObjects (buffer, future);
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

	url () .addInterest (&ImageTexture::set_url,    this);
	buffer .addInterest (&ImageTexture::set_buffer, this);

	set_url ();
}

void
ImageTexture::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (future)
		future -> setExecutionContext (executionContext);

	X3DUrlObject::setExecutionContext (executionContext);
	X3DTexture2DNode::setExecutionContext (executionContext);
}

void
ImageTexture::setTexture (const basic::uri URL, const TexturePtr & texture)
{
	X3DTexture2DNode::setTexture (texture);

	if (texture)
	{
		setLoadState (COMPLETE_STATE);
		setLoadedUrl (URL);
	}
	else
	{
		setLoadState (FAILED_STATE);
		setLoadedUrl ("");
	}
}

void
ImageTexture::requestImmediateLoad ()
{
	requestAsyncLoad ();
}

void
ImageTexture::requestAsyncLoad ()
{
	if (not getBrowser () -> getLoadUrlObjects ())
		return;

	if (checkLoadState () == COMPLETE_STATE or checkLoadState () == IN_PROGRESS_STATE)
		return;

	setLoadState (IN_PROGRESS_STATE);

	buffer .addEvent ();
}

void
ImageTexture::update ()
{
	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ();
}

void
ImageTexture::set_url ()
{
	setLoadState (NOT_STARTED_STATE);

	requestAsyncLoad ();
}

void
ImageTexture::set_buffer ()
{
	using namespace std::placeholders;

	future .setValue (new TextureFuture (getExecutionContext (),
	                                     url (),
	                                     resize ? getBrowser () -> getMinTextureSize () : 0,
	                                     resize ? getBrowser () -> getMaxTextureSize () : 0,
	                                     std::bind (&ImageTexture::setTexture, this, _1, _2)));
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
ImageTexture::setUrl (const Glib::RefPtr <Gdk::Pixbuf> & image)
{
	// Get PNG data.

	gchar* buffer      = nullptr;
	gsize  buffer_size = 0;

	image -> save_to_buffer (buffer, buffer_size, "png");

	if (not buffer)
		throw Error <DISPOSED> ("ImageTexture::setUrl");

	const std::string data (buffer, buffer_size);

	g_free (buffer);

	// Generate data url.

	url () = { "data:image/png;base64," + Glib::Base64::encode (data) };
}

///  throws Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
void
ImageTexture::setUrl (const Cairo::RefPtr <Cairo::ImageSurface> & surface)
{
	std::ostringstream osstream;

	surface -> write_to_png_stream (sigc::bind (sigc::mem_fun (this, &ImageTexture::write_to_png_stream), sigc::ref (osstream)));

	url () = { "data:image/png;base64," + Glib::Base64::encode (osstream .str ()) };
}

Cairo::ErrorStatus
ImageTexture::write_to_png_stream (const unsigned char* data, unsigned int length, std::ostringstream & osstream)
{
	osstream .write (reinterpret_cast <const char*> (data), length);

	return CAIRO_STATUS_SUCCESS;
}

void
ImageTexture::dispose ()
{
	X3DUrlObject::dispose ();
	X3DTexture2DNode::dispose ();
}

ImageTexture::~ImageTexture ()
{ }

} // X3D
} // titania
