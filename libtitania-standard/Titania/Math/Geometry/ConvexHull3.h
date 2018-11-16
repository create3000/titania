/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra?e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_MATH_GEOMETRY_CONVEX_HULL3_H__
#define __TITANIA_MATH_GEOMETRY_CONVEX_HULL3_H__

#include "../Numbers/Vector3.h"
#include "../Geometry/Line3.h"
#include "../Geometry/Plane3.h"

#include <algorithm>
#include <array>
#include <limits>
#include <map>
#include <stack>
#include <vector>

#include "../../LOG.h"

namespace titania {
namespace math {

/**
 *  Template to represent a convex hull in 3D space.
 *  This class is implemented using the quick hull algorithm.
 *  http://thomasdiewald.com/blog/?p=1888
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values.
 */
template <class Type>
class convex_hull3
{
public:

	///  @name Construction

	///  Constructs a convex_hull3 from a set of 3D points.
	convex_hull3 (const std::vector <vector3 <Type>> &);

	const std::vector <size_t> &
	indices () const
	{ return m_indices; }
	

private:

	std::vector <size_t>
	create_simplex () const;

	std::vector <std::pair <size_t, size_t>>
	extract_horizon_edges (const std::vector <size_t> &, const std::vector <std::array <size_t, 3>>&) const;

	///  @name Member access

	///  Returns the indices within the set of points of the convex hull in counterclockwise order.
	const std::vector <vector3 <Type>> & m_points;

	std::vector <size_t> m_indices;

};

template <class Type>
convex_hull3 <Type>::convex_hull3 (const std::vector <vector3 <Type>> & points) :
	   m_points (points),
	  m_indices ()
{
	//__LOG__ << "convex_hull3" << std::endl;

	// Create initial simplex (tetrahedron, 4 vertices).

	const auto simplex = create_simplex ();

	// Mark each simplex point as beeing used in a plane to prevent floating point errors when determining the distance
	// to this point as the result will probably be not exactly zero.

	std::vector <bool> m_set (m_points .size ());

	for (const auto & index : simplex)
		m_set [index] = true;

	// Assign points to faces. Each point in the point-list is assigned to the first face the point is in front of
	// (¡°point can see face¡±). So each point is assigned to only one face, and each face contains its own point-set.
	// Points that are behind all faces, are therefore automatically ignored and not used in the further process.

	std::map <size_t, plane3 <Type>> faces = {
		std::pair (0, plane3 <Type> (m_points [simplex [0]], m_points [simplex [1]], m_points [simplex [2]])),
		std::pair (1, plane3 <Type> (m_points [simplex [1]], m_points [simplex [0]], m_points [simplex [3]])),
		std::pair (2, plane3 <Type> (m_points [simplex [0]], m_points [simplex [2]], m_points [simplex [3]])),
		std::pair (3, plane3 <Type> (m_points [simplex [2]], m_points [simplex [1]], m_points [simplex [3]])),
	};

	std::vector <std::array <size_t, 3>> triangles = {
		{ simplex [0], simplex [1], simplex [2] },
		{ simplex [1], simplex [0], simplex [3] },
		{ simplex [0], simplex [2], simplex [3] },
		{ simplex [2], simplex [1], simplex [3] },
	};

	std::vector <std::vector <size_t>> points_to_faces (4);

	for (size_t i = 0, size = m_points .size (); i < size; ++ i)
	{
		if (m_set [i])
			continue;

		for (size_t k = 0; k < 4; ++ k)
		{
			if (faces [k] .distance (m_points [i]) > 0)
			{
				points_to_faces [k] .emplace_back (i);
				break;
			}
		}
	}
	
	//__LOG__ << "points_to_faces" << std::endl;

	//for (const auto & face : points_to_faces)
	//	__LOG__ << face .size () << std::endl;

	// Push the 4 faces on the stack. Faces without points are ignored.

	auto faces_stack = std::vector <size_t> ();

	for (size_t i = 0; i < 4; ++ i)
	{
		if (points_to_faces [i] .empty ())
			continue;

		faces_stack .push_back (i);
	}

	while (not faces_stack .empty ())
	{
		// If Stack is not empty Pop Face from Stack .

		const auto f = faces_stack .back ();

		faces_stack .pop_back ();

		// Check if it has a point-set, otherwise continue next iteration.
		// Although in fact empty faces are not pushed to the stack in the first place.

		auto & point_set = points_to_faces [f];

		if (point_set .empty ())
		{
			point_set .clear ();
			continue;
		}

		// Get most distant point of the face¡¯s point-set.

		const auto & face = faces [f];

		const auto result = std::max_element (point_set .begin (),
                                            point_set .end (),
		                                      [this, &face] (const size_t a, const size_t b)
		                                      { return face .distance (m_points [a]) < face .distance (m_points [b]); });

		const auto   most_distant_point_index = *result;
		const auto & most_distant_point       = m_points [most_distant_point_index];

		m_set [most_distant_point_index] = true;

		//__LOG__ << "most_distant_point " << most_distant_point << std::endl;

		// Find all faces that can be seen from that point. Those faces must be adjacent to the current face. I call them
		// lit-faces in my implementation, and therefore the point can be seen as a point-light. All found lit-faces
		// are labeled as such and also temporarily saved to a heap for later use.

		std::vector <size_t> lit_faces;

		for (const auto & pair : faces)
		{
			const auto   index = pair .first;
			const auto & face  = pair .second;

			//__LOG__ << "distance " << index << " " << face .distance (most_distant_point) << std::endl;

			if (face .distance (most_distant_point) > 0)
				lit_faces .emplace_back (index);
		}

		//for (const auto & i : lit_faces)
		//	__LOG__ << "lit_faces " << i << std::endl;

		// Extract horizon edges of lit-faces and extrude to Point. Clearly there is exactly one closed and convex
		// horizon from the points view that encloses all lit-faces. Now each horizon-segement and the current point
		// build a new triangle. So the horizon is somehow projected to the point. The new faces are build and attached
		// to the mesh (and also temporarly saved to a heap) while iteration through the horizon-edges, which
		// automatically detaches all light-faces.

		const auto horizon_edges = extract_horizon_edges (lit_faces, triangles);

		//for (const auto & edge : horizon_edges)
		//	__LOG__ << "horizon_edges " << edge .first << " " << edge .second << std::endl;

		const auto first = triangles .size ();

		for (const auto & edge : horizon_edges)
		{
			const auto p0 = edge .first;
			const auto p1 = edge .second;
			const auto p2 = most_distant_point_index;
			const auto f  = triangles .size ();

			faces_stack .push_back (f);

			faces .emplace (f, plane3 <Type> (m_points [p0], m_points [p1], m_points [p2]));

			triangles .emplace_back (std::array <size_t, 3> { p0, p1, p2 });

			points_to_faces .emplace_back ();
		}

		// Assign all points off all lit-faces to the new created faces. This is extacly the same procedure as in 1.2.
		// Each point is assigned to the first face it can see. I tried different assigning priorities, but it didn¡¯t help
		// much. But again, points behind all faces, are ignored in the further process.

		for (const auto & k : lit_faces)
		{
			for (const auto & p : points_to_faces [k])
			{
				if (m_set [p])
					continue;

				for (size_t i = first, size = triangles .size (); i < size; ++ i)
				{
					if (faces [i] .distance (m_points [p]) > 0)
					{
						points_to_faces [i] .emplace_back (p);
						break;
					}
				}
			}
		}

		for (const auto & i : lit_faces)
			faces .erase (i);

		// Push newly created faces on the stack, and start at (2.1). Faces without points are ignored.
	}

	for (const auto & pair : faces)
	{
		const auto & triangle = triangles [pair .first];
		const auto   p0       = triangle [0];
		const auto   p1       = triangle [1];
		const auto   p2       = triangle [2];

		m_indices .emplace_back (p0);
		m_indices .emplace_back (p1);
		m_indices .emplace_back (p2);

		//__LOG__ << "triangle " << p0 << " " << p1 << " " << p2 << std::endl;
	}
}

template <class Type>
std::vector <size_t>
convex_hull3 <Type>::create_simplex () const
{
	//__LOG__ << "create_simplex" << std::endl;

	// Create initial simplex (tetrahedron, 4 vertices).

	auto simplex = std::vector <size_t> (4);

	// To do this, the 6 Extreme Points [EP], min/max points in X,Y and Z, of the given pointcloud are extracted.
	
	const auto resultX = std::minmax_element (m_points .begin (),
	                                          m_points .end (),
	                                          [ ] (const vector3 <Type> & a, const vector3 <Type> & b)
                                             { return a .x () < b .x (); });

	const auto resultY = std::minmax_element (m_points .begin (),
	                                          m_points .end (),
	                                          [ ] (const vector3 <Type> & a, const vector3 <Type> & b)
                                             { return a .y () < b .y (); });

	const auto resultZ = std::minmax_element (m_points .begin (),
	                                          m_points .end (),
	                                          [ ] (const vector3 <Type> & a, const vector3 <Type> & b)
                                             { return a .z () < b .z (); });

	const std::vector <std::vector <size_t>::difference_type> extreme_points = {
		resultX .first  - m_points .begin (),
		resultX .second - m_points .begin (),
		resultY .first  - m_points .begin (),
		resultY .second - m_points .begin (),
		resultZ .first  - m_points .begin (),
		resultZ .second - m_points .begin (),
	};

	for (const auto & p : extreme_points)
		__LOG__ << "extreme_points " << p << std::endl;

	// From those 6 EP the two most distant build the base-line of the base triangle.

	{
		Type max_distance = -std::numeric_limits <Type>::infinity ();
	
		for (size_t i = 0; i < 6; ++ i)
		{
			for (size_t k = 0; k < 6; ++ k)
			{
				const auto p0       = extreme_points [i];
				const auto p1       = extreme_points [k];
				const auto distance = abs (m_points [p0] - m_points [p1]);
	
				if (distance > max_distance)
				{
					max_distance = distance;
					simplex [0]  = p0;
					simplex [1]  = p1;
				}
			}
		}
	}

	// The most distant point of EP to the base line is the 3rd point of the base-triangle.

	{
		const auto base_line    = line3 <Type> (m_points [simplex [0]], m_points [simplex [1]], points_type ());
		Type       max_distance = -std::numeric_limits <Type>::infinity ();
	
		for (size_t i = 0; i < 6; ++ i)
		{
			const auto extreme_point = extreme_points [i];
			const auto distance      = base_line .distance (m_points [extreme_point]);
	
			if (distance > max_distance)
			{
				max_distance = distance;
				simplex [2]  = extreme_point;
			}
		}
	}

	// To find the pyramids apex, the most distant point to the base-triangle is searched for in the whole point-list.
	// Now having 4 points, the inital pyramid can easily be created.

	{
		const auto base_plane  = plane3 <Type> (m_points [simplex [0]], m_points [simplex [1]],  m_points [simplex [2]]);
		bool       swap        = false;
		Type       max_distance = -std::numeric_limits <Type>::infinity ();

		//__LOG__ << base_plane .normal () << std::endl;

		for (size_t i = 0, size = m_points .size (); i < size; ++ i)
		{
			const auto distance     = base_plane .distance (m_points [i]);
			const auto abs_distance = std::abs (distance);

			//__LOG__ << "distance " << distance << std::endl;
	
			if (abs_distance > max_distance)
			{
				max_distance = abs_distance;
				swap         = distance > 0;
				simplex [3]  = i;
			}
		}

		// Ensure base triangle is in counter clockwise order.

		if (swap)
			std::swap (simplex [0], simplex [1]);
	}

	__LOG__ << simplex [0] << std::endl;
	__LOG__ << simplex [1] << std::endl;
	__LOG__ << simplex [2] << std::endl;
	__LOG__ << simplex [3] << std::endl;

	return simplex;
}

template <class Type>
std::vector <std::pair <size_t, size_t>>
convex_hull3 <Type>::extract_horizon_edges (const std::vector <size_t> & lit_faces, const std::vector <std::array <size_t, 3>> & triangles) const
{
	//__LOG__ << "extract_horizon_edges" << std::endl;

	// Horizon edges are all edges who belong only to one lit face.

	std::vector <std::pair <size_t, size_t>> horizon_edges;

	std::map <std::pair <size_t, size_t>, size_t> index;

	for (const auto & f : lit_faces)
	{
		const auto & triangle = triangles [f];
		const auto   p0       = triangle [0];
		const auto   p1       = triangle [1];
		const auto   p2       = triangle [2];

		++ index [p0 < p1 ? std::pair (p0, p1) : std::pair (p1, p0)];
		++ index [p1 < p2 ? std::pair (p1, p2) : std::pair (p2, p1)];
		++ index [p2 < p0 ? std::pair (p2, p0) : std::pair (p0, p2)];
	}

	for (const auto & f : lit_faces)
	{
		const auto & triangle = triangles [f];
		const auto   p0       = triangle [0];
		const auto   p1       = triangle [1];
		const auto   p2       = triangle [2];

		if (index [p0 < p1 ? std::pair (p0, p1) : std::pair (p1, p0)] == 1)
		{
			horizon_edges .emplace_back (p0, p1);
		}

		if (index [p1 < p2 ? std::pair (p1, p2) : std::pair (p2, p1)] == 1)
		{
			horizon_edges .emplace_back (p1, p2);
		}

		if (index [p2 < p0 ? std::pair (p2, p0) : std::pair (p0, p2)] == 1)
		{
			horizon_edges .emplace_back (p2, p0);
		}
	}

	return horizon_edges;
}

extern template class convex_hull3 <float>;
extern template class convex_hull3 <double>;
extern template class convex_hull3 <long double>;

} // math
} // titania

#endif
