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

#include "Face.h"

#include "Library.h"

#include FT_TRUETYPE_TABLES_H

namespace titania {
namespace FTGL {

Face::Face (const char* fontFilePath, bool precomputeKerning) :
	       numGlyphs (0),
	fontEncodingList (0),
	    kerningCache (),
	           error (0)
{
	const FT_Long DEFAULT_FACE_INDEX = 0;

	ftFace .reset (new FT_Face ());

	error = FT_New_Face (*Library::getInstance () .getLibrary (), fontFilePath, DEFAULT_FACE_INDEX, ftFace .get ());

	if (error)
	{
		ftFace .reset ();
		return;
	}

	numGlyphs       = (*ftFace) -> num_glyphs;
	hasKerningTable = (FT_HAS_KERNING ((*ftFace)) not_eq 0);

	if (hasKerningTable and precomputeKerning)
	{
		buildKerningCache ();
	}
}

Face::Face (const uint8_t* pBufferBytes, size_t bufferSizeInBytes,
            bool precomputeKerning) :
	       numGlyphs (0),
	fontEncodingList (0),
	    kerningCache (0),
	           error (0)
{
	const FT_Long DEFAULT_FACE_INDEX = 0;

	ftFace .reset (new FT_Face ());

	error = FT_New_Memory_Face (*Library::getInstance () .getLibrary (),
	                            (FT_Byte const*) pBufferBytes, (FT_Long) bufferSizeInBytes,
	                            DEFAULT_FACE_INDEX, ftFace .get ());

	if (error)
	{
		ftFace .reset ();
		return;
	}

	numGlyphs       = (*ftFace) -> num_glyphs;
	hasKerningTable = (FT_HAS_KERNING ((*ftFace)) not_eq 0);

	if (hasKerningTable and precomputeKerning)
	{
		buildKerningCache ();
	}
}

bool
Face::attach (const char* fontFilePath)
{
	error = FT_Attach_File (*ftFace, fontFilePath);
	return not error;
}

bool
Face::attach (const uint8_t* pBufferBytes, size_t bufferSizeInBytes)
{
	FT_Open_Args open;

	open .flags       = FT_OPEN_MEMORY;
	open .memory_base = (FT_Byte const*) pBufferBytes;
	open .memory_size = (FT_Long) bufferSizeInBytes;

	error = FT_Attach_Stream (*ftFace, &open);
	return not error;
}

const Size &
Face::getSize (const uint32_t size, const uint32_t res)
{
	charSize .setCharSize (ftFace .get (), size, res, res); // TODO: ftFace .get () to ftFace only, because of unique_ptr

	error = charSize .getError ();

	return charSize;
}

uint32_t
Face::getCharMapCount () const
{
	return (*ftFace) -> num_charmaps;
}

FT_Encoding* // TODO: std::shared_ptr
Face::getCharMapList () const
{
	if (not fontEncodingList)
	{
		fontEncodingList = new FT_Encoding [getCharMapCount ()];

		for (size_t i = 0; i < getCharMapCount (); ++ i)
		{
			fontEncodingList [i] = (*ftFace) -> charmaps [i] -> encoding;
		}
	}

	return fontEncodingList;
}

Vector3d
Face::getKernAdvance (uint32_t index1, uint32_t index2) const
{
	double x, y;

	if (not hasKerningTable or not index1 or not index2)
	{
		return Vector3d (0, 0, 0);
	}

	if (kerningCache .size () and index1 < Face::MAX_PRECOMPUTED and index2 < Face::MAX_PRECOMPUTED)
	{
		x = kerningCache [2 * (index2 * Face::MAX_PRECOMPUTED + index1)];
		y = kerningCache [2 * (index2 * Face::MAX_PRECOMPUTED + index1) + 1];
		return Vector3d (x, y, 0);
	}

	FT_Vector kernAdvance;
	kernAdvance .x = kernAdvance .y = 0;

	error = FT_Get_Kerning (*ftFace, index1, index2, ft_kerning_unfitted, &kernAdvance);

	if (error)
	{
		return Vector3d (0, 0, 0);
	}

	x = static_cast <double> (kernAdvance.x) / 64.0;
	y = static_cast <double> (kernAdvance.y) / 64.0;

	return Vector3d (x, y, 0);
}

FT_GlyphSlot
Face::getGlyph (uint32_t index, FT_Int load_flags) const
{
	error = FT_Load_Glyph (*ftFace, index, load_flags);

	if (error)
	{
		return nullptr;
	}

	return (*ftFace) -> glyph;
}

void
Face::buildKerningCache ()
{
	FT_Vector kernAdvance;

	kernAdvance .x = 0;
	kernAdvance .y = 0;

	kerningCache .resize (Face::MAX_PRECOMPUTED * Face::MAX_PRECOMPUTED * 2);

	for (uint32_t j = 0; j < Face::MAX_PRECOMPUTED; j ++)
	{
		for (uint32_t i = 0; i < Face::MAX_PRECOMPUTED; i ++)
		{
			error = FT_Get_Kerning (*ftFace, i, j, ft_kerning_unfitted, &kernAdvance);

			if (error)
			{
				kerningCache .clear ();
				return;
			}

			kerningCache [2 * (j * Face::MAX_PRECOMPUTED + i)]     = static_cast <double> (kernAdvance.x) / 64.0;
			kerningCache [2 * (j * Face::MAX_PRECOMPUTED + i) + 1] = static_cast <double> (kernAdvance.y) / 64.0;
		}
	}
}

Face::~Face ()
{
	if (ftFace)
	{
		FT_Done_Face (*ftFace);
	}
}

} // FTGL
} // titania
