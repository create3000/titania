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

#ifndef __TITANIA_X3D_COMPONENTS_SHAPE_APPEARANCE_H__
#define __TITANIA_X3D_COMPONENTS_SHAPE_APPEARANCE_H__

#include "../Shape/X3DAppearanceNode.h"

namespace titania {
namespace X3D {

class LineProperties;
class FillProperties;
class X3DMaterialNode;
class X3DTextureNode;
class X3DTextureTransformNode;
class X3DShaderNode;

class Appearance :
	public X3DAppearanceNode
{
public:

	Appearance (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFNode <X3DBaseNode> &
	lineProperties ()
	{ return *fields .lineProperties; }

	const SFNode <X3DBaseNode> &
	lineProperties () const
	{ return *fields .lineProperties; }

	SFNode <X3DBaseNode> &
	fillProperties ()
	{ return *fields .fillProperties; }

	const SFNode <X3DBaseNode> &
	fillProperties () const
	{ return *fields .fillProperties; }

	SFNode <X3DBaseNode> &
	material ()
	{ return *fields .material; }

	const SFNode <X3DBaseNode> &
	material () const
	{ return *fields .material; }

	SFNode <X3DBaseNode> &
	texture ()
	{ return *fields .texture; }

	const SFNode <X3DBaseNode> &
	texture () const
	{ return *fields .texture; }

	SFNode <X3DBaseNode> &
	textureTransform ()
	{ return *fields .textureTransform; }

	const SFNode <X3DBaseNode> &
	textureTransform () const
	{ return *fields .textureTransform; }

	MFNode &
	shaders ()
	{ return *fields .shaders; }

	const MFNode &
	shaders () const
	{ return *fields .shaders; }

	virtual
	bool
	isTransparent () const;

	virtual
	void
	draw ();


private:

	virtual
	void
	initialize ();

	void
	set_lineProperties ();

	void
	set_fillProperties ();

	void
	set_material ();

	void
	set_texture ();

	void
	set_textureTransform ();

	void
	set_shaders ();

	struct Fields
	{
		Fields ();

		SFNode <X3DBaseNode>* const lineProperties;
		SFNode <X3DBaseNode>* const fillProperties;
		SFNode <X3DBaseNode>* const material;
		SFNode <X3DBaseNode>* const texture;
		SFNode <X3DBaseNode>* const textureTransform;
		MFNode* const shaders;
	};

	Fields fields;

	LineProperties*             _lineProperties;
	FillProperties*             _fillProperties;
	X3DMaterialNode*            _material;
	X3DTextureNode*             _texture;
	X3DTextureTransformNode*    _textureTransform;
	std::deque <X3DShaderNode*> _shaders;

};

} // X3D
} // titania

#endif
