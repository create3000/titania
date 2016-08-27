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

#ifndef __TITANIA_FTGL_CHARMAP_H__
#define __TITANIA_FTGL_CHARMAP_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "CharToGlyphIndexMap.h"

#include <cstdint>

namespace titania {
namespace FTGL {

/**
 * Charmap takes care of specifying the encoding for a font and mapping
 * character codes to glyph indices.
 *
 * It doesn't preprocess all indices, only on an as needed basis. This may
 * seem like a performance penalty but it is quicker than using the 'raw'
 * freetype calls and will save significant amounts of memory when dealing
 * with unicode encoding
 *
 * @see "Freetype 2 Documentation"
 *
 */

class Face;

class Charmap
{
public:

	/**
	 * Constructor
	 */
	Charmap (Face* face);

	/**
	 * Destructor
	 */
	virtual
	~Charmap ();

	/**
	 * Queries for the current character map code.
	 *
	 * @return  The current character map code.
	 */
	FT_Encoding
	getEncoding () const
	{ return ftEncoding; }

	/**
	 * Sets the character map for the face. If an error occurs the object is not modified.
	 * Valid encodings as at Freetype 2.0.4
	 *      ft_encoding_none
	 *      ft_encoding_symbol
	 *      ft_encoding_unicode
	 *      ft_encoding_latin_2
	 *      ft_encoding_sjis
	 *      ft_encoding_gb2312
	 *      ft_encoding_big5
	 *      ft_encoding_wansung
	 *      ft_encoding_johab
	 *      ft_encoding_adobe_standard
	 *      ft_encoding_adobe_expert
	 *      ft_encoding_adobe_custom
	 *      ft_encoding_apple_roman
	 *
	 * @param encoding  the Freetype encoding symbol. See above.
	 * @return          <code>true</code> if charmap was valid and set
	 *                  correctly.
	 */
	bool
	setCharMap (FT_Encoding encoding);

	/**
	 * Get the GlyphContainer index of the input character.
	 *
	 * @param characterCode The character code of the requested glyph in
	 *                      the current encoding eg apple roman.
	 * @return      The GlyphContainer index for the character or zero
	 *              if it wasn't found
	 */
	uint32_t
	getGlyphListIndex (const uint32_t characterCode) const;

	/**
	 * Get the font glyph index of the input character.
	 *
	 * @param characterCode The character code of the requested glyph in
	 *                      the current encoding eg apple roman.
	 * @return      The glyph index for the character.
	 */
	uint32_t
	getFontIndex (const uint32_t characterCode);

	/**
	 * Set the GlyphContainer index of the character code.
	 *
	 * @param characterCode  The character code of the requested glyph in
	 *                       the current encoding eg apple roman.
	 * @param containerIndex The index into the GlyphContainer of the
	 *                       character code.
	 */
	void
	setInsertIndex (const uint32_t characterCode, const size_t containerIndex);

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code. Zero means no error.
	 */
	FT_Error
	getError () const
	{ return err; }


private:

	/**
	 * Current character map code.
	 */
	FT_Encoding ftEncoding;

	/**
	 * The current Freetype face.
	 */
	const FT_Face ftFace;

	/**
	 * A structure that maps glyph indices to character codes
	 *
	 * < character code, face glyph index>
	 */
	using CharacterMap = CharToGlyphIndexMap;

	CharacterMap charMap;

	/**
	 * Precomputed font indices.
	 */
	static constexpr uint32_t MAX_PRECOMPUTED = 128;

	uint32_t charIndexCache [MAX_PRECOMPUTED];

	/**
	 * Current error code.
	 */
	FT_Error err;

};

} // FTGL
} // titania

#endif  //  __FTCharmap__
