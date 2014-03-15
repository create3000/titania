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

#include "X3DTexture3DNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

X3DTexture3DNode::Fields::Fields () :
	          repeatS (new SFBool ()),
	          repeatT (new SFBool ()),
	          repeatR (new SFBool ()),
	textureProperties (new SFNode ())
{ }

X3DTexture3DNode::X3DTexture3DNode () :
	       X3DTextureNode (),
	               fields (),
	                width (0),
	               height (0),
	           components (0),
	          transparent (false),
	texturePropertiesNode ()
{
	addNodeType (X3DConstants::X3DTexture3DNode);

	addChildren (texturePropertiesNode);
}

void
X3DTexture3DNode::initialize ()
{
	X3DTextureNode::initialize ();

	repeatS ()           .addInterest (this, &X3DTexture3DNode::updateTextureProperties);
	repeatT ()           .addInterest (this, &X3DTexture3DNode::updateTextureProperties);
	repeatR ()           .addInterest (this, &X3DTexture3DNode::updateTextureProperties);
	textureProperties () .addInterest (this, &X3DTexture3DNode::set_textureProperties);

	set_textureProperties ();
}

void
X3DTexture3DNode::set_textureProperties ()
{
	if (texturePropertiesNode)
		texturePropertiesNode -> removeInterest (this, &X3DTexture3DNode::updateTextureProperties);

	texturePropertiesNode = x3d_cast <TextureProperties*> (textureProperties ());

	if (not texturePropertiesNode)
		texturePropertiesNode = x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties ());

	texturePropertiesNode -> addInterest (this, &X3DTexture3DNode::updateTextureProperties);

	updateTextureProperties ();
}

void
X3DTexture3DNode::setTexture (const Texture3DPtr & texture)
{
	if (texture)
	{
		setImage (getInternalFormat (texture -> getComponents ()),
		          texture -> getComponents (),
		          texture -> getWidth (), texture -> getHeight (),
		          texture -> getDepth (),
		          texture -> getFormat (),
		          texture -> getData ());
	}
	else
		setImage (getInternalFormat (3), 3, 0, 0, 0, GL_RGB, nullptr);
}

void
X3DTexture3DNode::setImage (const GLenum internalFormat, const size_t comp, const GLint w, const GLint h, const GLint depth, const GLenum format, const void* const data)
{
	if (glXGetCurrentContext ())
	{
		// transfer image

		width       = w;
		height      = h;
		components  = comp;
		transparent = math::is_even (comp);

		updateTextureProperties ();

		glBindTexture (GL_TEXTURE_3D, getTextureId ());

		glTexImage3D (GL_TEXTURE_3D,
		              0,     // This texture is level 0 in mimpap generation.
		              internalFormat,
		              width, height, depth,
		              0, /* clamp <int> (texturePropertiesNode -> borderWidth (), 0, 1), */ // This value must be 0.
		              format, GL_UNSIGNED_BYTE,
		              data);

		glBindTexture (GL_TEXTURE_3D, 0);

		addEvent ();
	}
}

void
X3DTexture3DNode::updateTextureProperties ()
{
	X3DTextureNode::updateTextureProperties (GL_TEXTURE_3D, textureProperties (), texturePropertiesNode, width, height, repeatS (), repeatT (), repeatR ());
}

void
X3DTexture3DNode::draw ()
{
	X3DTextureNode::draw (GL_TEXTURE_3D, components);
}

} // X3D
} // titania
