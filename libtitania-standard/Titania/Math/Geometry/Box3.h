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

#ifndef __TITANIA_MATH_GEOMETRY_BOX3_H__
#define __TITANIA_MATH_GEOMETRY_BOX3_H__

#include "../Geometry/Line3.h"
#include "../Geometry/Plane3.h"
#include "../Geometry/Sphere3.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"
#include <array>

namespace titania {
namespace math {

template <class Type>
class box3
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor. Constructs an empty box.
	constexpr
	box3 () :
		value (Type (0.5), 0, 0, 0,
		       0, Type (0.5), 0, 0,
		       0, 0, Type (0.5), 0,
		       0, 0, 0, 0)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	box3 (const box3 <Up> & box) :
		value (box .value)
	{ }

	///  Constructs a box of size @a size and center @a size.
	constexpr
	box3 (const vector3 <Type> & size, const vector3 <Type> & center) :
		value (size .x () / 2, 0, 0, 0,
		       0, size .y () / 2, 0, 0,
		       0, 0, size .z () / 2, 0,
		       center .x (), center .y (), center .z (), 1)
	{ }

	///  Constructs a box of min @a min and max @a max.
	constexpr
	box3 (const vector3 <Type> & min, const vector3 <Type> & max, bool) :
		box3 (max - min, (max + min) / Type (2))
	{ }

	///  @name Assignment operator

	///  Assign @a box3 to this box3.
	template <class Up>
	box3 &
	operator = (const box3 <Up> & box)
	{
		value = box .value;
		return *this;
	}

	///  @name Element access

	const matrix4 <Type> &
	matrix () const
	{ return value; }

	///  Return the size of this box.
	vector3 <Type>
	size () const;

	///  Return the center of this box.
	vector3 <Type>
	center () const
	{ return value .translation (); }

	///  Return whether this box is an empty box.
	bool
	empty () const
	{ return value [3] [3] == 0; }

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
		return multBoxMatrix (matrix);
	}

	///  Transform this box by @a matrix.
	box3 &
	multMatrixBox (const matrix4 <Type> & matrix)
	{
		value .multLeft (matrix);
		return *this;
	}

	///  Transform this box by @a matrix.
	box3 &
	multBoxMatrix (const matrix4 <Type> & matrix)
	{
		value .multRight (matrix);
		return *this;
	}

	///  @name Intersection

	///  Returns true if @a point is inside this box3 min and max extend.
	bool
	intersect (const vector3 <Type> &) const;

	///  Returns true if @a line intersects with this box3.
	bool
	intersect (const line3 <Type> &, vector3 <Type> &) const;

	///  Returns true if @a sphere intersects with this box3.
	bool
	intersect (const sphere3 <Type> &) const;


private:

	matrix4 <Type> value;

};

template <class Type>
vector3 <Type>
box3 <Type>::size () const
{
	vector3 <Type> x (value [0] [0], value [0] [1], value [0] [2]);
	vector3 <Type> y (value [1] [0], value [1] [1], value [1] [2]);
	vector3 <Type> z (value [2] [0], value [2] [1], value [2] [2]);

	auto r1 =  x + y;
	auto r2 = -x + y;
	auto r3 = -x - y;
	auto r4 =  x - y;

	auto p1 = r1 + z;
	auto p2 = r2 + z;
	auto p3 = r3 + z;
	auto p4 = r4 + z;

	auto p5 = r1 - z;
	auto p6 = r2 - z;
	auto p7 = r3 - z;
	auto p8 = r4 - z;

	vector3 <Type> min, max;

	min = math::min (p1, p2);
	min = math::min (min, p3);
	min = math::min (min, p4);
	min = math::min (min, p5);
	min = math::min (min, p6);
	min = math::min (min, p7);
	min = math::min (min, p8);

	max = math::max (p1, p2);
	max = math::max (max, p3);
	max = math::max (max, p4);
	max = math::max (max, p5);
	max = math::max (max, p6);
	max = math::max (max, p7);
	max = math::max (max, p8);

	return max - min;
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

	auto lsize1_2 = size () / 2.0f;
	auto lhs_min  = center () - lsize1_2;
	auto lhs_max  = center () + lsize1_2;

	auto rsize1_2 = box .size () / 2.0f;
	auto rhs_min  = box .center () - rsize1_2;
	auto rhs_max  = box .center () + rsize1_2;

	return *this = box3 (math::min (lhs_min, rhs_min), math::max (lhs_max, rhs_max), true);
}

template <class Type>
bool
box3 <Type>::intersect (const vector3 <Type> & point) const
{
	auto size1_2 = size () / Type (2);
	auto min     = center () - size1_2;
	auto max     = center () + size1_2;

	return min .x () <= point .x () and
	       max .x () >= point .x () and
	       min .y () <= point .y () and
	       max .y () >= point .y () and
	       min .z () <= point .z () and
	       max .z () >= point .z ();
}

template <class Type>
bool
box3 <Type>::intersect (const line3 <Type> & line, vector3 <Type> & intersection) const
{
	auto size1_2 = size () / Type (2);
	auto min     = center () - size1_2;
	auto max     = center () + size1_2;

	vector3 <Type> center = (min + max) / Type (2);

	vector3 <Type> points [6] = {
		vector3 <Type> (center .x (), center .y (), max .z ()), // right
		vector3 <Type> (center .x (), center .y (), min .z ()), // left

		vector3 <Type> (center .x (), max .y (), center .z ()), // top
		vector3 <Type> (center .x (), min .y (), center .z ()), // bottom

		vector3 <Type> (max .x (), center .y (), center .z ()), // front
		vector3 <Type> (min .x (), center .y (), center .z ())  // back
	};

	static const vector3 <Type> normals [6] = {
		vector3 <Type> (0,  0,  1),
		vector3 <Type> (0,  0, -1),
		vector3 <Type> (0,  1,  0),
		vector3 <Type> (0, -1,  0),
		vector3 <Type> (1,  0,  0),
		vector3 <Type> (-1,  0,  0)
	};

	for (size_t i = 0; i < 6; ++ i)
	{
		if (plane3 <Type> (points [i], normals [i]) .intersect (line, intersection))
		{
			switch (i)
			{
				case 0:
				case 1:

					if (intersection .x () >= min .x () and intersection .x () <= max .x () and
					    intersection .y () >= min .y () and intersection .y () <= max .y ())
						return true;

					break;
				case 2:
				case 3:

					if (intersection .x () >= min .x () and intersection .x () <= max .x () and
					    intersection .z () >= min .z () and intersection .z () <= max .z ())
						return true;

					break;
				case 4:
				case 5:

					if (intersection .y () >= min .y () and intersection .y () <= max .y () and
					    intersection .z () >= min .z () and intersection .z () <= max .z ())
						return true;

					break;
			}
		}
	}

	return false;
}

template <class Type>
bool
box3 <Type>::intersect (const sphere3 <Type> & sphere) const
{
	auto size1_2 = size () / Type (2);
	auto min     = center () - size1_2;
	auto max     = center () + size1_2;

	vector3 <Type> center = sphere .center ();
	Type           radius = sphere .radius ();

	return min .x () <= center .x () + radius and
	       max .x () >= center .x () - radius and
	       min .y () <= center .y () + radius and
	       max .y () >= center .y () - radius and
	       min .z () <= center .z () + radius and
	       max .z () >= center .z () - radius;
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
///  Return false if @a lhs is not equal to @a rhs.
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
	return box3 <Type> (lhs) .multBoxMatrix (rhs);
}

///  Return new box3 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box3 <Type>
operator * (const matrix4 <Type> & lhs, const box3 <Type> & rhs)
{
	return box3 <Type> (rhs) .multMatrixBox (lhs);
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
