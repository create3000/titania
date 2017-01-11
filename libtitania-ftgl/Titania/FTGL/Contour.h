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

#ifndef __TITANIA_FTGL_CONTOUR_H__
#define __TITANIA_FTGL_CONTOUR_H__

#include "Types.h"

#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace titania {
namespace FTGL {

/**
 * Contour class is a container of points that describe a vector font
 * outline. It is used as a container for the output of the bezier curve
 * evaluator in Vectorizer.
 *
 * @see FTOutlineGlyph
 * @see PolygonGlyph
 * @see Point
 */
class Contour
{
public:

	/**
	 * Constructor
	 *
	 * @param contour
	 * @param pointTags
	 * @param numberOfPoints
	 */
	Contour (FT_Vector* const contour, char* const pointTags, const size_t numberOfPoints, const size_t bezierSteps);

	/**
	 * Return a point at index.
	 *
	 * @param index of the point in the curve.
	 * @return const point reference
	 */
	const Vector3d &
	getPoint (size_t index) const
	{ return pointList [index]; }

	/**
	 * Return a point at index.
	 *
	 * @param index of the point in the outset curve.
	 * @return const point reference
	 */
	const Vector3d &
	getOutset (size_t index) const
	{ return outsetPointList [index]; }

	/**
	 * Return a point at index of the front outset contour.
	 *
	 * @param index of the point in the curve.
	 * @return const point reference
	 */
	const Vector3d &
	getFrontPoint (size_t index) const
	{
		if (frontPointList.size () == 0)
			return getPoint (index);

		return frontPointList [index];
	}

	/**
	 * Return a point at index of the back outset contour.
	 *
	 * @param index of the point in the curve.
	 * @return const point reference
	 */
	const Vector3d &
	getBackPoint (size_t index) const
	{
		if (backPointList.size () == 0)
			return getPoint (index);

		return backPointList [index];
	}

	/**
	 * How many points define this contour
	 *
	 * @return the number of points in this contour
	 */
	size_t
	getPointCount () const
	{ return pointList .size (); }

	/**
	 * Make sure the glyph has the proper parity and create the front/back
	 * outset contour.
	 *
	 * @param parity  The contour's parity within the glyph.
	 */
	void
	setParity (int32_t parity);

	// FIXME: this should probably go away.
	void
	buildFrontOutset (double outset);

	void
	buildBackOutset (double outset);

	/**
	 * Destructor
	 */
	~Contour ()
	{
		pointList .clear ();
		outsetPointList .clear ();
		frontPointList .clear ();
		backPointList .clear ();
	}


private:

	/**
	 * Add a point to this contour. This function tests for duplicate
	 * points.
	 *
	 * @param point The point to be added to the contour.
	 */
	inline
	void
	addPoint (const Vector3d & point);

	/**
	 * Add a point to this contour. This function tests for duplicate
	 * points.
	 *
	 * @param point The point to be added to the contour.
	 */
	inline
	void
	addOutsetPoint (const Vector3d & point);

	/*
	 * Add a point to this outset contour. This function tests for duplicate
	 * points.
	 *
	 * @param point The point to be added to the contour outset.
	 */
	inline
	void
	addFrontPoint (const Vector3d & point);

	inline
	void
	addBackPoint (const Vector3d & point);

	/**
	 * Compute the vector norm
	 */
	inline
	double
	setNormVector (const Vector3d & v);

	/**
	 * Compute a rotation matrix from a vector
	 */
	inline
	void
	sotationMatrix (const Vector3d & a, const Vector3d & b, double* matRot, double* invRot);

	/**
	 * Matrix and vector multiplication
	 */
	inline
	void
	setMultMatrixVect (double* mat, Vector3d & v);

	/**
	 * Compute the vector bisecting from a vector 'v' and a distance 'd'
	 */
	inline
	void
	computeBisec (Vector3d & v);

	/**
	 * Compute the outset point coordinates
	 */
	inline
	Vector3d
	computeOutsetPoint (const Vector3d & a, const Vector3d & b, const Vector3d & c);

	/**
	 *  The list of points in this contour
	 */
	using PointVector = std::vector <Vector3d>;

	PointVector pointList;
	PointVector outsetPointList;
	PointVector frontPointList;
	PointVector backPointList;

	/**
	 *  Is this contour clockwise or anti-clockwise?
	 */
	bool clockwise;

};

} // FTGL
} // titania

#endif
