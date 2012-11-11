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
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_EXTRUSION_H__

#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class Extrusion :
	public X3DGeometryNode
{
public:

	using X3DGeometryNode::ccw;
	using X3DGeometryNode::solid;
	using X3DGeometryNode::creaseAngle;

	MFVec2f    set_crossSection;
	MFRotation set_orientation;
	MFVec2f    set_scale;
	MFVec3f    set_spine;
	SFBool     beginCap;
	SFBool     convex;
	MFVec2f    crossSection;
	SFBool     endCap;
	MFRotation orientation;
	MFVec2f    scale;
	MFVec3f    spine;

	Extrusion (X3DExecutionContext* const);

	virtual
	X3DBasicNode*
	create (X3DExecutionContext* const) const;

	virtual
	void
	dispose ();


private:

	virtual
	void
	initialize ();

	std::vector <Vector3f>
	createPoints ();

	void
	build ();

	static void tessBeginData (GLenum, void*);

	static void
	tessVertexData (void*, void*);

	static void tessCombineData (GLdouble [3], void* [4], GLfloat [4], void**, void*);

	static void
	tessEndData (void*);

	static void tessError (GLenum);

	class Vertex
	{
	public:

		std::vector <GLdouble> location;
		int                    i;
		int                    k;
		Vertex (const std::vector <Vector3f> & points, const int i, const int k) :
			i (i),
			k (k)
		{
			float x = points [i] .x ();
			float y = points [i] .y ();
			float z = points [i] .z ();

			location .reserve (3);
			location .push_back (x);
			location .push_back (y);
			location .push_back (z);
		}

	};

	class PolygonElement
	{
	public:

		GLenum               type;
		std::deque <Vertex*> vertices;
		PolygonElement (GLenum type) :
			type (type)
		{ }

	};

	typedef std::deque <PolygonElement> Polygon;

	GLUtesselator* tess;

};

} // X3D
} // titania

#endif
