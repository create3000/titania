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

#include "Matrix2.h"
#include "Vector2.h"
#include "Vector3.h"

#include <cfloat>
#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

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

	static constexpr size_t Order   = 4;
	static constexpr size_t Rows    = Order;
	static constexpr size_t Columns = Order;

	///  C Array typedef.
	using array_type = std::array <Type, Rows * Columns>;


public:

	///  @name Member types

	///  Type.
	using value_type = vector3 <Type>;

	///  Type.
	using matrix_type = vector3 <value_type>;

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
	using vector_type = vector2 <Type>;

	///  Rotation typedef.
	using rotation_type = Type;

	///  @name Constructors

	///  Default constructor. A new matrix initialized with the identity matrix is created and returned.
	constexpr
	matrix3 () :
		matrix3 (1, 0, 0,
		         0, 1, 0,
		         0, 0, 1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	matrix3 (const matrix3 <Up> & other) :
		m_matrix (other [0], other [1], other [2])
	{ }

	///  Vector constructor.
	template <class Up>
	constexpr
	matrix3 (const vector3 <Up> & x, const vector3 <Up> & y, const vector3 <Up> & z) :
		m_matrix (x, y, z)
	{ }

	///  Components constructor. Set values from @a e11 to @a e33.
	constexpr
	matrix3 (const Type & e11, const Type & e12, const Type & e13,
	         const Type & e21, const Type & e22, const Type & e23,
	         const Type & e31, const Type & e32, const Type & e33) :
		m_array { e11, e12, e13,
		          e21, e22, e23,
		          e31, e32, e33 }
	{ }

	///  Constructs the matrix to from parameters.
	matrix3 (const vector2 <Type> & translation) :
		matrix3 ()
	{ set (translation); }

	///  Constructs the matrix to from parameters.
	matrix3 (const vector2 <Type> & translation,
	        const Type & rotation) :
		matrix3 ()
	{ set (translation, rotation); }

	///  Constructs the matrix to from parameters.
	matrix3 (const vector2 <Type> & translation, 
	         const Type & rotation,
	         const vector2 <Type> & scale) :
		matrix3 ()
	{ set (translation, rotation, scale); }

	///  Constructs the matrix to from parameters.
	matrix3 (const vector2 <Type> & translation,
	         const Type & rotation,
	         const vector2 <Type> & scale,
	         const Type & scaleOrientation) :
		matrix3 ()
	{ set (translation, rotation, scale, scaleOrientation); }

	///  Constructs the matrix to from parameters.
	matrix3 (const vector2 <Type> & translation,
	         const Type & rotation,
	         const vector2 <Type> & scale,
	         const Type & scaleOrientation,
	         const vector2 <Type> & center) :
		matrix3 ()
	{ set (translation, rotation, scale, scaleOrientation, center); }

	///  @name Assignment operators

	template <class T>
	matrix3 &
	operator = (const matrix3 <T> & other);

	///  @name Element access

	///  Set x component of this matrix.
	void
	x (const vector3 <Type> & vector)
	{ m_matrix .x (vector); }

	///  Return x component of this matrix.
	constexpr
	const vector3 <Type> &
	x () const
	{ return m_matrix .x (); }

	///  Set y component of this matrix.
	void
	y (const vector3 <Type> & vector)
	{ m_matrix .y (vector); }

	///  Return y component of this matrix.
	constexpr
	const vector3 <Type> &
	y () const
	{ return m_matrix .y (); }

	///  Set z component of this matrix.
	void
	z (const vector3 <Type> & vector)
	{ m_matrix .z (vector); }

	///  Return z component of this matrix.
	constexpr
	const vector3 <Type> &
	z () const
	{ return m_matrix .z (); }

	///  Set x-axis of this matrix.
	void
	x_axis (const vector2 <Type> & vector)
	{
		m_array [0] = vector .x ();
		m_array [1] = vector .y ();
	}

	///  Return x-axis of this matrix.
	constexpr
	vector2 <Type>
	x_axis () const
	{ return vector2 <Type> (m_array [0], m_array [1]); }

	///  Set y-axis of this matrix.
	void
	y_axis (const vector2 <Type> & vector)
	{
		m_array [3] = vector .x ();
		m_array [4] = vector .y ();
	}

	///  Return y-axis of this matrix.
	constexpr
	vector2 <Type>
	y_axis () const
	{ return vector2 <Type> (m_array [3], m_array [4]); }

	///  Set origin of this matrix.
	void
	origin (const vector2 <Type> & vector)
	{
		m_array [6] = vector .x ();
		m_array [7] = vector .y ();
	}

	///  Return origin of this matrix.
	constexpr
	vector2 <Type>
	origin () const
	{ return vector2 <Type> (m_array [6], m_array [7]); }

	///  Constructs a matrix3 from a matrix2 rotation matrix.
	matrix3 &
	submatrix (const matrix2 <Type> & other)
	{
		m_array [0] = other [0] [0];
		m_array [1] = other [0] [1];
		m_array [2] = 0;
		m_array [3] = other [1] [0];
		m_array [4] = other [1] [1];
		m_array [5] = 0;
		m_array [6] = 0;
		m_array [7] = 0;
		m_array [8] = 1;

		return *this;
	}

	///  Access 2x2 submatrix.
	constexpr
	matrix2 <Type>
	submatrix () const
	{ return matrix2 <Type> (m_array [0], m_array [1],
		                      m_array [3], m_array [4]); }

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set ();

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector2 <Type> & translation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector2 <Type> & translation,
	     const Type & rotation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector2 <Type> & translation, 
	     const Type & rotation,
	     const vector2 <Type> & scale);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector2 <Type> & translation,
	     const Type & rotation,
	     const vector2 <Type> & scale,
	     const Type & scaleOrientation);

	///  Sets the matrix to the new value calculated from the parameters.
	void
	set (const vector2 <Type> & translation,
	     const Type & rotation,
	     const vector2 <Type> & scale,
	     const Type & scaleOrientation,
	     const vector2 <Type> & center);

	/// Computes and returns the transformation values from the matrix.
	void
	get (vector2 <Type> & translation) const;

	void
	get (vector2 <Type> & translation,
	     Type & rotation) const;

	void
	get (vector2 <Type> & translation,
	     Type & rotation,
	     vector2 <Type> & scale) const;

	void
	get (vector2 <Type> & translation,
	     Type & rotation,
	     vector2 <Type> & scale,
	     Type & scaleOrientation) const;

	void
	get (vector2 <Type> & translation,
	     Type & rotation,
	     vector2 <Type> & scale,
	     Type & scaleOrientation,
	     vector2 <Type> & center) const;

	///  Access specified row with bounds checking.
	constexpr
	vector3 <Type> &
	at (const size_type index)
	{ return m_matrix .at (index); }

	///  Access specified row with bounds checking.
	constexpr
	const vector3 <Type> &
	at (const size_type index) const
	{ return m_matrix .at (index); }

	///  Access row by @a index.
	constexpr
	vector3 <Type> &
	operator [ ] (const size_type index)
	{ return m_matrix [index]; }

	///  Access row by @a index.
	constexpr
	const vector3 <Type> &
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
	swap (matrix3 & other)
	{ m_matrix .swap (other .m_matrix); }

	///  @name  Arithmetic operations
	///  All these operators modify this matrix inplace.

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
	operator += (const matrix3 & matrix);

	///  Add @a t to this matrix.
	matrix3 &
	operator += (const Type & t);

	///  Subtract @a matrix from this matrix.
	matrix3 &
	operator -= (const matrix3 & matrix);

	///  Subtract @a t from this matrix.
	matrix3 &
	operator -= (const Type & t);

	///  Returns this matrix right multiplied by @a matrix.
	matrix3 &
	operator *= (const matrix3 & matrix);

	///  Returns this matrix multiplies by @a t.
	matrix3 &
	operator *= (const Type & t);

	///  Returns this matrix divided by @a t.
	matrix3 &
	operator /= (const Type & t);

	///  Returns this matrix left multiplied by @a matrix.
	void
	mult_left (const matrix3 & matrix);

	///  Returns this matrix right multiplied by @a matrix.
	void
	mult_right (const matrix3 & matrix);

	///  Returns a new vector that is @a vector multiplies by matrix.
	vector2 <Type>
	mult_vec_matrix (const vector2 <Type> & vector) const;

	///  Returns a new vector that is @a vector multiplies by matrix.
	constexpr
	vector3 <Type>
	mult_vec_matrix (const vector3 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector.
	vector2 <Type>
	mult_matrix_vec (const vector2 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector.
	constexpr
	vector3 <Type>
	mult_matrix_vec (const vector3 <Type> & vector) const;

	///  Returns a new vector that is @a vector (a normal or direction vector) multiplies by matrix.
	constexpr
	vector2 <Type>
	mult_dir_matrix (const vector2 <Type> & vector) const;

	///  Returns a new vector that is matrix multiplies by @a vector (a normal or direction vector).
	constexpr
	vector2 <Type>
	mult_matrix_dir (const vector2 <Type> & vector) const;

	///  Returns this matrix translated by @a translation.
	void
	translate (const vector2 <Type> & translation);

	///  Returns this matrix rotated by @a rotation.
	void
	rotate (const Type & rotation);

	///  Returns this matrix scaled by @a scale.
	void
	scale (const vector2 <Type> & scale);

	///  Returns this matrix transformed by @a skewAngle.
	void
	skew_x (const Type & skewAngle);

	///  Returns this matrix transformed by @a skewAngle.
	void
	skew_y (const Type & skewAngle);


private:

	///  @name Operations

	void
	rotation (const Type & rotation);

	Type
	rotation () const;

	template <class T, class S>
	bool
	factor (vector2 <T> & translation,
	        matrix2 <Type> & rotation,
	        vector2 <S> & scale,
	        matrix2 <Type> & scaleOrientation) const;

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
matrix3 <Type> &
matrix3 <Type>::operator = (const matrix3 <Up> & matrix)
{
	m_matrix = matrix .value ();
	return *this;
}

template <class Type>
void
matrix3 <Type>::rotation (const Type & rotation)
{
	const Type sinAngle = std::sin (rotation);
	const Type cosAngle = std::cos (rotation);

	m_array [0] = cosAngle;
	m_array [1] = sinAngle;
	m_array [2] = 0;

	m_array [3] = -sinAngle;
	m_array [4] = cosAngle;
	m_array [5] = 0;

	m_array [6] = 0;
	m_array [7] = 0;
	m_array [8] = 1;
}

template <class Type>
Type
matrix3 <Type>::rotation () const
{
	return std::atan2 (m_array [1], m_array [0]);
}

template <class Type>
inline
void
matrix3 <Type>::set ()
{
	*this = matrix3 ();
}

template <class Type>
void
matrix3 <Type>::set (const vector2 <Type> & translation)
{
	m_matrix [0] = vector3 <Type> (1, 0, 0);
	m_matrix [1] = vector3 <Type> (0, 1, 0);
	m_matrix [2] = vector3 <Type> (translation .x (), translation .y (), 1);
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
	const matrix2 <Type> a = submatrix ();

	// (3) Compute det A. If negative, set sign = -1, else sign = 1
	const Type det      = a .determinant ();
	const Type det_sign = det < 0 ? -1 : 1;

	if (det == 0)
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
matrix3 <Type>::determinant () const
{
	const Type
		m0 = m_array [0], m1 = m_array [1], m2 = m_array [2],
		m3 = m_array [3], m4 = m_array [4], m5 = m_array [5],
		m6 = m_array [6], m7 = m_array [7], m8 = m_array [8];

	return m0 * (m4 * m8 - m5 * m7) -
	       m1 * (m3 * m8 - m5 * m6) +
	       m2 * (m3 * m7 - m4 * m6);
}

template <class Type>
void
matrix3 <Type>::transpose ()
{
	*this = matrix3 <Type> (m_array [0], m_array [3], m_array [6],
	                        m_array [1], m_array [4], m_array [7],
	                        m_array [2], m_array [5], m_array [8]);
}

template <class Type>
void
matrix3 <Type>::negate ()
{
	*this = matrix3 <Type> (-m_array [0], -m_array [1], -m_array [2],
	                        -m_array [3], -m_array [4], -m_array [5],
	                        -m_array [6], -m_array [7], -m_array [8]);
}

template <class Type>
void
matrix3 <Type>::inverse ()
throw (std::domain_error)
{
	const Type
		m0  = m_array [0],
		m1  = m_array [1],
		m2  = m_array [2],
		m3  = m_array [3],
		m4  = m_array [4],
		m5  = m_array [5],
		m6  = m_array [6],
		m7  = m_array [7],
		m8  = m_array [8],
		t4  = m0 * m4,
		t6  = m0 * m7,
		t8  = m3 * m1,
		t10 = m3 * m7,
		t12 = m6 * m1,
		t14 = m6 * m4;

	const Type d = (t4 * m8 - t6 * m5 - t8 * m8 + t10 * m2 + t12 * m5 - t14 * m2);

	if (d == 0)
		throw std::domain_error ("matrix3::inverse: determinant is 0.");

	const Type L = 1 / d;

	const Type
		b0 =  (m4 * m8 - m7 * m5) * L,
		b1 = -(m1 * m8 - m7 * m2) * L,
		b2 =  (m1 * m5 - m4 * m2) * L,
		b3 = -(m3 * m8 - m6 * m5) * L,
		b4 =  (m0 * m8 - m6 * m2) * L,
		b5 = -(m0 * m5 - m3 * m2) * L;

	m_array [0] = b0;
	m_array [1] = b1;
	m_array [2] = b2;
	m_array [3] = b3;
	m_array [4] = b4;
	m_array [5] = b5;
	m_array [6] =  (t10 - t14) * L;
	m_array [7] = -(t6 - t12) * L;
	m_array [8] =  (t4 - t8) * L;
}

template <class Type>
inline
matrix3 <Type> &
matrix3 <Type>::operator += (const matrix3 & matrix)
{
	m_matrix += matrix .value ();
	return *this;
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::operator += (const Type & t)
{
	m_matrix [0] += t;
	m_matrix [1] += t;
	m_matrix [2] += t;

	return *this;
}

template <class Type>
inline
matrix3 <Type> &
matrix3 <Type>::operator -= (const matrix3 & matrix)
{
	m_matrix -= matrix .value ();
	return *this;
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::operator -= (const Type & t)
{
	m_matrix [0] -= t;
	m_matrix [1] -= t;
	m_matrix [2] -= t;

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
matrix3 <Type>::operator *= (const Type & t)
{
	m_matrix [0] *= t;
	m_matrix [1] *= t;
	m_matrix [2] *= t;

	return *this;
}

template <class Type>
matrix3 <Type> &
matrix3 <Type>::operator /= (const Type & t)
{
	m_matrix [0] /= t;
	m_matrix [1] /= t;
	m_matrix [2] /= t;

	return *this;
}

template <class Type>
void
matrix3 <Type>::mult_left (const matrix3 <Type> & matrix)
{
	#define MULT_LEFT(i, j) \
	   (m_matrix [0] [j] * matrix .m_matrix [i] [0] +   \
	    m_matrix [1] [j] * matrix .m_matrix [i] [1] +   \
	    m_matrix [2] [j] * matrix .m_matrix [i] [2])

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
	   (m_matrix [i] [0] * matrix .m_matrix [0] [j] +   \
	    m_matrix [i] [1] * matrix .m_matrix [1] [j] +   \
	    m_matrix [i] [2] * matrix .m_matrix [2] [j])

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
	const Type w = vector .x () * m_array [2] + vector .y () * m_array [5] + m_array [8];

	return vector2 <Type> ((vector .x () * m_array [0] + vector .y () * m_array [3] + m_array [6]) / w,
	                       (vector .x () * m_array [1] + vector .y () * m_array [4] + m_array [7]) / w);
}

template <class Type>
constexpr
vector3 <Type>
matrix3 <Type>::mult_vec_matrix (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * m_array [0] + vector .y () * m_array [3] + vector .z () * m_array [6],
	                       vector .x () * m_array [1] + vector .y () * m_array [4] + vector .z () * m_array [7],
	                       vector .x () * m_array [2] + vector .y () * m_array [5] + vector .z () * m_array [8]);
}

template <class Type>
vector2 <Type>
matrix3 <Type>::mult_matrix_vec (const vector2 <Type> & vector) const
{
	const Type w = vector .x () * m_array [6] + vector .y () * m_array [7] + m_array [8];

	return vector2 <Type> ((vector .x () * m_array [0] + vector .y () * m_array [1] + m_array [2]) / w,
	                       (vector .x () * m_array [3] + vector .y () * m_array [4] + m_array [5]) / w);
}

template <class Type>
constexpr
vector3 <Type>
matrix3 <Type>::mult_matrix_vec (const vector3 <Type> & vector) const
{
	return vector3 <Type> (vector .x () * m_array [0] + vector .y () * m_array [1] + vector .z () * m_array [2],
	                       vector .x () * m_array [3] + vector .y () * m_array [4] + vector .z () * m_array [5],
	                       vector .x () * m_array [6] + vector .y () * m_array [7] + vector .z () * m_array [8]);
}

template <class Type>
constexpr
vector2 <Type>
matrix3 <Type>::mult_dir_matrix (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * m_array [0] + vector .y () * m_array [3],
	                       vector .x () * m_array [1] + vector .y () * m_array [4]);
}

template <class Type>
constexpr
vector2 <Type>
matrix3 <Type>::mult_matrix_dir (const vector2 <Type> & vector) const
{
	return vector2 <Type> (vector .x () * m_array [0] + vector .y () * m_array [1],
	                       vector .x () * m_array [3] + vector .y () * m_array [4]);
}

template <class Type>
void
matrix3 <Type>::translate (const vector2 <Type> & translation)
{
	#define TRANSLATE(i) \
	   (m_matrix [0] [i] * translation .x () +   \
	    m_matrix [1] [i] * translation .y ())

	m_matrix [2] [0] += TRANSLATE (0);
	m_matrix [2] [1] += TRANSLATE (1);

	#undef TRANSLATE
}

template <class Type>
void
matrix3 <Type>::rotate (const Type & rotation)
{
	matrix3 <Type> r;

	r .rotation (rotation);

	mult_left (r);
}

template <class Type>
void
matrix3 <Type>::scale (const vector2 <Type> & scaleFactor)
{
	m_matrix [0] [0] *= scaleFactor .x ();
	m_matrix [1] [0] *= scaleFactor .y ();

	m_matrix [0] [1] *= scaleFactor .x ();
	m_matrix [1] [1] *= scaleFactor .y ();
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
	return lhs .value () == rhs .value ();
}

///  Compares two matrix3 numbers.
///  Return true if @a a is not equal to @a b.
template <class Type>
inline
constexpr
bool
operator not_eq (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .value () not_eq rhs .value ();
}

///  Lexicographically compares two matrix3 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return lhs .value () < rhs .value ();;
}

///  Lexicographically compares two matrix3 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two matrix3 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two matrix3 numbers.
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

///  Returns new matrix value @a lhs plus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator + (const matrix3 <Type> & lhs, const Type & rhs)
{
	return matrix3 <Type> (lhs) += rhs;
}

///  Return matrix value @a lhs right multiplied by scalar @a rhs.
template <class Type>
inline
matrix3 <Type>
operator + (const Type & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (rhs) += lhs;
}

///  Returns new matrix value @a a minus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator - (const matrix3 <Type> & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator - (const matrix3 <Type> & lhs, const Type & rhs)
{
	return matrix3 <Type> (lhs) -= rhs;
}

///  Returns new matrix value @a lhs minus @a rhs.
template <class Type>
inline
matrix3 <Type>
operator - (const Type & lhs, const matrix3 <Type> & rhs)
{
	return matrix3 <Type> (-rhs) += lhs;
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

///  Return @a matrix normalized, so that there are no axes with zero length.
template <class Type>
matrix3 <Type> 
normalize (const matrix3 <Type> & matrix)
{
	auto x = matrix .x_axis ();
	auto y = matrix .y_axis ();

	if (norm (x) == 0)
	{
		if (norm (y))
			x = vector2 <Type> (y .y (), -y .x ());
		else
			x = vector2 <Type> (1, 0);

		x .normalize ();
	}

	if (norm (y) == 0)
	{
		if (norm (x))
			y = vector2 <Type> (-x .y (), x .x ());
		else
			y = vector2 <Type> (0, 1);

		y .normalize ();
	}

	const auto o = matrix .origin ();

	return matrix3 <Type> (x .x (), x .y (), 0,
	                       y .x (), y .y (), 0,
	                       o .x (), o .y (), 1);
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
	return ostream << matrix .value ();
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

namespace std {

/// Provides access to the number of elements in an matrix3 as a compile-time constant expression. 
template< class Type>
class tuple_size <titania::math::matrix3 <Type>> :
    public integral_constant <size_t, titania::math::matrix3 <Type> () .size ()>
{ };

/// Provides compile-time indexed access to the type of the elements of the matrix3 using tuple-like interface.
template <std::size_t I, class Type>
struct tuple_element <I, titania::math::matrix3 <Type>>
{
	using type = typename titania::math::matrix3 <Type>::value_type;
};

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
typename titania::math::matrix3 <Type>::value_type &
get (titania::math::matrix3 <Type> & matrix)
{
	return matrix [Index];
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
const typename titania::math::matrix3 <Type>::value_type &
get (const titania::math::matrix3 <Type> & matrix)
{
	return matrix [Index];
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
typename titania::math::matrix3 <Type>::value_type &&
get (titania::math::matrix3 <Type> && matrix)
{
	return std::move (matrix [Index]);
}

///  Extracts the Ith element element from the matrix.
template <size_t Index, class Type>
inline
constexpr
const typename titania::math::matrix3 <Type>::value_type &&
get (const titania::math::matrix3 <Type> && matrix)
{
	return std::move (matrix [Index]);
}

/// Specializes the std::swap algorithm for matrix3.
template <class Type>
inline
void
swap (titania::math::matrix3 <Type> & lhs, titania::math::matrix3 <Type> & rhs) noexcept
{
	lhs .swap (rhs);
}

} // std

#endif
