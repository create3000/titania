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

#include "../Functional.h"
#include "Vector3.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

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
template <class Type>
class color3
{
private:

	static constexpr size_t Size = 3;

	///  @name Member types

	using array_type = std::array <Type, Size>;


public:

	///  @name Member types

	///  Type.
	using value_type = Type;

	///  Size typedef.  Used for size and indices.
	using size_type = typename array_type::size_type;

	///  std::ptrdiff_t
	using difference_type = typename array_type::difference_type;

	///  value_type &
	using reference = typename array_type::reference;

	///  const value_type &
	using const_reference = typename array_type::const_reference;

	///  value_type*
	using pointer = typename array_type::pointer;

	///  const value_type*
	using const_pointer = typename array_type::const_pointer;

	///  Random access iterator
	using iterator = typename array_type::iterator;

	///  Constant random access iterator 
	using const_iterator = typename array_type::const_iterator;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = typename array_type::reverse_iterator;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = typename array_type::const_reverse_iterator;

	///  @name Constructors

	///  Default constructor.  All values default to 0.
	constexpr
	color3 () :
		m_array { Type (), Type (), Type () }
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	color3 (const color3 <Up> & color) :
		m_array { color .r (), color .g (), color .b () }
	{ }

	///  Components constructor. Set values to @a x, @a y and @a z.
	constexpr
	color3 (const Type & r, const Type & g, const Type & b) :
		m_array { clamp (r, Type (), Type (1)),
		          clamp (g, Type (), Type (1)),
		          clamp (b, Type (), Type (1)) }
	{ }

	///  Components constructor. Set values to @a v.
	explicit
	constexpr
	color3 (const Type & v) :
		m_array { clamp (v, Type (), Type (1)),
		          clamp (v, Type (), Type (1)),
		          clamp (v, Type (), Type (1)) }
	{ }

	///  @name Assignment operator

	///  Assign @a other to this color.
	template <class Up>
	color3 &
	operator = (const color3 <Up> & other);

	///  @name Element access

	///  Set red component of this color.
	void
	r (const Type & value)
	{ m_array [0] = clamp (value, Type (), Type (1)); }

	///  Return red component of this color.
	constexpr
	const_reference
	r () const
	{ return m_array [0]; }

	///  Set green component of this color.
	void
	g (const Type & value)
	{ m_array [1] = clamp (value, Type (), Type (1)); }

	///  Return green component of this color.
	constexpr
	const_reference
	g () const { return m_array [1]; }

	///  Set blue component of this color.
	void
	b (const Type & value)
	{ m_array [2] = clamp (value, Type (), Type (1)); }

	///  Return blue component of this color.
	constexpr
	const_reference
	b () const
	{ return m_array [2]; }

	///  Access specified element with bounds checking.
	constexpr
	reference
	at (const size_type index)
	{ return m_array .at (index); }

	///  Access specified element with bounds checking.
	constexpr
	const_reference
	at (const size_type index) const
	{ return m_array .at (index); }

	///  Access components by @a index.
	constexpr
	reference
	operator [ ] (const size_type index)
	{ return m_array [index]; }

	///  Access components by @a index.
	constexpr
	const_reference
	operator [ ] (const size_type index) const
	{ return m_array [index]; }

	///  Returns a reference to the first element in the container. 
	constexpr
	reference
	front ()
	{ return m_array .front (); }

	///  Returns a reference to the first element in the container. 
	constexpr
	const_reference
	front () const
	{ return m_array .front (); }

	///  Returns reference to the last element in the container. 
	constexpr
	reference
	back ()
	{ return m_array .back (); }

	///  Returns reference to the last element in the container. 
	constexpr
	const_reference
	back () const
	{ return m_array .back (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	pointer
	data ()
	{ return m_array .data (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	const_pointer
	data () const
	{ return m_array .data (); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	constexpr
	iterator
	begin ()
	{ return m_array .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	begin () const
	{ return m_array .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	cbegin () const
	{ return m_array .cbegin (); }

	///  Returns an iterator to the end.
	constexpr
	iterator
	end ()
	{ return m_array .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	end () const
	{ return m_array .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	cend () const
	{ return m_array .cend (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	reverse_iterator
	rbegin ()
	{ return m_array .rbegin (); }

	///  returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	rbegin () const
	{ return m_array .rbegin (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	crbegin () const
	{ return m_array .crbegin (); }

	///  Returns a reverse iterator to the end.
	constexpr
	reverse_iterator
	rend ()
	{ return m_array .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	rend () const
	{ return m_array .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	crend () const
	{ return m_array .crend (); }

	///  @name Capacity

	///  Checks whether the container is empty. Always returns false.
	constexpr
	bool
	empty () const
	{ return false; }

	///  Returns the number of elements in the container.
	constexpr
	size_type
	size () const
	{ return Size; }

	///  Returns the maximum possible number of elements. Because each vector is a fixed-size container,
	///  the value is also the value returned by size.
	constexpr
	size_type
	max_size () const
	{ return Size; }

	///  @name Operations

	///  Fill the container with specified @a value. 
	void
	fill (const Type & value)
	{ m_array .fill (value); }

	///  Swaps the contents.
	void
	swap (color3 & other)
	{ m_array .swap (other .m_array); }

	///  @name Arithmetic operations

	///  Set RGB components for this color. RGB is in the range (0,255).
	void
	rgb (const vector3 <uint8_t> & RGB);

	///  Return RGB components of this color. RGB is in the range (0,255).
	vector3 <uint8_t>
	rgb () const;

	///  Set HSV components for this color.
	void
	hsv (const vector3 <Type> & hsv);

	///  Return HSV components of this color.
	vector3 <Type>
	hsv () const;


private:

	///  @name Members

	array_type m_array;

};

template <class Type>
template <class Up>
color3 <Type> &
color3 <Type>::operator = (const color3 <Up> & other)
{
	m_array [0] = other .r ();
	m_array [1] = other .g ();
	m_array [2] = other .b ();
	return *this;
}

template <typename Type>
void
color3 <Type>::rgb (const vector3 <uint8_t> & RGB)
{
	r (RGB [0] / Type (255));
	g (RGB [1] / Type (255));
	b (RGB [2] / Type (255));
}

template <typename Type>
vector3 <uint8_t>
color3 <Type>::rgb () const
{
	return vector3 <uint8_t> (std::round (r () * 255),
	                          std::round (g () * 255),
	                          std::round (b () * 255));
}

template <class Type>
void
color3 <Type>::hsv (const vector3 <Type> & hsv)
{
	auto h = hsv [0];
	auto s = hsv [1];
	auto v = hsv [2];

	// H is given on [0, 2 * Pi]. S and V are given on [0, 1].
	// RGB are each returned on [0, 1].

	v = clamp (v, Type (), Type (1));

	if (s == 0)
	{
		// achromatic (grey)
		m_array [0] = m_array [1] = m_array [2] = v;
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
			m_array [0] = v;
			m_array [1] = t;
			m_array [2] = p;
			return;

		case 1:
			m_array [0] = q;
			m_array [1] = v;
			m_array [2] = p;
			return;

		case 2:
			m_array [0] = p;
			m_array [1] = v;
			m_array [2] = t;
			return;

		case 3:
			m_array [0] = p;
			m_array [1] = q;
			m_array [2] = v;
			return;

		case 4:
			m_array [0] = t;
			m_array [1] = p;
			m_array [2] = v;
			return;

		default:
			m_array [0] = v;
			m_array [1] = p;
			m_array [2] = q;
			return;
	}
}

template <class Type>
vector3 <Type>
color3 <Type>::hsv () const
{
	Type h = 0;
	Type s = 0;
	Type v = 0;

	const Type min = std::min ({ r (), g (), b () });
	const Type max = std::max ({ r (), g (), b () });

	v = max;                           // v

	const Type delta = max - min;

	if (max not_eq 0 and delta not_eq 0)
	{
		s = delta / max;                // s
	
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
	else
	{
		// r = g = b = 0                // s = 0, h is undefined
		s = 0;
		h = 0;
	}

	return vector3 <Type> (h, s, v);
}

///  @relates color3
///  @name Utility functions

///  Construct a color from RGB.
template <class Type>	
color3 <Type>
make_rgb (const uint8_t R, const uint8_t G, const uint8_t B)
{
	color3 <Type> color;
	color .rgb (vector3 <uint8_t> (R, G, B));
	return color;
}

///  Construct a color from RGB.
template <class Type>	
color3 <Type>
make_rgb (const vector3 <uint8_t> & RGB)
{
	color3 <Type> color;
	color .rgb (RGB);
	return color;
}

///  Construct a color from hsv.
template <class Type>	
color3 <Type>
make_hsv (const Type & h, const Type & s, const Type & v)
{
	color3 <Type> color;
	color .hsv (vector3 <Type> (h, s, v));
	return color;
}

///  Construct a color from hsv.
template <class Type>	
color3 <Type>
make_hsv (const vector3 <Type> & hsv)
{
	color3 <Type> color;
	color .hsv (hsv);
	return color;
}

///  @relates color3
///  @name Comparision operations

///  Return true if @a a is equal to @a b.
template <class Type>
constexpr bool
operator == (const color3 <Type> & a, const color3 <Type> & b)
{
	return a .r () == b .r () and
	       a .g () == b .g () and
	       a .b () == b .b ();
}

///  Return false if @a a is not equal to @a b.
template <class Type>
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
template <class Type>
color3 <Type>
lerp (const color3 <Type> & source, const color3 <Type> & destination, const Type & t)
{
	const auto a = vector3 <Type> (source .r (), source .g (), source .b ());
	const auto b = vector3 <Type> (destination .r (), destination .g (), destination .b ());
	const auto r = lerp (a, b, t);

	return color3 <Type> (r .x (), r .y (), r .z (), r .w ());
}

///  Circular linear interpolate between @a source color and @a destination color in hsv space by an amout of @a t.
template <class Type>
static
vector3 <Type>
hsv_lerp (const vector3 <Type> & source, const vector3 <Type> & destination, const Type & t)
{
	const auto & a     = source;
	const auto & b     = destination;
	auto         ha    = a [0];
	const auto   sa    = a [1];
	const auto   va    = a [2];
	auto         hb    = b [0];
	const auto   sb    = b [1];
	const auto   vb    = b [2];
	const auto   range = std::abs (hb - ha);

	if (sa == 0)
		ha = hb;

	if (sb == 0)
		hb = ha;

	if (range <= pi <Type>)
	{
		return vector3 <Type> (lerp (ha, hb, t),
		                       lerp (sa, sb, t),
		                       lerp (va, vb, t));
	}
	else
	{
		const Type step = (pi2 <Type>- range) * t;
		Type       h    = ha < hb ? ha - step : ha + step;

		if (h < 0)
			h += pi2 <Type>;

		else if (h > pi2 <Type>)
			h -= pi2 <Type>;

		return vector3 <Type> (h,
		                       lerp (sa, sb, t),
		                       lerp (va, vb, t));
	}
}

///  @relates color3
///  @name Input/Output operations

///  Extraction operator for color values.
template <class CharT, class Traits, class Type>
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
template <class CharT, class Traits, class Type>
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

namespace std {

/// Provides access to the number of elements in an color3 as a compile-time constant expression. 
template< class Type>
class tuple_size <titania::math::color3 <Type>> :
    public integral_constant <size_t, titania::math::color3 <Type> () .size ()>
{ };

/// Provides compile-time indexed access to the type of the elements of the color3 using tuple-like interface.
template <std::size_t I, class Type>
struct tuple_element <I, titania::math::color3 <Type>>
{
	using type = Type;
};

///  Extracts the Ith element element from the color.
template <size_t I, class Type>
inline
constexpr
Type &
get (titania::math::color3 <Type> & color)
{
	return color [I];
}

///  Extracts the Ith element element from the color.
template <size_t I, class Type>
inline
constexpr
const Type &
get (const titania::math::color3 <Type> & color)
{
	return color [I];
}

///  Extracts the Ith element element from the color.
template <size_t I, class Type>
inline
constexpr
Type &&
get (titania::math::color3 <Type> && color)
{
	return std::move (color [I]);
}

///  Extracts the Ith element element from the color.
template <size_t I, class Type>
inline
constexpr
const Type &&
get (const titania::math::color3 <Type> && color)
{
	return std::move (color [I]);
}

/// Specializes the std::swap algorithm for color3.
template <class Type>
inline
void
swap (titania::math::color3 <Type> & lhs, titania::math::color3 <Type> & rhs) noexcept
{
	lhs .swap (rhs);
}

} // std

#endif
