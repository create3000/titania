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

#ifndef __TITANIA_FTGL_FACE_H__
#define __TITANIA_FTGL_FACE_H__

#include "Size.h"
#include "Types.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <memory>
#include <vector>

namespace titania {
namespace FTGL {

/**
 * Face class provides an abstraction layer for the Freetype Face.
 *
 * @see "Freetype 2 Documentation"
 *
 */
class Face
{
public:

	/**
	 * Opens and reads a face file. Error is set.
	 *
	 * @param fontFilePath  font file path.
	 */
	Face (const char* fontFilePath, bool precomputeKerning = true);

	/**
	 * Read face data from an in-memory buffer. Error is set.
	 *
	 * @param pBufferBytes  the in-memory buffer
	 * @param bufferSizeInBytes  the length of the buffer in bytes
	 */
	Face (const uint8_t* pBufferBytes, size_t bufferSizeInBytes, bool precomputeKerning = true);

	/**
	 * Attach auxilliary file to font (e.g., font metrics).
	 *
	 * @param fontFilePath  auxilliary font file path.
	 * @return          <code>true</code> if file has opened
	 *                  successfully.
	 */
	bool
	attach (const char* fontFilePath);

	/**
	 * Attach auxilliary data to font (e.g., font metrics) from memory
	 *
	 * @param pBufferBytes  the in-memory buffer
	 * @param bufferSizeInBytes  the length of the buffer in bytes
	 * @return          <code>true</code> if file has opened
	 *                  successfully.
	 */
	bool
	attach (const uint8_t* pBufferBytes, size_t bufferSizeInBytes);

	/**
	 * Get the freetype face object..
	 *
	 * @return pointer to an FT_Face.
	 */
	const std::unique_ptr <FT_Face> &
	getFace () const
	{ return ftFace; }

	/**
	 * Sets the char size for the current face.
	 *
	 * This doesn't guarantee that the size was set correctly. Clients
	 * should check errors.
	 *
	 * @param size      the face size in points (1/72 inch)
	 * @param res       the resolution of the target device.
	 * @return          <code>Size</code> object
	 */
	const Size &
	getSize (const uint32_t size, const uint32_t res);

	/**
	 * Get the number of character maps in this face.
	 *
	 * @return character map count.
	 */
	uint32_t
	getCharMapCount () const;

	/**
	 * Get a list of character maps in this face.
	 *
	 * @return pointer to the first encoding.
	 */
	FT_Encoding*
	getCharMapList () const;

	/**
	 * Gets the kerning vector between two glyphs
	 */
	Vector3d
	getKernAdvance (uint32_t index1, uint32_t index2) const;

	/**
	 * Loads and creates a Freetype glyph.
	 */
	FT_GlyphSlot
	getGlyph (uint32_t index, FT_Int load_flags) const;

	/**
	 * Gets the number of glyphs in the current face.
	 */
	uint32_t
	getGlyphCount () const
	{ return numGlyphs; }

	/**
	 * Queries for errors.
	 *
	 * @return  The current error code.
	 */
	FT_Error
	getError () const
	{ return error; }

	/**
	 * Destructor
	 *
	 * Disposes of the current Freetype Face.
	 */
	virtual
	~Face ();


private:

	/**
	 * The Freetype face
	 */
	std::unique_ptr <FT_Face> ftFace;

	/**
	 * The size object associated with this face
	 */
	Size charSize;

	/**
	 * The number of glyphs in this face
	 */
	int32_t numGlyphs;

	mutable FT_Encoding* fontEncodingList;

	/**
	 * This face has kerning tables
	 */
	bool hasKerningTable;

	/**
	 * If this face has kerning tables, we can cache them.
	 */
	void
	buildKerningCache ();

	static constexpr uint32_t MAX_PRECOMPUTED = 128;

	std::vector <double> kerningCache;

	/**
	 * Current error code. Zero means no error.
	 */
	mutable FT_Error error;

};

} // FTGL
} // titania

#endif
