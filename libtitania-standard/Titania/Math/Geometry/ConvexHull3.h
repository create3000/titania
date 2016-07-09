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

#ifndef __TITANIA_MATH_GEOMETRY_CONVEX_HULL3_H__
#define __TITANIA_MATH_GEOMETRY_CONVEX_HULL3_H__

#include "../Numbers/Vector3.h"
#include "../Geometry/Plane3.h"

#include <map>
#include <vector>

namespace titania {
namespace math {

template <class Type>
class convex_hull3
{
public:

	struct face_t
	{
		face_t (const size_t, const size_t, const size_t, const size_t, const std::vector <vector3 <Type>> &);

		size_t indices [3];
		plane3 <Type> plane;
	};

	convex_hull3 (const std::vector <vector3 <Type>> &);

	const std::vector <face_t> &
	faces () const
	{ return m_faces; }

private:

	struct edge_t
	{
		edge_t () :
			a (-1),
			b (-1)
		{ }

		void
		insert (size_t x)
		{ (a == EMPTY ? a : b) = x; }

		bool
		contains (size_t x)
		{ return a == x or b == x; }

		void
		erase (size_t x)
		{ (a == x ? a : b) = EMPTY; }

		size_t
		size ()
		{ return (a not_eq EMPTY) + (b not_eq EMPTY); }

		static constexpr size_t EMPTY = -1;

		size_t a, b;
	};

	std::vector <face_t> m_faces;

};

template <class Type>
convex_hull3 <Type>::face_t::face_t (const size_t i, const size_t j, const size_t k, const size_t inside, const std::vector <vector3 <Type>> & points) :
	indices { i, j, k },
	  plane (points [i], points [j], points [k])
{
	if (plane .distance (points [inside]) >= 0)
		plane = plane3 <Type> (points [k], points [j], points [i]);
}

template <class Type>
convex_hull3 <Type>::convex_hull3 (const std::vector <vector3 <Type>> & points) :
	m_faces ()
{
	std::vector <std::map <size_t, edge_t>>  edges (points .size ());

	// Initially construct the hull as containing only the first four points.
	for (size_t i = 0; i < 4; ++ i)
	{
		for (size_t j = i + 1; j < 4; ++ j)
		{
			for (size_t k = j + 1; k < 4; ++ k)
			{
				edges [j] [k] .insert (i);
				edges [i] [k] .insert (j);
				edges [i] [j] .insert (k);

				m_faces .emplace_back (i, j, k, 6 - i - j - k, points);
			}
		}
	}

	__LOG__ << m_faces .size () << std::endl;

	// Now add a point into the hull one at a time.
	for (size_t i = 4; i < points .size (); ++ i)
	{
		// Find and delete all faces with their outside 'illuminated' by this point.
		for (size_t j = 0; j < m_faces .size (); ++ j)
		{
			const auto face = m_faces [j];

			if (face .plane .distance (points [i]) >= 0)
			{
				edges [face .indices [0]] [face .indices [1]] .erase (face .indices [2]);
				edges [face .indices [0]] [face .indices [2]] .erase (face .indices [1]);
				edges [face .indices [1]] [face .indices [2]] .erase (face .indices [0]);

				m_faces [j --] = m_faces .back ();

				m_faces .pop_back ();
			}
		}

		__LOG__ << m_faces .size () << std::endl;

		// Now for any edge still in the hull that is only part of one face
		// add another face containing the new point and that edge to the hull.
		size_t numFaces = m_faces .size ();

		for (size_t j = 0; j < numFaces; ++ j)
		{
			const auto face = m_faces [j];

			for (size_t a = 0; a < 3; ++ a)
			{
				for (size_t b = a + 1; b < 3; ++ b)
				{
					const size_t c = 3 - a - b;

					const auto x = face .indices [a];
					const auto y = face .indices [b];
					const auto z = i;

					__LOG__ << edges [x] [y] .size () << std::endl;

					if (edges [x] [y] .size () == 2)
						continue;

					edges [y] [z] .insert (x);
					edges [x] [z] .insert (y);
					edges [x] [y] .insert (z);

					m_faces .emplace_back (x, y, z, face .indices [c], points);

					__LOG__ << m_faces .size () << std::endl;
				}
			}
		}
	}
}

} // math
} // titania

#endif
