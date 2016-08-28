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

#ifndef __TITANIA_FTGL_GLYPH_POLYGON_GLYPH_H__
#define __TITANIA_FTGL_GLYPH_POLYGON_GLYPH_H__

#include "Glyph.h"

#include <GL/gl.h>

namespace titania {
namespace FTGL {

class Vectorizer;

/**
 * PolygonGlyph is a specialisation of Glyph for creating tessellated
 * polygon glyphs.
 */
class PolygonGlyph :
	public Glyph
{
public:

	///  @name Construction

	/**
	 * Constructor. Sets the Error to Invalid_Outline if the glyphs
	 * isn't an outline.
	 *
	 * @param glyph The Freetype glyph to be processed
	 * @param outset  The outset distance
	 * @param useDisplayList Enable or disable the use of Display Lists
	 *                       for this glyph
	 *                       <code>true</code> turns ON display lists.
	 *                       <code>false</code> turns OFF display lists.
	 */
	PolygonGlyph (FT_GlyphSlot glyph, double outset, bool useDisplayList);

	///  @name Operations

	/**
	 * Render this glyph at the current pen position.
	 *
	 * @param pen  The current pen position.
	 * @param renderMode  Render mode to display
	 * @return  The advance distance for this glyph.
	 */
	virtual
	const Vector3d &
	render (const Vector3d & pen, FTGL::RenderMode renderMode) final override;

	///  @name Destruction

	/**
	 * Destructor
	 */
	virtual
	~PolygonGlyph ();

private:

	///  @name Operations

	/**
	 * Private rendering method.
	 */
	void
	doRender ();

	///  @name Members

	/**
	 * Private rendering variables.
	 */
	uint32_t    hscale;
	uint32_t    vscale;
	Vectorizer* vectoriser;
	double      outset;

	/**
	 * OpenGL display list
	 */
	GLuint glList;

};

} // FTGL
} // titania

#endif
