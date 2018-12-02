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

#include <Titania/X3D.h>

#include <btBulletDynamicsCommon.h>
//#include <filesystem>

using namespace titania;
using namespace titania::X3D;

namespace test {
namespace math {

/**
 *  Template to represent vector3 numbers.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values x, y and z.
 */
template <class Type>
class vector3
{
private:

	static constexpr size_t Size = 3;

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
	vector3 () :
		m_array { Type (), Type (), Type () }
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	vector3 (const vector3 <Up> & vector) :
		m_array { vector .x (), vector .y (), vector .z () }
	{ }

	///  Components constructor. Set values to @a x, @a y and @a z.
	constexpr
	vector3 (const Type & x, const Type & y, const Type & z) :
		m_array { x, y, z }
	{ }

	template <class ... Args>
	vector3 (const Args & ... args) :
		vector3 ()
	{
		construct (begin (), args ...);
	}

	template <template <class> class Vector, class ... Args>
	inline
	void
	construct (iterator first, const Vector <Type> & value, const Args & ... args)
	{
		for (const auto & v : value)
		{
			if (first == end ())
				return;
		
			*first ++ = v;
		}
	
		construct (first, args ...);
	}

	template <class ... Args>
	inline
	void
	construct (iterator first, const Type & value, const Args & ... args)
	{
		if (first == end ())
			return;

		*first ++ = value;

		construct (first, args ...);
	}

	template <class ... Args>
	inline
	void
	construct (iterator first)
	{ }

	///  @name Assignment operator

	///  Assign @a other to this vector.
	template <class Up>
	vector3 &
	operator = (const vector3 <Up> & other);

	///  @name Element access

	///  Set x component of this vector.
	void
	x (const Type & value)
	{ m_array [0] = value; }

	///  Returns x component of this vector.
	constexpr
	const_reference
	x () const
	{ return m_array [0]; }

	///  Set y component of this vector.
	void
	y (const Type & value)
	{ m_array [1] = value; }

	///  Returns y component of this vector.
	constexpr
	const_reference
	y () const
	{ return m_array [1]; }

	///  Set z component of this vector.
	void
	z (const Type & value)
	{ m_array [2] = value; }

	///  Returns z component of this vector.
	constexpr
	const_reference
	z () const
	{ return m_array [2]; }

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
	swap (vector3 & other)
	{ m_array .swap (other .m_array); }

	///  @name  Arithmetic operations
	///  All these operators modify this vector2 inplace.

	///  Negates this vector.
	void
	negate ();

	///  Add @a vector to this vector.
	template <class Up>
	vector3 &
	operator += (const vector3 <Up> & vector);

	///  Add @a t to this vector.
	vector3 &
	operator += (const Type & t);

	///  Subtract @a vector from this vector.
	template <class Up>
	vector3 &
	operator -= (const vector3 <Up> & vector);

	///  Subtract @a t from this vector.
	vector3 &
	operator -= (const Type & t);

	///  Multiply this vector by @a vector.
	template <class Up>
	vector3 &
	operator *= (const vector3 <Up> & vector);

	///  Multiply this vector by @a t.
	vector3 &
	operator *= (const Type & t);

	///  Divide this vector by @a vector.
	template <class Up>
	vector3 &
	operator /= (const vector3 <Up> & vector);

	///  Divide this vector by @a t.
	vector3 &
	operator /= (const Type & t);

	///  Normalize this vector in place.
	void
	normalize ();


private:

	///  @name Members

	array_type m_array;

};

template <class Type>
template <class Up>
vector3 <Type> &
vector3 <Type>::operator = (const vector3 <Up> & other)
{
	m_array [0] = other .x ();
	m_array [1] = other .y ();
	m_array [2] = other .z ();
	return *this;
}

template <class Type>
void
vector3 <Type>::negate ()
{
	m_array [0] = -m_array [0];
	m_array [1] = -m_array [1];
	m_array [2] = -m_array [2];
}

template <class Type>
template <class Up>
vector3 <Type> &
vector3 <Type>::operator += (const vector3 <Up> & vector)
{
	m_array [0] += vector .x ();
	m_array [1] += vector .y ();
	m_array [2] += vector .z ();
	return *this;
}

template <class Type>
vector3 <Type> &
vector3 <Type>::operator += (const Type & t)
{
	m_array [0] += t;
	m_array [1] += t;
	m_array [2] += t;
	return *this;
}

template <class Type>
template <class Up>
vector3 <Type> &
vector3 <Type>::operator -= (const vector3 <Up> & vector)
{
	m_array [0] -= vector .x ();
	m_array [1] -= vector .y ();
	m_array [2] -= vector .z ();
	return *this;
}

template <class Type>
vector3 <Type> &
vector3 <Type>::operator -= (const Type & t)
{
	m_array [0] -= t;
	m_array [1] -= t;
	m_array [2] -= t;
	return *this;
}

template <class Type>
template <class Up>
vector3 <Type> &
vector3 <Type>::operator *= (const vector3 <Up> & vector)
{
	m_array [0] *= vector .x ();
	m_array [1] *= vector .y ();
	m_array [2] *= vector .z ();
	return *this;
}

template <class Type>
vector3 <Type> &
vector3 <Type>::operator *= (const Type & t)
{
	m_array [0] *= t;
	m_array [1] *= t;
	m_array [2] *= t;
	return *this;
}

template <class Type>
template <class Up>
vector3 <Type> &
vector3 <Type>::operator /= (const vector3 <Up> & vector)
{
	m_array [0] /= vector .x ();
	m_array [1] /= vector .y ();
	m_array [2] /= vector .z ();
	return *this;
}

template <class Type>
vector3 <Type> &
vector3 <Type>::operator /= (const Type & t)
{
	m_array [0] /= t;
	m_array [1] /= t;
	m_array [2] /= t;
	return *this;
}

template <class Type>
void
vector3 <Type>::normalize ()
{
	const Type length = abs (*this);

	if (length)
		*this /= length;
}

///  @relates vector3
///  @name Comparision operations

///  Compares two vector2 numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator == (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .x () == rhs .x () and
	       lhs .y () == rhs .y () and
	       lhs .z () == rhs .z ();
}

///  Compares two vector2 numbers.
///  Returns false if @a lhs is not equal to @a rhs.
template <class Type>
inline
constexpr
bool
operator not_eq (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .x () not_eq rhs .x () or
	       lhs .y () not_eq rhs .y () or
	       lhs .z () not_eq rhs .z ();
}

///  Lexicographically compares two vector3 numbers.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return std::tie (lhs .x (), lhs .y (), lhs .z ()) < std::tie (rhs .x (), rhs .y (), rhs .z ());
}

///  Lexicographically compares two vector3 numbers.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator > (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two vector3 numbers.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator <= (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two vector3 numbers.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return not (lhs < rhs);
}

///  @relates vector3
///  @name Arithmetic operations

///  Returns a copy @a vector.
template <class Type>
inline
constexpr
vector3 <Type>
operator + (const vector3 <Type> & vector)
{
	return vector;
}

///  Returns vector negation of @a vector.
template <class Type>
inline
vector3 <Type>
operator - (const vector3 <Type> & vector)
{
	vector3 <Type> result (vector);
	result .negate ();
	return result;
}

///  Returns vector negation of @a vector.
template <class Type>
inline
vector3 <Type>
negate (const vector3 <Type> & vector)
{
	vector3 <Type> result (vector);
	result .negate ();
	return result;
}

///  Returns new vector value @a lhs plus @a rhs.
template <class Type>
inline
vector3 <Type>
operator + (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs) += rhs;
}

///  Return new vector value @a lhs plus @a rhs.
template <class Type>
inline
vector3 <Type>
operator + (const vector3 <Type> & lhs, const Type & rhs)
{
	return vector3 <Type> (lhs) += rhs;
}

///  Return new vector value @a lhs plus @a rhs.
template <class Type>
inline
vector3 <Type>
operator + (const Type & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (rhs) += lhs;
}

///  Returns new vector value @a lhs minus @a rhs.
template <class Type>
inline
vector3 <Type>
operator - (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs) -= rhs;
}

///  Return new vector value @a lhs minus @a rhs.
template <class Type>
inline
vector3 <Type>
operator - (const vector3 <Type> & lhs, const Type & rhs)
{
	return vector3 <Type> (lhs) -= rhs;
}

///  Return new vector value @a lhs minus @a rhs.
template <class Type>
inline
vector3 <Type>
operator - (const Type & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (-rhs) += lhs;
}

///  Returns new vector value @a lhs times @a rhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs) *= rhs;
}

///  Returns new vector value @a lhs times @a rhs.
template <class Type>
inline
vector3 <Type>
operator * (const vector3 <Type> & lhs, const Type & rhs)
{
	return vector3 <Type> (lhs) *= rhs;
}

///  Returns new vector value @a lhs times @a rhs.
template <class Type>
inline
vector3 <Type>
operator * (const Type & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (rhs) *= lhs;
}

///  Returns new vector value @a lhs divided by @a rhs.
template <class Type>
inline
vector3 <Type>
operator / (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs) /= rhs;
}

///  Returns new vector value @a lhs divided by @a rhs.
template <class Type>
inline
vector3 <Type>
operator / (const vector3 <Type> & lhs, const Type & rhs)
{
	return vector3 <Type> (lhs) /= rhs;
}

///  Returns new vector value @a lhs divided by @a rhs.
template <class Type>
vector3 <Type>
operator / (const Type & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs / rhs .x (),
	                       lhs / rhs .y (),
	                       lhs / rhs .z ());
}

///  Returns new vector value @a lhs dot @a rhs.
template <class Type>
constexpr
Type
dot (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return lhs .x () * rhs .x () +
	       lhs .y () * rhs .y () +
	       lhs .z () * rhs .z ();
}

///  Returns new vector value @a lhs cross @a rhs.
template <class Type>
constexpr
vector3 <Type>
cross (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (lhs .y () * rhs .z () - lhs .z () * rhs .y (),
	                       lhs .z () * rhs .x () - lhs .x () * rhs .z (),
	                       lhs .x () * rhs .y () - lhs .y () * rhs .x ());
}

///  Returns the @a vector magnitude squared.
template <class Type>
inline
constexpr
Type
norm (const vector3 <Type> & vector)
{
	return dot (vector, vector);
}

///  Returns the @a vector magnitude.
template <class Type>
inline
constexpr
Type
abs (const vector3 <Type> & vector)
{
	// C++17
	//return std::hypot (vector .x (), vector .y (), vector .z ());

	return std::sqrt (norm (vector));
}

///  Returns @a vector normalized.
template <class Type>
inline
vector3 <Type>
normalize (const vector3 <Type> & vector)
{
	vector3 <Type> result (vector);
	result .normalize ();
	return result;
}

///  Returns the reflection vector for an incident vector.
template <class Type>
inline
vector3 <Type>
reflect (const vector3 <Type> & vector, const vector3 <Type> & normal)
{
	return vector - (normal * (2 * dot (vector, normal)));
}

///  Returns the distance between @a source and @a destination.
template <class Type>
inline
Type
distance (const vector3 <Type> & source, const vector3 <Type> & destination)
{
	return abs (destination - source);
}

/**
 * @returns the lesser of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
min (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (std::min (lhs .x (), rhs .x ()),
	                       std::min (lhs .y (), rhs .y ()),
	                       std::min (lhs .z (), rhs .z ()));
}

/**
 * @returns the greater of each component.
 * @param a, b Items to compare.\n
 * Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
max (const vector3 <Type> & lhs, const vector3 <Type> & rhs)
{
	return vector3 <Type> (std::max (lhs .x (), rhs .x ()),
	                       std::max (lhs .y (), rhs .y ()),
	                       std::max (lhs .z (), rhs .z ()));
}

/**
 * @returns Computes nearest integer not greater than arg. 
 * @param a vector3 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
floor (const vector3 <Type> & arg)
{
	return vector3 <Type> (std::floor (arg .x ()),
	                       std::floor (arg .y ()),
	                       std::floor (arg .z ()));
}

/**
 * @returns Computes nearest integer not less than arg.
 * @param arg vector to floor.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
ceil (const vector3 <Type> & arg)
{
	return vector3 <Type> (std::ceil (arg .x ()),
	                       std::ceil (arg .y ()),
	                       std::ceil (arg .z ()));
}

/**
 * @returns Computes the nearest integer value to @a arg (in floating-point format),
 *          rounding halfway cases away from zero, regardless of the current rounding mode.
 * @param arg vector to round.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
round (const vector3 <Type> & arg)
{
	return vector3 <Type> (std::round (arg .x ()),
	                       std::round (arg .y ()),
	                       std::round (arg .z ()));
}

/**
 * @returns Clamps @a arg between @a min and @a max .
 * @param a vector3 <Type>.\n
 * @a Type is any type supporting copy constructions and comparisons with operator<.
 */

template <class Type>
vector3 <Type>
clamp (const vector3 <Type> & arg, const Type & min, const Type & max)
{
	return vector3 <Type> (std::clamp (arg .x (), min, max),
	                       std::clamp (arg .y (), min, max),
	                       std::clamp (arg .z (), min, max));
}

/**
 * @returns Computes the fractional part of the @a arg.
 * @param a vector3 <Type>.\n
 * @a Type is any floating point type.
 */

template <class Type>
std::enable_if_t <std::is_floating_point_v <Type>, vector3 <Type>>
fract (const vector3 <Type> & arg)
{
	return vector3 <Type> (fract (arg .x ()),
	                       fract (arg .y ()),
	                       fract (arg .z ()));
}

///  @relates vector3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, vector3 <Type> & vector)
{
	Type x, y, z;

	istream >> x >> y >> z;

	if (istream)
		vector = vector3 <Type> (x, y, z);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const vector3 <Type> & vector)
{
	return ostream
	       << vector .x () << ' '
	       << vector .y () << ' '
	       << vector .z ();
}

} // math
} // test

void
test_uri_output (const basic::uri & uri)
{
	__LOG__ << uri << std::endl;
	__LOG__ << uri .path () << std::endl;
	__LOG__ << uri .basename () << std::endl;
	__LOG__ << uri .escape () << std::endl;
	__LOG__ << uri .escape (" ") << std::endl;
}

void
test_uri ()
{
	test_uri_output (basic::path ("/usr/share/data/foo/bah%20bah foo.foo") .escape ());
}

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;
	std::cout << std::boolalpha;
	std::clog << std::boolalpha;
	std::cerr << std::boolalpha;
	std::cout << std::setprecision (std::numeric_limits <float>::digits10);
	std::clog << std::setprecision (std::numeric_limits <float>::digits10);
	std::cerr << std::setprecision (std::numeric_limits <float>::digits10);
	//std::cout << std::setprecision (std::numeric_limits <double>::digits10);
	//std::clog << std::setprecision (std::numeric_limits <double>::digits10);
	//std::cerr << std::setprecision (std::numeric_limits <double>::digits10);
	std::cout .imbue (std::locale (""));
	std::clog .imbue (std::locale (""));
	std::cerr .imbue (std::locale (""));

	std::locale::global (std::locale (""));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using Vec3d = test::math::vector3 <double>;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
