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

#ifndef __TITANIA_MATH_NUMBERS_COLOR3_H__
#define __TITANIA_MATH_NUMBERS_COLOR3_H__

#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

#include "../Functional.h"
#include "Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent color3 numbers.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values r, g and b.
 */
template <typename Type>
class color3
{
public:

	///  Value typedef.
	typedef Type value_type;

	// Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  @name Constructors

	///  Default constructor.  All values default to 0.
	constexpr
	color3 () :
		value { Type (), Type (), Type () }

	{ }

	///  Copy constructor.
	template <typename T>
	constexpr
	color3 (const color3 <T> & color) :
		value { color .r (), color .g (), color .b () }

	{ }

	///  Components constructor. Set values to @a x, @a y and @a z.
	constexpr
	color3 (const Type & r, const Type & g, const Type & b) :
		value
	{
		clamp (r, Type (), Type (1)),
		clamp (g, Type (), Type (1)),
		clamp (b, Type (), Type (1))
	}

	{ }

	///  @name Element access

	///  Set red component of this color.
	void
	r (const Type & r)
	{ value [0] = clamp (r, Type (), Type (1)); }

	///  Return red component of this color.
	const Type &
	r () const
	{ return value [0]; }

	///  Set green component of this color.
	void
	g (const Type & g)
	{ value [1] = clamp (g, Type (), Type (1)); }

	///  Return green component of this color.
	const Type &
	g () const { return value [1]; }

	///  Set blue component of this color.
	void
	b (const Type & b)
	{ value [2] = clamp (b, Type (), Type (1)); }

	///  Return blue component of this color.
	const Type &
	b () const
	{ return value [2]; }

	///  Access components by @a index.
	Type &
	operator [ ] (const size_type index)
	{ return value [index]; }

	///  Access components by @a index.
	const Type &
	operator [ ] (const size_type index) const
	{ return value [index]; }

	///  Returns pointer to the underlying array serving as element storage.
	Type*
	data ()
	{ return value; }

	///  Returns pointer to the underlying array serving as element storage.
	const Type*
	data () const
	{ return value; }

	///  @name Capacity

	///  Returns number of components.
	static
	constexpr
	size_type
	size ()
	{ return 3; }

	///  @name Arithmetic operations

	///  Return hsv components of this color.
	void
	set_hsv (const Type &, Type, Type);

	///  Set hsv components of this color.
	void
	get_hsv (Type &, Type &, Type &) const;


private:

	Type value [size ()];

};

template <typename Type>
void
color3 <Type>::set_hsv (const Type & h, Type s, Type v)
{
	// H is given on [0, 2 * Pi]. S and V are given on [0, 1].
	// RGB are each returned on [0, 1].

	v = clamp (v, Type (), Type (1));

	if (s == 0)
	{
		// achromatic (grey)
		value [0] = value [1] = value [2] = v;
		return;
	}

	s = clamp (s, Type (), Type (1));

	const Type w = degrees (interval (h, Type (), pi2 <Type>)) / 60; // sector 0 to 5

	const Type i = std::floor (w);
	const Type f = w - i;                                              // factorial part of h
	const Type p = v * (1 - s);
	const Type q = v * (1 - s * f);
	const Type t = v * (1 - s * (1 - f));

	switch ((size_t) i)
	{
		case 0:
			value [0] = v;
			value [1] = t;
			value [2] = p;
			return;

		case 1:
			value [0] = q;
			value [1] = v;
			value [2] = p;
			return;

		case 2:
			value [0] = p;
			value [1] = v;
			value [2] = t;
			return;

		case 3:
			value [0] = p;
			value [1] = q;
			value [2] = v;
			return;

		case 4:
			value [0] = t;
			value [1] = p;
			value [2] = v;
			return;

		default:
			value [0] = v;
			value [1] = p;
			value [2] = q;
			return;
	}
}

template <typename Type>
void
color3 <Type>::get_hsv (Type & h, Type & s, Type & v) const
{
	const Type min = std::min ({ r (), g (), b () });
	const Type max = std::max ({ r (), g (), b () });

	v = max;                           // v

	const Type delta = max - min;

	if (max not_eq 0 and delta not_eq 0)
		s = delta / max;                // s
	else
	{
		// r = g = b = 0                // s = 0, h is undefined
		s = 0;
		h = 0;
		return;
	}

	if (r () == max)
		h = (g () - b ()) / delta;      // between yellow & magenta

	else if (g () == max)
		h = 2 + (b () - r ()) / delta;  // between cyan & yellow

	else
		h = 4 + (r () - g ()) / delta;  // between magenta & cyan

	h *= pi <Type> / 3;              // radians

	if (h < 0)
		h += 2 * pi <Type>;
}

///  @relates color3
///  @name Utility functions

///  Construct a color from hsv.
template <typename Type>	
color3 <Type>
make_hsv (const Type & h, const Type & s, const Type & v)
{
	color3 <Type> color;
	color .set_hsv (h, s, v);
	return color;
}

///  @relates color3
///  @name Comparision operations

///  Return true if @a a is equal to @a b.
template <typename Type>
constexpr bool
operator == (const color3 <Type> & a, const color3 <Type> & b)
{
	return a .r () == b .r () and
	       a .g () == b .g () and
	       a .b () == b .b ();
}

///  Return false if @a a is not equal to @a b.
template <typename Type>
constexpr bool
operator not_eq (const color3 <Type> & a, const color3 <Type> & b)
{
	return a .r () not_eq b .r () or
	       a .g () not_eq b .g () or
	       a .b () not_eq b .b ();
}

///  Lexicographically compares two color3 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const color3 <Type> & lhs, const color3 <Type> & rhs)
{
	return std::tie (lhs .r (), lhs .g (), lhs .b ()) < std::tie (rhs .r (), rhs .g (), rhs .b ());
}

///  Lexicographically compares two color3 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const color3 <Type> & lhs, const color3 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two color3 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const color3 <Type> & lhs, const color3 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two color3 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const color3 <Type> & lhs, const color3 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates color3
///  @name Arithmetic operations

///  Linear interpolate between @a source and @a destination by an amout of @a t in RGB space.
template <typename Type>
color3 <Type>
lerp (const color3 <Type> & source, const color3 <Type> & destination, const Type & t)
{
	const auto a = vector3 <Type> (source .r (), source .g (), source .b ());
	const auto b = vector3 <Type> (destination .r (), destination .g (), destination .b ());
	const auto r = lerp (a, b, t);

	return color3 <Type> (r .x (), r .y (), r .z (), r .w ());
}

///  Circular linear interpolate between @a source color and @a destination color in hsv space by an amout of @a t in HSV space.
template <typename Type>
color3 <Type>
clerp (const color3 <Type> & source, const color3 <Type> & destination, const Type & t)
{
	Type
	   a_h, a_s, a_v,
	   b_h, b_s, b_v;

	source      .get_hsv (a_h, a_s, a_v);
	destination .get_hsv (b_h, b_s, b_v);

	const Type range = std::abs (b_h - a_h);

	if (range <= pi <Type>)
	{
		return make_hsv <Type> (lerp (a_h, b_h, t),
		                        lerp (a_s, b_s, t),
		                        lerp (a_v, b_v, t));
	}
	else
	{
		const Type step = (pi2 <Type> - range) * t;
		Type       h    = a_h < b_h ? a_h - step : a_h + step;

		if (h < 0)
			h += pi2 <Type>;

		else if (h > pi2 <Type>)
			h -= pi2 <Type>;

		return make_hsv <Type> (h,
		                        lerp (a_s, b_s, t),
		                        lerp (a_v, b_v, t));
	}
}

///  Circular linear interpolate between source color and destination color in hsv space by an amout of @a t.
template <class Type>
static
void
hsv_lerp (const Type & a_h, const Type & a_s, const Type & a_v,
          const Type & b_h, const Type & b_s, const Type & b_v,
          const Type & t,
          Type & r_h, Type & r_s, Type & r_v)
{
	const Type range = std::abs (b_h - a_h);

	if (range <= pi <Type>)
	{
		r_h = lerp (a_h, b_h, t);
		r_s = lerp (a_s, b_s, t);
		r_v = lerp (a_v, b_v, t);
	}
	else
	{
		const Type step = (pi2 <Type>- range) * t;
		Type       h    = a_h < b_h ? a_h - step : a_h + step;

		if (h < 0)
			h += pi2 <Type>;

		else if (h > pi2 <Type>)
			h -= pi2 <Type>;

		r_h = h;
		r_s = lerp (a_s, b_s, t);
		r_v = lerp (a_v, b_v, t);
	}
}

///  @relates color3
///  @name Input/Output operations

///  Extraction operator for color values.
template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, color3 <Type> & color)
{
	Type r, g, b;

	istream >> r >> g >> b;

	if (istream)
		color = color3 <Type> (r, g, b);

	return istream;
}

///  Insertion operator for color values.
template <typename CharT, class Traits, typename Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const color3 <Type> & color)
{
	return ostream
	       << color .r () << ' '
	       << color .g () << ' '
	       << color .b ();
}

extern template class color3 <float>;
extern template class color3 <double>;
extern template class color3 <long double>;

//
extern template std::istream & operator >> (std::istream &, color3 <float> &);
extern template std::istream & operator >> (std::istream &, color3 <double> &);
extern template std::istream & operator >> (std::istream &, color3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const color3 <float> &);
extern template std::ostream & operator << (std::ostream &, const color3 <double> &);
extern template std::ostream & operator << (std::ostream &, const color3 <long double> &);

} // math
} // titania

#endif
