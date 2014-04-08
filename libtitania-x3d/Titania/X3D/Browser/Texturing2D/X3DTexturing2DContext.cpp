/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DTexturing2DContext.h"

#include "../Browser/X3DBrowser.h"
#include "../../Components/Texturing/TextureCoordinate.h"
#include "../../Components/Texturing/TextureProperties.h"
#include "../../Components/Texturing/TextureTransform.h"
#include "../../Rendering/OpenGL.h"
#include "../../Rendering/Context.h"

namespace titania {
namespace X3D {

X3DTexturing2DContext::X3DTexturing2DContext () :
	            X3DBaseNode (),
	                texture (false),
	          textureMemory (0),
	         minTextureSize (16),
	         maxTextureSize (0),
	        maxTextureUnits (0),
	maxCombinedTextureUnits (0),
	           textureUnits (),
	   combinedTextureUnits (),
	          textureStages (),
	               texCoord (new TextureCoordinate (getExecutionContext ())),
	      textureProperties (new TextureProperties (getExecutionContext ())),
	       textureTransform (new TextureTransform (getExecutionContext ()))
{
	addChildren (texCoord,
	             textureProperties,
	             textureTransform);
}

void
X3DTexturing2DContext::initialize ()
{
	if (glXGetCurrentContext ())
	{
		glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

		if (getBrowser () -> hasExtension ("GL_NVX_gpu_memory_info"))
		{
			int32_t kbytes = 0;

			glGetIntegerv (GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX, &kbytes);

			textureMemory = size_t (kbytes) * 1024;
		}

		int32_t maxTextureCoords;

		glGetIntegerv (GL_MAX_TEXTURE_SIZE,                 &maxTextureSize);
		glGetIntegerv (GL_MAX_TEXTURE_COORDS,               &maxTextureCoords); // Max multi texture coords
		glGetIntegerv (GL_MAX_TEXTURE_UNITS,                &maxTextureUnits);
		glGetIntegerv (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureUnits);

		maxTextureUnits = std::min (maxCombinedTextureUnits / 2, std::min (maxTextureCoords, maxTextureUnits));

		for (int32_t i = maxTextureUnits - 1; i >= 0; -- i)
			textureUnits .push (i);                                              // Don't add GL_TEXTURE0

		for (int32_t i = maxTextureUnits; i < maxCombinedTextureUnits; ++ i)
			combinedTextureUnits .push (i);                                      // Don't add GL_TEXTURE0
	}

	texCoord          -> setup ();
	textureProperties -> setup ();
	textureTransform  -> setup ();
}

size_t
X3DTexturing2DContext::getAvailableTextureMemory () const
{
	std::lock_guard <ContextMutex> contextLock (getContextMutex ());

	if (getBrowser () -> makeCurrent ())
	{
		if (getBrowser () -> hasExtension ("GL_NVX_gpu_memory_info"))
		{
			int32_t kbytes = 0;

			glGetIntegerv (GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &kbytes); // in KBytes

			return size_t (kbytes) * 1024;
		}

		if (getBrowser () -> hasExtension ("GL_ATI_meminfo"))
		{
			static constexpr GLenum TEXTURE_FREE_MEMORY_ATI = 0x87FC;

			int32_t kbytes [4] = { 0, 0, 0, 0 };

			glGetIntegerv (TEXTURE_FREE_MEMORY_ATI, kbytes); // in KBytes

			return size_t (kbytes [0]) * 1024;
		}
	}

	return 0;
}

X3DTexturing2DContext::~X3DTexturing2DContext ()
{ }

} // X3D
} // titania
