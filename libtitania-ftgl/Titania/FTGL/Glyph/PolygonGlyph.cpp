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

#include "config.h"

#include "../ftgl.h"

#include "../Internals.h"
#include "../Vectoriser.h"
#include "PolygonGlyphImpl.h"

namespace titania {
namespace FTGL {

//
//  FTGLPolyGlyph
//

PolygonGlyph::PolygonGlyph (FT_GlyphSlot glyph, float outset,
                                bool useDisplayList) :
	Glyph (new PolygonGlyphImpl (glyph, outset, useDisplayList))
{ }

PolygonGlyph::~PolygonGlyph ()
{ }

const Point &
PolygonGlyph::Render (const Point & pen, int renderMode)
{
	PolygonGlyphImpl* myimpl = dynamic_cast <PolygonGlyphImpl*> (impl);

	return myimpl -> RenderImpl (pen, renderMode);
}

//
//  FTGLPolyGlyphImpl
//

PolygonGlyphImpl::PolygonGlyphImpl (FT_GlyphSlot glyph, float _outset,
                                        bool useDisplayList) :
	GlyphImpl (glyph),
	     glList (0)
{
	if (ft_glyph_format_outline not_eq glyph -> format)
	{
		err = 0x14; // Invalid_Outline
		return;
	}

	vectoriser = new Vectoriser (glyph);

	if ((vectoriser -> getContourCount () < 1) or (vectoriser -> getPointCount () < 3))
	{
		delete vectoriser;
		vectoriser = NULL;
		return;
	}

	hscale = glyph -> face -> size -> metrics .x_ppem * 64;
	vscale = glyph -> face -> size -> metrics .y_ppem * 64;
	outset = _outset;

	if (useDisplayList)
	{
		glList = glGenLists (1);
		glNewList (glList, GL_COMPILE);

		DoRender ();

		glEndList ();

		delete vectoriser;
		vectoriser = NULL;
	}
}

PolygonGlyphImpl::~PolygonGlyphImpl ()
{
	if (glList)
	{
		glDeleteLists (glList, 1);
	}
	else if (vectoriser)
	{
		delete vectoriser;
	}
}

const Point &
PolygonGlyphImpl::RenderImpl (const Point & pen,
                                int renderMode)
{
	glTranslatef (pen.Xf (), pen .Yf (), pen .Zf ());

	if (glList)
	{
		glCallList (glList);
	}
	else if (vectoriser)
	{
		DoRender ();
	}

	glTranslatef (-pen.Xf (), -pen.Yf (), -pen.Zf ());

	return advance;
}

void
PolygonGlyphImpl::DoRender ()
{
	vectoriser -> makeMesh (1, 1, outset);

	const FTMesh* mesh = vectoriser -> getMesh ();

	for (unsigned int t = 0; t < mesh -> TesselationCount (); ++ t)
	{
		const FTTesselation* subMesh     = mesh -> Tesselation (t);
		unsigned int         polygonType = subMesh -> PolygonType ();

		glBegin (polygonType);

		for (unsigned int i = 0; i < subMesh -> PointCount (); ++ i)
		{
			Point point = subMesh -> getPoint (i);
			glTexCoord2f (point.Xf () / hscale, point .Yf () / vscale);
			glVertex3f (point.Xf () / 64.0f, point .Yf () / 64.0f, 0);
		}

		glEnd ();
	}
}

} // FTGL
} // titania
