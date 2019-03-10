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

#ifndef __TITANIA_X3D_COMPONENTS_CUBE_MAP_TEXTURING_COMPOSED_CUBE_MAP_TEXTURE_H__
#define __TITANIA_X3D_COMPONENTS_CUBE_MAP_TEXTURING_COMPOSED_CUBE_MAP_TEXTURE_H__

#include "../CubeMapTexturing/X3DEnvironmentTextureNode.h"

namespace titania {
namespace X3D {

class ComposedCubeMapTexture :
	public X3DEnvironmentTextureNode
{
public:

	///  @name Construction

	ComposedCubeMapTexture (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	const Component &
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	SFNode &
	front ()
	{ return *fields .front; }

	const SFNode &
	front () const
	{ return *fields .front; }

	SFNode &
	back ()
	{ return *fields .back; }

	const SFNode &
	back () const
	{ return *fields .back; }

	SFNode &
	left ()
	{ return *fields .left; }

	const SFNode &
	left () const
	{ return *fields .left; }

	SFNode &
	right ()
	{ return *fields .right; }

	const SFNode &
	right () const
	{ return *fields .right; }

	SFNode &
	top ()
	{ return *fields .top; }

	const SFNode &
	top () const
	{ return *fields .top; }

	SFNode &
	bottom ()
	{ return *fields .bottom; }

	const SFNode &
	bottom () const
	{ return *fields .bottom; }

	///  @name Operations

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return loadState; }

	virtual
	size_t
	getWidth () const final override
	{ return width; }

	virtual
	size_t
	getHeight () const final override
	{ return height; }

	virtual
	size_t
	getComponents () const final override
	{ return components; }

	///  @name Construction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Member access

	void
	setLoadState (const LoadState & value)
	{ loadState = value; }

	///  @name Event handlers

	void
	set_texture (const SFNode & node, const size_t index);

	void
	setTexture (const GLenum target, const SFNode & node);

	///  @name Static members

	static const Component   component;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFNode* const front;
		SFNode* const back;
		SFNode* const left;
		SFNode* const right;
		SFNode* const top;
		SFNode* const bottom;
	};

	Fields fields;

	MFNode nodes;
	size_t width;
	size_t height;
	size_t components;

	SFEnum <LoadState> loadState;

};

} // X3D
} // titania

#endif
