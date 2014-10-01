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

#include "ComposedCubeMapTexture.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Texturing/X3DTexture2DNode.h"

namespace titania {
namespace X3D {

const ComponentType ComposedCubeMapTexture::component      = ComponentType::CUBE_MAP_TEXTURING;
const std::string   ComposedCubeMapTexture::typeName       = "ComposedCubeMapTexture";
const std::string   ComposedCubeMapTexture::containerField = "texture";

const GLenum ComposedCubeMapTexture::targets [6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y

};

ComposedCubeMapTexture::Fields::Fields () :
	 front (new SFNode ()),
	  back (new SFNode ()),
	  left (new SFNode ()),
	 right (new SFNode ()),
	bottom (new SFNode ()),
	   top (new SFNode ())
{ }

ComposedCubeMapTexture::ComposedCubeMapTexture (X3DExecutionContext* const executionContext) :
	              X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentTextureNode (),
	                   fields (),
	                    nodes (),
	              transparent (false),
	               components (0)
{
	addType (X3DConstants::ComposedCubeMapTexture);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "front",    front ());
	addField (inputOutput, "back",     back ());
	addField (inputOutput, "left",     left ());
	addField (inputOutput, "right",    right ());
	addField (inputOutput, "bottom",   bottom ());
	addField (inputOutput, "top",      top ());

	addChildren (nodes);
}

X3DBaseNode*
ComposedCubeMapTexture::create (X3DExecutionContext* const executionContext) const
{
	return new ComposedCubeMapTexture (executionContext);
}

void
ComposedCubeMapTexture::initialize ()
{
	X3DEnvironmentTextureNode::initialize ();

	if (glXGetCurrentContext ())
	{
		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());

		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);

		front ()  .addInterest (this, &ComposedCubeMapTexture::set_texture, 0, std::cref (front ()));
		back ()   .addInterest (this, &ComposedCubeMapTexture::set_texture, 1, std::cref (back ()));
		left ()   .addInterest (this, &ComposedCubeMapTexture::set_texture, 2, std::cref (left ()));
		right ()  .addInterest (this, &ComposedCubeMapTexture::set_texture, 3, std::cref (right ()));
		bottom () .addInterest (this, &ComposedCubeMapTexture::set_texture, 4, std::cref (bottom ()));
		top ()    .addInterest (this, &ComposedCubeMapTexture::set_texture, 5, std::cref (top ()));
		
		nodes .resize (6);

		set_texture (0, front ());
		set_texture (1, back ());
		set_texture (2, right ());
		set_texture (3, left ());
		set_texture (4, bottom ());
		set_texture (5, top ());
	}
}

void
ComposedCubeMapTexture::set_texture (const size_t index, const SFNode & node)
{
	if (nodes [index])
		nodes [index] -> removeInterest (this, &ComposedCubeMapTexture::set_texture);

	nodes [index] = node;

	if (nodes [index])
		nodes [index] -> addInterest (this, &ComposedCubeMapTexture::set_texture, index, std::cref (node));

	setTexture (targets [index], node);
}

void
ComposedCubeMapTexture::setTexture (const GLenum target, const SFNode & node)
{
	const auto texture = x3d_cast <X3DTexture2DNode*> (node);

	if (texture)
	{
		// Get transparent and components

		transparent = false;
		components  = 0;

		for (const auto & node : nodes)
		{
				const auto texture = x3d_cast <X3DTexture2DNode*> (node);

				if (texture)
				{
					transparent = transparent or texture -> isTransparent ();
					components  = std::max (components, texture -> getComponents ());
				}
		}

		// Get texture 2d data

		const size_t width      = texture -> getWidth ();
		const size_t height     = texture -> getHeight ();
		const size_t components = 4;

		std::vector <char> image (width * height * components);

		glBindTexture (GL_TEXTURE_2D, texture -> getTextureId ());
		glGetTexImage (GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_2D, 0);

		// Transfer image
		// Important: width and height must be equal, and all images must be of the same size!!!

		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());
		glTexImage2D (target, 0, GL_RGBA, width, height, false, GL_RGBA, GL_UNSIGNED_BYTE, image .data ());
		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	}
	else
	{
		glBindTexture (GL_TEXTURE_CUBE_MAP, getTextureId ());
		glTexImage2D (target, 0, GL_RGBA, 0, 0, false, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture (GL_TEXTURE_CUBE_MAP, 0);
	}
}

void
ComposedCubeMapTexture::draw ()
{
	glMatrixMode (GL_TEXTURE);
	glScalef (-1, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	X3DEnvironmentTextureNode::draw (GL_TEXTURE_CUBE_MAP, components);
}

} // X3D
} // titania
