/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
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

#ifndef __TITANIA_FTGL_VECTORIZER_H__
#define __TITANIA_FTGL_VECTORIZER_H__

#include "Contour.h"

#include <Titania/Math/Mesh/Tessellator.h>

#include <list>
#include <memory>
#include <vector>

namespace titania {
namespace FTGL {

/**
 * Vectorizer class is a helper class that converts font outlines into
 * point data.
 *
 * @see FTExtrudeGlyph
 * @see FTOutlineGlyph
 * @see PolygonGlyph
 * @see Contour
 * @see Vector3d
 *
 */
class Vectorizer
{
public:

	///  @name Construction

	/**
	 * Constructor
	 *
	 * @param glyph         The freetype glyph to be processed.
	 * @param bezierSteps   Number of bezier steps. A good value is 5.
	 */
	Vectorizer (const FT_GlyphSlot glyph, const size_t bezierSteps);

	///  @name Operations

	/**
	 * Build an Mesh from the vector outline data.
	 *
	 * @param zNormal  The direction of the z axis of the normal
	 *                 for this mesh
	 * FIXME: change the following for a constant
	 * @param outsetType  Specify the outset type contour
	 *  0 : Original
	 *  1 : Front
	 *  2 : Back
	 * @param outsetSize  Specify the outset size contour
	 * @param indices     Specify the output array of indices of the mesh
	 * @param points      Specify the output array of points of the mesh
	 */
	void
	triangulate (const double zNormal,
	             const int32_t outsetType,
	             const double outsetSize,
	             std::vector <size_t> & indices,
	             std::vector <Vector3d> & points);

	///  @name Destruction

	/**
	 *  Destructor
	 */
	virtual
	~Vectorizer ();


private:

	using Tesselator = math::tessellator <double, size_t>;

	///  @name Member access

	/**
	 * Get the total count of points in this outline
	 *
	 * @return the number of points
	 */
	size_t
	getPointCount () const;

	/**
	 * Get the count of contours in this outline
	 *
	 * @return the number of contours
	 */
	size_t
	getContourCount () const
	{ return ftContourCount; }

	/**
	 * Return a contour at index
	 *
	 * @return the number of contours
	 */
	std::shared_ptr <Contour>
	getContour (size_t index) const;

	/**
	 * Get the number of points in a specific contour in this outline
	 *
	 * @param c     The contour index
	 * @return      the number of points in contour[c]
	 */
	size_t
	getContourSize (int32_t c) const
	{ return contourList [c] -> getPointCount (); }

	/**
	 * Get the flag for the tesselation rule for this outline
	 *
	 * @return The contour flag
	 */
	int32_t
	getContourFlag () const
	{ return contourFlag; }

	///  @name Operations

	/**
	 * Process the freetype outline data into contours of points
	 *
	 * @param bezierSteps   number of bezier steps
	 */
	void
	processContours (const size_t bezierSteps);

	// Combine function for tesselator
	Tesselator::data_type
	combine (std::vector <Vector3d> & points, const Vector3d & coord) const;

	///  @name Members

	/**
	 * The list of contours in the glyph
	 */
	std::vector <std::shared_ptr <Contour>> contourList;

	/**
	 * The number of contours reported by Freetype
	 */
	short ftContourCount;

	/**
	 * A flag indicating the tesselation rule for the glyph
	 */
	int32_t contourFlag;

	/**
	 * A Freetype outline
	 */
	FT_Outline outline;

};

} // FTGL
} // titania

#endif
