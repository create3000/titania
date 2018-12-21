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

#ifndef __TITANIA_FTGL_FONT_POLYGON_FONT_H__
#define __TITANIA_FTGL_FONT_POLYGON_FONT_H__

#include "Font.h"

namespace titania {
namespace FTGL {

/**
 * PolygonFont is a specialisation of the Font class for handling
 * tesselated Polygon Mesh fonts
 *
 * @see     Font
 */
class PolygonFont :
	public Font
{
public:

	///  @name Construction

	/**
	 * Open and read a font file. Sets Error flag.
	 *
	 * @param fontFilePath  font file path.
	 */
	PolygonFont (const std::string & fontFilePath, const size_t bezierSteps = 5);

	/**
	 * Open and read a font from a buffer in memory. Sets Error flag.
	 * The buffer is owned by the client and is NOT copied by FTGL. The
	 * pointer must be valid while using FTGL.
	 *
	 * @param pBufferBytes  the in-memory buffer
	 * @param bufferSizeInBytes  the length of the buffer in bytes
	 */
	PolygonFont (const uint8_t* pBufferBytes, const size_t bufferSizeInBytes, const size_t bezierSteps);

	///  @name Member access

	/**
	 * Set the outset distance for the font. Only implemented by
	 * FTOutlineFont, PolygonFont and FTExtrudeFont
	 *
	 * @param depth  The outset distance.
	 */
	void
	setOutset (double value)
	{ outset = value; }

	///  @name Destruction

	/**
	 * Destructor
	 */
	virtual
	~PolygonFont ();


protected:

	///  @name Operations

	/**
	 * Construct a glyph of the correct type.
	 *
	 * Clients must override the function and return their specialised
	 * Glyph.
	 *
	 * @param slot  A FreeType glyph slot.
	 * @return  An FT****Glyph or <code>null</code> on failure.
	 */
	virtual
	std::shared_ptr <Glyph>
	makeGlyph (FT_GlyphSlot slot) const final override;


private:

	///  @name Members

	/**
	 * The number of bezier steps.
	 */
	const size_t bezierSteps;

	/**
	 * The outset distance (front and back) for the font.
	 */
	double outset;

};

} // FTGL
} // titania

#endif
