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

#ifndef __TITANIA_MATH_GEOMETRY_LINE_SEGMENT3_H__
#define __TITANIA_MATH_GEOMETRY_LINE_SEGMENT3_H__

#include "../Geometry/Line3.h"
#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent a line segment.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of the two point values.
 */
template <class Type>
class line_segment3
{
public:

	///  @name Construction

	///  Default constructor. Constructs a line segement with length 0 on point (0, 0, 0).
	constexpr
	line_segment3 () :
		line_segment3 (vector3 <Type> (), vector3 <Type> ())
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	line_segment3 (const line_segment3 <Up> & other) :
		value ({ other .point0 (), other .point1 (), other .line () })
	{ }

	///  Constructs a line segment from @a point0 and @a point1.
	constexpr
	line_segment3 (const vector3 <Type> & point0, const vector3 <Type> & point1) :
		value ({ point0, point1, line3 <Type> (point0, point1, point_type ()) })
	{ }

	///  @name Member access

	const vector3 <Type> &
	point0 () const
	{ return value .point0; }

	const vector3 <Type> &
	point1 () const
	{ return value .point1; }

	const line3 <Type> &
	line () const
	{ return value .line; }

	///  @name Operations

	Type
	distance (const vector3 <Type> & point) const;

private:

	struct Value
	{
		vector3 <Type> point0;
		vector3 <Type> point1;
		line3 <Type> line;
	};

	Value value;

};

///  Returns the distance to @a point.
template <class Type>
inline
Type
line_segment3 <Type>::distance (const vector3 <Type> & point) const
{
	const auto closest = line () .closest_point (point);
	const auto between = abs ((closest - point0 ()) + (closest - point1 ())) <= abs (point0 () - point1 ());

	// Closest point lies between point0 and point1.
	if (between)
		return abs (point - closest);

	const auto distance0 = abs (point - point0 ());
	const auto distance1 = abs (point - point1 ());

	return std::min (distance0, distance1);
}

///  @relates line_segment3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, line_segment3 <Type> & segment)
{
	vector3 <Type> point0, point1;

	istream >> point0 >> point1;

	if (istream)
		segment = line_segment3 <Type> (point0, point1);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const line_segment3 <Type> & segment)
{
	return ostream << segment .point0 () << "  " << segment .point1 ();
}

extern template class line_segment3 <float>;
extern template class line_segment3 <double>;
extern template class line_segment3 <long double>;

//
extern template std::istream & operator >> (std::istream &, line_segment3 <float> &);
extern template std::istream & operator >> (std::istream &, line_segment3 <double> &);
extern template std::istream & operator >> (std::istream &, line_segment3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const line_segment3 <float> &);
extern template std::ostream & operator << (std::ostream &, const line_segment3 <double> &);
extern template std::ostream & operator << (std::ostream &, const line_segment3 <long double> &);

} // math
} // titania

#endif
