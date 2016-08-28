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

class GlyphImpl;

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
protected:

	/**
	 * Create a glyph.
	 *
	 * @param glyph  The Freetype glyph to be processed
	 */
	Glyph (FT_GlyphSlot glyph);


private:

	/**
	 * Internal FTGL Glyph constructor. For private use only.
	 *
	 * @param pImpl  Internal implementation object. Will be destroyed
	 *               upon Glyph deletion.
	 */
	Glyph (GlyphImpl* pImpl);

	/* Allow our internal subclasses to access the private constructor */
	friend class FTBitmapGlyph;
	friend class FTBufferGlyph;
	friend class FTExtrudeGlyph;
	friend class FTOutlineGlyph;
	friend class FTPixmapGlyph;
	friend class PolygonGlyph;
	friend class FTTextureGlyph;


public:

	/**
	 * Destructor
	 */
	virtual
	~Glyph ();

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

	/**
	 * Return the advance width for this glyph.
	 *
	 * @return  advance width.
	 */
	virtual
	double
	getAdvance () const;

	/**
	 * Return the bounding box for this glyph.
	 *
	 * @return  bounding box.
	 */
	virtual
	const BBox &
	getBBox () const;

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	virtual
	FT_Error
	getError () const;


private:

	/**
	 * Internal FTGL Glyph implementation object. For private use only.
	 */
	GlyphImpl* impl;

};

} // FTGL
} // titania

#endif
