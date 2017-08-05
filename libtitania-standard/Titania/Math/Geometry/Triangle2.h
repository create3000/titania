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

#ifndef __TITANIA_MATH_GEOMETRY_TRIANGLE2_H__
#define __TITANIA_MATH_GEOMETRY_TRIANGLE2_H__

#include "../Numbers/Vector2.h"
#include "../Geometry/Plane3.h"

namespace titania {
namespace math {

/**
 *  Template to represent a triangle in 2D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class triangle2
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Construction

	///  Default constructor. Constructs a box of size 0 0 and center 0 0.
	constexpr
	triangle2 () :
		m_a (),
		m_b (),
		m_c ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	triangle2 (const triangle2 <Up> & other) :
		m_a (other .a ()),
		m_b (other .b ()),
		m_c (other .c ())
	{ }

	///  Constructs a trangle from vertices @a a, @a b, and @a c in counterclockwise order.
	constexpr
	triangle2 (const vector2 <Type> & a, const vector2 <Type> & b, const vector2 <Type> & c) :
		m_a (a),
		m_b (b),
		m_c (c)
	{ }

	///  @name Member access

	const vector2 <Type> &
	a () const
	{ return m_a; }

	const vector2 <Type> &
	b () const
	{ return m_b; }

	const vector2 <Type> &
	c () const
	{ return m_c; }

	///  @name Intersection

	///  Returns true if @a point intersects with this triangle, otherwise false.
	bool
	intersects (const vector2 <Type> & point) const;


private:

	///  @name Members;

	vector2 <Type> m_a;
	vector2 <Type> m_b;
	vector2 <Type> m_c;

};

template <class Type>
bool
triangle2 <Type>::intersects (const vector2 <Type> & point) const
{
	const auto p = vector3 <Type> (point .x (), point .y (), 0);
	const auto A = b () - a ();
	const auto P = plane3 <Type> (vector3 <Type> (a () .x (), a () .y (), 0), normalize (vector3 <Type> (A .y (), -A .x (), 0)));

	if (P .distance (p) < 0)
		return false;

	const auto B = c () - b ();
	const auto Q = plane3 <Type> (vector3 <Type> (b () .x (), b () .y (), 0), normalize (vector3 <Type> (B .y (), -B .x (), 0)));

	if (Q .distance (p) < 0)
		return false;

	const auto C = a () - c ();
	const auto R = plane3 <Type> (vector3 <Type> (c () .x (), c () .y (), 0), normalize (vector3 <Type> (C .y (), -C .x (), 0)));

	if (R .distance (p) < 0)
		return false;

	return true;
}

///  @relates triangle2
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, triangle2 <Type> & triangle)
{
	vector2 <Type> a;
	vector2 <Type> b;
	vector2 <Type> c;

	istream >> a >> b >> c;

	if (istream)
		triangle = triangle2 <Type> (a, b, c);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const triangle2 <Type> & triangle)
{
	return ostream << triangle .a () << "  " << triangle .b () << "  " << triangle .c ();
}

extern template class triangle2 <float>;
extern template class triangle2 <double>;
extern template class triangle2 <long double>;

//
extern template std::istream & operator >> (std::istream &, triangle2 <float> &);
extern template std::istream & operator >> (std::istream &, triangle2 <double> &);
extern template std::istream & operator >> (std::istream &, triangle2 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const triangle2 <float> &);
extern template std::ostream & operator << (std::ostream &, const triangle2 <double> &);
extern template std::ostream & operator << (std::ostream &, const triangle2 <long double> &);

} // math
} // titania

#endif
