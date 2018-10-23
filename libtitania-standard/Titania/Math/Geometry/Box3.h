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

#ifndef __TITANIA_MATH_GEOMETRY_BOX3_H__
#define __TITANIA_MATH_GEOMETRY_BOX3_H__

#include "../../Utility/Range.h"
#include "../Geometry/Line3.h"
#include "../Geometry/Plane3.h"
#include "../Geometry/Sphere3.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"
#include "../Utility/Types.h"
#include "../Algorithms/SAT.h"
#include <array>
#include <vector>

namespace titania {
namespace math {

/**
 *  Template to represent a oriented bounding box in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class box3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor. Constructs an empty box.
	constexpr
	box3 () :
		m_matrix (Type (0.5), 0, 0, 0,
		          0, Type (0.5), 0, 0,
		          0, 0, Type (0.5), 0,
		          0, 0, 0, 0)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	box3 (const box3 <Up> & other) :
		m_matrix (other .matrix ())
	{ }

	///  Constructs a box of size @a size and center @a size.
	constexpr
	box3 (const vector3 <Type> & size, const vector3 <Type> & center) :
		m_matrix (size .x () / 2, 0, 0, 0,
		          0, size .y () / 2, 0, 0,
		          0, 0, size .z () / 2, 0,
		          center .x (), center .y (), center .z (), 1)
	{ }

	///  Constructs a box of min @a min and max @a max.
	constexpr
	box3 (const vector3 <Type> & min, const vector3 <Type> & max, const extents_type &) :
		box3 (max - min, (max + min) / Type (2))
	{ }

	template <class InputIterator>
	box3 (InputIterator first, InputIterator last, const iterator_type &) :
		box3 ()
	{
		if (first == last)
			return;

		vector3 <Type> min = *first;
		vector3 <Type> max = min;

		while (++ first not_eq last)
		{
			min = math::min (min, (vector3 <Type>) * first);
			max = math::max (max, (vector3 <Type>) * first);
		}

		*this = box3 (min, max, extents_type ());
	}

	///  Constructs a box from @a matrix.
	constexpr
	box3 (const matrix4 <Type> & matrix) :
		m_matrix (matrix)
	{ }

	///  @name Assignment operator

	///  Assign @a box3 to this box3.
	template <class Up>
	box3 &
	operator = (const box3 <Up> & other)
	{
		m_matrix = other .m_matrix;
		return *this;
	}

	///  @name Element access

	void
	matrix (const matrix4 <Type> & value)
	{ m_matrix = value; }

	const matrix4 <Type> &
	matrix () const
	{ return m_matrix; }

	///  Returns the min and max extents of this box.
	std::pair <vector3 <Type>, vector3 <Type>>
	extents () const;

	///  Returns whether this box is an empty box.
	bool
	empty () const
	{ return m_matrix [3] [3] == 0; }

	///  Returns the volume of this box.
	Type
	volume () const;

	///  Returns the size of this box.
	vector3 <Type>
	size () const;

	///  Returns the center of this box.
	vector3 <Type>
	center () const
	{ return m_matrix .origin (); }

	///  Returns the aabb box of this box.
	box3 <Type>
	aabb () const
	{ return box3 (size (), center ()); }

	///  Returns the transformed points of this box.
	std::vector <vector3 <Type>> 
	points () const;

	///  Returns the scaled axes of this box.
	vector3 <vector3 <Type>> 
	axes () const;

	///  Returns the three unique normals of this box.
	vector3 <vector3 <Type>> 
	normals () const;

	///  @name  Arithmetic operations
	///  All these operators modify this box3 inplace.

	///  Add @a box3 to this box.
	template <class Up>
	box3 &
	operator += (const box3 <Up> &);

	///  Transform this box by @a matrix.
	box3 &
	operator *= (const matrix4 <Type> & matrix)
	{
		mult_right (matrix);
		return *this;
	}

	///  Transform this box by @a matrix.
	void
	mult_left (const matrix4 <Type> & matrix)
	{ m_matrix .mult_left (matrix); }

	///  Transform this box by @a matrix.
	void
	mult_right (const matrix4 <Type> & matrix)
	{ m_matrix .mult_right (matrix); }

	///  Translate this box by @a translation.
	void
	translate (const vector3 <Type> & translation)
	{
		matrix4 <Type> t;

		t .translate (translation);
		m_matrix *= t;
	}

	///  Rotate this box by @a rotation.
	void
	rotate (const rotation4 <Type> & rotation)
	{ m_matrix *= matrix4 <Type> (rotation); }

	///  Scale this box by @a scaleFactor.
	void
	scale (const vector3 <Type> & scaleFactor)
	{
		matrix4 <Type> s;

		s .scale (scaleFactor);
		m_matrix *= s;
	}

	///  @name Intersection

	///  Returns true if @a point is inside this box's min and max extents.
	bool
	intersects (const vector3 <Type> & point) const;

	///  Returns true if @a line intersects with this box's min and max extends.
	bool
	intersects (const line3 <Type> & line) const;

	///  Returns true if @a sphere intersects with this box's min and max extends.
	bool
	intersects (const sphere3 <Type> & sphere) const;

	///  Returns true if @a box intersects with this oriented box.
	bool
	intersects (const box3 & box) const;

	///  Returns true if the triangle defined by the points @a a, @a b, and @a c intersects with this oriented box.
	bool
	intersects (const vector3 <Type> & a, const vector3 <Type> & b, const vector3 <Type> & c) const;

	///  Returns true if @a box is within this box's min and max extends.
	bool
	contains (const box3 & box) const;


private:

	///  Returns the absolute min and max extents of this box.
	std::pair <vector3 <Type>, vector3 <Type>>
	absolute_extents () const;

	matrix4 <Type> m_matrix;

};

template <class Type>
inline
Type
box3 <Type>::volume () const
{
	if (empty ())
		return 0;

	return std::abs (matrix () .submatrix () .determinant ()) * 8;
}

template <class Type>
inline
vector3 <Type>
box3 <Type>::size () const
{
	const auto extents = absolute_extents ();

	return extents .second - extents .first;
}

template <class Type>
std::vector <vector3 <Type>> 
box3 <Type>::points () const
{
	/*
	 * p6 ---------- p5
	 * | \           | \
	 * | p2------------ p1
	 * |  |          |  |
	 * |  |          |  |
	 * p7 |_________ p8 |
	 *  \ |           \ |
	 *   \|            \|
	 *    p3 ---------- p4
	 */

	std::vector <vector3 <Type>>  points;
	points .reserve (8);

	const auto x = matrix () .x_axis ();
	const auto y = matrix () .y_axis ();
	const auto z = matrix () .z_axis ();

	const auto r1 = y + z;
	const auto r2 = z - y;

	points .emplace_back (x + r1);
	points .emplace_back (r1 - x);
	points .emplace_back (r2 - x);
	points .emplace_back (x + r2);

	points .emplace_back (-points [2]);
	points .emplace_back (-points [3]);
	points .emplace_back (-points [0]);
	points .emplace_back (-points [1]);

	for (auto & point : points)
		point += center ();

	return points;
}

template <class Type>
vector3 <vector3 <Type>> 
box3 <Type>::axes () const
{
	return vector3 <vector3 <Type>> (
		matrix () .x_axis (),
		matrix () .y_axis (),
		matrix () .z_axis ()
	);
}

template <class Type>
vector3 <vector3 <Type>> 
box3 <Type>::normals () const
{
	vector3 <vector3 <Type>> normals;

	const auto x = matrix () .x_axis ();
	const auto y = matrix () .y_axis ();
	const auto z = matrix () .z_axis ();

	normals .x (normalize (cross (y, z)));
	normals .y (normalize (cross (z, x)));
	normals .z (normalize (cross (x, y)));

	return normals;
}

template <class Type>
inline
std::pair <vector3 <Type>, vector3 <Type>>
box3 <Type>::extents () const
{
	auto extents = absolute_extents ();

	extents .first  += center ();
	extents .second += center ();

	return extents;
}

template <class Type>
std::pair <vector3 <Type>, vector3 <Type>>
box3 <Type>::absolute_extents () const
{
	const vector3 <Type> x (m_matrix .x_axis ());
	const vector3 <Type> y (m_matrix .y_axis ());
	const vector3 <Type> z (m_matrix .z_axis ());

	const auto r1 = y + z;
	const auto r2 = z - y;

	const auto p1 =  x + r1;
	const auto p2 = r1 - x;
	const auto p3 = r2 - x;
	const auto p4 =  x + r2;

	const auto p5 = -p3;
	const auto p6 = -p4;
	const auto p7 = -p1;
	const auto p8 = -p2;

	const auto min = math::min ({ p1, p2, p3, p4, p5, p6, p7, p8 });
	const auto max = math::max ({ p1, p2, p3, p4, p5, p6, p7, p8 });

	return std::make_pair (min, max);
}

template <class Type>
template <class Up>
box3 <Type> &
box3 <Type>::operator += (const box3 <Up> & box)
{
	if (empty ())
		return *this = box;

	if (box .empty ())
		return *this;

	const auto   extents1 = this -> extents ();
	const auto   extents2 = box .extents ();
	const auto & min1     = extents1 .first;
	const auto & max1     = extents1 .second;
	const auto & min2     = extents2 .first;
	const auto & max2     = extents2 .second;

	return *this = box3 (math::min (min1, min2), math::max (max1, max2), extents_type ());
}

template <class Type>
bool
box3 <Type>::intersects (const vector3 <Type> & point) const
{
	const auto   extents = this -> extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;

	return min .x () <= point .x () and
	       max .x () >= point .x () and
	       min .y () <= point .y () and
	       max .y () >= point .y () and
	       min .z () <= point .z () and
	       max .z () >= point .z ();
}
template <class Type>
bool
box3 <Type>::intersects (const line3 <Type> & line) const
{
	static const vector3 <Type> normals [5] = {
		vector3 <Type> (0,  0,  1), // front
		vector3 <Type> (0,  0, -1), // back
		vector3 <Type> (0,  1,  0), // top
		vector3 <Type> (0, -1,  0), // bottom
		vector3 <Type> (1,  0,  0)  // right
		// left: We do not have to test for left.
	};

	const auto   extents = this -> extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;


	for (size_t i = 0; i < 5; ++ i)
	{
		const auto pair = plane3 <Type> (is_odd (i) ? min : max, normals [i]) .intersects (line);

		if (pair .second)
		{
			const auto & intersection = pair .first;

			switch (i)
			{
				case 0:
				case 1:
				{
					if (intersection .x () >= min .x () and intersection .x () <= max .x () and
					    intersection .y () >= min .y () and intersection .y () <= max .y ())
						return true;

					break;
				}
				case 2:
				case 3:
				{
					if (intersection .x () >= min .x () and intersection .x () <= max .x () and
					    intersection .z () >= min .z () and intersection .z () <= max .z ())
						return true;

					break;
				}
				case 4:
				{
					if (intersection .y () >= min .y () and intersection .y () <= max .y () and
					    intersection .z () >= min .z () and intersection .z () <= max .z ())
						return true;

					break;
				}
			}
		}
	}

	return false;
}

template <class Type>
bool
box3 <Type>::intersects (const sphere3 <Type> & sphere) const
{
	const auto   extents = this -> extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;

	const vector3 <Type> center = sphere .center ();
	const Type           radius = sphere .radius ();

	return min .x () <= center .x () + radius and
	       max .x () >= center .x () - radius and
	       min .y () <= center .y () + radius and
	       max .y () >= center .y () - radius and
	       min .z () <= center .z () + radius and
	       max .z () >= center .z () - radius;
}

template <class Type>
bool
box3 <Type>::intersects (const box3 & other) const
{
	// Test special cases.

	if (empty ())
		return false;

	if (other .empty ())
		return false;

	// Get points.

	const auto points1 = points ();
	const auto points2 = other .points ();

	// Test the three planes spanned by the normal vectors of the faces of the first parallelepiped.

	const auto normals1 = normals ();

	if (sat::separated (std::vector <vector3 <Type>> (normals1 .begin (), normals1 .end ()), points1, points2))
		return false;

	// Test the three planes spanned by the normal vectors of the faces of the second parallelepiped.

	const auto normals2 = other .normals ();

	if (sat::separated (std::vector <vector3 <Type>> (normals2 .begin (), normals2 .end ()), points1, points2))
		return false;

	// Test the nine other planes spanned by the edges of each parallelepiped.

	std::vector <vector3 <Type>>  axes9;

	for (const auto & axis1 : axes ())
	{
		for (const auto & axis2 : other .axes ())
			axes9 .emplace_back (cross (axis1, axis2));
	}

	if (sat::separated (axes9, points1, points2))
		return false;

	// Both boxes intersect.

	return true;
}

template <class Type>
bool
box3 <Type>::intersects (const vector3 <Type> & a, const vector3 <Type> & b, const vector3 <Type> & c) const
{
	// Test special cases.

	if (empty ())
		return false;

	// Get points.

	const std::vector <vector3 <Type>>  points1 = points ();
	const std::vector <vector3 <Type>>  points2 = { a, b, c };

	// Test the three planes spanned by the normal vectors of the faces of the first parallelepiped.

	const auto normals1 = normals ();

	if (sat::separated (std::vector <vector3 <Type>> (normals1 .begin (), normals1 .end ()), points1, points2))
		return false;

	// Test the normal of the triangle.

	if (sat::separated ({ triangle3 <Type> (a, b, c) .normal () }, points1, points2))
		return false;

	// Test the nine other planes spanned by the edges of the parallelepiped and the edges of the triangle.

	const std::array <vector3 <Type>, 3> triangleEdges = {
		a - b,
		b - c,
		c - a,
	};

	std::vector <vector3 <Type>>  axes9;

	for (const auto & axis1 : axes ())
	{
		for (const auto & axis2 : triangleEdges)
			axes9 .emplace_back (cross (axis1, axis2));
	}

	if (sat::separated (axes9, points1, points2))
		return false;

	// Box and triangle intersect.

	return true;
}


template <class Type>
bool
box3 <Type>::contains (const box3 <Type> & other) const
{
	if (empty () or other .empty ())
		return false;

	const auto   extents1 = this -> extents ();
	const auto   extents2 = other .extents ();
	const auto & min1     = extents1 .first;
	const auto & max1     = extents1 .second;
	const auto & min2     = extents2 .first;
	const auto & max2     = extents2 .second;

	if (min2 .x () < min1 .x () or min2 .y () < min1 .y () or min2 .z () < min1 .z ())
		return false;

	if (max2 .x () > max1 .x () or max2 .y () > max1 .y () or max2 .z () > max1 .z ())
		return false;

	return true;
}

///  @relates box3
///  @name Comparision operations

///  Compares two box3 numbers.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
constexpr bool
operator == (const box3 <Type> & lhs, const box3 <Type> & rhs)
{
	return lhs .matrix () == rhs .matrix ();
}

///  Compares two box3 numbers.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
constexpr bool
operator not_eq (const box3 <Type> & lhs, const box3 <Type> & rhs)
{
	return lhs .matrix () not_eq rhs .matrix ();
}

///  @relates box3
///  @name Arithmetic operations

///  Return new box3 value @a lhs plus @a rhs.
template <class Type>
inline
box3 <Type>
operator + (const box3 <Type> & lhs, const box3 <Type> & rhs)
{
	return box3 <Type> (lhs) += rhs;
}

///  Return new box3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box3 <Type>
operator * (const box3 <Type> & lhs, const matrix4 <Type> & rhs)
{
	box3 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return new box3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box3 <Type>
operator * (const matrix4 <Type> & lhs, const box3 <Type> & rhs)
{
	box3 <Type> result (rhs);
	result .mult_left (lhs);
	return result;
}

///  @relates box3
///  @name Input/Output operations

///  Extraction operator for box3 values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, box3 <Type> & box)
{
	vector3 <Type> size;
	vector3 <Type> center;

	istream >> size >> center;

	if (istream)
		box = box3 <Type> (size, center);

	return istream;
}

///  Insertion operator for box3 values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const box3 <Type> & box)
{
	return ostream << box .size () << ", " << box .center ();
}

extern template class box3 <float>;
extern template class box3 <double>;
extern template class box3 <long double>;

//
extern template std::istream & operator >> (std::istream &, box3 <float> &);
extern template std::istream & operator >> (std::istream &, box3 <double> &);
extern template std::istream & operator >> (std::istream &, box3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const box3 <float> &);
extern template std::ostream & operator << (std::ostream &, const box3 <double> &);
extern template std::ostream & operator << (std::ostream &, const box3 <long double> &);

} // math
} // titania

#endif
