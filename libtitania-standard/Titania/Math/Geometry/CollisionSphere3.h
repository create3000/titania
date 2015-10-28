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

#ifndef __TITANIA_MATH_GEOMETRY_ELLIPSOID3_H__
#define __TITANIA_MATH_GEOMETRY_ELLIPSOID3_H__

#include "../Geometry/Box3.h"
#include "../Geometry/Sphere3.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

/**
 *  Template to represent a collision sphere in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class collision_sphere3
{
public:

	///  Value typedef.
	using value_type = Type;

	///  @name Constructors

	///  Default constructor.
	constexpr
	collision_sphere3 () :
		collision_sphere3 (matrix4 <Type> (), 1, vector3 <Type> ())
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	collision_sphere3 (const collision_sphere3<Up> & other) :
		collision_sphere3 (other .matrix (), other .radius (), other .center ())
	{ }

	///  Constructs a line of from @a matrix.
	constexpr
	collision_sphere3 (const matrix4 <Type> & matrix, const Type & radius, const vector3 <Type> & center) :
		matrix_ (matrix),
		sphere_ (radius, center)
	{ }

	///  @name Element access

	///  Sets the matrix of this collision sphere.
	void
	matrix (const matrix4 <Type> & value)
	{ matrix_ = value; }

	///  Returns the matrix of this collision sphere.
	const matrix4 <Type> &
	matrix () const
	{ return matrix_; }

	///  Sets the radius of this collision sphere.
	void
	radius (const Type & value)
	{ sphere_ .radius (value); }

	///  Returns the radius of this collision sphere.
	const Type &
	radius () const
	{ return sphere_ .radius (); }

	///  Sets the center of this collision sphere.
	void
	center (const vector3 <Type> & value)
	{ sphere_ .center (value); }

	///  Returns the center of this collision sphere.
	const vector3 <Type> &
	center () const
	{ return sphere_ .center (); }

	///  Returns true if the triangle of points @a A, @a B and @a C intersects with this collision sphere.
	bool
	intersects (const vector3 <Type> &, const vector3 <Type> &, const vector3 <Type> &) const;

	bool
	intersects (const box3 <Type> & box) const;


private:

	matrix4 <Type> matrix_;
	sphere3 <Type> sphere_;

};

template <class Type>
inline
bool
collision_sphere3 <Type>::intersects (const vector3 <Type> & A, const vector3 <Type> & B, const vector3 <Type> & C) const
{
	return sphere_ .intersects (A * matrix (),
	                            B * matrix (),
	                            C * matrix ());
}

template <class Type>
inline
bool
collision_sphere3 <Type>::intersects (const box3 <Type> & box) const
{
	return (box * matrix ()) .intersects (sphere_);
}

///  @relates collision_sphere3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, collision_sphere3 <Type> & collision_sphere)
{
	matrix4 <Type> matrix;
	Type           radius;
	vector3 <Type> center;

	istream >> matrix >> radius >> center;

	if (istream)
		collision_sphere = collision_sphere3 <Type> (matrix, radius, center);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const collision_sphere3 <Type> & collision_sphere)
{
	return ostream
		<< collision_sphere .matrix ()
		<< "  " << collision_sphere .radius ()
		<< "  " << collision_sphere .center ();
}

extern template class collision_sphere3 <float>;
extern template class collision_sphere3 <double>;
extern template class collision_sphere3 <long double>;

//
extern template std::istream & operator >> (std::istream &, collision_sphere3 <float> &);
extern template std::istream & operator >> (std::istream &, collision_sphere3 <double> &);
extern template std::istream & operator >> (std::istream &, collision_sphere3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const collision_sphere3 <float> &);
extern template std::ostream & operator << (std::ostream &, const collision_sphere3 <double> &);
extern template std::ostream & operator << (std::ostream &, const collision_sphere3 <long double> &);

} // math
} // titania

#endif
