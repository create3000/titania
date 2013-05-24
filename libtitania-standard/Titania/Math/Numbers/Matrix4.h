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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX4_H__
#define __TITANIA_MATH_NUMBERS_MATRIX4_H__

#include <cfloat>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "Matrix3.h"
#include "Rotation4.h"
#include "Vector3.h"
#include "Vector4.h"

#include "../Algorithms/EigenDecomposition.h"

namespace titania {
namespace math {

template <class Type>
class matrix4;

template <class Type>
inline
matrix4 <Type>
operator ! (const matrix4 <Type> & matrix);

template <class Type>
inline
matrix4 <Type>
transpose (const matrix4 <Type> & matrix);

template <class Type>
inline
matrix4 <Type>
operator ~ (const matrix4 <Type> & matrix);

template <class Type>
inline
matrix4 <Type>
inverse (const matrix4 <Type> & matrix);

template <class Type>
inline
matrix4 <Type>
operator * (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs);

template <class Type>
class matrix4
{
private:

	static constexpr size_t Order = 4;
	static constexpr size_t Size  = Order * Order;


public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  Vector2 typedef.
	typedef vector3 <Type> vector3_type;

	///  Vector3 typedef.
	typedef rotation4 <Type> rotation4_type;

	///  Vector3 typedef.
	typedef vector4 <Type> vector4_type;

	///  Vector typedef.
	///  This is the type for the vector representation of this matrix.
	///  The matrix consists of four vectors of type vector4.
	///  0 [       0 ]
	///  1 [       0 ]
	///  2 [       0 ]
	///  3 [ x y z 1 ]
	typedef vector4 <vector4_type> vector_type;

	///  Array typedef.
	typedef Type array_type [Size];

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	constexpr
	matrix4 () :
		value (Identity .value)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix4 (const matrix4 <Up> & matrix) :
		value (matrix [0], matrix [1], matrix [2], matrix [3])
	{ }

	///  Value constructor.
	explicit
	constexpr
	matrix4 (const array_type & matrix) :
		array
	{
		matrix [ 0], matrix [ 1], matrix [ 2], matrix [ 3],
		matrix [ 4], matrix [ 5], matrix [ 6], matrix [ 7],
		matrix [ 8], matrix [ 9], matrix [10], matrix [11],
		matrix [12], matrix [13], matrix [14], matrix [15]
	} { }

	///  Components constructor. Set values from @a e11 to @a e44.
	constexpr
	matrix4 (const Type & e11, const Type & e12, const Type & e13, const Type & e14,
	         const Type & e21, const Type & e22, const Type & e23, const Type & e24,
	         const Type & e31, const Type & e32, const Type & e33, const Type & e34,
	         const Type & e41, const Type & e42, const Type & e43, const Type & e44) :
		array
	{
		e11, e12, e13, e14,
		e21, e22, e23, e24,
		e31, e32, e33, e34,
		e41, e42, e43, e44
	} { }

	///  Constructs a matrix4 from a rotation4.
	matrix4 (const rotation4 <Type> & rot)
	{ rotation (rot); }

	///  Constructs a matrix4 from a matrix3.
	matrix4 (const matrix3 <Type> & matrix)
	{ *this = matrix; }

	///  @name Assignment operators

	template <class Up>
	matrix4 &
	operator = (const matrix3 <Up> &);

	template <class Up>
	matrix4 &
	operator = (const matrix4 <Up> &);

	///  @name Element access

	vector3_type
	translation () const
	{
		return vector3_type (array [12], array [13], array [14]);
	}

	void
	rotation (const rotation4 <Type> &);

	rotation4 <Type>
	rotation () const;

	void
	set ();

	void
	set (const vector3 <Type> &);

	void
	set (const vector3 <Type> &,
	     const rotation4 <Type> &);

	void
	set (const vector3 <Type> &,
	     const rotation4 <Type> &,
	     const vector3 <Type> &);

	void
	set (const vector3 <Type> &,
	     const rotation4 <Type> &,
	     const vector3 <Type> &,
	     const rotation4 <Type> &);

	void
	set (const vector3 <Type> &,
	     const rotation4 <Type> &,
	     const vector3 <Type> &,
	     const rotation4 <Type> &,
	     const vector3 <Type> &);

	template <class T>
	void
	get (vector3 <T> &) const;

	template <class T, class R>
	void
	get (vector3 <T> &, rotation4 <R> &) const;

	template <class T, class R, class S>
	void
	get (vector3 <T> &, rotation4 <R> &, vector3 <S> &) const;

	template <class T, class R, class S, class SO>
	void
	get (vector3 <T> &, rotation4 <R> &, vector3 <S> &, rotation4 <SO> &) const;

	template <class T, class R, class S, class SO, class C>
	void
	get (vector3 <T> &, rotation4 <R> &, vector3 <S> &, rotation4 <SO> &, const vector3 <C> &) const;

	///  Access rows by @a index.
	vector4_type &
	operator [ ] (const size_type index) { return value [index]; }

	constexpr vector4_type
	operator [ ] (const size_type index) const { return value [index]; }

	///  Returns pointer to the underlying array serving as element storage.
	///  Specifically the pointer is such that range [data (); data () + size ()) is valid.
	Type*
	data () { return array; }

	const Type*
	data () const { return array; }

	///  Get access to the underlying vector representation of this matrix.
	void
	vector (const vector_type & vector) { value = vector; }

	constexpr vector_type
	vector () const { return value; }

	///  @name Capacity

	///  Returns the order of the matrix.
	static
	constexpr size_type
	order () { return Order; }

	///  Returns the number of elements in the matrix. The size is the same as order () * order ().
	static
	constexpr size_type
	size () { return Size; }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix4 inplace.

	///  Returns the deteminant of this matrix.
	Type
	determinant () const;

	///  Returns this matrix transposed.
	matrix4 &
	transpose ();

	///  Returns this matrix inversed.
	matrix4 &
	inverse ();

	///  Returns this matrix multiplies by @a scalar.
	matrix4 &
	operator *= (const Type &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	matrix4 &
	operator *= (const matrix4 <T> &);

	///  Returns this matrix divided by @a scalar.
	matrix4 &
	operator /= (const Type &);

	///  Returns this matrix left multiplied by @a matrix.
	template <class T>
	matrix4 &
	multLeft (const matrix4 <T> &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	matrix4 &
	multRight (const matrix4 <T> &);

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	vector3 <Type>
	multVecMatrix (const vector3 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	vector3 <Type>
	multMatrixVec (const vector3 <T> &) const;

	///  Returns a new vector that is @vector (a normal or direction vector) multiplies by matrix.
	template <class T>
	vector3 <Type>
	multDirMatrix (const vector3 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector (a normal or direction vector).
	template <class T>
	vector3 <Type>
	multMatrixDir (const vector3 <T> &) const;

	///  Returns this matrix translated by @a translation.
	matrix4 &
	translate (const vector3 <Type> &);

	///  Returns this matrix rotated by @a rotation.
	matrix4 &
	rotate (const rotation4 <Type> &);

	///  Returns this matrix scaled by @a scale.
	matrix4 &
	scale (const vector3 <Type> &);


private:

	bool
	factor (vector3 <Type> & translation,
	        matrix4 & rotation,
	        vector3 <Type> & scale,
	        matrix4 & scaleOrientation) const;

	Type
	det3 () const;

	Type
	det3 (int r1, int r2, int r3, int c1, int c2, int c3) const;

	union
	{
		vector_type value;
		array_type array;
	};

	static const matrix4 Identity;

};

template <class Type>
const matrix4 <Type> matrix4 <Type>::Identity = { 1, 0, 0, 0,
	                                               0, 1, 0, 0,
	                                               0, 0, 1, 0,
	                                               0, 0, 0, 1 };

template <class Type>
template <class Up>
matrix4 <Type> &
matrix4 <Type>::operator = (const matrix3 <Up> & matrix)
{
	array [ 0] = matrix [0] [0];
	array [ 1] = matrix [0] [1];
	array [ 2] = 0;
	array [ 3] = matrix [0] [2];

	array [ 4] = matrix [1] [0];
	array [ 5] = matrix [1] [1];
	array [ 6] = 0;
	array [ 7] = matrix [1] [2];

	array [ 8] = 0;
	array [ 9] = 0;
	array [10] = 1;
	array [11] = 0;

	array [12] = matrix [2] [0];
	array [13] = matrix [2] [1];
	array [14] = 0;
	array [15] = matrix [2] [2];

	return *this;
}

template <class Type>
template <class Up>
inline
matrix4 <Type> &
matrix4 <Type>::operator = (const matrix4 <Up> & matrix)
{
	value = matrix .vector ();
	return *this;
}

template <class Type>
void
matrix4 <Type>::rotation (const rotation4 <Type> & r)
{
	Type x = r .quat () .x ();
	Type y = r .quat () .y ();
	Type z = r .quat () .z ();
	Type w = r .quat () .w ();

	array [ 0] = 1 - 2 * (y * y + z * z);
	array [ 1] =     2 * (x * y + z * w);
	array [ 2] =     2 * (z * x - y * w);
	array [ 3] = 0;

	array [ 4] =     2 * (x * y - z * w);
	array [ 5] = 1 - 2 * (z * z + x * x);
	array [ 6] =     2 * (y * z + x * w);
	array [ 7] = 0;

	array [ 8] =     2 * (z * x + y * w);
	array [ 9] =     2 * (y * z - x * w);
	array [10] = 1 - 2 * (y * y + x * x);
	array [11] = 0;

	array [12] = 0;
	array [13] = 0;
	array [14] = 0;
	array [15] = 1;
}

template <class Type>
rotation4 <Type>
matrix4 <Type>::rotation () const
{
	Type quat [4];

	int i;

	// First, find largest diagonal in matrix:
	if (value [0] [0] > value [1] [1])
	{
		i = value [0] [0] > value [2] [2] ? 0 : 2;
	}
	else
	{
		i = value [1] [1] > value [2] [2] ? 1 : 2;
	}

	Type scalerow = value [0] [0] + value [1] [1] + value [2] [2];

	if (scalerow > value [i] [i])
	{
		// Compute w first:
		quat [3] = std::sqrt (scalerow + value [3] [3]) / 2;

		// And compute other values:
		quat [0] = (value [1] [2] - value [2] [1]) / (4 * quat [3]);
		quat [1] = (value [2] [0] - value [0] [2]) / (4 * quat [3]);
		quat [2] = (value [0] [1] - value [1] [0]) / (4 * quat [3]);
	}
	else
	{
		// Compute x, y, or z first:
		int j = (i + 1) % 3;
		int k = (i + 2) % 3;

		// Compute first value:
		quat [i] = std::sqrt (value [i] [i] - value [j] [j] - value [k] [k] + value [3] [3]) / 2;

		// And the others:
		quat [j] = (value [i] [j] + value [j] [i]) / (4 * quat [i]);
		quat [k] = (value [i] [k] + value [k] [i]) / (4 * quat [i]);

		quat [3] = (value [j] [k] - value [k] [j]) / (4 * quat [i]);
	}

	return rotation4 <Type> (quaternion <Type> (quat [0], quat [1], quat [2], quat [3]));
}

template <class Type>
void
matrix4 <Type>::set ()
{
	value = Identity .value;
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation)
{
	value [0] = vector4 <Type> (1, 0, 0, 0);
	value [1] = vector4 <Type> (0, 1, 0, 0);
	value [2] = vector4 <Type> (0, 0, 1, 0);
	value [3] = vector4 <Type> (translation .x (), translation .y (), translation .z (), 1);
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation,
                     const rotation4 <Type> & rotation)
{
	set (translation);

	if (rotation not_eq rotation4 <Type> ())
		rotate (rotation);
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation,
                     const rotation4 <Type> &  rotation,
                     const vector3 <Type> & scaleFactor)
{
	set (translation);

	if (rotation not_eq rotation4 <Type> ())
		rotate (rotation);

	if (scaleFactor not_eq vector3 <Type> (1, 1, 1))
		scale (scaleFactor);
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation,
                     const rotation4 <Type> &  rotation,
                     const vector3 <Type> & scaleFactor,
                     const rotation4 <Type> &  scaleOrientation)
{
	set (translation);

	if (rotation not_eq rotation4 <Type> ())
		rotate (rotation);

	if (scaleFactor not_eq vector3 <Type> (1, 1, 1))
	{
		if (scaleOrientation not_eq rotation4 <Type> ())
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq rotation4 <Type> ())
			rotate (~scaleOrientation);
	}
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation,
                     const rotation4 <Type> &  rotation,
                     const vector3 <Type> & scaleFactor,
                     const rotation4 <Type> &  scaleOrientation,
                     const vector3 <Type> & center)
{
	// P' = T * C * R * SR * S * -SR * -C * P
	set (translation);

	if (center not_eq vector3 <Type> ())
		translate (center);

	if (rotation not_eq rotation4 <Type> ())
		rotate (rotation);

	if (scaleFactor not_eq vector3 <Type> (1, 1, 1))
	{
		if (scaleOrientation not_eq rotation4 <Type> ())
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq rotation4 <Type> ())
			rotate (~scaleOrientation);
	}

	if (center not_eq vector3 <Type> ())
		translate (-center);
}

template <class Type>
template <class T>
void
matrix4 <Type>::get (vector3 <T> & translation) const
{
	translation = this -> translation ();
}

template <class Type>
template <class T, class R>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation) const
{
	matrix4 <Type> so, rot;
	vector3 <Type> scaleFactor;
	factor (translation, rot, scaleFactor, so);
	rotation = rot .rotation ();
}

template <class Type>
template <class T, class R, class S>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation,
                     vector3 <S> & scaleFactor) const
{
	matrix4 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation = rot .rotation ();
}

template <class Type>
template <class T, class R, class S, class SO>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation,
                     vector3 <S> & scaleFactor,
                     rotation4 <SO> & scaleOrientation) const
{
	matrix4 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation         = rot .rotation ();
	scaleOrientation = so .rotation ();
}

template <class Type>
template <class T, class R, class S, class SO, class C>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation,
                     vector3 <S> & scaleFactor,
                     rotation4 <SO> & scaleOrientation,
                     const vector3 <C> & center) const
{
	matrix4 <Type> m, c;

	m .set (-center);
	m .multLeft (*this);
	c .set (center);
	m .multLeft (c);

	m .get (translation, rotation, scaleFactor, scaleOrientation);
}

template <class Type>
bool
matrix4 <Type>::factor (vector3 <Type> & translation,
                        matrix4 & rotation,
                        vector3 <Type> & scale,
                        matrix4 & scaleOrientation) const
{
	matrix4 a (*this);

	// (2) Get translation and create 3x3 matrix.
	for (size_t i = 0; i < 3; ++ i)
	{
		translation [i]  = value [3] [i];
		a .value [3] [i] = a .value [i] [3] = 0;
	}

	a .value [3] [3] = 1;

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	Type det      = a .det3 ();
	Type det_sign = (det < 0 ? -1 : 1);

	if (det_sign * det == 0)
		return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              // singular

	// (4) B = A * !A  (here !A means A transpose)
	matrix4 b = a * ! a;

	Type evalues [3];
	Type evectors [3] [3];

	eigen_decomposition (b, evalues, evectors);

	// find min / max eigenvalues and do ratio test to determine singularity

	scaleOrientation = matrix4 (evectors [0] [0], evectors [0] [1], evectors [0] [2], 0,
	                            evectors [1] [0], evectors [1] [1], evectors [1] [2], 0,
	                            evectors [2] [0], evectors [2] [1], evectors [2] [2], 0,
	                            0, 0, 0, 1);

	// Compute s = sqrt(evalues), with sign. Set si = s-inverse
	matrix4 si;

	for (size_t i = 0; i < 3; ++ i)
	{
		scale [i]         = det_sign * std::sqrt (evalues [i]);
		si .value [i] [i] = 1 / scale [i];
	}

	// (5) Compute U = !R ~S R A.
	rotation = scaleOrientation * si * ! scaleOrientation * a;

	scaleOrientation .transpose ();

	return true;
}

template <class Type>
inline
Type
matrix4 <Type>::det3 () const
{
	return det3 (0, 1, 2, 0, 1, 2);
}

template <class Type>
Type
matrix4 <Type>::det3 (int r1, int r2, int r3, int c1, int c2, int c3) const
{
	Type a11 = value [r1] [c1];
	Type a12 = value [r1] [c2];
	Type a13 = value [r1] [c3];
	Type a21 = value [r2] [c1];
	Type a22 = value [r2] [c2];
	Type a23 = value [r2] [c3];
	Type a31 = value [r3] [c1];
	Type a32 = value [r3] [c2];
	Type a33 = value [r3] [c3];

	Type M11 =   a22 * a33 - a32 * a23;
	Type M21 = -(a12 * a33 - a32 * a13);
	Type M31 =   a12 * a23 - a22 * a13;

	return (a11 * M11 + a21 * M21 + a31 * M31);
}

template <class Type>
Type
matrix4 <Type>::determinant () const
{
	return value [0] [3] * det3 (1, 2, 3, 0, 1, 2)
	       + value [1] [3] * det3 (0, 2, 3, 0, 1, 2)
	       + value [2] [3] * det3 (0, 1, 3, 0, 1, 2)
	       + value [3] [3] * det3 (0, 1, 2, 0, 1, 2);
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::transpose ()
{
	return *this = matrix4 <Type> (array [0], array [4], array [8],  array [12],
	                               array [1], array [5], array [9],  array [13],
	                               array [2], array [6], array [10], array [14],
	                               array [3], array [7], array [11], array [15]);
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::inverse ()
{
	Type det = determinant ();

	if (det == 0)
		throw std::domain_error ("matrix4::inverse: determinant is 0.");

	return *this = matrix4 <Type> (det3 (1, 2, 3, 1, 2, 3) / det,
	                               -det3 (0, 2, 3, 1, 2, 3) / det,
	                               det3 (0, 1, 3, 1, 2, 3) / det,
	                               -det3 (0, 1, 2, 1, 2, 3) / det,
	                               -det3 (1, 2, 3, 0, 2, 3) / det,
	                               det3 (0, 2, 3, 0, 2, 3) / det,
	                               -det3 (0, 1, 3, 0, 2, 3) / det,
	                               det3 (0, 1, 2, 0, 2, 3) / det,
	                               det3 (1, 2, 3, 0, 1, 3) / det,
	                               -det3 (0, 2, 3, 0, 1, 3) / det,
	                               det3 (0, 1, 3, 0, 1, 3) / det,
	                               -det3 (0, 1, 2, 0, 1, 3) / det,
	                               -det3 (1, 2, 3, 0, 1, 2) / det,
	                               det3 (0, 2, 3, 0, 1, 2) / det,
	                               -det3 (0, 1, 3, 0, 1, 2) / det,
	                               det3 (0, 1, 2, 0, 1, 2) / det);
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;
	value [2] *= t;
	value [3] *= t;

	return *this;
}

template <class Type>
template <class T>
matrix4 <Type> &
matrix4 <Type>::operator *= (const matrix4 <T> & matrix)
{
	return multRight (matrix);
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;
	value [3] /= t;

	return *this;
}

/**
 * It takes 64 multiplies and 64 adds.
 */
template <class Type>
template <class T>
matrix4 <Type> &
matrix4 <Type>::multLeft (const matrix4 <T> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (array [0 * 4 + j] * matrix .array [i * 4 + 0] +   \
	    array [1 * 4 + j] * matrix .array [i * 4 + 1] +   \
	    array [2 * 4 + j] * matrix .array [i * 4 + 2] +   \
	    array [3 * 4 + j] * matrix .array [i * 4 + 3])

	return *this = matrix4 <Type> (MULT_LEFT (0, 0),
	                               MULT_LEFT (0, 1),
	                               MULT_LEFT (0, 2),
	                               MULT_LEFT (0, 3),

	                               MULT_LEFT (1, 0),
	                               MULT_LEFT (1, 1),
	                               MULT_LEFT (1, 2),
	                               MULT_LEFT (1, 3),

	                               MULT_LEFT (2, 0),
	                               MULT_LEFT (2, 1),
	                               MULT_LEFT (2, 2),
	                               MULT_LEFT (2, 3),

	                               MULT_LEFT (3, 0),
	                               MULT_LEFT (3, 1),
	                               MULT_LEFT (3, 2),
	                               MULT_LEFT (3, 3));

	#undef MULT_LEFT
}

/**
 * It takes 64 multiplies and 64 adds.
 */
template <class Type>
template <class T>
matrix4 <Type> &
matrix4 <Type>::multRight (const matrix4 <T> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (array [i * 4 + 0] * matrix .array [0 * 4 + j] +   \
	    array [i * 4 + 1] * matrix .array [1 * 4 + j] +   \
	    array [i * 4 + 2] * matrix .array [2 * 4 + j] +   \
	    array [i * 4 + 3] * matrix .array [3 * 4 + j])

	return *this = matrix4 <Type> (MULT_RIGHT (0, 0),
	                               MULT_RIGHT (0, 1),
	                               MULT_RIGHT (0, 2),
	                               MULT_RIGHT (0, 3),

	                               MULT_RIGHT (1, 0),
	                               MULT_RIGHT (1, 1),
	                               MULT_RIGHT (1, 2),
	                               MULT_RIGHT (1, 3),

	                               MULT_RIGHT (2, 0),
	                               MULT_RIGHT (2, 1),
	                               MULT_RIGHT (2, 2),
	                               MULT_RIGHT (2, 3),

	                               MULT_RIGHT (3, 0),
	                               MULT_RIGHT (3, 1),
	                               MULT_RIGHT (3, 2),
	                               MULT_RIGHT (3, 3));

	#undef MULT_RIGHT
}

/**
 * It takes 12 multiplies, 3 divs and 12 adds.
 */
template <class Type>
template <class T>
vector3 <Type>
matrix4 <Type>::multVecMatrix (const vector3 <T> & vector) const
{
	Type w = vector .x () * array [3] + vector .y () * array [7] + vector .z () * array [11] + array [15];

	return vector3 <Type> ((vector .x () * array [0] + vector .y () * array [4] + vector .z () * array [ 8] + array [12]) / w,
	                       (vector .x () * array [1] + vector .y () * array [5] + vector .z () * array [ 9] + array [13]) / w,
	                       (vector .x () * array [2] + vector .y () * array [6] + vector .z () * array [10] + array [14]) / w);
}

/**
 * It takes 12 multiplies, 3 divs and 12 adds.
 */
template <class Type>
template <class T>
vector3 <Type>
matrix4 <Type>::multMatrixVec (const vector3 <T> & vector) const
{
	Type w = vector .x () * array [12] + vector .y () * array [13] + vector .z () * array [14] + array [15];

	return vector3 <Type> ((vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [ 2] + array [ 3]) / w,
	                       (vector .x () * array [4] + vector .y () * array [5] + vector .z () * array [ 6] + array [ 7]) / w,
	                       (vector .x () * array [8] + vector .y () * array [9] + vector .z () * array [10] + array [11]) / w);
}

/**
 * It takes 9 multiplies and 6 adds.
 */
template <class Type>
template <class T>
vector3 <Type>
matrix4 <Type>::multDirMatrix (const vector3 <T> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [4] + vector .z () * array [ 8],
	                       vector .x () * array [1] + vector .y () * array [5] + vector .z () * array [ 9],
	                       vector .x () * array [2] + vector .y () * array [6] + vector .z () * array [10]);
}

/**
 * It takes 9 multiplies and 6 adds.
 */
template <class Type>
template <class T>
vector3 <Type>
matrix4 <Type>::multMatrixDir (const vector3 <T> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [ 2],
	                       vector .x () * array [4] + vector .y () * array [5] + vector .z () * array [ 6],
	                       vector .x () * array [8] + vector .y () * array [9] + vector .z () * array [10]);
}

/**
 * It takes 9 multiplies and 15 adds.
 */
template <class Type>
matrix4 <Type> &
matrix4 <Type>::translate (const vector3 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (value [0] [i] * translation .x () +   \
	    value [1] [i] * translation .y () +   \
	    value [2] [i] * translation .z ())

	value [3] [0] += TRANSLATE (0);
	value [3] [1] += TRANSLATE (1);
	value [3] [2] += TRANSLATE (2);

	#undef TRANSLATE

	return *this;
}

/**
 * Takes the same as a multLeft and a rotation (rot).
 */
template <class Type>
inline
matrix4 <Type> &
matrix4 <Type>::rotate (const rotation4 <Type> & rotation)
{
	return multLeft (matrix4 <Type> (rotation));
}

/**
 * It takes 9 multiplies.
 */
template <class Type>
matrix4 <Type> &
matrix4 <Type>::scale (const vector3 <Type> & scaleFactor)
{
	value [0] [0] *= scaleFactor .x ();
	value [0] [1] *= scaleFactor .x ();
	value [0] [2] *= scaleFactor .x ();

	value [1] [0] *= scaleFactor .y ();
	value [1] [1] *= scaleFactor .y ();
	value [1] [2] *= scaleFactor .y ();

	value [2] [0] *= scaleFactor .z ();
	value [2] [1] *= scaleFactor .z ();
	value [2] [2] *= scaleFactor .z ();

	return *this;
}

///  @relates matrix4
///  @name Comparision operations

///  Compares two matrix4 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
inline
bool
operator == (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .vector () == rhs .vector ();
}

///  Compares two matrix4 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
bool
operator not_eq (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .vector () not_eq rhs .vector ();
}

///  @relates matrix4
///  @name Arithmetic operations

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix4 <Type>
operator ! (const matrix4 <Type> & matrix)
{
	return matrix4 <Type> (matrix) .transpose ();
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix4 <Type>
transpose (const matrix4 <Type> & matrix)
{
	return matrix4 <Type> (matrix) .transpose ();
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix4 <Type>
operator ~ (const matrix4 <Type> & matrix)
{
	return matrix4 <Type> (matrix) .inverse ();
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix4 <Type>
inverse (const matrix4 <Type> & matrix)
{
	return matrix4 <Type> (matrix) .inverse ();
}

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix4 <Type>
operator * (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (lhs) .multRight (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const matrix4 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .multMatrixVec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return rhs .multVecMatrix (lhs);
}

///  @relates matrix4
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, matrix4 <Type> & matrix)
{
	Type
	   e11, e12, e13, e14,
	   e21, e22, e23, e24,
	   e31, e32, e33, e34,
	   e41, e42, e43, e44;

	istream
		>> e11
		>> e12
		>> e13
		>> e14
		>> e21
		>> e22
		>> e23
		>> e24
		>> e31
		>> e32
		>> e33
		>> e34
		>> e41
		>> e42
		>> e43
		>> e44;

	if (istream)
		matrix = matrix4 <Type> (e11, e12, e13, e14,
		                         e21, e22, e23, e24,
		                         e31, e32, e33, e34,
		                         e41, e42, e43, e44);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const matrix4 <Type> & matrix)
{
	return ostream << matrix .vector ();
}

extern template class matrix4 <float>;
extern template class matrix4 <double>;
extern template class matrix4 <long double>;

//
extern template std::istream & operator >> (std::istream &, matrix4 <float> &);
extern template std::istream & operator >> (std::istream &, matrix4 <double> &);
extern template std::istream & operator >> (std::istream &, matrix4 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const matrix4 <float> &);
extern template std::ostream & operator << (std::ostream &, const matrix4 <double> &);
extern template std::ostream & operator << (std::ostream &, const matrix4 <long double> &);

} // math
} // titania

#endif
