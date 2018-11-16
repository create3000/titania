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

#ifndef __TITANIA_MATH_GEOMETRY_LINE3_H__
#define __TITANIA_MATH_GEOMETRY_LINE3_H__

#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"
#include "../Utility/Types.h"

#include "../../LOG.h"

namespace titania {
namespace math {

/**
 *  Template to represent a line in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class line3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor. Constructs a line of from (0, 0, 0) to (0, 0, 1).
	constexpr
	line3 () :
		    m_point (),
		m_direction (0, 0, 1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	line3 (const line3 <Up> & line) :
		    m_point (line .point ()),
		m_direction (line .direction ())
	{ }

	///  Constructs a line of from @a point and @a direction, where direction must be normalized.
	constexpr
	line3 (const vector3 <Type> & point, const vector3 <Type> & direction) :
		    m_point (point),
		m_direction (direction)
	{ }

	///  Constructs a line of from @a point and @a point.
	line3 (const vector3 <Type> & point1, const vector3 <Type> & point2, const points_type &) :
		    m_point (point1),
		m_direction (normalize (point2 - point1))
	{ }

	///  @name Element access

	///  Returns the point of this line.
	const vector3 <Type> &
	point () const { return m_point; }

	/// Returns the direction of this line as normal a vector.
	const vector3 <Type> &
	direction () const { return m_direction; }

	///  @name  Arithmetic operations
	///  All these operators modify this box3 inplace.

	///  Transform this line by @a matrix.
	line3 &
	operator *= (const matrix4 <Type> & matrix)
	{
		mult_right (matrix);
		return *this;
	}

	///  Transform this line by @a matrix.
	void
	mult_left (const matrix4 <Type> & matrix)
	{
		m_point     = matrix .mult_matrix_vec (m_point);
		m_direction = normalize (matrix .mult_matrix_dir (m_direction));
	}

	///  Transform this line by @a matrix.
	void
	mult_right (const matrix4 <Type> & matrix)
	{
		m_point     = matrix .mult_vec_matrix (m_point);
		m_direction = normalize (matrix .mult_dir_matrix (m_direction));
	}

	//  @name Distance

	///  Returns the perpendicular distance from the @a point to this line.
	Type
	distance (const vector3 <Type> & point) const
	{
		return abs (perpendicular_vector (point));
	}

	///  Returns the perpendicular distance from the @a line to this line.
	Type
	distance (const line3 <Type> & line) const
	{
		return abs (perpendicular_vector (line));
	}

	///  Returns the perpendicular vector from @a point to this line.
	vector3 <Type>
	perpendicular_vector (const vector3 <Type> & point) const
	{
		const vector3 <Type> d = this -> point () - point;
		return d - dot (d, direction ()) * direction ();
	}

	///  Returns the perpendicular vector from @a line to this line.
	vector3 <Type>
	perpendicular_vector (const line3 <Type> & line) const;

	///  Returns the closest point from @a point to this line on this line.
	vector3 <Type>
	closest_point (const vector3 <Type> & point) const
	{
		return this -> point () + direction () * dot (point - this -> point (), direction ());
	}

	///  Returns a pair consisting of the closest point from @a line to this line on this line,
	///  and a bool denoting whether both lines are parallel.
	std::pair <vector3 <Type>, bool>
	closest_point (const line3 & line) const;

	///  @name Intersection

	///  Returns a pair consisting of the barycentric intersection coordinates for the triangle,
	///  and a bool denoting whether the intersection was successful.
	std::pair <vector3 <Type>, bool>
	intersects (const vector3 <Type> & A,
	            const vector3 <Type> & B,
	            const vector3 <Type> & C) const;


private:

	vector3 <Type> m_point;
	vector3 <Type> m_direction;

};

template <class Type>
vector3 <Type>
line3 <Type>::perpendicular_vector (const line3 <Type> & line) const
{
	const vector3 <Type> d = point () - line .point ();

	const Type re1 = dot (d, direction ());
	const Type re2 = dot (d, line .direction ());
	const Type e12 = dot (direction (), line .direction ());
	const Type E12 = std::pow (e12, 2);

	const Type a = (re1 - re2 * e12) / (1 - E12);
	const Type b = -(re2 - re1 * e12) / (1 - E12);

	return d + b * line .direction () - a * direction ();

}

template <class Type>
std::pair <vector3 <Type>, bool>
line3 <Type>::closest_point (const line3 & line) const
{
	const auto & p1 = this -> point ();
	const auto & p2 = line .point ();

	const auto & d1 = direction ();
	const auto & d2 = line .direction ();

	const auto theta = dot (d1, d2);

	if (std::abs (theta) >= 1)
		return std::pair (vector3 <Type> (), false);  // lines are parallel

	const auto u     = p2 - p1;
	const auto t     = (dot (u, d1) - theta * dot (u, d2)) / (1 - theta * theta);
	const auto point = p1 + t * d1;

	return std::pair (point, true);
}

template <class Type>
std::pair <vector3 <Type>, bool>
line3 <Type>::intersects (const vector3 <Type> & A,
                          const vector3 <Type> & B,
                          const vector3 <Type> & C) const
{
	// Möller–Trumbore intersection algorithm.
	// https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm

	// find vectors for two edges sharing vert0
	const auto edge1 = B - A;
	const auto edge2 = C - A;

	// begin calculating determinant - also used to calculate U parameter
	const auto pvec = cross (direction (), edge2);

	// if determinant is near zero, ray lies in plane of triangle
	const auto det = dot (edge1, pvec);

	// Non culling intersection

	if (det == 0)
		return std::pair (vector3 <Type> (), false);

	const auto inv_det = 1 / det;

	// calculate distance from vert0 to ray point
	const auto tvec = point () - A;

	// calculate U parameter and test bounds
	const auto u = dot (tvec, pvec) * inv_det;

	if (u < 0 or u > 1)
		return std::pair (vector3 <Type> (), false);

	// prepare to test V parameter
	const auto qvec = cross (tvec, edge1);

	// calculate V parameter and test bounds
	const auto v = dot (direction (), qvec) * inv_det;

	if (v < 0 or u + v > 1)
		return std::pair (vector3 <Type> (), false);

	// Don' know what this is.
	//t = dot (edge2, qvec) * inv_det;

	return std::pair (vector3 <Type> (1 - u - v, u, v), true);
}

///  @relates line3
///  @name Arithmetic operations

///  Return new line3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line3 <Type>
operator * (const line3 <Type> & lhs, const matrix4 <Type> & rhs)
{
	line3 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return new line3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line3 <Type>
operator * (const matrix4 <Type> & lhs, const line3 <Type> & rhs)
{
	line3 <Type> result (rhs);
	result .mult_left (lhs);
	return result;
}

///  @relates line3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, line3 <Type> & line)
{
	vector3 <Type> point;
	vector3 <Type> direction;

	istream >> point >> direction;

	if (istream)
		line = line3 <Type> (point, direction);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const line3 <Type> & line)
{
	return ostream << line .point () << " " << line .direction ();
}

extern template class line3 <float>;
extern template class line3 <double>;
extern template class line3 <long double>;

//
extern template std::istream & operator >> (std::istream &, line3 <float> &);
extern template std::istream & operator >> (std::istream &, line3 <double> &);
extern template std::istream & operator >> (std::istream &, line3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const line3 <float> &);
extern template std::ostream & operator << (std::ostream &, const line3 <double> &);
extern template std::ostream & operator << (std::ostream &, const line3 <long double> &);

} // math
} // titania

#endif
