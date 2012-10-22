/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "TextureCoordinateGenerator.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

TextureCoordinateGenerator::TextureCoordinateGenerator (X3DExecutionContext* const executionContext) :
	            X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DTextureCoordinateNode (),                                 
	                    mode ("SPHERE"),                         // SFString [in,out] mode       "SPHERE"        [see Table 18.6]
	               parameter (),                                 // MFFloat  [in,out] parameter  [ ]             [see Table 18.6]
	          textureGenMode (GL_SPHERE_MAP)
{
	setComponent ("Texturing");
	setTypeName ("TextureCoordinateGenerator");

	appendField (inputOutput, "metadata",  metadata);
	appendField (inputOutput, "mode",      mode);
	appendField (inputOutput, "parameter", parameter);
}

X3DBasicNode*
TextureCoordinateGenerator::create (X3DExecutionContext* const executionContext) const
{
	return new TextureCoordinateGenerator (executionContext);
}

void
TextureCoordinateGenerator::initialize ()
{
	X3DTextureCoordinateNode::initialize ();

	mode .addInterest (this, &TextureCoordinateGenerator::set_mode);

	set_mode ();
}

void
TextureCoordinateGenerator::set_mode ()
{
	if (mode == "SPHERE-REFLECT-LOCAL")
	{
		textureGenMode = GL_SPHERE_MAP;
	}
	else if (mode == "SPHERE-REFLECT")
	{
		textureGenMode = GL_SPHERE_MAP;
	}
	else if (mode == "SPHERE-LOCAL")
	{
		textureGenMode = GL_SPHERE_MAP;
	}
	else if (mode == "SPHERE")
	{
		textureGenMode = GL_SPHERE_MAP;
	}
	else if (mode == "CAMERASPACENORMAL")
	{
		textureGenMode = GL_NORMAL_MAP;
	}
	else if (mode == "CAMERASPACEPOSITION")
	{
		textureGenMode = GL_OBJECT_LINEAR;
	}
	else if (mode == "CAMERASPACEREFLECTION")
	{
		textureGenMode = GL_REFLECTION_MAP;
	}
	else if (mode == "COORD-EYE")
	{
		textureGenMode = GL_EYE_LINEAR;
	}
	else if (mode == "COORD")
	{
		textureGenMode = GL_EYE_LINEAR;
	}
	else if (mode == "NOISE-EYE")
	{
		textureGenMode = GL_EYE_LINEAR;
	}
	else if (mode == "NOISE")
	{
		textureGenMode = GL_EYE_LINEAR;
	}
	else
	{
		textureGenMode = GL_SPHERE_MAP;
	}
}

void
TextureCoordinateGenerator::enable ()
{
	glTexGeni (GL_S, GL_TEXTURE_GEN_MODE, textureGenMode);
	glTexGeni (GL_T, GL_TEXTURE_GEN_MODE, textureGenMode);
	glEnable (GL_TEXTURE_GEN_S);
	glEnable (GL_TEXTURE_GEN_T);
}

void
TextureCoordinateGenerator::disable ()
{
	glDisable (GL_TEXTURE_GEN_S);
	glDisable (GL_TEXTURE_GEN_T);
}

} // X3D
} // titania
