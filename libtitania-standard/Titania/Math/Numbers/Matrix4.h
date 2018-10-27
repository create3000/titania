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

#ifndef __TITANIA_MATH_NUMBERS_MATRIX4_H__
#define __TITANIA_MATH_NUMBERS_MATRIX4_H__

#include "Matrix3.h"
#include "Rotation4.h"
#include "Vector3.h"
#include "Vector4.h"

#include <cfloat>
#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

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

	static constexpr size_t Order   = 4;
	static constexpr size_t Rows    = Order;
	static constexpr size_t Columns = Order;

	///  C Array typedef.
	using array_type = std::array <Type, Rows * Columns>;


public:

	///  @name Member types

	///  Type.
	using value_type = vector4 <Type>;

	///  Type.
	using matrix_type = vector4 <value_type>;

	///  Size typedef.  Used for size and indices.
	using size_type = typename matrix_type::size_type;

	///  std::ptrdiff_t
	using difference_type = typename matrix_type::difference_type;

	///  matrix_type &
	using reference = typename matrix_type::reference;

	///  const matrix_type &
	using const_reference = typename matrix_type::const_reference;

	///  matrix_type*
	using pointer = typename matrix_type::pointer;

	///  const matrix_type*
	using const_pointer = typename matrix_type::const_pointer;

	///  Random access iterator
	using iterator = typename matrix_type::iterator;

	///  Constant random access iterator 
	using const_iterator = typename matrix_type::const_iterator;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = typename matrix_type::reverse_iterator;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = typename matrix_type::const_reverse_iterator;

	///  Vector typedef.
	using vector_type = vector3 <Type>;

	///  Rotation typedef.
	using rotation_type = rotation4 <Type>;

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	constexpr
	matrix4 () :
		matrix4 (1, 0, 0, 0,
		         0, 1, 0, 0,
		         0, 0, 1, 0,
		         0, 0, 0, 1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix4 (const matrix4 <Up> & other) :
		m_matrix (other [0], other [1], other [2], other [3])
	{ }

	///  Vector constructor.
	template <class Up>
	constexpr
	matrix4 (const vector4 <Up> & x, const vector4 <Up> & y, const vector4 <Up> & z, const vector4 <Up> & w) :
		m_matrix (x, y, z, w)
	{ }

	///  Components constructor. Set values from @a e11 to @a e44.
	constexpr
	matrix4 (const Type & e11, const Type & e12, const Type & e13, const Type & e14,
	         const Type & e21, const Type & e22, const Type & e23, const Type & e24,
	         const Type & e31, const Type & e32, const Type & e33, const Type & e34,
	         const Type & e41, const Type & e42, const Type & e43, const Type & e44) :
		m_array { e11, e12, e13, e14,
		          e21, e22, e23, e24,
		          e31, e32, e33, e34,
		          e41, e42, e43, e44 }
	{ }

	///  Constructs the matrix to from parameters.
	matrix4 (const vector3 <Type> & translation) :
		matrix4 ()
	{ set (translation); }

	///  Constructs the matrix to from parameters.
	matrix4 (const vector3 <Type> & translation,
	         const rotation4 <Type> & rotation) :
		matrix4 ()
	{ set (translation, rotation); }

	///  Constructs the matrix to from parameters.
	matrix4 (const vector3 <Type> & translation,
	         const rotation4 <Type> & rotation,
	         const vector3 <Type> & scale) :
		matrix4 ()
	{ set (translation, rotation, scale); }

	///  Constructs the matrix to from parameters.
	matrix4 (const vector3 <Type> & translation,
	         const rotation4 <Type> & rotation,
	         const vector3 <Type> & scale,
	         const rotation4 <Type> & scaleOrientation) :
		matrix4 ()
	{ set (translation, rotation, scale, scaleOrientation); }

	///  Constructs the matrix to from parameters.
	matrix4 (const vector3 <Type> & translation,
	         const rotation4 <Type> & rotation,
	         const vector3 <Type> & scale,
	         const rotation4 <Type> & scaleOrientation,
	         const vector3 <Type> & center) :
		matrix4 ()
	{ set (translation, rotation, scale, scaleOrientation, center); }
	

	///  Constructs a matrix4 from a rotation4.
	explicit
	matrix4 (const rotation4 <Type> & rotation) :
		matrix4 ()
	{ submatrix (rotation .matrix ()); }

	///  @name Assignment operators

	template <class Up>
	matrix4 &
	operator = (const matrix4 <Up> & other);

	///  @name Element access

	///  Set x component of this matrix.
	void
	x (const vector4 <Type> & vector)
	{ m_matrix .x (vector); }

	///  Return x component of this matrix.
	constexpr
	const vector4 <Type> &
	x () const
	{ return m_matrix .x (); }

	///  Set y component of this matrix.
	void
	y (const vector4 <Type> & vector)
	{ m_matrix .y (vector); }

	///  Return y component of this matrix.
	constexpr
	const vector4 <Type> &
	y () const
	{ return m_matrix .y (); }

	///  Set z component of this matrix.
	void
	z (const vector4 <Type> & vector)
	{ m_matrix .z (vector); }

	///  Return z component of this matrix.
	constexpr
	const vector4 <Type> &
	z () const
	{ return m_matrix .z (); }

	///  Set w component of this matrix.
	void
	w (const vector4 <Type> & vector)
	{ m_matrix .w (vector); }

	///  Return w component of this matrix.
	constexpr
	const vector4 <Type> &
	w () const
	{ return m_matrix .w (); }

	///  Set x-axis of this matrix.
	void
	x_axis (const vector3 <Type> & vector)
	{
		m_array [0] = vector .x ();
		m_array [1] = vector .y ();
		m_array [2] = vector .z ();
	}

	///  Return x-axis of this matrix.
	constexpr
	vector_type
	x_axis () const
	{ return vector3 <Type> (m_array [0], m_array [1], m_array [2]); }

	///  Set y-axis of this matrix.
	void
	y_axis (const vector3 <Type> & vector)
	{
		m_array [4] = vector .x ();
		m_array [5] = vector .y ();
		m_array [6] = vector .z ();
	}

	///  Return y-axis of this matrix.
	constexpr
	vector_type
	y_axis () const
	{ return vector3 <Type> (m_array [4], m_array [5], m_array [6]); }

	///  Set z-axis of this matrix.
	void
	z_axis (const vector3 <Type> & vector)
	{
		m_array [ 8] = vector .x ();
		m_array [ 9] = vector .y ();
		m_array [10] = vector .z ();
	}

	///  Return z-axis of this matrix.
	constexpr
	vector_type
	z_axis () const
	{ return vector3 <Type> (m_array [8], m_array [9], m_array [10]); }

	///  Set origin of this matrix.
	void
	origin (const vector3 <Type> & vector)
	{
		m_array [12] = vector .x ();
		m_array [13] = vector .y ();
		m_array [14] = vector .z ();
	}

	///  Return origin of this matrix.
	constexpr
	vector_type
	origin () const
	{ return vector3 <Type> (m_array [12], m_array [13], m_array [14]); }

	///  Constructs a matrix4 from a matrix3 rotation matrix.
	matrix4 &
	submatrix (const matrix3 <Type> & other)
	{
		m_array [ 0] = other [0] [0];
		m_array [ 1] = other [0] [1];
		m_array [ 2] = other [0] [2];
		m_array [ 3] = 0;
		m_array [ 4] = other [1] [0];
		m_array [ 5] = other [1] [1];
		m_array [ 6] = other [1] [2];
		m_array [ 7] = 0;
		m_array [ 8] = other [2] [0];
		m_array [ 9] = other [2] [1];
		m_array [10] = other [2] [2];
		m_array [11] = 0;
		m_array [12] = 0;
		m_array [13] = 0;
		m_array [14] = 0;
		m_array [15] = 1;

		return *this;
	}

	///  Access 3x3 submatrix.
	constexpr
	matrix3 <Type>
	submatrix () const
	{ return matrix3 <Type> (m_array [0], m_array [1], m_array [2],
		                      m_array [4], m_array [5], m_array [6],
		                      m_array [8], m_array [9], m_array [10]); }

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set ();

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector3 <Type> & translation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector3 <Type> & translation,
	     const rotation4 <Type> & rotation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector3 <Type> & translation,
	     const rotation4 <Type> & rotation,
	     const vector3 <Type> & scale);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector3 <Type> & translation,
	     const rotation4 <Type> & rotation,
	     const vector3 <Type> & scale,
	     const rotation4 <Type> & scaleOrientation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector3 <Type> & translation,
	     const rotation4 <Type> & rotation,
	     const vector3 <Type> & scale,
	     const rotation4 <Type> & scaleOrientation,
	     const vector3 <Type> & center);

	/// Computes and returns the transformation values from the matrix.
	template <class T>
	void
	get (vector3 <T> & translation) const;

	template <class T, class R>
	void
	get (vector3 <T> & translation,
	     rotation4 <R> & rotation) const;

	template <class T, class R, class S>
	void
	get (vector3 <T> & translation,
	     rotation4 <R> & rotation,
	     vector3 <S> & scale) const;

	template <class T, class R, class S, class SO>
	void
	get (vector3 <T> & translation,
	     rotation4 <R> & rotation,
	     vector3 <S> & scale,
	     rotation4 <SO> & scaleOrientation) const;

	template <class T, class R, class S, class SO, class C>
	void
	get (vector3 <T> & translation,
	     rotation4 <R> & rotation,
	     vector3 <S> & scale,
	     rotation4 <SO> & scaleOrientation, 
	     const vector3 <C> & center) const;

	///  Access specified row with bounds checking.
	constexpr
	vector4 <Type> &
	at (const size_type index)
	{ return m_matrix .at (index); }

	///  Access specified row with bounds checking.
	constexpr
	const vector4 <Type> &
	at (const size_type index) const
	{ return m_matrix .at (index); }

	///  Access row by @a index.
	constexpr
	vector4 <Type> &
	operator [ ] (const size_type index)
	{ return m_matrix [index]; }

	///  Access row by @a index.
	constexpr
	const vector4 <Type> &
	operator [ ] (const size_type index) const
	{ return m_matrix [index]; }

	///  Returns a reference to the first element in the container. 
	constexpr
	reference
	front ()
	{ return m_matrix .front (); }

	///  Returns a reference to the first element in the container. 
	constexpr
	const_reference
	front () const
	{ return m_matrix .front (); }

	///  Returns reference to the last element in the container. 
	constexpr
	reference
	back ()
	{ return m_matrix .back (); }

	///  Returns reference to the last element in the container. 
	constexpr
	const_reference
	back () const
	{ return m_matrix .back (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	pointer
	data ()
	{ return m_matrix .data (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	const_pointer
	data () const
	{ return m_matrix .data (); }

	///  Get access to the underlying vector representation of this matrix.
	void
	value (const matrix_type & value)
	{ m_matrix = value; }

	const matrix_type &
	value () const
	{ return m_matrix; }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	constexpr
	iterator
	begin ()
	{ return m_matrix .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	begin () const
	{ return m_matrix .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	cbegin () const
	{ return m_matrix .cbegin (); }

	///  Returns an iterator to the end.
	constexpr
	iterator
	end ()
	{ return m_matrix .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	end () const
	{ return m_matrix .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	cend () const
	{ return m_matrix .cend (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	reverse_iterator
	rbegin ()
	{ return m_matrix .rbegin (); }

	///  returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	rbegin () const
	{ return m_matrix .rbegin (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	crbegin () const
	{ return m_matrix .crbegin (); }

	///  Returns a reverse iterator to the end.
	constexpr
	reverse_iterator
	rend ()
	{ return m_matrix .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	rend () const
	{ return m_matrix .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	crend () const
	{ return m_matrix .crend (); }

	///  @name Capacity

	///  Checks whether the container is empty. Always returns false.
	constexpr
	bool
	empty () const
	{ return false; }

	///  Returns the order of this matrix.
	constexpr
	size_type
	order () const
	{ return Order; }

	///  Returns the number of coloums of this matrix. As this is a square matrix, the number is the same as order ().
	constexpr
	size_type
	columns () const
	{ return Columns; }

	///  Returns the number of rows of this matrix. As this is a square matrix, the number is the same as order ().
	constexpr
	size_type
	rows () const
	{ return Rows; }

	///  Returns the number of elements in the matrix. The size is the same as rows ().
	constexpr
	size_type
	size () const
	{ return Rows * Columns; }

	///  Returns the maximum possible number of elements. Because each matrix is a fixed-size container,
	///  the value is also the value returned by size.
	constexpr
	size_type
	max_size () const
	{ return Rows * Columns; }

	///  @name Operations

	///  Fill the container with specified @a value. 
	void
	fill (const value_type & value)
	{ m_matrix .fill (value); }

	///  Swaps the contents.
	void
	swap (matrix4 & other)
	{ m_matrix .swap (other .m_matrix); }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix inplace.

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
	matrix4 &
	operator += (const matrix4 & matrix);

	///  Add @a t to this matrix.
	matrix4 &
	operator += (const Type & t);

	///  Subtract @a matrix from this matrix.
	matrix4 &
	operator -= (const matrix4 & matrix);

	///  Subtract @a t from this matrix.
	matrix4 &
	operator -= (const Type & t);

	///  Returns this matrix right multiplied by @a matrix.
	matrix4 &
	operator *= (const matrix4 & matrix);

	///  Returns this matrix multiplies by @a t.
	matrix4 &
	operator *= (const Type & t);

	///  Returns this matrix divided by @a t.
	matrix4 &
	operator /= (const Type & t);

	///  Returns this matrix left multiplied by @a matrix.
	void
	mult_left (const matrix4 & matrix);

	///  Returns this matrix right multiplied by @a matrix.
	void
	mult_right (const matrix4 & matrix);

	///  Returns a new vector that is @a vector multiplies by matrix.
	vector3 <Type>
	mult_vec_matrix (const vector3 <Type> & vector) const;

	///  Returns a new vector that is @vector multiplies by matrix.
	constexpr
	vector4 <Type>
	mult_vec_matrix (const vector4 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector.
	vector3 <Type>
	mult_matrix_vec (const vector3 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector.
	constexpr
	vector4 <Type>
	mult_matrix_vec (const vector4 <Type> & vector) const;

	///  Returns a new vector that is @a vector (a normal or direction vector) multiplies by matrix.
	constexpr
	vector3 <Type>
	mult_dir_matrix (const vector3 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector (a normal or direction vector).
	constexpr
	vector3 <Type>
	mult_matrix_dir (const vector3 <Type> & vector) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const vector3 <Type> & translation);

	///  Returns this matrix rotated by @a rotation.
	void
	rotate (const rotation4 <Type> & rotation);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const vector3 <Type> & scale);


private:

	///  @name Operations

	template <class T, class S>
	bool
	factor (vector3 <T> & translation,
	        matrix3 <Type> & rotation,
	        vector3 <S> & scale,
	        matrix3 <Type> & scaleOrientation) const;

	///  @name Members

	union
	{
		matrix_type m_matrix;
		array_type  m_array;
	};

};

template <class Type>
template <class Up>
inline
matrix4 <Type> &
matrix4 <Type>::operator = (const matrix4 <Up> & matrix)
{
	m_matrix = matrix .value ();
	return *this;
}

template <class Type>
inline
void
matrix4 <Type>::set ()
{
	*this = matrix4 ();
}

template <class Type>
void
matrix4 <Type>::set (const vector3 <Type> & translation)
{
	m_matrix [0] = vector4 <Type> (1, 0, 0, 0);
	m_matrix [1] = vector4 <Type> (0, 1, 0, 0);
	m_matrix [2] = vector4 <Type> (0, 0, 1, 0);
	m_matrix [3] = vector4 <Type> (translation .x (), translation .y (), translation .z (), 1);
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
		{
			rotate (scaleOrientation);
			scale (scaleFactor);
			rotate (~scaleOrientation);
		}
		else
			scale (scaleFactor);
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
		{
			rotate (scaleOrientation);
			scale (scaleFactor);
			rotate (~scaleOrientation);
		}
		else
			scale (scaleFactor);
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
template <class T, class S>
bool
matrix4 <Type>::factor (vector3 <T> & translation,
                        matrix3 <Type> & rotation,
                        vector3 <S> & scale,
                        matrix3 <Type> & scaleOrientation) const
{
	// (1) Get translation.
	translation = origin ();

	// (2) Create 3x3 matrix.
	const matrix3 <Type> a = submatrix ();

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	const Type det      = a .determinant ();
	const Type det_sign = det < 0 ? -1 : 1;

	if (det == 0)
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
Type
matrix4 <Type>::determinant () const
{
	const Type
		m00 = m_array [ 0],
		m01 = m_array [ 1],
		m02 = m_array [ 2],
		m03 = m_array [ 3],
		m04 = m_array [ 4],
		m05 = m_array [ 5],
		m06 = m_array [ 6],
		m07 = m_array [ 7],
		m08 = m_array [ 8],
		m09 = m_array [ 9],
		m10 = m_array [10],
		m11 = m_array [11],
		m12 = m_array [12],
		m13 = m_array [13],
		m14 = m_array [14],
		m15 = m_array [15],
		b = m10 * m15,
		c = m14 * m11,
		d = m06 * m15,
		e = m14 * m07,
		f = m06 * m11,
		g = m10 * m07,
		h = m02 * m15,
		i = m14 * m03,
		j = m02 * m11,
		o = m10 * m03,
		r = m02 * m07,
		x = m06 * m03,
		H = b * m05 + e * m09 + f * m13 - (c * m05) - (d * m09) - (g * m13),
		I = c * m01 + h * m09 + o * m13 - (b * m01) - (i * m09) - (j * m13),
		J = d * m01 + i * m05 + r * m13 - (e * m01) - (h * m05) - (x * m13),
		K = g * m01 + j * m05 + x * m09 - (f * m01) - (o * m05) - (r * m09);

	return m00 * H + m04 * I + m08 * J + m12 * K;
}

template <class Type>
void
matrix4 <Type>::transpose ()
{
	*this = matrix4 <Type> (m_array [0], m_array [4], m_array [8],  m_array [12],
	                        m_array [1], m_array [5], m_array [9],  m_array [13],
	                        m_array [2], m_array [6], m_array [10], m_array [14],
	                        m_array [3], m_array [7], m_array [11], m_array [15]);
}

template <class Type>
void
matrix4 <Type>::negate ()
{
	*this = matrix4 <Type> (-m_array [ 0], -m_array [ 1], -m_array [ 2], -m_array [ 3],
	                        -m_array [ 4], -m_array [ 5], -m_array [ 6], -m_array [ 7],
	                        -m_array [ 8], -m_array [ 9], -m_array [10], -m_array [11],
	                        -m_array [12], -m_array [13], -m_array [14], -m_array [15]);
}

template <class Type>
void
matrix4 <Type>::inverse ()
throw (std::domain_error)
{
	const Type
		m00 = m_array [ 0],
		m01 = m_array [ 1],
		m02 = m_array [ 2],
		m03 = m_array [ 3],
		m04 = m_array [ 4],
		m05 = m_array [ 5],
		m06 = m_array [ 6],
		m07 = m_array [ 7],
		m08 = m_array [ 8],
		m09 = m_array [ 9],
		m10 = m_array [10],
		m11 = m_array [11],
		m12 = m_array [12],
		m13 = m_array [13],
		m14 = m_array [14],
		m15 = m_array [15],
		b = m10 * m15,
		c = m14 * m11,
		d = m06 * m15,
		e = m14 * m07,
		f = m06 * m11,
		g = m10 * m07,
		h = m02 * m15,
		i = m14 * m03,
		j = m02 * m11,
		o = m10 * m03,
		r = m02 * m07,
		x = m06 * m03,
		t = m08 * m13,
		p = m12 * m09,
		v = m04 * m13,
		s = m12 * m05,
		y = m04 * m09,
		z = m08 * m05,
		A = m00 * m13,
		C = m12 * m01,
		D = m00 * m09,
		E = m08 * m01,
		F = m00 * m05,
		G = m04 * m01,
		H = b * m05 + e * m09 + f * m13 - ((c * m05) + (d * m09) + (g * m13)),
		I = c * m01 + h * m09 + o * m13 - ((b * m01) + (i * m09) + (j * m13)),
		J = d * m01 + i * m05 + r * m13 - ((e * m01) + (h * m05) + (x * m13)),
		K = g * m01 + j * m05 + x * m09 - ((f * m01) + (o * m05) + (r * m09)),
		B = m00 * H + m04 * I + m08 * J + m12 * K;

	if (B == 0)
		throw std::domain_error ("matrix4::inverse: determinant is 0.");

	const Type L = 1 / B;

	m_array [ 0] = L * H;
	m_array [ 1] = L * I;
	m_array [ 2] = L * J;
	m_array [ 3] = L * K;
	m_array [ 4] = L * (c * m04 + d * m08 + g * m12 - (b * m04) - (e * m08) - (f * m12));
	m_array [ 5] = L * (b * m00 + i * m08 + j * m12 - (c * m00) - (h * m08) - (o * m12));
	m_array [ 6] = L * (e * m00 + h * m04 + x * m12 - (d * m00) - (i * m04) - (r * m12));
	m_array [ 7] = L * (f * m00 + o * m04 + r * m08 - (g * m00) - (j * m04) - (x * m08));
	m_array [ 8] = L * (t * m07 + s * m11 + y * m15 - (p * m07) - (v * m11) - (z * m15));
	m_array [ 9] = L * (p * m03 + A * m11 + E * m15 - (t * m03) - (C * m11) - (D * m15));
	m_array [10] = L * (v * m03 + C * m07 + F * m15 - (s * m03) - (A * m07) - (G * m15));
	m_array [11] = L * (z * m03 + D * m07 + G * m11 - (y * m03) - (E * m07) - (F * m11));
	m_array [12] = L * (v * m10 + z * m14 + p * m06 - (y * m14) - (t * m06) - (s * m10));
	m_array [13] = L * (D * m14 + t * m02 + C * m10 - (A * m10) - (E * m14) - (p * m02));
	m_array [14] = L * (A * m06 + G * m14 + s * m02 - (F * m14) - (v * m02) - (C * m06));
	m_array [15] = L * (F * m10 + y * m02 + E * m06 - (D * m06) - (G * m10) - (z * m02));
}

template <class Type>
inline
matrix4 <Type> &
matrix4 <Type>::operator += (const matrix4 & matrix)
{
	m_matrix += matrix .value ();
	return *this;
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator += (const Type & t)
{
	m_matrix [0] += t;
	m_matrix [1] += t;
	m_matrix [2] += t;
	m_matrix [3] += t;

	return *this;
}

template <class Type>
inline
matrix4 <Type> &
matrix4 <Type>::operator -= (const matrix4 & matrix)
{
	m_matrix -= matrix .value ();
	return *this;
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator -= (const Type & t)
{
	m_matrix [0] -= t;
	m_matrix [1] -= t;
	m_matrix [2] -= t;
	m_matrix [3] -= t;

	return *this;
}

template <class Type>
inline
matrix4 <Type> &
matrix4 <Type>::operator *= (const matrix4 & matrix)
{
	mult_right (matrix);
	return *this;
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator *= (const Type & t)
{
	m_matrix [0] *= t;
	m_matrix [1] *= t;
	m_matrix [2] *= t;
	m_matrix [3] *= t;

	return *this;
}

template <class Type>
matrix4 <Type> &
matrix4 <Type>::operator /= (const Type & t)
{
	m_matrix [0] /= t;
	m_matrix [1] /= t;
	m_matrix [2] /= t;
	m_matrix [3] /= t;

	return *this;
}

/**
 * It takes 64 multiplies and 64 adds.
 */
template <class Type>
void
matrix4 <Type>::mult_left (const matrix4 & matrix)
{
	#define MULT_LEFT(i, j) \
	   (m_matrix [0] [j] * matrix .m_matrix [i] [0] +   \
	    m_matrix [1] [j] * matrix .m_matrix [i] [1] +   \
	    m_matrix [2] [j] * matrix .m_matrix [i] [2] +   \
	    m_matrix [3] [j] * matrix .m_matrix [i] [3])

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
void
matrix4 <Type>::mult_right (const matrix4 & matrix)
{
	#define MULT_RIGHT(i, j) \
	   (m_matrix [i] [0] * matrix .m_matrix [0] [j] +   \
	    m_matrix [i] [1] * matrix .m_matrix [1] [j] +   \
	    m_matrix [i] [2] * matrix .m_matrix [2] [j] +   \
	    m_matrix [i] [3] * matrix .m_matrix [3] [j])

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
vector3 <Type>
matrix4 <Type>::mult_vec_matrix (const vector3 <Type> & vector) const
{
	const Type w = vector .x () * m_array [3] + vector .y () * m_array [7] + vector .z () * m_array [11] + m_array [15];

	return vector3 <Type> ((vector .x () * m_array [0] + vector .y () * m_array [4] + vector .z () * m_array [ 8] + m_array [12]) / w,
	                       (vector .x () * m_array [1] + vector .y () * m_array [5] + vector .z () * m_array [ 9] + m_array [13]) / w,
	                       (vector .x () * m_array [2] + vector .y () * m_array [6] + vector .z () * m_array [10] + m_array [14]) / w);
}

/**
 * It takes 16 multiplies and 12 adds.
 */
template <class Type>
constexpr
vector4 <Type>
matrix4 <Type>::mult_vec_matrix (const vector4 <Type> & vector) const
{
	return vector4 <Type> (vector .x () * m_array [0] + vector .y () * m_array [4] + vector .z () * m_array [ 8] + vector .w () * m_array [12],
	                       vector .x () * m_array [1] + vector .y () * m_array [5] + vector .z () * m_array [ 9] + vector .w () * m_array [13],
	                       vector .x () * m_array [2] + vector .y () * m_array [6] + vector .z () * m_array [10] + vector .w () * m_array [14],
	                       vector .x () * m_array [3] + vector .y () * m_array [7] + vector .z () * m_array [11] + vector .w () * m_array [15]);
}

/**
 * It takes 12 multiplies, 3 divs and 12 adds.
 */
template <class Type>
vector3 <Type>
matrix4 <Type>::mult_matrix_vec (const vector3 <Type> & vector) const
{
	const Type w = vector .x () * m_array [12] + vector .y () * m_array [13] + vector .z () * m_array [14] + m_array [15];

	return vector3 <Type> ((vector .x () * m_array [0] + vector .y () * m_array [1] + vector .z () * m_array [ 2] + m_array [ 3]) / w,
	                       (vector .x () * m_array [4] + vector .y () * m_array [5] + vector .z () * m_array [ 6] + m_array [ 7]) / w,
	                       (vector .x () * m_array [8] + vector .y () * m_array [9] + vector .z () * m_array [10] + m_array [11]) / w);
}

/**
 * It takes 16 multiplies and 12 adds.
 */
template <class Type>
constexpr
vector4 <Type>
matrix4 <Type>::mult_matrix_vec (const vector4 <Type> & vector) const
{
	return vector4 <Type> (vector .x () * m_array [ 0] + vector .y () * m_array [ 1] + vector .z () * m_array [ 2] + vector .w () * m_array [ 3],
	                       vector .x () * m_array [ 4] + vector .y () * m_array [ 5] + vector .z () * m_array [ 6] + vector .w () * m_array [ 7],
	                       vector .x () * m_array [ 8] + vector .y () * m_array [ 9] + vector .z () * m_array [10] + vector .w () * m_array [11],
	                       vector .x () * m_array [12] + vector .y () * m_array [13] + vector .z () * m_array [14] + vector .w () * m_array [15]);
}

/**
 * It takes 9 multiplies and 6 adds.
 */
template <class Type>
constexpr
vector3 <Type>
matrix4 <Type>::mult_dir_matrix (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * m_array [0] + vector .y () * m_array [4] + vector .z () * m_array [ 8],
	                       vector .x () * m_array [1] + vector .y () * m_array [5] + vector .z () * m_array [ 9],
	                       vector .x () * m_array [2] + vector .y () * m_array [6] + vector .z () * m_array [10]);
}

/**
 * It takes 9 multiplies and 6 adds.
 */
template <class Type>
constexpr
vector3 <Type>
matrix4 <Type>::mult_matrix_dir (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * m_array [0] + vector .y () * m_array [1] + vector .z () * m_array [ 2],
	                       vector .x () * m_array [4] + vector .y () * m_array [5] + vector .z () * m_array [ 6],
	                       vector .x () * m_array [8] + vector .y () * m_array [9] + vector .z () * m_array [10]);
}

/**
 * It takes 9 multiplies and 15 adds.
 */
template <class Type>
void
matrix4 <Type>::translate (const vector3 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (m_matrix [0] [i] * translation .x () +   \
	    m_matrix [1] [i] * translation .y () +   \
	    m_matrix [2] [i] * translation .z ())

	m_matrix [3] [0] += TRANSLATE (0);
	m_matrix [3] [1] += TRANSLATE (1);
	m_matrix [3] [2] += TRANSLATE (2);

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
	m_matrix [0] [0] *= scaleFactor .x ();
	m_matrix [0] [1] *= scaleFactor .x ();
	m_matrix [0] [2] *= scaleFactor .x ();

	m_matrix [1] [0] *= scaleFactor .y ();
	m_matrix [1] [1] *= scaleFactor .y ();
	m_matrix [1] [2] *= scaleFactor .y ();

	m_matrix [2] [0] *= scaleFactor .z ();
	m_matrix [2] [1] *= scaleFactor .z ();
	m_matrix [2] [2] *= scaleFactor .z ();
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
	return lhs .value () == rhs .value ();
}

///  Compares two matrix4 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .value () not_eq rhs .value ();
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return lhs .value () < rhs .value ();
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two matrix4 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates matrix4
///  @name Arithmetic operations

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

///  Returns new matrix value @a lhs plus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator + (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (lhs) += rhs;
}

///  Returns new matrix value @a lhs plus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator + (const matrix4 <Type> & lhs, const Type & rhs)
{
	return matrix4 <Type> (lhs) += rhs;
}

///  Return matrix value @a lhs right multiplied by scalar @a rhs.
template <class Type>
inline
matrix4 <Type>
operator + (const Type & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (rhs) += lhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator - (const matrix4 <Type> & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator - (const matrix4 <Type> & lhs, const Type & rhs)
{
	return matrix4 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix4 <Type>
operator - (const Type & lhs, const matrix4 <Type> & rhs)
{
	return matrix4 <Type> (-rhs) += lhs;
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

///  Return matrix value @a lhs right multiplied by @a rhs.
template <class Type>
inline
matrix4 <Type>
operator * (const matrix4 <Type> & lhs, const Type & rhs)
{
	return matrix4 <Type> (lhs) *= rhs;
}

///  Return matrix value @a lhs right multiplied by @a rhs.
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

///  Return @a matrix normalized, so that there are no axes with zero length.
template <class Type>
matrix4 <Type> 
normalize (const matrix4 <Type> & matrix)
{
	auto x = matrix .x_axis ();
	auto y = matrix .y_axis ();
	auto z = matrix .z_axis ();

	if (norm (x) == 0 and norm (y) == 0 and norm (z) == 0)
	{
		x = vector3 <Type> (1, 0, 0);
		y = vector3 <Type> (0, 1, 0);
		z = vector3 <Type> (0, 0, 1);
	}
	else
	{
		std::array <vector3 <Type>, 3> axes = { vector3 <Type> (1, 0, 0), vector3 <Type> (0, 1, 0), vector3 <Type> (0, 0, 1) };

		if (norm (x) == 0)
		{
			x = cross (y, z);
	
			if (norm (x) == 0)
			{
				for (const auto & a : axes)
				{
					x = cross (a, y);
	
					if (norm (x) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (x) == 0)
			{
				for (const auto & a : axes)
				{
					x = cross (a, z);
	
					if (norm (x) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (x) == 0)
				x = vector3 <Type> (1, 0, 0);
			else
				x .normalize ();
		}
	
		if (norm (y) == 0)
		{
			y = cross (z, x);
	
			if (norm (y) == 0)
			{
				for (const auto & a : axes)
				{
					y = cross (a, z);
	
					if (norm (y) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (y) == 0)
			{
				for (const auto & a : axes)
				{
					y = cross (a, x);
	
					if (norm (y) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (y) == 0)
				y = vector3 <Type> (0, 1, 0);
			else
				y .normalize ();
		}
	
		if (norm (z) == 0)
		{
			z = cross (x, y);
	
			if (norm (z) == 0)
			{
				for (const auto & a : axes)
				{
					z = cross (a, x);
	
					if (norm (z) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (z) == 0)
			{
				for (const auto & a : axes)
				{
					z = cross (a, y);
	
					if (norm (z) == 0)
						continue;
	
					break;
				}
			}
	
			if (norm (z) == 0)
				z = vector3 <Type> (0, 0, 1);
			else
				z .normalize ();
		}
	}

	const auto o = matrix .origin ();

	return matrix4 <Type> (x .x (), x .y (), x .z (), 0,
	                       y .x (), y .y (), y .z (), 0,
	                       z .x (), z .y (), z .z (), 0,
	                       o .x (), o .y (), o .z (), 1);
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
	return ostream << matrix .value ();
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

namespace std {

/// Provides access to the number of elements in an matrix4 as a compile-time constant expression. 
template< class Type>
class tuple_size <titania::math::matrix4 <Type>> :
    public integral_constant <size_t, titania::math::matrix4 <Type> () .size ()>
{ };

/// Provides compile-time indexed access to the type of the elements of the matrix4 using tuple-like interface.
template <std::size_t I, class Type>
struct tuple_element <I, titania::math::matrix4 <Type>>
{
	using type = typename titania::math::matrix4 <Type>::value_type;
};

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
typename titania::math::matrix4 <Type>::value_type &
get (typename titania::math::matrix4 <Type> & matrix)
{
	return matrix [Index];
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
const typename titania::math::matrix4 <Type>::value_type &
get (const typename titania::math::matrix4 <Type> & matrix)
{
	return matrix [Index];
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
typename titania::math::matrix4 <Type>::value_type &&
get (typename titania::math::matrix4 <Type> && matrix)
{
	return std::move (matrix [Index]);
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
const typename titania::math::matrix4 <Type>::value_type &&
get (const typename titania::math::matrix4 <Type> && matrix)
{
	return std::move (matrix [Index]);
}

/// Specializes the std::swap algorithm for matrix4.
template <class Type>
inline
void
swap (titania::math::matrix4 <Type> & lhs, titania::math::matrix4 <Type> & rhs) noexcept
{
	lhs .swap (rhs);
}

} // std

#endif
