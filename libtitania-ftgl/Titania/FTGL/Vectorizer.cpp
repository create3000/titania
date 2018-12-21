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

namespace titania {
namespace FTGL {

Vectorizer::Vectorizer (const FT_GlyphSlot glyph, const size_t bezierSteps) :
	   contourList (),
	ftContourCount (0),
	   contourFlag (0),
	       outline ()
{
	if (glyph)
	{
		outline = glyph -> outline;

		ftContourCount = outline .n_contours;
		contourFlag    = outline .flags;

		processContours (bezierSteps);
	}
}

size_t
Vectorizer::getPointCount () const
{
	size_t s = 0;

	for (size_t c = 0; c < getContourCount (); ++ c)
	{
		s += contourList [c] -> getPointCount ();
	}

	return s;
}

std::shared_ptr <Contour>
Vectorizer::getContour (size_t index) const
{
	return (index < getContourCount ()) ? contourList [index] : nullptr;
}

void
Vectorizer::processContours (const size_t bezierSteps)
{
	short contourLength = 0;
	short startIndex    = 0;
	short endIndex      = 0;

	contourList .resize (ftContourCount);

	for (int32_t i = 0; i < ftContourCount; ++ i)
	{
		FT_Vector* pointList = &outline .points [startIndex];
		char*      tagList   = &outline .tags [startIndex];

		endIndex      = outline .contours [i];
		contourLength = (endIndex - startIndex) + 1;

		contourList [i] = std::make_shared <Contour> (pointList, tagList, contourLength, bezierSteps);

		startIndex = endIndex + 1;
	}

	// Compute each contour's parity. FIXME: see if FT_Outline_Get_Orientation
	// can do it for us.
	for (int32_t i = 0; i < ftContourCount; i ++)
	{
		const auto c1 = contourList [i];

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

			const auto c2 = contourList [j];

			for (size_t n = 0; n < c2 -> getPointCount (); n ++)
			{
				Vector3d p1 = c2 -> getPoint (n);
				Vector3d p2 = c2 -> getPoint ((n + 1) % c2 -> getPointCount ());

				/* FIXME: combinations of >= > <= and < do not seem stable */
				if ((p1.y () < leftmost .y () and p2 .y () < leftmost .y ())
				    or (p1.y () >= leftmost .y () and p2 .y () >= leftmost .y ())
				    or (p1.x () > leftmost .x () and p2 .x () > leftmost .x ()))
				{
					continue;
				}
				else if (p1.x () < leftmost .x () and p2 .x () < leftmost .x ())
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

void
Vectorizer::triangulate (const double zNormal,
                         const int32_t outsetType,
                         const double outsetSize,
                         std::vector <size_t> & indices,
                         std::vector <Vector3d> & points)
{
	using namespace std::placeholders;

	if (getContourCount () < 1)
		return;

	if (getPointCount () < 3)
		return;

	Tesselator tessellator;

	tessellator .combine (std::bind (&Vectorizer::combine, this, std::ref (points), _1));

	if (contourFlag & ft_outline_even_odd_fill) // ft_outline_reverse_fill
	{
		tessellator .property (GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
	}
	else
	{
		tessellator .property (GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	}

	tessellator .property (GLU_TESS_TOLERANCE, 0);
	tessellator .normal (math::vector3 <double> (0, 0, zNormal));
	tessellator .begin_polygon ();

	for (size_t c = 0; c < getContourCount (); ++ c)
	{
		const auto contour = contourList [c];

		// Build the outsetType
		switch (outsetType)
		{
			case 1: contour -> buildFrontOutset (outsetSize); break;
			case 2: contour -> buildBackOutset  (outsetSize); break;
		}

		tessellator .begin_contour ();

		for (size_t p = 0; p < contour -> getPointCount (); ++ p)
		{
			const auto index = points .size ();

			switch (outsetType)
			{
				case 1:          points .emplace_back (contour -> getFrontPoint (p) / 64.0); break;
				case 2:          points .emplace_back (contour -> getBackPoint  (p) / 64.0); break;
				case 0: default: points .emplace_back (contour -> getPoint      (p) / 64.0); break;
			}

			tessellator .add_vertex (points .back (), index);
		}

		tessellator .end_contour ();
	}

	tessellator .end_polygon ();

	for (const auto & vertex : tessellator .triangles ())
		indices .emplace_back (std::get <0> (vertex .data ()));
}

Vectorizer::Tesselator::data_type
Vectorizer::combine (std::vector <Vector3d> & points, const Vector3d & coord) const
{
	const auto index = points .size ();

	points .emplace_back (coord);

	return Tesselator::data_type (index);
}

Vectorizer::~Vectorizer ()
{ }

} // FTGL
} // titania
