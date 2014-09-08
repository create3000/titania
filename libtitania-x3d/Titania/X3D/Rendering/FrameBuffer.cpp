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

#include "FrameBuffer.h"

#include "../Browser/ContextLock.h"

#include <Titania/LOG.h>
#include <stdexcept>

namespace titania {
namespace X3D {

FrameBuffer::FrameBuffer (const X3DBrowserContext* const browser, const size_t width, const size_t height, const bool hasColorBuffer) :
	    browser (browser),
	      width (width),
	     height (height),
	         id (0),
	colorBuffer (0),
	depthBuffer (0),
	      color (3 * width * height),  // DEBUG
	      depth (width * height)
{
	if (glXGetCurrentContext ()) // GL_EXT_framebuffer_object
	{
		glGenFramebuffers (1, &id);

		// Bind frame buffer.
		glBindFramebuffer (GL_FRAMEBUFFER, id);

		// The color buffer
		if (hasColorBuffer)
		{
			glGenRenderbuffers (1, &colorBuffer);
			glBindRenderbuffer (GL_RENDERBUFFER, colorBuffer);
			glRenderbufferStorage (GL_RENDERBUFFER, GL_RGBA8, width, height);
			glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);
		}

		// The depth buffer
		glGenRenderbuffers (1, &depthBuffer);
		glBindRenderbuffer (GL_RENDERBUFFER, depthBuffer);
		glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus (GL_FRAMEBUFFER) not_eq GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error ("Couldn't create frame buffer.");

		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}
}

double
FrameBuffer::getDistance (const double zNear, const double zFar)
{
	glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());

	const auto distance = std::min_element (depth .begin (), depth .end ());

	return zNear + (zFar - zNear) * *distance;
}

void
FrameBuffer::bind ()
{
	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, id);

	glGetIntegerv (GL_VIEWPORT, viewport);
	glViewport (0, 0, width, height);
	glScissor  (0, 0, width, height);
	glEnable (GL_SCISSOR_TEST);
	glClear (GL_DEPTH_BUFFER_BIT);
	// glClear (GL_COLOR_BUFFER_BIT); // DEBUG
}

void
FrameBuffer::unbind ()
{
	glDisable (GL_SCISSOR_TEST);
	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);

	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void
FrameBuffer::get (std::vector <uint8_t> & pixels) const
{
	pixels .resize (4 * width * height);

	glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels .data ());
}

// DEBUG

void
FrameBuffer::save ()
{
	glBindFramebuffer (GL_FRAMEBUFFER, id);
	glReadPixels (0, 0, width, height, GL_RGB, GL_FLOAT, color .data ());
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

// DEBUG

void
FrameBuffer::display ()
{
	glWindowPos2i (0, 0);
	glDrawPixels (width, height, GL_RGB, GL_FLOAT, color .data ());

	//	glBindFramebuffer (GL_READ_FRAMEBUFFER, id);
	//	glBindFramebuffer (GL_DRAW_FRAMEBUFFER, 0);
	//
	//	glBlitFramebuffer (0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	//
	//	// Bind frame buffer.
	//	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer ()
{
	ContextLock lock (browser);

	if (lock)
	{
		if (colorBuffer)
			glDeleteRenderbuffers (1, &colorBuffer);

		if (depthBuffer)
			glDeleteRenderbuffers (1, &depthBuffer);

		if (id)
			glDeleteFramebuffers (1, &id);
	}
}

} // X3D
} // titania
