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
 ******************************************************************************/

#ifndef __TITANIA_MATH_GEOMETRY_BOX2_H__
#define __TITANIA_MATH_GEOMETRY_BOX2_H__

#include "../Numbers/Vector2.h"

namespace titania {
namespace math {

template <class Type>
class box2
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor. Constructs a box of size 0 0 0 and center 0 0 0,
	constexpr
	box2 () :
		value ({ { } })
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	box2 (const box2 <Up> & box) :
		value ({ { box .min (), box .max () } })
	{ }

	///  Constructs a box of min @a min and max @a max.
	constexpr
	box2 (const vector2 <Type> & min, const vector2 <Type> & max, bool) :
		value ({ min, max })
	{ }

	///  Constructs a box of size @a size and center @a size.
	box2 (const vector2 <Type> & size, const vector2 <Type> & center)
	{
		vector2 <Type> size_1_2 = size / Type (2);
		value .min = center - size_1_2;
		value .max = center + size_1_2;
	}

	///  @name Assignment operator

	///  Assign @a vector to this vector.
	template <class Up>
	box2 &
	operator = (const box2 <Up> &);

	///  @name Element access

	///  Return the center of this box.
	const vector2 <Type> &
	min () const { return value .min; }

	const vector2 <Type> &
	max () const { return value .max; }

	///  Return the size of this box.
	constexpr vector2 <Type>
	size () const { return max () - min (); }

	///  Return the center of this box.
	constexpr vector2 <Type>
	center () const { return min () + size () / Type (2); }

	///  @name  Arithmetic operations
	///  All these operators modify this vector2 inplace.

	///  Add @a box2 to this box.
	template <class Up>
	box2 &
	operator += (const box2 <Up> & box)
	{
		if (box .min () not_eq box .max ())
		{
			value .min = math::min (value .min, box .min ());
			value .max = math::max (value .max, box .max ());
		}

		return *this;
	}

	///  Translate this box by @a translation.
	template <class Up>
	box2 &
	operator += (const vector2 <Up> & translation)
	{
		value .min += translation;
		value .max += translation;
		return *this;
	}

	///  Translate this box by @a translation.
	template <class Up>
	box2 &
	operator -= (const vector2 <Up> & translation)
	{
		value .min -= translation;
		value .max -= translation;
		return *this;
	}

	///  Scale this box by @a scale.
	box2 &
	operator *= (const Type & scale)
	{
		value .min *= scale;
		value .max *= scale;
		return *this;
	}

	///  Scale this box by @a scale.
	box2 &
	operator /= (const Type & scale)
	{
		value .min /= scale;
		value .max /= scale;
		return *this;
	}

private:

	struct Value
	{
		vector2 <Type> min;
		vector2 <Type> max;
	};

	Value value;

};

template <class Type>
template <class Up>
box2 <Type> &
box2 <Type>::operator = (const box2 <Up> & box)
{
	value .min = box .min ();
	value .max = box .max ();
	return *this;
}

///  @relates box2
///  @name Comparision operations

///  Compares two box2 numbers.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
constexpr bool
operator == (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return
	   lhs .min () == rhs .min () and
	   lhs .max () == rhs .max ();
}

///  Compares two box2 numbers.
///  Return false if @a lhs is not equal to @a rhs.
template <class Type>
constexpr bool
operator not_eq (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return
	   lhs .min () not_eq rhs .min () or
	   lhs .max () not_eq rhs .max ();
}

///  @relates box2
///  @name Arithmetic operations

///  Return new box value @a lhs plus @a rhs.
template <class Type>
inline
box2 <Type>
operator + (const box2 <Type> & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (lhs) += rhs;
}

///  Return new box value @a lhs translated @a rhs.
template <class Type>
inline
box2 <Type>
operator + (const box2 <Type> & lhs, const vector2 <Type> & rhs)
{
	return box2 <Type> (lhs) += rhs;
}

///  Return new box value @a rhs translated @a lhs.
template <class Type>
inline
box2 <Type>
operator + (const vector2 <Type> & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (rhs) += lhs;
}

///  Return new box value @a lhs translated @a rhs.
template <class Type>
inline
box2 <Type>
operator - (const box2 <Type> & lhs, const vector2 <Type> & rhs)
{
	return box2 <Type> (lhs) -= rhs;
}

///  Return new box value @a lhs scaled @a rhs.
template <class Type>
inline
box2 <Type>
operator * (const box2 <Type> & lhs, const Type & rhs)
{
	return box2 <Type> (lhs) *= rhs;
}

///  Return new box value @a rhs scaled @a lhs.
template <class Type>
inline
box2 <Type>
operator * (const Type & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (rhs) *= lhs;
}

///  Return new box value @a rhs scaled @a lhs.
template <class Type>
inline
box2 <Type>
operator / (const Type & lhs, const box2 <Type> & rhs)
{
	return box2 <Type> (rhs) /= lhs;
}

///  @relates box2
///  @name Input/Output operations

///  Extraction operator for vector values.
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

///  Insertion operator for vector values.
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
