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

FrameBuffer::FrameBuffer (const X3DBrowserContext* const browser, const size_t width, const size_t height, const size_t samples_, const bool withColorBuffer) :
	      browser (browser),
	        width (width),
	       height (height),
	      samples (samples_),
	           id (0),
	colorBufferId (0),
	depthBufferId (0),
	        color (3 * width * height),  // DEBUG
	        depth (width * height)
{
	if (not browser -> hasExtension ("GL_EXT_framebuffer_multisample"))
		samples = 0;

	if (glXGetCurrentContext ()) // GL_EXT_framebuffer_object
	{
		glGenFramebuffers (1, &id);

		// Bind frame buffer.
		glBindFramebuffer (GL_FRAMEBUFFER, id);

		// The color buffer
		if (withColorBuffer)
		{
			glGenRenderbuffers (1, &colorBufferId);
			glBindRenderbuffer (GL_RENDERBUFFER, colorBufferId);

			if (samples)
				glRenderbufferStorageMultisample (GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
			else
				glRenderbufferStorage (GL_RENDERBUFFER, GL_RGBA8, width, height);

			glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferId);
		}

		// The depth buffer
		glGenRenderbuffers (1, &depthBufferId);
		glBindRenderbuffer (GL_RENDERBUFFER, depthBufferId);

		if (samples)
			glRenderbufferStorageMultisample (GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT, width, height);
		else
			glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferId);

		// Always check that our framebuffer is ok
		if (glCheckFramebufferStatus (GL_FRAMEBUFFER) not_eq GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error ("Couldn't create frame buffer.");

		glBindFramebuffer (GL_FRAMEBUFFER, 0);
	}
}

double
FrameBuffer::getDistance (const double radius, const double zNear, const double zFar)
{
	glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());

	double       distance = std::numeric_limits <double>::infinity ();
	const double w1       = 2.0 / (width  - 1);
	const double h1       = 2.0 / (height - 1);
	const double zWidth   = zFar - zNear;

	for (size_t py = 0, i = 0; py < height; ++ py)
	{
		const double y2 = math::sqr ((py * h1 - 1) * radius);

	   for (size_t px = 0; px < width; ++ px, ++ i)
	   {
		   const double x = (px * w1 - 1) * radius;
			const double z = zNear + zWidth * depth [i];

			distance = std::min (distance, std::sqrt (x * x + y2 + z * z));
	   }
	}

	return distance;
}

void
FrameBuffer::bind ()
{
	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, id);

	glGetIntegerv (GL_VIEWPORT, viewport);
	glViewport (0, 0, width, height);
	glScissor  (0, 0, width, height);
	glClear (GL_DEPTH_BUFFER_BIT);
	// glClear (GL_COLOR_BUFFER_BIT); // DEBUG
}

void
FrameBuffer::unbind ()
{
	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);

	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void
FrameBuffer::get (std::vector <uint8_t> & pixels) const
{
	pixels .resize (4 * width * height);

	if (samples)
	{
		FrameBuffer frameBuffer (browser, width, height, 0, colorBufferId);

		glBindFramebuffer (GL_READ_FRAMEBUFFER, id);
		glBindFramebuffer (GL_DRAW_FRAMEBUFFER, frameBuffer .id);
		glBlitFramebuffer (0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer .id);

		glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels .data ());
		glBindFramebuffer (GL_FRAMEBUFFER_EXT, id);
	}
	else
		glReadPixels (0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels .data ());
}

bool
FrameBuffer::test (const size_t x, const size_t y) const
{
	std::vector <uint8_t> pixels (4);

	glReadPixels (x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixels .data ());

	return pixels [0];
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
		if (colorBufferId)
			glDeleteRenderbuffers (1, &colorBufferId);

		if (depthBufferId)
			glDeleteRenderbuffers (1, &depthBufferId);

		if (id)
			glDeleteFramebuffers (1, &id);
	}
}

} // X3D
} // titania
