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

#ifndef __TITANIA_MATH_NUMBERS_COLOR3_H__
#define __TITANIA_MATH_NUMBERS_COLOR3_H__

#include <algorithm>
#include <cmath>
#include <istream>
#include <ostream>

#include "../Functional.h"

namespace titania {
namespace math {

template <typename Type>
class color3
{
public:

	///  Value typedef.
	typedef Type value_type;

	// Size typedef.  Used for size and indices.
	typedef size_t size_type;

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

	///  Construct a color from hsv.
	static
	color3
	HSV (const Type & h, const Type & s, const Type & v)
	{
		color3 <Type> color;
		color .setHSV (h, s, v);
		return color;
	}

	///  Return number of components.
	static
	constexpr size_type
	size ()
	{ return 3; }

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

	//  Set all components.
	void
	set (const Type &, const Type &, const Type &);

	//  Get all components.
	template <typename T>
	void
	get (T &, T &, T &) const;

	///  Return hsv components of this color.
	void
	setHSV (const Type &, Type, Type);

	///  Set hsv components of this color.
	void
	getHSV (Type &, Type &, Type &) const;


private:

	Type value [size ()];

};

template <typename Type>
void
color3 <Type>::set (const Type & r, const Type & g, const Type & b)
{
	value [0] = clamp (r, Type (0), Type (1));
	value [1] = clamp (g, Type (0), Type (1));
	value [2] = clamp (b, Type (0), Type (1));
}

template <typename Type>
template <typename T>
void
color3 <Type>::get (T & r, T & g, T & b) const
{
	r = value [0];
	g = value [1];
	b = value [2];
}

template <typename Type>
void
color3 <Type>::setHSV (const Type & h, Type s, Type v)
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

	Type w = degree (interval (h, Type (), Type (M_PI2))) / 60; // sector 0 to 5

	Type i = std::floor (w);
	Type f = w - i;                                             // factorial part of h
	Type p = v * (1 - s);
	Type q = v * (1 - s * f);
	Type t = v * (1 - s * (1 - f));

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
color3 <Type>::getHSV (Type & h, Type & s, Type & v) const
{
	Type min = std::min ({ r (), g (), b () });
	Type max = std::max ({ r (), g (), b () });

	v = max;                           // v

	Type delta = max - min;

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

	h *= Type (M_PI) / 3;              // radiants

	if (h < 0)
		h += 2 * Type (M_PI);
}

// Operators:
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

///  Circular linear interpolate between @a source color and @a destination color in hsv space by an amout of @a t.
template <typename Type>
color3 <Type>
clerp (const color3 <Type> & source, const color3 <Type> & destination, const Type & t)
{
	Type
	   a_h, a_s, a_v,
	   b_h, b_s, b_v;

	source      .getHSV (a_h, a_s, a_v);
	destination .getHSV (b_h, b_s, b_v);

	Type range = std::abs (b_h - a_h);

	if (range <= Type (M_PI))
	{
		return color3 <Type>::HSV (lerp (a_h, b_h, t),
		                           lerp (a_s, b_s, t),
		                           lerp (a_v, b_v, t));
	}
	else
	{
		Type step = (Type (M_PI2) - range) * t;
		Type h    = a_h < b_h ? a_h - step : a_h + step;

		if (h < 0)
			h += Type (M_PI2);

		else if (h > Type (M_PI2))
			h -= Type (M_PI2);

		return color3 <Type>::HSV (h,
		                           lerp (a_s, b_s, t),
		                           lerp (a_v, b_v, t));
	}
}

template <class Type>
static
void
hsv_lerp (const Type & a_h, const Type & a_s, const Type & a_v,
          const Type & b_h, const Type & b_s, const Type & b_v,
          const Type & t,
          Type & r_h, Type & r_s, Type & r_v)
{
	Type range = std::abs (b_h - a_h);

	if (range <= Type (M_PI))
	{
		r_h = lerp (a_h, b_h, t);
		r_s = lerp (a_s, b_s, t);
		r_v = lerp (a_v, b_v, t);
	}
	else
	{
		Type step = (Type (M_PI2) - range) * t;
		Type h    = a_h < b_h ? a_h - step : a_h + step;

		if (h < 0)
			h += Type (M_PI2);

		else if (h > Type (M_PI2))
			h -= Type (M_PI2);

		r_h = h;
		r_s = lerp (a_s, b_s, t);
		r_v = lerp (a_v, b_v, t);
	}
}

///  Extraction operator for color values.
template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, color3 <Type> & color)
{
	Type r, g, b;

	istream >> r >> g >> b;

	if (istream)
		color .set (r, g, b);

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
