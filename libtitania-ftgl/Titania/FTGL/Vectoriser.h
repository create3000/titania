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

#ifndef __TITANIA_FTGL_VECTORISER_H__
#define __TITANIA_FTGL_VECTORISER_H__

#include "Contour.h"

#include <GL/gl.h>

#include <list>
#include <vector>

namespace titania {
namespace FTGL {

/**
 * Tesselation captures points that are output by OpenGL's gluTesselator.
 */
class Tesselation
{
public:

	/**
	 * Default constructor
	 */
	Tesselation (GLenum m) :
		meshType (m)
	{
		pointList .reserve (128);
	}

	/**
	 *  Destructor
	 */
	~Tesselation ()
	{
		pointList .clear ();
	}

	/**
	 * Add a point to the mesh.
	 */
	void
	AddPoint (const double x, const double y,
	          const double z)
	{
		pointList .emplace_back (x, y, z);
	}

	/**
	 * The number of points in this mesh
	 */
	size_t
	PointCount () const { return pointList .size (); }

	/**
	 *
	 */
	const Vector3d &
	getPoint (uint32_t index) const
	{ return pointList [index]; }

	/**
	 * Return the OpenGL polygon type.
	 */
	GLenum
	PolygonType () const { return meshType; }


private:

	/**
	 * Points generated by gluTesselator.
	 */
	using PointVector = std::vector <Vector3d>;

	PointVector pointList;

	/**
	 * OpenGL primitive type from gluTesselator.
	 */
	GLenum meshType;

};

/**
 * FTMesh is a container of Tesselation's that make up a polygon glyph
 */
class FTMesh
{
	using TesselationVector = std::vector <Tesselation*>;
	using PointList         = std::list <Vector3d>;


public:

	/**
	 * Default constructor
	 */
	FTMesh ();

	/**
	 *  Destructor
	 */
	~FTMesh ();

	/**
	 * Add a point to the mesh
	 */
	void
	addPoint (const double x, const double y,
	          const double z);

	/**
	 *  Create a combine point for the gluTesselator
	 */
	const double*
	combine (const double x, const double y,
	         const double z);

	/**
	 * Begin a new polygon
	 */
	void
	begin (GLenum meshType);

	/**
	 * End a polygon
	 */
	void
	end ();

	/**
	 * Record a gluTesselation error
	 */
	void
	setError (GLenum e)
	{ err = e; }

	/**
	 * The number of tesselations in the mesh
	 */
	size_t
	getTesselationCount () const
	{ return tesselationList .size (); }

	/**
	 * Get a tesselation by index
	 */
	const Tesselation* const
	getTesselation (size_t index) const;

	/**
	 * Return the temporary point list. For testing only.
	 */
	const PointList &
	getTempPointList () const
	{ return tempPointList; }

	/**
	 * Get the GL ERROR returned by the glu tesselator
	 */
	GLenum
	getError () const { return err; }


private:

	/**
	 * The current sub mesh that we are constructing.
	 */
	Tesselation* currentTesselation;

	/**
	 * Holds each sub mesh that comprises this glyph.
	 */
	TesselationVector tesselationList;

	/**
	 * Holds extra points created by gluTesselator. See ftglCombine.
	 */
	PointList tempPointList;

	/**
	 * GL ERROR returned by the glu tesselator
	 */
	GLenum err;

};

static constexpr double FTGL_FRONT_FACING = 1;
static constexpr double FTGL_BACK_FACING  = -1;

/**
 * Vectoriser class is a helper class that converts font outlines into
 * point data.
 *
 * @see FTExtrudeGlyph
 * @see FTOutlineGlyph
 * @see PolygonGlyph
 * @see Contour
 * @see Vector3d
 *
 */
class Vectoriser
{
public:

	/**
	 * Constructor
	 *
	 * @param glyph The freetype glyph to be processed
	 */
	Vectoriser (const FT_GlyphSlot glyph);

	/**
	 *  Destructor
	 */
	virtual
	~Vectoriser ();

	/**
	 * Build an FTMesh from the vector outline data.
	 *
	 * @param zNormal   The direction of the z axis of the normal
	 *                  for this mesh
	 * FIXME: change the following for a constant
	 * @param outsetType Specify the outset type contour
	 *  0 : Original
	 *  1 : Front
	 *  2 : Back
	 * @param outsetSize Specify the outset size contour
	 */
	void
	makeMesh (double zNormal = FTGL_FRONT_FACING, int32_t outsetType = 0, double outsetSize = 0);

	/**
	 * Get the current mesh.
	 */
	const FTMesh* const
	getMesh () const
	{ return mesh; }

	/**
	 * Get the total count of points in this outline
	 *
	 * @return the number of points
	 */
	size_t
	getPointCount ();

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
	const Contour* const
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


private:

	/**
	 * Process the freetype outline data into contours of points
	 *
	 * @param front front outset distance
	 * @param back back outset distance
	 */
	void
	processContours ();

	/**
	 * The list of contours in the glyph
	 */
	Contour** contourList;

	/**
	 * A Mesh for tesselations
	 */
	FTMesh* mesh;

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

#endif  //  __FTVectoriser__
