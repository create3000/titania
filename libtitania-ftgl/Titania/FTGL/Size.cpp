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

#include "Size.h"

namespace titania {
namespace FTGL {

Size::Size () :
	     ftFace (0),
	     ftSize (0),
	       size (0),
	xResolution (0),
	yResolution (0),
	        err (0)
{ }

Size::~Size ()
{ }

bool
Size::setCharSize (FT_Face* face, unsigned int pointSize, unsigned int xRes, unsigned int yRes)
{
	if (size not_eq pointSize or xResolution not_eq xRes or yResolution not_eq yRes)
	{
		err = FT_Set_Char_Size (*face, 0L, pointSize * 64, xResolution, yResolution);

		if (! err)
		{
			ftFace      = face;
			size        = pointSize;
			xResolution = xRes;
			yResolution = yRes;
			ftSize      = (*ftFace) -> size;
		}
	}

	return ! err;
}

unsigned int
Size::getCharSize () const
{
	return size;
}

double
Size::getAscender () const
{
	return ftSize == 0 ? 0 : static_cast <double> (ftSize -> metrics .ascender) / 64.0;
}

double
Size::getDescender () const
{
	return ftSize == 0 ? 0 : static_cast <double> (ftSize -> metrics .descender) / 64.0;
}

double
Size::getHeight () const
{
	if (0 == ftSize)
	{
		return 0;
	}

	if (FT_IS_SCALABLE ((*ftFace)))
	{
		return ((*ftFace) -> bbox .yMax - (*ftFace) -> bbox .yMin) * ((double) ftSize -> metrics .y_ppem / (double) (*ftFace) -> units_per_EM);
	}
	else
	{
		return static_cast <double> (ftSize -> metrics .height) / 64.0;
	}
}

double
Size::getWidth () const
{
	if (0 == ftSize)
	{
		return 0;
	}

	if (FT_IS_SCALABLE ((*ftFace)))
	{
		return ((*ftFace) -> bbox .xMax - (*ftFace) -> bbox .xMin) * (static_cast <double> (ftSize -> metrics .x_ppem) / static_cast <double> ((*ftFace) -> units_per_EM));
	}
	else
	{
		return static_cast <double> (ftSize -> metrics .max_advance) / 64.0;
	}
}

double
Size::getUnderline () const
{
	return 0;
}

} // FTGL
} // titania
