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

#include "FrameBuffer.h"

#include "../Bits/Error.h"
#include "../Rendering/ViewVolume.h"
#include "../RenderingSurface/ContextLock.h"

#include <Titania/LOG.h>
#include <stdexcept>

namespace titania {
namespace X3D {

FrameBuffer::FrameBuffer (X3DRenderingSurface* const renderingSurface, const size_t width, const size_t height, const size_t samples_, const bool withColorBuffer) :
	 renderingSurface (renderingSurface),
	            width (width),
	           height (height),
	          samples (samples_),
	  withColorBuffer (withColorBuffer),
	               id (0),
	    colorBufferId (0),
	    depthBufferId (0),
	      pixelBuffer (samples ? new FrameBuffer (renderingSurface, width, height, 0, withColorBuffer) : nullptr),
	           pixels (),
	            depth (width * height),
	      frameBuffer (0),
	         viewport ()
{ }

void
FrameBuffer::setup ()
{
	if (not renderingSurface -> getExtension ("GL_EXT_framebuffer_multisample"))
		samples = 0;

	// Setup frame buffer with no antialiasing for blit.
	if (samples)
		pixelBuffer -> setup ();

	// Generate and bind frame buffer.
	glGetIntegerv (GL_FRAMEBUFFER_BINDING, &frameBuffer);
	glGenFramebuffers (1, &id);
	glBindFramebuffer (GL_FRAMEBUFFER, id);

	if (withColorBuffer)
	{
		// The color buffer

		glGenRenderbuffers (1, &colorBufferId);
		glBindRenderbuffer (GL_RENDERBUFFER, colorBufferId);

		if (samples)
			glRenderbufferStorageMultisample (GL_RENDERBUFFER, samples, GL_RGBA8, width, height);
		else
			glRenderbufferStorage (GL_RENDERBUFFER, GL_RGBA8, width, height);

		glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferId);
	}
	else
	{
		glDrawBuffer (GL_NONE); // No color buffer is drawn to.
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

	const bool complete = glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer);

	if (complete)
		return;

	throw Error <INSUFFICIENT_CAPABILITIES> ("Couldn't create frame buffer.");
}

double
FrameBuffer::getDepth (const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	try
	{
		glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());
	
		const auto invProjectionMatrix = inverse (projectionMatrix);
		double     winx                = 0;
		double     winy                = 0;
		double     winz                = std::numeric_limits <double>::infinity ();
	
		for (size_t wy = 0, i = 0; wy < height; ++ wy)
		{
		   for (size_t wx = 0; wx < width; ++ wx, ++ i)
		   {
				const auto wz = depth [i];

				if (wz < winz)
				{
					winx = wx;
					winy = wy;
					winz = wz;
				}
		   }
		}

		const auto point = ViewVolume::unProjectPoint (winx, winy, winz, invProjectionMatrix, viewport);

		return point .z ();
	}
	catch (const std::domain_error &)
	{
		return 0;
	}
}

void
FrameBuffer::bind ()
{
	glGetIntegerv (GL_VIEWPORT, viewport);
	glGetIntegerv (GL_FRAMEBUFFER_BINDING, &frameBuffer);

	glBindFramebuffer (GL_FRAMEBUFFER, id);
	glViewport (0, 0, width, height);
	glScissor  (0, 0, width, height);
}

void
FrameBuffer::unbind ()
{
	glBindFramebuffer (GL_FRAMEBUFFER, frameBuffer);
	glViewport (viewport [0], viewport [1], viewport [2], viewport [3]);
	glScissor  (viewport [0], viewport [1], viewport [2], viewport [3]);
}

const std::vector <uint8_t> &
FrameBuffer::readPixels (const GLenum format)
{
	pixels .resize (4 * width * height);

	if (samples)
	{
		try
		{
			glBindFramebuffer (GL_READ_FRAMEBUFFER, id);
			glBindFramebuffer (GL_DRAW_FRAMEBUFFER, pixelBuffer -> id);
			glBlitFramebuffer (0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer (GL_FRAMEBUFFER, pixelBuffer -> id);
	
			glReadPixels (0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels .data ());
			glBindFramebuffer (GL_FRAMEBUFFER, id);
		}
		catch (const std::runtime_error & error)
		{
			__LOG__ << error .what () << std::endl;

			std::fill (pixels .begin (), pixels .end (), 0);
		}
	}
	else
	{
		glReadPixels (0, 0, width, height, format, GL_UNSIGNED_BYTE, pixels .data ());
	}

	return pixels;
}

const std::vector <float> &
FrameBuffer::readDepth ()
{
	depth .resize (width * height);

	if (samples)
	{
		glBindFramebuffer (GL_READ_FRAMEBUFFER, id);
		glBindFramebuffer (GL_DRAW_FRAMEBUFFER, pixelBuffer -> id);
		glBlitFramebuffer (0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer (GL_FRAMEBUFFER, pixelBuffer -> id);

		glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());
		glBindFramebuffer (GL_FRAMEBUFFER, id);
	}
	else
	{
		glReadPixels (0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depth .data ());
	}

	return depth;
}

FrameBuffer::~FrameBuffer ()
{
	try
	{
		ContextLock lock (renderingSurface);

		if (colorBufferId)
			glDeleteRenderbuffers (1, &colorBufferId);

		if (depthBufferId)
			glDeleteRenderbuffers (1, &depthBufferId);

		if (id)
			glDeleteFramebuffers (1, &id);
	}
	catch (const Error <INVALID_OPERATION_TIMING> & error)
	{
		//__LOG__ << error .what () << std::endl;
	}
}

} // X3D
} // titania
