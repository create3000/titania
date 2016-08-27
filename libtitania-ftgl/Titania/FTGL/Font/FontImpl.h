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

#ifndef __TITANIA_FTFONT_FTFONT_IMPL_H__
#define __TITANIA_FTFONT_FTFONT_IMPL_H__

#include "../ftgl.h"

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

	FontImpl (Font* ftFont, const unsigned char* pBufferBytes,
	            size_t bufferSizeInBytes);

	virtual
	~FontImpl ();

	virtual
	bool
	attach (const char* fontFilePath);

	virtual
	bool
	attach (const unsigned char* pBufferBytes,
	        size_t bufferSizeInBytes);

	virtual
	void
	setGlyphLoadFlags (FT_Int flags);

	virtual
	bool
	setCharMap (FT_Encoding encoding);

	virtual
	unsigned int
	CharMapCount () const;

	virtual
	FT_Encoding*
	getCharMapList ();

	virtual
	void
	setUseDisplayList (bool useList);

	virtual
	float
	getAscender () const;

	virtual
	float
	getDescender () const;

	virtual
	float
	getLineHeight () const;

	virtual
	bool
	setFaceSize (const unsigned int size, const unsigned int res);

	virtual
	unsigned int
	getFaceSize () const;

	virtual
	void
	setDepth (float depth);

	virtual
	void
	setOutset (float outset);

	virtual
	void
	setOutset (float front, float back);

	virtual
	BBox
	getBBox (const char* s, const int len, Point, Point);

	virtual
	BBox
	getBBox (const wchar_t* s, const int len, Point, Point);

	virtual
	float
	advance (const char* s, const int len, Point);

	virtual
	float
	advance (const wchar_t* s, const int len, Point);

	virtual
	Point
	render (const char* s, const int len, Point, Point, int);

	virtual
	Point
	render (const wchar_t* s, const int len, Point, Point, int);

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
	checkGlyph (const unsigned int chr);

	/**
	 * An object that holds a list of glyphs
	 */
	GlyphContainer* glyphList;

	/**
	 * Current pen or cursor position;
	 */
	Point pen;

	/* Internal generic BBox() implementation */
	template <typename T>
	inline
	BBox
	getBBoxI (const T* s, const int len,
	       Point position, Point spacing);

	/* Internal generic Advance() implementation */
	template <typename T>
	inline
	float
	advanceI (const T* s, const int len, Point spacing);

	/* Internal generic Render() implementation */
	template <typename T>
	inline
	Point
	renderI (const T* s, const int len, Point position, Point spacing, int mode);

};

} // FTGL
} // titania

#endif  //  __FTFontImpl__
