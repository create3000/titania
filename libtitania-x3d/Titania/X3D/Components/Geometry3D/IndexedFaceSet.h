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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__

#include "../Rendering/X3DComposedGeometryNode.h"
#include <array>

namespace titania {
namespace X3D {

class IndexedFaceSet :
	public X3DComposedGeometryNode
{
public:

	IndexedFaceSet (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final;

	///  @name Fields

	SFBool &
	convex ()
	{ return *fields .convex; }

	const SFBool &
	convex () const
	{ return *fields .convex; }

	SFFloat &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	const SFFloat &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	MFInt32 &
	texCoordIndex ()
	{ return *fields .texCoordIndex; }

	const MFInt32 &
	texCoordIndex () const
	{ return *fields .texCoordIndex; }

	MFInt32 &
	colorIndex ()
	{ return *fields .colorIndex; }

	const MFInt32 &
	colorIndex () const
	{ return *fields .colorIndex; }

	MFInt32 &
	normalIndex ()
	{ return *fields .normalIndex; }

	const MFInt32 &
	normalIndex () const
	{ return *fields .normalIndex; }

	MFInt32 &
	coordIndex ()
	{ return *fields .coordIndex; }

	const MFInt32 &
	coordIndex () const
	{ return *fields .coordIndex; }


private:

	typedef std::deque <size_t>   Vertices;
	typedef std::deque <size_t>   Element;
	typedef std::deque <Element>  ElementArray;

	struct Polygon
	{
		Polygon (Vertices && vertices, ElementArray && elements) :
			vertices (std::move (vertices)),
			elements (std::move (elements))
		{ }
	
		Vertices vertices;
		ElementArray elements;

	};

	typedef std::deque <Polygon> PolygonArray;

	virtual
	void
	initialize () final;

	void
	set_coordIndex ();

	void
	set_texCoordIndex ();

	void
	set_colorIndex ();

	void
	set_normalIndex ();

	virtual
	void
	build () final;

	void
	buildNormals (const PolygonArray &);

	void
	tesselate (PolygonArray &, size_t &);

	ElementArray
	tesselate (const Vertices & polygon);

	struct Fields
	{
		Fields ();

		SFBool* const convex;
		SFFloat* const creaseAngle;
		MFInt32* const texCoordIndex;
		MFInt32* const colorIndex;
		MFInt32* const normalIndex;
		MFInt32* const coordIndex;
	};

	Fields fields;

	size_t numPolygons;

};

} // X3D
} // titania

#endif
