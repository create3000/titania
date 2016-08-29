/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
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

#include "Contour.h"

#include <cmath>

namespace titania {
namespace FTGL {

static constexpr uint32_t BEZIER_STEPS = 5;

void
Contour::addPoint (Vector3d point)
{
	if (pointList.empty () or (point not_eq pointList [pointList.size () - 1]
	                           and point not_eq pointList [0]))
	{
		pointList .emplace_back (point);
	}
}

void
Contour::addOutsetPoint (Vector3d point)
{
	outsetPointList .emplace_back (point);
}

void
Contour::addFrontPoint (Vector3d point)
{
	frontPointList .emplace_back (point);
}

void
Contour::addBackPoint (Vector3d point)
{
	backPointList .emplace_back (point);
}

void
Contour::evaluateQuadraticCurve (Vector3d A, Vector3d B, Vector3d C)
{
	for (uint32_t i = 1; i < BEZIER_STEPS; i ++)
	{
		double t = static_cast <double> (i) / BEZIER_STEPS;

		Vector3d U = (1 - t) * A + t * B;
		Vector3d V = (1 - t) * B + t * C;

		addPoint ((1 - t) * U + t * V);
	}
}

void
Contour::evaluateCubicCurve (Vector3d A, Vector3d B, Vector3d C, Vector3d D)
{
	for (uint32_t i = 0; i < BEZIER_STEPS; i ++)
	{
		double t = static_cast <double> (i) / BEZIER_STEPS;

		Vector3d U = (1 - t) * A + t * B;
		Vector3d V = (1 - t) * B + t * C;
		Vector3d W = (1 - t) * C + t * D;

		Vector3d M = (1 - t) * U + t * V;
		Vector3d N = (1 - t) * V + t * W;

		addPoint ((1 - t) * M + t * N);
	}
}

// This function is a bit tricky. Given a path ABC, it returns the
// coordinates of the outset point facing B on the left at a distance
// of 64.0.
//                                         M
//                            - - - - - - X
//                             ^         / '
//                             | 64.0   /   '
//  X---->-----X     ==>    X--v-------X     '
// A          B \          A          B \   .>'
//               \                       \<'  64.0
//                \                       \                  .
//                 \                       \                 .
//                C X                     C X
//
Vector3d
Contour::computeOutsetPoint (Vector3d A, Vector3d B, Vector3d C)
{
	/* Build the rotation matrix from 'ba' vector */
	Vector3d ba = normalize (A - B);
	Vector3d bc = C - B;

	/* Rotate bc to the left */
	Vector3d tmp (bc .x () * -ba .x () + bc .y () * -ba.y (),
	              bc .x () * ba .y () + bc .y () * -ba.x (),
	              0);

	/* Compute the vector bisecting 'abc' */
	double norm = std::sqrt (tmp.x () * tmp .x () + tmp .y () * tmp .y ());
	double dist = 64.0 * std::sqrt ((norm - tmp .x ()) / (norm + tmp .x ()));

	tmp .x (tmp .y () < 0 ? dist : -dist);
	tmp .y (64.0);

	/* Rotate the new bc to the right */
	return Vector3d (tmp .x () * -ba.x () + tmp .y () * ba .y (),
	                 tmp .x () * -ba.y () + tmp .y () * -ba .x (),
	                 0);
}

void
Contour::setParity (int32_t parity)
{
	size_t   size = getPointCount ();
	Vector3d vOutset;

	if (((parity & 1) and clockwise) or (! (parity & 1) and ! clockwise))
	{
		// Contour orientation is wrong! We must reverse all points.
		// FIXME: could it be worth writing Vector::reverse() for this?
		for (size_t i = 0; i < size / 2; i ++)
		{
			Vector3d tmp = pointList [i];
			pointList [i]            = pointList [size - 1 - i];
			pointList [size - 1 - i] = tmp;
		}

		clockwise = ! clockwise;
	}

	for (size_t i = 0; i < size; i ++)
	{
		size_t prev, cur, next;

		prev = (i + size - 1) % size;
		cur  = i;
		next = (i + size + 1) % size;

		vOutset = computeOutsetPoint (getPoint (prev), getPoint (cur), getPoint (next));
		addOutsetPoint (vOutset);
	}
}

Contour::Contour (FT_Vector* contour, char* tags, uint32_t n)
{
	uint32_t c = (n - 1) % n;

	Vector3d prev;
	Vector3d cur (contour [c] .x, contour [c] .y, 0);
	Vector3d next (contour [0] .x, contour [0] .y, 0);
	Vector3d a;
	double   olddir;
	double   dir   = atan2 ((next - cur) .y (), (next - cur) .x ());
	double   angle = 0.0;

	// See http://freetype.sourceforge.net/freetype2/docs/glyphs/glyphs-6.html
	// for a full description of FreeType tags.
	for (uint32_t i = 0; i < n; i ++)
	{
		uint32_t i1 = (i + 1) % n;

		prev   = cur;
		cur    = next;
		next   = Vector3d (contour [i1] .x, contour [i1] .y, 0);
		olddir = dir;
		dir    = atan2 ((next - cur) .y (), (next - cur) .x ());

		// Compute our path's new direction.
		double t = dir - olddir;

		if (t < -M_PI)
			t += 2 * M_PI;

		if (t > M_PI)
			t -= 2 * M_PI;

		angle += t;

		// Only process point tags we know.
		if (n < 2 or FT_CURVE_TAG (tags [i]) == FT_Curve_Tag_On)
		{
			addPoint (cur);
		}
		else if (FT_CURVE_TAG (tags [i]) == FT_Curve_Tag_Conic)
		{
			Vector3d prev2 = prev, next2 = next;

			// Previous point is either the real previous point (an "on"
			// point), or the midpoint between the current one and the
			// previous "conic off" point.
			if (FT_CURVE_TAG (tags [(i - 1 + n) % n]) == FT_Curve_Tag_Conic)
			{
				prev2 = (cur + prev) * 0.5;
				addPoint (prev2);
			}

			// Next point is either the real next point or the midpoint.
			if (FT_CURVE_TAG (tags [(i + 1) % n]) == FT_Curve_Tag_Conic)
			{
				next2 = (cur + next) * 0.5;
			}

			evaluateQuadraticCurve (prev2, cur, next2);
		}
		else if (FT_CURVE_TAG (tags [i]) == FT_Curve_Tag_Cubic
		         and FT_CURVE_TAG (tags [(i + 1) % n]) == FT_Curve_Tag_Cubic)
		{
			uint32_t i2 = (i + 2) % n;

			evaluateCubicCurve (prev, cur, next,
			                    Vector3d (contour [i2] .x, contour [i2] .y, 0));
		}
	}

	// If final angle is positive (+2PI), it's an anti-clockwise contour,
	// otherwise (-2PI) it's clockwise.
	clockwise = (angle < 0.0);
}

void
Contour::buildFrontOutset (double outset)
{
	for (size_t i = 0; i < getPointCount (); ++ i)
	{
		addFrontPoint (getPoint (i) + getOutset (i) * outset);
	}
}

void
Contour::buildBackOutset (double outset)
{
	for (size_t i = 0; i < getPointCount (); ++ i)
	{
		addBackPoint (getPoint (i) + getOutset (i) * outset);
	}
}

} // FTGL
} // titania
