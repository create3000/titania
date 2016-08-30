/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 * Copyright (c) 2008 Sean Morrison <learner@brlcad.org>
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

#ifndef __TITANIA_FTGL_BBOX_H__
#define __TITANIA_FTGL_BBOX_H__

#include "Types.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_OUTLINE_H

namespace titania {
namespace FTGL {

/**
 * BBox is a convenience class for handling bounding boxes.
 */
class BBox
{
public:

	/**
	 * Default constructor. Bounding box is set to zero.
	 */
	BBox () :
		lower (0, 0, 0),
		upper (0, 0, 0)
	{ }

	/**
	 * Constructor.
	 */
	BBox (const double lx, const double ly, const double lz, const double ux, const double uy, const double uz) :
		lower (lx, ly, lz),
		upper (ux, uy, uz)
	{ }

	/**
	 * Constructor.
	 */
	BBox (const Vector3d & l, const Vector3d & u) :
		lower (l),
		upper (u)
	{ }

	/**
	 * Constructor. Extracts a bounding box from a freetype glyph. Uses
	 * the control box for the glyph. <code>FT_Glyph_Get_CBox()</code>
	 *
	 * @param glyph A freetype glyph
	 */
	BBox (FT_GlyphSlot glyph) :
		lower (0, 0, 0),
		upper (0, 0, 0)
	{
		FT_BBox bbox;

		FT_Outline_Get_CBox (&(glyph -> outline), &bbox);

		lower .x (static_cast <double> (bbox.xMin) / 64.0);
		lower .y (static_cast <double> (bbox.yMin) / 64.0);
		lower .z (0);
		upper .x (static_cast <double> (bbox.xMax) / 64.0);
		upper .y (static_cast <double> (bbox.yMax) / 64.0);
		upper .z (0);
	}

	/**
	 * Mark the bounds invalid by setting all lower dimensions greater
	 * than the upper dimensions.
	 */
	void
	invalidate ()
	{
		lower = Vector3d (1, 1, 1);
		upper = Vector3d (-1, -1, -1);
	}

	/**
	 * Determines if this bounding box is valid.
	 *
	 * @return True if all lower values are <= the corresponding
	 *         upper values.
	 */
	bool
	isValid () const
	{
		return lower .x () <= upper .x ()
		       and lower .y () <= upper .y ()
		       and lower .z () <= upper .z ();
	}

	/**
	 * Move the Bounding Box by a vector.
	 *
	 * @param vector  The vector to move the bbox in 3D space.
	 */
	BBox &
	operator += (const Vector3d & vector)
	{
		lower += vector;
		upper += vector;

		return *this;
	}

	/**
	 * Combine two bounding boxes. The result is the smallest bounding
	 * box containing the two original boxes.
	 *
	 * @param bbox  The bounding box to merge with the second one.
	 */
	BBox &
	operator |= (const BBox & bbox)
	{
		if (bbox .lower .x () < lower .x ()) lower .x (bbox .lower.x ());

		if (bbox .lower .y () < lower .y ()) lower .y (bbox .lower.y ());

		if (bbox .lower .z () < lower .z ()) lower .z (bbox .lower.z ());

		if (bbox .upper .x () > upper .x ()) upper .x (bbox .upper.x ());

		if (bbox .upper .y () > upper .y ()) upper .y (bbox .upper.y ());

		if (bbox .upper .z () > upper .z ()) upper .z (bbox .upper.z ());

		return *this;
	}

	void
	setDepth (const double depth)
	{
		if (depth > 0)
			upper .z (lower.z () + depth);
		else
			lower .z (upper.z () + depth);
	}

	const Vector3d &
	getUpper () const
	{ return upper; }

	const Vector3d &
	getLower () const
	{ return lower; }

	/**
	 * Destructor
	 */
	~BBox ()
	{ }


private:

	/**
	 * The bounds of the box
	 */
	Vector3d lower;
	Vector3d upper;

};

} // FTGL
} // titania

#endif
