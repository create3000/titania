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

#ifndef __TITANIA_FTGL_FONT_POLYGON_FONT_IMPL_H__
#define __TITANIA_FTGL_FONT_POLYGON_FONT_IMPL_H__

#include "FontImpl.h"

namespace titania {
namespace FTGL {

class Glyph;

class PolygonFontImpl :
	public FontImpl
{
	friend class PolygonFont;


protected:

	PolygonFontImpl (Font* ftFont, const char* fontFilePath);

	PolygonFontImpl (Font* ftFont, const uint8_t* pBufferBytes,
	                 size_t bufferSizeInBytes);

	/**
	 * Set the outset distance for the font. Only implemented by
	 * FTOutlineFont, PolygonFont and FTExtrudeFont
	 *
	 * @param depth  The outset distance.
	 */
	virtual
	void
	setOutset (double o) final override
	{ outset = o; }


private:

	/**
	 * The outset distance (front and back) for the font.
	 */
	double outset;

};

} // FTGL
} // titania

#endif  //  __FTPolygonFontImpl__
