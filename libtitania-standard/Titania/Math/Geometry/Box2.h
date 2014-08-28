/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_MATH_GEOMETRY_BOX2_H__
#define __TITANIA_MATH_GEOMETRY_BOX2_H__

#include "../Numbers/Matrix3.h"
#include "../Numbers/Vector2.h"
#include "../Utility/Types.h"
#include "../Functional.h"
#include "../../Utility/Range.h"
#include <array>

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

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor. Constructs a box of size 0 0 and center 0 0.
	constexpr
	box2 () :
		value (Type (0.5), 0, 0,
		       0, Type (0.5), 0,
		       0, 0, 0)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	box2 (const box2 <Up> & box) :
		value (box .matrix ())
	{ }

	///  Constructs a box of size @a size and center @a size.
	constexpr
	box2 (const vector2 <Type> & size, const vector2 <Type> & center) :
		value (size .x () / 2, 0, 0,
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

	///  @name Assignment operator

	///  Assign @a box2 to this box2.
	template <class Up>
	box2 &
	operator = (const box2 <Up> & box)
	{
		value = box .value;
		return *this;
	}

	///  @name Element access

	const matrix3 <Type> &
	matrix () const
	{ return value; }

	///  Returns the min and max extents of this box.
	void
	extents (vector2 <Type> &, vector2 <Type> &) const;

	///  Returns the size of this box.
	vector2 <Type>
	size () const;

	///  Returns the center of this box.
	vector2 <Type>
	center () const
	{ return value .origin (); }

	///  Returns whether this box is an empty box.
	bool
	empty () const
	{ return value [2] [2] == 0; }

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
		mult_box_matrix (matrix);
		return *this;
	}

	///  Transform this box by matrix.
	void
	mult_matrix_box (const matrix3 <Type> & matrix)
	{ value .mult_left (matrix); }

	///  Transform this box by matrix.
	void
	mult_box_matrix (const matrix3 <Type> & matrix)
	{ value .mult_right (matrix); }

	///  Translate this box by @a translation.
	void
	translate (const vector2 <Type> & translation)
	{
		matrix3 <Type> t;

		t .translate (translation);
		value *= t;
	}

	///  Rotate this box by @a rotation.
	void
	rotate (const Type & rotation)
	{ value *= matrix3 <Type> (rotation); }

	///  Scale this box by @a scaleFactor.
	void
	scale (const vector2 <Type> & scaleFactor)
	{
		matrix3 <Type> s;

		s .scale (scaleFactor);
		value *= s;
	}

	///  @name Intersection

	///  Returns true if @a point is inside this box3 min and max extend.
	bool
	intersect (const vector2 <Type> &) const;

	///  Returns true if this box contains @a box.
	bool
	contains (const box2 & box) const;

private:

	///  Returns the absolute min and max extents of this box.
	void
	absolute_extents (vector2 <Type> &, vector2 <Type> &) const;

	matrix3 <Type> value;

};

template <class Type>
inline
void
box2 <Type>::extents (vector2 <Type> & min, vector2 <Type> & max) const
{
	absolute_extents (min, max);

	min += center ();
	max += center ();
}

template <class Type>
inline
vector2 <Type>
box2 <Type>::size () const
{
	vector2 <Type> min, max;

	absolute_extents (min, max);

	return max - min;
}

template <class Type>
void
box2 <Type>::absolute_extents (vector2 <Type> & min, vector2 <Type> & max) const
{
	const vector2 <Type> x (value .x ());
	const vector2 <Type> y (value .y ());

	const auto p1 = x + y;
	const auto p2 = y - x;
	const auto p3 = -p1;
	const auto p4 = -p2;

	min = math::min ({ p1, p2, p3, p4 });
	max = math::max ({ p1, p2, p3, p4 });
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

	vector2 <Type> lhs_min, lhs_max, rhs_min, rhs_max;
	
	extents (lhs_min, lhs_max);
	box .extents (rhs_min, rhs_max);

	return *this = box2 (math::min (lhs_min, rhs_min), math::max (lhs_max, rhs_max), extents_type ());
}

template <class Type>
bool
box2 <Type>::intersect (const vector2 <Type> & point) const
{
	vector2 <Type> min, max;

	extents (min, max);

	return min .x () <= point .x () and
	       max .x () >= point .x () and
	       min .y () <= point .y () and
	       max .y () >= point .y ();
}

template <class Type>
bool
box2 <Type>::contains (const box2 <Type> & box) const
{
	if (empty () or box .empty ())
		return false;

	vector2 <Type> min1, max1, min2, max2;

	extents (min1, max1);
	box .extents (min2, max2);

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
	result .mult_box_matrix (rhs);
	return result;
}

///  Return new box2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box2 <Type>
operator * (const matrix3 <Type> & lhs, const box2 <Type> & rhs)
{
	box2 <Type> result (rhs);
	result .mult_matrix_box (lhs);
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

} // math
} // titania

#endif
