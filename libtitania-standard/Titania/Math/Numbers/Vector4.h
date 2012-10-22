/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __TITANIA_MATH_NUMBERS_VECTOR4_H__
#define __TITANIA_MATH_NUMBERS_VECTOR4_H__

#include <cmath>
#include <istream>
#include <ostream>

namespace titania {
namespace math {

/**
 *  Template to represent vector4 numbers.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values x, y, z and w.
 */
template <class Type>
class vector4
{
public:

	///  Size typedef.  Used for size and indices.
	typedef size_t size_type;

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///@{
	///  Default constructor.  All values default to 0.
	constexpr
	vector4 () :
		value { Type (), Type (), Type (), Type () } { }

	///  Copy constructor.
	template <class T>
	constexpr
	vector4 (const vector4 <T> & vector) :
		value { vector .x (), vector .y (), vector .z (), vector .w () } { }

	///  Components constructor. Set values to @a x, @a y, @a z and @a w.
	constexpr
	vector4 (const Type & x, const Type & y, const Type & z, const Type & w) :
		value { x, y, z, w } { }
	///@}

	///  @name Assignment operator

	///@{
	///  Assign @a vector to this vector.
	template <class T>
	vector4 &
	operator = (const vector4 <T> &);

	///@}

	///  @name Element access

	///@{
	///  Set x component of this vector.
	void
	x (const Type & t) { value [0] = t; }

	///  Return x component of this vector.
	constexpr Type
	x () const { return value [0]; }

	///  Set y component of this vector.
	void
	y (const Type & t) { value [1] = t; }

	///  Return y component of this vector.
	constexpr Type
	y () const { return value [1]; }

	///  Set z component of this vector.
	void
	z (const Type & t) { value [2] = t; }

	///  Return z component of this vector.
	constexpr Type
	z () const { return value [2]; }

	///  Set w component of this vector.
	void
	w (const Type & t) { value [3] = t; }

	///  Return w component of this vector.
	constexpr Type
	w () const { return value [3]; }

	//@{
	///  Access components by @a index.
	Type &
	operator [ ] (const size_type index) { return value [index]; }

	///  Access components by @a index.
	constexpr Type
	operator [ ] (const size_type index) const { return value [index]; }
	//@}
	///@}

	///  @name Capacity

	///@{
	///  Return number of components.
	static constexpr size_type
	size () { return 4; }

	///@}

	///  @name  Arithmetic operations
	///  All these operators modify this vector2 inplace.

	///@{
	///  Add @a vector to this vector.
	template <class T>
	vector4 &
	operator += (const vector4 <T> &);

	///  Add @a t to this vector.
	vector4 &
	operator += (const Type &);

	///  Subtract @a vector from this vector.
	template <class T>
	vector4 &
	operator -= (const vector4 <T> &);

	///  Subtract @a t from this vector.
	vector4 &
	operator -= (const Type &);

	///  Multiply this vector by @a vector.
	template <class T>
	vector4 &
	operator *= (const vector4 <T> &);

	///  Multiply this vector by @a t.
	vector4 &
	operator *= (const Type &);

	///  Divide this vector by @a vector.
	template <class T>
	vector4 &
	operator /= (const vector4 <T> &);

	///  Divide this vector by @a t.
	vector4 &
	operator /= (const Type &);

	///  Normalize this vector in place.
	vector4 &
	normalize ();

	///@}


private:

	Type value [size ()];

};

template <class Type>
template <class T>
vector4 <Type> &
vector4 <Type>::operator = (const vector4 <T> & vector)
{
	value [0] = vector .x ();
	value [1] = vector .y ();
	value [2] = vector .z ();
	value [3] = vector .w ();
	return *this;
}

template <class Type>
template <class T>
vector4 <Type> &
vector4 <Type>::operator += (const vector4 <T> & vector)
{
	value [0] += vector .x ();
	value [1] += vector .y ();
	value [2] += vector .z ();
	value [3] += vector .w ();
	return *this;
}

template <class Type>
vector4 <Type> &
vector4 <Type>::operator += (const Type & t)
{
	value [0] += t;
	value [1] += t;
	value [2] += t;
	value [3] += t;
	return *this;
}

template <class Type>
template <class T>
vector4 <Type> &
vector4 <Type>::operator -= (const vector4 <T> & vector)
{
	value [0] -= vector .x ();
	value [1] -= vector .y ();
	value [2] -= vector .z ();
	value [3] -= vector .w ();
	return *this;
}

template <class Type>
vector4 <Type> &
vector4 <Type>::operator -= (const Type & t)
{
	value [0] -= t;
	value [1] -= t;
	value [2] -= t;
	value [3] -= t;
	return *this;
}

template <class Type>
template <class T>
vector4 <Type> &
vector4 <Type>::operator *= (const vector4 <T> & vector)
{
	value [0] *= vector .x ();
	value [1] *= vector .y ();
	value [2] *= vector .z ();
	value [3] *= vector .w ();
	return *this;
}

template <class Type>
vector4 <Type> &
vector4 <Type>::operator *= (const Type & t)
{
	value [0] *= t;
	value [1] *= t;
	value [2] *= t;
	value [3] *= t;
	return *this;
}

template <class Type>
template <class T>
vector4 <Type> &
vector4 <Type>::operator /= (const vector4 <T> & vector)
{
	value [0] /= vector .x ();
	value [1] /= vector .y ();
	value [2] /= vector .z ();
	value [3] /= vector .w ();
	return *this;
}

template <class Type>
inline
vector4 <Type> &
vector4 <Type>::operator /= (const Type & t)
{
	value [0] /= t;
	value [1] /= t;
	value [2] /= t;
	value [3] /= t;
	return *this;
}

template <class Type>
vector4 <Type> &
vector4 <Type>::normalize ()
{
	Type length = abs (*this);

	if (length)
		return *this /= length;

	return *this;
}

///  @relates vector4
///  @name Comparision operations

///@{
//@{
///  Compares two vector4 numbers.
///  Return true if @a a is equal to @a b.
template <class Type>
constexpr bool
operator == (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return
	   a .x () == b .x () and
	   a .y () == b .y () and
	   a .z () == b .z () and
	   a .w () == b .w ();
}

///  Compares two vector4 numbers.
///  Return false if @a a is not equal to @a b.
template <class Type>
constexpr bool
operator not_eq (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return
	   a .x () not_eq b .x () or
	   a .y () not_eq b .y () or
	   a .z () not_eq b .z () or
	   a .w () not_eq b .w ();
}
//@}
///@}

///  @relates vector4
///  @name Arithmetic operations

///@{
//@{
///  Return @a a.
template <class Type>
constexpr vector4 <Type>
operator + (const vector4 <Type> & a)
{
	return vector4 <Type> (+a .x (),
	                       +a .y (),
	                       +a .z (),
	                       +a .w ());
}

///  Return vector negation of @a a.
template <class Type>
constexpr vector4 <Type>
operator - (const vector4 <Type> & a)
{
	return vector4 <Type> (-a .x (),
	                       -a .y (),
	                       -a .z (),
	                       -a .w ());
}
//@}

//@{
///  Return new vector value @a a plus @a b.
template <class Type>
inline
vector4 <Type>
operator + (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (a) += b;
}

///  Return new vector value @a a plus @a b.
template <class Type>
inline
vector4 <Type>
operator + (const vector4 <Type> & a, const Type & b)
{
	return vector4 <Type> (a) += b;
}

///  Return new vector value @a a plus @a b.
template <class Type>
inline
vector4 <Type>
operator + (const Type & a, const vector4 <Type> & b)
{
	return vector4 <Type> (b) += a;
}
//@}

//@{
///  Return new vector value @a a minus @a b.
template <class Type>
inline
vector4 <Type>
operator - (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (a) -= b;
}

///  Return new vector value @a a minus @a b.
template <class Type>
inline
vector4 <Type>
operator - (const vector4 <Type> & a, const Type & b)
{
	return vector4 <Type> (a) -= b;
}

///  Return new vector value @a a minus @a b.
template <class Type>
inline
constexpr vector4 <Type>
operator - (const Type & a, const vector4 <Type> & b)
{
	return vector4 <Type> (a - b .x (),
	                       a - b .y (),
	                       a - b .z (),
	                       a - b .w ());
}
//@}

//@{
///  Return new vector value @a a times @a b.
template <class Type>
inline
vector4 <Type>
operator * (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (a) *= b;
}

///  Return new vector value @a a times @a b.
template <class Type>
inline
vector4 <Type>
operator * (const vector4 <Type> & a, const Type & b)
{
	return vector4 <Type> (a) *= b;
}

///  Return new vector value @a a times @a b.
template <class Type>
inline
vector4 <Type>
operator * (const Type & a, const vector4 <Type> & b)
{
	return vector4 <Type> (b) *= a;
}
//@}

//@{
///  Return new vector value @a a divided by @a b.
template <class Type>
inline
vector4 <Type>
operator / (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (a) /= b;
}

///  Return new vector value @a a divided by @a b.
template <class Type>
inline
vector4 <Type>
operator / (const vector4 <Type> & a, const Type & b)
{
	return vector4 <Type> (a) /= b;
}

///  Return new vector value @a a divided by @a b.
template <class Type>
constexpr vector4 <Type>
operator / (const Type & a, const vector4 <Type> & b)
{
	return vector4 <Type> (
	          a / b .x (),
	          a / b .y (),
	          a / b .z (),
	          a / b .w ());
}
//@}

//@{
///  Return new vector value @a a dot @a b.
template <class Type>
constexpr Type
dot (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return
	   a .x () * b .x ()
	   + a .y () * b .y ()
	   + a .z () * b .z ()
	   + a .w () * b .w ();
}

///  Return the @a vector magnitude.
template <class Type>
inline
constexpr Type
abs (const vector4 <Type> & vector)
{
	return std::sqrt (norm (vector));
}

///  Returns the @a vector magnitude squared.
template <class Type>
inline
constexpr Type
norm (const vector4 <Type> & vector)
{
	return dot (vector, vector);
}

///  Returns @a vector normalized.
template <class Type>
inline
vector4 <Type>
normalize (const vector4 <Type> & vector)
{
	return vector4 <Type> (vector) .normalize ();
}
//@}

//@{
/**
 * @returns the lesser of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector4 <Type>
min (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (std::min (a .x (), b .x ()),
	                       std::min (a .y (), b .y ()),
	                       std::min (a .z (), b .z ()),
	                       std::min (a .w (), b .w ()));
}

/**
 * @returns the greater of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector4 <Type>
max (const vector4 <Type> & a, const vector4 <Type> & b)
{
	return vector4 <Type> (std::max (a .x (), b .x ()),
	                       std::max (a .y (), b .y ()),
	                       std::max (a .z (), b .z ()),
	                       std::max (a .w (), b .w ()));
}
//@}
///@}

///  @relates vector4
///  @name Input/Output operations

///@{
//@{
///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, vector4 <Type> & vector)
{
	Type x, y, z, w;

	istream >> x >> y >> z >> w;

	if (istream)
		vector = vector4 <Type> (x, y, z, w);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const vector4 <Type> & vector)
{
	return ostream
	       << vector .x () << ' '
	       << vector .y () << ' '
	       << vector .z () << ' '
	       << vector .w ();
}
//@}
///@}

extern template class vector4 <float>;
extern template class vector4 <double>;
extern template class vector4 <long double>;

//
extern template std::istream & operator >> (std::istream &, vector4 <float> &);
extern template std::istream & operator >> (std::istream &, vector4 <double> &);
extern template std::istream & operator >> (std::istream &, vector4 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const vector4 <float> &);
extern template std::ostream & operator << (std::ostream &, const vector4 <double> &);
extern template std::ostream & operator << (std::ostream &, const vector4 <long double> &);

} // math
} // titania

#endif
