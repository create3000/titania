/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__

#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/Normal.h"
#include "../Rendering/X3DComposedGeometryNode.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Texturing/TextureCoordinateGenerator.h"

namespace titania {
namespace X3D {

class IndexedFaceSet :
	public X3DComposedGeometryNode
{
public:

	MFInt32 set_colorIndex;
	MFInt32 set_coordIndex;
	MFInt32 set_normalIndex;
	MFInt32 set_texCoordIndex;
	MFInt32 colorIndex;
	SFBool  convex;
	MFInt32 coordIndex;
	SFFloat creaseAngle;
	MFInt32 normalIndex;
	MFInt32 texCoordIndex;

	IndexedFaceSet (X3DExecutionContext* const);

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

	void
	_set_coordIndex (const MFInt32::value_type &);

	void
	_set_texCoordIndex (const MFInt32::value_type &);

	void
	_set_colorIndex (const MFInt32::value_type &);

	void
	_set_normalIndex (const MFInt32::value_type &);

	Box3f
	createBBox ();

	std::vector <Vector2f>
	getTexCoord ();

	std::vector <Vector3f>
	getNormals ();

	void
	build ();

	void
	setPoint (const MFInt32::const_iterator &,
	          const int,
	          const SFNode <TextureCoordinate> &,
	          const SFNode <TextureCoordinateGenerator> &,
	          const std::vector <Vector2f> &,
	          const SFNode <Normal> &,
	          const Vector3f &,
	          const std::vector <Vector3f> &,
	          const SFNode <Color> &,
	          const SFNode <ColorRGBA> &,
	          const SFColor &,
	          const SFColorRGBA,
	          const SFNode <Coordinate> &);

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

		Vertex (MFVec3f* point, const MFInt32::const_iterator & index, const int i) :
			index (index),
			i (i)
		{
			float x, y, z;

			point -> at (*index) .getValue (x, y, z);
			location .reserve (3);
			location .push_back (x);
			location .push_back (y);
			location .push_back (z);
		}

		std::vector <GLdouble>  location;
		MFInt32::const_iterator index;
		int                     i;
	};

	class PolygonElement
	{
	public:

		PolygonElement (GLenum type) :
			type (type)
		{ }

		GLenum               type;
		std::deque <Vertex*> vertices;
	};

	typedef std::deque <PolygonElement> Polygon;

	GLUtesselator* tess;
	int            numPoints;
	int            numFaces;
	int            numTexCoord;
	int            numColors;
	int            numNormals;
	Box3f          bbox;

};

} // X3D
} // titania

#endif
