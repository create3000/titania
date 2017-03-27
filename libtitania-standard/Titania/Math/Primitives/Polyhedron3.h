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
	dimension () const
	{ return m_dimension; }

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

	basic_polyhedron3 (const size_t dimension);

	///  @name Operations

	void
	add_point (const vector3 <Type> & point);

	void
	add_triangle (const int32_t i1, const int32_t i2, const int32_t i3)
	{ add_triangle (i1, i2, i3, m_coord_index); }

	std::vector <vector3 <Type>> &
	get_simplex ()
	{ return m_simplex; }

	void
	create_triangles ();

	void
	create_tex_coord ();


private:

	int32_t
	add_point (const vector3 <Type> & point, const int32_t p0, const int32_t p1, const int32_t p2, const int32_t x, const int32_t y, const int32_t z);

	const vector3 <Type> &
	get_edge_point (const int32_t p0, const int32_t p1, const int32_t p2, const int32_t x, const int32_t y, const int32_t z);

	void
	add_triangle (const int32_t i1, const int32_t i2, const int32_t i3, std::vector <int32_t> & m_coord_index);

	vector3 <Type>
	create_point (const vector3 <Type> & U0,
	              const vector3 <Type> & U1,
	              const vector3 <Type> & V0,
	              const vector3 <Type> & V1,
	              const vector3 <Type> & T0,
	              const vector3 <Type> & T1,
	              const int32_t x,
	              const int32_t y,
	              const int32_t z,
	              const int32_t xDimension,
	              const int32_t yDimension,
	              const int32_t zDimension);

	int32_t
	resolve_overlap (const int32_t i0, const int32_t i1);

	///  @name Static members

	static constexpr Type pole_threshold    = 0.001;
	static constexpr Type overlap_threshold = 0.5;

	///  @name Members

	using Key     = std::tuple <int32_t, int32_t, int32_t, int32_t, int32_t, int32_t>;
	using EdgeKey = std::pair <std::pair <int32_t, int32_t>, int32_t>;

	size_t                         m_dimension;
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
basic_polyhedron3 <Type>::basic_polyhedron3 (const size_t dimension) :
	         m_dimension (std::max (1UL, dimension)),
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

		const auto one = std::find (axes .begin (), axes .end (), m_dimension);

		if (one not_eq axes .end ())
		{
			const auto i0   = one - axes .begin ();
			const auto key  = indices [i0];
			const auto pair = m_vertex_point_cache .emplace (key, m_points .size ());

			if (pair .second)
				m_points .emplace_back (normalize (point));

			return pair .first -> second;
		}

		const auto zero   = std::find (axes .begin (), axes .end (), 0);
		const auto i0     = (zero - axes .begin () + 1) % axes .size ();
		const auto i1     = (zero - axes .begin () + 2) % axes .size ();
		const auto index0 = indices [i0];
		const auto index1 = indices [i1];
		const auto weight = index0 < index1 ? axes [i0] : m_dimension - axes [i0];
		const auto key    = std::make_pair (index0 < index1 ? std::make_pair (index0, index1) : std::make_pair (index1, index0), weight);
		const auto pair   = m_edge_point_cache .emplace (key, m_points .size ());

		if (pair .second)
			m_points .emplace_back (normalize (point));

		return pair .first -> second;
	}

	const auto key  = std::make_tuple (p0, p1, p2, x, y, z);
	const auto pair = m_point_cache .emplace (key, m_points .size ());

	if (pair .second)
		m_points .emplace_back (normalize (point));

	return pair .first -> second;
}

template <class Type>
const vector3 <Type> &
basic_polyhedron3 <Type>::get_edge_point (const int32_t p0, const int32_t p1, const int32_t p2, const int32_t x, const int32_t y, const int32_t z)
{
	return m_points [add_point (vector3 <Type> (), p0, p1, p2, x, y, z)];
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

	const auto yDimension = int32_t (m_dimension);

	for (size_t i = 1 * 3, size = m_coord_index .size (); i < size; i += 3)
	{
		// Indices
		const auto p0 = m_coord_index [i + 0];
		const auto p1 = m_coord_index [i + 1];
		const auto p2 = m_coord_index [i + 2];

		// Points
		const auto & point0 = m_simplex [p0];
		const auto & point1 = m_simplex [p1];
		const auto & point2 = m_simplex [p2];

		const auto distances = vector3 <Type> (distance (point0, point1), distance (point1, point2), distance (point2, point0));

__LOG__ << distances << std::endl;
__LOG__ << std::endl;

		// Add vertices
		add_point (point0, p0, p1, p2, m_dimension, 0, 0);
		add_point (point1, p0, p1, p2, 0, m_dimension, 0);
		add_point (point2, p0, p1, p2, 0, 0, m_dimension);

		// Add edge points
		for (int32_t e = 1; e < yDimension; ++ e)
		{
			const auto d = m_dimension - e;

         // If the point is normalized by add point, dividing by edge distances after create_point can be left away if all three distances are equal.
			const auto bu = create_point (point2, point0, point0, point1, point1, point2, 0, d, e, m_dimension, m_dimension, m_dimension) / distances;
			const auto bv = create_point (point2, point0, point0, point1, point1, point2, e, 0, d, m_dimension, m_dimension, m_dimension) / distances;
			const auto bt = create_point (point2, point0, point0, point1, point1, point2, d, e, 0, m_dimension, m_dimension, m_dimension) / distances;

			add_point (point0 * bu .x () + point1 * bu .y () + point2 * bu .z (), p0, p1, p2, 0, d, e);
			add_point (point0 * bv .x () + point1 * bv .y () + point2 * bv .z (), p0, p1, p2, e, 0, d);
			add_point (point0 * bt .x () + point1 * bt .y () + point2 * bt .z (), p0, p1, p2, d, e, 0);
		}

__LOG__ << m_points .size () << std::endl;

		for (int32_t y = 0; y < yDimension; ++ y)
		{
__LOG__ << std::endl;

			const auto xDimension = yDimension - y;

			for (int32_t x = 0; x < xDimension; ++ x)
			{
__LOG__ << std::endl;

				// Interpolate triangles over barycentric coordinates.
				const auto x0 = x + 0;
				const auto y0 = y + 0;
				const auto x1 = x + 1;
				const auto y1 = y + 1;
				const auto x2 = x - 1;
				const auto z0 = m_dimension - x0 - y0;
				const auto z1 = m_dimension - x1 - y0;
				const auto z2 = m_dimension - x0 - y1;
				const auto z3 = m_dimension - x2 - y1;

				// Get u edge points.
				const auto eu00 = get_edge_point (p0, p1, p2, 0,              y0, xDimension);
				const auto eu01 = get_edge_point (p0, p1, p2, xDimension,     y0, 0);
				const auto eu10 = get_edge_point (p0, p1, p2, 0,              y1, xDimension - 1);
				const auto eu11 = get_edge_point (p0, p1, p2, xDimension - 1, y1, 0);

				// Get v edge points.
				const auto ev00 = get_edge_point (p0, p1, p2, m_dimension - z0, 0, z0);
				const auto ev01 = get_edge_point (p0, p1, p2, 0, m_dimension - z0, z0);
				const auto ev10 = get_edge_point (p0, p1, p2, m_dimension - z1, 0, z1);
				const auto ev11 = get_edge_point (p0, p1, p2, 0, m_dimension - z1, z1);

				// Get t edge points.
				const auto et00 = get_edge_point (p0, p1, p2, x0, m_dimension - x0, 0);
				const auto et01 = get_edge_point (p0, p1, p2, x0, 0, m_dimension - x0);
				const auto et10 = get_edge_point (p0, p1, p2, x1, m_dimension - x1, 0);
				const auto et11 = get_edge_point (p0, p1, p2, x1, 0, m_dimension - x1);

				// Barycentric coordinates for first three points of quad
				const auto b0 = create_point (eu00, eu01, ev00, ev01, et00, et01, x0, y0, z0, xDimension,     m_dimension - z0, m_dimension - x0) / distances;
				const auto b1 = create_point (eu00, eu01, ev10, ev11, et10, et11, x1, y0, z1, xDimension,     m_dimension - z1, m_dimension - x1) / distances;
				const auto b2 = create_point (eu10, eu11, ev10, ev11, et00, et01, x0, y1, z2, xDimension - 1, m_dimension - z1, m_dimension - x0) / distances;

				//const auto b2 = create_point (eu10, eu11, point0, point1, point1, point2, x0, y1, z2, xDimension - 1, m_dimension, m_dimension) / distances;

				//const auto b0 = create_point (point2, point0, point0, point1, point1, point2, x0, y0, z0, m_dimension, m_dimension, m_dimension) / distances;
				//const auto b1 = create_point (point2, point0, point0, point1, point1, point2, x1, y0, z1, m_dimension, m_dimension, m_dimension) / distances;
				//const auto b2 = create_point (point2, point0, point0, point1, point1, point2, x0, y1, z2, m_dimension, m_dimension, m_dimension) / distances;

				// Create points from barycentric coordinates
				const auto index0 = add_point (point0 * b0 .x () + point1 * b0 .y () + point2 * b0 .z (), p0, p1, p2, x0, y0, z0);
				const auto index1 = add_point (point0 * b1 .x () + point1 * b1 .y () + point2 * b1 .z (), p0, p1, p2, x1, y0, z1);
				const auto index2 = add_point (point0 * b2 .x () + point1 * b2 .y () + point2 * b2 .z (), p0, p1, p2, x0, y1, z2);

				add_triangle (index0, index1, index2, m_coord_index2);

				if (x2 >= 0)
				{
					// Get t edge points.
					const auto et20 = get_edge_point (p0, p1, p2, x2, m_dimension - x2, 0);
					const auto et21 = get_edge_point (p0, p1, p2, x2, 0, m_dimension - x2);

					// Barycentric coordinates for fourth point of quad
					const auto b3     = create_point (eu10, eu11, ev00, ev01, et20, et21, x2, y1, z3, xDimension - 1, m_dimension - z0, m_dimension - x2) / distances;
					//const auto b3     = create_point (point2, point0, point0, point1, point1, point2, x2, y1, z3, m_dimension, m_dimension, m_dimension) / distances;
					const auto index3 = add_point (point0 * b3 .x () + point1 * b3 .y () + point2 * b3 .y (), p0, p1, p2, x2, y1, z3);

					add_triangle (index0, index2, index3, m_coord_index2);
				}
			}
		}

break;
	}

	std::swap (m_coord_index, m_coord_index2);
}

template <class Type>
vector3 <Type>
basic_polyhedron3 <Type>::create_point (const vector3 <Type> & u0,
                                        const vector3 <Type> & u1,
                                        const vector3 <Type> & v0,
                                        const vector3 <Type> & v1,
                                        const vector3 <Type> & t0,
                                        const vector3 <Type> & t1,
                                        const int32_t x,
                                        const int32_t y,
                                        const int32_t z,
                                        const int32_t xDimension,
                                        const int32_t yDimension,
                                        const int32_t zDimension
)
{
	// Barycentric input coordinates.
	const auto u = xDimension ? x / Type (xDimension) : Type (0);
	const auto v = yDimension ? y / Type (yDimension) : Type (0);
	const auto t = zDimension ? z / Type (zDimension) : Type (0);

	// Angle between points
	const auto alphaU = std::acos (clamp <Type> (dot (u0, u1), -1, 1)) * u;
	const auto alphaV = std::acos (clamp <Type> (dot (v0, v1), -1, 1)) * v;
	const auto alphaT = std::acos (clamp <Type> (dot (t0, t1), -1, 1)) * t;

	// Angle between point and corresponding edge
	const auto betaU = pi <Type> - std::acos (clamp <Type> (dot (normalize (u0 - u1), u0), -1, 1));
	const auto betaV = pi <Type> - std::acos (clamp <Type> (dot (normalize (v0 - v1), v0), -1, 1));
	const auto betaT = pi <Type> - std::acos (clamp <Type> (dot (normalize (t0 - t1), t0), -1, 1));

	// Law of sines
	const auto a = std::sin (alphaU) / std::sin (betaU - alphaU);
	const auto b = std::sin (alphaV) / std::sin (betaV - alphaV);
	const auto c = std::sin (alphaT) / std::sin (betaT - alphaT);

	// Barycentric coordinates.
	return vector3 <Type> (a, b, c);
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

	octahedron3 (const size_t dimension);

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
octahedron3 <Type>::octahedron3 (const size_t dimension) :
	basic_polyhedron3 <Type> (dimension)
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

	icosahedron3 (const size_t dimension);

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
icosahedron3 <Type>::icosahedron3 (const size_t dimension) :
	basic_polyhedron3 <Type> (dimension)
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

	for (auto & point : this -> get_simplex ())
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
