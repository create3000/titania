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
#include "../Algorithms/SAT.h"

#include <vector>

namespace titania {
namespace math {

/**
 *  Template to represent a triangle in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class triangle3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Construction

	///  Default constructor. Constructs a box of size 0 0 and center 0 0.
	constexpr
	triangle3 () :
		m_a (),
		m_b (),
		m_c ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	triangle3 (const triangle3 <Up> & other) :
		m_a (other .a ()),
		m_b (other .b ()),
		m_c (other .c ())
	{ }

	///  Constructs a trangle from vertices @a a, @a b, and @a c in counterclockwise order.
	constexpr
	triangle3 (const vector3 <Type> & a, const vector3 <Type> & b, const vector3 <Type> & c) :
		m_a (a),
		m_b (b),
		m_c (c)
	{ }

	///  @name Member access

	const vector3 <Type> &
	a () const
	{ return m_a; }

	const vector3 <Type> &
	b () const
	{ return m_b; }

	const vector3 <Type> &
	c () const
	{ return m_c; }

	///  @name Operations

	///  Returns the area of the triangle
	Type
	area () const 
	{ return abs (cross (b () - a (), c () - a ())) / 2; }

	///  Returns the normal of the triangle.
	vector3 <Type>
	normal () const
	{ return normalize (cross (c () - b (), a () - b ())); }

	///  Returns the index of one of the three points defined by @a a, @a b, @a c to @a point.
	size_t
	closest_point (const vector3 <Type> & point) const;

	///  Returns the squared distance from the triangle defined by @a A, @a B, @a C to @a point.
	Type
	distance_to_point (const vector3 <Type> & point) const;

	///  @name Intersection

	///  Returns true if @a point intersects with geometry, otherwise false.
	bool
	intersects (const std::vector <vector3 <Type>> & points,
	            const std::vector <vector3 <Type>> & edges,
	            const std::vector <vector3 <Type>> & normals) const;


private:

	///  @name Members;

	vector3 <Type> m_a;
	vector3 <Type> m_b;
	vector3 <Type> m_c;

};

template <class Type>
size_t
triangle3 <Type>::closest_point (const vector3 <Type> & point) const
{
	const auto distance0 = abs (a () - point);
	const auto distance1 = abs (b () - point);
	const auto distance2 = abs (c () - point);

	if (distance0 < distance1)
	{
		if (distance0 < distance2)
			return 0;
	}
   
	if (distance1 < distance2)
		return 1;

	return 2;
}

template <class Type>
Type
triangle3 <Type>::distance_to_point (const vector3 <Type> & point) const
{
	// http://www.geometrictools.com/GTEngine/Include/GteDistPoint3Triangle3.inl

	Type sqrDistance;

	const auto diff  = this -> a () - point;
	const auto edge0 = this -> b () - this -> a ();
	const auto edge1 = this -> c () - this -> a ();
	auto       a00   = dot (edge0, edge0);
	auto       a01   = dot (edge0, edge1);
	auto       a11   = dot (edge1, edge1);
	auto       b0    = dot (diff, edge0);
	auto       b1    = dot (diff, edge1);
	auto       c     = dot (diff, diff);
	auto       det   = std::abs (a00 * a11 - a01 * a01);
	auto       s     = a01 * b1 - a11 * b0;
	auto       t     = a01 * b0 - a00 * b1;

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

// 
template <class Type>
bool
triangle3 <Type>::intersects (const std::vector <vector3 <Type>> & points,
	                           const std::vector <vector3 <Type>> & edges,
	                           const std::vector <vector3 <Type>> & normals) const
{
	// Test special cases.

	// Get points.

	const std::vector <vector3 <Type>> points2 = { a (), b (), c () };

	// Test the three planes spanned by the normal vectors of the faces of the first parallelepiped.

	if (sat::separated (normals, points, points2))
		return false;

	// Test the normal of the triangle.

	if (sat::separated ({ normal () }, points, points2))
		return false;

	// Test the nine other planes spanned by the edges of the parallelepiped and the edges of the triangle.

	const std::array <vector3 <Type>, 3> edges2 = {
		a () - b (),
		b () - c (),
		c () - a (),
	};

	std::vector <vector3 <Type>> axes;

	for (const auto & axis1 : edges)
	{
		for (const auto & axis2 : edges2)
			axes .emplace_back (cross (axis1, axis2));
	}

	if (sat::separated (axes, points, points2))
		return false;

	// Box and triangle intersect.

	return true;
}

// D ----- C
//  |       |
//  |       |
//  |       |
// A ----- B

template <class Type>
inline
vector3 <Type>
normal (const vector3 <Type> & A, const vector3 <Type> & B, const vector3 <Type> & C, const vector3 <Type> & D)
{
	// (p3 - B) x (p4 - C)
	return normalize (cross (C - A, D - B));
}

///  @relates triangle3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, triangle3 <Type> & triangle)
{
	vector3 <Type> a;
	vector3 <Type> b;
	vector3 <Type> c;

	istream >> a >> b >> c;

	if (istream)
		triangle = triangle3 <Type> (a, b, c);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const triangle3 <Type> & triangle)
{
	return ostream << triangle .a () << "  " << triangle .b () << "  " << triangle .c ();
}

extern template class triangle3 <float>;
extern template class triangle3 <double>;
extern template class triangle3 <long double>;

//
extern template std::istream & operator >> (std::istream &, triangle3 <float> &);
extern template std::istream & operator >> (std::istream &, triangle3 <double> &);
extern template std::istream & operator >> (std::istream &, triangle3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const triangle3 <float> &);
extern template std::ostream & operator << (std::ostream &, const triangle3 <double> &);
extern template std::ostream & operator << (std::ostream &, const triangle3 <long double> &);

} // math
} // titania

#endif
