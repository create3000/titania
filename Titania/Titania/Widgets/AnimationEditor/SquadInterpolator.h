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

#ifndef __TEST_TITANIA_MATH_ALGORITHMS_SQUAD_INTERPOLATOR_H__
#define __TEST_TITANIA_MATH_ALGORITHMS_SQUAD_INTERPOLATOR_H__

#include <cmath>
#include <vector>

#include <Titania/Math/Numbers/Rotation4.h>

namespace titania {
namespace test {

///  Returns the logarithm of its argument.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
math::quaternion <Type>
log (const math::quaternion <Type> & quat)
{
	if (is_real (quat))
	{
		if (quat .w () > 0)
			return math::quaternion <Type> (0, 0, 0, std::log (quat .w ()));

		else
			return math::quaternion <Type> (M_PI, 0, 0, std::log (-quat .w ()));
	}

	const Type l = abs (quat);
	const auto v = normalize (imag (quat)) * std::acos (quat .w () / l);
	const Type w = std::log (l);

	return math::quaternion <Type> (v .x (), v .y (), v .z (), w);
}

///  Exponential operator e^q.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
math::quaternion <Type>
exp (const math::quaternion <Type> & quat)
{
	if (is_real (quat))
		return math::quaternion <Type> (0, 0, 0, std::exp (quat .w ()));

	const auto i  = imag (quat);
	const Type li = abs (i);
	const Type ew = std::exp (quat .w ());
	const Type w  = ew * std::cos (li);
	const auto v  = i * (ew * std::sin (li) / li);

	return math::quaternion <Type> (v .x (), v .y (), v .z (), w);
}

//template <class Type>
//math::quaternion <Type>
//log (const math::quaternion <Type> & quat)
//{
//	const Type theta     = std::acos (quat .w ());
//	const Type sin_theta = std::sin (theta);
//
//	if (sin_theta)
//	{
//		const auto v = imag (quat) * (theta / sin_theta);
//
//		return math::quaternion <Type> (v .x (), v .y (), v .z (), 0);
//	}
//
//	return math::quaternion <Type> (quat .x (), quat .y (), quat .z (), 0);
//}
//
/////  Returns the logarithm of its argument.
/////  http://de.wikipedia.org/wiki/Quaternion
//template <class Type>
//math::quaternion <Type>
//exp (const math::quaternion <Type> & quat)
//{
//	const Type theta     = abs (imag (quat));
//	const Type sin_theta = std::sin (theta);
//	const Type cos_theta = std::cos (theta);
//
//	if (theta)
//	{
//		const auto v = imag (quat) * (sin_theta / theta);
//
//		return math::quaternion <Type> (v .x (), v .y (), v .z (), cos_theta);
//	}
//
//	return math::quaternion <Type> (quat .x (), quat .y (), quat .z (), cos_theta);
//}

/// Given 3 quaternions, qn-1, qn and qn+1, calculate a control point for qn to be used in squat interpolation.
template <class Type>
inline
math::quaternion <Type>
spline (math::quaternion <Type> q0,
        const math::quaternion <Type> & q1,
        math::quaternion <Type> q2)
{
	// If the dot product is smaller than 0 we must negate the quaternion to prevent flipping. If we negate all
	// the terms we get a different quaternion but it represents the same rotation.

	if (dot (q0, q1) < 0)
		q0 .negate ();

	if (dot (q2, q1) < 0)
		q2 .negate ();

	const math::quaternion <Type> q1_i = ~q1;

	// The result must be normalized as it will be used in slerp and we can only slerp normalized vectors.

	return normalize (q1 * test::exp ((test::log (q1_i * q0) + test::log (q1_i * q2)) / Type (-4)));
}

/// Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in squad interpolation.
template <class Type>
inline
math::rotation4 <Type>
spline (const math::rotation4 <Type> & q0,
        const math::rotation4 <Type> & q1,
        const math::rotation4 <Type> & q2)
{
	return math::rotation4 <Type> (test::spline (q0 .quat (), q1 .quat (), q2 .quat ()));
}


///  Spherical cubic interpolation of @a source, @a a, @a b and @a destination by an amout of @a t.
template <class Type>
inline
math::quaternion <Type>
squad (const math::quaternion <Type> & source,
       const math::quaternion <Type> & a,
       const math::quaternion <Type> & b,
       const math::quaternion <Type> & destination,
       const Type & t)
{
	// We must use shortest path slerp to prevent flipping.  Also see spline.

	return math::slerp (math::slerp (source, destination, t), math::slerp (a, b, t), 2 * t * (1 - t));
}

///  Spherical cubic interpolation of @a source, @a ab, @a b and @a destination by an amout of @a t.
template <class Type>
inline
math::rotation4 <Type>
squad (const math::rotation4 <Type> & source,
       const math::rotation4 <Type> & a,
       const math::rotation4 <Type> & b,
       const math::rotation4 <Type> & destination,
       const Type & t)
{
	return math::rotation4 <Type> (test::squad (source .quat (), a .quat (), b .quat (), destination .quat (), t));
}

template <class Type, class Scalar>
class squad_interpolator
{
public:

	squad_interpolator () :
		s ()
	{ }

	template <class Key, class KeyValue>
	squad_interpolator (const bool closed,
                       const Key & key,
                       const KeyValue & keyValue) :
      squad_interpolator ()
	{ generate (closed, key, keyValue); }

	template <class Key, class KeyValue>
	void
	generate (const bool, const Key &, const KeyValue &);

	template <class KeyValue>
	Type
	interpolate (const size_t, const size_t, const Scalar &, const KeyValue &) const;


private:

	std::vector <Type> s;

};

template <class Type, class Scalar>
template <class Key, class KeyValue>
void
squad_interpolator <Type, Scalar>::generate (const bool closed,
                                             const Key & key,
                                             const KeyValue & keyValue)
{
	s .clear ();
	s. reserve (key .size ());

	if (key .size () > 1)
	{
		if (closed)
		{
			s .emplace_back (test::spline <Scalar> (keyValue [key .size () - 2],
			                                  keyValue [0],
			                                  keyValue [1]));
		}
		else
		{
			s .emplace_back (keyValue .front ());
		}

		for (size_t i = 1, size = key .size () - 1; i < size; ++ i)
		{
			s .emplace_back (test::spline <Scalar> (keyValue [i - 1],
			                                  keyValue [i],
			                                  keyValue [i + 1]));
		}

		if (closed)
		{
			s .emplace_back (test::spline <Scalar> (keyValue [key .size () - 2],
			                                  keyValue [key .size () - 1],
			                                  keyValue [1]));
		}
		else
		{
			s .emplace_back (keyValue [key .size () - 1]);
		}
	}
	else if (key .size () > 0)
		s .emplace_back (keyValue .front ());
}

template <class Type, class Scalar>
template <class KeyValue>
Type
squad_interpolator <Type, Scalar>::interpolate (const size_t index0,
                                                const size_t index1,
                                                const Scalar & weight,
                                                const KeyValue & keyValue) const
{
	return test::squad <Scalar> (keyValue [index0], s [index0], s [index1], keyValue [index1], weight);
}

} // math
} // titania

#endif
