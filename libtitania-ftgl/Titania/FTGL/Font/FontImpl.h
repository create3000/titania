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

#ifndef __TITANIA_FTGL_FONT_FONT_IMPL_H__
#define __TITANIA_FTGL_FONT_FONT_IMPL_H__

#include "../Face.h"

namespace titania {
namespace FTGL {

class GlyphContainer;
class Glyph;

class FontImpl
{
	friend class Font;


protected:

	FontImpl (Font* ftFont, char const* fontFilePath);

	FontImpl (Font* ftFont, const uint8_t* pBufferBytes,
	          size_t bufferSizeInBytes);

	virtual
	~FontImpl ();

	virtual
	bool
	attach (const char* fontFilePath);

	virtual
	bool
	attach (const uint8_t* pBufferBytes,
	        size_t bufferSizeInBytes);

	virtual
	void
	setGlyphLoadFlags (FT_Int flags);

	virtual
	bool
	setCharMap (FT_Encoding encoding);

	virtual
	uint32_t
	CharMapCount () const;

	virtual
	FT_Encoding*
	getCharMapList ();

	virtual
	void
	setUseDisplayList (bool useList);

	virtual
	double
	getAscender () const;

	virtual
	double
	getDescender () const;

	virtual
	double
	getLineHeight () const;

	virtual
	bool
	setFaceSize (const uint32_t size, const uint32_t res);

	virtual
	uint32_t
	getFaceSize () const;

	virtual
	void
	setDepth (double depth);

	virtual
	void
	setOutset (double outset);

	virtual
	void
	setOutset (double front, double back);

	virtual
	BBox
	getBBox (const char* s, const int32_t len, Vector3d, Vector3d);

	virtual
	double
	advance (const char* s, const int32_t len, Vector3d);

	virtual
	Vector3d
	render (const char* s, const int32_t len, Vector3d, Vector3d, FTGL::RenderMode);

	/**
	 * Current face object
	 */
	Face face;

	/**
	 * Current size object
	 */
	Size charSize;

	/**
	 * Flag to enable or disable the use of Display Lists inside FTGL
	 * <code>true</code> turns ON display lists.
	 * <code>false</code> turns OFF display lists.
	 */
	bool useDisplayLists;

	/**
	 * The default glyph loading flags.
	 */
	FT_Int load_flags;

	/**
	 * Current error code. Zero means no error.
	 */
	FT_Error err;


private:

	/**
	 * A link back to the interface of which we are the implementation.
	 */
	Font* intf;

	/**
	 * Check that the glyph at <code>chr</code> exist. If not load it.
	 *
	 * @param chr  character index
	 * @return <code>true</code> if the glyph can be created.
	 */
	bool
	checkGlyph (const uint32_t chr);

	/**
	 * An object that holds a list of glyphs
	 */
	GlyphContainer* glyphList;

	/**
	 * Current pen or cursor position;
	 */
	Vector3d pen;

};

} // FTGL
} // titania

#endif
