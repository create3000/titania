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

#ifndef __TITANIA_X3D_COMPONENTS_TEXTURING_X3DTEXTURE2DNODE_H__
#define __TITANIA_X3D_COMPONENTS_TEXTURING_X3DTEXTURE2DNODE_H__

#include "../../Browser/Texturing/Texture.h"
#include "../Texturing/X3DTextureNode.h"

namespace titania {
namespace X3D {

class X3DTexture2DNode :
	public X3DTextureNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) override;

	///  @name Fields

	SFBool &
	repeatS ()
	{ return *fields .repeatS; }

	const SFBool &
	repeatS () const
	{ return *fields .repeatS; }

	SFBool &
	repeatT ()
	{ return *fields .repeatT; }

	const SFBool &
	repeatT () const
	{ return *fields .repeatT; }

	SFNode &
	textureProperties ()
	{ return *fields .textureProperties; }

	const SFNode &
	textureProperties () const
	{ return *fields .textureProperties; }

	const SFInt32 &
	width () const
	{ return fields .width; }

	SFInt32 &
	width ()
	{ return fields .width; }

	SFInt32 &
	height ()
	{ return fields .height; }

	const SFInt32 &
	height () const
	{ return fields .height; }

	SFInt32 &
	components ()
	{ return fields .components; }

	const SFInt32 &
	components () const
	{ return fields .components; }

	///  @name Member access

	virtual
	GLenum
	getTarget () const final override
	{ return GL_TEXTURE_2D; }

	size_t
	getWidth () const
	{ return textureWidth; }

	size_t
	getHeight () const
	{ return textureHeight; }

	///  throws Error <X3D::INVALID_NODE>, Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
	Magick::Image
	getImage () const;

	///  @name Operations

	virtual
	void
	draw (X3DRenderObject* const renderObject) override;

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const shaderObject, const size_t i) const final override;


protected:

	///  @name Construction

	X3DTexture2DNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	const X3DPtr <TextureProperties> &
	getDefaultTextureProperties () const;

	virtual
	void
	setTexture (const TexturePtr & texture);

	void
	clearTexture ();

	void
	setImage (const GLenum internalFormat,
	          const bool transparent,
	          const size_t comp,
	          const GLint width,
	          const GLint height,
	          const GLenum format,
	          const void* const data);

	void
	updateImage (const GLenum format, const void* const data);

	///  @name Operations

	virtual
	void
	update () = 0;


private:

	///  @name Event handlers

	void
	set_textureProperties ();

	///  @name Member access

	GLenum
	getInternalFormat (const size_t components) const
	{ return texturePropertiesNode -> getInternalFormat (components); }

	///  @name Operations

	void
	updateTextureProperties ();

	///  throws Error <X3D::INVALID_NODE>, Error <X3D::INVALID_OPERATION_TIMING>, Error <X3D::DISPOSED>
	std::vector <uint8_t>
	getImageData () const;

	///  @name Fields

	struct Fields
	{
		Fields ();

		SFBool* const repeatS;
		SFBool* const repeatT;
		SFNode* const textureProperties;
		SFInt32 width;
		SFInt32 height;
		SFInt32 components;
	};

	Fields fields;

	///  @name Members

	size_t                     textureWidth;
	size_t                     textureHeight;
	X3DPtr <TextureProperties> texturePropertiesNode;

};

} // X3D
} // titania

#endif
