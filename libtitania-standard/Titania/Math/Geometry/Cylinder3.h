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

#ifndef __TITANIA_MATH_GEOMETRY_CYLINDER3_H__
#define __TITANIA_MATH_GEOMETRY_CYLINDER3_H__

#include "../Geometry/Line3.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Rotation4.h"
#include "../Numbers/Vector3.h"

#include <tuple>

namespace titania {
namespace math {

/**
 *  Template to represent a cylinder in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class cylinder3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor. Constructs a line of from (0, 0, 0) to (0, 0, 1).
	constexpr
	cylinder3 () :
		  m_axis (vector3 <Type> (), vector3 <Type> (0, 1, 0)),
		m_radius (1)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	cylinder3 (const cylinder3 <Up> & cylinder) :
		  m_axis (cylinder .axis ()),
		m_radius (cylinder .radius ())
	{ }

	///  Constructs a line of from @a point and @a point.
	constexpr
	cylinder3 (const line3 <Type> & axis, const Type & radius) :
		  m_axis (axis),
		m_radius (radius)
	{ }

	///  @name Element access

	///  Returns the point of this line.
	const line3 <Type> &
	axis () const
	{ return m_axis; }

	/// Returns the direction of this line as normal a vector.
	const Type &
	radius () const
	{ return m_radius; }

	std::tuple <vector3 <Type>, vector3 <Type>, bool>
	intersects (const line3 <Type> & line) const;


private:

	std::tuple <vector3 <Type>, vector3 <Type>, bool>
	unit_cylinder_intersects (const line3 <Type> & line) const;

	///  @name Members

	line3 <Type> m_axis;
	Type         m_radius;

};

////////////////////////////////////////////////////////////////////////
//
// Description:
//  Intersect given line with this cylinder, returning the
//  results in enter and exit. Returns TRUE if there was an
//  intersection (and results are valid).
//
// Taken from Inventor SbCylinder.c++

template <class Type>
std::tuple <vector3 <Type>, vector3 <Type>, bool>
cylinder3 <Type>::intersects (const line3 <Type> & line) const
{
	try
	{
		// The intersection will actually be done on a radius 1 cylinder
		// aligned with the y axis, so we transform the line into that
		// space, then intersect, then transform the results back.
	
		// rotation to y axis
		const auto rotToYAxis = rotation4 <Type> (axis () .direction (), vector3 <Type> (0, 1, 0));
		const auto mtxToYAxis = matrix4 <Type> (rotToYAxis);
	
		// scale to unit space
		const auto scaleFactor = 1 / radius ();

		matrix4 <Type> toUnitCylSpace;
		toUnitCylSpace .scale (vector3 <Type> (scaleFactor, scaleFactor, scaleFactor));
		toUnitCylSpace .mult_left (mtxToYAxis);
	
		// find the given line un-translated
		const auto point             = line .point () - axis () .point ();
		const auto noTranslationLine = line3 <Type> (point, line .direction ());

		// find the un-translated line in unit cylinder's space
		const auto cylLine = noTranslationLine * toUnitCylSpace;
	
		// find the intersection on the unit cylinder
		auto intersection = unit_cylinder_intersects (cylLine);
	
		if (not std::get <2> (intersection))
			return intersection;

		// transform back to original space
		const auto fromUnitCylSpace = inverse (toUnitCylSpace);

		auto enter = fromUnitCylSpace .mult_vec_matrix (std::get <0> (intersection));
		auto exit  = fromUnitCylSpace .mult_vec_matrix (std::get <1> (intersection));

		enter += axis () .point ();
		exit  += axis () .point ();
	
		return std::tuple (enter, exit, true);
	}
	catch (const std::domain_error &)
	{
		return std::tuple (vector3 <Type> (), vector3 <Type> (), false);
	}
}

////////////////////////////////////////////////////////////////////////
//
// Description:
//  Intersect the line with a unit cylinder. Returns TRUE if
//  there was an intersection, and returns the intersection points
//  in enter and exit.
//
//  The cylinder has radius 1 and is aligned with the
//  y axis, such that x^2 + z^2 - 1 = 0
//
//  Taken from Pat Hanrahan's chapter in Glassner's
//  _Intro to Ray Tracing_, page 91, and some code
//  stolen from Paul Strauss.
//
// Taken from Inventor SbCylinder.c++

template <class Type>
std::tuple <vector3 <Type>, vector3 <Type>, bool>
cylinder3 <Type>::unit_cylinder_intersects (const line3 <Type> & line) const
{
	Type t0, t1;

	const vector3 <Type> & pos = line .point ();
	const vector3 <Type> & dir = line .direction ();

	const Type A = dir [0] * dir [0] + dir [2] * dir [2];
	const Type B = 2 * (pos [0] * dir [0] + pos [2] * dir [2]);
	const Type C = pos [0] * pos [0] + pos [2] * pos [2] - 1;

	// discriminant = B^2 - 4AC
	const Type discr = B * B - 4 * A * C;

	// if discriminant is negative, no intersection
	if (discr < 0)
		return std::tuple (vector3 <Type> (), vector3 <Type> (), false);

	const Type sqroot = std::sqrt (discr);

	// magic to stabilize the answer
	if (B > 0)
	{
		t0 = -(2 * C) / (sqroot + B);
		t1 = -(sqroot + B) / (2 * A);
	}
	else
	{
		t0 = (2 * C) / (sqroot - B);
		t1 = (sqroot - B) / (2 * A);
	}

	const auto enter = pos + (dir * t0);
	const auto exit  = pos + (dir * t1);

	return std::tuple (enter, exit, true);
}

///  @relates line3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, cylinder3 <Type> & line)
{
	line3 <Type> axis;
	Type         radius;

	istream >> axis >> radius;

	if (istream)
		line = cylinder3 <Type> (axis, radius);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const cylinder3 <Type> & line)
{
	return ostream << line .axis () << " " << line .radius ();
}

extern template class cylinder3 <float>;
extern template class cylinder3 <double>;
extern template class cylinder3 <long double>;

//
extern template std::istream & operator >> (std::istream &, cylinder3 <float> &);
extern template std::istream & operator >> (std::istream &, cylinder3 <double> &);
extern template std::istream & operator >> (std::istream &, cylinder3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const cylinder3 <float> &);
extern template std::ostream & operator << (std::ostream &, const cylinder3 <double> &);
extern template std::ostream & operator << (std::ostream &, const cylinder3 <long double> &);

} // math
} // titania

#endif
