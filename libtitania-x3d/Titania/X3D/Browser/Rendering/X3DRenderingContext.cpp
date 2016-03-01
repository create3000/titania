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

#include "X3DRenderingContext.h"

#include "../X3DBrowser.h"
#include "../Rendering/MotionBlur.h"
#include "../../Rendering/OpenGL.h"

#include <Titania/String.h>

namespace titania {
namespace X3D {

X3DRenderingContext::X3DRenderingContext () :
	  X3DBaseNode (),
	maxClipPlanes (0),
	   clipPlanes (),
	    depthTest (),
	  depthOffset (),
	   motionBlur (new MotionBlur (getExecutionContext ()))
{
	addChildren (motionBlur);

	depthTest   .push (true);
	depthOffset .push (0);
}

void
X3DRenderingContext::initialize ()
{
	if (glXGetCurrentContext ())
	{
		glEnable (GL_SCISSOR_TEST);

		glCullFace (GL_BACK);
		glEnable (GL_NORMALIZE);

		glDepthFunc (GL_LEQUAL);
		glClearDepth (1);

		glBlendFuncSeparate (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquationSeparate (GL_FUNC_ADD, GL_FUNC_ADD);

		glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);

		glHint (GL_FOG_HINT, GL_NICEST);

		// ClipPlanes

		glGetIntegerv (GL_MAX_CLIP_PLANES, &maxClipPlanes);

		for (int32_t i = maxClipPlanes - 1; i >= 0; -- i)
			clipPlanes .push (GL_CLIP_PLANE0 + i);
	}

	motionBlur -> setup ();
}

void
X3DRenderingContext::renderBackground ()
{
	const auto rectangle = getBrowser () -> getRectangle ();

	glViewport (rectangle [0], rectangle [1], rectangle [2], rectangle [3]);
	glScissor  (rectangle [0], rectangle [1], rectangle [2], rectangle [3]);

	glClearColor (0, 0, 0, 0);
	glClear (GL_COLOR_BUFFER_BIT);
}

X3DRenderingContext::~X3DRenderingContext ()
{ }

} // X3D
} // titania
