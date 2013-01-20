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

#include "TextureProperties.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

// http://new.web3d.org/files/specifications/19775-1/V3.2/Part01/components/texturing.html#TextureProperties

TextureProperties::TextureProperties (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	    X3DPropertyNode (),                                                    
	        borderColor (),                                                    // SFColorRGBA [ ] borderColor         0 0 0 0    [0,1]
	        borderWidth (),                                                    // SFInt32     [ ] borderWidth         0          [0,1]
	      boundaryModeS ("REPEAT"),                                            // SFString    [ ] boundaryModeS       "REPEAT"
	      boundaryModeT ("REPEAT"),                                            // SFString    [ ] boundaryModeT       "REPEAT"
	      boundaryModeR ("REPEAT"),                                            // SFString    [ ] boundaryModeR       "REPEAT"
	 minificationFilter ("FASTEST"),                                           // SFString    [ ] minificationFilter  "FASTEST"
	magnificationFilter ("FASTEST"),                                           // SFString    [ ] magnificationFilter "FASTEST"
	 textureCompression ("FASTEST"),                                           // SFString    [ ] textureCompression  "FASTEST"
	    generateMipMaps (),                                                    // SFBool      [ ] generateMipMaps      FALSE
	  anisotropicDegree (1),                                                   // SFFloat     [ ] anisotropicDegree    1.0        [1,?)
	    texturePriority ()                                                     // SFFloat     [ ] texturePriority      0
{
	setComponent ("Texturing");
	setTypeName ("TextureProperties");

	addField (inputOutput,    "metadata",            metadata);
	addField (inputOutput,    "borderColor",         borderColor);
	addField (inputOutput,    "borderWidth",         borderWidth);
	addField (inputOutput,    "boundaryModeS",       boundaryModeS);
	addField (inputOutput,    "boundaryModeT",       boundaryModeT);
	addField (inputOutput,    "boundaryModeR",       boundaryModeR);
	addField (inputOutput,    "minificationFilter",  minificationFilter);
	addField (inputOutput,    "magnificationFilter", magnificationFilter);
	addField (inputOutput,    "textureCompression",  textureCompression);
	addField (initializeOnly, "generateMipMaps",     generateMipMaps);
	addField (inputOutput,    "anisotropicDegree",   anisotropicDegree);
	addField (inputOutput,    "texturePriority",     texturePriority);
}

X3DBaseNode*
TextureProperties::create (X3DExecutionContext* const executionContext) const
{
	return new TextureProperties (executionContext);
}

void
TextureProperties::initialize ()
{
	X3DPropertyNode::initialize ();
}

GLenum
TextureProperties::getBoundaryMode (const std::string & boundaryMode) const
{
	if (boundaryMode == "CLAMP")
		return GL_CLAMP;

	if (boundaryMode == "CLAMP_TO_EDGE")
		return GL_CLAMP_TO_EDGE;

	if (boundaryMode == "CLAMP_TO_BOUNDARY")
		return GL_CLAMP_TO_BORDER;

	if (boundaryMode == "MIRRORED_REPEAT")
		return GL_MIRRORED_REPEAT;

	//if (boundaryMode == "REPEAT")
	return GL_REPEAT;
}

GLenum
TextureProperties::getBoundaryModeS () const
{
	return getBoundaryMode (boundaryModeS);
}

GLenum
TextureProperties::getBoundaryModeT () const
{
	return getBoundaryMode (boundaryModeT);
}

GLenum
TextureProperties::getBoundaryModeR () const
{
	return getBoundaryMode (boundaryModeR);
}

GLenum
TextureProperties::getMinificationFilter () const
{
	if (minificationFilter == "AVG_PIXEL")
		return GL_LINEAR;

	if (minificationFilter == "AVG_PIXEL_AVG_MIPMAP")
		return GL_LINEAR_MIPMAP_LINEAR;

	if (minificationFilter == "AVG_PIXEL_NEAREST_MIPMAP")
		return GL_LINEAR_MIPMAP_NEAREST;

	if (minificationFilter == "NEAREST_PIXEL")
		return GL_NEAREST;

	if (minificationFilter == "NEAREST_PIXEL_AVG_MIPMAP")
		return GL_NEAREST_MIPMAP_LINEAR;

	if (minificationFilter == "NEAREST_PIXEL_NEAREST_MIPMAP")
		return GL_NEAREST_MIPMAP_NEAREST;

	if (minificationFilter == "DEFAULT")
		return getBrowser () -> getBrowserOptions () -> textureProperties -> getMinificationFilter ();

	if (minificationFilter == "FASTEST")
		return GL_NEAREST;

	// if (minificationFilter == "NICEST")
	return generateMipMaps
	       ? GL_LINEAR_MIPMAP_LINEAR
			 : GL_LINEAR;
}

GLenum
TextureProperties::getMagnificationFilter () const
{
	if (magnificationFilter == "AVG_PIXEL")
		return GL_LINEAR;

	if (magnificationFilter == "NEAREST_PIXEL")
		return GL_NEAREST;

	if (magnificationFilter == "DEFAULT")
		return getBrowser () -> getBrowserOptions () -> textureProperties -> getMagnificationFilter ();

	if (magnificationFilter == "FASTEST")
		return GL_NEAREST;

	// if (magnificationFilter == "NICEST")
	return GL_LINEAR;
}

CompressionMode
TextureProperties::getTextureCompression () const
{
	if (textureCompression == "LOW")
		return CompressionMode::LOW;

	if (textureCompression == "MEDIUM")
		return CompressionMode::MEDIUM;

	if (textureCompression == "HIGH")
		return CompressionMode::HIGH;

	if (textureCompression == "DEFAULT")
		return getBrowser () -> getBrowserOptions () -> textureProperties -> getTextureCompression ();

	if (textureCompression == "FASTEST")
		return CompressionMode::FASTEST;

	//if (textureCompression == "NICEST")
	return CompressionMode::NICEST;

}

GLenum
TextureProperties::getInternalFormat (int32_t components) const
{
	switch (components)
	{
		case 1:
		{
			switch (getTextureCompression ())
			{
				case LOW:
				case MEDIUM:
				case HIGH:
					return GL_COMPRESSED_LUMINANCE;
				case DEFAULT:
				case FASTEST:
				case NICEST:
					return GL_LUMINANCE;
			}
		}
		case 2:
		{
			switch (getTextureCompression ())
			{
				case LOW:
				case MEDIUM:
				case HIGH:
					return GL_COMPRESSED_LUMINANCE_ALPHA;
				case DEFAULT:
				case FASTEST:
				case NICEST:
					return GL_LUMINANCE_ALPHA;
			}
		}
		case 3:
		{
			switch (getTextureCompression ())
			{
				case LOW:
				case MEDIUM:
				case HIGH:
					return GL_COMPRESSED_RGB;
				case DEFAULT:
				case FASTEST:
				case NICEST:
					return GL_RGB;
			}
		}
		case 4:
		{
			switch (getTextureCompression ())
			{
				case LOW:
				case MEDIUM:
				case HIGH:
					return GL_COMPRESSED_RGBA;
				case DEFAULT:
				case FASTEST:
				case NICEST:
					return GL_RGBA;
			}
		}
	}

	return GL_RGBA;
}

} // X3D
} // titania
