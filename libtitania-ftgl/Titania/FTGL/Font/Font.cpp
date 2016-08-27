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

#include "Font.h"

#include "../GlyphContainer.h"
#include "../Unicode.h"
#include "FontImpl.h"

namespace titania {
namespace FTGL {

//
//  Font
//

Font::Font (char const* fontFilePath)
{
	impl = new FontImpl (this, fontFilePath);
}

Font::Font (const unsigned char* pBufferBytes, size_t bufferSizeInBytes)
{
	impl = new FontImpl (this, pBufferBytes, bufferSizeInBytes);
}

Font::Font (FontImpl* pImpl)
{
	impl = pImpl;
}

Font::~Font ()
{
	delete impl;
}

bool
Font::attach (const char* fontFilePath)
{
	return impl -> attach (fontFilePath);
}

bool
Font::attach (const unsigned char* pBufferBytes, size_t bufferSizeInBytes)
{
	return impl -> attach (pBufferBytes, bufferSizeInBytes);
}

bool
Font::setFaceSize (const unsigned int size, const unsigned int res)
{
	return impl -> setFaceSize (size, res);
}

unsigned int
Font::getFaceSize () const
{
	return impl -> getFaceSize ();
}

void
Font::setDepth (double depth)
{
	return impl -> setDepth (depth);
}

void
Font::setOutset (double outset)
{
	return impl -> setOutset (outset);
}

void
Font::setOutset (double front, double back)
{
	return impl -> setOutset (front, back);
}

void
Font::setGlyphLoadFlags (FT_Int flags)
{
	return impl -> setGlyphLoadFlags (flags);
}

bool
Font::setCharMap (FT_Encoding encoding)
{
	return impl -> setCharMap (encoding);
}

unsigned int
Font::getCharMapCount () const
{
	return impl -> CharMapCount ();
}

FT_Encoding*
Font::getCharMapList ()
{
	return impl -> getCharMapList ();
}

void
Font::setUseDisplayList (bool useList)
{
	return impl -> setUseDisplayList (useList);
}

double
Font::getAscender () const
{
	return impl -> getAscender ();
}

double
Font::getDescender () const
{
	return impl -> getDescender ();
}

double
Font::getLineHeight () const
{
	return impl -> getLineHeight ();
}

Point
Font::render (const char* string, const int len, Point position, Point spacing, FTGL::RenderMode renderMode)
{
	return impl -> render (string, len, position, spacing, renderMode);
}

Point
Font::render (const wchar_t* string, const int len, Point position, Point spacing, FTGL::RenderMode renderMode)
{
	return impl -> render (string, len, position, spacing, renderMode);
}

double
Font::advance (const char* string, const int len, Point spacing)
{
	return impl -> advance (string, len, spacing);
}

double
Font::advance (const wchar_t* string, const int len, Point spacing)
{
	return impl -> advance (string, len, spacing);
}

BBox
Font::getBBox (const char* string, const int len,
               Point position, Point spacing)
{
	return impl -> getBBox (string, len, position, spacing);
}

BBox
Font::getBBox (const wchar_t* string, const int len,
               Point position, Point spacing)
{
	return impl -> getBBox (string, len, position, spacing);
}

FT_Error
Font::getError () const
{
	return impl -> err;
}

//
//  FontImpl
//

FontImpl::FontImpl (Font* ftFont, char const* fontFilePath) :
	           face (fontFilePath),
	useDisplayLists (true),
	     load_flags (FT_LOAD_DEFAULT),
	           intf (ftFont),
	      glyphList (0)
{
	err = face .getError ();

	if (err == 0)
	{
		glyphList = new GlyphContainer (&face);
	}
}

FontImpl::FontImpl (Font* ftFont, const unsigned char* pBufferBytes,
                    size_t bufferSizeInBytes) :
	           face (pBufferBytes, bufferSizeInBytes),
	useDisplayLists (true),
	     load_flags (FT_LOAD_DEFAULT),
	           intf (ftFont),
	      glyphList (0)
{
	err = face .getError ();

	if (err == 0)
	{
		glyphList = new GlyphContainer (&face);
	}
}

FontImpl::~FontImpl ()
{
	if (glyphList)
	{
		delete glyphList;
	}
}

bool
FontImpl::attach (const char* fontFilePath)
{
	if (! face .attach (fontFilePath))
	{
		err = face .getError ();
		return false;
	}

	err = 0;
	return true;
}

bool
FontImpl::attach (const unsigned char* pBufferBytes,
                  size_t bufferSizeInBytes)
{
	if (! face .attach (pBufferBytes, bufferSizeInBytes))
	{
		err = face .getError ();
		return false;
	}

	err = 0;
	return true;
}

bool
FontImpl::setFaceSize (const unsigned int size, const unsigned int res)
{
	if (glyphList not_eq NULL)
	{
		delete glyphList;
		glyphList = NULL;
	}

	charSize = face .getSize (size, res);
	err      = face .getError ();

	if (err not_eq 0)
	{
		return false;
	}

	glyphList = new GlyphContainer (&face);
	return true;
}

unsigned int
FontImpl::getFaceSize () const
{
	return charSize .getCharSize ();
}

void
FontImpl::setDepth (double depth)
{ }

void
FontImpl::setOutset (double outset)
{ }

void
FontImpl::setOutset (double front, double back)
{ }

void
FontImpl::setGlyphLoadFlags (FT_Int flags)
{
	load_flags = flags;
}

bool
FontImpl::setCharMap (FT_Encoding encoding)
{
	bool result = glyphList -> setCharMap (encoding);

	err = glyphList -> getError ();
	return result;
}

unsigned int
FontImpl::CharMapCount () const
{
	return face .getCharMapCount ();
}

FT_Encoding*
FontImpl::getCharMapList ()
{
	return face .getCharMapList ();
}

void
FontImpl::setUseDisplayList (bool useList)
{
	useDisplayLists = useList;
}

double
FontImpl::getAscender () const
{
	return charSize .getAscender ();
}

double
FontImpl::getDescender () const
{
	return charSize .getDescender ();
}

double
FontImpl::getLineHeight () const
{
	return charSize .getHeight ();
}

template <typename T>
inline
BBox
FontImpl::getBBoxI (const T* string, const int len, Point position, Point spacing)
{
	BBox totalBBox;

	/* Only compute the bounds if string is non-empty. */
	if (string && ('\0' not_eq string [0]))
	{
		// for multibyte - we can't rely on sizeof(T) == character
		UnicodeStringItr <T> ustr (string);
		unsigned int         thisChar = *ustr ++;
		unsigned int         nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			totalBBox  = glyphList -> getBBox (thisChar);
			totalBBox += position;

			position += Point (glyphList -> advance (thisChar, nextChar), 0);
		}

		/* Expand totalBox by each glyph in string */
		for (int i = 1; (len < 0 && *ustr) or (len >= 0 && i < len); i ++)
		{
			thisChar = *ustr ++;
			nextChar = *ustr;

			if (checkGlyph (thisChar))
			{
				position += spacing;

				BBox tempBBox = glyphList -> getBBox (thisChar);
				tempBBox  += position;
				totalBBox |= tempBBox;

				position += Point (glyphList -> advance (thisChar, nextChar), 0);
			}
		}
	}

	return totalBBox;
}

BBox
FontImpl::getBBox (const char* string, const int len, Point position, Point spacing)
{
	/* The chars need to be unsigned because they are cast to int later */
	return getBBoxI ((const unsigned char*) string, len, position, spacing);
}

BBox
FontImpl::getBBox (const wchar_t* string, const int len, Point position, Point spacing)
{
	return getBBoxI (string, len, position, spacing);
}

template <typename T>
inline
double
FontImpl::advanceI (const T* string, const int len, Point spacing)
{
	double advance = 0;

	UnicodeStringItr <T> ustr (string);

	for (int i = 0; (len < 0 && *ustr) or (len >= 0 && i < len); i ++)
	{
		unsigned int thisChar = *ustr ++;
		unsigned int nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			advance += glyphList -> advance (thisChar, nextChar);
		}

		if (nextChar)
		{
			advance += spacing .X ();
		}
	}

	return advance;
}

double
FontImpl::advance (const char* string, const int len, Point spacing)
{
	/* The chars need to be unsigned because they are cast to int later */
	return advanceI ((const unsigned char*) string, len, spacing);
}

double
FontImpl::advance (const wchar_t* string, const int len, Point spacing)
{
	return advanceI (string, len, spacing);
}

template <typename T>
inline Point
FontImpl::renderI (const T* string, const int len,
                   Point position, Point spacing,
                   FTGL::RenderMode renderMode)
{
	// for multibyte - we can't rely on sizeof(T) == character
	UnicodeStringItr <T> ustr (string);

	for (int i = 0; (len < 0 && *ustr) or (len >= 0 && i < len); i ++)
	{
		unsigned int thisChar = *ustr ++;
		unsigned int nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			position += glyphList -> render (thisChar, nextChar, position, renderMode);
		}

		if (nextChar)
		{
			position += spacing;
		}
	}

	return position;
}

Point
FontImpl::render (const char* string, const int len, Point position, Point spacing, FTGL::RenderMode renderMode)
{
	return renderI ((const unsigned char*) string,
	                len, position, spacing, renderMode);
}

Point
FontImpl::render (const wchar_t* string, const int len, Point position, Point spacing, FTGL::RenderMode renderMode)
{
	return renderI (string, len, position, spacing, renderMode);
}

bool
FontImpl::checkGlyph (const unsigned int characterCode)
{
	if (glyphList -> getGlyph (characterCode))
	{
		return true;
	}

	unsigned int glyphIndex = glyphList -> getFontIndex (characterCode);
	FT_GlyphSlot ftSlot     = face .getGlyph (glyphIndex, load_flags);

	if (! ftSlot)
	{
		err = face .getError ();
		return false;
	}

	Glyph* tempGlyph = intf -> makeGlyph (ftSlot);

	if (! tempGlyph)
	{
		if (0 == err)
		{
			err = 0x13;
		}

		return false;
	}

	glyphList -> add (tempGlyph, characterCode);

	return true;
}

} // FTGL
} // titania
