/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_MATH_ALGORITHMS_BARYCENTRIC_H__
#define __TITANIA_MATH_ALGORITHMS_BARYCENTRIC_H__

#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"
#include "../Numbers/Vector4.h"

#include <tuple>

namespace titania {
namespace math {

/**
 * @returns Barycentric coordinates (u, v, w) for @a point with respect to triangle (a, b, c).
 * @param point  in cartesian coordinate system.
 * @param a      first point of triangle.
 * @param b      second point of triangle.
 * @param c      third point of triangle.\n
 * Type is any type supporting copy constructions.
 */
template <class Type, class Vector>
vector3 <Type>
to_barycentric (const vector3 <Type> & point,
                const Vector & a,
                const Vector & b,
                const Vector & c)
{
	const auto v0 = b - a;
	const auto v1 = c - a;
	const auto v2 = point - a;

	const auto d00   = dot (v0, v0);
	const auto d01   = dot (v0, v1);
	const auto d11   = dot (v1, v1);
	const auto d20   = dot (v2, v0);
	const auto d21   = dot (v2, v1);
	const auto denom = d00 * d11 - d01 * d01;

	const auto v = (d11 * d20 - d01 * d21) / denom;
	const auto t = (d00 * d21 - d01 * d20) / denom;
	const auto u = 1 - v - t;

	return vector3 <Type> (u, v, t);
}

/**
 * @returns Computes coordinates on triangle defined @a point0, @a point1, @a point2 by from @a barycentric coordinates.
 * @param  point0  first point of triangle.
 * @param  point1  second point of triangle.
 * @param  point2  third point of triangle.
 * @param  barycentric  barycentric vector of triangle.
 */
template <class Type, class Vector>
inline
Vector
from_barycentric (const vector3 <Type> & barycentric,
                  const Vector & point0,
                  const Vector & point1,
                  const Vector & point2)
{
	return point0 * barycentric .x () + point1 * barycentric .y () + point2 * barycentric .z ();
}

/**
 * @returns Computes the greates element of the barycentric axes.
 * @param  barycentric  barycentric vector of triangle.
 */
template <class Type>
inline
vector3 <Type>
barycentric_max (const vector3 <Type> & barycentric)
{
	return vector3 <Type> (barycentric [0] + barycentric [2], barycentric [1] + barycentric [0], barycentric [2] + barycentric [1]);
}

/**
 * @returns Returns the vertices of the triangle an arbitary barycentric point.
 * @param  barycentric  barycentric vector within triangle.
 */
template <class Type>
inline
std::tuple <vector3 <Type>, vector3 <Type>, vector3 <Type>>
barycentric_triangle (const vector3 <Type> & barycentric)
{
	const auto min    = floor (barycentric);
	const auto max    = ceil (barycentric);
	const auto even   = min .x () + min .y () + min .z () == 0;
	auto       A      = even ? vector3 <Type> (max .x (), min .y (), min .z ()) : vector3 <Type> (min .x (), max .y (), max .z ());
	auto       B      = even ? vector3 <Type> (min .x (), max .y (), min .z ()) : vector3 <Type> (max .x (), min .y (), max .z ());
	auto       C      = even ? vector3 <Type> (min .x (), min .y (), max .z ()) : vector3 <Type> (max .x (), max .y (), min .z ());

	if (min .x () == max .x ())
	{
		A .x (1 - A .y () - A .z ());
		B .x (1 - B .y () - B .z ());
		C .x (1 - C .y () - C .z ());
	}

	if (min .y () == max .y ())
	{
		A .y (1 - A .z () - A .x ());
		B .y (1 - B .z () - B .x ());
		C .y (1 - C .z () - C .x ());
	}

	if (min .z () == max .z ())
	{
		A .z (1 - A .x () - A .y ());
		B .z (1 - B .x () - B .y ());
		C .z (1 - C .x () - C .y ());
	}

	return std::tuple (A, B, C);
}

template <class Type, class Vector>
Type
spherical_barycentric_coordinate (const Type x,
                                  const Vector & point0,
                                  const Vector & point1)
{
	if (x == 0)
		return 0;

	const auto a1 = std::acos (std::clamp <Type> (dot (point0, point1), -1, 1));
	const auto a2 = a1 * x;
	const auto s1 = 2 * std::sin (a1 / 2);
	const auto s2 = 2 * std::sin (a2 / 2);
	const auto b1 = std::asin (sin (a1) / s1);
	const auto b2 = std::asin (sin (a2) / s2);

	const auto c1 = pi <Type> - (b2 + b2 - b1);
	const auto s  = s2 / std::sin (c1) * std::sin (b2);

	return s;
}

template <class Type, class Vector>
inline
vector3 <Type>
spherical_barycentric_coordinates (const vector3 <Type> & barycentric,
                                   const Vector & point0,
                                   const Vector & point1,
                                   const Vector & point2)
{
	const auto x = spherical_barycentric_coordinate (barycentric .x (), point0, point1);
	const auto y = spherical_barycentric_coordinate (barycentric .y (), point1, point2);
	const auto z = spherical_barycentric_coordinate (barycentric .z (), point2, point0);

	return vector3 <Type> (x, y, z);
}

} // math
} // titania

#endif
