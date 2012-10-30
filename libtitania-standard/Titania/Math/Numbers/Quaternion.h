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

#ifndef __TITANIA_MATH_NUMBERS_QUATERNION_H__
#define __TITANIA_MATH_NUMBERS_QUATERNION_H__

#include <cmath>
#include <istream>
#include <ostream>

#include "Vector3.h"

namespace titania {
namespace math {

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

	///@{
	///  Default constructor.  All values default to 0.
	constexpr
	quaternion () :
		value { Type (), Type (), Type (), Type () } { }

	///  Copy constructor.
	template <class T>
	constexpr
	quaternion (const quaternion <T> & quat) :
		value { quat .x (), quat .y (), quat .z (), quat .w () } { }

	///  Component constructor. Set values to @a x, @a y and @a z.
	constexpr
	quaternion (const Type & x, const Type & y, const Type & z, const Type & w) :
		value { x, y, z, w } { }

	///  Construct quaternion from vector @a imag and @a w.
	template <class T>
	constexpr
	quaternion (const vector3 <T> & imag, const Type & w) :
		value { imag .x (), imag .y (), imag .z (), w } { }
	///@}

	///  @name Assignment operator

	///@{
	///  Assign @a quaternion to this quaternion.
	template <class T>
	quaternion &
	operator = (const quaternion <T> &);

	///@}

	///  @name Element access

	///@{
	///  Set x component of this quaternion.
	void
	x (const Type & v) { value [0] = v; }

	///  Return x component of this quaternion.
	constexpr Type
	x () const { return value [0]; }

	///  Set y component of this quaternion.
	void
	y (const Type & v) { value [1] = v; }

	///  Return y component of this quaternion.
	constexpr Type
	y () const { return value [1]; }

	///  Set z component of this quaternion.
	void
	z (const Type & v) { value [2] = v; }

	///  Return z component of this quaternion.
	constexpr Type
	z () const { return value [2]; }

	///  Set w component of this quaternion which is the real part.
	void
	w (const Type & v) { value [3] = v; }

	///  Return w component of this quaternion which is the real part.
	constexpr Type
	w () const { return value [3]; }

	//@{
	///  Access components by @a index.
	Type &
	operator [ ] (const size_type index) { return value [index]; }

	///  Access components by @a index.
	constexpr Type
	operator [ ] (const size_type index) const { return value [index]; }
	//@}
	///@}

	///  @name Capacity

	///@{
	///  Return number of components.
	static
	constexpr size_type
	size () { return 4; }

	///@}

	///  @name  Arithmetic operations
	///  All these operators modify this quaternion inplace.

	///@{
	///  Invert this quaternion in place.
	quaternion &
	inverse ();

	///  Add @a quaternion to this quaternion.
	template <class T>
	quaternion &
	operator += (const quaternion <T> &);

	///  Subtract @a quaternion to this quaternion.
	template <class T>
	quaternion &
	operator -= (const quaternion <T> &);

	///  Multiply this quaternion by @a t.
	quaternion &
	operator *= (const Type &);

	///  Divide this quaternion by @a t.
	quaternion &
	operator /= (const Type &);

	///  Normalize this quaternion in place.
	quaternion &
	normalize ();

	///@}


private:

	Type value [size ()];

};

template <class Type>
template <class T>
quaternion <Type> &
quaternion <Type>::operator = (const quaternion <T> & quat)
{
	value [0] = quat .x ();
	value [1] = quat .y ();
	value [2] = quat .z ();
	value [3] = quat .w ();
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::inverse ()
{
	value [0] = -value [0];
	value [1] = -value [1];
	value [2] = -value [2];
	return *this;
}

template <class Type>
template <class T>
quaternion <Type> &
quaternion <Type>::operator += (const quaternion <T> & quat)
{
	value [0] += quat .x ();
	value [1] += quat .y ();
	value [2] += quat .z ();
	value [3] += quat .w ();
	return *this;
}

template <class Type>
template <class T>
quaternion <Type> &
quaternion <Type>::operator -= (const quaternion <T> & quat)
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
quaternion <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;
	value [3] /= t;
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::normalize ()
{
	Type length = abs (*this);

	if (length)
		return *this /= length;

	return *this;
}

///  @name Element access
///  @relates quaternion

///@{
//@{
///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
Type
real (const quaternion <Type> & quat)
{
	return quat .w ();
}

///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
constexpr vector3 <Type>
imag (const quaternion <Type> & quat)
{
	return vector3 <Type> (quat .x (), quat .y (), quat .z ());
}
//@}
///@}

///  @name Comparision operations
///  @relates quaternion

///@{
//@{
///  Compares two quaternion numbers.
///  Returns true if @a a is equal to @a b.
template <class Type>
constexpr bool
operator == (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return
	   a .x () == b .x () and
	   a .y () == b .y () and
	   a .z () == b .z () and
	   a .w () == b .w ();
}

///  Compares two quaternion numbers.
///  Returns false if @a a is not equal to @a b.
template <class Type>
constexpr bool
operator not_eq (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return
	   a .x () not_eq b .x () or
	   a .y () not_eq b .y () or
	   a .z () not_eq b .z () or
	   a .w () not_eq b .w ();
}
//@}
///@}

///  @relates quaternion
///  @name Arithmetic operations

///@{
//@{
///  Returns a copy of @a quaternion.
template <class Type>
inline
constexpr Type
operator + (const quaternion <Type> & quat)
{
	return quat;
}

///  Returns quaternion negation of @a quaternion.
template <class Type>
quaternion <Type>
operator - (const quaternion <Type> & quat)
{
	return quaternion <Type> (-quat .x (),
	                          -quat .y (),
	                          -quat .z (),
	                          -quat .w ());
}

///  Returns the inverse quaternion for @a quaternion.
template <class Type>
constexpr quaternion <Type>
operator ~ (const quaternion <Type> & quat)
{
	return quaternion <Type> (quat) .inverse ();
}
//@}

//@{
///  Returns new quaternion value @a a plus @a b.
template <class Type>
inline
quaternion <Type>
operator + (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return quaternion <Type> (a) += b;
}
//@}

//@{
///  Returns new quaternion value @a a minus @a b.
template <class Type>
inline
quaternion <Type>
operator - (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return quaternion <Type> (a) -= b;
}
//@}

//@{
///  Returns new quaternion value @a x times @a y.
template <class Type>
constexpr quaternion <Type>
operator * (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return quaternion <Type> (a .w () * b .x ()
	                          + a .x () * b .w ()
	                          + a .y () * b .z ()
	                          - a .z () * b .y (),

	                          a .w () * b .y ()
	                          + a .y () * b .w ()
	                          + a .z () * b .x ()
	                          - a .x () * b .z (),

	                          a .w () * b .z ()
	                          + a .z () * b .w ()
	                          + a .x () * b .y ()
	                          - a .y () * b .x (),

	                          a .w () * b .w ()
	                          - a .x () * b .x ()
	                          - a .y () * b .y ()
	                          - a .z () * b .z ());
}

///  Returns new quaternion value @a x times @a y.
template <class Type>
inline
constexpr quaternion <Type>
operator * (const quaternion <Type> & a, const Type & b)
{
	return quaternion <Type> (a) *= b;
}

///  Returns new quaternion value @a x times @a y.
template <class Type>
inline
constexpr quaternion <Type>
operator * (const Type & a, const quaternion <Type> & b)
{
	return quaternion <Type> (b) *= a;
}

///  Returns new vector value @a vector multiplied by @a quaternion.
template <class Type>
inline
constexpr vector3 <Type>
operator * (const quaternion <Type> & a, const vector3 <Type> & vector)
{
	return imag (a * quaternion <Type> (vector, Type ()) * ~a);
}
//@}

//template <class Type>
//constexpr quaternion <Type>
//operator / (const quaternion <Type> & a, const quaternion <Type> & b)
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

//@{
///  Returns new quaternion value @a a divided by @a b.
template <class Type>
inline
quaternion <Type>
operator / (const quaternion <Type> & a, const Type & b)
{
	return quaternion <Type> (a) /= b;
}
//@}

//@{
///  Returns new quaternion value @a x dot @a y.
template <class Type>
constexpr Type
dot (const quaternion <Type> & a, const quaternion <Type> & b)
{
	return a .x () * b .x ()
	       + a .y () * b .y ()
	       + a .z () * b .z ()
	       + a .w () * b .w ();
}

///  Returns @a quaternion magnitude.
template <class Type>
inline
constexpr Type
abs (const quaternion <Type> & quat)
{
	return std::sqrt (norm (quat));
}

///  Returns @a quaternion magnitude squared.
template <class Type>
inline
constexpr Type
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
	return quaternion <Type> (quat) .normalize ();
}
//@}

///  Spherical linear interpolate between @a source quaternion and @a destination quaternion by an amout of @a t.
//template <class Type>
//quaternion <Type>
//slerp (const quaternion <Type> & source,
//       const quaternion <Type> & destination,
//       const Type & t)
//{
//	quaternion <Type> _destination;
//
//	Type dotprod = dot (source, destination);
//
//	if (dotprod < 0)
//	{
//		// Reverse signs so we travel the short way round
//		dotprod      = -dotprod;
//		_destination = -destination;
//	}
//	else
//		_destination = destination;
//
//	Type scale0, scale1;
//
//	/* calculate coefficients */
//	if ((1 - dotprod) > 1e-5)
//	{
//		/* standard case (SLERP) */
//		Type omega = std::acos (dotprod);
//		Type sinom = std::sin (omega);
//		scale0 = std::sin ((1 - t) * omega) / sinom;
//		scale1 = std::sin (t * omega) / sinom;
//	}
//	else
//	{
//		/* q1 & q2 are very close, so do linear interpolation */
//		scale0 = 1 - t;
//		scale1 = t;
//	}
//
//	return source * scale0 + _destination * scale1;
//}
///@}

///  @relates quaternion
///  @name Input/Output operations

///@{
//@{
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
//@}
///@}

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
