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

#ifndef __TITANIA_X3D_COMPONENTS_TEXTURING3D_X3DTEXTURE3DNODE_H__
#define __TITANIA_X3D_COMPONENTS_TEXTURING3D_X3DTEXTURE3DNODE_H__

#include "../../Browser/Texturing3D/Texture3D.h"
#include "../Texturing/X3DTextureNode.h"

namespace titania {
namespace X3D {

class X3DTexture3DNode :
	public X3DTextureNode
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

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

	SFBool &
	repeatR ()
	{ return *fields .repeatR; }

	const SFBool &
	repeatR () const
	{ return *fields .repeatR; }

	SFNode &
	textureProperties ()
	{ return *fields .textureProperties; }

	const SFNode &
	textureProperties () const
	{ return *fields .textureProperties; }

	///  @name Member access

	virtual
	bool
	isTransparent () const override
	{ return transparent; }

	size_t
	getWidth () const
	{ return width; }

	size_t
	getHeight () const
	{ return height; }

	size_t
	getDepth () const
	{ return depth; }

	size_t
	getComponents () const
	{ return components; }

	virtual
	size_t
	getImageWidth () const = 0;

	virtual
	size_t
	getImageHeight () const = 0;

	///  @name Operations

	virtual
	void
	draw (X3DRenderObject* const renderObject) override;

	virtual
	void
	setShaderUniforms (X3DProgrammableShaderObject* const, const size_t) const final override;


protected:

	///  @name Construction

	X3DTexture3DNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	GLenum
	getInternalFormat (const size_t components) const
	{ return texturePropertiesNode -> getInternalFormat (components); }

	virtual
	void
	setTexture (const Texture3DPtr &);

	void
	clearTexture ();

	void
	setImage (const GLenum, size_t, const GLint w, const GLint, const GLint, const GLenum, const void* const);

	virtual
	void
	update () = 0;


private:

	///  @name Event handlers

	void
	set_textureProperties ();

	void
	updateTextureProperties ();

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const repeatS;
		SFBool* const repeatT;
		SFBool* const repeatR;
		SFNode* const textureProperties;
	};

	Fields fields;

	size_t                     width;
	size_t                     height;
	size_t                     depth;
	size_t                     components;
	bool                       transparent;
	X3DPtr <TextureProperties> texturePropertiesNode;

};

} // X3D
} // titania

#endif
