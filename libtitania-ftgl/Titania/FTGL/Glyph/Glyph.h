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

#ifndef __TITANIA_FTGL_GLYPH_GLYPH_H__
#define __TITANIA_FTGL_GLYPH_GLYPH_H__

#include "../BBox.h"
#include "../Types.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

namespace titania {
namespace FTGL {

/**
 * Glyph is the base class for FTGL glyphs.
 *
 * It provides the interface between Freetype glyphs and their openGL
 * renderable counterparts. This is an abstract class and derived classes
 * must implement the <code>Render</code> function.
 *
 * @see BBox
 * @see Point
 */
class Glyph
{
public:

	///  @name Member access

	/**
	 * Return the advance width for this glyph.
	 *
	 * @return  advance width.
	 */
	const Vector3d &
	getAdvance () const;

	/**
	 * Return the bounding box for this glyph.
	 *
	 * @return  bounding box.
	 */
	const BBox &
	getBBox () const;

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error
	getError () const;

	///  @name Operations

	/**
	 * Renders this glyph at the current pen position.
	 *
	 * @param pen  The current pen position.
	 * @param renderMode  Render mode to display
	 * @return  The advance distance for this glyph.
	 */
	virtual
	const Vector3d &
	render (const Vector3d & pen, FTGL::RenderMode renderMode) = 0;

	///  @name Destruction

	/**
	 * Destructor
	 */
	virtual
	~Glyph ();


protected:

	///  @name Construction

	/**
	 * Create a glyph.
	 *
	 * @param glyph  The Freetype glyph to be processed
	 */
	Glyph (FT_GlyphSlot glyph);

	///  @name Member access

	/**
	 * Set the error state.
	 */
	void
	setError (FT_Error);


private:

	///  @name Members

	/**
	 * The advance distance for this glyph
	 */
	Vector3d advance;

	/**
	 * The bounding box of this glyph.
	 */
	BBox bbox;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error error;

};

} // FTGL
} // titania

#endif
