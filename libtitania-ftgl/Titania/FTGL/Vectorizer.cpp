/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "Vectorizer.h"

#include <GL/glu.h>

namespace titania {
namespace FTGL {

using GLUTesselatorFunction = GLvoid (*)();

static
void
ftglError (GLenum errCode, Mesh* mesh)
{
	mesh -> setError (errCode);
}

static
void
ftglVertex (void* data, Mesh* mesh)
{
	double* vertex = static_cast <double*> (data);

	mesh -> addPoint (vertex [0], vertex [1], vertex [2]);
}

static
void
ftglCombine (double coords [3], void* vertex_data [4], GLfloat weight [4], void** outData, Mesh* mesh)
{
	const double* vertex = static_cast <const double*> (coords);

	*outData = const_cast <double*> (mesh -> combine (vertex [0], vertex [1], vertex [2]));
}

static
void
ftglBegin (GLenum type, Mesh* mesh)
{
	mesh -> begin (type);
}

static
void
ftglEnd (Mesh* mesh)
{
	mesh -> end ();
}

Mesh::Mesh () :
	currentTesselation (0),
	               err (0)
{
	tesselationList .reserve (16);
}

Mesh::~Mesh ()
{
	for (size_t t = 0; t < tesselationList .size (); ++ t)
	{
		delete tesselationList [t];
	}

	tesselationList .clear ();
}

void
Mesh::addPoint (const double x, const double y, const double z)
{
	currentTesselation -> AddPoint (x, y, z);
}

const double*
Mesh::combine (const double x, const double y, const double z)
{
	tempPointList .emplace_back (x, y, z);
	return tempPointList .back () .data ();
}

void
Mesh::begin (GLenum meshType)
{
	currentTesselation = new Tesselation (meshType);
}

void
Mesh::end ()
{
	tesselationList .emplace_back (currentTesselation);
}

const Tesselation* const
Mesh::getTesselation (size_t index) const
{
	return (index < tesselationList .size ()) ? tesselationList [index] : nullptr;
}

Vectorizer::Vectorizer (const FT_GlyphSlot glyph) :
	   contourList (0),
	          mesh (0),
	ftContourCount (0),
	   contourFlag (0)
{
	if (glyph)
	{
		outline = glyph -> outline;

		ftContourCount = outline .n_contours;
		contourList    = 0;
		contourFlag    = outline .flags;

		processContours ();
	}
}

Vectorizer::~Vectorizer ()
{
	for (size_t c = 0; c < getContourCount (); ++ c)
	{
		delete contourList [c];
	}

	delete [ ] contourList;
	delete mesh;
}

void
Vectorizer::processContours ()
{
	short contourLength = 0;
	short startIndex    = 0;
	short endIndex      = 0;

	contourList = new Contour* [ftContourCount];

	for (int32_t i = 0; i < ftContourCount; ++ i)
	{
		FT_Vector* pointList = &outline.points [startIndex];
		char*      tagList   = &outline.tags [startIndex];

		endIndex      = outline .contours [i];
		contourLength =  (endIndex - startIndex) + 1;

		Contour* contour = new Contour (pointList, tagList, contourLength);

		contourList [i] = contour;

		startIndex = endIndex + 1;
	}

	// Compute each contour's parity. FIXME: see if FT_Outline_Get_Orientation
	// can do it for us.
	for (int32_t i = 0; i < ftContourCount; i ++)
	{
		Contour* c1 = contourList [i];

		// 1. Find the leftmost point.
		Vector3d leftmost (65536, 0, 0);

		for (size_t n = 0; n < c1 -> getPointCount (); n ++)
		{
			Vector3d p = c1 -> getPoint (n);

			if (p.x () < leftmost .x ())
			{
				leftmost = p;
			}
		}

		// 2. Count how many other contours we cross when going further to
		// the left.
		int32_t parity = 0;

		for (int32_t j = 0; j < ftContourCount; j ++)
		{
			if (j == i)
			{
				continue;
			}

			Contour* c2 = contourList [j];

			for (size_t n = 0; n < c2 -> getPointCount (); n ++)
			{
				Vector3d p1 = c2 -> getPoint (n);
				Vector3d p2 = c2 -> getPoint ((n + 1) % c2 -> getPointCount ());

				/* FIXME: combinations of >= > <= and < do not seem stable */
				if ((p1.y () < leftmost .y () && p2 .y () < leftmost .y ())
				    or (p1.y () >= leftmost .y () && p2 .y () >= leftmost .y ())
				    or (p1.x () > leftmost .x () && p2 .x () > leftmost .x ()))
				{
					continue;
				}
				else if (p1.x () < leftmost .x () && p2 .x () < leftmost .x ())
				{
					parity ++;
				}
				else
				{
					Vector3d a = p1 - leftmost;
					Vector3d b = p2 - leftmost;

					if (b.x () * a .y () > b .y () * a .x ())
					{
						parity ++;
					}
				}
			}
		}

		// 3. Make sure the glyph has the proper parity.
		c1 -> setParity (parity);
	}
}

size_t
Vectorizer::getPointCount ()
{
	size_t s = 0;

	for (size_t c = 0; c < getContourCount (); ++ c)
	{
		s += contourList [c] -> getPointCount ();
	}

	return s;
}

const Contour* const
Vectorizer::getContour (size_t index) const
{
	return (index < getContourCount ()) ? contourList [index] : nullptr;
}

void
Vectorizer::makeMesh (double zNormal, int32_t outsetType, double outsetSize)
{
	if (mesh)
	{
		delete mesh;
	}

	mesh = new Mesh;

	GLUtesselator* tobj = gluNewTess ();

	gluTessCallback (tobj, GLU_TESS_BEGIN_DATA,     (GLUTesselatorFunction) ftglBegin);
	gluTessCallback (tobj, GLU_TESS_VERTEX_DATA,    (GLUTesselatorFunction) ftglVertex);
	gluTessCallback (tobj, GLU_TESS_COMBINE_DATA,   (GLUTesselatorFunction) ftglCombine);
	gluTessCallback (tobj, GLU_TESS_END_DATA,       (GLUTesselatorFunction) ftglEnd);
	gluTessCallback (tobj, GLU_TESS_ERROR_DATA,     (GLUTesselatorFunction) ftglError);

	if (contourFlag & ft_outline_even_odd_fill) // ft_outline_reverse_fill
	{
		gluTessProperty (tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	}
	else
	{
		gluTessProperty (tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	}

	gluTessProperty (tobj, GLU_TESS_TOLERANCE, 0);
	gluTessNormal (tobj, 0, 0, zNormal);
	gluTessBeginPolygon (tobj, mesh);

	for (size_t c = 0; c < getContourCount (); ++ c)
	{
		/* Build the */
		switch (outsetType)
		{
			case 1: contourList [c] -> buildFrontOutset (outsetSize); break;
			case 2: contourList [c] -> buildBackOutset  (outsetSize); break;
		}

		const Contour* contour = contourList [c];

		gluTessBeginContour (tobj);

		for (size_t p = 0; p < contour -> getPointCount (); ++ p)
		{
			const double* d;

			switch (outsetType)
			{
				case 1: d          = contour -> getFrontPoint (p) .data (); break;
				case 2: d          = contour -> getBackPoint  (p) .data (); break;
				case 0: default: d = contour -> getPoint      (p) .data (); break;
			}

			// XXX: gluTessVertex doesn't modify the data but does not
			// specify "const" in its prototype, so we cannot cast to
			// a const type.
			gluTessVertex (tobj, (GLdouble*) d, (GLvoid*) d);
		}

		gluTessEndContour (tobj);
	}

	gluTessEndPolygon (tobj);

	gluDeleteTess (tobj);
}

} // FTGL
} // titania
