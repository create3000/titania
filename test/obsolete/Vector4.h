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

#ifndef __OBSOLETE_VECTOR4_H__
#define __OBSOLETE_VECTOR4_H__

#include <cmath>
#include <exception>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>

//#include "../Basic/BasicValue.h"

namespace titania {
namespace Obsolete {

template <typename Type>
class Vector4
//:	public BasicValue
{
public:

	typedef Type   value_type;
	typedef size_t size_type;

	Vector4 ();

	Vector4 (const Type, const Type, const Type, const Type);

	template <typename T>
	Vector4 (const Vector4 <T> &);

	void
	setValue (const Type, const Type, const Type, const Type);

	template <typename T>
	void
	getValue (T &, T &, T &, T &) const;

	void
	set1Value (const size_type, const Type);

	const
	Type &
	get1Value (const size_type) const;

	Type &
	operator [ ] (const size_type);

	const Type &
	operator [ ] (const size_type) const;

	template <typename T>
	bool
	operator == (const Vector4 <T> &) const;

	template <typename T>
	bool
	operator not_eq (const Vector4 <T> &) const;

	template <typename T>
	Vector4 &
	operator = (const Vector4 <T> &);

	Vector4
	negate () const;

	Vector4
	operator - () const;

	template <typename T>
	Vector4 &
	operator += (const Vector4 <T> &);

	template <typename T>
	Vector4
	add (const Vector4 <T> &) const;

	template <typename T>
	Vector4
	operator + (const Vector4 <T> &) const;

	template <typename T>
	Vector4 &
	operator -= (const Vector4 <T> &);

	template <typename T>
	Vector4
	subtract (const Vector4 <T> &) const;

	template <typename T>
	Vector4
	operator - (const Vector4 <T> &) const;

	Vector4 &
	operator *= (const Type);

	Vector4
	multiply (const Type value) const;

	Vector4
	operator * (const Type value) const;

	template <typename T>
	Vector4 &
	operator *= (const Vector4 <T> &);

	template <typename T>
	Vector4
	multiply (const Vector4 <T> & value) const;

	Vector4 &
	operator /= (const Type);

	Vector4
	divide (const Type) const;

	Vector4
	operator / (const Type) const;

	template <typename T>
	Vector4 &
	operator /= (const Vector4 <T> &);

	template <typename T>
	Vector4
	divide (const Vector4 <T> &) const;

	template <typename T>
	Vector4
	operator / (const Vector4 <T> &) const;

	template <typename T>
	Type
	dot (const Vector4 <T> &) const;

	Vector4
	normalize () const;

	Type
	length () const;

	Type
	norm () const;

	/*
	 * io
	 */

	//void
	//fromStream (std::istream& stream);

	//void
	//toStream (std::ostream &) const;

	/*
	 * public members
	 */

	static const Vector4 Zero;
	static const Vector4 Base;

	Type x;
	Type y;
	Type z;
	Type w;

};

template <typename Type>
const Vector4 <Type> Vector4 <Type>::Zero;
template <typename Type>
const Vector4 <Type> Vector4 <Type>::Base (1, 1, 1, 1);
template <typename Type>
Vector4 <Type>::Vector4 () :
	x (0),
	y (0),
	z (0),
	w (0)
{ }

template <typename Type>
Vector4 <Type>::Vector4 (const Type x, const Type y, const Type z, const Type w) :
	x (x),
	y (y),
	z (z),
	w (w)
{ }

template <typename Type>
template <typename T>
Vector4 <Type>::Vector4 (const Vector4 <T> & v) :
	x (v .x),
	y (v .y),
	z (v .z),
	w (v .w)
{ }

template <typename Type>
inline
void
Vector4 <Type>::setValue (const Type x, const Type y, const Type z, const Type w)
{
	this -> x = x;
	this -> y = y;
	this -> z = z;
	this -> w = w;
}

template <typename Type>
template <typename T>
inline
void
Vector4 <Type>::getValue (T & x, T & y, T & z, T & w) const
{
	x = this -> x;
	y = this -> y;
	z = this -> z;
	w = this -> w;
}

template <typename Type>
inline
void
Vector4 <Type>::set1Value (const size_type index, const Type value)
{
	*this [index] = value;
}

template <typename Type>
inline
const
Type &
Vector4 <Type>::get1Value (const size_type index) const
{
	return *this [index];
}

template <typename Type>
inline
Type &
Vector4 <Type>::operator [ ] (const size_type index)
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}

	throw std::out_of_range ("Vector4::operator [ ] (index)");
}

template <typename Type>
inline
const
Type &
Vector4 <Type>::operator [ ] (const size_type index) const
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}

	throw std::out_of_range ("Vector4::operator [ ] (index) const");
}

template <typename Type>
template <typename T>
inline
bool
Vector4 <Type>::operator == (const Vector4 <T> & value) const
{
	return
	   x == value .x and
	   y == value .y and
	   z == value .z and
	   w == value .w;
}

template <typename Type>
template <typename T>
inline
bool
Vector4 <Type>::operator not_eq (const Vector4 <T> & value) const
{
	return
	   x not_eq value .x or
	   y not_eq value .y or
	   z not_eq value .z or
	   w not_eq value .w;
}

template <typename Type>
template <typename T>
Vector4 <Type> &
Vector4 <Type>::operator = (const Vector4 <T> & value)
{
	x = value .x;
	y = value .y;
	z = value .z;
	w = value .w;
	return *this;
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::negate () const
{
	return Vector4 <Type> (-x, -y, -z, -w);
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::operator - () const
{
	return negate ();
}

template <typename Type>
template <typename T>
Vector4 <Type> &
Vector4 <Type>::operator += (const Vector4 <T> & value)
{
	x += value .x;
	y += value .y;
	z += value .z;
	w += value .w;
	return *this;
}

template <typename Type>
template <typename T>
Vector4 <Type>
Vector4 <Type>::add (const Vector4 <T> & value) const
{
	return Vector4 <Type> (
	          x + value .x,
	          y + value .y,
	          z + value .z,
	          w + value .w
	          );
}

template <typename Type>
template <typename T>
inline
Vector4 <Type>
Vector4 <Type>::operator + (const Vector4 <T> & value) const
{
	return add (value);
}

template <typename Type>
template <typename T>
Vector4 <Type> &
Vector4 <Type>::operator -= (const Vector4 <T> & value)
{
	x -= value .x;
	y -= value .y;
	z -= value .z;
	w -= value .w;
	return *this;
}

template <typename Type>
template <typename T>
Vector4 <Type>
Vector4 <Type>::subtract (const Vector4 <T> & value) const
{
	return Vector4 <Type> (
	          x - value .x,
	          y - value .y,
	          z - value .z,
	          w - value .w
	          );
}

template <typename Type>
template <typename T>
inline
Vector4 <Type>
Vector4 <Type>::operator - (const Vector4 <T> & value) const
{
	return subtract (value);
}

template <typename Type>
Vector4 <Type> &
Vector4 <Type>::operator *= (const Type value)
{
	x *= value;
	y *= value;
	z *= value;
	w *= value;
	return *this;
}

template <typename Type>
Vector4 <Type>
Vector4 <Type>::multiply (const Type value) const
{
	return Vector4 <Type> (
	          x * value,
	          y * value,
	          z * value,
	          w * value
	          );
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::operator * (const Type value) const
{
	return multiply (value);
}

template <typename Type>
template <typename T>
Vector4 <Type> &
Vector4 <Type>::operator *= (const Vector4 <T> & vector)
{
	x *= vector .x;
	y *= vector .y;
	z *= vector .z;
	w *= vector .w;
	return *this;
}

template <typename Type>
template <typename T>
Vector4 <Type>
Vector4 <Type>::multiply (const Vector4 <T> & vector) const
{
	return Vector4 <Type> (
	          x * vector .x,
	          y * vector .y,
	          z * vector .z,
	          w * vector .w
	          );
}

template <typename Type>
inline
Vector4 <Type>
operator * (const Vector4 <Type> & vectorA, const Vector4 <Type> & vectorB)
{
	return vectorA .multiply (vectorB);
}

template <typename Type>
inline
Vector4 <Type> &
Vector4 <Type>::operator /= (const Type value)
{
	return *this *= (1 / value);
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::divide (const Type value) const
{
	return multiply (1 / value);
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::operator / (const Type value) const
{
	return multiply (1 / value);
}

template <typename Type>
template <typename T>
Vector4 <Type> &
Vector4 <Type>::operator /= (const Vector4 <T> & vector)
{
	x /= vector .x;
	y /= vector .y;
	z /= vector .z;
	w /= vector .w;
	return *this;
}

template <typename Type>
template <typename T>
Vector4 <Type>
Vector4 <Type>::divide (const Vector4 <T> & vector) const
{
	return Vector4 <Type> (
	          x / vector .x,
	          y / vector .y,
	          z / vector .z,
	          w / vector .w
	          );
}

template <typename Type>
template <typename T>
inline
Vector4 <Type>
Vector4 <Type>::operator / (const Vector4 <T> & vector) const
{
	return divide (vector);
}

template <typename Type>
template <typename T>
inline
Type
Vector4 <Type>::dot (const Vector4 <T> & vector) const
{
	return x * vector .x + y * vector .y + z * vector .z + w * vector .w;
}

template <typename Type>
inline
Vector4 <Type>
Vector4 <Type>::normalize () const
{
	Type l = length ();

	if (l)
		return divide (l);
	else
		return Vector4 <Type> ();
}

template <typename Type>
inline
Type
Vector4 <Type>::length () const
{
	return std::sqrt (norm ());
}

template <typename Type>
inline
Type
Vector4 <Type>::norm () const
{
	return x * x + y * y + z * z + w * w;
}

template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, Vector4 <Type> & vector)
{
	return istream
	       >> vector [0]
	       >> vector [1]
	       >> vector [2]
	       >> vector [3];
}

template <typename CharT, class Traits, typename Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const Vector4 <Type> & vector)
{
	return ostream
	       << std::setprecision (std::numeric_limits <Type>::digits10) << vector [0] << " "
	       << std::setprecision (std::numeric_limits <Type>::digits10) << vector [1] << " "
	       << std::setprecision (std::numeric_limits <Type>::digits10) << vector [2] << " "
	       << std::setprecision (std::numeric_limits <Type>::digits10) << vector [3];
}

/*
 * template <typename Type>
 * void
 * Vector4 <Type>::fromStream (std::istream& stream)
 * {
 * stream >> x >> y >> z >> w;
 * }
 *
 * template <typename Type>
 * void
 * Vector4 <Type>::toStream (std::ostream& stream) const
 * {
 * stream
 * << std::setprecision (std::numeric_limits <Type>::digits10) << x << " "
 * << std::setprecision (std::numeric_limits <Type>::digits10) << y << " "
 * << std::setprecision (std::numeric_limits <Type>::digits10) << z << " "
 * << std::setprecision (std::numeric_limits <Type>::digits10) << w;
 * }
 */

///  Return @a vector magnitude squared.
template <typename Type>
Type
dot (const Vector4 <Type> & vectorA, const Vector4 <Type> & vectorB)
{
	return vectorA .dot (vectorB);
}

///  Return @a vector magnitude.
template <typename Type>
inline
Type
abs (const Vector4 <Type> & vector)
{
	return vector .length ();
}

///  Return @a vector magnitude squared.
template <typename Type>
inline
Type
norm (const Vector4 <Type> & vector)
{
	return vector .norm ();
}

///  Return @a vector magnitude squared.
template <typename Type>
inline
Vector4 <Type>
normal (const Vector4 <Type> & vector)
{
	return vector .normalize ();
}

// friend
template <typename ScalarT, typename Type>
inline
Vector4 <Type>
operator * (const ScalarT value, const Vector4 <Type> & vector)
{
	return vector .multiply (value);
}

// friend
template <typename Type>
Vector4 <Type>
operator / (const Type value, const Vector4 <Type> & vector)
{
	return Vector4 <Type> (
	          value / vector .x,
	          value / vector .y,
	          value / vector .z,
	          value / vector .w
	          );
}

/**
 * @returns the lesser of each component.
 * @param a, b Items to compare.
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <typename Type>
Vector4 <Type>
min (const Vector4 <Type> & a, const Vector4 <Type> & b)
{
	return Vector4 <Type> (
	          std::min (a .x, b .x),
	          std::min (a .y, b .y),
	          std::min (a .z, b .z),
	          std::min (a .w, b .w)
	          );
}

/**
 * @returns the greater of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <typename Type>
Vector4 <Type>
max (const Vector4 <Type> & a, const Vector4 <Type> & b)
{
	return Vector4 <Type> (
	          std::max (a .x, b .x),
	          std::max (a .y, b .y),
	          std::max (a .z, b .z),
	          std::max (a .w, b .w)
	          );
}

typedef Vector4 <double> Vector4d;
typedef Vector4 <float>  Vector4f;

} // basic
} // titania

#endif
