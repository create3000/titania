/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
 *
 * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
 *
 * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
 *
 * The copyright notice above does not evidence any actual of intended
 * publication of such source code, and is an unpublished work by create3000.
 * This material contains CONFIDENTIAL INFORMATION that is the property of
 * create3000.
 *
 * No permission is granted to copy, distribute, or create derivative works from
 * the contents of this software, in whole or in part, without the prior written
 * permission of create3000.
 *
 * NON-MILITARY USE ONLY
 *
 * All create3000 software are effectively free software with a non-military use
 * restriction. It is free. Well commented source is provided. You may reuse the
 * source in any way you please with the exception anything that uses it must be
 * marked to indicate is contains 'non-military use only' components.
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
 *
 * This file is part of the Titania Project.
 *
 * Titania is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License version 3 only, as published by the
 * Free Software Foundation.
 *
 * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
 * details (a copy is included in the LICENSE file that accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version 3
 * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
 * copy of the GPLv3 License.
 *
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_MATH_GEOMETRY_TRIANGLE3_H__
#define __TITANIA_MATH_GEOMETRY_TRIANGLE3_H__

#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

// v1
//  |
//  |
//  |
//  |
//  ----------
// v2         v3

///  Returns the area of the triangle with the vertices @a v1, @a v2 and @a v3.
template <class Type>
inline
Type
area (const vector3 <Type> & v1, const vector3 <Type> & v2, const vector3 <Type> & v3)
{
	return abs (cross (v2 - v1, v3 - v1)) / 2;
}

// v1
//  |
//  |
//  |
//  |
//  ----------
// v2         v3

template <class Type>
inline
vector3 <Type>
normal (const vector3 <Type> & v1, const vector3 <Type> & v2, const vector3 <Type> & v3)
{
	return normalize (cross (v3 - v2, v1 - v2));
}

// v4 ----- v3
//  |       |
//  |       |
//  |       |
// v1 ----- v2

template <class Type>
inline
vector3 <Type>
normal (const vector3 <Type> & v1, const vector3 <Type> & v2, const vector3 <Type> & v3, const vector3 <Type> & v4)
{
	// (p3 - p1) x (p4 - p2)
	return normalize (cross (v3 - v1, v4 - v2));
}

///  Returns the index of one of the three points given defined by @a p0, @a p1, @a p2 to @a point.
template <class Type>
size_t
triangle_closest_point (const vector3 <Type> & p0, const vector3 <Type> & p1, const vector3 <Type> & p2, const vector3 <Type> & point)
{
	const auto distance0 = abs (p0 - point);
	const auto distance1 = abs (p1 - point);
	const auto distance2 = abs (p2 - point);

	if (distance0 < distance1)
	{
		if (distance0 < distance2)
			return 0;
	}
   
	if (distance1 < distance2)
		return 1;

	return 2;
}

///  Returns the squared distance from the triangle defined by @a p0, @a p1, @a p2 to @a point.
template <class Type>
Type
triangle_distance_to_point (const vector3 <Type> & p0, const vector3 <Type> & p1, const vector3 <Type> & p2, const vector3 <Type> & point)
{
	// http://www.geometrictools.com/GTEngine/Include/GteDistPoint3Triangle3.inl

	Type sqrDistance;

	vector3 <Type> diff  = p0 - point;
	vector3 <Type> edge0 = p1 - p0;
	vector3 <Type> edge1 = p2 - p0;
	Type           a00   = dot (edge0, edge0);
	Type           a01   = dot (edge0, edge1);
	Type           a11   = dot (edge1, edge1);
	Type           b0    = dot (diff, edge0);
	Type           b1    = dot (diff, edge1);
	Type           c     = dot (diff, diff);
	Type           det   = std::abs (a00 * a11 - a01 * a01);
	Type           s     = a01 * b1 - a11 * b0;
	Type           t     = a01 * b0 - a00 * b1;

	if (s + t <= det)
	{
		if (s < 0)
		{
			if (t < 0)  // region 4
			{
				if (b0 < 0)
				{
					t = 0;

					if (-b0 >= a00)
					{
						s           = 1;
						sqrDistance = a00 + 2 * b0 + c;
					}
					else
					{
						s           = -b0 / a00;
						sqrDistance = b0 * s + c;
					}
				}
				else
				{
					s = 0;

					if (b1 >= 0)
					{
						t           = 0;
						sqrDistance = c;
					}
					else if (-b1 >= a11)
					{
						t           = 1;
						sqrDistance = a11 + 2 * b1 + c;
					}
					else
					{
						t           = -b1 / a11;
						sqrDistance = b1 * t + c;
					}
				}
			}
			else  // region 3
			{
				s = 0;

				if (b1 >= 0)
				{
					t           = 0;
					sqrDistance = c;
				}
				else if (-b1 >= a11)
				{
					t           = 1;
					sqrDistance = a11 + 2 * b1 + c;
				}
				else
				{
					t           = -b1 / a11;
					sqrDistance = b1 * t + c;
				}
			}
		}
		else if (t < 0)  // region 5
		{
			t = 0;

			if (b0 >= 0)
			{
				s           = 0;
				sqrDistance = c;
			}
			else if (-b0 >= a00)
			{
				s           = 1;
				sqrDistance = a00 + 2 * b0 + c;
			}
			else
			{
				s           = -b0 / a00;
				sqrDistance = b0 * s + c;
			}
		}
		else  // region 0
		{
			// minimum at interior point
			Type invDet = 1 / det;
			s          *= invDet;
			t          *= invDet;
			sqrDistance = s * (a00 * s + a01 * t + 2 * b0) +
			              t * (a01 * s + a11 * t + 2 * b1) + c;
		}
	}
	else
	{
		Type tmp0, tmp1, numer, denom;

		if (s < 0)  // region 2
		{
			tmp0 = a01 + b0;
			tmp1 = a11 + b1;

			if (tmp1 > tmp0)
			{
				numer = tmp1 - tmp0;
				denom = a00 - 2 * a01 + a11;

				if (numer >= denom)
				{
					s           = 1;
					t           = 0;
					sqrDistance = a00 + 2 * b0 + c;
				}
				else
				{
					s           = numer / denom;
					t           = 1 - s;
					sqrDistance = s * (a00 * s + a01 * t + 2 * b0) +
					              t * (a01 * s + a11 * t + 2 * b1) + c;
				}
			}
			else
			{
				s = 0;

				if (tmp1 <= 0)
				{
					t           = 1;
					sqrDistance = a11 + 2 * b1 + c;
				}
				else if (b1 >= 0)
				{
					t           = 0;
					sqrDistance = c;
				}
				else
				{
					t           = -b1 / a11;
					sqrDistance = b1 * t + c;
				}
			}
		}
		else if (t < 0)  // region 6
		{
			tmp0 = a01 + b1;
			tmp1 = a00 + b0;

			if (tmp1 > tmp0)
			{
				numer = tmp1 - tmp0;
				denom = a00 - 2 * a01 + a11;

				if (numer >= denom)
				{
					t           = 1;
					s           = 0;
					sqrDistance = a11 + 2 * b1 + c;
				}
				else
				{
					t           = numer / denom;
					s           = 1 - t;
					sqrDistance = s * (a00 * s + a01 * t + 2 * b0) +
					              t * (a01 * s + a11 * t + 2 * b1) + c;
				}
			}
			else
			{
				t = 0;

				if (tmp1 <= 0)
				{
					s           = 1;
					sqrDistance = a00 + 2 * b0 + c;
				}
				else if (b0 >= 0)
				{
					s           = 0;
					sqrDistance = c;
				}
				else
				{
					s           = -b0 / a00;
					sqrDistance = b0 * s + c;
				}
			}
		}
		else  // region 1
		{
			numer = a11 + b1 - a01 - b0;

			if (numer <= 0)
			{
				s           = 0;
				t           = 1;
				sqrDistance = a11 + 2 * b1 + c;
			}
			else
			{
				denom = a00 - 2 * a01 + a11;

				if (numer >= denom)
				{
					s           = 1;
					t           = 0;
					sqrDistance = a00 + 2 * b0 + c;
				}
				else
				{
					s           = numer / denom;
					t           = 1 - s;
					sqrDistance = s * (a00 * s + a01 * t + 2 * b0) +
					              t * (a01 * s + a11 * t + 2 * b1) + c;
				}
			}
		}
	}

	// Account for numerical round-off error.
	if (sqrDistance < 0)
	{
		sqrDistance = 0;
	}

	return sqrDistance;

	//    result.triangleClosestPoint = triangle.v[0] + s*edge0 + t*edge1;
	//    result.triangleParameter[1] = s;
	//    result.triangleParameter[2] = t;
	//    result.triangleParameter[0] = 1 - s - t;
	//    return result;
}

} // math
} // titania

#endif
