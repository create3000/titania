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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX3_H__
#define __TITANIA_MATH_NUMBERS_MATRIX3_H__

#include <cfloat>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "Matrix2.h"
#include "Vector2.h"
#include "Vector3.h"

#include "../Algorithms/EigenDecomposition.h"

namespace titania {
namespace math {

/**
 *  Template to represent square matrix of order 3.
 *
 *  Matrix3 matrices are organized in row-major fashion.  The first row of the
 *  matrix stores information for the x dimension, and the second for the
 *  y dimension.  Since these data types are commonly used for transformation
 *  matrices, translation values are stored in the third row.
 *
 *  0 [ x-axis 0 ]
 *  1 [ y-axis 0 ]
 *  2 [ x y    1 ]
 *
 *  The default value of a matrix3 number is the identity matrix [1 0 0 0 1 0 0 0 1].
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values.
 */
template <class Type>
class matrix3
{
private:

	static constexpr size_t ORDER = 3;
	static constexpr size_t SIZE  = ORDER * ORDER;


public:

	///  @name Member types

	///  Value typedef.
	using value_type = Type;

	///  Size typedef.  Used for size and indices.
	using size_type = size_t;

	///  Array typedef.
	using array_type = Type [SIZE];

	///  Matrix typedef.
	using matrix_type = vector3 <vector3 <Type>>;

	///  Translation typedef.
	using translation_type = vector2 <Type>;

	///  Rotation typedef.
	using rotation_type = Type;

	///  Translation typedef.
	using scale_type = vector2 <Type>;

	///  Vector typedef.
	using vector_type = vector3 <Type>;

	///  Point typedef.
	using point_type = vector2 <Type>;

	///  Normal typedef.
	using normal_type = vector2 <Type>;

	///  Random access iterator
	using iterator = Type*;

	///  Constant random access iterator 
	using const_iterator = const Type*;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = std::reverse_iterator <iterator>;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = std::reverse_iterator <const_iterator>;

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	constexpr
	matrix3 () :
		array
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	}

	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix3 (const matrix3 <Up> & matrix) :
		value (matrix [0], matrix [1], matrix [2])
	{ }

	///  Copy constructor.
	matrix3 (const matrix3 & matrix)
	{ *this = matrix; }

	///  Value constructor.
	explicit
	constexpr
	matrix3 (const array_type & matrix) :
		array
	{
		matrix [ 0], matrix [ 1], matrix [ 2],
		matrix [ 3], matrix [ 4], matrix [ 5],
		matrix [ 6], matrix [ 7], matrix [ 8]
	}

	{ }

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
	}

	{ }

	///  Constructs a matrix4 from a rotation4.
	explicit
	matrix3 (const Type & rot)
	{ rotation (rot); }

	///  Constructs a matrix2 from a matrix2 rotation matrix.
	explicit
	constexpr
	matrix3 (const matrix2 <Type> & matrix) :
		array
	{
		matrix [0] [0],
		matrix [0] [1],
		0,
		matrix [1] [0],
		matrix [1] [1],
		0,
		0, 0, 1,
	}

	{ }

	///  @name Assignment operators

	template <class T>
	matrix3 &
	operator = (const matrix3 <T> &);

	matrix3 &
	operator = (const matrix3 &);

	///  @name Element access

	constexpr
	vector2 <Type>
	x () const
	{ return vector2 <Type> (array [0], array [1]); }

	constexpr
	vector2 <Type>
	y () const
	{ return vector2 <Type> (array [3], array [4]); }

	constexpr
	vector2 <Type>
	origin () const
	{ return vector2 <Type> (array [6], array [7]); }

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
	vector_type &
	operator [ ] (const size_type index)
	{ return value [index]; }

	const vector_type &
	operator [ ] (const size_type index) const
	{ return value [index]; }

	constexpr
	operator matrix2 <Type> () const
	{ return matrix2 <Type> (array [0], array [1],
		                      array [3], array [4]); }

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

	///  Returns the order of the matrix.
	static
	constexpr size_type
	order ()
	{ return ORDER; }

	///  Returns the number of elements in the matrix.
	static
	constexpr size_type
	size ()
	{ return SIZE; }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix4 inplace.

	///  Returns the determinant of the 2x2 sub-matrix.
	constexpr
	Type
	determinant2 () const;

	///  Returns the determinant of this matrix.
	constexpr
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
	matrix3 &
	operator += (const matrix3 <Type> &);

	///  Add @a matrix to this matrix.
	matrix3 &
	operator -= (const matrix3 <Type> &);

	///  Returns this matrix multiplies by @a scalar.
	matrix3 &
	operator *= (const Type &);

	///  Returns this matrix right multiplied by @a matrix.
	matrix3 &
	operator *= (const matrix3 &);

	///  Returns this matrix divided by @a scalar.
	matrix3 &
	operator /= (const Type &);

	///  Returns this matrix left multiplied by @a matrix.
	void
	mult_left (const matrix3 &);

	///  Returns this matrix right multiplied by @a matrix.
	void
	mult_right (const matrix3 &);

	///  Returns a new vector that is @vector multiplies by matrix.
	vector2 <Type>
	mult_vec_matrix (const vector2 <Type> &) const;

	///  Returns a new vector that is @vector multiplies by matrix.
	constexpr
	vector3 <Type>
	mult_vec_matrix (const vector3 <Type> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	vector2 <Type>
	mult_matrix_vec (const vector2 <Type> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	constexpr
	vector3 <Type>
	mult_matrix_vec (const vector3 <Type> &) const;

	///  Returns a new vector that is @vector (a normal or direction vector) multiplies by matrix.
	constexpr
	vector2 <Type>
	mult_dir_matrix (const vector2 <Type> &) const;

	///  Returns a new vector that is matrix multiplies by @vector (a normal or direction vector).
	constexpr
	vector2 <Type>
	mult_matrix_dir (const vector2 <Type> &) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const vector2 <Type> &);

	///  Returns this matrix rotated by @a rotation.
	void
	rotate (const Type &);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const vector2 <Type> &);

	///  Returns this matrix transformed by @a skewAngle.
	void
	skew_x (const Type &);

	///  Returns this matrix transformed by @a skewAngle.
	void
	skew_y (const Type &);


private:

	void
	rotation (const Type &);

	Type
	rotation () const;

	template <class T, class S>
	bool
	factor (vector2 <T> &,
	        matrix2 <Type> &,
	        vector2 <S> &,
	        matrix2 <Type> &) const;

	union
	{
		matrix_type value;
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
inline
matrix3 <Type> &
matrix3 <Type>::operator = (const matrix3 <Type> & matrix)
{
	std::memmove (data (), matrix .data (), size () * sizeof (Type));
	return *this;
}

template <class Type>
void
matrix3 <Type>::rotation (const Type & rotation)
{
	const Type sinAngle = std::sin (rotation);
	const Type cosAngle = std::cos (rotation);

	array [0] = cosAngle;
	array [1] = sinAngle;
	array [2] = 0;

	array [3] = -sinAngle;
	array [4] = cosAngle;
	array [5] = 0;

	array [6] = 0;
	array [7] = 0;
	array [8] = 1;
}

template <class Type>
Type
matrix3 <Type>::rotation () const
{
	return std::atan2 (array [1], array [0]);
}

template <class Type>
inline
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
		{
			rotate (scaleOrientation);
			scale (scaleFactor);
			rotate (-scaleOrientation);
		}
		else
			scale (scaleFactor);
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
		{
			rotate (scaleOrientation);
			scale (scaleFactor);
			rotate (-scaleOrientation);
		}
		else
			scale (scaleFactor);
	}

	if (center not_eq vector2 <Type> ())
		translate (-center);
}

template <class Type>
inline
void
matrix3 <Type>::get (vector2 <Type> & translation) const
{
	translation = origin ();
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation) const
{
	matrix2 <Type> so, rot;
	vector2 <Type> scaleFactor;
	factor (translation, rot, scaleFactor, so);
	rotation = std::atan2 (rot [0] [1], rot [0] [0]);
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor) const
{
	matrix2 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation = std::atan2 (rot [0] [1], rot [0] [0]);
}

template <class Type>
void
matrix3 <Type>::get (vector2 <Type> & translation,
                     Type & rotation,
                     vector2 <Type> & scaleFactor,
                     Type & scaleOrientation) const
{
	matrix2 <Type> so, rot;
	factor (translation, rot, scaleFactor, so);
	rotation         = std::atan2 (rot [0] [1], rot [0] [0]);
	scaleOrientation = std::atan2 (so [0] [1], so [0] [0]);
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
	m .mult_left (*this);
	c .set (center);
	m .mult_left (c);

	m .get (translation, rotation, scaleFactor, scaleOrientation);
}

template <class Type>
template <class T, class S>
bool
matrix3 <Type>::factor (vector2 <T> & translation,
                        matrix2 <Type> & rotation,
                        vector2 <S> & scale,
                        matrix2 <Type> & scaleOrientation) const
{
	// (1) Get translation.
	translation = origin ();

	// (2) Create 2x2 matrix.
	const matrix2 <Type> a (*this);

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	const Type det      = a .determinant ();
	const Type det_sign = det < 0 ? -1 : 1;

	if (det_sign * det == 0)
		return false;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            // singular

	// (4) B = A * !A  (here !A means A transpose)
	const matrix2 <Type> b = a * ! a;

	Type evalues [2];
	Type evectors [2] [2];

	eigen_decomposition (b, evalues, evectors);

	// find min / max eigenvalues and do ratio test to determine singularity

	scaleOrientation = matrix2 <Type> (evectors [0] [0], evectors [0] [1],
	                                   evectors [1] [0], evectors [1] [1]);

	// Compute s = sqrt(evalues), with sign. Set si = s-inverse
	matrix2 <Type> si;

	for (size_t i = 0; i < 2; ++ i)
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
constexpr
Type
matrix3 <Type>::determinant2 () const
{
	return
	   array [0] * array [4] -
	   array [1] * array [3];
}

template <class Type>
constexpr
Type
matrix3 <Type>::determinant () const
{
	return array [0] * (array [4] * array [8] - array [5] * array [7]) -
	       array [1] * (array [3] * array [8] - array [5] * array [6]) +
	       array [2] * (array [3] * array [7] - array [4] * array [6]);
}

template <class Type>
void
matrix3 <Type>::transpose ()
{
	*this = matrix3 <Type> (array [0], array [3], array [6],
	                        array [1], array [4], array [7],
	                        array [2], array [5], array [8]);
}

template <class Type>
void
matrix3 <Type>::negate ()
{
	*this = matrix3 <Type> (-array [0], -array [1], -array [2],
	                        -array [3], -array [4], -array [5],
	                        -array [6], -array [7], -array [8]);
}

template <class Type>
void
matrix3 <Type>::inverse ()
throw (std::domain_error)
{
	const Type m00 = array [0];
	const Type m01 = array [1];
	const Type m02 = array [2];

	const Type m10 = array [3];
	const Type m11 = array [4];
	const Type m12 = array [5];

	const Type m20 = array [6];
	const Type m21 = array [7];
	const Type m22 = array [8];

	const Type t4  = m00 * m11;
	const Type t6  = m00 * m21;
	const Type t8  = m10 * m01;
	const Type t10 = m10 * m21;
	const Type t12 = m20 * m01;
	const Type t14 = m20 * m11;

	const Type d = (t4 * m22 - t6 * m12 - t8 * m22 + t10 * m02 + t12 * m12 - t14 * m02);

	if (d == 0)
		throw std::domain_error ("matrix3::inverse: determinant is 0.");

	*this = matrix3 <Type> ((m11 * m22 - m21 * m12) / d,
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
inline
matrix3 <Type> &
matrix3 <Type>::operator += (const matrix3 & matrix)
{
	value += matrix .vector ();
	return *this;
}

template <class Type>
inline
matrix3 <Type> &
matrix3 <Type>::operator -= (const matrix3 & matrix)
{
	value -= matrix .vector ();
	return *this;
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
inline
matrix3 <Type> &
matrix3 <Type>::operator *= (const matrix3 & matrix)
{
	mult_right (matrix);
	return *this;
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
void
matrix3 <Type>::mult_left (const matrix3 <Type> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (array [0 * 3 + j] * matrix .array [i * 3 + 0] +   \
	    array [1 * 3 + j] * matrix .array [i * 3 + 1] +   \
	    array [2 * 3 + j] * matrix .array [i * 3 + 2])

	*this = matrix3 <Type> (MULT_LEFT (0, 0),
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
void
matrix3 <Type>::mult_right (const matrix3 <Type> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (array [i * 3 + 0] * matrix .array [0 * 3 + j] +   \
	    array [i * 3 + 1] * matrix .array [1 * 3 + j] +   \
	    array [i * 3 + 2] * matrix .array [2 * 3 + j])

	*this = matrix3 <Type> (MULT_RIGHT (0, 0),
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
vector2 <Type>
matrix3 <Type>::mult_vec_matrix (const vector2 <Type> & vector) const
{
	const Type w = vector .x () * array [2] + vector .y () * array [5] + array [8];

	return vector2 <Type> ((vector .x () * array [0] + vector .y () * array [3] + array [6]) / w,
	                       (vector .x () * array [1] + vector .y () * array [4] + array [7]) / w);
}

template <class Type>
constexpr
vector3 <Type>
matrix3 <Type>::mult_vec_matrix (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [3] + vector .z () * array [6],
	                       vector .x () * array [1] + vector .y () * array [4] + vector .z () * array [7],
	                       vector .x () * array [2] + vector .y () * array [5] + vector .z () * array [8]);
}

template <class Type>
vector2 <Type>
matrix3 <Type>::mult_matrix_vec (const vector2 <Type> & vector) const
{
	const Type w = vector .x () * array [6] + vector .y () * array [7] + array [8];

	return vector2 <Type> ((vector .x () * array [0] + vector .y () * array [1] + array [2]) / w,
	                       (vector .x () * array [3] + vector .y () * array [4] + array [5]) / w);
}

template <class Type>
constexpr
vector3 <Type>
matrix3 <Type>::mult_matrix_vec (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * array [0] + vector .y () * array [1] + vector .z () * array [2],
	                       vector .x () * array [3] + vector .y () * array [4] + vector .z () * array [5],
	                       vector .x () * array [6] + vector .y () * array [7] + vector .z () * array [8]);
}

template <class Type>
constexpr
vector2 <Type>
matrix3 <Type>::mult_dir_matrix (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [3],
	                       vector .x () * array [1] + vector .y () * array [4]);
}

template <class Type>
constexpr
vector2 <Type>
matrix3 <Type>::mult_matrix_dir (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [1],
	                       vector .x () * array [3] + vector .y () * array [4]);
}

template <class Type>
void
matrix3 <Type>::translate (const vector2 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (value [0] [i] * translation .x () +   \
	    value [1] [i] * translation .y ())

	value [2] [0] += TRANSLATE (0);
	value [2] [1] += TRANSLATE (1);

	#undef TRANSLATE
}

template <class Type>
void
matrix3 <Type>::rotate (const Type & rotation)
{
	mult_left (matrix3 <Type> (rotation));
}

template <class Type>
void
matrix3 <Type>::scale (const vector2 <Type> & scaleFactor)
{
	value [0] [0] *= scaleFactor .x ();
	value [1] [0] *= scaleFactor .y ();

	value [0] [1] *= scaleFactor .x ();
	value [1] [1] *= scaleFactor .y ();
}

template <class Type>
void
matrix3 <Type>::skew_x (const Type & skewAngle)
{
	mult_left (matrix3 (1, std::tan (skewAngle), 0, 0, 1, 0, 0, 0, 1));
}

template <class Type>
void
matrix3 <Type>::skew_y (const Type & skewAngle)
{
	mult_left (matrix3 (1, 0, 0, std::tan (skewAngle), 1, 0, 0, 0, 1));
}

///  @relates matrix3
///  @name Comparision operations

///  Compares two matrix3 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
inline
constexpr
bool
operator == (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .vector () == rhs .vector ();
}

///  Compares two matrix3 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .vector () not_eq rhs .vector ();
}

///  Lexicographically compares two matrix3 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .vector () < rhs .vector ();;
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates matrix3
///  @name Arithmetic operations

///  Returns the determinant of the 2x2 submatrix of @a matrix.
template <class Type>
inline
constexpr
Type
determinant2 (const matrix3 <Type> & matrix)
{
	return matrix .determinant2 ();
}

///  Returns the determinant of the @a matrix.
template <class Type>
inline
constexpr
Type
determinant (const matrix3 <Type> & matrix)
{
	return matrix .determinant ();
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix3 <Type>
operator ! (const matrix3 <Type> & matrix)
{
	matrix3 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix3 <Type>
transpose (const matrix3 <Type> & matrix)
{
	matrix3 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix3 <Type>
operator - (const matrix3 <Type> & matrix)
{
	matrix3 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix3 <Type>
negate (const matrix3 <Type> & matrix)
{
	matrix3 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix3 <Type>
operator ~ (const matrix3 <Type> & matrix)
throw (std::domain_error)
{
	matrix3 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix3 <Type>
inverse (const matrix3 <Type> & matrix)
throw (std::domain_error)
{
	matrix3 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns new matrix value @a a plus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator + (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (lhs) += rhs;
}

///  Returns new matrix value @a a minus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator - (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (lhs) -= rhs;
}

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	matrix3 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return matrix value @a lhs multiplied by scalar @a rhs.
template <class Type>
inline
matrix3 <Type>
operator * (const matrix3 <Type> & lhs, const Type & rhs)
{
	return matrix3 <Type> (lhs) *= rhs;
}

///  Return matrix value @a lhs multiplied by scalar @a rhs.
template <class Type>
inline
matrix3 <Type>
operator * (const Type & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (rhs) *= lhs;
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const matrix3 <Type> & lhs, const vector2 <Type> & rhs)
{
	return lhs .mult_matrix_vec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const matrix3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .mult_matrix_vec (rhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const vector2 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs .mult_vec_matrix (lhs);
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs .mult_vec_matrix (lhs);
}

///  Return column vector @a rhs multiplied by row vector @a lhs.
template <class Type>
matrix3 <Type>
multiply (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	matrix3 <Type> result;

	result [0] = lhs [0] * rhs;
	result [1] = lhs [1] * rhs;
	result [2] = lhs [2] * rhs;

	return result;
}

///  Return scalar value @a lhs  divided by matrix @a rhs.
template <class Type>
inline
matrix3 <Type>
operator / (const Type & lhs, const matrix3 <Type> & rhs)
{
	matrix3 <Type> result (rhs);

	result [0] = lhs / result [0];
	result [1] = lhs / result [1];
	result [2] = lhs / result [2];

	return result;
}

///  Return matrix value @a lhs divided by scalar @a rhs.
template <class Type>
inline
matrix3 <Type>
operator / (const matrix3 <Type> & lhs, const Type & rhs)
{
	return matrix3 <Type> (lhs) /= rhs;
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
