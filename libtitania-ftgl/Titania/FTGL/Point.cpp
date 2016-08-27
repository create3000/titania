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

#include "Point.h"

#include <cmath>

namespace titania {
namespace FTGL {

bool
operator == (const Vector3d & a, const Vector3d & b)
{
	return ((a.values [0] == b .values [0]) && (a.values [1] == b .values [1]) && (a.values [2] == b .values [2]));
}

bool
operator not_eq (const Vector3d & a, const Vector3d & b)
{
	return ((a.values [0] not_eq b .values [0]) or (a.values [1] not_eq b .values [1]) or (a.values [2] not_eq b .values [2]));
}

Vector3d
normalize (const Vector3d & value)
{
	double norm = sqrt (value .x () * value .x ()
	                    + value .y () * value .y ()
	                    + value .z () * value .z ());

	if (norm == 0.0)
	{
		return value;
	}

	Vector3d temp (value .x () / norm, value .y () / norm, value .z () / norm);
	return temp;
}

} // FTGL
} // titania
