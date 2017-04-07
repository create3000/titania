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

#ifndef __TITANIA_MATH_NUMBERS_ROTATION4_H__
#define __TITANIA_MATH_NUMBERS_ROTATION4_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "../../Utility/MemberValue.h"
#include "../Functional.h"
#include "Quaternion.h"
#include "Matrix3.h"
#include "Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent an arbitary rotation about an axis.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of axis and angle.
 */
template <class Type>
class rotation4
{
public:

	///  Size typedef.  Used for size and indices.
	using size_type = size_t;

	///  Value typedef.
	using value_type = Type;

	///  Quaternion typedef.
	using quaternion_type = quaternion <Type>;

	///  Vector3 typedef.
	using vector3_type = vector3 <Type>;

	///  Member value typedef.
	using member_value_type = basic::member_value <Type, rotation4 <Type>>;

	///  @name Constructors

	///  Default constructor.
	///  The rotation will be set to its default value 0 0 1  0.
	constexpr
	rotation4 () :
		value ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	rotation4 (const rotation4 <Up> & rotation) :
		value (rotation .quat ())
	{ }

	///  Construct a rotation from normalized @a quaternion.
	template <class Up>
	explicit
	constexpr
	rotation4 (const quaternion <Up> & quaternion) :
		value (quaternion)
	{ }

	///  Components constructor. Construct a rotation from @a x, @a y, @a z and @a angle.
	rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle);

	///  Construct a rotation from @a axis and @a angle.
	template <class Up>
	rotation4 (const vector3 <Up> & axis, const Type & angle);

	///  Construct a rotation from @a fromVector and @a toVector.
	///  The vectors are normalized and the rotation value that would rotate
	///  from the fromVector to the toVector is stored in the object.
	template <class Up>
	rotation4 (const vector3 <Up> & fromVector, const vector3 <Up> & toVector);

	///  Construct a rotation from @a matrix3.
	template <class Up>
	explicit
	rotation4 (const matrix3 <Up> & matrix)
	{ *this = matrix; }

	///  @name Assignment operator

	///  Assign @a rotation to this rotation.
	template <class Up>
	rotation4 &
	operator = (const rotation4 <Up> & other);

	///  Assign @a rotation to this rotation.
	template <class Up>
	rotation4 &
	operator = (const matrix3 <Up> & other);

	///  @name Element access

	///  Set x axis of this rotation.
	void
	x (const Type & value);

	///  Returns x axis of this rotation.
	Type
	x () const
	{ return axis () .x (); }

	///  Set y axis of this rotation.
	void
	y (const Type & value);

	///  Returns y axis of this rotation.
	Type
	y () const
	{ return axis () .y (); }

	///  Set z axis of this rotation.
	void
	z (const Type & value);

	///  Returns z axis of this rotation.
	Type
	z () const
	{ return axis () .z (); }

	///  Set @a axis of this rotation from vector.
	void
	axis (const vector3 <Type> & value)
	{ *this = rotation4 (value, angle ()); }

	///  Returns axis of this rotation.
	vector3_type
	axis () const;

	///  Set angle of this rotation.
	void
	angle (const Type & value);

	///  Returns angle of this rotation.
	Type
	angle () const;

	///  Access x, y, z or angle.
	member_value_type
	operator [ ] (const size_type index);

	///  Access x, y, z or angle.
	Type
	operator [ ] (const size_type index) const;

	operator matrix3 <Type> () const;

	///  Set @a x, @a y, @a z and @a angle componentwise.
	void
	set (const Type & x, const Type & y, const Type & z, const Type & angle);

	///  Get @a x, @a y, @a z and @a angle componentwise.
	template <class T>
	void
	get (T & x, T & y, T & z, T & angle) const;

	///  Set quaternion of this rotation.
	void
	quat (const quaternion <Type> & q)
	{ value = q; }

	///  Returns quaternion of this rotation.
	const quaternion <Type> &
	quat () const
	{ return value; }

	///  @name Capacity

	///  Returns number of components.
	static
	constexpr
	size_type
	size ()
	{ return 4; }

	///  @name Arithmetic operations

	///  Invert this quaternion in place.
	void
	inverse ();

	///  Multiply this rotation by @a rotation.
	rotation4 &
	operator *= (const rotation4 & rotation);

	///  Left multiply this rotation by @a rotation in place.
	void
	mult_left (const rotation4 & rotation);

	///  Right multiply this rotation by @a rotation in place.
	void
	mult_right (const rotation4 & rotation);

	///  Returns the value of @a vector left multiplied by the quaternion corresponding to this rotation.
	vector3 <Type>
	mult_vec_rot (const vector3 <Type> & vector) const;

	///  Returns the value of @a vector right multiplied by the quaternion corresponding to this rotation.
	vector3 <Type>
	mult_rot_vec (const vector3 <Type> & vector) const;


private:

	quaternion <Type> value;

};

template <class Type>
rotation4 <Type>::rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	set (x, y, z, angle);
}

template <class Type>
template <class Up>
inline
rotation4 <Type>::rotation4 (const vector3 <Up> & axis, const Type & angle) :
	rotation4 (axis .x (), axis .y (), axis .z (), angle)
{ }

template <class Type>
template <class Up>
rotation4 <Type>::rotation4 (const vector3 <Up> & fromVector, const vector3 <Up> & toVector) :
	rotation4 ()
{
	// https://bitbucket.org/Coin3D/coin/src/abc9f50968c9/src/base/SbRotation.cpp

	const vector3 <Type> from (normalize (fromVector));
	const vector3 <Type> to (normalize (toVector));

	const Type     cos_angle = clamp <Type> (dot (from, to), -1, 1);
	vector3 <Type> crossvec  = normalize (cross (from, to));
	const Type     crosslen  = abs (crossvec);

	if (crosslen == 0)
	{
		// Parallel vectors
		// Check if they are pointing in the same direction.
		if (cos_angle > 0)
			;              // standard rotation

		// Ok, so they are parallel and pointing in the opposite direction
		// of each other.
		else
		{
			// Try crossing with x axis.
			vector3 <Type> t = cross (from, vector3 <Type> (1, 0, 0));

			// If not ok, cross with y axis.
			if (norm (t) == 0)
				t = cross (from, vector3 <Type> (0, 1, 0));

			t .normalize ();

			value = quaternion <Type> (t [0], t [1], t [2], 0);
		}
	}
	else
	{
		// Vectors are not parallel
		// The abs () wrapping is to avoid problems when `dot' "overflows" a tiny wee bit,
		// which can lead to sqrt () returning NaN.
		crossvec *= std::sqrt (std::abs (1 - cos_angle) / 2);
		value     = quaternion <Type> (crossvec [0], crossvec [1], crossvec [2], std::sqrt ((1 + cos_angle) / 2));
	}
}

template <class Type>
template <class Up>
inline
rotation4 <Type> &
rotation4 <Type>::operator = (const rotation4 <Up> & rotation)
{
	value = rotation .quat ();
	return *this;
}

template <class Type>
template <class Up>
inline
rotation4 <Type> &
rotation4 <Type>::operator = (const matrix3 <Up> & matrix)
{
	Type quat [4];

	int i;

	// First, find largest diagonal in matrix:
	if (matrix [0] [0] > matrix [1] [1])
	{
		i = matrix [0] [0] > matrix [2] [2] ? 0 : 2;
	}
	else
	{
		i = matrix [1] [1] > matrix [2] [2] ? 1 : 2;
	}

	const Type scalerow = matrix [0] [0] + matrix [1] [1] + matrix [2] [2];

	if (scalerow > matrix [i] [i])
	{
		// Compute w first:
		quat [3] = std::sqrt (scalerow + 1) / 2;

		// And compute other values:
		const Type d = 4 * quat [3];
		quat [0] = (matrix [1] [2] - matrix [2] [1]) / d;
		quat [1] = (matrix [2] [0] - matrix [0] [2]) / d;
		quat [2] = (matrix [0] [1] - matrix [1] [0]) / d;
	}
	else
	{
		// Compute x, y, or z first:
		const int j = (i + 1) % 3;
		const int k = (i + 2) % 3;

		// Compute first value:
		quat [i] = std::sqrt (matrix [i] [i] - matrix [j] [j] - matrix [k] [k] + 1) / 2;

		// And the others:
		const Type d = 4 * quat [i];
		quat [j] = (matrix [i] [j] + matrix [j] [i]) / d;
		quat [k] = (matrix [i] [k] + matrix [k] [i]) / d;
		quat [3] = (matrix [j] [k] - matrix [k] [j]) / d;
	}

	value = quaternion <Type> (quat [0], quat [1], quat [2], quat [3]);
	return *this;
}

template <class Type>
void
rotation4 <Type>::x (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (value, y, z, angle);
}

template <class Type>
void
rotation4 <Type>::y (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (x, value, z, angle);
}

template <class Type>
void
rotation4 <Type>::z (const Type & value)
{
	Type x, y, z, angle;

	get (x, y, z, angle);

	*this = rotation4 (x, y, value, angle);
}

template <class Type>
vector3 <Type>
rotation4 <Type>::axis () const
{
	if (std::abs (value .w ()) >= 1)
		return vector3_type (0, 0, 1);

	return normalize (imag (value));
}

template <class Type>
inline
void
rotation4 <Type>::angle (const Type & value)
{
	*this = rotation4 (axis (), value);
}

template <class Type>
Type
rotation4 <Type>::angle () const
{
	if (std::abs (value .w ()) >= 1)
		return 0;

	return 2 * std::acos (value .w ());
}

///  Access components by @a index.
template <class Type>
typename rotation4 <Type>::member_value_type
rotation4 <Type>::operator [ ] (const size_type index)
{
	switch (index)
	{
		case 0: return member_value_type (this, &rotation4::x, &rotation4::x);
		case 1: return member_value_type (this, &rotation4::y, &rotation4::y);
		case 2: return member_value_type (this, &rotation4::z, &rotation4::z);
		case 3: return member_value_type (this, &rotation4::angle, &rotation4::angle);
	}

	throw std::out_of_range ("index out of range");
}

///  Access components by @a index.
template <class Type>
Type
rotation4 <Type>::operator [ ] (const size_type index) const
{
	switch (index)
	{
		case 0: return x ();
		case 1: return y ();
		case 2: return z ();
		case 3: return angle ();
	}

	throw std::out_of_range ("index out of range");
}

///  Convert this rotation to a matrix3.
template <class Type>
rotation4 <Type>::operator matrix3 <Type> () const
{
	const Type x = quat () .x ();
	const Type y = quat () .y ();
	const Type z = quat () .z ();
	const Type w = quat () .w ();

	const Type a = x * x;
	const Type b = x * y;
	const Type c = y * y;
	const Type d = y * z;
	const Type e = z * x;
	const Type f = z * z;
	const Type g = w * x;
	const Type h = w * y;
	const Type i = w * z;

	return matrix3 <Type> (
		1 - 2 * (c + f),
		    2 * (b + i),
		    2 * (e - h),

		    2 * (b - i),
		1 - 2 * (f + a),
		    2 * (d + g),

		    2 * (e + h),
		    2 * (d - g),
		1 - 2 * (c + a)
	);
}


template <class Type>
void
rotation4 <Type>::set (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	Type scale = std::sqrt (x * x + y * y + z * z);

	if (scale == 0)
	{
		value = quaternion <Type> ();
		return;
	}

	// Calculate quaternion

	const Type halfTheta = interval <Type> (angle / 2, 0, pi <Type>);

	scale = std::sin (halfTheta) / scale;

	value = quaternion <Type> (x * scale,
	                           y * scale,
	                           z * scale,
	                           std::cos (halfTheta));
}

template <class Type>
template <class T>
void
rotation4 <Type>::get (T & x, T & y, T & z, T & angle) const
{
	if (std::abs (value .w ()) >= 1)
	{
		x     = 0;
		y     = 0;
		z     = 1;
		angle = 0;
		return;
	}

	const vector3_type vector = normalize (imag (value));

	x     = vector .x ();
	y     = vector .y ();
	z     = vector .z ();
	angle = 2 * std::acos (value .w ());
}

template <class Type>
inline
void
rotation4 <Type>::inverse ()
{
	value .inverse ();
}

template <class Type>
inline
rotation4 <Type> &
rotation4 <Type>::operator *= (const rotation4 & rotation)
{
	value .mult_right (rotation .quat ());
	return *this;
}

template <class Type>
inline
void
rotation4 <Type>::mult_left (const rotation4 & rotation)
{
	value .mult_left (rotation .quat ());
	value .normalize ();
}

template <class Type>
inline
void
rotation4 <Type>::mult_right (const rotation4 & rotation)
{
	value .mult_right (rotation .quat ());
	value .normalize ();
}

template <class Type>
inline
vector3 <Type>
rotation4 <Type>::mult_vec_rot (const vector3 <Type> & vector) const
{
	return value .mult_vec_quat (vector);
}

template <class Type>
inline
vector3 <Type>
rotation4 <Type>::mult_rot_vec (const vector3 <Type> & vector) const
{
	return value .mult_quat_vec (vector);
}

///  @relates rotation4
///  @name Comparision operations

///  Compares two rotation numbers.
///  Returns true if @a a is equal to @a b.
template <class Type>
inline
constexpr quaternion <Type>
quat (const rotation4 <Type> & rotation)
{
	return rotation .quat ();
}

///  Compares two rotation numbers.
///  Returns true if @a a is equal to @a b.
template <class Type>
inline
constexpr bool
operator == (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return lhs .quat () == rhs .quat ();
}

///  Compares two rotation numbers.
///  Returns false if @a a is not equal to @a b.
template <class Type>
inline
constexpr bool
operator not_eq (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return lhs .quat () not_eq rhs .quat ();
}

///  Lexicographically compares two rotation4 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return lhs .quat () < rhs .quat ();;
}

///  Lexicographically compares two rotation4 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two rotation4 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two rotation4 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates rotation4
///  @name Arithmetic operations

///  Returns the inverse rotation for @a rotation.
template <class Type>
inline
rotation4 <Type>
operator ~ (const rotation4 <Type> & rotation)
{
	rotation4 <Type> result (rotation);
	result .inverse ();
	return result;
}

///  Returns the inverse rotation for @a rotation.
template <class Type>
inline
rotation4 <Type>
inverse (const rotation4 <Type> & rotation)
{
	rotation4 <Type> result (rotation);
	result .inverse ();
	return result;
}

///  Left multiply @a lhs by @a rhs.
template <class Type>
inline
rotation4 <Type>
operator * (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs)
{
	rotation4 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Returns the value of @a vector left multiplied by the quaternion corresponding to this object's rotation.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & vector, const rotation4 <Type> & rotation)
{
	return rotation .mult_vec_rot (vector);
}

///  Returns the value of @a vector right multiplied by the quaternion corresponding to this object's rotation.
template <class Type>
inline
vector3 <Type>
operator * (const rotation4 <Type> & rotation, const vector3 <Type> & vector)
{
	return rotation .mult_rot_vec (vector);
}

///  Spherical linear interpolate between @a source quaternion and @a destination quaternion by an amout of @a t.
template <class Type>
inline
rotation4 <Type>
slerp (const rotation4 <Type> & source,
       const rotation4 <Type> & destination,
       const Type & t)
{
	return rotation4 <Type> (slerp (source .quat (), destination .quat (), t));
}

///  Spherical cubic interpolation of @a source, @a ab, @a b and @a destination by an amout of @a t.
template <class Type>
inline
rotation4 <Type>
squad (const rotation4 <Type> & source,
       const rotation4 <Type> & a,
       const rotation4 <Type> & b,
       const rotation4 <Type> & destination,
       const Type & t)
{
	return rotation4 <Type> (squad (source .quat (), a .quat (), b .quat (), destination .quat (), t));
}

///  Shoemake-Bezier interpolation using De Castlejau algorithm
template <class Type, class T>
rotation4 <Type>
bezier (const rotation4 <Type> & source, const rotation4 <Type> & a, const rotation4 <Type> & b, const rotation4 <Type> & destination, T t)
{
	return rotation4 <Type> (bezier (source .quat (), a .quat (), b .quat (), destination .quat (), t));
}

/// Given 3 quaternions, qn-1,qn and qn+1, calculate a control point to be used in squad interpolation.
template <class Type>
inline
rotation4 <Type>
spline (const rotation4 <Type> & q0,
        const rotation4 <Type> & q1,
        const rotation4 <Type> & q2)
{
	return rotation4 <Type> (spline (q0 .quat (), q1 .quat (), q2 .quat ()));
}

///  @relates rotation4
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	istream >> x >> y >> z >> angle;

	if (istream)
		rotation = rotation4 <Type> (x, y, z, angle);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	rotation .get (x, y, z, angle);

	return ostream
	       << x << ' '
	       << y << ' '
	       << z << ' '
	       << angle;
}

extern template class rotation4 <float>;
extern template class rotation4 <double>;
extern template class rotation4 <long double>;

//
extern template std::istream & operator >> (std::istream &, rotation4 <float> &);
extern template std::istream & operator >> (std::istream &, rotation4 <double> &);
extern template std::istream & operator >> (std::istream &, rotation4 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const rotation4 <float> &);
extern template std::ostream & operator << (std::ostream &, const rotation4 <double> &);
extern template std::ostream & operator << (std::ostream &, const rotation4 <long double> &);

} // math
} // titania

#endif
