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

#ifndef __TITANIA_MATH_GEOMETRY_BOX2_H__
#define __TITANIA_MATH_GEOMETRY_BOX2_H__

#include "../Algorithms/SAT.h"
#include "../Numbers/Matrix3.h"
#include "../Numbers/Vector2.h"
#include "../Geometry/ConvexHull2.h"
#include "../Geometry/Line2.h"
#include "../Utility/Types.h"
#include "../Functional.h"
#include "../../Utility/Range.h"

#include <algorithm>
#include <array>
#include <vector>

namespace titania {
namespace math {

/**
 *  Template to represent a oriented bounding box in 2D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class box2
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor. Constructs a box of size 0 0 and center 0 0.
	constexpr
	box2 () :
		m_matrix (Type (0.5), 0, 0,
		          0, Type (0.5), 0,
		          0, 0, 0)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	box2 (const box2 <Up> & other) :
		m_matrix (other .matrix ())
	{ }

	///  Constructs a box of size @a size and center @a size.
	constexpr
	box2 (const vector2 <Type> & size, const vector2 <Type> & center) :
		m_matrix (size .x () / 2, 0, 0,
		          0, size .y () / 2, 0,
		          center .x (), center .y (), 1)
	{ }

	///  Constructs a box of min @a min and max @a max.
	constexpr
	box2 (const vector2 <Type> & min, const vector2 <Type> & max, const extents_type &) :
		box2 (max - min, (max + min) / Type (2))
	{ }

	template <class InputIterator>
	box2 (InputIterator first, InputIterator last, const iterator_type &) :
		box2 ()
	{
		if (first == last)
			return;

		vector2 <Type> min = *first;
		vector2 <Type> max = min;

		while (++ first not_eq last)
		{
			min = math::min (min, (vector2 <Type>) *first);
			max = math::max (max, (vector2 <Type>) *first);
		}

		*this = box2 (min, max, extents_type ());
	}

	///  Constructs a box from @a matrix.
	constexpr
	box2 (const matrix3 <Type> & matrix) :
		m_matrix (matrix)
	{ }

	///  @name Assignment operator

	///  Assign @a box2 to this box2.
	template <class Up>
	box2 &
	operator = (const box2 <Up> & other)
	{
		m_matrix = other .m_matrix;
		return *this;
	}

	///  @name Element access

	void
	matrix (const matrix3 <Type> & value)
	{ m_matrix = value; }

	const matrix3 <Type> &
	matrix () const
	{ return m_matrix; }

	///  Returns the min and max extents of this box.
	std::pair <vector2 <Type>, vector2 <Type>>
	extents () const;

	///  Returns whether this box is an empty box.
	bool
	empty () const
	{ return m_matrix [2] [2] == 0; }

	///  Returns the area of this box.
	Type
	area () const;

	///  Returns the size of this box.
	vector2 <Type>
	size () const;

	///  Returns the aabb box of this box.
	box2 <Type>
	aabb () const
	{ return box2 (size (), center ()); }

	///  Returns the center of this box.
	vector2 <Type>
	center () const
	{ return m_matrix .origin (); }

	///  Returns the transformed points of this box.
	std::vector <vector2 <Type>> 
	points () const;

	///  Returns the scaled axes of this box.
	vector2 <vector2 <Type>> 
	axes () const;

	///  Returns the two unique normals of this box.
	vector2 <vector2 <Type>> 
	normals () const;

	///  @name  Arithmetic operations
	///  All these operators modify this box2 inplace.

	///  Add @a box2 to this box.
	template <class Up>
	box2 &
	operator += (const box2 <Up> &);

	///  Scale this box by @a scale.
	box2 &
	operator *= (const matrix3 <Type> & matrix)
	{
		mult_right (matrix);
		return *this;
	}

	///  Transform this box by matrix.
	void
	mult_left (const matrix3 <Type> & matrix)
	{ m_matrix .mult_left (matrix); }

	///  Transform this box by matrix.
	void
	mult_right (const matrix3 <Type> & matrix)
	{ m_matrix .mult_right (matrix); }

	///  Translate this box by @a translation.
	void
	translate (const vector2 <Type> & translation)
	{
		matrix3 <Type> t;

		t .translate (translation);

		m_matrix *= t;
	}

	///  Rotate this box by @a rotation.
	void
	rotate (const Type & rotation)
	{
		matrix3 <Type> r;

		r .rotate (rotation);

		m_matrix *= r;
	}

	///  Scale this box by @a scaleFactor.
	void
	scale (const vector2 <Type> & scaleFactor)
	{
		matrix3 <Type> s;

		s .scale (scaleFactor);

		m_matrix *= s;
	}

	///  @name Intersection

	///  Returns true if @a point is inside this box min and max extents.
	bool
	intersects (const vector2 <Type> & point) const;

	///  Returns true if @a box intersects with this oriented box.
	bool
	intersects (const box2 & box) const;

	///  Returns true if this box contains @a box.
	bool
	contains (const box2 & box) const;

private:

	///  Returns the absolute min and max extents of this box.
	std::pair <vector2 <Type>, vector2 <Type>>
	absolute_extents () const;

	matrix3 <Type> m_matrix;

};

template <class Type>
inline
std::pair <vector2 <Type>, vector2 <Type>>
box2 <Type>::extents () const
{
	auto extents = absolute_extents ();

	extents .first  += center ();
	extents .second += center ();

	return extents;
}

template <class Type>
Type
box2 <Type>::area () const
{
	if (empty ())
		return 0;

	return std::abs (matrix () .submatrix () .determinant ()) * 4;
}

template <class Type>
inline
vector2 <Type>
box2 <Type>::size () const
{
	const auto extents = absolute_extents ();

	return extents .second - extents .first;
}

template <class Type>
std::pair <vector2 <Type>, vector2 <Type>>
box2 <Type>::absolute_extents () const
{
	const vector2 <Type> x (m_matrix .x_axis ());
	const vector2 <Type> y (m_matrix .y_axis ());

	const auto p1 = x + y;
	const auto p2 = y - x;
	const auto p3 = -p1;
	const auto p4 = -p2;

	const auto min = math::min ({ p1, p2, p3, p4 });
	const auto max = math::max ({ p1, p2, p3, p4 });

	return std::make_pair (min, max);
}

template <class Type>
std::vector <vector2 <Type>> 
box2 <Type>::points () const
{
	/*
	 *  p2------------ p1
	 *  |             |
	 *  |             |
	 *  |             |
	 *  |             |
	 *  |             |
	 *  p3 ---------- p4
	 */

	std::vector <vector2 <Type>>  points;
	points .reserve (4);

	const auto x = matrix () .x_axis ();
	const auto y = matrix () .y_axis ();

	const auto p1 = x + y;
	const auto p2 = y - x;

	points .emplace_back (p1);
	points .emplace_back (p2);
	points .emplace_back (-p1);
	points .emplace_back (-p2);

	for (auto & point : points)
		point += center ();

	return points;
}

template <class Type>
vector2 <vector2 <Type>> 
box2 <Type>::axes () const
{
	return vector2 <vector2 <Type>> (
		matrix () .x_axis (),
		matrix () .y_axis ()
	);
}

template <class Type>
vector2 <vector2 <Type>> 
box2 <Type>::normals () const
{
	vector2 <vector2 <Type>> normals;

	const auto n = normalize (matrix ());
	const auto x = n .x_axis ();
	const auto y = n .y_axis ();

	normals .x (normalize (vector2 <Type> (-x .y (),  x .x ())));
	normals .y (normalize (vector2 <Type> ( y .y (), -y .x ())));

	return normals;
}

template <class Type>
template <class Up>
box2 <Type> &
box2 <Type>::operator += (const box2 <Up> & box)
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

	return *this = box2 (math::min (min1, min2), math::max (max1, max2), extents_type ());
}

template <class Type>
bool
box2 <Type>::intersects (const vector2 <Type> & point) const
{
	const auto   extents = this -> extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;

	return min .x () <= point .x () and
	       max .x () >= point .x () and
	       min .y () <= point .y () and
	       max .y () >= point .y ();
}

template <class Type>
bool
box2 <Type>::intersects (const box2 <Type> & other) const
{
	// Test special cases.

	if (empty ())
		return false;

	if (other .empty ())
		return false;

	// Get points.

	const auto points1  = points ();
	const auto points2  = other .points ();
	const auto normals1 = normals ();

	if (sat::separated (std::vector <vector2 <Type>> (normals1 .begin (), normals1 .end ()), points1, points2))
		return false;

	const auto normals2 = other .normals ();

	if (sat::separated (std::vector <vector2 <Type>> (normals2 .begin (), normals2 .end ()), points1, points2))
		return false;

	// Both boxes intersect.

	return true;
}

template <class Type>
bool
box2 <Type>::contains (const box2 <Type> & box) const
{
	if (empty () or box .empty ())
		return false;

	const auto  extents1 = this -> extents ();
	const auto  extents2 = box .extents ();
	const auto & min1    = extents1 .first;
	const auto & max1    = extents1 .second;
	const auto & min2    = extents2 .first;
	const auto & max2    = extents2 .second;

	if (min2 .x () < min1 .x () or min2 .y () < min1 .y ())
		return false;

	if (max2 .x () > max1 .x () or max2 .y () > max1 .y ())
		return false;

	return true;
}

///  @relates box2
///  @name Comparision operations

///  Compares two box2 numbers.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
constexpr bool
operator == (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return lhs .matrix () == rhs .matrix ();
}

///  Compares two box2 numbers.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
constexpr bool
operator not_eq (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return lhs .matrix () not_eq rhs .matrix ();
}

///  @relates box2
///  @name Arithmetic operations

///  Return new box2 value @a lhs plus @a rhs.
template <class Type>
inline
box2 <Type>
operator + (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (lhs) += rhs;
}

///  Return new box2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box2 <Type>
operator * (const box2 <Type> & lhs, const matrix3 <Type> & rhs)
{
	box2 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return new box2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box2 <Type>
operator * (const matrix3 <Type> & lhs, const box2 <Type> & rhs)
{
	box2 <Type> result (rhs);
	result .mult_left (lhs);
	return result;
}

///  @relates box2
///  @name Input/Output operations

///  Extraction operator for box2 values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, box2 <Type> & box)
{
	vector2 <Type> size;
	vector2 <Type> center;

	istream >> size >> center;

	if (istream)
		box = box2 <Type> (size, center);

	return istream;
}

///  Insertion operator for box2 values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const box2 <Type> & box)
{
	return ostream << box .size () << ", " << box .center ();
}

extern template class box2 <float>;
extern template class box2 <double>;
extern template class box2 <long double>;

//
extern template std::istream & operator >> (std::istream &, box2 <float> &);
extern template std::istream & operator >> (std::istream &, box2 <double> &);
extern template std::istream & operator >> (std::istream &, box2 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const box2 <float> &);
extern template std::ostream & operator << (std::ostream &, const box2 <double> &);
extern template std::ostream & operator << (std::ostream &, const box2 <long double> &);

///  @relates box2
///  @name Misc

///  Returns the minimum bounding rectangle from @a polygon, where polygon must be convex.
///  References: http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
///  https://en.wikipedia.org/wiki/Rotating_calipers
template <class Type>
box2 <Type>
minimum_bounding_rectangle (const std::vector <vector2 <Type>> & points)
{
	// minimum_bounding_rectangle

	box2 <Type> rectangle;

	const auto   hull    = convex_hull2 <Type> (points); 
	const auto & polygon = hull .indices ();

	// Find most left and most right points.

	const auto size = polygon .size ();

	const auto resultX = std::minmax_element (polygon .begin (),
	                                          polygon .end (),
	                                          [&points] (const size_t & a, const size_t & b)
                                             { return points [a] .x () < points [b] .x (); });

	// Find most lower and most upper points.

	const auto resultY = std::minmax_element (polygon .begin (),
	                                          polygon .end (),
	                                          [&points] (const size_t & a, const size_t & b)
                                             { return points [a] .y () < points [b] .y (); });

	std::vector <size_t> indices; // Index of hull point on line.

	indices .emplace_back (resultY .second - polygon .begin ());
	indices .emplace_back (resultX .second - polygon .begin ());
	indices .emplace_back (resultY .first  - polygon .begin ());
	indices .emplace_back (resultX .first  - polygon .begin ());

	// Construct initial bounding rectangle lines.

	//          a
	//    <------------
	//    |           |
	//  d |           | b
	//    |           |
	//    ------------>
	//          c

	const auto yMax = points [polygon [indices [0]]] .y ();
	const auto xMax = points [polygon [indices [1]]] .x ();
	const auto yMin = points [polygon [indices [2]]] .y ();
	const auto xMin = points [polygon [indices [3]]] .x ();

	std::vector <line2 <Type>> lines;

	lines .emplace_back (vector2 <Type> (xMax, yMax), vector2 <Type> (xMin, yMax), points_type ());
	lines .emplace_back (vector2 <Type> (xMax, yMin), vector2 <Type> (xMax, yMax), points_type ());
	lines .emplace_back (vector2 <Type> (xMin, yMin), vector2 <Type> (xMax, yMin), points_type ());
	lines .emplace_back (vector2 <Type> (xMin, yMax), vector2 <Type> (xMin, yMin), points_type ());


	//for (size_t i = 0; i < 4; ++ i)
	//	std::clog << indices [i] << " ";
	//std::clog << " : ";

	//const Type xSize = lines [0] .distance (polygon [indices [2]]);
	//const Type ySize = lines [1] .distance (polygon [indices [3]]);
	//std::clog << xSize * ySize << std::endl;

	// Rotate lines about vertex with the smalles angle to the next vertex.

	Type min_area = std::numeric_limits <Type>::infinity ();
	Type rotation = 0;

	while (rotation < pi <Type>)
	{
		Type max_cos_theta = -std::numeric_limits <Type>::infinity ();
		int  maxIndex      = -1;

		for (size_t i = 0; i < 4; ++ i)
		{
			const auto & A = points [polygon [indices [i]]];
			const auto & B = points [polygon [(indices [i] + 1) % size]];
	
			const auto cos_theta = dot (lines [i] .direction (), normalize (B - A));
	
			if (cos_theta > max_cos_theta)
			{
				max_cos_theta = cos_theta;
				maxIndex      = i;
			}
		}

		// Rotate lines.

		const auto theta = std::acos (clamp <Type> (max_cos_theta, 0, 1));

		matrix3 <Type> matrix;

		for (size_t i = 0; i < 4; ++ i)
		{
			matrix .set (vector2 <Type> (), theta, vector2 <Type> (1, 1), Type (0), points [polygon [indices [i]]]);

			lines [i] .mult_right (matrix);
		}

		rotation += theta;

		// Associate index of next point to line with closest angle.

		indices [maxIndex] = (indices [maxIndex] + 1) % size;

		//for (size_t i = 0; i < 4; ++ i)
		//	std::clog << indices [i] << " ";
		//std::clog << " : ";

		auto       xAxis = lines [3] .perpendicular_vector (points [polygon [indices [1]]]);
		auto       yAxis = lines [2] .perpendicular_vector (points [polygon [indices [0]]]);
		const Type area  = abs (xAxis) * abs (yAxis);

		if (area < min_area)
		{
			min_area = area;

			// Determine center.

			vector2 <Type> A, B;

			lines [0] .intersects (lines [1], A);
			lines [2] .intersects (lines [3], B);

			const auto center = (A + B) / Type (2);

			// Build minimum area rectangle.

			xAxis /= Type (2);
			yAxis /= Type (2);

			rectangle .matrix (matrix3 <Type> (xAxis  .x (), xAxis  .y (), 0,
			                                   yAxis  .x (), yAxis  .y (), 0,
			                                   center .x (), center .y (), 1));
		}

		//std::clog << area << std::endl;
	}

	//std::clog << std::endl;

	return rectangle;
}

} // math
} // titania

#endif
