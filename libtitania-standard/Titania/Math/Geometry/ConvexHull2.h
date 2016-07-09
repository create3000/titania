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
 * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
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

#ifndef __TITANIA_MATH_GEOMETRY_CONVEX_HULL2_H__
#define __TITANIA_MATH_GEOMETRY_CONVEX_HULL2_H__

#include "../Geometry/Plane3.h"
#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"

#include <algorithm>
#include <limits>
#include <map>
#include <vector>

namespace titania {
namespace math {

template <class Type>
class convex_hull2
{
public:

	///  @name Construction

	convex_hull2 (const std::vector <vector2 <Type>> &);

	///  @name Member access

	const std::vector <size_t> &
	indices () const
	{ return m_indices; }


private:

	///  @name Operations

	void
	quick_hull (const std::vector <vector2 <Type>> & points, const size_t, const size_t);

	///  @name Member types

	std::vector <size_t> m_indices;
	std::vector <bool>   m_set;

};

template <class Type>
convex_hull2 <Type>::convex_hull2 (const std::vector <vector2 <Type>> & points) :
	m_indices (),
	    m_set (points .size ())
{
	// Find most left and most right point to have a start condition.

	const auto result = std::minmax_element (points .begin (),
	                                         points .end (),
	                                         [ ] (const vector2 <Type> & a, const vector2 <Type> & b)
                                            { return a .x () < b .x (); });

	m_indices .emplace_back (result .first  - points .begin ());
	m_indices .emplace_back (result .second - points .begin ());

	m_set [m_indices [0]] = true;
	m_set [m_indices [1]] = true;

	quick_hull (points, 0, 1);
	quick_hull (points, 1, 0);
}

template <class Type>
void
convex_hull2 <Type>::quick_hull (const std::vector <vector2 <Type>> & points,
                                 const size_t i0,
                                 const size_t i1)
{
	// Construct plane from points for distance determination.

	const auto & p0 = points [m_indices [i0]];
	const auto & p1 = points [m_indices [i1]];

	const auto p03 = vector3 <Type> (p0 .x (), p0 .y (), 0);
	const auto p13 = vector3 <Type> (p1 .x (), p1 .y (), 0);
	const auto n3  = normalize (cross (vector3 <Type> (0, 0, 1), p13 - p03));

	const auto plane = plane3 <Type> (p03, n3);

	// Find most distant point to plane in set.

	auto   dMax = -std::numeric_limits <Type>::infinity ();
	size_t iMax = -1;

	for (size_t i = 0, size = points .size (); i < size; ++ i)
	{
		if (m_set [i])
			continue;

		const auto & p = points [i];
		const auto   d = plane .distance (vector3 <Type> (p .x (), p .y (), 0));

		if (d > dMax)
		{
			dMax = d;
			iMax = i;
		}
	}

	if (dMax <= 0)
		return;

	m_set [iMax] = true;

//	const auto result = std::max_element (points .begin (),
//	                                      points .end (),
//	                                      [&plane] (const vector2 <Type> & a, const vector2 <Type> & b)
//                                         {
//		                                       const auto d1 = plane .distance (vector3 <Type> (a .x (), a .y (), 0));
//		                                       const auto d2 = plane .distance (vector3 <Type> (b .x (), b .y (), 0));
//
//	                                          return d1 < d2;
//	                                      });
//
//	const size_t iMax = result - points .begin ();
//
//	if (m_set [iMax])
//		return;
//
//	m_set [iMax] = true;
//
//	const auto & p = *result;
//	const auto   d = plane .distance (vector3 <Type> (p .x (), p .y (), 0));
//
//	if (d <= 0)
//		return;

	// Insert most distant point, and recurse. 

	if (i0 < i1)
	{
		m_indices .insert (m_indices .begin () + (i1 + 1), iMax);

		quick_hull (points, i0,     i1 + 1);
		quick_hull (points, i1 + 1, i1);
	}
	else
	{
		m_indices .insert (m_indices .begin () + i0, iMax);

		quick_hull (points, i0 + 1, i0);
		quick_hull (points, i0,     i1);
	}
}

extern template class convex_hull2 <float>;
extern template class convex_hull2 <double>;

} // math
} // titania

#endif
