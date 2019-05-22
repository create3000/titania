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

#include "Texture3D.h"

#include "NRRDParser.h"

#include <Titania/LOG.h>

namespace titania {
namespace X3D {

Texture3D::Texture3D (size_t components, size_t width, size_t height, size_t depth, GLenum format, std::vector <uint8_t> && data) :
	components (components),
	     width (width),
	    height (height),
	     depth (depth),
	    format (format),
	      data (std::move (data))
{ }

Texture3D::Texture3D (const std::string & document) :
	Texture3D (0, 0, 0, 0, GL_RGB, std::vector <uint8_t> ())
{
	if (readNRRD (document))
		return;
}

bool
Texture3D::readNRRD (const std::string & document)
{
	const auto nrrd = NRRDParser (document) .parse ();

	if (nrrd .nrrd)
	{
		if (not nrrd .valid)
			throw std::invalid_argument (nrrd .error);

		components = nrrd .components;
		width      = nrrd .width;
		height     = nrrd .height;
		depth      = nrrd .depth;

		data .assign (nrrd .pixels .begin (), nrrd .pixels .end ());

		switch (components)
		{
			case 1:
			{
				format = GL_LUMINANCE;
				break;
			}
			case 2:
			{
				format = GL_LUMINANCE_ALPHA;
				break;
			}
			case 3:
			{
				format = GL_RGB;
				break;
			}
			case 4:
			{
				format = GL_RGBA;
				break;
			}
			default:
			{
				throw std::invalid_argument ("Unsupported NRRD channel size");
			}
		}

		return true;
	}

	return false;
}

Texture3D::~Texture3D ()
{ }

} // X3D
} // titania
