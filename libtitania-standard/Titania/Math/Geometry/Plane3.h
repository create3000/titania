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

#ifndef __TITANIA_MATH_GEOMETRY_PLANE3_H__
#define __TITANIA_MATH_GEOMETRY_PLANE3_H__

#include "../Geometry/Line3.h"
#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent a plane equation.
 *  This discribes a plane in the form:
 *
 *  \f$Ax + By + Cz = D\f$
 *
 *  It is known as the cartesian form of the equation of a plane because it is in terms of
 *  the cartesian coordinates x, y and z.  The equation of a plane in 3D space is defined
 *  with normal vector (perpendicular to the plane) and the the perpendicular distance from
 *  the origin.  The plane is defined as a plane that generates two half-spaces depending on
 *  the direction of the normal vector.  The upper half space is defined as the space to
 *  which the normal vector points to.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of normal values x, y, z and the perpendicular distance from the origin.
 */
template <class Type>
class plane3
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///@{
	///  Copy constructor.
	constexpr
	plane3 (const plane3 & plane) :
		value { plane .normal (), plane .distanceFromOrigin () }
	{ }

	///  Constructs a plane from @a point and @a normal.
	constexpr
	plane3 (const vector3 <Type> & point, const vector3 <Type> & normal) :
		value { normal, dot (normal, point) }
	{ }

	///  Constructs a plane from @a vector. The point the vector points to is taken as a point on a plane
	///  and the direction of this vector as normal.
	constexpr
	plane3 (const vector3 <Type> & vector) :
		value { normalize (vector), abs (vector) }
	{ }
	///@}

	///  @name Element access

	///@{
	///  Returns the normal of this plane.
	const vector3 <Type> &
	normal () const { return value .normal; }

	///  Returns the distance form origin.
	const Type &
	distanceFromOrigin () const { return value .distanceFromOrigin; }

	///@}

	constexpr Type
	distance (const vector3 <Type> &) const;

	bool
	intersect (const line3 <Type> &, vector3 <Type> &) const;


private:

	struct Value
	{
		vector3 <Type> normal;
		Type distanceFromOrigin;
	};

	Value value;

};

///  @name Arithmetic operations

///@{
//@{
///  Returns the distance from @a point.
///  The value can be both negative and positive depending
///  on the direction of the normal vector of this plane. If the point lies in the upper half
///  space the value will be positive otherwise a negative value is returned.
template <class Type>
inline
constexpr Type
plane3 <Type>::distance (const vector3 <Type> & point) const
{
	return dot (point, normal ()) - distanceFromOrigin ();
}

///  Return true if @a line intersects with this plane otherwise false.  The intersection
///  point is stored in @a point.
template <class Type>
bool
plane3 <Type>::intersect (const line3 <Type> & line, vector3 <Type> & point) const
{
	// Check if the line is parallel to the plane.
	Type theta = dot (line .direction (), normal ());

	// Plane and line are parallel.
	if (theta == 0)
		return false;

	// Plane and line are not parallel. The intersection point can be calculated now.
	Type t = (distanceFromOrigin () - dot (normal (), line .point ())) / theta;

	point = line .point () + line .direction () * t;

	return true;
}
//@}
///@}

///  @relates plane3
///  @name Input/Output operations

///@{
//@{
///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, plane3 <Type> & plane)
{
	vector3 <Type> vector;

	istream >> vector;

	if (istream)
		plane = plane3 <Type> (vector);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const plane3 <Type> & plane)
{
	return ostream << plane .normal () * plane .distanceFromOrigin ();
}
//@}
///@}

extern template class plane3 <float>;
extern template class plane3 <double>;
extern template class plane3 <long double>;

//
extern template std::istream & operator >> (std::istream &, plane3 <float> &);
extern template std::istream & operator >> (std::istream &, plane3 <double> &);
extern template std::istream & operator >> (std::istream &, plane3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const plane3 <float> &);
extern template std::ostream & operator << (std::ostream &, const plane3 <double> &);
extern template std::ostream & operator << (std::ostream &, const plane3 <long double> &);

} // math
} // titania

#endif
