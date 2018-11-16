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

#ifndef __TITANIA_MATH_GEOMETRY_LINE2_H__
#define __TITANIA_MATH_GEOMETRY_LINE2_H__

#include "../Numbers/Matrix3.h"
#include "../Numbers/Vector2.h"
#include "../Utility/Types.h"

namespace titania {
namespace math {

/**
 *  Template to represent a line in 2D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class line2
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor. Constructs a line of from (0, 0) to (0, 1).
	constexpr
	line2 () :
		    m_point (),
		m_direction (0, 1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	line2 (const line2 <Up> & line) :
		    m_point (line .point ()), 
		m_direction (line .direction ())
	{ }

	///  Constructs a line of from @a point and @a direction, where direction must be normalized.
	constexpr
	line2 (const vector2 <Type> & point, const vector2 <Type> & direction) :
		    m_point (point),
		m_direction (direction)
	{ }

	///  Constructs a line of from @a point1 to @a point2.
	line2 (const vector2 <Type> & point1, const vector2 <Type> & point2, const points_type &) :
		    m_point (point1),
		m_direction (normalize (point2 - point1))
	{ }

	///  @name Element access

	///  Returns the point of this line.
	const vector2 <Type> &
	point () const
	{ return m_point; }

	/// Returns the direction of this line as normal a vector.
	const vector2 <Type> &
	direction () const
	{ return m_direction; }

	///  Returns a normal of this line, this vector is orthogonal to this line's direction.
	vector2 <Type>
	normal () const
	{
		return vector2 <Type> (-direction () .y (), direction () .x ());
	}

	///  @name  Arithmetic operations
	///  All these operators modify this box3 inplace.

	///  Transform this line by @a matrix.
	line2 &
	operator *= (const matrix3 <Type> & matrix)
	{
		mult_right (matrix);
		return *this;
	}

	///  Transform this box by @a matrix.
	void
	mult_left (const matrix3 <Type> & matrix)
	{
		m_point     = matrix .mult_matrix_vec (m_point);
		m_direction = normalize (matrix .mult_matrix_dir (m_direction));
	}

	///  Transform this box by @a matrix.
	void
	mult_right (const matrix3 <Type> & matrix)
	{
		m_point     = matrix .mult_vec_matrix (m_point);
		m_direction = normalize (matrix .mult_dir_matrix (m_direction));
	}

	//  @name Distance

	///  Returns the perpendicular distance from the @a point to this line.
	Type
	distance (const vector2 <Type> & point) const
	{
		return abs (perpendicular_vector (point));
	}

	///  Returns the perpendicular vector from @a point to this line.
	vector2 <Type>
	perpendicular_vector (const vector2 <Type> & point) const
	{
		const vector2 <Type> d = this -> point () - point;
		return d - dot (d, direction ()) * direction ();
	}

	///  Returns the closest point from @a point to this line on this line.
	vector2 <Type>
	closest_point (const vector2 <Type> & point) const
	{
		return this -> point () + direction () * dot (point - this -> point (), direction ());
	}

	///  @name Intersection

	///  Returns a pair consisting of the intersection point,
	///  and a bool denoting whether the intersection was successful.
	std::pair <vector2 <Type>, bool>
	intersects (const line2 & line) const;
	
	///  Returns true if the polygon given by a range of points intersects with this line, otherwise false.
	template <class Iterator>
	bool
	intersects (Iterator first, Iterator last) const;


private:

	vector2 <Type> m_point;
	vector2 <Type> m_direction;	

};

template <class Type>
std::pair <vector2 <Type>, bool>
line2 <Type>::intersects (const line2 & line) const
{
	const auto & p1 = this -> point ();
	const auto & p2 = line .point ();

	const auto & d1 = direction ();
	const auto & d2 = line .direction ();

	const auto theta = dot (d1, d2); // angle between both lines

	if (std::abs (theta) >= 1)
		return std::pair (vector2 <Type> (), false);  // lines are parallel

	const auto u     = p2 - p1;
	const auto t     = (dot (u, d1) - theta * dot (u, d2)) / (1 - theta * theta);
	const auto point = p1 + t * d1;

	return std::pair (point, true);
}

template <class Type>
template <class Iterator>
bool
line2 <Type>::intersects (Iterator first, Iterator last) const
{
	const auto n      = normal ();
	int32_t    signum = 0;

	for (; first not_eq last; ++ first)
	{
		const auto theta = dot (perpendicular_vector (*first), n);

		if (theta < 0)
		{
			if (signum > 0)
				return true;

			signum = -1;
		}
		else if (theta > 0)
		{
			if (signum < 0)
				return true;

			signum = 1;
		}
		else
			return true;
	}

	return false;
}

///  @relates line2
///  @name Arithmetic operations

///  Return new line2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line2 <Type>
operator * (const line2 <Type> & lhs, const matrix3 <Type> & rhs)
{
	line2 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return new line2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line2 <Type>
operator * (const matrix3 <Type> & lhs, const line2 <Type> & rhs)
{
	line2 <Type> result (rhs);
	result .mult_left (lhs);
	return result;
}

///  @relates line2
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, line2 <Type> & line)
{
	vector2 <Type> point;
	vector2 <Type> direction;

	istream >> point >> direction;

	if (istream)
		line = line2 <Type> (point, direction);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const line2 <Type> & line)
{
	return ostream << line .point () << " " << line .direction ();
}

extern template class line2 <float>;
extern template class line2 <double>;
extern template class line2 <long double>;

//
extern template std::istream & operator >> (std::istream &, line2 <float> &);
extern template std::istream & operator >> (std::istream &, line2 <double> &);
extern template std::istream & operator >> (std::istream &, line2 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const line2 <float> &);
extern template std::ostream & operator << (std::ostream &, const line2 <double> &);
extern template std::ostream & operator << (std::ostream &, const line2 <long double> &);

} // math
} // titania

#endif
