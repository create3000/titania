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

#ifndef __TITANIA_MATH_NUMBERS_QUATERNION_H__
#define __TITANIA_MATH_NUMBERS_QUATERNION_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

#include "../Functional.h"
#include "Vector3.h"

namespace titania {
namespace math {

template <class Type>
class quaternion;

template <class Type>
quaternion <Type>
log (const quaternion <Type> &);

template <class Type>
quaternion <Type>
exp (const quaternion <Type> &);

/**
 *  Template to represent quaternion numbers.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of real and imaginary values.
 */
template <class Type>
class quaternion
{
public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  Vector3 typedef.
	typedef vector3 <Type> vector3_type;

	///  @name Constructors

	///  Default constructor.  All values default to 0.
	constexpr
	quaternion () :
		value { Type (), Type (), Type (), Type () }
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	quaternion (const quaternion <Up> & quat) :
		value { quat .x (), quat .y (), quat .z (), quat .w () }
	{ }

	///  Component constructor. Set values to @a x, @a y and @a z.
	constexpr
	quaternion (const Type & x, const Type & y, const Type & z, const Type & w) :
		value { x, y, z, w }
	{ }

	///  Construct quaternion from vector @a imag and @a w.
	template <class Up>
	constexpr
	quaternion (const vector3 <Up> & imag, const Type & w) :
		value { imag .x (), imag .y (), imag .z (), w }
	{ }

	///  @name Assignment operator

	///  Assign @a quaternion to this quaternion.
	template <class Up>
	quaternion &
	operator = (const quaternion <Up> &);

	///  @name Element access

	///  Set x component of this quaternion.
	void
	x (const Type & v)
	{ value [0] = v; }

	///  Return x component of this quaternion.
	const Type &
	x () const
	{ return value [0]; }

	///  Set y component of this quaternion.
	void
	y (const Type & v)
	{ value [1] = v; }

	///  Return y component of this quaternion.
	const Type &
	y () const
	{ return value [1]; }

	///  Set z component of this quaternion.
	void
	z (const Type & v)
	{ value [2] = v; }

	///  Return z component of this quaternion.
	const Type &
	z () const
	{ return value [2]; }

	///  Set w component of this quaternion which is the real part.
	void
	w (const Type & v)
	{ value [3] = v; }

	///  Return w component of this quaternion which is the real part.
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

	///  @name Capacity

	///  Return number of components.
	static
	constexpr
	size_type
	size ()
	{ return 4; }

	///  @name  Arithmetic operations
	///  All these operators modify this quaternion inplace.

	///  Negates this quaternion in place.
	void
	negate ();

	///  Invert this quaternion in place.
	void
	inverse ();

	///  Add @a quaternion to this quaternion.
	quaternion &
	operator += (const quaternion &);

	///  Subtract @a quaternion to this quaternion.
	quaternion &
	operator -= (const quaternion &);

	///  Multiply this quaternion by @a t.
	quaternion &
	operator *= (const Type &);

	///  Left multiply this quaternion by @a quaternion.
	quaternion &
	operator *= (const quaternion &);

	///  Divide this quaternion by @a t.
	quaternion &
	operator /= (const Type &);

	///  Left multiply this quaternion by @a quaternion in place.
	void
	mult_left (const quaternion &);

	///  Right multiply this quaternion by @a quaternion in place.
	void
	mult_right (const quaternion &);

	///  Returns the value of @a vector left multiplied by this quaternion.
	vector3 <Type>
	mult_vec_quat (const vector3 <Type> &) const;

	///  Returns the value of @a vector right multiplied by this quaternion.
	vector3 <Type>
	mult_quat_vec (const vector3 <Type> &) const;

	///  Normalize this quaternion in place.
	void
	normalize ();


private:

	Type value [size ()];

};

template <class Type>
template <class Up>
quaternion <Type> &
quaternion <Type>::operator = (const quaternion <Up> & quat)
{
	value [0] = quat .x ();
	value [1] = quat .y ();
	value [2] = quat .z ();
	value [3] = quat .w ();
	return *this;
}

template <class Type>
void
quaternion <Type>::negate ()
{
	value [0] = -value [0];
	value [1] = -value [1];
	value [2] = -value [2];
	value [3] = -value [3];
}

template <class Type>
void
quaternion <Type>::inverse ()
{
	value [0] = -value [0];
	value [1] = -value [1];
	value [2] = -value [2];
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator += (const quaternion & quat)
{
	value [0] += quat .x ();
	value [1] += quat .y ();
	value [2] += quat .z ();
	value [3] += quat .w ();
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator -= (const quaternion & quat)
{
	value [0] -= quat .x ();
	value [1] -= quat .y ();
	value [2] -= quat .z ();
	value [3] -= quat .w ();
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;
	value [2] *= t;
	value [3] *= t;
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator *= (const quaternion & quat)
{
	mult_right (quat);
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;
	value [3] /= t;
	return *this;
}

template <class Type>
void
quaternion <Type>::mult_left (const quaternion & quat)
{
	*this = quaternion (w () * quat .x () +
	                    x () * quat .w () +
	                    y () * quat .z () -
	                    z () * quat .y (),

	                    w () * quat .y () +
	                    y () * quat .w () +
	                    z () * quat .x () -
	                    x () * quat .z (),

	                    w () * quat .z () +
	                    z () * quat .w () +
	                    x () * quat .y () -
	                    y () * quat .x (),

	                    w () * quat .w () -
	                    x () * quat .x () -
	                    y () * quat .y () -
	                    z () * quat .z ());
}

template <class Type>
void
quaternion <Type>::mult_right (const quaternion & quat)
{
	*this = quaternion (quat .w () * x () +
	                    quat .x () * w () +
	                    quat .y () * z () -
	                    quat .z () * y (),

	                    quat .w () * y () +
	                    quat .y () * w () +
	                    quat .z () * x () -
	                    quat .x () * z (),

	                    quat .w () * z () +
	                    quat .z () * w () +
	                    quat .x () * y () -
	                    quat .y () * x (),

	                    quat .w () * w () -
	                    quat .x () * x () -
	                    quat .y () * y () -
	                    quat .z () * z ());
}

///  Returns new vector value @a vector left multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
quaternion <Type>::mult_vec_quat (const vector3 <Type> & vector) const
{
	//return imag (~*this * quaternion <Type> (vector, Type ()) * *this);

	const Type a = w () * w () - x () * x () - y () * y () - z () * z ();                     
	const Type b = 2 * (vector .x () * x () + vector .y () * y () + vector .z () * z ());  
	const Type c = 2 * w ();                                       

	return vector3 <Type> (a * vector .x () + b * x () + c * (y () * vector .z () - z () * vector .y ()),
	                       a * vector .y () + b * y () + c * (z () * vector .x () - x () * vector .z ()),
	                       a * vector .z () + b * z () + c * (x () * vector .y () - y () * vector .x ()));
}

///  Returns new vector value @a vector right multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
quaternion <Type>::mult_quat_vec (const vector3 <Type> & vector) const
{
	//return imag (*this * quaternion <Type> (vector, Type ()) * ~*this);

	const Type a = w () * w () - x () * x () - y () * y () - z () * z ();                     
	const Type b = 2 * (vector .x () * x () + vector .y () * y () + vector .z () * z ());  
	const Type c = 2 * w ();                                       

	return vector3 <Type> (a * vector .x () + b * x () - c * (y () * vector .z () - z () * vector .y ()),
	                       a * vector .y () + b * y () - c * (z () * vector .x () - x () * vector .z ()),
	                       a * vector .z () + b * z () - c * (x () * vector .y () - y () * vector .x ()));
}

template <class Type>
void
quaternion <Type>::normalize ()
{
	const Type length = abs (*this);

	if (length)
		*this /= length;
}

///  @name Element access
///  @relates quaternion

///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
const Type &
real (const quaternion <Type> & quat)
{
	return quat .w ();
}

///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
constexpr
vector3 <Type>
imag (const quaternion <Type> & quat)
{
	return vector3 <Type> (quat .x (), quat .y (), quat .z ());
}

///  @name Comparision operations
///  @relates quaternion

///  Compares two quaternion numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator == (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () == rhs .x () and
	       lhs .y () == rhs .y () and
	       lhs .z () == rhs .z () and
	       lhs .w () == rhs .w ();
}

///  Compares two quaternion numbers.
///  Returns false if @a lhs is not equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator not_eq (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () not_eq rhs .x () or
	       lhs .y () not_eq rhs .y () or
	       lhs .z () not_eq rhs .z () or
	       lhs .w () not_eq rhs .w ();
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return std::tie (lhs .x (), lhs .y (), lhs .z (), lhs .w ()) < std::tie (rhs .x (), rhs .y (), rhs .z (), rhs .w ());
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates quaternion
///  @name Arithmetic operations

///  Returns a copy of @a quaternion.
template <class Type>
inline
constexpr
Type
operator + (const quaternion <Type> & quat)
{
	return quat;
}

///  Returns quaternion negation of @a quaternion.
template <class Type>
inline
quaternion <Type>
operator - (const quaternion <Type> & quat)
{
	quaternion <Type> result (quat);
	result .negate ();
	return result;
}

///  Returns the inverse quaternion for @a quaternion.
template <class Type>
inline
quaternion <Type>
operator ~ (const quaternion <Type> & quat)
{
	quaternion <Type> result (quat);
	result .inverse ();
	return result;
}

///  Returns new quaternion value @a lhs plus @a rhs.
template <class Type>
inline
quaternion <Type>
operator + (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs) += rhs;
}

///  Returns new quaternion value @a lhs minus @a rhs.
template <class Type>
inline
quaternion <Type>
operator - (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs) -= rhs;
}

///  Returns new quaternion value @a lhs left multiplied by @a rhs.
template <class Type>
inline
quaternion <Type>
operator * (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	quaternion <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Returns new quaternion value @a lhs right multiplied @a rhs.
template <class Type>
inline
constexpr
quaternion <Type>
operator * (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) *= rhs;
}

///  Returns new quaternion value @a lhs times @a rhs.
template <class Type>
inline
constexpr
quaternion <Type>
operator * (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (rhs) *= lhs;
}

///  Returns the value of @a vector left multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & vector, const quaternion <Type> & quat)
{
	return quat .mult_vec_quat (vector);
}

///  Returns the value of @a vector right multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
operator * (const quaternion <Type> & quat, const vector3 <Type> & vector)
{
	return quat .mult_quat_vec (vector);
}

//template <class Type>
//constexpr quaternion <Type>
//operator / (const quaternion <Type> & rhs, const quaternion <Type> & lhs)
//{
//	return quaternion <Type> (-a .w () * b .x ()
//	                          + a .x () * b .w ()
//	                          - a .y () * b .z ()
//	                          + a .z () * b .y (),
//
//	                          -a .w () * b .y ()
//	                          + a .y () * b .w ()
//	                          - a .z () * b .x ()
//	                          + a .x () * b .z (),
//
//	                          -a .w () * b .z ()
//	                          + a .z () * b .w ()
//	                          - a .x () * b .y ()
//	                          + a .y () * b .x (),
//
//	                          a .w () * b .w ()
//	                          + a .x () * b .x ()
//	                          + a .y () * b .y ()
//	                          + a .z () * b .z ());
//}

///  Returns new quaternion value @a lhs divided by @a rhs.
template <class Type>
inline
quaternion <Type>
operator / (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) /= rhs;
}

///  Returns new quaternion value @a lhs divided by @a rhs.
template <class Type>
quaternion <Type>
operator / (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs / rhs .x (),
	                          lhs / rhs .y (),
	                          lhs / rhs .z (),
	                          lhs / rhs .w ());
}

///  Returns new quaternion value @a lhs dot @a rhs.
template <class Type>
constexpr
Type
dot (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () * rhs .x () +
	       lhs .y () * rhs .y () +
	       lhs .z () * rhs .z () +
	       lhs .w () * rhs .w ();
}

///  Returns whether @a quaternion is pure real.
template <class Type>
inline
constexpr
Type
is_real (const quaternion <Type> & quat)
{
	return not (quat .x () or quat .y () or quat .z ());
}

///  Returns whether @a quaternion is pure imaginary.
template <class Type>
inline
constexpr
Type
is_imag (const quaternion <Type> & quat)
{
	return not (quat .w ());
}

///  Returns @a quaternion magnitude.
template <class Type>
inline
constexpr
Type
abs (const quaternion <Type> & quat)
{
	return std::sqrt (norm (quat));
}

///  Returns @a quaternion magnitude squared.
template <class Type>
inline
constexpr
Type
norm (const quaternion <Type> & quat)
{
	return dot (quat, quat);
}

///  Returns @a quaternion normalized.
template <class Type>
inline
quaternion <Type>
normalize (const quaternion <Type> & quat)
{
	quaternion <Type> result (quat);
	result .normalize ();
	return result;
}

///  Raise @a quaternion to @a quaternion power.
template <class Type>
inline
quaternion <Type>
pow (const quaternion <Type> & base, const quaternion <Type> & exponent)
{
	// For quaternion^quaternion, we use exp and log.
	// b^x = e^{x*ln b}
	return exp (exponent * log (base));
}

///  Raise @a quaternion to @a scalar power.
///  http://cpansearch.perl.org/src/JCHIN/Math-Quaternion-0.07/lib/Math/Quaternion.pm
///  XXX: not testet yet
template <class Type>
quaternion <Type>
pow (const quaternion <Type> & base, const Type & exponent)
{
	if (is_real (base))
		return quaternion <Type> (0, 0, 0, std::pow (base .w (), exponent));

	const Type l     = abs (base);
	const Type theta = std::acos (base .w () / l);
	const Type li    = abs (imag (base));
	const Type ltoe  = std::pow (l, exponent);
	const Type et    = exponent * theta;
	const Type scale = ltoe / li * std::sin (et);

	return quaternion <Type> (base .x () * scale,
	                          base .y () * scale,
	                          base .z () * scale,
	                          ltoe * std::cos (et));
}

///  Returns the logarithm of its argument.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
quaternion <Type>
log (const quaternion <Type> & quat)
{
	if (is_real (quat))
	{
		if (quat .w () > 0)
			return quaternion <Type> (0, 0, 0, std::log (quat .w ()));

		else
			return quaternion <Type> (PI <Type>, 0, 0, std::log (-quat .w ()));
	}

	const Type l = abs (quat);
	const auto v = normalize (imag (quat)) * std::acos (quat .w () / l);
	const Type w = std::log (l);

	return quaternion <Type> (v .x (), v .y (), v .z (), w);
}

///  Exponential operator e^q.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
quaternion <Type>
exp (const quaternion <Type> & quat)
{
	if (is_real (quat))
		return quaternion <Type> (0, 0, 0, std::exp (quat .w ()));

	const auto i  = imag (quat);
	const Type li = abs (i);
	const Type ew = std::exp (quat .w ());
	const Type w  = ew * std::cos (li);
	const auto v  = i * (ew * std::sin (li) / li);

	return quaternion <Type> (v .x (), v .y (), v .z (), w);
}

///  Spherical cubic interpolation of @a source, @a a, @a b and @a destination by an amout of @a t.
template <class Type>
inline
quaternion <Type>
squad (const quaternion <Type> & source,
       const quaternion <Type> & a,
       const quaternion <Type> & b,
       const quaternion <Type> & destination,
       const Type & t)
{
	// We must use shortest path slerp to prevent flipping.  Also see spline.

	return slerp (slerp (source, destination, t), slerp (a, b, t), 2 * t * (1 - t));
}

///  Shoemake-Bezier interpolation using De Castlejau algorithm.
template <class Type>
quaternion <Type>
bezier (const quaternion <Type> & q0,
        const quaternion <Type> & a,
        const quaternion <Type> & b,
        const quaternion <Type> & q1,
        Type t)
{
	const quaternion <Type> q11 = slerp (q0, a, t);
	const quaternion <Type> q12 = slerp (a, b, t);
	const quaternion <Type> q13 = slerp (b, q1, t);

	return slerp (slerp (q11, q12, t), slerp (q12, q13, t), t);
}

/// Given 3 quaternions, qn-1, qn and qn+1, calculate a control point for qn to be used in squat interpolation.
template <class Type>
inline
quaternion <Type>
spline (quaternion <Type> q0,
        const quaternion <Type> & q1,
        quaternion <Type> q2)
{
	// If the dot product is smaller than 0 we must negate the quaternion to prevent flipping. If we negate all
	// the terms we get a different quaternion but it represents the same rotation.

	if (dot (q0, q1) < 0)
		q0 .negate ();

	if (dot (q2, q1) < 0)
		q2 .negate ();

	const quaternion <Type> q1_i = ~q1;

	// The result must be normalized as it will be used in slerp and we can only slerp normalized vectors.

	return normalize (q1 * exp ((log (q1_i * q0) + log (q1_i * q2)) / Type (-4)));
}

///  @relates quaternion
///  @name Input/Output operations

///  Extraction operator for quaternion values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, quaternion <Type> & quat)
{
	Type x, y, z, w;

	istream >> x >> y >> z >> w;

	if (istream)
		quat = quaternion <Type> (x, y, z, w);

	return istream;
}

///  Insertion operator for quaternion values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const quaternion <Type> & quat)
{
	return ostream
	       << quat .x () << ' '
	       << quat .y () << ' '
	       << quat .z () << ' '
	       << quat .w ();
}

extern template class quaternion <float>;
extern template class quaternion <double>;
extern template class quaternion <long double>;

//
extern template std::istream & operator >> (std::istream &, quaternion <float> &);
extern template std::istream & operator >> (std::istream &, quaternion <double> &);
extern template std::istream & operator >> (std::istream &, quaternion <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const quaternion <float> &);
extern template std::ostream & operator << (std::ostream &, const quaternion <double> &);
extern template std::ostream & operator << (std::ostream &, const quaternion <long double> &);

} // math
} // titania

#endif
