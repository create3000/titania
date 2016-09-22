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

#ifndef __TITANIA_MATH_GEOMETRY_PLANE3_H__
#define __TITANIA_MATH_GEOMETRY_PLANE3_H__

#include "../Geometry/Line3.h"
#include "../Geometry/Triangle3.h"
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

	///  Default constructor. Constructs a plane in the Z=0 plane with distance 0.
	constexpr
	plane3 () :
		              m_normal (0, 0, 1),
		m_distance_from_origin (0)
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	plane3 (const plane3 <Up> & plane) :
		              m_normal (plane .normal ()),
		m_distance_from_origin (plane .distance_from_origin ())
	{ }

	///  Constructs a plane from @a point1, @a point2 and @a point3. The normal is calculated
	///  counter clockwise from the points.
	plane3 (const vector3 <Type> & point1, const vector3 <Type> & point2, const vector3 <Type> & point3) :
		plane3 (point1, math::normal (point1, point2, point3))
	{ }

	///  Constructs a plane from @a point and @a normal.
	constexpr
	plane3 (const vector3 <Type> & point, const vector3 <Type> & normal) :
		              m_normal (normal),
		m_distance_from_origin (dot (normal, point))
	{ }

	///  Constructs a plane from @a normal and @a distance_from_origin.
	constexpr
	plane3 (const vector3 <Type> & normal, const Type & distance_from_origin) :
		              m_normal (normal),
		m_distance_from_origin (distance_from_origin)
	{ }

	///  @name Element access

	///  Returns the normal of this plane.
	const vector3 <Type> &
	normal () const
	{ return m_normal; }

	///  Returns the distance form origin.
	const Type &
	distance_from_origin () const
	{ return m_distance_from_origin; }
	
	///  @name  Arithmetic operations
	///  All these operators modify this box3 inplace.

	///  Transform this line by @a matrix.
	plane3 &
	operator *= (const matrix4 <Type> & matrix)
	throw (std::domain_error)
	{
		mult_right (matrix);
		return *this;
	}

	///  Transform this plane by @a matrix.
	void
	mult_right (const matrix4 <Type> &)
	throw (std::domain_error);

	///  Transform this plane by @a matrix.
	void
	mult_left (const matrix4 <Type> &)
	throw (std::domain_error);

	//  @name Distance

	///  Returns the distance from @a point.
	constexpr Type
	distance (const vector3 <Type> &) const;

	///  Returns the closest point on the plane to a given point @a point.
	vector3 <Type>
	closest_point (const vector3 <Type> & point)
	{
		vector3 <Type> closest_point;
		intersects (line3 <Type> (point, m_normal), closest_point);
		return closest_point;
	}

	///  Returns true if @a line intersects with this box3. The intersection point is stored in @a point.
	bool
	intersects (const line3 <Type> &, vector3 <Type> &) const;


private:

	vector3 <Type> m_normal;
	Type           m_distance_from_origin;

};

///  @name Arithmetic operations

///  Transform a plane by the given matrix
template <class Type>
void
plane3 <Type>::mult_right (const matrix4 <Type> & matrix)
throw (std::domain_error)
{
	// Taken from Inventor:

	// Find the point on the plane along the normal from the origin
	auto	point = distance_from_origin () * normal ();

	// Transform the plane normal by the matrix
	// to get the new normal. Use the inverse transpose
	// of the matrix so that normals are not scaled incorrectly.
	// n' = n * !~m = ~m * n
	const auto inv       = inverse (matrix);
	const auto newNormal = normalize (inv .mult_matrix_dir (normal ()));

	// Transform the point by the matrix
	point = matrix .mult_vec_matrix (point);

	// The new distance is the projected distance of the vector to the
	// transformed point onto the (unit) transformed normal. This is
	// just a dot product.
	*this = plane3 (point, newNormal);
}

template <class Type>
void
plane3 <Type>::mult_left (const matrix4 <Type> & matrix)
throw (std::domain_error)
{
	// Taken from Inventor:

	// Find the point on the plane along the normal from the origin
	auto	point = distance_from_origin () * normal ();

	// Transform the plane normal by the matrix
	// to get the new normal. Use the inverse transpose
	// of the matrix so that normals are not scaled incorrectly.
	// n' = !~m * n = n * ~m
	const auto inv       = inverse (matrix);
	const auto newNormal = normalize (inv .mult_dir_matrix (normal ()));

	// Transform the point by the matrix
	point = matrix .mult_matrix_vec (point);

	// The new distance is the projected distance of the vector to the
	// transformed point onto the (unit) transformed normal. This is
	// just a dot product.
	*this = plane3 (point, newNormal);
}

///  Returns the distance from @a point.
///  The value can be both negative and positive depending
///  on the direction of the normal vector of this plane. If the point lies in the upper half
///  space the value will be positive otherwise a negative value is returned.
template <class Type>
inline
constexpr Type
plane3 <Type>::distance (const vector3 <Type> & point) const
{
	return dot (point, normal ()) - distance_from_origin ();
}

///  Return true if @a line intersects with this plane otherwise false.  The intersection
///  point is stored in @a point.
template <class Type>
bool
plane3 <Type>::intersects (const line3 <Type> & line, vector3 <Type> & point) const
{
	// Check if the line is parallel to the plane.
	const Type theta = dot (line .direction (), normal ());

	// Plane and line are parallel.
	if (theta == 0)
		return false;

	// Plane and line are not parallel. The intersection point can be calculated now.
	const Type t = (distance_from_origin () - dot (normal (), line .point ())) / theta;

	point = line .point () + line .direction () * t;

	return true;
}

///  @relates plane3
///  @name Arithmetic operations

///  Extraction operator for vector values.
template <class Type>
plane3 <Type>
operator * (const plane3 <Type> & plane, const matrix4 <Type> & matrix)
throw (std::domain_error)
{
	plane3 <Type> result (plane);
	result .mult_right (matrix);
	return result;
}

template <class Type>
plane3 <Type>
operator * (const matrix4 <Type> & matrix, const plane3 <Type> & plane)
throw (std::domain_error)
{
	plane3 <Type> result (plane);
	result .mult_left (matrix);
	return result;
}

///  @relates plane3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, plane3 <Type> & plane)
{
	vector3 <Type> normal;
	Type           distance_from_origin;

	istream >> normal >> distance_from_origin;

	if (istream)
		plane = plane3 <Type> (normal, distance_from_origin);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const plane3 <Type> & plane)
{
	return ostream << plane .normal () << " " << plane .distance_from_origin ();
}

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
