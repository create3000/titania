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

#ifndef __VECTOR4_H__
#define __VECTOR4_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

namespace titania {
namespace math {

/**
 *  Template to represent xvector4 numbers.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values x, y, z and w.
 */
template <class Type>
class xvector4
{
public:

	///  @name Member types

	///  Type.
	using value_type = Type;

	///  Size typedef.  Used for size and indices.
	using size_type = size_t;

	///  Random access iterator
	using iterator = Type *;

	///  Constant random access iterator
	using const_iterator = const Type *;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = std::reverse_iterator <iterator>;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = std::reverse_iterator <const_iterator>;

	///  @name Constructors

	///  Default constructor.  All values default to 0.
	constexpr
	xvector4 () :
		value {Type (), Type (), Type (), Type ()}
	{ }

	///  Copy constructor.
	template <class T>
	constexpr
	xvector4 (const xvector4 <T> & vector) :
		value {vector .x (), vector .y (), vector .z (), vector .w ()}
	{ }

	///  Components constructor. Set values to @a x, @a y, @a z and @a w.
	constexpr
	xvector4 (const Type & x, const Type & y, const Type & z, const Type & w) :
		value {x, y, z, w}
	{ }

	///  Components constructor. Set values to @a v.
	explicit
	constexpr
	xvector4 (const Type & v) :
		value {v, v, v, v}
	{ }

	template <class ... Args>
	xvector4 (const Args & ... args)
	{
		size_type i = 0;

		resolve (i, args ...);
	}

	template <class Arg, class ... Args>
	void
	resolve (size_type & i, const Arg & arg, const Args & ... args)
	{
		construct (i, arg);

		resolve (i, args ...);
	}

	void
	resolve (size_type & i)
	{ }

	void
	construct (size_type & i, const Type & arg)
	{
		value [i ++] = arg;
	}

	void
	construct (size_type & i, const vector2 <Type> & arg)
	{
		for (const auto & v : arg)
		{
			if (i < size ())
				value [i ++] = v;
		}
	}

	void
	construct (size_type & i, const vector3 <Type> & arg)
	{
		for (const auto & v : arg)
		{
			if (i < size ())
				value [i ++] = v;
		}
	}

	void
	construct (size_type & i, const vector4 <Type> & arg)
	{
		for (const auto & v : arg)
		{
			if (i < size ())
				value [i ++] = v;
		}
	}

	///  @name Assignment operator

	///  Assign @a vector to this vector.
	template <class T>
	xvector4 &
	operator = (const xvector4 <T> &);

	///  @name Element access

	///  Set x component of this vector.
	void
	x (const Type & t)
	{ value [0] = t; }

	///  Return x component of this vector.
	const Type &
	x () const
	{ return value [0]; }

	///  Set y component of this vector.
	void
	y (const Type & t)
	{ value [1] = t; }

	///  Return y component of this vector.
	const Type &
	y () const
	{ return value [1]; }

	///  Set z component of this vector.
	void
	z (const Type & t)
	{ value [2] = t; }

	///  Return z component of this vector.
	const Type &
	z () const
	{ return value [2]; }

	///  Set w component of this vector.
	void
	w (const Type & t)
	{ value [3] = t; }

	///  Return w component of this vector.
	const Type &
	w () const
	{ return value [3]; }

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

	///  @name Iterators

	///  Returns an iterator to the beginning.
	iterator
	begin ()
	{ return data (); }

	///  Returns an iterator to the beginning.
	const_iterator
	begin () const
	{ return data (); }

	///  Returns an iterator to the beginning.
	const_iterator
	cbegin () const
	{ return data (); }

	///  Returns an iterator to the end.
	iterator
	end ()
	{ return data () + size (); }

	///  Returns an iterator to the end.
	const_iterator
	end () const
	{ return data () + size (); }

	///  Returns an iterator to the end.
	const_iterator
	cend () const
	{ return data () + size (); }

	///  Returns a reverse iterator to the beginning.
	reverse_iterator
	rbegin ()
	{ return std::make_reverse_iterator (end ()); }

	///  returns a reverse iterator to the beginning.
	const_reverse_iterator
	rbegin () const
	{ return std::make_reverse_iterator (end ()); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	crbegin () const
	{ return std::make_reverse_iterator (cend ()); }

	///  Returns a reverse iterator to the end.
	reverse_iterator
	rend ()
	{ return std::make_reverse_iterator (begin ()); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	rend () const
	{ return std::make_reverse_iterator (begin ()); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	crend () const
	{ return std::make_reverse_iterator (cbegin ()); }

	///  @name Capacity

	///  Return number of components.
	static
	constexpr
	size_type
	size ()
	{ return 4; }

	///  @name  Arithmetic operations
	///  All these operators modify this vector2 inplace.

	///  Negates this vector.
	void
	negate ();

	///  Add @a vector to this vector.
	template <class T>
	xvector4 &
	operator += (const xvector4 <T> & vector);

	///  Add @a t to this vector.
	xvector4 &
	operator += (const Type & t);

	///  Subtract @a vector from this vector.
	template <class T>
	xvector4 &
	operator -= (const xvector4 <T> & vector);

	///  Subtract @a t from this vector.
	xvector4 &
	operator -= (const Type & t);

	///  Multiply this vector by @a vector.
	template <class T>
	xvector4 &
	operator *= (const xvector4 <T> & vector);

	///  Multiply this vector by @a t.
	xvector4 &
	operator *= (const Type & t);

	///  Divide this vector by @a vector.
	template <class T>
	xvector4 &
	operator /= (const xvector4 <T> & vector);

	///  Divide this vector by @a t.
	xvector4 &
	operator /= (const Type & t);

	///  Normalize this vector in place.
	void
	normalize ();


private:

	Type value [size ()];

};

template <class Type>
template <class T>
xvector4 <Type> &
xvector4 <Type>::operator = (const xvector4 <T> & vector)
{
	value [0] = vector .x ();
	value [1] = vector .y ();
	value [2] = vector .z ();
	value [3] = vector .w ();
	return *this;
}

template <class Type>
void
xvector4 <Type>::negate ()
{
	value [0] = -value [0];
	value [1] = -value [1];
	value [2] = -value [2];
	value [3] = -value [3];
}

template <class Type>
template <class T>
xvector4 <Type> &
xvector4 <Type>::operator += (const xvector4 <T> & vector)
{
	value [0] += vector .x ();
	value [1] += vector .y ();
	value [2] += vector .z ();
	value [3] += vector .w ();
	return *this;
}

template <class Type>
xvector4 <Type> &
xvector4 <Type>::operator += (const Type & t)
{
	value [0] += t;
	value [1] += t;
	value [2] += t;
	value [3] += t;
	return *this;
}

template <class Type>
template <class T>
xvector4 <Type> &
xvector4 <Type>::operator -= (const xvector4 <T> & vector)
{
	value [0] -= vector .x ();
	value [1] -= vector .y ();
	value [2] -= vector .z ();
	value [3] -= vector .w ();
	return *this;
}

template <class Type>
xvector4 <Type> &
xvector4 <Type>::operator -= (const Type & t)
{
	value [0] -= t;
	value [1] -= t;
	value [2] -= t;
	value [3] -= t;
	return *this;
}

template <class Type>
template <class T>
xvector4 <Type> &
xvector4 <Type>::operator *= (const xvector4 <T> & vector)
{
	value [0] *= vector .x ();
	value [1] *= vector .y ();
	value [2] *= vector .z ();
	value [3] *= vector .w ();
	return *this;
}

template <class Type>
xvector4 <Type> &
xvector4 <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;
	value [2] *= t;
	value [3] *= t;
	return *this;
}

template <class Type>
template <class T>
xvector4 <Type> &
xvector4 <Type>::operator /= (const xvector4 <T> & vector)
{
	value [0] /= vector .x ();
	value [1] /= vector .y ();
	value [2] /= vector .z ();
	value [3] /= vector .w ();
	return *this;
}

template <class Type>
xvector4 <Type> &
xvector4 <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;
	value [3] /= t;
	return *this;
}

template <class Type>
void
xvector4 <Type>::normalize ()
{
	const Type length = abs (*this);

	if (length)
		*this /= length;
}

///  @relates xvector4
///  @name Comparision operations

///  Compares two xvector4 numbers.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator == (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return
	   lhs .x () == rhs .x () and
	   lhs .y () == rhs .y () and
	   lhs .z () == rhs .z () and
	   lhs .w () == rhs .w ();
}

///  Compares two xvector4 numbers.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator not_eq (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return
	   lhs .x () not_eq rhs .x () or
	   lhs .y () not_eq rhs .y () or
	   lhs .z () not_eq rhs .z () or
	   lhs .w () not_eq rhs .w ();
}

///  Lexicographically compares two xvector4 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return std::tie (lhs .x (), lhs .y (), lhs .z (), lhs .w ()) < std::tie (rhs .x (), rhs .y (), rhs .z (), rhs .w ());
}

///  Lexicographically compares two xvector4 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two xvector4 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two xvector4 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates xvector4
///  @name Arithmetic operations

///  Return @a lhs.
template <class Type>
inline
constexpr
xvector4 <Type>
operator + (const xvector4 <Type> & vector)
{
	return vector;
}

///  Return vector negation of @a lhs.
template <class Type>
inline
xvector4 <Type>
operator - (const xvector4 <Type> & vector)
{
	xvector4 <Type> result (vector);
	result .negate ();
	return result;
}

///  Return vector negation of @a lhs.
template <class Type>
inline
xvector4 <Type>
negate (const xvector4 <Type> & vector)
{
	xvector4 <Type> result (vector);
	result .negate ();
	return result;
}

///  Return new vector value @a lhs plus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator + (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (lhs) += rhs;
}

///  Return new vector value @a lhs plus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator + (const xvector4 <Type> & lhs, const Type & rhs)
{
	return xvector4 <Type> (lhs) += rhs;
}

///  Return new vector value @a lhs plus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator + (const Type & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (rhs) += lhs;
}

///  Return new vector value @a lhs minus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator - (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (lhs) -= rhs;
}

///  Return new vector value @a lhs minus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator - (const xvector4 <Type> & lhs, const Type & rhs)
{
	return xvector4 <Type> (lhs) -= rhs;
}

///  Return new vector value @a lhs minus @a rhs.
template <class Type>
inline
xvector4 <Type>
operator - (const Type & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (-rhs) += lhs;
}

///  Return new vector value @a lhs times @a rhs.
template <class Type>
inline
xvector4 <Type>
operator * (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (lhs) *= rhs;
}

///  Return new vector value @a lhs times @a rhs.
template <class Type>
inline
xvector4 <Type>
operator * (const xvector4 <Type> & lhs, const Type & rhs)
{
	return xvector4 <Type> (lhs) *= rhs;
}

///  Return new vector value @a lhs times @a rhs.
template <class Type>
inline
xvector4 <Type>
operator * (const Type & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (rhs) *= lhs;
}

///  Return new vector value @a lhs divided by @a rhs.
template <class Type>
inline
xvector4 <Type>
operator / (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (lhs) /= rhs;
}

///  Return new vector value @a lhs divided by @a rhs.
template <class Type>
inline
xvector4 <Type>
operator / (const xvector4 <Type> & lhs, const Type & rhs)
{
	return xvector4 <Type> (lhs) /= rhs;
}

///  Return new vector value @a lhs divided by @a rhs.
template <class Type>
xvector4 <Type>
operator / (const Type & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (lhs / rhs .x (),
	                        lhs / rhs .y (),
	                        lhs / rhs .z (),
	                        lhs / rhs .w ());
}

///  Return new vector value @a lhs dot @a rhs.
template <class Type>
constexpr
Type
dot (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return
	   lhs .x () * rhs .x () +
	   lhs .y () * rhs .y () +
	   lhs .z () * rhs .z () +
	   lhs .w () * rhs .w ();
}

///  Returns the @a vector magnitude squared.
template <class Type>
inline
constexpr
Type
norm (const xvector4 <Type> & vector)
{
	return dot (vector, vector);
}

///  Return the @a vector magnitude.
template <class Type>
inline
constexpr
Type
abs (const xvector4 <Type> & vector)
{
	return std::sqrt (norm (vector));
}

///  Returns @a vector normalized.
template <class Type>
inline
xvector4 <Type>
normalize (const xvector4 <Type> & vector)
{
	xvector4 <Type> result (vector);
	result .normalize ();
	return result;
}

///  Returns the reflection vector for an incident vector.
template <class Type>
inline
xvector4 <Type>
reflect (const xvector4 <Type> & vector, const xvector4 <Type> & normal)
{
	return vector - (normal * (2 * dot (vector, normal)));
}

///  Returns the distance between @a source and @a destination.
template <class Type>
inline
Type
distance (const xvector4 <Type> & source, const xvector4 <Type> & destination)
{
	return abs (destination - source);
}

/**
 * @returns the lesser of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
min (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (std::min (lhs .x (), rhs .x ()),
	                        std::min (lhs .y (), rhs .y ()),
	                        std::min (lhs .z (), rhs .z ()),
	                        std::min (lhs .w (), rhs .w ()));
}

/**
 * @returns the greater of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
max (const xvector4 <Type> & lhs, const xvector4 <Type> & rhs)
{
	return xvector4 <Type> (std::max (lhs .x (), rhs .x ()),
	                        std::max (lhs .y (), rhs .y ()),
	                        std::max (lhs .z (), rhs .z ()),
	                        std::max (lhs .w (), rhs .w ()));
}

/**
 * @returns Computes nearest integer not greater than arg.
 * @param a xvector4 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
floor (const xvector4 <Type> & arg)
{
	return xvector4 <Type> (std::floor (arg .x ()),
	                        std::floor (arg .y ()),
	                        std::floor (arg .z ()),
	                        std::floor (arg .w ()));
}

/**
 * @returns Computes nearest integer not less than arg.
 * @param a xvector4 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
ceil (const xvector4 <Type> & arg)
{
	return xvector4 <Type> (std::ceil (arg .x ()),
	                        std::ceil (arg .y ()),
	                        std::ceil (arg .z ()),
	                        std::ceil (arg .w ()));
}

/**
 * @returns Computes the nearest integer value to arg (in floating-point format),
 *          rounding halfway cases away from zero, regardless of the current rounding mode.
 * @param a xvector4 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
round (const xvector4 <Type> & arg)
{
	return xvector4 <Type> (std::round (arg .x ()),
	                        std::round (arg .y ()),
	                        std::round (arg .z ()),
	                        std::round (arg .w ()));
}

/**
 * @returns Clamps @a arg between @a min and @a max .
 * @param a xvector4 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
xvector4 <Type>
clamp (const xvector4 <Type> & arg, const Type & min, const Type & max)
{
	return xvector4 <Type> (clamp (arg .x (), min, max),
	                        clamp (arg .y (), min, max),
	                        clamp (arg .z (), min, max),
	                        clamp (arg .w (), min, max));
}

///  @relates xvector4
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, xvector4 <Type> & vector)
{
	Type x, y, z, w;

	istream >> x >> y >> z >> w;

	if (istream)
		vector = xvector4 <Type> (x, y, z, w);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const xvector4 <Type> & vector)
{
	return ostream
	       << vector .x () << ' '
	       << vector .y () << ' '
	       << vector .z () << ' '
	       << vector .w ();
}

} // math
} // titania

#endif
