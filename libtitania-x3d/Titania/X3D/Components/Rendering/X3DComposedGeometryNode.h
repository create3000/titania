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

#ifndef __TITANIA_X3D_COMPONENTS_RENDERING_X3DCOMPOSED_GEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_RENDERING_X3DCOMPOSED_GEOMETRY_NODE_H__

#include "../EnvironmentalEffects/FogCoordinate.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Rendering/X3DNormalNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

namespace titania {
namespace X3D {

class X3DComposedGeometryNode :
	public X3DGeometryNode
{
public:

	///  @name Fields

	SFBool &
	colorPerVertex ()
	{ return *fields .colorPerVertex; }

	const SFBool &
	colorPerVertex () const
	{ return *fields .colorPerVertex; }

	SFBool &
	normalPerVertex ()
	{ return *fields .normalPerVertex; }

	const SFBool &
	normalPerVertex () const
	{ return *fields .normalPerVertex; }

	SFBool &
	solid ()
	{ return *fields .solid; }

	const SFBool &
	solid () const
	{ return *fields .solid; }

	SFBool &
	ccw ()
	{ return *fields .ccw; }

	const SFBool &
	ccw () const
	{ return *fields .ccw; }

	MFNode &
	attrib ()
	{ return *fields .attrib; }

	const MFNode &
	attrib () const
	{ return *fields .attrib; }

	SFNode &
	fogCoord ()
	{ return *fields .fogCoord; }

	const SFNode &
	fogCoord () const
	{ return *fields .fogCoord; }

	SFNode &
	color ()
	{ return *fields .color; }

	const SFNode &
	color () const
	{ return *fields .color; }

	SFNode &
	texCoord ()
	{ return *fields .texCoord; }

	const SFNode &
	texCoord () const
	{ return *fields .texCoord; }

	SFNode &
	normal ()
	{ return *fields .normal; }

	const SFNode &
	normal () const
	{ return *fields .normal; }

	SFNode &
	coord ()
	{ return *fields .coord; }

	const SFNode &
	coord () const
	{ return *fields .coord; }

	///  @name Tests

	virtual
	bool
	isTransparent () const final override
	{ return transparent; }


protected:

	///  @name Construction

	X3DComposedGeometryNode ();

	virtual
	void
	initialize () override;

	///  @name Event handlers

	void
	set_attrib ();

	void
	set_color ();

	void
	set_texCoord ();

	void
	set_normal ();

	void
	set_coord ();

	///  @name Member access

	const X3DPtrArray <X3DVertexAttributeNode> &
	getAttrib () const
	{ return attribNodes; }

	const X3DPtr <X3DColorNode> &
	getColor () const
	{ return colorNode; }

	const X3DPtr <X3DTextureCoordinateNode> &
	getTexCoord () const
	{ return texCoordNode; }

	const X3DPtr <X3DNormalNode> &
	getNormal () const
	{ return normalNode; }

	const X3DPtr <X3DCoordinateNode> &
	getCoord () const
	{ return coordNode; }

	virtual
	size_t
	getIndex (const size_t index) const
	{ return index; }

	///  @name Operations

	virtual
	Box3f
	createBBox () final override;

	void
	build (const size_t, size_t);

	virtual
	void
	buildNormals (const size_t, const size_t);

	void
	buildFaceNormals (const size_t, const size_t);

	SFNode
	toPrimitive (const size_t, size_t) const;


private:

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const colorPerVertex;
		SFBool* const normalPerVertex;
		SFBool* const solid;
		SFBool* const ccw;
		MFNode* const attrib;
		SFNode* const fogCoord;
		SFNode* const color;
		SFNode* const texCoord;
		SFNode* const normal;
		SFNode* const coord;
	};

	Fields fields;

	X3DPtrArray <X3DVertexAttributeNode> attribNodes;
	X3DPtr <X3DColorNode>                colorNode;
	X3DPtr <X3DTextureCoordinateNode>    texCoordNode;
	X3DPtr <X3DNormalNode>               normalNode;
	X3DPtr <X3DCoordinateNode>           coordNode;
	bool                                 transparent;

};

} // X3D
} // titania

#endif
