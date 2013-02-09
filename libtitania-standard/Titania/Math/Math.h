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

#ifndef __TITANIA_MATH_MATH_H__
#define __TITANIA_MATH_MATH_H__

#include <cmath>
#include <stdexcept>

namespace titania {
namespace math {

#define M_PI1_2  (M_PI / 2)
#define M_PI2    (2 * M_PI)

#define M_PHI 1.6180339887498948482045868343656381177203091798057628

///  Convert @a value from degrees to radiants.
template <class Type>
inline
constexpr Type
radiant (const Type & value)
{
	return value * (M_PI / 180);
}

///  Convert @a value from radiants to degrees.
template <class Type>
inline
constexpr Type
degree (const Type & value)
{
	return value * (180 / M_PI);
}

///  Clamp @a value in the range @a low and @a high.
template <class Type>
inline
constexpr Type
clamp (const Type & value, const Type & low, const Type & high)
{
	return value > low ? (value < high ? value : high) : low;
}

///  Wrap value in the interval (low;high) so low <= result < high.
template <class Type>
static
Type
interval (const Type & value, const Type & low, const Type & high)
{
	Type range = high - low;

	if (value >= high)
		return std::fmod ((value - low), range) + low;

	if (value < low)
		return std::fmod ((value - high), range) + high;

	return value;
}

///  Map @a value in the interval (@a fromLow;@a fromHigh) to the interval (@a toLow;@a toHigh).
template <class Type>
inline
constexpr Type
project (const Type & value, const Type & fromLow, const Type & fromHigh, const Type & toLow, const Type & toHigh)
{
	return toLow + ((value - fromLow) / (fromHigh - fromLow)) * (toHigh - toLow);
}

///  Linear interpolate between @a source and @a destination by an amout of @a t.
template <class Type, class T>
constexpr Type
lerp (const Type & source, const Type & destination, const T & t)
{
	return source + t * (destination - source);
}

///  Spherical linear interpolate between two normal vectors @a source vector
///  and @a destination vector by an amout of @a t.
template <typename Type, typename T>
Type
slerp (const Type & source, const Type & destination, const T & t)
{
	Type dest  = destination;
	T    cosom = dot (source, destination);

	if (cosom == -1)
		throw std::domain_error ("slerp is not possible: vectors are inverse collinear.");

	if (cosom == 1) // both normal vectors are equal
		return source;

	if (cosom < 0)
	{
		// Reverse signs so we travel the short way round
		cosom = -cosom;
		dest  = -dest;
	}

	T omega = std::acos (cosom);
	T sinom = std::sin  (omega);

	T scale0 = std::sin ((1 - t) * omega);
	T scale1 = std::sin (t * omega);

	return (scale0 * source + scale1 * dest) / sinom;
}

///  Returns true if @a i is a power of two otherwise false.
template <class Type>
inline
constexpr bool
is_power_of_two (Type i)
{
	return ((i - 1) & i) == 0;
}

///  Returns the next power of @a i.
template <class Type>
Type
next_power_of_two (Type i)
{
	-- i;
	i |= i >> 16;
	i |= i >> 8;
	i |= i >> 4;
	i |= i >> 2;
	i |= i >> 1;
	++ i;
	return i;
}

} // math
} // titania

#endif
