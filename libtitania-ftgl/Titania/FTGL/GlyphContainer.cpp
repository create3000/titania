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

GlyphContainer::GlyphContainer (Face* f) :
	face (f),
	 err (0)
{
	glyphs .push_back (NULL);
	charMap = new Charmap (face);
}

GlyphContainer::~GlyphContainer ()
{
	GlyphVector::iterator it;

	for (it = glyphs .begin (); it not_eq glyphs .end (); ++ it)
	{
		delete *it;
	}

	glyphs .clear ();
	delete charMap;
}

bool
GlyphContainer::setCharMap (FT_Encoding encoding)
{
	bool result = charMap -> setCharMap (encoding);

	err = charMap -> getError ();
	return result;
}

unsigned int
GlyphContainer::getFontIndex (const unsigned int charCode) const
{
	return charMap -> getFontIndex (charCode);
}

void
GlyphContainer::add (Glyph* tempGlyph, const unsigned int charCode)
{
	charMap -> setInsertIndex (charCode, glyphs .size ());
	glyphs .push_back (tempGlyph);
}

const Glyph* const
GlyphContainer::getGlyph (const unsigned int charCode) const
{
	unsigned int index = charMap -> getGlyphListIndex (charCode);

	return glyphs [index];
}

BBox
GlyphContainer::getBBox (const unsigned int charCode) const
{
	return getGlyph (charCode) -> getBBox ();
}

double
GlyphContainer::advance (const unsigned int charCode,
                         const unsigned int nextCharCode)
{
	unsigned int left  = charMap -> getFontIndex (charCode);
	unsigned int right = charMap -> getFontIndex (nextCharCode);

	return face -> getKernAdvance (left, right) .x () + getGlyph (charCode) -> getAdvance ();
}

Vector3d
GlyphContainer::render (const unsigned int charCode,
                        const unsigned int nextCharCode,
                        Vector3d penPosition, FTGL::RenderMode renderMode)
{
	unsigned int left  = charMap -> getFontIndex (charCode);
	unsigned int right = charMap -> getFontIndex (nextCharCode);

	Vector3d kernAdvance = face -> getKernAdvance (left, right);

	if (! face -> getError ())
	{
		unsigned int index = charMap -> getGlyphListIndex (charCode);
		kernAdvance += glyphs [index] -> render (penPosition, renderMode);
	}

	return kernAdvance;
}

} // FTGL
} // titania
