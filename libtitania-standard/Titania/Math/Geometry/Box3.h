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
#include "../Utility/Types.h"
#include "../../Utility/Adapter.h"
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
		value (box .matrix ())
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
	box3 (const vector3 <Type> & min, const vector3 <Type> & max, const extends_type &) :
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
			min = math::min (min, (vector3 <Type>) *first);
			max = math::max (max, (vector3 <Type>) *first);
		}

		*this = box3 (min, max, extends_type ());
	}

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

	///  Returns the min and max extends of this box.
	void
	extends (vector3 <Type> &, vector3 <Type> &) const;

	///  Returns the size of this box.
	vector3 <Type>
	size () const;

	///  Returns the center of this box.
	vector3 <Type>
	center () const
	{ return value .origin (); }

	///  Returns whether this box is an empty box.
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
	intersect (const line3 <Type> &) const;

	///  Returns true if @a sphere intersects with this box3.
	bool
	intersect (const sphere3 <Type> &) const;

	///  Returns true if this box contains @a box.
	bool
	contains (const box3 &) const;


private:

	///  Returns the absolute min and max extends of this box.
	void
	absolute_extends (vector3 <Type> &, vector3 <Type> &) const;

	matrix4 <Type> value;

};

template <class Type>
inline
void
box3 <Type>::extends (vector3 <Type> & min, vector3 <Type> & max) const
{
	absolute_extends (min, max);

	min += center ();
	max += center ();
}

template <class Type>
inline
vector3 <Type>
box3 <Type>::size () const
{
	vector3 <Type> min, max;

	absolute_extends (min, max);

	return max - min;
}

template <class Type>
void
box3 <Type>::absolute_extends (vector3 <Type> & min, vector3 <Type> & max) const
{
	vector3 <Type> x (value .x ());
	vector3 <Type> y (value .y ());
	vector3 <Type> z (value .z ());

	auto r1 = y + z;
	auto r2 = z - y;

	auto p1 =  x + r1;
	auto p2 =  x + r2;
	auto p3 = r1 -  x;
	auto p4 = r2 -  x;

	auto p5 = -p1;
	auto p6 = -p2;
	auto p7 = -p3;
	auto p8 = -p4;

	min = math::min ({ p1, p2, p3, p4, p5, p6, p7, p8 });
	max = math::max ({ p1, p2, p3, p4, p5, p6, p7, p8 });
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

	vector3 <Type> lhs_min, lhs_max, rhs_min, rhs_max;
	
	extends (lhs_min, lhs_max);
	box .extends (rhs_min, rhs_max);

	return *this = box3 (math::min (lhs_min, rhs_min), math::max (lhs_max, rhs_max), extends_type ());
}

template <class Type>
bool
box3 <Type>::intersect (const vector3 <Type> & point) const
{
	vector3 <Type> min, max;

	extends (min, max);

	return min .x () <= point .x () and
	       max .x () >= point .x () and
	       min .y () <= point .y () and
	       max .y () >= point .y () and
	       min .z () <= point .z () and
	       max .z () >= point .z ();
}

template <class Type>
bool
box3 <Type>::intersect (const line3 <Type> & line) const
{
	vector3 <Type> min, max;

	extends (min, max);

	static const vector3 <Type> normals [5] = {
		vector3 <Type> (0,  0,  1), // front
		vector3 <Type> (0,  0, -1), // back
		vector3 <Type> (0,  1,  0), // top
		vector3 <Type> (0, -1,  0), // bottom
		vector3 <Type> (1,  0,  0)  // right
		// left: We do not have to test for left.
	};

	vector3 <Type> intersection;

	for (size_t i = 0; i < 5; ++ i)
	{
		if (plane3 <Type> (is_odd (i) ? min : max, normals [i]) .intersect (line, intersection))
		{
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
box3 <Type>::intersect (const sphere3 <Type> & sphere) const
{
	vector3 <Type> min, max;

	extends (min, max);

	vector3 <Type> center = sphere .center ();
	Type           radius = sphere .radius ();

	return min .x () <= center .x () + radius and
	       max .x () >= center .x () - radius and
	       min .y () <= center .y () + radius and
	       max .y () >= center .y () - radius and
	       min .z () <= center .z () + radius and
	       max .z () >= center .z () - radius;
}

template <class Type>
bool
box3 <Type>::contains (const box3 <Type> & box) const
{
	if (empty () or box .empty ())
		return false;

	vector3 <Type> min1, max1, min2, max2;

	extends (min1, max1);
	box .extends (min2, max2);

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
