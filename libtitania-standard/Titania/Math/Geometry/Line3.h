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

#ifndef __TITANIA_MATH_GEOMETRY_LINE3_H__
#define __TITANIA_MATH_GEOMETRY_LINE3_H__

#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"

#include "../../LOG.h"

namespace titania {
namespace math {

template <class Type>
class line3
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor. Constructs a line of from (0, 0, 0) to (0, 0, 1).
	constexpr
	line3 () :
		value { { 0, 0, 0 }, { 0, 0, 1 } }

	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	line3 (const line3 <Up> & line) :
		value { line .origin (), line .direction () }

	{ }

	///  Constructs a line of from @a origin and @a direction.
	constexpr
	line3 (const vector3 <Type> & origin, const vector3 <Type> & direction) :
		value { origin, direction }

	{ }

	///  Constructs a line of from @a point and @a point.
	constexpr
	line3 (const vector3 <Type> & point1, const vector3 <Type> & point2, bool) :
		value { point1, normalize (point2 - point1) }

	{ }

	///  @name Element access

	///  Returns the point of this line.
	const vector3 <Type> &
	origin () const { return value .point; }

	/// Returns the direction of this line as normal a vector.
	const vector3 <Type> &
	direction () const { return value .direction; }

	///  @name  Arithmetic operations
	///  All these operators modify this box3 inplace.

	///  Transform this line by @a matrix.
	line3 &
	operator *= (const matrix4 <Type> & matrix)
	{
		return multLineMatrix (matrix);
	}

	///  Transform this box by @a matrix.
	line3 &
	multMatrixLine (const matrix4 <Type> & matrix)
	{
		value .point     = matrix .multMatrixVec (value .point);
		value .direction = normalize (matrix .multMatrixDir (value .direction));
		return *this;
	}

	///  Transform this box by @a matrix.
	line3 &
	multLineMatrix (const matrix4 <Type> & matrix)
	{
		value .point     = matrix .multVecMatrix (value .point);
		value .direction = normalize (matrix .multDirMatrix (value .direction));
		return *this;
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
		vector3 <Type> d = origin () - point;
		return d - dot (d, direction ()) * direction ();
	}

	///  Returns the perpendicular vector from @a line to this line.
	vector3 <Type>
	perpendicular_vector (const line3 <Type> & line) const
	{
		vector3 <Type> d = origin () - line .origin ();

		Type re1 = dot (d, direction ());
		Type re2 = dot (d, line .direction ());
		Type e12 = dot (direction (), line .direction ());
		Type E12 = std::pow (e12, 2);

		Type a = (re1 - re2 * e12) / (1 - E12);
		Type b = -(re2 - re1 * e12) / (1 - E12);

		return d + b* line .direction () - a*
		       direction ();

	}

	///  @name Intersection

	///  Returns true if the triangle of points @a A, @a B and @a C intersects with this line.
	bool
	intersect (const vector3 <Type> &, const vector3 <Type> &, const vector3 <Type> &,
	           Type &, Type &, Type &) const;


private:

	struct Value
	{
		vector3 <Type> point;
		vector3 <Type> direction;
	};

	Value value;

};

template <class Type>
bool
line3 <Type>::intersect (const vector3 <Type> & A, const vector3 <Type> & B, const vector3 <Type> & C,
                         Type & u, Type & v, Type & t) const
{
	// find vectors for two edges sharing vert0
	vector3 <Type> edge1 = B - A;
	vector3 <Type> edge2 = C - A;

	// begin calculating determinant - also used to calculate U parameter
	vector3 <Type> pvec = cross (direction (), edge2);

	// if determinant is near zero, ray lies in plane of triangle
	Type det = dot (edge1, pvec);

	// Non culling intersection

	if (det == 0)
		return false;

	Type inv_det = 1 / det;

	// calculate distance from vert0 to ray origin
	vector3 <Type> tvec = origin () - A;

	// calculate U parameter and test bounds
	u = dot (tvec, pvec) * inv_det;

	if (u < 0 or u > 1)
		return false;

	// prepare to test V parameter
	vector3 <Type> qvec = cross (tvec, edge1);

	// calculate V parameter and test bounds
	v = dot (direction (), qvec) * inv_det;

	if (v < 0 or u + v > 1)
		return false;

	t = dot (edge2, qvec) * inv_det;

	return true;
}

///  @relates line3
///  @name Arithmetic operations

///  Return new line3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line3 <Type>
operator * (const line3 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return line3 <Type> (lhs) .multLineMatrix (rhs);
}

///  Return new line3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
line3 <Type>
operator * (const matrix4 <Type> & lhs, const line3 <Type> & rhs)
{
	return line3 <Type> (rhs) .multMatrixLine (lhs);
}

///  @relates line3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, line3 <Type> & line)
{
	vector3 <Type> origin;
	vector3 <Type> direction;

	istream >> origin >> direction;

	if (istream)
		line = line3 <Type> (origin, direction);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const line3 <Type> & line)
{
	return ostream << line .origin () << " " << line .direction ();
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
