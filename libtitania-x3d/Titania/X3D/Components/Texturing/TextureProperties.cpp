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

#include "TextureProperties.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string TextureProperties::componentName  = "Texturing";
const std::string TextureProperties::typeName       = "TextureProperties";
const std::string TextureProperties::containerField = "textureProperties";

// http://new.web3d.org/files/specifications/19775-1/V3.2/Part01/components/texturing.html#TextureProperties

TextureProperties::Fields::Fields () :
	        borderColor (new SFColorRGBA ()),
	        borderWidth (new SFInt32 ()),
	  anisotropicDegree (new SFFloat (1)),
	    generateMipMaps (new SFBool ()),
	 minificationFilter (new SFString ("FASTEST")),
	magnificationFilter (new SFString ("FASTEST")),
	      boundaryModeS (new SFString ("REPEAT")),
	      boundaryModeT (new SFString ("REPEAT")),
	      boundaryModeR (new SFString ("REPEAT")),
	 textureCompression (new SFString ("FASTEST")),
	    texturePriority (new SFFloat ())
{ }

TextureProperties::TextureProperties (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DNode (),
	     fields ()
{
	addType (X3DConstants::TextureProperties);

	addField (inputOutput,    "metadata",            metadata ());
	addField (inputOutput,    "borderColor",         borderColor ());
	addField (inputOutput,    "borderWidth",         borderWidth ());
	addField (inputOutput,    "anisotropicDegree",   anisotropicDegree ());
	addField (initializeOnly, "generateMipMaps",     generateMipMaps ());
	addField (inputOutput,    "minificationFilter",  minificationFilter ());
	addField (inputOutput,    "magnificationFilter", magnificationFilter ());
	addField (inputOutput,    "boundaryModeS",       boundaryModeS ());
	addField (inputOutput,    "boundaryModeT",       boundaryModeT ());
	addField (inputOutput,    "boundaryModeR",       boundaryModeR ());
	addField (inputOutput,    "textureCompression",  textureCompression ());
	addField (inputOutput,    "texturePriority",     texturePriority ());
}

X3DBaseNode*
TextureProperties::create (X3DExecutionContext* const executionContext) const
{
	return new TextureProperties (executionContext);
}

void
TextureProperties::initialize ()
{
	X3DNode::initialize ();
}

GLenum
TextureProperties::getBoundaryMode (const std::string & boundaryMode) const
{
	static const std::map <std::string, GLenum> boundaryModes = {
		std::pair ("CLAMP",             GL_CLAMP),
		std::pair ("CLAMP_TO_EDGE",     GL_CLAMP_TO_EDGE),
		std::pair ("CLAMP_TO_BOUNDARY", GL_CLAMP_TO_BORDER),
		std::pair ("MIRRORED_REPEAT",   GL_MIRRORED_REPEAT),
		std::pair ("REPEAT",            GL_REPEAT),
	};

	try
	{
		return boundaryModes .at (boundaryMode);
	}
	catch (const std::out_of_range &)
	{
		// DEFAULT
		return GL_REPEAT;
	}
}

GLenum
TextureProperties::getBoundaryModeS () const
{
	return getBoundaryMode (boundaryModeS ());
}

GLenum
TextureProperties::getBoundaryModeT () const
{
	return getBoundaryMode (boundaryModeT ());
}

GLenum
TextureProperties::getBoundaryModeR () const
{
	return getBoundaryMode (boundaryModeR ());
}

GLenum
TextureProperties::getMinificationFilter () const
{
	static const std::map <std::string, GLenum> minificationFilters = {
		std::pair ("AVG_PIXEL_AVG_MIPMAP",         GL_LINEAR_MIPMAP_LINEAR),
		std::pair ("AVG_PIXEL",                    GL_LINEAR),
		std::pair ("AVG_PIXEL_NEAREST_MIPMAP",     GL_LINEAR_MIPMAP_NEAREST),
		std::pair ("NEAREST_PIXEL_AVG_MIPMAP",     GL_NEAREST_MIPMAP_LINEAR),
		std::pair ("NEAREST_PIXEL_NEAREST_MIPMAP", GL_NEAREST_MIPMAP_NEAREST),
		std::pair ("NEAREST_PIXEL",                GL_NEAREST),
		std::pair ("NICEST",                       GL_LINEAR_MIPMAP_LINEAR),
		std::pair ("FASTEST",                      GL_NEAREST)
	};

	try
	{
		if (generateMipMaps ())
			return minificationFilters .at (minificationFilter ());
		
		return GL_LINEAR; // or GL_NEAREST
	}
	catch (const std::out_of_range &)
	{
		// DEFAULT
		return getBrowser () -> getDefaultTextureProperties () -> getMinificationFilter ();
	}
}

GLenum
TextureProperties::getMagnificationFilter () const
{
	static const std::map <std::string, GLenum> magnificationFilters = {
		std::pair ("AVG_PIXEL",     GL_LINEAR),
		std::pair ("NEAREST_PIXEL", GL_NEAREST),
		std::pair ("NICEST",        GL_LINEAR),
		std::pair ("FASTEST",       GL_NEAREST)
	};

	try
	{
		return magnificationFilters .at (magnificationFilter ());
	}
	catch (const std::out_of_range &)
	{
		// DEFAULT
		return getBrowser () -> getDefaultTextureProperties () -> getMagnificationFilter ();
	}
}

CompressionMode
TextureProperties::getTextureCompression () const
{
	static const std::map <std::string, CompressionMode> textureCompressions = {
		std::pair ("HIGH",    CompressionMode::HIGH),
		std::pair ("MEDIUM",  CompressionMode::MEDIUM),
		std::pair ("LOW",     CompressionMode::LOW),
		std::pair ("FASTEST", CompressionMode::FASTEST),
		std::pair ("NICEST",  CompressionMode::NICEST)
	};

	try
	{
		return textureCompressions .at (textureCompression ());
	}
	catch (const std::out_of_range &)
	{
		// DEFAULT
		return getBrowser () -> getDefaultTextureProperties () -> getTextureCompression ();
	}
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
