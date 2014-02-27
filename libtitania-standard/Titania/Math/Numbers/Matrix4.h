/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

/**
 *  Template to represent square matrix of order 4.
 *
 *  Matrix4 matrices are organized in row-major fashion.  The first row of the
 *  matrix stores information for the x dimension, the second for the y dimension,
 *  and the third for the z dimension.  Since these data types are commonly used
 *  for transformation matrices, translation values are stored in the fourth row.
 *
 *  0 [ x-axis 0 ]
 *  1 [ y-axis 0 ]
 *  2 [ z-axis 0 ]
 *  3 [ x y z  1 ]
 *
 *  The default value of a matrix4 number is the identity matrix [1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1].
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values.
 */
template <class Type>
class matrix4
{
private:

	static constexpr size_t Order = 4;
	static constexpr size_t Size  = Order * Order;


public:

	///  Size typedef.  Used for size and indices.
	using size_type = size_t;

	///  Value typedef.
	using value_type = Type;

	///  Matrix typedef.
	using matrix_type = vector4 <vector4 <Type>>;

	///  Array typedef.
	typedef Type array_type [Size];

	///  Translation typedef.
	using translation_type = vector3 <Type>;

	///  Rotation typedef.
	using rotation_type = rotation4 <Type>;

	///  Scale typedef.
	using scale_type = vector3 <Type>;

	///  Vector typedef.
	using vector_type = vector4 <Type>;

	///  Point typedef.
	using point_type = vector3 <Type>;

	///  Normal typedef.
	using normal_type = vector3 <Type>;

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	matrix4 () :
		matrix4 (Identity)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix4 (const matrix4 <Up> & matrix) :
		value (matrix [0], matrix [1], matrix [2], matrix [3])
	{ }

	matrix4 (const matrix4 & matrix)
	{ *this = matrix; }

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
	}

	{ }

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
	}

	{ }

	///  Constructs a matrix4 from a rotation4.
	explicit
	matrix4 (const rotation4 <Type> & rot) :
		matrix4 (matrix3 <Type> (rot))
	{ }

	///  Constructs a matrix4 from a matrix3 rotation matrix.
	explicit
	constexpr
	matrix4 (const matrix3 <Type> & matrix) :
		array
	{
		matrix [0] [0],
		matrix [0] [1],
		matrix [0] [2],
		0,
		matrix [1] [0],
		matrix [1] [1],
		matrix [1] [2],
		0,
		matrix [2] [0],
		matrix [2] [1],
		matrix [2] [2],
		0,
		0, 0, 0, 1,
	}

	{ }

	///  @name Assignment operators

	template <class Up>
	matrix4 &
	operator = (const matrix4 <Up> &);

	matrix4 &
	operator = (const matrix4 &);

	///  @name Element access

	constexpr
	point_type
	x () const
	{ return point_type (array [0], array [1], array [2]); }

	constexpr
	point_type
	y () const
	{ return point_type (array [4], array [5], array [6]); }

	constexpr
	point_type
	z () const
	{ return point_type (array [8], array [9], array [10]); }

	constexpr
	point_type
	origin () const
	{ return point_type (array [12], array [13], array [14]); }

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
	vector_type &
	operator [ ] (const size_type index)
	{ return value [index]; }

	const vector_type &
	operator [ ] (const size_type index) const
	{ return value [index]; }

	constexpr
	operator matrix3 <Type> () const
	{ return matrix3 <Type> (array [0], array [1], array [2],
		                      array [4], array [5], array [6],
		                      array [8], array [9], array [10]); }

	///  Returns pointer to the underlying array serving as element storage.
	///  Specifically the pointer is such that range [data (); data () + size ()) is valid.
	Type*
	data ()
	{ return array; }

	const Type*
	data () const
	{ return array; }

	///  Get access to the underlying vector representation of this matrix.
	void
	vector (const matrix_type & vector)
	{ value = vector; }

	const matrix_type &
	vector () const
	{ return value; }

	///  @name Capacity

	///  Returns the order of the matrix.
	static
	constexpr size_type
	order ()
	{ return Order; }

	///  Returns the number of elements in the matrix. The size is the same as order () * order ().
	static
	constexpr size_type
	size ()
	{ return Size; }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix4 inplace.

	///  Returns the determinant of the 3x3 sub-matrix.
	Type
	determinant3 () const;

	///  Returns the determinant of this matrix.
	Type
	determinant () const;

	///  Transposes this matrx in place;
	void
	transpose ();

	///  Negates this matrix in place.
	void
	negate ();

	///  Inverses this matrix in place.
	void
	inverse ()
	throw (std::domain_error);

	///  Add @a matrix to this matrix.
	template <class T>
	matrix4 &
	operator += (const matrix4 <T> &);

	///  Add @a matrix to this matrix.
	template <class T>
	matrix4 &
	operator -= (const matrix4 <T> &);

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
	void
	mult_left (const matrix4 <T> &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	void
	mult_right (const matrix4 <T> &);

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	vector3 <Type>
	mult_vec_matrix (const vector3 <T> &) const;

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	constexpr
	vector4 <Type>
	mult_vec_matrix (const vector4 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	vector3 <Type>
	mult_matrix_vec (const vector3 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	constexpr
	vector4 <Type>
	mult_matrix_vec (const vector4 <T> &) const;

	///  Returns a new vector that is @vector (a normal or direction vector) multiplies by matrix.
	template <class T>
	constexpr
	vector3 <Type>
	mult_dir_matrix (const vector3 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector (a normal or direction vector).
	template <class T>
	constexpr
	vector3 <Type>
	mult_matrix_dir (const vector3 <T> &) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const vector3 <Type> &);

	///  Returns this matrix rotated by @a rotation.
	void
	rotate (const rotation4 <Type> &);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const vector3 <Type> &);


private:

	bool
	factor (vector3 <Type> & translation,
	        matrix3 <Type> & rotation,
	        vector3 <Type> & scale,
	        matrix3 <Type> & scaleOrientation) const;

	Type
	determinant3 (const int r1, const int r2, const int r3, const int c1, const int c2, const int c3) const;

	union
	{
		matrix_type value;
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
inline
matrix4 <Type> &
matrix4 <Type>::operator = (const matrix4 <Up> & matrix)
{
	value = matrix .vector ();
	return *this;
}

template <class Type>
inline
matrix4 <Type> &
matrix4 <Type>::operator = (const matrix4 <Type> & matrix)
{
	std::memmove (data (), matrix .data (), size () * sizeof (Type));
	return *this;
}

template <class Type>
inline
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
inline
void
matrix4 <Type>::get (vector3 <T> & translation) const
{
	translation = origin ();
}

template <class Type>
template <class T, class R>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation) const
{
	matrix3 <Type> so, rot;
	vector3 <Type> scaleFactor;
	factor (translation, rot, scaleFactor, so);
	rotation = rotation4 <R> (rot);
}

template <class Type>
template <class T, class R, class S>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation,
                     vector3 <S> & scaleFactor) const
{
	matrix3 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation = rotation4 <R> (rot);
}

template <class Type>
template <class T, class R, class S, class SO>
void
matrix4 <Type>::get (vector3 <T> & translation,
                     rotation4 <R> & rotation,
                     vector3 <S> & scaleFactor,
                     rotation4 <SO> & scaleOrientation) const
{
	matrix3 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation         = rotation4 <R> (rot);
	scaleOrientation = rotation4 <R> (so);
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
	m .mult_left (*this);
	c .set (center);
	m .mult_left (c);

	m .get (translation, rotation, scaleFactor, scaleOrientation);
}

template <class Type>
bool
matrix4 <Type>::factor (vector3 <Type> & translation,
                        matrix3 <Type> & rotation,
                        vector3 <Type> & scale,
                        matrix3 <Type> & scaleOrientation) const
{
	// (1) Get translation.
	translation = origin ();

	// (2) Create 3x3 matrix.
	const matrix3 <Type> a (*this);

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	const Type det      = a .determinant ();
	const Type det_sign = det < 0 ? -1 : 1;

	if (det_sign * det == 0)
		return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             // singular

	// (4) B = A * !A  (here !A means A transpose)
	const matrix3 <Type> b = a * ! a;

	Type evalues [3];
	Type evectors [3] [3];

	eigen_decomposition (b, evalues, evectors);

	// find min / max eigenvalues and do ratio test to determine singularity

	scaleOrientation = matrix3 <Type> (evectors [0] [0], evectors [0] [1], evectors [0] [2],
	                                   evectors [1] [0], evectors [1] [1], evectors [1] [2],
	                                   evectors [2] [0], evectors [2] [1], evectors [2] [2]);

	// Compute s = sqrt(evalues), with sign. Set si = s-inverse
	matrix3 <Type> si;

	for (size_t i = 0; i < 3; ++ i)
	{
		scale [i]  = det_sign * std::sqrt (evalues [i]);
		si [i] [i] = 1 / scale [i];
	}

	// (5) Compute U = !R ~S R A.
	rotation = scaleOrientation * si * ! scaleOrientation * a;

	scaleOrientation .transpose ();

	return true;
}

template <class Type>
inline
Type
matrix4 <Type>::determinant3 () const
{
	return determinant3 (0, 1, 2, 0, 1, 2);
}

template <class Type>
Type
matrix4 <Type>::determinant3 (const int r1, const int r2, const int r3, const int c1, const int c2, const int c3) const
{
	const Type a11 = value [r1] [c1];
	const Type a12 = value [r1] [c2];
	const Type a13 = value [r1] [c3];
	const Type a21 = value [r2] [c1];
	const Type a22 = value [r2] [c2];
	const Type a23 = value [r2] [c3];
	const Type a31 = value [r3] [c1];
	const Type a32 = value [r3] [c2];
	const Type a33 = value [r3] [c3];

	const Type M11 =   a22 * a33 - a32 * a23;
	const Type M21 = -(a12 * a33 - a32 * a13);
	const Type M31 =   a12 * a23 - a22 * a13;

	return (a11 * M11 + a21 * M21 + a31 * M31);
}

template <class Type>
Type
matrix4 <Type>::determinant () const
{
	return value [0] [3] * determinant3 (1, 2, 3, 0, 1, 2)
	       + value [1] [3] * determinant3 (0, 2, 3, 0, 1, 2)
	       + value [2] [3] * determinant3 (0, 1, 3, 0, 1, 2)
	       + value [3] [3] * determinant3 (0, 1, 2, 0, 1, 2);
}

template <class Type>
void
matrix4 <Type>::transpose ()
{
	*this = matrix4 <Type> (array [0], array [4], array [8],  array [12],
	                        array [1], array [5], array [9],  array [13],
	                        array [2], array [6], array [10], array [14],
	                        array [3], array [7], array [11], array [15]);
}

template <class Type>
void
matrix4 <Type>::negate ()
{
	*this = matrix4 <Type> (-array [ 0], -array [ 1], -array [ 2], -array [ 3],
	                        -array [ 4], -array [ 5], -array [ 6], -array [ 7],
	                        -array [ 8], -array [ 9], -array [10], -array [11],
	                        -array [12], -array [13], -array [14], -array [15]);
}

template <class Type>
void
matrix4 <Type>::inverse ()
throw (std::domain_error)
{
	const Type det = determinant ();

	if (det == 0)
		throw std::domain_error ("matrix4::inverse: determinant is 0.");

	*this = matrix4 <Type> (determinant3 (1, 2, 3, 1, 2, 3) / det,
	                        -determinant3 (0, 2, 3, 1, 2, 3) / det,
	                        determinant3 (0, 1, 3, 1, 2, 3) / det,
	                        -determinant3 (0, 1, 2, 1, 2, 3) / det,
	                        -determinant3 (1, 2, 3, 0, 2, 3) / det,
	                        determinant3 (0, 2, 3, 0, 2, 3) / det,
	                        -determinant3 (0, 1, 3, 0, 2, 3) / det,
	                        determinant3 (0, 1, 2, 0, 2, 3) / det,
	                        determinant3 (1, 2, 3, 0, 1, 3) / det,
	                        -determinant3 (0, 2, 3, 0, 1, 3) / det,
	                        determinant3 (0, 1, 3, 0, 1, 3) / det,
	                        -determinant3 (0, 1, 2, 0, 1, 3) / det,
	                        -determinant3 (1, 2, 3, 0, 1, 2) / det,
	                        determinant3 (0, 2, 3, 0, 1, 2) / det,
	                        -determinant3 (0, 1, 3, 0, 1, 2) / det,
	                        determinant3 (0, 1, 2, 0, 1, 2) / det);
}

template <class Type>
template <class T>
inline
matrix4 <Type> &
matrix4 <Type>::operator += (const matrix4 <T> & matrix)
{
	value += matrix .vector ();
	return *this;
}

template <class Type>
template <class T>
inline
matrix4 <Type> &
matrix4 <Type>::operator -= (const matrix4 <T> & matrix)
{
	value -= matrix .vector ();
	return *this;
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
inline
matrix4 <Type> &
matrix4 <Type>::operator *= (const matrix4 <T> & matrix)
{
	mult_right (matrix);
	return *this;
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
void
matrix4 <Type>::mult_left (const matrix4 <T> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (array [0 * 4 + j] * matrix .array [i * 4 + 0] +   \
	    array [1 * 4 + j] * matrix .array [i * 4 + 1] +   \
	    array [2 * 4 + j] * matrix .array [i * 4 + 2] +   \
	    array [3 * 4 + j] * matrix .array [i * 4 + 3])

	*this = matrix4 <Type> (MULT_LEFT (0, 0),
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
void
matrix4 <Type>::mult_right (const matrix4 <T> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (array [i * 4 + 0] * matrix .array [0 * 4 + j] +   \
	    array [i * 4 + 1] * matrix .array [1 * 4 + j] +   \
	    array [i * 4 + 2] * matrix .array [2 * 4 + j] +   \
	    array [i * 4 + 3] * matrix .array [3 * 4 + j])

	*this = matrix4 <Type> (MULT_RIGHT (0, 0),
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
matrix4 <Type>::mult_vec_matrix (const vector3 <T> & vector) const
{
	const Type w = vector .x () * array [3] + vector .y () * array [7] + vector .z () * array [11] + array [15];

	return vector3 <Type> ((vector .x () * array [0] + vector .y () * array [4] + vector .z () * array [ 8] + array [12]) / w,
	                       (vector .x () * array [1] + vector .y () * array [5] + vector .z () * array [ 9] + array [13]) / w,
	                       (vector .x () * array [2] + vector .y () * array [6] + vector .z () * array [10] + array [14]) / w);
}

/**
 * It takes 16 multiplies and 12 adds.
 */
template <class Type>
template <class T>
constexpr
vector4 <Type>
matrix4 <Type>::mult_vec_matrix (const vector4 <T> & vector) const
{
	return vector4 <Type> (vector .x () * array [0] + vector .y () * array [4] + vector .z () * array [ 8] + vector .w () * array [12],
	                       vector .x () * array [1] + vector .y () * array [5] + vector .z () * array [ 9] + vector .w () * array [13],
	                       vector .x () * array [2] + vector .y () * array [6] + vector .z () * array [10] + vector .w () * array [14],
	                       vector .x () * array [3] + vector .y () * array [7] + vector .z () * array [11] + vector .w () * array [15]);
}

/**
 * It takes 12 multiplies, 3 divs and 12 adds.
 */
template <class Type>
template <class T>
vector3 <Type>
matrix4 <Type>::mult_matrix_vec (const vector3 <T> & vector) const
{
	const Type w = vector .x () * array [12] + vector .y () * array [13] + vector .z () * array [14] + array [15];

	return vector3 <Type> ((vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [ 2] + array [ 3]) / w,
	                       (vector .x () * array [4] + vector .y () * array [5] + vector .z () * array [ 6] + array [ 7]) / w,
	                       (vector .x () * array [8] + vector .y () * array [9] + vector .z () * array [10] + array [11]) / w);
}

/**
 * It takes 16 multiplies and 12 adds.
 */
template <class Type>
template <class T>
constexpr
vector4 <Type>
matrix4 <Type>::mult_matrix_vec (const vector4 <T> & vector) const
{
	return vector4 <Type> (vector .x () * array [ 0] + vector .y () * array [ 1] + vector .z () * array [ 2] + vector .w () * array [ 3],
	                       vector .x () * array [ 4] + vector .y () * array [ 5] + vector .z () * array [ 6] + vector .w () * array [ 7],
	                       vector .x () * array [ 8] + vector .y () * array [ 9] + vector .z () * array [10] + vector .w () * array [11],
	                       vector .x () * array [12] + vector .y () * array [13] + vector .z () * array [14] + vector .w () * array [15]);
}

/**
 * It takes 9 multiplies and 6 adds.
 */
template <class Type>
template <class T>
constexpr
vector3 <Type>
matrix4 <Type>::mult_dir_matrix (const vector3 <T> & vector) const
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
constexpr
vector3 <Type>
matrix4 <Type>::mult_matrix_dir (const vector3 <T> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [ 2],
	                       vector .x () * array [4] + vector .y () * array [5] + vector .z () * array [ 6],
	                       vector .x () * array [8] + vector .y () * array [9] + vector .z () * array [10]);
}

/**
 * It takes 9 multiplies and 15 adds.
 */
template <class Type>
void
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
}

/**
 * Takes the same as a mult_left and a rotation (rot).
 */
template <class Type>
inline
void
matrix4 <Type>::rotate (const rotation4 <Type> & rotation)
{
	mult_left (matrix4 <Type> (rotation));
}

/**
 * It takes 9 multiplies.
 */
template <class Type>
void
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
}

///  @relates matrix4
///  @name Comparision operations

///  Compares two matrix4 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
inline
constexpr
bool
operator == (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .vector () == rhs .vector ();
}

///  Compares two matrix4 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .vector () not_eq rhs .vector ();
}

///  @relates matrix4
///  @name Arithmetic operations

///  Returns the determinant of the 3x3 submatrix of @a matrix.
template <class Type>
inline
Type
determinant3 (const matrix4 <Type> & matrix)
{
	return matrix .determinant3 ();
}

///  Returns the determinant of the @a matrix.
template <class Type>
inline
Type
determinant (const matrix4 <Type> & matrix)
{
	return matrix .determinant ();
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix4 <Type>
operator ! (const matrix4 <Type> & matrix)
{
	matrix4 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix4 <Type>
transpose (const matrix4 <Type> & matrix)
{
	matrix4 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix4 <Type>
operator - (const matrix4 <Type> & matrix)
{
	matrix4 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix4 <Type>
negate (const matrix4 <Type> & matrix)
{
	matrix4 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix4 <Type>
operator ~ (const matrix4 <Type> & matrix)
throw (std::domain_error)
{
	matrix4 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix4 <Type>
inverse (const matrix4 <Type> & matrix)
throw (std::domain_error)
{
	matrix4 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns new matrix value @a a plus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator + (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (lhs) += rhs;
}

///  Returns new matrix value @a a minus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator - (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (lhs) -= rhs;
}

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix4 <Type>
operator * (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	matrix4 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return matrix value @a lhs right multiplied by scalar @a rhs.
template <class Type>
inline
matrix4 <Type>
operator * (const matrix4 <Type> & lhs, const Type & rhs)
{
	return matrix4 <Type> (lhs) *= rhs;
}

///  Return matrix value @a lhs right multiplied by scalar @a rhs.
template <class Type>
inline
matrix4 <Type>
operator * (const Type & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (rhs) *= lhs;
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const matrix4 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .mult_matrix_vec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector4 <Type>
operator * (const matrix4 <Type> & lhs, const vector4 <Type> & rhs)
{
	return lhs .mult_matrix_vec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return rhs .mult_vec_matrix (lhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector4 <Type>
operator * (const vector4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return rhs .mult_vec_matrix (lhs);
}

///  Return column vector @a rhs multiplied by row vector @a lhs.
template <class Type>
matrix4 <Type>
multiply (const vector4 <Type> & lhs, const vector4 <Type> & rhs)
{
	matrix4 <Type> result;

	result [0] = lhs [0] * rhs;
	result [1] = lhs [1] * rhs;
	result [2] = lhs [2] * rhs;
	result [3] = lhs [3] * rhs;

	return result;
}

///  Return scalar value @a lhs  divided by matrix @a rhs.
template <class Type>
inline
matrix4 <Type>
operator / (const Type & lhs, const matrix4 <Type> & rhs)
{
	matrix4 <Type> result (rhs);

	result [0] = lhs / result [0];
	result [1] = lhs / result [1];
	result [2] = lhs / result [2];
	result [3] = lhs / result [3];

	return result;
}

///  Return matrix value @a lhs divided by scalar @a rhs.
template <class Type>
inline
matrix4 <Type>
operator / (const matrix4 <Type> & lhs, const Type & rhs)
{
	return matrix4 <Type> (lhs) /= rhs;
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
