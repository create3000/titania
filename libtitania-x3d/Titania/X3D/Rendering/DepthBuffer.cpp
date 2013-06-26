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

#include "DepthBuffer.h"

#include <stdexcept>

namespace titania {
namespace X3D {

DepthBuffer::DepthBuffer (size_t width, size_t height) :
	      width (width),
	     height (height),
	         id (0),
	colorBuffer (0),
	depthBuffer (0),
	      depth (width * height)
{
	if (glXGetCurrentContext ()) // GL_EXT_framebuffer_object
	{
		glGenFramebuffers (1, &id);

		// Bind frame buffer.
		glBindFramebuffer (GL_FRAMEBUFFER, id);

		// The color buffer 1
		//			glGenRenderbuffers (1, &colorBuffer);
		//			glBindRenderbuffer (GL_RENDERBUFFER, colorBuffer);
		//			glRenderbufferStorage (GL_RENDERBUFFER, GL_RGBA8, width, height);
		//			glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBuffer);

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
DepthBuffer::getDistance (float zNear, float zFar)
{
	glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());

	float distance = zNear + (zFar - zNear) * depth [0];

	for (const auto & d : depth)
	{
		distance = std::min (distance, zNear + (zFar - zNear) * d);
	}

	return distance;
}

void
DepthBuffer::bind ()
{
	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, id);

	glGetIntegerv (GL_VIEWPORT, viewport);
	glViewport (0, 0, width, height);
	glScissor  (0, 0, width, height);
	glEnable (GL_SCISSOR_TEST);
	glClear (GL_DEPTH_BUFFER_BIT);
}

void
DepthBuffer::unbind ()
{
	glDisable (GL_SCISSOR_TEST);
	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);

	// Bind frame buffer.
	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

DepthBuffer::~DepthBuffer ()
{
	if (colorBuffer)
		glDeleteRenderbuffers (1, &colorBuffer);

	if (depthBuffer)
		glDeleteRenderbuffers (1, &depthBuffer);

	if (id)
		glDeleteFramebuffers (1, &id);
}

} // X3D
} // titania
