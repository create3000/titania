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

#ifndef __TITANIA_MATH_FUNCTIONAL_H__
#define __TITANIA_MATH_FUNCTIONAL_H__

#include "../Math/Constants.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

namespace titania {
namespace math {

using std::abs;

template <typename Type>
inline
constexpr
int
signum (const Type & x, std::false_type is_signed)
{
	return Type (0) < x;
}

template <typename Type>
inline
constexpr
int
signum (const Type & x, std::true_type is_signed)
{
	return (Type (0) < x) - (x < Type (0));
}

template <typename Type>
inline
constexpr
int
signum (const Type & x)
{
	return signum (x, std::is_signed <Type> ());
}

template <class Type>
inline
constexpr
bool
is_odd (const Type & value)
{
	return value & Type (1);
}

template <class Type>
inline
constexpr
bool
is_even (const Type & value)
{
	return not is_odd (value);
}

///  Calculate the square of @a value.
template <class Type>
inline
constexpr
Type
sqr (const Type & value)
{
	return value * value;
}

///  Convert @a value from degrees to radians.
template <class Type>
inline
constexpr
Type
radians (const Type & value)
{
	return value * Type (pi <Type> / 180);
}

///  Convert @a value from radians to degrees.
template <class Type>
inline
constexpr
Type
degrees (const Type & value)
{
	return value * Type (180 / pi <Type>);
}

inline
constexpr
long double
operator "" _deg (const long double value)
{
	return radians (value);
}

inline
constexpr
long double
operator "" _rad (const long double value)
{
	return degrees (value);
}

template <class Type>
inline
constexpr
std::enable_if_t <std::is_floating_point_v <Type>, Type>
fract (const Type & value)
{
	Type intpart = 0;

	return std::modf (value, &intpart);
}

///  Wrap value in the interval (low;high) so low <= result < high.
template <class Type>
Type
interval (const Type & value, const Type & low, const Type & high)
{
	if (value >= high)
		return std::fmod ((value - low), high - low) + low;

	if (value < low)
		return std::fmod ((value - high), high - low) + high;

	return value;
}

///  Map @a value in the interval (@a fromLow;@a fromHigh) to the interval (@a toLow;@a toHigh).
template <class Type>
inline
constexpr
Type
project (const Type & value, const Type & fromLow, const Type & fromHigh, const Type & toLow, const Type & toHigh)
{
	return toLow + ((value - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow);
}

template <class Type>
typename Type::value_type
maximum_norm (const Type & value)
{
	if (value .size () == 0)
		return 0;

	typename Type::value_type norm = std::abs (value [0]);

	for (size_t i = 1, size = value .size (); i < size; ++ i)
	{
		norm = std::max (norm, std::abs (value [i]));
	}

	return norm;
}

///  Returns the distance between @a source and @a destination.
template <class Type>
inline
Type
distance (const Type & source, const Type & destination)
{
	return std::abs (destination - source);
}

///  Linear interpolate between @a source and @a destination by an amout of @a t.
template <class Type, class T>
inline
constexpr
Type
lerp (const Type & source, const Type & destination, const T & t)
{
	return source + t * (destination - source);
}

///  Spherical linear interpolate between two normal vectors @a source vector
///  and @a destination vector by an amout of @a t.  Always returns a normal vector
///  if the inputs vectors are normalized.
template <typename Type, typename T>
Type
slerp (const Type & source, Type destination, const T & t)
{
	T cosom = dot (source, destination);

	if (cosom <= -1)
		throw std::domain_error ("slerp is not possible: vectors are inverse collinear.");

	if (cosom >= 1) // both normal vectors are equal
		return destination;

	if (cosom < 0)
	{
		// Reverse signs so we travel the short way round
		cosom       = -cosom;
		destination = -destination;
	}

	const T omega = std::acos (cosom);
	const T sinom = std::sin  (omega);

	const T scale0 = std::sin ((1 - t) * omega);
	const T scale1 = std::sin (t * omega);

	return (scale0 * source + scale1 * destination) / sinom;
}

///  Spherical linear interpolate between two normal vectors @a source vector
///  and @a destination vector by an amout of @a t. This version does not travel
///  the short way when appropriate.  Always returns a normal vector
///  if the inputs vectors are normalized.
template <typename Type, typename T>
Type
simple_slerp (const Type & source, const Type & destination, const T & t)
{
	const T cosom = dot (source, destination);

	if (cosom <= -1)
		throw std::domain_error ("slerp is not possible: vectors are inverse collinear.");

	if (cosom >= 1) // both normal vectors are equal
		return destination;

	const T omega = std::acos (cosom);
	const T sinom = std::sin  (omega);

	const T scale0 = std::sin ((1 - t) * omega);
	const T scale1 = std::sin (t * omega);

	return (scale0 * source + scale1 * destination) / sinom;
}

///  Returns true if @a n is a power of two otherwise false.
template <class Type>
inline
constexpr
bool
is_power_of_two (Type n)
{
	return ((n - 1) & n) == 0;
}

///  Returns the next power of @a n. If n is a power of two it returns @a n.
///  If n is zero it returns zero.
template <class Type>
Type
next_power_of_two (Type n)
{
	-- n;

	for (Type k = 1; not (k & (1 << (sizeof (n) + 1))); k <<= 1)
		n |= n >> k;

	return ++ n;
}

template <class Type>
Type
min (std::initializer_list <Type> list)
{
	auto       first = list .begin ();
	const auto last  = list .end ();

	if (first == last)
		return *first;

	Type result = *first;

	while (++ first not_eq last)
		result = min (result, *first);

	return result;
}

template <class Type>
Type
max (std::initializer_list <Type> list)
{
	auto       first = list .begin ();
	const auto last  = list .end ();

	if (first == last)
		return *first;

	Type result = *first;

	while (++ first not_eq last)
		result = max (result, *first);

	return result;
}

} // math
} // titania

#endif
