/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
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

#ifndef __TITANIA_FTGL_SIZE_H__
#define __TITANIA_FTGL_SIZE_H__

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cstdint>

namespace titania {
namespace FTGL {

/**
 * Size class provides an abstraction layer for the Freetype Size.
 *
 * @see "Freetype 2 Documentation"
 *
 */
class Size
{
public:

	/**
	 * Default Constructor
	 */
	Size ();

	/**
	 * Destructor
	 */
	virtual
	~Size ();

	/**
	 * Sets the char size for the current face.
	 *
	 * This doesn't guarantee that the size was set correctly. Clients
	 * should check errors. If an error does occur the size object isn't modified.
	 *
	 * @param face           Parent face for this size object
	 * @param point_size     the face size in points (1/72 inch)
	 * @param x_resolution   the horizontal resolution of the target device.
	 * @param y_resolution   the vertical resolution of the target device.
	 * @return          <code>true</code> if the size has been set. Clients should check Error() for more information if this function returns false()
	 */
	bool
	setCharSize (FT_Face* face, uint32_t point_size,
	             uint32_t x_resolution, uint32_t y_resolution);

	/**
	 * get the char size for the current face.
	 *
	 * @return The char size in points
	 */
	uint32_t
	getCharSize () const;

	/**
	 * Gets the global ascender height for the face in pixels.
	 *
	 * @return  Ascender height
	 */
	double
	getAscender () const;

	/**
	 * Gets the global descender height for the face in pixels.
	 *
	 * @return  Ascender height
	 */
	double
	getDescender () const;

	/**
	 * Gets the global face height for the face.
	 *
	 * If the face is scalable this returns the height of the global
	 * bounding box which ensures that any glyph will be less than or
	 * equal to this height. If the font isn't scalable there is no
	 * guarantee that glyphs will not be taller than this value.
	 *
	 * @return  height in pixels.
	 */
	double
	getHeight () const;

	/**
	 * Gets the global face width for the face.
	 *
	 * If the face is scalable this returns the width of the global
	 * bounding box which ensures that any glyph will be less than or
	 * equal to this width. If the font isn't scalable this value is
	 * the max_advance for the face.
	 *
	 * @return  width in pixels.
	 */
	double
	getWidth () const;

	/**
	 * Gets the underline position for the face.
	 *
	 * @return  underline position in pixels
	 */
	double
	getUnderline () const;

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error
	getError () const { return err; }


private:

	/**
	 * The current Freetype face that this Size object relates to.
	 */
	FT_Face* ftFace;

	/**
	 *  The Freetype size.
	 */
	FT_Size ftSize;

	/**
	 *  The size in points.
	 */
	uint32_t size;

	/**
	 *  The horizontal resolution.
	 */
	uint32_t xResolution;

	/**
	 *  The vertical resolution.
	 */
	uint32_t yResolution;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;

};

} // FTGL
} // titania

#endif  //  __FTSize__
