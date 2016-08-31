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

#include "TextureBuffer.h"

#include "../Browser/ContextLock.h"

#include <Titania/LOG.h>
#include <stdexcept>

namespace titania {
namespace X3D {

TextureBuffer::TextureBuffer (X3DBrowserContext* const browser, const size_t width, const size_t height, const bool withColorBuffer) :
	        browser (browser),
	          width (width),
	         height (height),
	withColorBuffer (withColorBuffer),
	             id (0),
	 colorTextureId (0),
	 depthTextureId (0),
	    frameBuffer (0),
	       viewport ()
{ }

void
TextureBuffer::setup ()
{
	// GL_EXT_framebuffer_object

	if (not glXGetCurrentContext ())
		return;

	// Generate and bind frame buffer.
	glGenFramebuffers (1, &id);
	glBindFramebuffer (GL_FRAMEBUFFER, id);

	if (withColorBuffer)
	{
		// The color buffer
	}
	else
		glDrawBuffer (GL_NONE); // No color buffer is drawn to.

	// The depth buffer

	glGenTextures   (1, &depthTextureId);
	glBindTexture   (GL_TEXTURE_2D, depthTextureId);
	glTexImage2D    (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);

	glFramebufferTexture (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTextureId, 0);

	glBindTexture (GL_TEXTURE_2D, 0);

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus (GL_FRAMEBUFFER) not_eq GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error ("Couldn't create frame buffer.");

	glBindFramebuffer (GL_FRAMEBUFFER, 0);
}

void
TextureBuffer::bind ()
{
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetIntegerv (GL_FRAMEBUFFER_BINDING, &frameBuffer);

	glBindFramebuffer (GL_FRAMEBUFFER, id);
	glViewport (0, 0, width, height);
	glScissor  (0, 0, width, height);
}

void
TextureBuffer::unbind ()
{
	glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer);
	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);
}

TextureBuffer::~TextureBuffer ()
{
	try
	{
		ContextLock lock (browser);

		if (colorTextureId)
			glDeleteTextures (1, &colorTextureId);

		if (depthTextureId)
			glDeleteTextures (1, &depthTextureId);

		if (id)
			glDeleteFramebuffers (1, &id);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }
}

} // X3D
} // titania
