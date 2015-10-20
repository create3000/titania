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

#include "Background.h"

#include <Titania/Math/Geometry/Camera.h>
#include <Titania/LOG.h>

namespace titania {
namespace opengl {

Background::Background () :
	       textureId (0),
	projectionMatrix (math::ortho <float> (0, 1, 0, 1, -1, 1))
{ }

void
Background::setup ()
{
	glGenTextures (1, &textureId);
}

void
Background::configure (const Glib::RefPtr <Gtk::StyleContext> & styleContext, const size_t width, const size_t height)
{
	Cairo::RefPtr <Cairo::ImageSurface> surface = Cairo::ImageSurface::create (Cairo::FORMAT_ARGB32, width, height);

	const auto cairo = Cairo::Context::create (surface);

	styleContext -> render_background (cairo, 0, 0, width, height);

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);
	glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface -> get_data ());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBindTexture (GL_TEXTURE_2D, 0);
}

void
Background::draw ()
{
	glMatrixMode (GL_PROJECTION);
	glLoadMatrixf (projectionMatrix .data ());

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();

	glDisable (GL_LIGHTING);
	glDisable (GL_DEPTH_TEST);
	glDepthMask (GL_FALSE);

	glEnable (GL_TEXTURE_2D);
	glBindTexture (GL_TEXTURE_2D, textureId);

	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin (GL_QUADS);
	glNormal3f (0, 0, 1);
	glTexCoord2f (0, 0);
	glVertex2f (0, 0);
	glTexCoord2f (1, 0);
	glVertex2f (1, 0);
	glTexCoord2f (1, 1);
	glVertex2f (1, 1);
	glTexCoord2f (0, 1);
	glVertex2f (0, 1);
	glEnd ();

	glBindTexture (GL_TEXTURE_2D, 0);
	glDisable (GL_TEXTURE_2D);
	glDepthMask (GL_TRUE);
	glEnable (GL_DEPTH_TEST);
}

void
Background::dispose ()
{
	if (textureId)
		glDeleteTextures (1, &textureId);
}

Background::~Background ()
{ }


} // opengl
} // titania
