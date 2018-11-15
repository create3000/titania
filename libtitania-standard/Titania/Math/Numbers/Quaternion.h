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

#ifndef __TITANIA_MATH_NUMBERS_QUATERNION_H__
#define __TITANIA_MATH_NUMBERS_QUATERNION_H__

#include "../Functional.h"
#include "Vector3.h"
#include "Matrix3.h"

#include <array>
#include <cmath>
#include <istream>
#include <ostream>
#include <tuple>

namespace titania {
namespace math {

template <class Type>
class quaternion;

template <class Type>
quaternion <Type>
log (const quaternion <Type> &);

template <class Type>
quaternion <Type>
exp (const quaternion <Type> &);

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
private:

	static constexpr size_t Size = 4;

	///  @name Member types

	using array_type = std::array <Type, Size>;


public:

	///  @name Member types

	///  Type.
	using value_type = Type;

	///  Size typedef.  Used for size and indices.
	using size_type = typename array_type::size_type;

	///  std::ptrdiff_t
	using difference_type = typename array_type::difference_type;

	///  value_type &
	using reference = typename array_type::reference;

	///  const value_type &
	using const_reference = typename array_type::const_reference;

	///  value_type*
	using pointer = typename array_type::pointer;

	///  const value_type*
	using const_pointer = typename array_type::const_pointer;

	///  Random access iterator
	using iterator = typename array_type::iterator;

	///  Constant random access iterator 
	using const_iterator = typename array_type::const_iterator;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = typename array_type::reverse_iterator;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = typename array_type::const_reverse_iterator;

	///  @name Constructors

	///  Default constructor.  All values default to 0.
	constexpr
	quaternion () :
		m_array { Type (), Type (), Type (), Type (1) }
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	quaternion (const quaternion <Up> & quat) :
		m_array { quat .x (), quat .y (), quat .z (), quat .w () }
	{ }

	///  Component constructor. Set values to @a x, @a y and @a z.
	constexpr
	quaternion (const Type & x, const Type & y, const Type & z, const Type & w) :
		m_array { x, y, z, w }
	{ }

	///  Construct quaternion from vector @a imag and @a w.
	template <class Up>
	constexpr
	quaternion (const vector3 <Up> & imag, const Type & w) :
		m_array { imag .x (), imag .y (), imag .z (), w }
	{ }

	///  @name Assignment operator

	///  Assign @a other to this quaternion.
	template <class Up>
	quaternion &
	operator = (const quaternion <Up> & other);

	///  @name Element access

	///  Set x component of this quaternion.
	void
	x (const Type & value)
	{ m_array [0] = value; }

	///  Return x component of this quaternion.
	constexpr
	const_reference
	x () const
	{ return m_array [0]; }

	///  Set y component of this quaternion.
	void
	y (const Type & value)
	{ m_array [1] = value; }

	///  Return y component of this quaternion.
	constexpr
	const_reference
	y () const
	{ return m_array [1]; }

	///  Set z component of this quaternion.
	void
	z (const Type & value)
	{ m_array [2] = value; }

	///  Return z component of this quaternion.
	constexpr
	const_reference
	z () const
	{ return m_array [2]; }

	///  Set w component of this quaternion which is the real part.
	void
	w (const Type & value)
	{ m_array [3] = value; }

	///  Return w component of this quaternion which is the real part.
	constexpr
	const_reference
	w () const
	{ return m_array [3]; }

	///  Assign rotation @a matrix to this quaternion.
	template <class Up>
	void
	matrix (const matrix3 <Up> & matrix);

	///  Returns the 3x3 rotation matrix of this quaternion.
	matrix3 <Type>
	matrix () const;

	///  Access specified element with bounds checking.
	constexpr
	reference
	at (const size_type index)
	{ return m_array .at (index); }

	///  Access specified element with bounds checking.
	constexpr
	const_reference
	at (const size_type index) const
	{ return m_array .at (index); }

	///  Access components by @a index.
	constexpr
	reference
	operator [ ] (const size_type index)
	{ return m_array [index]; }

	///  Access components by @a index.
	constexpr
	const_reference
	operator [ ] (const size_type index) const
	{ return m_array [index]; }

	///  Returns a reference to the first element in the container. 
	constexpr
	reference
	front ()
	{ return m_array .front (); }

	///  Returns a reference to the first element in the container. 
	constexpr
	const_reference
	front () const
	{ return m_array .front (); }

	///  Returns reference to the last element in the container. 
	constexpr
	reference
	back ()
	{ return m_array .back (); }

	///  Returns reference to the last element in the container. 
	constexpr
	const_reference
	back () const
	{ return m_array .back (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	pointer
	data ()
	{ return m_array .data (); }

	///  Returns pointer to the underlying array serving as element storage.
	constexpr
	const_pointer
	data () const
	{ return m_array .data (); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	constexpr
	iterator
	begin ()
	{ return m_array .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	begin () const
	{ return m_array .begin (); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	cbegin () const
	{ return m_array .cbegin (); }

	///  Returns an iterator to the end.
	constexpr
	iterator
	end ()
	{ return m_array .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	end () const
	{ return m_array .end (); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	cend () const
	{ return m_array .cend (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	reverse_iterator
	rbegin ()
	{ return m_array .rbegin (); }

	///  returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	rbegin () const
	{ return m_array .rbegin (); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	crbegin () const
	{ return m_array .crbegin (); }

	///  Returns a reverse iterator to the end.
	constexpr
	reverse_iterator
	rend ()
	{ return m_array .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	rend () const
	{ return m_array .rend (); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	crend () const
	{ return m_array .crend (); }

	///  @name Capacity

	///  Checks whether the container is empty. Always returns false.
	constexpr
	bool
	empty () const
	{ return false; }

	///  Returns the number of elements in the container.
	constexpr
	size_type
	size () const
	{ return Size; }

	///  Returns the maximum possible number of elements. Because each vector is a fixed-size container,
	///  the value is also the value returned by size.
	constexpr
	size_type
	max_size () const
	{ return Size; }

	///  @name Operations

	///  Fill the container with specified @a value. 
	void
	fill (const Type & value)
	{ m_array .fill (value); }

	///  Swaps the contents.
	void
	swap (quaternion & other)
	{ m_array .swap (other .m_array); }

	///  @name  Arithmetic operations
	///  All these operators modify this quaternion inplace.

	///  Negates this quaternion in place.
	void
	negate ();

	///  Invert this quaternion in place.
	void
	inverse ();

	///  Add @a quat to this quaternion.
	quaternion &
	operator += (const quaternion & quat);

	///  Add @a t to this quaternion.
	quaternion &
	operator += (const Type & t);

	///  Subtract @a quat to this quaternion.
	quaternion &
	operator -= (const quaternion & quat);

	///  Subtract @a t from this quaternion.
	quaternion &
	operator -= (const Type & t);

	///  Multiply this quaternion by @a t.
	quaternion &
	operator *= (const Type & t);

	///  Left multiply this quaternion by @a quat.
	quaternion &
	operator *= (const quaternion & quat);

	///  Divide this quaternion by @a t.
	quaternion &
	operator /= (const Type & t);

	///  Left multiply this quaternion by @a quaternion in place.
	void
	mult_left (const quaternion & quat);

	///  Right multiply this quaternion by @a quaternion in place.
	void
	mult_right (const quaternion & quat);

	///  Returns the value of @a vector left multiplied by this quaternion.
	vector3 <Type>
	mult_vec_quat (const vector3 <Type> & vector) const;

	///  Returns the value of @a vector right multiplied by this quaternion.
	vector3 <Type>
	mult_quat_vec (const vector3 <Type> & vector) const;

	///  Normalize this quaternion in place.
	void
	normalize ();


private:

	///  @name Members

	array_type m_array;

};

template <class Type>
template <class Up>
quaternion <Type> &
quaternion <Type>::operator = (const quaternion <Up> & other)
{
	m_array [0] = other .x ();
	m_array [1] = other .y ();
	m_array [2] = other .z ();
	m_array [3] = other .w ();
	return *this;
}

template <class Type>
template <class Up>
void
quaternion <Type>::matrix (const matrix3 <Up> & matrix)
{
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
		(*this) [3] = std::sqrt (scalerow + 1) / 2;

		// And compute other values:
		const Type d = 4 * (*this) [3];
		(*this) [0] = (matrix [1] [2] - matrix [2] [1]) / d;
		(*this) [1] = (matrix [2] [0] - matrix [0] [2]) / d;
		(*this) [2] = (matrix [0] [1] - matrix [1] [0]) / d;
	}
	else
	{
		// Compute x, y, or z first:
		const int j = (i + 1) % 3;
		const int k = (i + 2) % 3;

		// Compute first value:
		(*this) [i] = std::sqrt (matrix [i] [i] - matrix [j] [j] - matrix [k] [k] + 1) / 2;

		// And the others:
		const Type d = 4 * (*this) [i];
		(*this) [j] = (matrix [i] [j] + matrix [j] [i]) / d;
		(*this) [k] = (matrix [i] [k] + matrix [k] [i]) / d;
		(*this) [3] = (matrix [j] [k] - matrix [k] [j]) / d;
	}
}

///  Convert this rotation to a matrix3.
template <class Type>
matrix3 <Type>
quaternion <Type>::matrix () const
{
	const auto x = this -> x ();
	const auto y = this -> y ();
	const auto z = this -> z ();
	const auto w = this -> w ();

	const auto a = x * x;
	const auto b = x * y;
	const auto c = y * y;
	const auto d = y * z;
	const auto e = z * x;
	const auto f = z * z;
	const auto g = w * x;
	const auto h = w * y;
	const auto i = w * z;

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
quaternion <Type>::negate ()
{
	m_array [0] = -m_array [0];
	m_array [1] = -m_array [1];
	m_array [2] = -m_array [2];
	m_array [3] = -m_array [3];
}

template <class Type>
void
quaternion <Type>::inverse ()
{
	m_array [0] = -m_array [0];
	m_array [1] = -m_array [1];
	m_array [2] = -m_array [2];
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator += (const quaternion & quat)
{
	m_array [0] += quat .x ();
	m_array [1] += quat .y ();
	m_array [2] += quat .z ();
	m_array [3] += quat .w ();
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator += (const Type & t)
{
	m_array [0] += t;
	m_array [1] += t;
	m_array [2] += t;
	m_array [3] += t;
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator -= (const quaternion & quat)
{
	m_array [0] -= quat .x ();
	m_array [1] -= quat .y ();
	m_array [2] -= quat .z ();
	m_array [3] -= quat .w ();
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator -= (const Type & t)
{
	m_array [0] -= t;
	m_array [1] -= t;
	m_array [2] -= t;
	m_array [3] -= t;
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator *= (const Type & t)
{
	m_array [0] *= t;
	m_array [1] *= t;
	m_array [2] *= t;
	m_array [3] *= t;
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator *= (const quaternion & quat)
{
	mult_right (quat);
	return *this;
}

template <class Type>
quaternion <Type> &
quaternion <Type>::operator /= (const Type & t)
{
	m_array [0] /= t;
	m_array [1] /= t;
	m_array [2] /= t;
	m_array [3] /= t;
	return *this;
}

template <class Type>
void
quaternion <Type>::mult_left (const quaternion & quat)
{
	*this = quaternion (w () * quat .x () +
	                    x () * quat .w () +
	                    y () * quat .z () -
	                    z () * quat .y (),

	                    w () * quat .y () +
	                    y () * quat .w () +
	                    z () * quat .x () -
	                    x () * quat .z (),

	                    w () * quat .z () +
	                    z () * quat .w () +
	                    x () * quat .y () -
	                    y () * quat .x (),

	                    w () * quat .w () -
	                    x () * quat .x () -
	                    y () * quat .y () -
	                    z () * quat .z ());
}

template <class Type>
void
quaternion <Type>::mult_right (const quaternion & quat)
{
	*this = quaternion (quat .w () * x () +
	                    quat .x () * w () +
	                    quat .y () * z () -
	                    quat .z () * y (),

	                    quat .w () * y () +
	                    quat .y () * w () +
	                    quat .z () * x () -
	                    quat .x () * z (),

	                    quat .w () * z () +
	                    quat .z () * w () +
	                    quat .x () * y () -
	                    quat .y () * x (),

	                    quat .w () * w () -
	                    quat .x () * x () -
	                    quat .y () * y () -
	                    quat .z () * z ());
}

///  Returns new vector value @a vector left multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
quaternion <Type>::mult_vec_quat (const vector3 <Type> & vector) const
{
	//return imag (~*this * quaternion <Type> (vector, Type ()) * *this);

	const Type a = w () * w () - x () * x () - y () * y () - z () * z ();                     
	const Type b = 2 * (vector .x () * x () + vector .y () * y () + vector .z () * z ());  
	const Type c = 2 * w ();                                       

	return vector3 <Type> (a * vector .x () + b * x () + c * (y () * vector .z () - z () * vector .y ()),
	                       a * vector .y () + b * y () + c * (z () * vector .x () - x () * vector .z ()),
	                       a * vector .z () + b * z () + c * (x () * vector .y () - y () * vector .x ()));
}

///  Returns new vector value @a vector right multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
quaternion <Type>::mult_quat_vec (const vector3 <Type> & vector) const
{
	//return imag (*this * quaternion <Type> (vector, Type ()) * ~*this);

	const Type a = w () * w () - x () * x () - y () * y () - z () * z ();                     
	const Type b = 2 * (vector .x () * x () + vector .y () * y () + vector .z () * z ());  
	const Type c = 2 * w ();                                       

	return vector3 <Type> (a * vector .x () + b * x () - c * (y () * vector .z () - z () * vector .y ()),
	                       a * vector .y () + b * y () - c * (z () * vector .x () - x () * vector .z ()),
	                       a * vector .z () + b * z () - c * (x () * vector .y () - y () * vector .x ()));
}

template <class Type>
void
quaternion <Type>::normalize ()
{
	const Type length = abs (*this);

	if (length)
		*this /= length;
}

///  @name Element access
///  @relates quaternion

///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
const Type &
real (const quaternion <Type> & quat)
{
	return quat .w ();
}

///  Returns imaginary part of @a quaternion as vector.
template <class Type>
inline
constexpr
vector3 <Type>
imag (const quaternion <Type> & quat)
{
	return vector3 <Type> (quat .x (), quat .y (), quat .z ());
}

///  @name Comparision operations
///  @relates quaternion

///  Compares two quaternion numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator == (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () == rhs .x () and
	       lhs .y () == rhs .y () and
	       lhs .z () == rhs .z () and
	       lhs .w () == rhs .w ();
}

///  Compares two quaternion numbers.
///  Returns false if @a lhs is not equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator not_eq (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () not_eq rhs .x () or
	       lhs .y () not_eq rhs .y () or
	       lhs .z () not_eq rhs .z () or
	       lhs .w () not_eq rhs .w ();
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return std::tie (lhs .x (), lhs .y (), lhs .z (), lhs .w ()) < std::tie (rhs .x (), rhs .y (), rhs .z (), rhs .w ());
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two quaternion numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates quaternion
///  @name Arithmetic operations

///  Returns a copy of @a quaternion.
template <class Type>
inline
constexpr
Type
operator + (const quaternion <Type> & quat)
{
	return quat;
}

///  Returns quaternion negation of @a quaternion.
template <class Type>
inline
quaternion <Type>
operator - (const quaternion <Type> & quat)
{
	quaternion <Type> result (quat);
	result .negate ();
	return result;
}

///  Returns the inverse quaternion for @a quaternion.
template <class Type>
inline
quaternion <Type>
operator ~ (const quaternion <Type> & quat)
{
	quaternion <Type> result (quat);
	result .inverse ();
	return result;
}

///  Returns new quaternion value @a lhs plus @a rhs.
template <class Type>
inline
quaternion <Type>
operator + (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs) += rhs;
}

///  Returns new quaternion value @a lhs plus @a rhs.
template <class Type>
inline
constexpr
quaternion <Type>
operator + (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) += rhs;
}

///  Returns new quaternion value @a lhs plus @a rhs.
template <class Type>
inline
constexpr
quaternion <Type>
operator + (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (rhs) += lhs;
}

///  Returns new quaternion value @a lhs minus @a rhs.
template <class Type>
inline
quaternion <Type>
operator - (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs) -= rhs;
}

///  Returns new quaternion value @a lhs minus @a rhs.
template <class Type>
inline
constexpr
quaternion <Type>
operator - (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) -= rhs;
}

///  Returns new quaternion value @a lhs minus @a rhs.
template <class Type>
inline
quaternion <Type>
operator - (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (-rhs) += lhs;
}

///  Returns new quaternion value @a lhs left multiplied by @a rhs.
template <class Type>
inline
quaternion <Type>
operator * (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	quaternion <Type> result (lhs);
	result .mult_right (rhs);
	return result;
}

///  Returns new quaternion value @a lhs right multiplied @a rhs.
template <class Type>
inline
quaternion <Type>
operator * (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) *= rhs;
}

///  Returns new quaternion value @a lhs times @a rhs.
template <class Type>
inline
quaternion <Type>
operator * (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (rhs) *= lhs;
}

///  Returns the value of @a vector left multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & vector, const quaternion <Type> & quat)
{
	return quat .mult_vec_quat (vector);
}

///  Returns the value of @a vector right multiplied by @a quaternion.
template <class Type>
inline
vector3 <Type>
operator * (const quaternion <Type> & quat, const vector3 <Type> & vector)
{
	return quat .mult_quat_vec (vector);
}

//template <class Type>
//constexpr quaternion <Type>
//operator / (const quaternion <Type> & rhs, const quaternion <Type> & lhs)
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

///  Returns new quaternion value @a lhs divided by @a rhs.
template <class Type>
inline
quaternion <Type>
operator / (const quaternion <Type> & lhs, const Type & rhs)
{
	return quaternion <Type> (lhs) /= rhs;
}

///  Returns new quaternion value @a lhs divided by @a rhs.
template <class Type>
quaternion <Type>
operator / (const Type & lhs, const quaternion <Type> & rhs)
{
	return quaternion <Type> (lhs / rhs .x (),
	                          lhs / rhs .y (),
	                          lhs / rhs .z (),
	                          lhs / rhs .w ());
}

///  Returns new quaternion value @a lhs dot @a rhs.
template <class Type>
constexpr
Type
dot (const quaternion <Type> & lhs, const quaternion <Type> & rhs)
{
	return lhs .x () * rhs .x () +
	       lhs .y () * rhs .y () +
	       lhs .z () * rhs .z () +
	       lhs .w () * rhs .w ();
}

///  Returns whether @a quaternion is pure real.
template <class Type>
inline
constexpr
Type
is_real (const quaternion <Type> & quat)
{
	return not (quat .x () or quat .y () or quat .z ());
}

///  Returns whether @a quaternion is pure imaginary.
template <class Type>
inline
constexpr
Type
is_imag (const quaternion <Type> & quat)
{
	return not (quat .w ());
}

///  Returns @a quaternion magnitude.
template <class Type>
inline
constexpr
Type
abs (const quaternion <Type> & quat)
{
	return std::sqrt (norm (quat));
}

///  Returns @a quaternion magnitude squared.
template <class Type>
inline
constexpr
Type
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
	quaternion <Type> result (quat);
	result .normalize ();
	return result;
}

///  Raise @a quaternion to @a quaternion power.
template <class Type>
inline
quaternion <Type>
pow (const quaternion <Type> & base, const quaternion <Type> & exponent)
{
	// For quaternion^quaternion, we use exp and log.
	// b^x = e^{x*ln b}
	return exp (exponent * log (base));
}

///  Raise @a quaternion to @a scalar power.
///  http://cpansearch.perl.org/src/JCHIN/Math-Quaternion-0.07/lib/Math/Quaternion.pm
///  XXX: not testet yet
template <class Type>
quaternion <Type>
pow (const quaternion <Type> & base, const Type & exponent)
{
	if (is_real (base))
		return quaternion <Type> (0, 0, 0, std::pow (base .w (), exponent));

	const Type l     = abs (base);
	const Type theta = std::acos (base .w () / l);
	const Type li    = abs (imag (base));
	const Type ltoe  = std::pow (l, exponent);
	const Type et    = exponent * theta;
	const Type scale = ltoe / li * std::sin (et);

	return quaternion <Type> (base .x () * scale,
	                          base .y () * scale,
	                          base .z () * scale,
	                          ltoe * std::cos (et));
}

///  Returns the logarithm of its argument.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
quaternion <Type>
log (const quaternion <Type> & quat)
{
	if (is_real (quat))
	{
		if (quat .w () > 0)
			return quaternion <Type> (0, 0, 0, std::log (quat .w ()));

		else
			return quaternion <Type> (pi <Type>, 0, 0, std::log (-quat .w ()));
	}

	const Type l = abs (quat);
	const auto v = normalize (imag (quat)) * std::acos (quat .w () / l);
	const Type w = std::log (l);

	return quaternion <Type> (v .x (), v .y (), v .z (), w);
}

///  Exponential operator e^q.
///  http://de.wikipedia.org/wiki/Quaternion
template <class Type>
quaternion <Type>
exp (const quaternion <Type> & quat)
{
	if (is_real (quat))
		return quaternion <Type> (0, 0, 0, std::exp (quat .w ()));

	const auto i  = imag (quat);
	const Type li = abs (i);
	const Type ew = std::exp (quat .w ());
	const Type w  = ew * std::cos (li);
	const auto v  = i * (ew * std::sin (li) / li);

	return quaternion <Type> (v .x (), v .y (), v .z (), w);
}

///  Spherical cubic interpolation of @a source, @a a, @a b and @a destination by an amout of @a t.
template <class Type>
inline
quaternion <Type>
squad (const quaternion <Type> & source,
       const quaternion <Type> & a,
       const quaternion <Type> & b,
       const quaternion <Type> & destination,
       const Type & t)
{
	// We must use shortest path slerp to prevent flipping.  Also see spline.

	return slerp (slerp (source, destination, t), slerp (a, b, t), 2 * t * (1 - t));
}

///  Shoemake-Bezier interpolation using De Castlejau algorithm.
template <class Type>
quaternion <Type>
bezier (const quaternion <Type> & q0,
        const quaternion <Type> & a,
        const quaternion <Type> & b,
        const quaternion <Type> & q1,
        Type t)
{
	const quaternion <Type> q11 = slerp (q0, a, t);
	const quaternion <Type> q12 = slerp (a, b, t);
	const quaternion <Type> q13 = slerp (b, q1, t);

	return slerp (slerp (q11, q12, t), slerp (q12, q13, t), t);
}

/// Given 3 quaternions, qn-1, qn and qn+1, calculate a control point for qn to be used in squat interpolation.
template <class Type>
inline
quaternion <Type>
spline (quaternion <Type> q0,
        const quaternion <Type> & q1,
        quaternion <Type> q2)
{
	// If the dot product is smaller than 0 we must negate the quaternion to prevent flipping. If we negate all
	// the terms we get a different quaternion but it represents the same rotation.

	if (dot (q0, q1) < 0)
		q0 .negate ();

	if (dot (q2, q1) < 0)
		q2 .negate ();

	const quaternion <Type> q1_i = ~q1;

	// The result must be normalized as it will be used in slerp and we can only slerp normalized vectors.

	return normalize (q1 * exp ((log (q1_i * q0) + log (q1_i * q2)) / Type (-4)));
}

///  @relates quaternion
///  @name Input/Output operations

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

namespace std {

/// Provides access to the number of elements in an quaternion as a compile-time constant expression. 
template< class Type>
class tuple_size <titania::math::quaternion <Type>> :
    public integral_constant <size_t, titania::math::quaternion <Type> () .size ()>
{ };

/// Provides compile-time indexed access to the type of the elements of the quaternion using tuple-like interface.
template <std::size_t I, class Type>
struct tuple_element <I, titania::math::quaternion <Type>>
{
	using type = Type;
};

///  Extracts the Ith element element from the quaternion.
template <size_t I, class Type>
inline
constexpr
Type &
get (titania::math::quaternion <Type> & quaternion)
{
	return quaternion [I];
}

///  Extracts the Ith element element from the quaternion.
template <size_t I, class Type>
inline
constexpr
const Type &
get (const titania::math::quaternion <Type> & quaternion)
{
	return quaternion [I];
}

///  Extracts the Ith element element from the quaternion.
template <size_t I, class Type>
inline
constexpr
Type &&
get (titania::math::quaternion <Type> && quaternion)
{
	return std::move (quaternion [I]);
}

///  Extracts the Ith element element from the quaternion.
template <size_t I, class Type>
inline
constexpr
const Type &&
get (const titania::math::quaternion <Type> && quaternion)
{
	return std::move (quaternion [I]);
}

/// Specializes the std::swap algorithm for quaternion.
template <class Type>
inline
void
swap (titania::math::quaternion <Type> & lhs, titania::math::quaternion <Type> & rhs) noexcept
{
	lhs .swap (rhs);
}

} // std

#endif
