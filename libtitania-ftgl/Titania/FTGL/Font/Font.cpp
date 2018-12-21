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

#include <Titania/LOG.h>

namespace titania {
namespace FTGL {

//
//  Font
//

Font::Font (const std::string & fontFilePath) :
	       face (fontFilePath .c_str ()),
	   charSize (),
	 load_flags (FT_LOAD_DEFAULT),
	  glyphList (),
	        pen (),
	      error (0)
{
	error = face .getError ();

	if (error == 0)
	{
		glyphList .reset (new GlyphContainer (&face));
	}
}

Font::Font (const uint8_t* pBufferBytes, const size_t bufferSizeInBytes) :
	       face (pBufferBytes, bufferSizeInBytes),
	   charSize (),
	 load_flags (FT_LOAD_DEFAULT),
	  glyphList (),
	        pen (),
	      error (0)
{
	error = face .getError ();

	if (error == 0)
	{
		glyphList .reset (new GlyphContainer (&face));
	}
}

bool
Font::setFaceSize (const uint32_t size, const uint32_t res)
{
	if (glyphList)
		glyphList .reset ();

	charSize = face .getSize (size, res);
	error    = face .getError ();

	if (error not_eq 0)
	{
		return false;
	}

	glyphList .reset (new GlyphContainer (&face));
	return true;
}

uint32_t
Font::getFaceSize () const
{
	return charSize .getCharSize ();
}

void
Font::setGlyphLoadFlags (FT_Int flags)
{
	load_flags = flags;
}

bool
Font::setCharMap (FT_Encoding encoding)
{
	bool result = glyphList -> setCharMap (encoding);

	error = glyphList -> getError ();
	return result;
}

uint32_t
Font::getCharMapCount () const
{
	return face .getCharMapCount ();
}

FT_Encoding*
Font::getCharMapList () const
{
	return face .getCharMapList ();
}

double
Font::getAscender () const
{
	return charSize .getAscender ();
}

double
Font::getDescender () const
{
	return charSize .getDescender ();
}

double
Font::getLineHeight () const
{
	return charSize .getHeight ();
}

BBox
Font::getBBox (const char* string, const int32_t len, Vector3d position, Vector3d spacing)
{
	BBox totalBBox;

	/* Only compute the bounds if string is non-empty. */
	if (string and ('\0' not_eq string [0]))
	{
		// for multibyte - we can't rely on sizeof(T) == character
		UnicodeStringItr <uint8_t> ustr ((const uint8_t*) string);

		uint32_t thisChar = *ustr ++;
		uint32_t nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			totalBBox  = glyphList -> getBBox (thisChar);
			totalBBox += position;

			position += Vector3d (glyphList -> advance (thisChar, nextChar), 0, 0);
		}

		/* Expand totalBox by each glyph in string */
		for (int32_t i = 1; (len < 0 and *ustr) or (len >= 0 and i < len); i ++)
		{
			thisChar = *ustr ++;
			nextChar = *ustr;

			if (checkGlyph (thisChar))
			{
				position += spacing;

				BBox tempBBox = glyphList -> getBBox (thisChar);
				tempBBox  += position;
				totalBBox |= tempBBox;

				position += Vector3d (glyphList -> advance (thisChar, nextChar), 0, 0);
			}
		}
	}

	return totalBBox;
}

FT_Error
Font::getError () const
{
	return error;
}

bool
Font::attach (const char* fontFilePath)
{
	if (not face .attach (fontFilePath))
	{
		error = face .getError ();
		return false;
	}

	error = 0;
	return true;
}

bool
Font::attach (const uint8_t* pBufferBytes, size_t bufferSizeInBytes)
{
	if (not face .attach (pBufferBytes, bufferSizeInBytes))
	{
		error = face .getError ();
		return false;
	}

	error = 0;
	return true;
}

Vector3d
Font::triangulate (const std::string & string,
                   Vector3d position,
                   const Vector3d & spacing,
                   std::vector <size_t> & indices,
                   std::vector <Vector3d> & points) const
{
	// for multibyte - we can't rely on sizeof(T) == character
	UnicodeStringItr <uint8_t> ustr ((const uint8_t*) string .c_str ());

	for (int32_t i = 0; *ustr; ++ i)
	{
		uint32_t thisChar = *ustr ++;
		uint32_t nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			position += glyphList -> triangulate (thisChar, nextChar, position, indices, points);
		}

		if (nextChar)
		{
			position += spacing;
		}
	}

	return position;
}

double
Font::advance (const char* string, const int32_t len, Vector3d spacing)
{
	double advance = 0;

	UnicodeStringItr <uint8_t> ustr ((const uint8_t*) string);

	for (int32_t i = 0; (len < 0 and *ustr) or (len >= 0 and i < len); i ++)
	{
		uint32_t thisChar = *ustr ++;
		uint32_t nextChar = *ustr;

		if (checkGlyph (thisChar))
		{
			advance += glyphList -> advance (thisChar, nextChar);
		}

		if (nextChar)
		{
			advance += spacing .x ();
		}
	}

	return advance;
}

bool
Font::checkGlyph (const uint32_t characterCode) const
{
	if (glyphList -> getGlyph (characterCode))
	{
		return true;
	}

	uint32_t     glyphIndex = glyphList -> getFontIndex (characterCode);
	FT_GlyphSlot ftSlot     = face .getGlyph (glyphIndex, load_flags);

	if (not ftSlot)
	{
		error = face .getError ();
		return false;
	}

	const auto tempGlyph = makeGlyph (ftSlot);

	if (not tempGlyph)
	{
		if (error == 0)
		{
			error = 0x13;
		}

		return false;
	}

	glyphList -> add (tempGlyph, characterCode);

	return true;
}

Font::~Font ()
{ }

//
//  FontImpl
//

} // FTGL
} // titania
