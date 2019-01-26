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

#include "ComposedTexture3D.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const Component   ComposedTexture3D::component      = Component ("Texturing3D", 1);
const std::string ComposedTexture3D::typeName       = "ComposedTexture3D";
const std::string ComposedTexture3D::containerField = "texture";

ComposedTexture3D::Fields::Fields () :
	texture (new MFNode ())
{ }

ComposedTexture3D::ComposedTexture3D (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DTexture3DNode (),
	          fields (),
	       loadState (NOT_STARTED_STATE),
	    textureNodes ()
{
	addType (X3DConstants::ComposedTexture3D);

	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "repeatS",           repeatS ());
	addField (initializeOnly, "repeatT",           repeatT ());
	addField (initializeOnly, "repeatR",           repeatR ());
	addField (initializeOnly, "textureProperties", textureProperties ());
	addField (inputOutput,    "texture",           texture ());
	
	addChildObjects (loadState, textureNodes);
}

X3DBaseNode*
ComposedTexture3D::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedTexture3D (executionContext);
}

void
ComposedTexture3D::initialize ()
{
	X3DTexture3DNode::initialize ();

	if (getBrowser () -> getLoadUrlObjects ())
	{
		texture () .addInterest (&ComposedTexture3D::set_texture, this);

		textureNodes .addInterest (&ComposedTexture3D::update, this);

		set_texture ();
	}
}

void
ComposedTexture3D::set_texture ()
{
	for (const auto & node : textureNodes)
		node -> removeInterest (textureNodes);

	textureNodes = texture ();

	for (const auto & node : textureNodes)
		node -> addInterest (textureNodes);
}

void
ComposedTexture3D::update ()
{
	width ()      = 0;
	height ()     = 0;
	components () = 0;
	depth ()      = 0;

	std::vector <char> image;

	for (const auto & node : texture ())
	{
		auto texture = x3d_cast <X3DTexture2DNode*> (node);

		if (texture)
		{
			++ depth ();

			// Get texture 2d data

			width ()      = texture -> getWidth ();
			height ()     = texture -> getHeight ();
			components () = std::max <int32_t> (components (), texture -> components ());

			size_t first = image .size ();
			image .resize (first + width () * height () * 4);

			glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());
			glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data () + first);
		}
	}

	glBindTexture (GL_TEXTURE_2D, 0);

	image .resize (width () * height () * depth () * 4, 0xFF);

	setImage (getInternalFormat (components ()), components (), width (), height (), depth (), GL_RGBA, image .data ());

	loadState = COMPLETE_STATE;
}

} // X3D
} // titania
