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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX2_H__
#define __TITANIA_MATH_NUMBERS_MATRIX2_H__

#include <cfloat>
#include <cmath>
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

	static constexpr size_t ORDER = 2;
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
	matrix2 () :
		matrix2 (1, 0,
		         0, 1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix2 (const matrix2 <Up> & other) :
		m_value (other [0], other [1])
	{ }

	///  Vector constructor.
	template <class Up>
	constexpr
	matrix2 (const vector2 <Up> & x, const vector2 <Up> & y) :
		m_value (x, y)
	{ }

	///  Components constructor. Set values from @a e11 to @a e33.
	constexpr
	matrix2 (const Type & e11, const Type & e12,
	         const Type & e21, const Type & e22) :
		m_array
	{
		e11, e12,
		e21, e22,
	}

	{ }

	///  Components constructor. Set values to @a v.
	explicit
	constexpr
	matrix2 (const Type & v) :
		m_array
	{
		v, v,
		v, v,
	}

	{ }

	///  Value constructor.
	explicit
	constexpr
	matrix2 (const array_type & matrix) :
		m_array
	{
		matrix [0], matrix [1],
		matrix [2], matrix [3],
	}

	{ }

	///  @name Assignment operators

	template <class T>
	matrix2 &
	operator = (const matrix2 <T> & other);

	///  @name Element access

	void
	x (const vector_type & vector)
	{ m_value .x (vector); }

	const vector_type &
	x () const
	{ return m_value .x (); }

	void
	y (const vector_type & vector)
	{ m_value .y (vector); }

	const vector_type &
	y () const
	{ return m_value .y (); }

	void
	x_axis (const Type & vector)
	{ m_array [0] = vector; }

	const Type &
	x_axis () const
	{ return m_array [0]; }

	void
	origin (const Type & vector)
	{ m_array [2] = vector; }

	const Type &
	origin () const
	{ return m_array [2]; }

	void
	set ();

	void
	set (const Type & translation);

	void
	set (const Type & translation, const Type & scale);

	void
	get (Type & translation) const;

	void
	get (Type & translation, Type & scale) const;

	///  Access rows by @a index.
	vector_type &
	operator [ ] (const size_type index)
	{ return m_value [index]; }

	const vector_type &
	operator [ ] (const size_type index) const
	{ return m_value [index]; }

	///  Returns pointer to the underlying array serving as element storage.
	///  Specifically the pointer is such that range [data (); data () + size ()) is valid.
	Type*
	data ()
	{ return m_array; }

	const Type*
	data () const
	{ return m_array; }

	///  Get access to the underlying vector representation of this matrix.
	void
	value (const matrix_type & vector)
	{ m_value = vector; }

	const matrix_type &
	value () const
	{ return m_value; }

	///  Constructs a matrix3 from a matrix2 rotation matrix.
	matrix2 &
	submatrix (const Type & other)
	{
		m_array [0] = other;
		m_array [1] = 0;
		m_array [2] = 0;
		m_array [3] = 1;

		return *this;
	}

	///  Access 2x2 submatrix.
	constexpr
	Type
	submatrix () const
	{ return m_array [0]; }

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
	constexpr
	size_type
	order ()
	{ return ORDER; }

	///  Returns the number of elements in the matrix. The size is the same as order () * order ().
	static
	constexpr
	size_type
	size ()
	{ return SIZE; }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix2 inplace.

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
	matrix2 &
	operator += (const matrix2 & matrix);

	///  Add @a t to this matrix.
	matrix2 &
	operator += (const Type & t);

	///  Subtract @a matrix from this matrix.
	matrix2 &
	operator -= (const matrix2 & matrix);

	///  Subtract @a t from this matrix.
	matrix2 &
	operator -= (const Type & t);

	///  Returns this matrix right multiplied by @a matrix.
	matrix2 &
	operator *= (const matrix2 & matrix);

	///  Returns this matrix multiplies by @a t.
	matrix2 &
	operator *= (const Type & t);

	///  Returns this matrix divided by @a t.
	matrix2 &
	operator /= (const Type & t);

	///  Returns this matrix left multiplied by @a matrix.
	void
	mult_left (const matrix2 & matrix);

	///  Returns this matrix right multiplied by @a matrix.
	void
	mult_right (const matrix2 & matrix);

	///  Returns a new vector that is @a vector multiplies by matrix.
	Type
	mult_vec_matrix (const Type & vector) const;

	///  Returns a new vector that is @a vector multiplies by matrix.
	constexpr
	vector2 <Type>
	mult_vec_matrix (const vector2 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector.
	Type
	mult_matrix_vec (const Type & vector) const;

	///  Returns a new vector that is matrix multiplies by @va ector.
	constexpr
	vector2 <Type>
	mult_matrix_vec (const vector2 <Type> & vector) const;

	///  Returns a new vector that is @a vector (a normal or direction vector) multiplies by matrix.
	constexpr
	Type
	mult_dir_matrix (const Type & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector (a normal or direction vector).
	constexpr
	Type
	mult_matrix_dir (const Type &) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const Type & translation);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const Type & scale);


private:

	///  @name Members

	union
	{
		matrix_type m_value;
		array_type m_array;
	};

};

template <class Type>
template <class Up>
inline
matrix2 <Type> &
matrix2 <Type>::operator = (const matrix2 <Up> & matrix)
{
	m_value = matrix .value ();
	return *this;
}

template <class Type>
inline
void
matrix2 <Type>::set ()
{
	*this = matrix2 ();
}

template <class Type>
void
matrix2 <Type>::set (const Type & translation)
{
	m_value [0] = vector2 <Type> (1, 0);
	m_value [1] = vector2 <Type> (translation, 1);
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
	scaleFactor = m_array [0];
}

template <class Type>
inline
constexpr
Type
matrix2 <Type>::determinant1 () const
{
	return m_array [0];
}

template <class Type>
constexpr
Type
matrix2 <Type>::determinant () const
{
	return
	   m_array [0] * m_array [3] -
	   m_array [1] * m_array [2];
}

template <class Type>
void
matrix2 <Type>::transpose ()
{
	*this = matrix2 <Type> (m_array [0], m_array [2],
	                        m_array [1], m_array [3]);
}

template <class Type>
void
matrix2 <Type>::negate ()
{
	*this = matrix2 <Type> (-m_array [0], -m_array [1],
	                        -m_array [2], -m_array [3]);
}

template <class Type>
void
matrix2 <Type>::inverse ()
throw (std::domain_error)
{
	const Type d = determinant ();

	if (d == 0)
		throw std::domain_error ("matrix2::inverse: determinant is 0.");

	m_array [0] = m_array [0] / d;
	m_array [1] = -m_array [1] / d;
	m_array [2] = -m_array [2] / d;
	m_array [3] = m_array [3] / d;
}

template <class Type>
inline
matrix2 <Type> &
matrix2 <Type>::operator += (const matrix2 & matrix)
{
	m_value += matrix .value ();
	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator += (const Type & t)
{
	m_value [0] += t;
	m_value [1] += t;

	return *this;
}

template <class Type>
inline
matrix2 <Type> &
matrix2 <Type>::operator -= (const matrix2 & matrix)
{
	m_value -= matrix .value ();
	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator -= (const Type & t)
{
	m_value [0] -= t;
	m_value [1] -= t;

	return *this;
}

template <class Type>
inline
matrix2 <Type> &
matrix2 <Type>::operator *= (const matrix2 & matrix)
{
	mult_right (matrix);
	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator *= (const Type & t)
{
	m_value [0] *= t;
	m_value [1] *= t;

	return *this;
}

template <class Type>
matrix2 <Type> &
matrix2 <Type>::operator /= (const Type & t)
{
	m_value [0] /= t;
	m_value [1] /= t;

	return *this;
}

template <class Type>
void
matrix2 <Type>::mult_left (const matrix2 & matrix)
{
	#define MULT_LEFT(i, j) \
	   (m_array [0 * 2 + j] * matrix .m_array [i * 2 + 0] +   \
	    m_array [1 * 2 + j] * matrix .m_array [i * 2 + 1])

	*this = matrix2 <Type> (MULT_LEFT (0, 0),
	                        MULT_LEFT (0, 1),

	                        MULT_LEFT (1, 0),
	                        MULT_LEFT (1, 1));

	#undef MULT_LEFT
}

template <class Type>
void
matrix2 <Type>::mult_right (const matrix2 & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (m_array [i * 2 + 0] * matrix .m_array [0 * 2 + j] +   \
	    m_array [i * 2 + 1] * matrix .m_array [1 * 2 + j])

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
	const Type w = vector * m_array [1] + m_array [3];

	return (vector * m_array [0] + m_array [2]) / w;
}

template <class Type>
constexpr
vector2 <Type>
matrix2 <Type>::mult_vec_matrix (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * m_array [0] + vector .y () * m_array [2],
	                       vector .x () * m_array [1] + vector .y () * m_array [3]);
}

template <class Type>
Type
matrix2 <Type>::mult_matrix_vec (const Type & vector) const
{
	const Type w = vector * m_array [2] + m_array [3];

	return (vector * m_array [0] + m_array [1]) / w;
}

template <class Type>
constexpr
vector2 <Type>
matrix2 <Type>::mult_matrix_vec (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * m_array [0] + vector .y () * m_array [1],
	                       vector .x () * m_array [2] + vector .y () * m_array [3]);
}

template <class Type>
constexpr
Type
matrix2 <Type>::mult_dir_matrix (const Type & vector) const
{
	return vector * m_array [0];
}

template <class Type>
constexpr
Type
matrix2 <Type>::mult_matrix_dir (const Type & vector) const
{
	return vector * m_array [0];
}

template <class Type>
void
matrix2 <Type>::translate (const Type & translation)
{
	#define TRANSLATE(i) \
	   (m_value [0] [i] * translation)

	m_value [1] [0] += TRANSLATE (0);

	#undef TRANSLATE
}

template <class Type>
void
matrix2 <Type>::scale (const Type & scaleFactor)
{
	m_value [0] [0] *= scaleFactor;
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
	return lhs .value () == rhs .value ();
}

///  Compares two matrix2 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return lhs .value () not_eq rhs .value ();
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

///  Returns new matrix value @a lhs plus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator + (const matrix2 <Type> & lhs, const Type & rhs)
{
	return matrix2 <Type> (lhs) += rhs;
}

///  Return matrix value @a lhs right multiplied by scalar @a rhs.
template <class Type>
inline
matrix2 <Type>
operator + (const Type & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (rhs) += lhs;
}

///  Returns new matrix value @a a minus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator - (const matrix2 <Type> & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator - (const matrix2 <Type> & lhs, const Type & rhs)
{
	return matrix2 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix2 <Type>
operator - (const Type & lhs, const matrix2 <Type> & rhs)
{
	return matrix2 <Type> (-rhs) += lhs;
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
	return lhs .mult_matrix_vec (rhs);
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
	return ostream << matrix .value ();
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
