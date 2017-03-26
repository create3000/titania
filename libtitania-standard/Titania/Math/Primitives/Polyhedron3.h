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

#ifndef __TITANIA_MATH_PRIMITIVES_TRIANGLE_SPHERE3_H__
#define __TITANIA_MATH_PRIMITIVES_TRIANGLE_SPHERE3_H__

#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"
#include "../Numbers/Rotation4.h"

#include <map>
#include <tuple>
#include <vector>

namespace titania {
namespace math {

template <class Type>
class basic_polyhedron3
{
public:

	///  @name Member access

	size_t
	order () const
	{ return m_order - 1; }

	const std::vector <int32_t> &
	coord_index () const
	{ return m_coord_index; }

	const std::vector <vector3 <Type>> &
	points () const
	{ return m_points; }

	const std::vector <int32_t> &
	tex_coord_index () const
	{ return m_tex_coord_index; }

	const std::vector <vector2 <Type>> &
	tex_points () const
	{ return m_tex_points; }

	///  @name Destruction

	virtual
	~basic_polyhedron3 () = default;


protected:

	///  @name Construction

	basic_polyhedron3 (const size_t order);

	///  @name Operations

	void
	add_point (const vector3 <Type> & point);

	void
	add_triangle (const int32_t i1, const int32_t i2, const int32_t i3)
	{ add_triangle (i1, i2, i3, m_coord_index); }

	std::vector <vector3 <Type>> &
	get_points ()
	{ return m_points; }

	void
	create_triangles ();

	void
	create_tex_coord ();


private:

	int32_t
	add_point (const vector3 <Type> & point, const int32_t p0, const int32_t p1, const int32_t p2, const int32_t x, const int32_t y, const int32_t z);

	void
	add_triangle (const int32_t i1, const int32_t i2, const int32_t i3, std::vector <int32_t> & m_coord_index);

	int32_t
	resolve_overlap (const int32_t i0, const int32_t i1);

	///  @name Static members

	static constexpr Type pole_threshold    = 0.001;
	static constexpr Type overlap_threshold = 0.5;

	///  @name Members

	using Key     = std::tuple <int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>;
	using EdgeKey = std::pair <std::pair <int32_t, int32_t>, int32_t>;

	size_t                         m_order;
	std::vector <int32_t>          m_coord_index;
	std::vector <vector3 <Type>>   m_simplex;
	std::vector <vector3 <Type>>   m_points;
	std::map <Key, int32_t>        m_point_cache;
	std::map <EdgeKey, int32_t>    m_edge_point_cache;
	std::map <int32_t, int32_t>    m_vertex_point_cache;
	std::vector <int32_t>          m_tex_coord_index;
	std::vector <vector2 <Type>>   m_tex_points;

};

template <class Type>
basic_polyhedron3 <Type>::basic_polyhedron3 (const size_t order) :
	             m_order (order + 1),
	       m_coord_index (),
	           m_simplex (),
	            m_points (),
	       m_point_cache (),
	  m_edge_point_cache (),
	m_vertex_point_cache ()
{ }

template <class Type>
void
basic_polyhedron3 <Type>::add_point (const vector3 <Type> & point)
{
	m_simplex .emplace_back (normalize (point));
}

template <class Type>
int32_t
basic_polyhedron3 <Type>::add_point (const vector3 <Type> & point, const int32_t p0, const int32_t p1, const int32_t p2, const int32_t x, const int32_t y, const int32_t z)
{
	if (x == 0 or y == 0 or z == 0)
	{
		std::array <int32_t, 3> indices = { p0, p1, p2 };
		std::array <int32_t, 3> axes    = { x, y, z };

		const auto one = std::find (axes .begin (), axes .end (), m_order);

		if (one not_eq axes .end ())
		{
			const auto i0  = one - axes .begin ();
			const auto key = indices [i0];
			
			const auto iter = m_vertex_point_cache .find (key);
	
			if (iter not_eq m_vertex_point_cache .end ())
				return iter -> second;
			
			const auto index = m_points .size ();

			m_vertex_point_cache .emplace (key, index);
			m_points .emplace_back (normalize (point));
	
			return index;
		}

		const auto zero   = std::find (axes .begin (), axes .end (), 0);
		const auto i0     = (zero - axes .begin () + 1) % 3;
		const auto i1     = (zero - axes .begin () + 2) % 3;
		const auto index0 = indices [i0];
		const auto index1 = indices [i1];
		const auto weight = index0 < index1 ? axes [i0] : m_order - axes [i0];
		const auto key    = std::make_pair (index0 < index1 ? std::make_pair (index0, index1) : std::make_pair (index1, index0), weight);
		
		const auto iter = m_edge_point_cache .find (key);

		if (iter not_eq m_edge_point_cache .end ())
			return iter -> second;
		
		const auto index = m_points .size ();
	
		m_edge_point_cache .emplace (key, index);
		m_points .emplace_back (normalize (point));

		return index;
	}

	const auto key  = std::make_tuple (p0, p1, p2, x, y, z);
	const auto iter = m_point_cache .find (key);

	if (iter not_eq m_point_cache .end ())
		return iter -> second;

	const auto index = m_points .size ();

	m_point_cache .emplace (key, index);
	m_points .emplace_back (normalize (point));

	return index;
}

template <class Type>
void
basic_polyhedron3 <Type>::add_triangle (const int32_t i1, const int32_t i2, const int32_t i3, std::vector <int32_t> & m_coord_index)
{
	m_coord_index .emplace_back (i1);
	m_coord_index .emplace_back (i2);
	m_coord_index .emplace_back (i3);
}

template <class Type>
void
basic_polyhedron3 <Type>::create_triangles ()
{
	auto m_coord_index2 = std::vector <int32_t> ();

	const Type order  = m_order;
	const auto yOrder = int32_t (m_order);

	for (size_t i = 0, size = m_coord_index .size (); i < size; i += 3)
	{
		// Indices
		const auto p0 = m_coord_index [i + 0];
		const auto p1 = m_coord_index [i + 1];
		const auto p2 = m_coord_index [i + 2];

		// Points
		const auto & point0 = m_simplex [p0];
		const auto & point1 = m_simplex [p1];
		const auto & point2 = m_simplex [p2];

		// Angle between points
		const auto angle0 = std::acos (clamp <Type> (dot (point0, point1), -1, 1));
		const auto angle1 = std::acos (clamp <Type> (dot (point1, point2), -1, 1));
		const auto angle2 = std::acos (clamp <Type> (dot (point2, point0), -1, 1));

		// Length of triangle edges
		const auto length0 = distance (point1, point0);
		const auto length1 = distance (point2, point1);
		const auto length2 = distance (point0, point2);

		// Angle between point and corresponding edge
		const auto beta0 = pi <Type> - std::acos (clamp <Type> (dot (normalize (point0 - point1), point0), -1, 1));
		const auto beta1 = pi <Type> - std::acos (clamp <Type> (dot (normalize (point1 - point2), point1), -1, 1));
		const auto beta2 = pi <Type> - std::acos (clamp <Type> (dot (normalize (point2 - point0), point2), -1, 1));

		for (int32_t y = 0; y < yOrder; ++ y)
		{
			const auto xOrder = yOrder - y;

			for (int32_t x = 0; x < xOrder; ++ x)
			{
				// Interpolate triangles over barycentric coordinates.

				const auto x0 = x + 0;
				const auto y0 = y + 0;
				const auto x1 = x + 1;
				const auto y1 = y + 1;
				const auto x2 = x - 1;
				const auto z0 = m_order - x0 - y0;
				const auto z1 = m_order - x1 - y0;
				const auto z2 = m_order - x0 - y1;
				const auto z3 = m_order - x2 - y1;

				const auto u0 = x0 / order;
				const auto v0 = y0 / order;
				const auto u1 = x1 / order;
				const auto v1 = y1 / order;
				const auto u2 = x2 / order;
				const auto t0 = z0 / order;
				const auto t1 = z1 / order;
				const auto t2 = z2 / order;
				const auto t3 = z3 / order;

				const auto alphaU0 = angle0 * u0;
				const auto alphaU1 = angle0 * u1;
				const auto alphaU2 = angle0 * u2;
				const auto alphaV0 = angle1 * v0;
				const auto alphaV1 = angle1 * v1;
				const auto alphaT0 = angle2 * t0;
				const auto alphaT1 = angle2 * t1;
				const auto alphaT2 = angle2 * t2;
				const auto alphaT3 = angle2 * t3;

				// Law of sines
				const auto a0 = std::sin (alphaU0) / std::sin (beta0 - alphaU0) / length0;
				const auto a1 = std::sin (alphaU1) / std::sin (beta0 - alphaU1) / length0;
				const auto a2 = std::sin (alphaU2) / std::sin (beta0 - alphaU2) / length0;
				const auto b0 = std::sin (alphaV0) / std::sin (beta1 - alphaV0) / length1;
				const auto b1 = std::sin (alphaV1) / std::sin (beta1 - alphaV1) / length1;
				const auto c0 = std::sin (alphaT0) / std::sin (beta2 - alphaT0) / length2;
				const auto c1 = std::sin (alphaT1) / std::sin (beta2 - alphaT1) / length2;
				const auto c2 = std::sin (alphaT2) / std::sin (beta2 - alphaT2) / length2;
				const auto c3 = std::sin (alphaT3) / std::sin (beta2 - alphaT3) / length2;

				const auto index0 = add_point (a0 * point0 + b0 * point1 + c0 * point2, p0, p1, p2, x0, y0, z0);
				const auto index1 = add_point (a1 * point0 + b0 * point1 + c1 * point2, p0, p1, p2, x1, y0, z1);
				const auto index2 = add_point (a0 * point0 + b1 * point1 + c2 * point2, p0, p1, p2, x0, y1, z2);

				add_triangle (index0, index1, index2, m_coord_index2);

				if (x2 >= 0)
				{
					const auto index3 = add_point (a2 * point0 + b1 * point1 + c3 * point2, p0, p1, p2, x2, y1, z3);

					add_triangle (index0, index2, index3, m_coord_index2);
				}
			}
		}
	}

	std::swap (m_coord_index, m_coord_index2);
}

template <class Type>
void
basic_polyhedron3 <Type>::create_tex_coord ()
{
	//
	// Create texture coordinates
	// Apply spherical mapping
	//

	// Copy coordIndex
	m_tex_coord_index = m_coord_index;

	for (const auto & point : m_points)
	{
		m_tex_points .emplace_back (std::atan2 (point .x (), point .z ()) / (2 * pi <Type>) + Type (0.5),
		                            std::asin (clamp <Type> (point .y (), -1, 1)) / pi <Type> + Type (0.5));
	}

	// Refine poles
	static constexpr auto north_pole_threshold = 1 - pole_threshold;
	static constexpr auto south_pole_threshold = pole_threshold;

	for (size_t i = 0, size = m_tex_coord_index .size (); i < size; i += 3)
	{
		int32_t i0 = -1;
		int32_t i1 = -1;
		int32_t i2 = -1;

		// Find north pole

		if (m_tex_points [m_tex_coord_index [i]] .y () > north_pole_threshold)
		{
			i0 = i;
			i1 = i + 1;
			i2 = i + 2;
		}

		else if (m_tex_points [m_tex_coord_index [i + 1]] .y () > north_pole_threshold)
		{
			i0 = i + 1;
			i1 = i;
			i2 = i + 2;
		}

		else if (m_tex_points [m_tex_coord_index [i + 2]] .y () > north_pole_threshold)
		{
			i0 = i + 2;
			i1 = i;
			i2 = i + 1;
		}

		// North pole found
		if (i0 > -1)
		{
			const auto index0 = m_tex_coord_index [i0]; // North pole
			const auto index1 = m_tex_coord_index [i1];
			const auto index2 = resolve_overlap (i1, i2);

			m_tex_coord_index [i0] = m_tex_points .size ();

			m_tex_points .emplace_back ((m_tex_points [index1] .x () + m_tex_points [index2] .x ()) / 2, m_tex_points [index0] .y ());
			continue;
		}

		// Find south pole

		if (m_tex_points [m_tex_coord_index [i]] .y () < south_pole_threshold)
		{
			i0 = i;
			i1 = i + 1;
			i2 = i + 2;
		}

		else if (m_tex_points [m_tex_coord_index [i + 1]] .y () < south_pole_threshold)
		{
			i0 = i + 1;
			i1 = i;
			i2 = i + 2;
		}

		else if (m_tex_points [m_tex_coord_index [i + 2]] .y () < south_pole_threshold)
		{
			i0 = i + 2;
			i1 = i;
			i2 = i + 1;
		}

		// South pole found
		if (i0 > -1)
		{
			const auto index0 = m_tex_coord_index [i0]; // South pole
			const auto index1 = m_tex_coord_index [i1];
			const auto index2 = resolve_overlap (i1, i2);

			m_tex_coord_index [i0] = m_tex_points .size ();

			m_tex_points .emplace_back ((m_tex_points [index1] .x () + m_tex_points [index2] .x ()) / 2, m_tex_points [index0] .y ());
			continue;
		}

		resolve_overlap (i, i + 1);
		resolve_overlap (i, i + 2);
	}
}

template <class Type>
int32_t
basic_polyhedron3 <Type>::resolve_overlap (const int32_t i0, const int32_t i1)
{
	const auto index1   = m_tex_coord_index [i1];
	const auto distance = m_tex_points [m_tex_coord_index [i0]] .x () - m_tex_points [index1] .x ();

	if (distance > overlap_threshold)
	{
		m_tex_coord_index [i1] = m_tex_points .size ();

		m_tex_points .emplace_back (m_tex_points [index1] .x () + 1, m_tex_points [index1] .y ());
	}
	else if (distance < -overlap_threshold)
	{
		m_tex_coord_index [i1] = m_tex_points .size ();

		m_tex_points .emplace_back (m_tex_points [index1] .x () - 1, m_tex_points [index1] .y ());
	}

	return m_tex_coord_index [i1];
}

template <class Type>
class octahedron3 :
	public basic_polyhedron3 <Type>
{
public:

	///  @name Construction

	octahedron3 (const size_t order);

	///  @name Destruction

	virtual
	~octahedron3 () final override
	{ }


private:

	///  @name Operations

	void
	create_primitive ();

};

template <class Type>
octahedron3 <Type>::octahedron3 (const size_t order) :
	basic_polyhedron3 <Type> (order)
{
	this -> create_primitive ();
	this -> create_triangles ();
	this -> create_tex_coord ();
}

template <class Type>
void
octahedron3 <Type>::create_primitive ()
{
	this -> add_point (vector3 <Type> ( 0,  1,  0));

	this -> add_point (vector3 <Type> ( 0,  0,  1));
	this -> add_point (vector3 <Type> ( 1,  0,  0));
	this -> add_point (vector3 <Type> ( 0,  0, -1));
	this -> add_point (vector3 <Type> (-1,  0,  0));

	this -> add_point (vector3 <Type> ( 0, -1,  0));
	
	// 8 faces
	this -> add_triangle (0, 1, 2);
	this -> add_triangle (0, 2, 3);
	this -> add_triangle (0, 3, 4);
	this -> add_triangle (0, 4, 1);
	
	this -> add_triangle (5, 2, 1);
	this -> add_triangle (5, 3, 2);
	this -> add_triangle (5, 4, 3);
	this -> add_triangle (5, 1, 4);
}

template <class Type>
class icosahedron3 :
	public basic_polyhedron3 <Type>
{
public:

	///  @name Construction

	icosahedron3 (const size_t order);

	///  @name Destruction

	virtual
	~icosahedron3 () final override
	{ }


private:

	///  @name Operations

	void
	create_primitive ();

};

template <class Type>
icosahedron3 <Type>::icosahedron3 (const size_t order) :
	basic_polyhedron3 <Type> (order)
{
	this -> create_primitive ();
	this -> create_triangles ();
	this -> create_tex_coord ();
}

template <class Type>
void
icosahedron3 <Type>::create_primitive ()
{
	// Create 12 vertices of a icosahedron

	static constexpr auto p = phi <Type>;

	this -> add_point (vector3 <Type> (-1,  p,  0));
	this -> add_point (vector3 <Type> ( 1,  p,  0));
	this -> add_point (vector3 <Type> (-1, -p,  0));
	this -> add_point (vector3 <Type> ( 1, -p,  0));

	this -> add_point (vector3 <Type> (0, -1,  p));
	this -> add_point (vector3 <Type> (0,  1,  p));
	this -> add_point (vector3 <Type> (0, -1, -p));
	this -> add_point (vector3 <Type> (0,  1, -p));

	this -> add_point (vector3 <Type> ( p,  0, -1));
	this -> add_point (vector3 <Type> ( p,  0,  1));
	this -> add_point (vector3 <Type> (-p,  0, -1));
	this -> add_point (vector3 <Type> (-p,  0,  1));

	// Rotate point thus a vertice is a pole

	const auto rotation = rotation4 <Type> (0, 0, 1, std::atan (1 / p)) * rotation4 <Type> (0, 1, 0, -pi <Type> / 10);

	for (auto & point : this -> get_points ())
		point = normalize (rotation .mult_vec_rot (point));

	// 5 faces around point 0
	this -> add_triangle ( 0, 11,  5);
	this -> add_triangle ( 0,  5,  1);
	this -> add_triangle ( 0,  1,  7);
	this -> add_triangle ( 0,  7, 10);
	this -> add_triangle ( 0, 10, 11);

	// 5 adjacent faces
	this -> add_triangle ( 1,  5,  9);
	this -> add_triangle ( 5, 11,  4);
	this -> add_triangle (11, 10,  2);
	this -> add_triangle (10,  7,  6);
	this -> add_triangle ( 7,  1,  8);

	// 5 faces around point 3
	this -> add_triangle ( 3,  9,  4);
	this -> add_triangle ( 3,  4,  2);
	this -> add_triangle ( 3,  2,  6);
	this -> add_triangle ( 3,  6,  8);
	this -> add_triangle ( 3,  8,  9);

	// 5 adjacent faces
	this -> add_triangle ( 4,  9,  5);
	this -> add_triangle ( 2,  4, 11);
	this -> add_triangle ( 6,  2, 10);
	this -> add_triangle ( 8,  6,  7);
	this -> add_triangle ( 9,  8,  1);
}

} // math
} // titania

#endif
