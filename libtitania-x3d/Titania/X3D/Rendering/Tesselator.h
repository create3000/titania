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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_TESSELATOR_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_TESSELATOR_H__

namespace titania {
namespace X3D {

class TesselatorVertex
{
public:

	TesselatorVertex (const Vector3f & point, const int i) :
		point (point),
		i (i)
	{ }

	Vector3d point;
	size_t   i;

};

class PolygonElements
{
public:

	PolygonElements (GLenum type) :
		type (type)
	{ }

	GLenum                         type;
	std::deque <TesselatorVertex*> vertices;

};

typedef std::deque <PolygonElements> TesselatedPolygon;

class Tesselator
{
public:

	Tesselator ();

	void
	addVertex (const Vector3f &, const int);

	void
	tesselate ();

	const TesselatedPolygon &
	getPolygon () { return tesselatedPolygon; }

	~Tesselator ();


private:

	static void tessBeginData (GLenum, void*);

	static void
	tessVertexData (void*, void*);

	static void tessCombineData (GLdouble [3], void* [4], GLfloat [4], void**, void*);

	static void
	tessEndData (void*);

	static void tessError (GLenum);

	GLUtesselator*                tesselator;
	std::deque <TesselatorVertex> vertices;
	TesselatedPolygon             tesselatedPolygon;

};

Tesselator::Tesselator ()
{
	tesselator = gluNewTess ();

	if (tesselator)
	{
		gluTessProperty (tesselator, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
		gluTessCallback (tesselator, GLU_TESS_BEGIN_DATA, _GLUfuncptr (&Tesselator::tessBeginData));
		gluTessCallback (tesselator, GLU_TESS_VERTEX_DATA, _GLUfuncptr (&Tesselator::tessVertexData));

		//gluTessCallback(tesselator, GLU_TESS_COMBINE_DATA, (_GLUfuncptr)&IndexedFaceSet::tessCombineData);
		gluTessCallback (tesselator, GLU_TESS_END_DATA, _GLUfuncptr (&Tesselator::tessEndData));
		gluTessCallback (tesselator, GLU_TESS_ERROR, _GLUfuncptr (&Tesselator::tessError));
	}
}

void
Tesselator::addVertex (const Vector3f & point, const int i)
{
	vertices .emplace_back (point, i);
}

void
Tesselator::tesselate ()
{
	gluTessBeginPolygon (tesselator, &tesselatedPolygon);
	gluTessBeginContour (tesselator);

	for (auto & vertex : vertices)
		gluTessVertex (tesselator, vertex .point .data (), &vertex);

	//gluTessEndContour(tess);
	gluEndPolygon (tesselator);
}

void
Tesselator::tessBeginData (GLenum type, void* polygon_data)
{
	TesselatedPolygon* polygon = (TesselatedPolygon*) polygon_data;

	polygon -> emplace_back (type);
}

void
Tesselator::tessVertexData (void* vertex_data, void* polygon_data)
{
	TesselatedPolygon* polygon = (TesselatedPolygon*) polygon_data;
	TesselatorVertex*  vertex  = (TesselatorVertex*) vertex_data;

	polygon -> back () .vertices .emplace_back (vertex);
}

void
Tesselator::tessCombineData (GLdouble coords [3], void* vertex_data [4],
                             GLfloat weight [4], void** outData,
                             void* polygon_data)
{
	// Not used yet
}

void
Tesselator::tessEndData (void* polygon_data)
{ }

void
Tesselator::tessError (GLenum err_no)
{
	std::clog << "Warning: tesselation error: '" << (char*) gluErrorString (err_no) << "'." << std::endl;
}

Tesselator::~Tesselator ()
{
	if (tesselator)
		gluDeleteTess (tesselator);
}

} // X3D
} // titania

#endif
