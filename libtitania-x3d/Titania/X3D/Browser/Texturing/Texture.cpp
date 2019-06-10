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

#include "Texture.h"

#include <Titania/LOG.h>
#include <giomm.h>

namespace titania {
namespace X3D {

Texture::Texture (size_t width, size_t height, size_t components, GLenum format, std::vector <uint8_t> && data) :
	      width (width),
	     height (height),
	 components (components),
	transparent (false),
	     format (format),
	       data (data),
	     pixbuf ()
{ }

Texture::Texture (const std::string & document, const bool process) :
	Texture (0, 0, 0, GL_RGB, std::vector <uint8_t> ())
{
	const auto stream = Gio::MemoryInputStream::create ();

	stream -> add_data (document .data (), document .size (), nullptr);

	pixbuf = Gdk::Pixbuf::create_from_stream (stream);

	if (process)
		pixbuf = pixbuf -> flip (false);

	width      = pixbuf -> get_width ();
	height     = pixbuf -> get_height ();
	components = pixbuf -> get_n_channels ();

	switch (components)
	{
		case 1:
			format = GL_LUMINANCE;
			break;
		case 2:
			format = GL_LUMINANCE_ALPHA;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
	}

	const auto rowstride = pixbuf -> get_rowstride ();
	const auto pixels    = pixbuf -> get_pixels ();

	for (size_t h = 0; h < height; ++ h)
	{
		for (size_t w = 0, ws = width * components; w < ws; ++ w)
			data .emplace_back (pixels [h * rowstride + w]);
	}

	// Verify image transparency.

	switch (components)
	{
		case 2:
		case 4:
		{
			for (size_t i = components - 1, size = data .size (); i < size; i += components)
			{
				if (data [i] == 255)
					continue;

				transparent = true;
				break;
			}

			break;
		}
	}
}

const void*
Texture::getData () const
{
	return data .data ();
}

} // X3D
} // titania
