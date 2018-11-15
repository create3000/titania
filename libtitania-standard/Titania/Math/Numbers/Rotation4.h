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

#include "../../Basic/SubscriptIterator.h"
#include "../../Utility/MemberValue.h"
#include "../Functional.h"
#include "Quaternion.h"
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
private:

	static constexpr size_t Size = 4;


public:

	///  Type.
	using value_type = Type;

	///  Size typedef.  Used for size and indices.
	using size_type = std::size_t;

	///  std::ptrdiff_t
	using difference_type = std::ptrdiff_t;

	///  value_type &
	using reference = basic::member_value <Type>;

	///  const value_type &
	using const_reference = const Type &;

	///  value_type*
	using pointer = value_type*;

	///  const value_type*
	using const_pointer = const value_type*;

	///  Random access iterator
	using iterator = basic::subscript_iterator <rotation4>;

	///  Constant random access iterator 
	using const_iterator = const basic::subscript_iterator <rotation4>;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = std::reverse_iterator <iterator>;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = const std::reverse_iterator <iterator>;

	///  vector3 <Type>
	using vector_type = vector3 <Type>;

	///  @name Constructors

	///  Default constructor.
	///  The rotation will be set to its default value 0 0 1  0.
	constexpr
	rotation4 () :
		    m_x (0),
		    m_y (0),
		    m_z (1),
		m_angle (0),
		 m_quat ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	rotation4 (const rotation4 <Up> & rotation) :
		    m_x (rotation .x ()),
		    m_y (rotation .y ()),
		    m_z (rotation .z ()),
		m_angle (rotation .angle ()),
		 m_quat (rotation .quat ())
	{ }

	///  Construct a rotation from normalized @a quaternion.
	template <class Up>
	explicit
	constexpr
	rotation4 (const quaternion <Up> & quaternion) :
		    m_x (0),
		    m_y (0),
		    m_z (1),
		m_angle (0),
		 m_quat (quaternion)
	{ update (); }

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

	///  Construct a rotation from @a matrix.
	template <class Up>
	explicit
	rotation4 (const matrix3 <Up> & matrix)
	{ this -> matrix (matrix); }

	///  @name Assignment operator

	///  Assign @a rotation to this rotation.
	template <class Up>
	rotation4 &
	operator = (const rotation4 <Up> & other);

	///  Assign rotation @a matrix to this rotation.
	template <class Up>
	rotation4 &
	operator = (const matrix3 <Up> & matrix)
	{
		this -> matrix (matrix);
		return *this;
	}

	///  @name Element access

	///  Set x axis of this rotation.
	void
	x (const Type & value)
	{ *this = rotation4 (value, y (), z (), angle ()); }

	///  Returns x axis of this rotation.
	Type
	x () const
	{ return m_x; }

	///  Set y axis of this rotation.
	void
	y (const Type & value)
	{ *this = rotation4 (x (), value, z (), angle ()); }

	///  Returns y axis of this rotation.
	Type
	y () const
	{ return m_y; }

	///  Set z axis of this rotation.
	void
	z (const Type & value)
	{ *this = rotation4 (x (), y (), value, angle ()); }

	///  Returns z axis of this rotation.
	Type
	z () const
	{ return m_z; }

	///  Set @a axis of this rotation from vector.
	void
	axis (const vector3 <Type> & value)
	{ *this = rotation4 (value, angle ()); }

	///  Returns axis of this rotation.
	vector3 <Type>
	axis () const
	{ return vector3 <Type> (x (), y (), z ()); }

	///  Set angle of this rotation.
	void
	angle (const Type & value)
	{ *this = rotation4 (x (), y (), z (), value); }

	///  Returns angle of this rotation.
	Type
	angle () const
	{ return m_angle; }

	///  Set quaternion of this rotation.
	void
	quat (const quaternion <Type> & quat)
	{
		m_quat = quat;
		update ();
	}

	///  Returns quaternion of this rotation.
	const quaternion <Type> &
	quat () const
	{ return m_quat; }

	///  Assign rotation @a matrix to this rotation.
	template <class Up>
	void
	matrix (const matrix3 <Up> & matrix)
	{
		m_quat .matrix (matrix);
		m_quat .normalize ();
		update ();
	}

	///  Returns the 3x3 rotation matrix of this rotation.
	matrix3 <Type>
	matrix () const
	{ return m_quat .matrix (); }

	///  Set @a x, @a y, @a z and @a angle componentwise.
	void
	set (const Type & x, const Type & y, const Type & z, const Type & angle)
	{ *this = rotation4 (x, y, z, angle); }

	///  Access specified element with bounds checking.
	constexpr
	reference
	at (const size_type index)
	{
		if (index < 4)
			return (*this) [index];

		throw std::out_of_range ("rotation4::at index > 3");
	}

	///  Access specified element with bounds checking.
	constexpr
	Type
	at (const size_type index) const
	{
		if (index < 4)
			return (*this) [index];

		throw std::out_of_range ("rotation4::at index > 3");
	}

	///  Access x, y, z or angle.
	reference
	operator [ ] (const size_type index);

	///  Access x, y, z or angle.
	Type
	operator [ ] (const size_type index) const;

	///  Returns a reference to the first element in the container. 
	constexpr
	reference
	front ()
	{ return (*this) [0]; }

	///  Returns a reference to the first element in the container. 
	constexpr
	Type
	front () const
	{ return (*this) [0]; }

	///  Returns reference to the last element in the container. 
	constexpr
	reference
	back ()
	{ return (*this) [3]; }

	///  Returns reference to the last element in the container. 
	constexpr
	Type
	back () const
	{ return (*this) [3]; }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	constexpr
	iterator
	begin ()
	{ return iterator (this, 0); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	begin () const
	{ return const_iterator (const_cast <rotation4*> (this), 0); }

	///  Returns an iterator to the beginning.
	constexpr
	const_iterator
	cbegin () const
	{ return const_iterator (const_cast <rotation4*> (this), 0); }

	///  Returns an iterator to the end.
	constexpr
	iterator
	end ()
	{ return iterator (this, 4); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	end () const
	{ return const_iterator (const_cast <rotation4*> (this), 4); }

	///  Returns an iterator to the end.
	constexpr
	const_iterator
	cend () const
	{ return const_iterator (const_cast <rotation4*> (this), 4); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	reverse_iterator
	rbegin ()
	{ return reverse_iterator (end ()); }

	///  returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	rbegin () const
	{ return const_reverse_iterator (end ()); }

	///  Returns a reverse iterator to the beginning.
	constexpr
	const_reverse_iterator
	crbegin () const
	{ return const_reverse_iterator (cend ()); }

	///  Returns a reverse iterator to the end.
	constexpr
	reverse_iterator
	rend ()
	{ return reverse_iterator (begin ()); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	rend () const
	{ return const_reverse_iterator (begin ()); }

	///  Returns a reverse iterator to the end.
	constexpr
	const_reverse_iterator
	crend () const
	{ return const_reverse_iterator (cbegin ()); }

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

	///  Swaps the contents.
	void
	swap (rotation4 & other)
	{
		std::swap (m_x,     other .m_x);
		std::swap (m_y,     other .m_y);
		std::swap (m_z,     other .m_z);
		std::swap (m_angle, other .m_angle);

		m_quat .swap (other .m_quat);
	}

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

	///  Normalize this rotation in place.
	void
	normalize ();


private:

	///  @name Operations

	void
	update ();

	///  @name Members

	Type              m_x;
	Type              m_y;
	Type              m_z;
	Type              m_angle;
	quaternion <Type> m_quat;

};

template <class Type>
rotation4 <Type>::rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle) :
	    m_x (x),
	    m_y (y),
	    m_z (z),
	m_angle (angle),
	 m_quat ()
{
	Type scale = std::sqrt (x * x + y * y + z * z);

	if (scale == 0)
	{
		m_quat = quaternion <Type> ();
		return;
	}

	// Calculate quaternion

	const Type halfTheta = interval <Type> (angle / 2, 0, pi <Type>);

	scale = std::sin (halfTheta) / scale;

	m_quat = quaternion <Type> (x * scale,
	                            y * scale,
	                            z * scale,
	                            std::cos (halfTheta));
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

	const vector3 <Type> from (math::normalize (fromVector));
	const vector3 <Type> to (math::normalize (toVector));

	const Type     cos_angle = std::clamp <Type> (dot (from, to), -1, 1);
	vector3 <Type> crossvec  = math::normalize (cross (from, to));
	const Type     crosslen  = abs (crossvec);

	if (crosslen == 0)
	{
		// Parallel vectors
		// Check if they are pointing in the same direction.
		if (cos_angle > 0)
		{
			// standard rotation
		}
		// Ok, so they are parallel and pointing in the opposite direction
		// of each other.
		else
		{
			// Try crossing with x axis.
			auto t = cross (from, vector3 <Type> (1, 0, 0));

			// If not ok, cross with y axis.
			if (norm (t) == 0)
				t = cross (from, vector3 <Type> (0, 1, 0));

			t .normalize ();

			m_quat = quaternion <Type> (t [0], t [1], t [2], 0);
		}
	}
	else
	{
		// Vectors are not parallel
		// The abs () wrapping is to avoid problems when `dot' "overflows" a tiny wee bit,
		// which can lead to sqrt () returning NaN.
		crossvec *= std::sqrt (std::abs (1 - cos_angle) / 2);
		m_quat   = quaternion <Type> (crossvec [0], crossvec [1], crossvec [2], std::sqrt (std::abs (1 + cos_angle) / 2));
	}

	update ();
}

template <class Type>
template <class Up>
rotation4 <Type> &
rotation4 <Type>::operator = (const rotation4 <Up> & other)
{
	m_x     = other .x ();
	m_y     = other .y ();
	m_z     = other .z ();
	m_angle = other .angle ();
	m_quat  = other .quat ();
	return *this;
}

///  Access components by @a index.
template <class Type>
typename rotation4 <Type>::reference
rotation4 <Type>::operator [ ] (const size_type index)
{
	using namespace std::placeholders;

	using Getter = Type (rotation4::*) () const;
	using Setter = void (rotation4::*) (const Type &);

	switch (index)
	{
		case 0: return reference (std::bind ((Setter) &rotation4::x,     this, _1), std::bind ((Getter) &rotation4::x,     this));
		case 1: return reference (std::bind ((Setter) &rotation4::y,     this, _1), std::bind ((Getter) &rotation4::y,     this));
		case 2: return reference (std::bind ((Setter) &rotation4::z,     this, _1), std::bind ((Getter) &rotation4::z,     this));
		case 3: return reference (std::bind ((Setter) &rotation4::angle, this, _1), std::bind ((Getter) &rotation4::angle, this));
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

template <class Type>
void
rotation4 <Type>::update ()
{
	if (std::abs (m_quat .w ()) > 1)
	{
		m_x     = 0;
		m_y     = 0;
		m_z     = 1;
		m_angle = 0;
	}
	else
	{
		const auto angle = std::acos (m_quat .w ()) * 2;
		const auto scale = std::sin (angle / 2);

		if (scale == 0)
		{
			m_x     = 0;
			m_y     = 0;
			m_z     = 1;
			m_angle = 0;
		}
		else
		{
			const auto axis = imag (m_quat) / scale;
	
			m_x     = axis .x ();
			m_y     = axis .y ();
			m_z     = axis .z ();
			m_angle = angle;
		}
	}
}

template <class Type>
inline
void
rotation4 <Type>::inverse ()
{
	m_quat .inverse ();
	update ();
}

template <class Type>
inline
rotation4 <Type> &
rotation4 <Type>::operator *= (const rotation4 & rotation)
{
	m_quat .mult_right (rotation .quat ());
	update ();
	return *this;
}

template <class Type>
inline
void
rotation4 <Type>::mult_left (const rotation4 & rotation)
{
	m_quat .mult_left (rotation .quat ());
	m_quat .normalize ();
	update ();
}

template <class Type>
inline
void
rotation4 <Type>::mult_right (const rotation4 & rotation)
{
	m_quat .mult_right (rotation .quat ());
	m_quat .normalize ();
	update ();
}

template <class Type>
inline
vector3 <Type>
rotation4 <Type>::mult_vec_rot (const vector3 <Type> & vector) const
{
	return m_quat .mult_vec_quat (vector);
}

template <class Type>
inline
vector3 <Type>
rotation4 <Type>::mult_rot_vec (const vector3 <Type> & vector) const
{
	return m_quat .mult_quat_vec (vector);
}

template <class Type>
inline
void
rotation4 <Type>::normalize ()
{
	m_quat .normalize ();
	update ();
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

///  Returns @a rotation4 normalized.
template <class Type>
inline
rotation4 <Type>
normalize (const rotation4 <Type> & rotation)
{
	rotation4 <Type> result (rotation);
	result .normalize ();
	return result;
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
	return ostream
	       << rotation .x () << ' '
	       << rotation .y () << ' '
	       << rotation .z () << ' '
	       << rotation .angle ();
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

namespace std {

/// Provides access to the number of elements in an rotation4 as a compile-time constant expression. 
template< class Type>
class tuple_size <titania::math::rotation4 <Type>> :
    public integral_constant <size_t, titania::math::rotation4 <Type> () .size ()>
{ };

/// Provides compile-time indexed access to the type of the elements of the rotation4 using tuple-like interface.
template <std::size_t I, class Type>
struct tuple_element <I, titania::math::rotation4 <Type>>
{
	using type = Type;
};

///  Extracts the Ith element element from the rotation.
template <size_t I, class Type>
inline
constexpr
typename titania::math::rotation4 <Type>::reference
get (titania::math::rotation4 <Type> & rotation)
{
	return rotation [I];
}

///  Extracts the Ith element element from the rotation.
template <size_t I, class Type>
inline
constexpr
Type
get (const titania::math::rotation4 <Type> & rotation)
{
	return std::move (rotation [I]);
}

/////  Extracts the Ith element element from the rotation.
//template <size_t I, class Type>
//inline
//constexpr
//Type &&
//get (titania::math::rotation4 <Type> && rotation)
//{
//	return std::move (rotation [I]);
//}
//
/////  Extracts the Ith element element from the rotation.
//template <size_t I, class Type>
//inline
//constexpr
//const Type &&
//get (const titania::math::rotation4 <Type> && rotation)
//{
//	return rotation [I];
//}

/// Specializes the std::swap algorithm for rotation.
template <class Type>
inline
void
swap (titania::math::rotation4 <Type> & lhs, titania::math::rotation4 <Type> & rhs) noexcept
{
	lhs .swap (rhs);
}

} // std

#endif
