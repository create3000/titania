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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX2_H__
#define __TITANIA_MATH_NUMBERS_MATRIX2_H__

#include <cfloat>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <ostream>
#include <stdexcept>

#include "Vector2.h"

#include "../Algorithms/EigenDecomposition.h"

namespace titania {
namespace math {

/**
 *  Template to represent square matrix of order 2.
 *
 *  Matrix2 matrices are organized in row-major fashion.  The first row of the
 *  matrix stores information for the x dimension.  Since these data types are
 *  commonly used for transformation matrices, translation values are stored in
 *  the second row.
 *
 *  0 [ x-axis 0 ]
 *  1 [ x      1 ]
 *
 *  The default value of a matrix2 number is the identity matrix [1 0 0 1].
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values.
 */
template <class Type>
class matrix2
{
private:

	static constexpr size_t Order = 2;
	static constexpr size_t Size  = Order * Order;


public:

	///  Size typedef.  Used for size and indices.
	using size_type = size_t;

	///  Value typedef.
	using value_type = Type;

	///  Array typedef.
	typedef Type array_type [Size];

	///  Matrix typedef.
	using matrix_type = vector2 <vector2 <Type>>;

	///  Translation typedef.
	using translation_type = Type;

	///  Translation typedef.
	using scale_type = Type;

	///  Vector typedef.
	using vector_type = vector2 <Type>;

	///  Point typedef.
	using point_type = Type;

	///  Normal typedef.
	using normal_type = Type;

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	matrix2 () :
		matrix2 (Identity)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix2 (const matrix2 <Up> & matrix) :
		value (matrix [0], matrix [1])
	{ }

	///  Copy constructor.
	matrix2 (const matrix2 & matrix)
	{ *this = matrix; }

	///  Value constructor.
	explicit
	constexpr
	matrix2 (const array_type & matrix) :
		array
	{
		matrix [0], matrix [1],
		matrix [2], matrix [3],
	}

	{ }

	///  Components constructor. Set values from @a e11 to @a e33.
	constexpr
	matrix2 (const Type & e11, const Type & e12,
	         const Type & e21, const Type & e22) :
		array
	{
		e11, e12,
		e21, e22,
	}

	{ }

	///  @name Assignment operators

	template <class T>
	matrix2 &
	operator = (const matrix2 <T> &);

	matrix2 &
	operator = (const matrix2 &);

	///  @name Element access

	const Type &
	x () const
	{ return array [0]; }

	const Type &
	origin () const
	{ return array [2]; }

	void
	set ();

	void
	set (const Type &);

	void
	set (const Type &, const Type &);

	void
	get (Type &) const;

	void
	get (Type &, Type &) const;

	///  Access rows by @a index.
	vector_type &
	operator [ ] (const size_type index)
	{ return value [index]; }

	const vector_type &
	operator [ ] (const size_type index) const
	{ return value [index]; }

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
	constexpr
	size_type
	order ()
	{ return Order; }

	///  Returns the number of elements in the matrix. The size is the same as order () * order ().
	static
	constexpr
	size_type
	size ()
	{ return Size; }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix4 inplace.

	///  Returns the determinant of the 2x2 sub-matrix.
	constexpr
	Type
	determinant1 () const;

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
	template <class T>
	matrix2 &
	operator += (const matrix2 <T> &);

	///  Add @a matrix to this matrix.
	template <class T>
	matrix2 &
	operator -= (const matrix2 <T> &);

	///  Returns this matrix multiplies by @a scalar.
	matrix2 &
	operator *= (const Type &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	matrix2 &
	operator *= (const matrix2 <T> &);

	///  Returns this matrix divided by @a scalar.
	matrix2 &
	operator /= (const Type &);

	///  Returns this matrix left multiplied by @a matrix.
	template <class T>
	void
	mult_left (const matrix2 <T> &);

	///  Returns this matrix right multiplied by @a matrix.
	template <class T>
	void
	mult_right (const matrix2 <T> &);

	///  Returns a new vector that is @vector multiplies by matrix.
	Type
	mult_vec_matrix (const Type &) const;

	///  Returns a new vector that is @vector multiplies by matrix.
	template <class T>
	constexpr
	vector2 <Type>
	mult_vec_matrix (const vector2 <T> &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	Type
	mult_matrix_vec (const Type &) const;

	///  Returns a new vector that is matrix multiplies by @vector.
	template <class T>
	constexpr
	vector2 <Type>
	mult_matrix_vec (const vector2 <T> &) const;

	///  Returns a new vector that is @vector (a normal or direction vector) multiplies by matrix.
	constexpr
	Type
	mult_dir_matrix (const Type &) const;

	///  Returns a new vector that is matrix multiplies by @vector (a normal or direction vector).
	constexpr
	Type
	mult_matrix_dir (const Type &) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const Type &);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const Type &);


private:

	union
	{
		matrix_type value;
		array_type array;
	};

	static const matrix2 Identity;

};

template <class Type>
const matrix2 <Type> matrix2 <Type>::Identity = { 1, 0,
	                                               0, 1 };

template <class Type>
template <class Up>
inline
matrix2 <Type> &
matrix2 <Type>::operator = (const matrix2 <Up> & matrix)
{
	value = matrix .vector ();
	return *this;
}

template <class Type>
inline
matrix2 <Type> &
matrix2 <Type>::operator = (const matrix2 <Type> & matrix)
{
	std::memmove (data (), matrix .data (), size () * sizeof (Type));
	return *this;
}

template <class Type>
inline
void
matrix2 <Type>::set ()
{
	value = Identity .value;
}

template <class Type>
void
matrix2 <Type>::set (const Type & translation)
{
	value [0] = vector2 <Type> (1, 0);
	value [1] = vector2 <Type> (translation, 1);
}

template <class Type>
void
matrix2 <Type>::set (const Type & translation,
                     const Type & scaleFactor)
{
	set (translation);

	if (scaleFactor not_eq 1)
		scale (scaleFactor);
}

template <class Type>
inline
void
matrix2 <Type>::get (Type & translation) const
{
	translation = origin ();
}

template <class Type>
void
matrix2 <Type>::get (Type & translation,
                     Type & scaleFactor) const
{
	translation = origin ();
	scaleFactor = array [0];
}

template <class Type>
inline
constexpr
Type
matrix2 <Type>::determinant1 () const
{
	return array [0];
}

template <class Type>
constexpr
Type
matrix2 <Type>::determinant () const
{
	return
	   array [0] * array [3] -
	   array [1] * array [2];
}

template <class Type>
void
matrix2 <Type>::transpose ()
{
	*this = matrix2 <Type> (array [0], array [2],
	                        array [1], array [3]);
}

template <class Type>
void
matrix2 <Type>::negate ()
{
	*this = matrix2 <Type> (-array [0], -array [1],
	                        -array [2], -array [3]);
}

template <class Type>
void
matrix2 <Type>::inverse ()
throw (std::domain_error)
{
	const Type d = determinant ();

	if (d == 0)
		throw std::domain_error ("matrix2::inverse: determinant is 0.");

	*this = matrix2 <Type> (array [0] / d, -array [1] / d,
	                        -array [2] / d, array [3] / d);
}

template <class Type>
template <class T>
inline
matrix2 <Type> &
matrix2 <Type>::operator += (const matrix2 <T> & matrix)
{
	value += matrix .vector ();
	return *this;
}

template <class Type>
template <class T>
inline
matrix2 <Type> &
matrix2 <Type>::operator -= (const matrix2 <T> & matrix)
{
	value -= matrix .vector ();
	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;

	return *this;
}

template <class Type>
template <class T>
inline
matrix2 <Type> &
matrix2 <Type>::operator *= (const matrix2 <T> & matrix)
{
	mult_right (matrix);
	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;

	return *this;
}

template <class Type>
template <class T>
void
matrix2 <Type>::mult_left (const matrix2 <T> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (array [0 * 2 + j] * matrix .array [i * 2 + 0] +   \
	    array [1 * 2 + j] * matrix .array [i * 2 + 1])

	*this = matrix2 <Type> (MULT_LEFT (0, 0),
	                        MULT_LEFT (0, 1),

	                        MULT_LEFT (1, 0),
	                        MULT_LEFT (1, 1));

	#undef MULT_LEFT
}

template <class Type>
template <class T>
void
matrix2 <Type>::mult_right (const matrix2 <T> & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (array [i * 2 + 0] * matrix .array [0 * 2 + j] +   \
	    array [i * 2 + 1] * matrix .array [1 * 2 + j])

	*this = matrix2 <Type> (MULT_RIGHT (0, 0),
	                        MULT_RIGHT (0, 1),

	                        MULT_RIGHT (1, 0),
	                        MULT_RIGHT (1, 1));

	#undef MULT_RIGHT
}

template <class Type>
Type
matrix2 <Type>::mult_vec_matrix (const Type & vector) const
{
	const Type w = vector * array [1] + array [3];

	return (vector * array [0] + array [2]) / w;
}

template <class Type>
template <class T>
constexpr
vector2 <Type>
matrix2 <Type>::mult_vec_matrix (const vector2 <T> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [2],
	                       vector .x () * array [1] + vector .y () * array [3]);
}

template <class Type>
Type
matrix2 <Type>::mult_matrix_vec (const Type & vector) const
{
	const Type w = vector * array [2] + array [3];

	return (vector * array [0] + array [1]) / w;
}

template <class Type>
template <class T>
constexpr
vector2 <Type>
matrix2 <Type>::mult_matrix_vec (const vector2 <T> & vector) const
{
	return vector2 <Type> (vector .x () * array [0] + vector .y () * array [1],
	                       vector .x () * array [2] + vector .y () * array [3]);
}

template <class Type>
constexpr
Type
matrix2 <Type>::mult_dir_matrix (const Type & vector) const
{
	return vector * array [0];
}

template <class Type>
constexpr
Type
matrix2 <Type>::mult_matrix_dir (const Type & vector) const
{
	return vector * array [0];
}

template <class Type>
void
matrix2 <Type>::translate (const Type & translation)
{
	#define TRANSLATE(i) \
	   (value [0] [i] * translation)

	value [1] [0] += TRANSLATE (0);

	#undef TRANSLATE
}

template <class Type>
void
matrix2 <Type>::scale (const Type & scaleFactor)
{
	value [0] [0] *= scaleFactor;
}

///  @relates matrix2
///  @name Comparision operations

///  Compares two matrix2 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
inline
constexpr
bool
operator == (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return lhs .vector () == rhs .vector ();
}

///  Compares two matrix2 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return lhs .vector () not_eq rhs .vector ();
}

///  @relates matrix2
///  @name Arithmetic operations

///  Returns the determinant of the 1x1 submatrix of @a matrix.
template <class Type>
inline
constexpr
Type
determinant1 (const matrix2 <Type> & matrix)
{
	return matrix .determinant1 ();
}

///  Returns the determinant of the @a matrix.
template <class Type>
inline
constexpr
Type
determinant (const matrix2 <Type> & matrix)
{
	return matrix .determinant ();
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix2 <Type>
operator ! (const matrix2 <Type> & matrix)
{
	matrix2 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns the transpose of the @a matrix.
template <class Type>
inline
matrix2 <Type>
transpose (const matrix2 <Type> & matrix)
{
	matrix2 <Type> result (matrix);
	result .transpose ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix2 <Type>
operator - (const matrix2 <Type> & matrix)
{
	matrix2 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns matrix negation of @a matrix.
template <class Type>
inline
matrix2 <Type>
negate (const matrix2 <Type> & matrix)
{
	matrix2 <Type> result (matrix);
	result .negate ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix2 <Type>
operator ~ (const matrix2 <Type> & matrix)
throw (std::domain_error)
{
	matrix2 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns the inverse of the @a matrix.
template <class Type>
inline
matrix2 <Type>
inverse (const matrix2 <Type> & matrix)
throw (std::domain_error)
{
	matrix2 <Type> result (matrix);
	result .inverse ();
	return result;
}

///  Returns new matrix value @a a plus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator + (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (lhs) += rhs;
}

///  Returns new matrix value @a a minus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator - (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (lhs) -= rhs;
}

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix2 <Type>
operator * (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	matrix2 <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Return matrix value @a lhs multiplied by scalar @a rhs.
template <class Type>
inline
matrix2 <Type>
operator * (const matrix2 <Type> & lhs, const Type & rhs)
{
	return matrix2 <Type> (lhs) *= rhs;
}

///  Return matrix value @a lhs multiplied by scalar @a rhs.
template <class Type>
inline
matrix2 <Type>
operator * (const Type & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (rhs) *= lhs;
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const matrix2 <Type> & lhs, const vector2 <Type> & rhs)
{
	matrix2 <Type> result (lhs);
	result *= rhs;
	return result;
}

///  Return vector value @a rhs multiplied by @a lhs.
template <class Type>
inline
vector2 <Type>
operator * (const vector2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return rhs .mult_vec_matrix (lhs);
}

///  Return column vector @a rhs multiplied by row vector @a lhs.
template <class Type>
matrix2 <Type>
multiply (const vector2 <Type> & lhs, const vector2 <Type> & rhs)
{
	matrix2 <Type> result;

	result [0] = lhs [0] * rhs;
	result [1] = lhs [1] * rhs;

	return result;
}

///  Return scalar value @a lhs divided by matrix @a rhs.
template <class Type>
inline
matrix2 <Type>
operator / (const Type & lhs, const matrix2 <Type> & rhs)
{
	matrix2 <Type> result (rhs);

	result [0] = lhs / result [0];
	result [1] = lhs / result [1];

	return result;
}

///  Return matrix value @a lhs divided by scalar @a rhs.
template <class Type>
inline
matrix2 <Type>
operator / (const matrix2 <Type> & lhs, const Type & rhs)
{
	return matrix2 <Type> (lhs) /= rhs;
}

///  @relates matrix2
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, matrix2 <Type> & matrix)
{
	Type
	   e11, e12,
	   e21, e22;

	istream
		>> e11
		>> e12
		>> e21
		>> e22;

	if (istream)
		matrix = matrix2 <Type> (e11, e12,
		                         e21, e22);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const matrix2 <Type> & matrix)
{
	return ostream << matrix .vector ();
}

extern template class matrix2 <float>;
extern template class matrix2 <double>;
extern template class matrix2 <long double>;

//
extern template std::istream & operator >> (std::istream &, matrix2 <float> &);
extern template std::istream & operator >> (std::istream &, matrix2 <double> &);
extern template std::istream & operator >> (std::istream &, matrix2 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const matrix2 <float> &);
extern template std::ostream & operator << (std::ostream &, const matrix2 <double> &);
extern template std::ostream & operator << (std::ostream &, const matrix2 <long double> &);

} // math
} // titania

#endif
