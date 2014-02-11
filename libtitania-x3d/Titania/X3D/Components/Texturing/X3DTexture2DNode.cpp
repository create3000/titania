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

#include "X3DTexture2DNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"

namespace titania {
namespace X3D {

X3DTexture2DNode::Fields::Fields () :
	          repeatS (new SFBool (true)),
	          repeatT (new SFBool (true)),
	textureProperties (new SFNode ())
{ }

X3DTexture2DNode::X3DTexture2DNode () :
	       X3DTextureNode (),
	               fields (),
	          transparent (false),
	                width (0),
	               height (0),
	           components (0),
	texturePropertiesNode ()
{
	addNodeType (X3DConstants::X3DTexture2DNode);

	addChildren (texturePropertiesNode);
}

void
X3DTexture2DNode::initialize ()
{
	X3DTextureNode::initialize ();

	repeatS ()           .addInterest (this, &X3DTexture2DNode::updateTextureProperties);
	repeatT ()           .addInterest (this, &X3DTexture2DNode::updateTextureProperties);
	textureProperties () .addInterest (this, &X3DTexture2DNode::set_textureProperties);

	set_textureProperties ();
}

void
X3DTexture2DNode::set_textureProperties ()
{
	if (texturePropertiesNode)
		texturePropertiesNode -> removeInterest (this, &X3DTexture2DNode::updateTextureProperties);

	texturePropertiesNode = x3d_cast <TextureProperties*> (textureProperties ());

	if (not texturePropertiesNode)
		texturePropertiesNode = x3d_cast <TextureProperties*> (getBrowser () -> getBrowserOptions () -> textureProperties ());

	texturePropertiesNode -> addInterest (this, &X3DTexture2DNode::updateTextureProperties);

	updateTextureProperties ();
}

void
X3DTexture2DNode::setTexture (const TexturePtr & texture)
{
	if (texture)
	{
		setImage (getInternalFormat (texture -> getComponents ()),
		          texture -> getComponents (),
		          texture -> getWidth (), texture -> getHeight (),
		          texture -> getFormat (),
		          texture -> getData ());
	}
	else
		setImage (getInternalFormat (3), 3, 0, 0, GL_RGB, nullptr);
}

void
X3DTexture2DNode::setImage (const GLenum internalFormat, const size_t comp, const GLint w, const GLint h, const GLenum format, const void* const data)
{
	// transfer image

	width       = w;
	height      = h;
	components  = comp;
	transparent = math::is_even (comp);

	updateTextureProperties ();

	glBindTexture (GL_TEXTURE_2D, getTextureId ());

	glTexImage2D (GL_TEXTURE_2D,
	              0,     // This texture is level 0 in mimpap generation.
	              internalFormat,
	              width, height,
	              0, /* clamp <int> (texturePropertiesNode -> borderWidth (), 0, 1), */ // This value must be 0.
	              format, GL_UNSIGNED_BYTE,
	              data);

	glBindTexture (GL_TEXTURE_2D, 0);

	addEvent ();
}

void
X3DTexture2DNode::updateTextureProperties ()
{
	X3DTextureNode::updateTextureProperties (GL_TEXTURE_2D, textureProperties (), texturePropertiesNode, width, height, repeatS (), repeatT (), false);
}

void
X3DTexture2DNode::updateImage (GLenum format, GLint width, GLint height, const void* const data)
{
	// update image

	glBindTexture (GL_TEXTURE_2D, getTextureId ());
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);
	glBindTexture (GL_TEXTURE_2D, 0);

	addEvent ();
}

void
X3DTexture2DNode::draw ()
{
	X3DTextureNode::draw (GL_TEXTURE_2D, components);
}

void
X3DTexture2DNode::dispose ()
{
	texturePropertiesNode .dispose ();

	X3DTextureNode::dispose ();
}

} // X3D
} // titania
