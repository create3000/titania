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

#ifndef __TITANIA_MATH_GEOMETRY_BOX2_H__
#define __TITANIA_MATH_GEOMETRY_BOX2_H__

#include "../Numbers/Matrix3.h"
#include "../Numbers/Vector2.h"
#include <array>

namespace titania {
namespace math {

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
		value (box .value)
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
	box2 (const vector2 <Type> & min, const vector2 <Type> & max, bool) :
		box2 (max - min, (max + min) / Type (2))
	{ }

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

	///  Return the size of this box.
	vector2 <Type>
	size () const;

	///  Return the center of this box.
	vector2 <Type>
	center () const
	{ return value .translation (); }

	///  Return whether this box is an empty box.
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
		return multBoxMatrix (matrix);
	}

	///  Transform this box by matrix.
	box2 &
	multMatrixBox (const matrix3 <Type> & matrix)
	{
		value .multLeft (matrix);
		return *this;
	}

	///  Transform this box by matrix.
	box2 &
	multBoxMatrix (const matrix3 <Type> & matrix)
	{
		value .multRight (matrix);
		return *this;
	}

private:

	matrix3 <Type> value;

};

template <class Type>
vector2 <Type>
box2 <Type>::size () const
{
	vector2 <Type> x (value [0] [0], value [0] [1]);
	vector2 <Type> y (value [1] [0], value [1] [1]);

	auto p1 =  x + y;
	auto p2 = -x + y;
	auto p3 = -x - y;
	auto p4 =  x - y;

	vector2 <Type> min, max;

	min = math::min (p1, p2);
	min = math::min (min, p3);
	min = math::min (min, p4);

	max = math::max (p1, p2);
	max = math::max (max, p3);
	max = math::max (max, p4);

	return max - min;
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

	auto lsize1_2 = size () / Type (2);
	auto lhs_min  = center () - lsize1_2;
	auto lhs_max  = center () + lsize1_2;

	auto rsize1_2 = box .size () / Type (2);
	auto rhs_min  = box .center () - rsize1_2;
	auto rhs_max  = box .center () + rsize1_2;

	return *this = box2 (math::min (lhs_min, rhs_min), math::max (lhs_max, rhs_max), true);
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
///  Return false if @a lhs is not equal to @a rhs.
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
	return box2 <Type> (lhs) .multBoxMatrix (rhs);
}

///  Return new box2 value @a rhs transformed by matrix @a lhs.
template <class Type>
inline
box2 <Type>
operator * (const matrix3 <Type> & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (rhs) .multMatrixBox (lhs);
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
