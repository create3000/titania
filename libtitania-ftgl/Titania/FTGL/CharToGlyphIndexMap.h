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

#ifndef __TITANIA_FTGL_CHAR_TO_GLYPH_INDEX_MAP_H__
#define __TITANIA_FTGL_CHAR_TO_GLYPH_INDEX_MAP_H__

//#include <stdlib.h>

namespace titania {
namespace FTGL {

/**
 * Provides a non-STL alternative to the STL map<unsigned long, unsigned long>
 * which maps character codes to glyph indices inside Charmap.
 *
 * Implementation:
 *   - NumberOfBuckets buckets are considered.
 *   - Each bucket has BucketSize entries.
 *   - When the glyph index for the character code C has to be stored, the
 *     bucket this character belongs to is found using 'C div BucketSize'.
 *     If this bucket has not been allocated yet, do it now.
 *     The entry in the bucked is found using 'C mod BucketSize'.
 *     If it is set to IndexNotFound, then the glyph entry has not been set.
 *   - Try to mimic the calls made to the STL map API.
 *
 * Caveats:
 *   - The glyph index is now a signed long instead of unsigned long, so
 *     the special value IndexNotFound (= -1) can be used to specify that the
 *     glyph index has not been stored yet.
 */
class CharToGlyphIndexMap
{
public:

	using CharacterCode = unsigned long;
	using GlyphIndex    = signed long;

	enum
	{
		NumberOfBuckets = 256,
		BucketSize      = 256,
		IndexNotFound   = -1
	};

	CharToGlyphIndexMap ()
	{
		indices = 0;
	}

	virtual
	~CharToGlyphIndexMap ()
	{
		if (indices)
		{
			// Free all buckets
			this -> clear ();

			// Free main structure
			delete [ ] indices;
			indices = 0;
		}
	}

	void
	clear ()
	{
		if (indices)
		{
			for (int32_t i = 0; i < CharToGlyphIndexMap::NumberOfBuckets; i ++)
			{
				if (indices [i])
				{
					delete [ ] indices [i];
					indices [i] = 0;
				}
			}
		}
	}

	const GlyphIndex
	find (CharacterCode c) const
	{
		if (! indices)
		{
			return 0;
		}

		// Find position of char code in buckets
		div_t pos = div (c, CharToGlyphIndexMap::BucketSize);

		if (! indices [pos.quot])
		{
			return 0;
		}

		const CharToGlyphIndexMap::GlyphIndex* ptr = &indices [pos.quot] [pos.rem];

		if (*ptr == CharToGlyphIndexMap::IndexNotFound)
		{
			return 0;
		}

		return *ptr;
	}

	void
	insert (CharacterCode c, GlyphIndex g)
	{
		if (! indices)
		{
			indices = new GlyphIndex* [CharToGlyphIndexMap::NumberOfBuckets];

			for (int32_t i = 0; i < CharToGlyphIndexMap::NumberOfBuckets; i ++)
			{
				indices [i] = 0;
			}
		}

		// Find position of char code in buckets
		div_t pos = div (c, CharToGlyphIndexMap::BucketSize);

		// Allocate bucket if does not exist yet
		if (! indices [pos.quot])
		{
			indices [pos.quot] = new GlyphIndex [CharToGlyphIndexMap::BucketSize];

			for (int32_t i = 0; i < CharToGlyphIndexMap::BucketSize; i ++)
			{
				indices [pos.quot] [i] = CharToGlyphIndexMap::IndexNotFound;
			}
		}

		indices [pos.quot] [pos.rem] = g;
	}

private:

	GlyphIndex** indices;

};

} // FTGL
} // titania

#endif  //  __FTCharToGlyphIndexMap__
