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

#include "GlyphContainer.h"

#include "Charmap.h"
#include "Face.h"
#include "Glyph/Glyph.h"

namespace titania {
namespace FTGL {

GlyphContainer::GlyphContainer (Face* const face) :
	       face (face),
	    charMap (new Charmap (face)),
	     glyphs (),
	      error (0)
{
	glyphs .emplace_back (nullptr);
}

bool
GlyphContainer::setCharMap (FT_Encoding encoding)
{
	const bool result = charMap -> setCharMap (encoding);

	error = charMap -> getError ();

	return result;
}

uint32_t
GlyphContainer::getFontIndex (const uint32_t charCode) const
{
	return charMap -> getFontIndex (charCode);
}

void
GlyphContainer::add (const std::shared_ptr <Glyph> & tempGlyph, const uint32_t charCode)
{
	charMap -> setInsertIndex (charCode, glyphs .size ());

	glyphs .emplace_back (tempGlyph);
}

std::shared_ptr <Glyph>
GlyphContainer::getGlyph (const uint32_t charCode) const
{
	const uint32_t index = charMap -> getGlyphListIndex (charCode);

	return glyphs [index];
}

BBox
GlyphContainer::getBBox (const uint32_t charCode) const
{
	return getGlyph (charCode) -> getBBox ();
}

double
GlyphContainer::advance (const uint32_t charCode,
                         const uint32_t nextCharCode)
{
	uint32_t left  = charMap -> getFontIndex (charCode);
	uint32_t right = charMap -> getFontIndex (nextCharCode);

	return face -> getKernAdvance (left, right) .x () + getGlyph (charCode) -> getAdvance () .x ();
}

Vector3d
GlyphContainer::triangulate (const uint32_t charCode,
                             const uint32_t nextCharCode,
                             const Vector3d & penPosition,
                             std::vector <size_t> & indices,
                             std::vector <Vector3d> & points) const
{
	const uint32_t left        = charMap -> getFontIndex (charCode);
	const uint32_t right       = charMap -> getFontIndex (nextCharCode);
	Vector3d       kernAdvance = face -> getKernAdvance (left, right);

	if (not face -> getError ())
		kernAdvance += getGlyph (charCode) -> triangulate (penPosition, indices, points);

	return kernAdvance;
}

GlyphContainer::~GlyphContainer ()
{ }

} // FTGL
} // titania
