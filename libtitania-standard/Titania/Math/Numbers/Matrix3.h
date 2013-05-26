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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX3_H__
#define __TITANIA_MATH_NUMBERS_MATRIX3_H__

#include <cfloat>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "Vector2.h"
#include "Vector3.h"

#include "../Algorithms/EigenDecomposition.h"

namespace titania {
namespace math {

template <class Type>
class matrix3;

template <class Type>
inline
matrix3 <Type>
operator ! (const matrix3 <Type> & matrix);

template <class Type>
inline
matrix3 <Type>
transpose (const matrix3 <Type> & matrix);

template <class Type>
inline
matrix3 <Type>
operator ~ (const matrix3 <Type> & matrix);

template <class Type>
inline
matrix3 <Type>
inverse (const matrix3 <Type> & matrix);

template <class Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs);

template <class Type>
class matrix3
{
private:

	static constexpr size_t Order = 3;
	static constexpr size_t Size  = Order * Order;


public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  Vector2 typedef.
	typedef vector2 <Type> vector2_type;

	///  Vector3 typedef.
	typedef Type rotation_type;

	///  Vector3 typedef.
	typedef vector3 <Type> vector3_type;

	///  Vector typedef.
	///  This is the type for the vector representation of this matrix.
	///  The matrix consists of four vectors of type vector3.
	///  0 [     0 ]
	///  1 [     0 ]
	///  2 [ x y 1 ]
	typedef vector3 <vector3_type> vector_type;

	///  Array typedef.
	typedef Type array_type [Size];

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	constexpr
	matrix3 () :
		value (Identity .value)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix3 (const matrix3 <Up> & matrix) :
		value (matrix [0], matrix [1], matrix [2])
	{ }

	///  Value constructor.
	explicit
	constexpr
	matrix3 (const array_type & matrix) :
		array
	{
		matrix [ 0], matrix [ 1], matrix [ 2],
		matrix [ 3], matrix [ 4], matrix [ 5],
		matrix [ 6], matrix [ 7], matrix [ 8]
	} { }

	///  Components constructor. Set values from @a e11 to @a e33.
	constexpr
	matrix3 (const Type & e11, const Type & e12, const Type & e13,
	         const Type & e21, const Type & e22, const Type & e23,
	         const Type & e31, const Type & e32, const Type & e33) :
		array
	{
		e11, e12, e13,
		e21, e22, e23,
		e31, e32, e33
	} { }

	///  Constructs a matrix4 from a rotation4.
	explicit
	matrix3 (const Type & rot)
	{ rotation (rot); }

	///  @name Assignment operators

	template <class T>
	matrix3 &
	operator = (const matrix3 <T> &);

	///  @name Element access

	vector2 <Type>
	translation () const;

	void
	rotation (const Type &);

	Type
	rotation () const;

	void
	set ();

	void
	set (const vector2 <Type> &);

	void
	set (const vector2 <Type> &, const Type &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &, const Type &);

	void
	set (const vector2 <Type> &, const Type &, const vector2 <Type> &, const Type &, const vector2 <Type> &);

	void
	get (vector2 <Type> &) const;

	void
	get (vector2 <Type> &, Type &) const;

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &) const;

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &, Type &) const;

	void
	get (vector2 <Type> &, Type &, vector2 <Type> &, Type &, vector2 <Type> &) const;

	///  Access rows by @a index.
	vector3_type &
	operator [ ] (const size_type index) { return value [index]; }

	constexpr vector3_type
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
	matrix3 &
	transpose ();

	///  Returns this matrix inversed.
	matrix3 &
	inverse ();

	///  Returns this matrix multiplies by @a scalar.
	matrix3 &
	operator *= (const Type &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	matrix3 &
	operator *= (const matrix3 <T> &);

	///  Returns this matrix divided by @a scalar.
	matrix3 &
	operator /= (const Type &);

	///  Returns this matrix left multiplied by @a matrix.
	template <class T>
	matrix3 &
	multLeft (const matrix3 <T> &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	matrix3 &
	multRight (const matrix3 <T> &);

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	vector2 <Type>
	multVecMatrix (const vector2 <T> &) const;

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	vector3 <Type>
	multVecMatrix (const vector3 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	vector2 <Type>
	multMatrixVec (const vector2 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	vector3 <Type>
	multMatrixVec (const vector3 <T> &) const;

	///  Returns a new vector that is @vector (a normal or direction vector) multiplies by matrix.
	template <class T>
	vector2 <Type>
	multDirMatrix (const vector2 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector (a normal or direction vector).
	template <class T>
	vector2 <Type>
	multMatrixDir (const vector2 <T> &) const;

	///  Returns this matrix translated by @a translation.
	matrix3 &
	translate (const vector2 <Type> &);

	///  Returns this matrix rotated by @a rotation.
	matrix3 &
	rotate (const Type &);

	///  Returns this matrix scaled by @a scale.
	matrix3 &
	scale (const vector2 <Type> &);


private:

	bool
	factor (vector2 <Type> & translation,
	        matrix3 & rotation,
	        vector2 <Type> & scale,
	        matrix3 & scaleOrientation) const;

	Type
	det2 () const;

	union
	{
		vector_type value;
		array_type array;
	};

	static const matrix3 Identity;

};

template <class Type>
const matrix3 <Type> matrix3 <Type>::Identity = { 1, 0, 0,
	                                               0, 1, 0,
	                                               0, 0, 1 };

template <class Type>
template <class Up>
inline
matrix3 <Type> &
matrix3 <Type>::operator = (const matrix3 <Up> & matrix)
{
	value = matrix .vector ();
	return *this;
}

template <class Type>
vector2 <Type>
matrix3 <Type>::translation () const
{
	return vector2 <Type> (array [6],
	                       array [7]);
}

template <class Type>
void
matrix3 <Type>::rotation (const Type & rotation)
{
	Type sinAngle = std::sin (rotation);
	Type cosAngle = std::cos (rotation);

	*this = Identity;

	array [0] = cosAngle;
	array [1] = sinAngle;
	array [3] = -sinAngle;
	array [4] = cosAngle;
}

template <class Type>
Type
matrix3 <Type>::rotation () const
{
	return std::atan2 (array [1], array [0]);
}

template <class Type>
void
matrix3 <Type>::set ()
{
	value = Identity .value;
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation)
{
	value [0] = vector3 <Type> (1, 0, 0);
	value [1] = vector3 <Type> (0, 1, 0);
	value [2] = vector3 <Type> (translation .x (), translation .y (), 1);
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation)
{
	set (translation);

	if (rotation not_eq Type (0))
		rotate (rotation);
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor)
{
	set (translation);

	if (rotation not_eq Type (0))
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
		scale (scaleFactor);
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor,
                     const Type & scaleOrientation)
{
	set (translation);

	if (rotation not_eq Type (0))
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
	{
		if (scaleOrientation not_eq Type (0))
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq Type (0))
			rotate (-scaleOrientation);
	}
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation,
                     const Type & rotation,
                     const vector2 <Type> & scaleFactor,
                     const Type & scaleOrientation,
                     const vector2 <Type> & center)
{
	set (translation);

	if (center not_eq vector2 <Type> ())
		translate (center);

	if (rotation not_eq Type (0))
		rotate (rotation);

	if (scaleFactor not_eq vector2 <Type> (1, 1))
	{
		if (scaleOrientation not_eq Type (0))
			rotate (scaleOrientation);

		scale (scaleFactor);

		if (scaleOrientation not_eq Type (0))
			rotate (-scaleOrientation);
	}

	if (center not_eq vector2 <Type> ())
		translate (-center);
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation) const
{
	translation = this -> translation ();
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation) const
{
	matrix3 <Type> so, rot;
	vector2 <Type> scaleFactor;
	factor (translation, rot, scaleFactor, so);
	rotation = rot .rotation ();
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor) const
{
	matrix3 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation = rot .rotation ();
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor,
                     Type & scaleOrientation) const
{
	matrix3 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation         = rot .rotation ();
	scaleOrientation = so .rotation ();
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor,
                     Type & scaleOrientation,
                     vector2 <Type> & center) const
{
	matrix3 <Type> m, c;

	m .set (-center);
	m .multLeft (*this);
	c .set (center);
	m .multLeft (c);

	m .get (translation, rotation, scaleFactor, scaleOrientation);
}

template <class Type>
bool
matrix3 <Type>::factor (vector2 <Type> & translation,
                        matrix3 & rotation,
                        vector2 <Type> & scale,
                        matrix3 & scaleOrientation) const
{
	matrix3 a (*this);

	// (2) Get translation and create 3x3 matrix.
	for (size_t i = 0; i < 2; ++ i)
	{
		translation [i]  = value [2] [i];
		a .value [2] [i] = a .value [i] [2] = 0;
	}

	a .value [2] [2] = 1;

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	Type det      = a .det2 ();
	Type det_sign = (det < 0 ? -1 : 1);

	if (det_sign * det == 0)
		return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             // singular

	// (4) B = A * !A  (here !A means A transpose)
	matrix3 b = a * ! a;

	Type evalues [2];
	Type evectors [2] [2];

	eigen_decomposition (b, evalues, evectors);

	// find min / max eigenvalues and do ratio test to determine singularity

	scaleOrientation = matrix3 (evectors [0] [0], evectors [0] [1], 0,
	                            evectors [1] [0], evectors [1] [1], 0,
	                            0, 0, 1);

	// Compute s = sqrt(evalues), with sign. Set si = s-inverse
	matrix3 si;

	for (size_t i = 0; i < 2; ++ i)
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
Type
matrix3 <Type>::det2 () const
{
	return
	   array [0] * array [4] -
	   array [1] * array [3];
}

template <class Type>
Type
matrix3 <Type>::determinant () const
{
	return array [0] * (array [4] * array [8] - array [5] * array [7]) -
	       array [1] * (array [3] * array [8] - array [5] * array [6]) +
	       array [2] * (array [3] * array [7] - array [4] * array [6]);
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::transpose ()
{
	return *this = matrix3 <Type> (array [0], array [3], array [6],
	                               array [1], array [4], array [7],
	                               array [2], array [5], array [8]);
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::inverse ()
{
	Type m00 = array [0];
	Type m01 = array [1];
	Type m02 = array [2];

	Type m10 = array [3];
	Type m11 = array [4];
	Type m12 = array [5];

	Type m20 = array [6];
	Type m21 = array [7];
	Type m22 = array [8];

	Type t4  = m00 * m11;
	Type t6  = m00 * m21;
	Type t8  = m10 * m01;
	Type t10 = m10 * m21;
	Type t12 = m20 * m01;
	Type t14 = m20 * m11;

	Type d = (t4 * m22 - t6 * m12 - t8 * m22 + t10 * m02 + t12 * m12 - t14 * m02);

	if (d == 0)
		throw std::domain_error ("matrix3::inverse: determinant is 0.");

	return *this = matrix3 <Type> ((m11 * m22 - m21 * m12) / d,
	                               -(m01 * m22 - m21 * m02) / d,
	                               (m01 * m12 - m11 * m02) / d,

	                               -(m10 * m22 - m20 * m12) / d,
	                               (m00 * m22 - m20 * m02) / d,
	                               -(m00 * m12 - m10 * m02) / d,

	                               (t10 - t14) / d,
	                               -(t6 - t12) / d,
	                               (t4 - t8) / d);
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;
	value [2] *= t;

	return *this;
}

template <class Type>
template <class T>
matrix3 <Type> &
matrix3 <Type>::operator *= (const matrix3 <T> & matrix)
{
	return multRight (matrix);
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;

	return *this;
}

template <class Type>
template <class T>
matrix3 <Type> &
matrix3 <Type>::multLeft (const matrix3 <T> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (array [0 * 3 + j] * matrix .array [i * 3 + 0] +   \
	    array [1 * 3 + j] * matrix .array [i * 3 + 1] +   \
	    array [2 * 3 + j] * matrix .array [i * 3 + 2])

	return *this = matrix3 <Type> (MULT_LEFT (0, 0),
	                               MULT_LEFT (0, 1),
	                               MULT_LEFT (0, 2),

	                               MULT_LEFT (1, 0),
	                               MULT_LEFT (1, 1),
	                               MULT_LEFT (1, 2),

	                               MULT_LEFT (2, 0),
	                               MULT_LEFT (2, 1),
	                               MULT_LEFT (2, 2));

	#undef MULT_LEFT
}

template <class Type>
template <class T>
matrix3 <Type> &
matrix3 <Type>::multRight (const matrix3 <T> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (array [i * 3 + 0] * matrix .array [0 * 3 + j] +   \
	    array [i * 3 + 1] * matrix .array [1 * 3 + j] +   \
	    array [i * 3 + 2] * matrix .array [2 * 3 + j])

	return *this = matrix3 <Type> (MULT_RIGHT (0, 0),
	                               MULT_RIGHT (0, 1),
	                               MULT_RIGHT (0, 2),

	                               MULT_RIGHT (1, 0),
	                               MULT_RIGHT (1, 1),
	                               MULT_RIGHT (1, 2),

	                               MULT_RIGHT (2, 0),
	                               MULT_RIGHT (2, 1),
	                               MULT_RIGHT (2, 2));

	#undef MULT_RIGHT
}

template <class Type>
template <class T>
vector2 <Type>
matrix3 <Type>::multVecMatrix (const vector2 <T> & vector) const
{
	Type w = vector .x () * array [2] + vector .y () * array [5] + array [8];

	return vector2 <Type> ((vector .x () * array [0] + vector .y () * array [3] + array [6]) / w,
	                       (vector .x () * array [1] + vector .y () * array [4] + array [7]) / w);
}

template <class Type>
template <class T>
vector3 <Type>
matrix3 <Type>::multVecMatrix (const vector3 <T> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [3] + vector .z () * array [6],
	                       vector .x () * array [1] + vector .y () * array [4] + vector .z () * array [7],
	                       vector .x () * array [2] + vector .y () * array [5] + vector .z () * array [8]);
}

template <class Type>
template <class T>
vector2 <Type>
matrix3 <Type>::multMatrixVec (const vector2 <T> & vector) const
{
	Type w = vector .x () * array [6] + vector .y () * array [7] + array [8];

	return vector2 <Type> ((vector .x () * array [0] + vector .y () * array [1] + array [2]) / w,
	                       (vector .x () * array [3] + vector .y () * array [4] + array [5]) / w);
}

template <class Type>
template <class T>
vector3 <Type>
matrix3 <Type>::multMatrixVec (const vector3 <T> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [2],
	                       vector .x () * array [3] + vector .y () * array [4] + vector .z () * array [5],
	                       vector .x () * array [6] + vector .y () * array [7] + vector .z () * array [8]);
}

template <class Type>
template <class T>
vector2 <Type>
matrix3 <Type>::multDirMatrix (const vector2 <T> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [3],
	                       vector .x () * array [1] + vector .y () * array [4]);
}

template <class Type>
template <class T>
vector2 <Type>
matrix3 <Type>::multMatrixDir (const vector2 <T> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [1],
	                       vector .x () * array [3] + vector .y () * array [4]);
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::translate (const vector2 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (value [0] [i] * translation .x () +   \
	    value [1] [i] * translation .y ())

	value [2] [0] += TRANSLATE (0);
	value [2] [1] += TRANSLATE (1);

	#undef TRANSLATE

	return *this;
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::rotate (const Type & rotation)
{
	return multLeft (matrix3 <Type> (rotation));
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::scale (const vector2 <Type> & scaleFactor)
{
	value [0] [0] *= scaleFactor .x ();
	value [1] [0] *= scaleFactor .y ();

	value [0] [1] *= scaleFactor .x ();
	value [1] [1] *= scaleFactor .y ();

	return *this;
}

///  @relates matrix3
///  @name Comparision operations

///  Compares two matrix3 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
bool
operator == (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .vector () == rhs .vector ();
}

///  Compares two matrix3 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
bool
operator not_eq (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .vector () not_eq rhs .vector ();
}

///  @relates matrix3
///  @name Arithmetic operations

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix3 <Type>
operator ! (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .transpose ();
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix3 <Type>
transpose (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .transpose ();
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix3 <Type>
operator ~ (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .inverse ();
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix3 <Type>
inverse (const matrix3 <Type> & matrix)
{
	return matrix3 <Type> (matrix) .inverse ();
}

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (lhs) .multRight (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const matrix3 <Type> & lhs, const vector2 <Type> & rhs)
{
	return lhs .multMatrixVec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const matrix3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .multMatrixVec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const vector2 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs .multVecMatrix (lhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs .multVecMatrix (lhs);
}

///  @relates matrix3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, matrix3 <Type> & matrix)
{
	Type
	   e11, e12, e13,
	   e21, e22, e23,
	   e31, e32, e33;

	istream
		>> e11
		>> e12
		>> e13
		>> e21
		>> e22
		>> e23
		>> e31
		>> e32
		>> e33;

	if (istream)
		matrix = matrix3 <Type> (e11, e12, e13,
		                         e21, e22, e23,
		                         e31, e32, e33);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const matrix3 <Type> & matrix)
{
	return ostream << matrix .vector ();
}

extern template class matrix3 <float>;
extern template class matrix3 <double>;
extern template class matrix3 <long double>;

//
extern template std::istream & operator >> (std::istream &, matrix3 <float> &);
extern template std::istream & operator >> (std::istream &, matrix3 <double> &);
extern template std::istream & operator >> (std::istream &, matrix3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const matrix3 <float> &);
extern template std::ostream & operator << (std::ostream &, const matrix3 <double> &);
extern template std::ostream & operator << (std::ostream &, const matrix3 <long double> &);

} // math
} // titania

#endif
