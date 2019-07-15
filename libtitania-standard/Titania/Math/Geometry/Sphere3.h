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

#ifndef __TITANIA_MATH_GEOMETRY_SPHERE3_H__
#define __TITANIA_MATH_GEOMETRY_SPHERE3_H__

#include "../Geometry/Line3.h"
#include "../Numbers/Matrix4.h"
#include "../Numbers/Vector3.h"

#include <tuple>

namespace titania {
namespace math {

/**
 *  Template to represent a sphere in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of values.
 */
template <class Type>
class sphere3
{
public:

	///  @name Member types

	using value_type = Type;

	///  @name Constructors

	///  Default constructor.
	constexpr
	sphere3 () :
		sphere3 (1, vector3 <Type> ())
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	sphere3 (const sphere3 <Up> & sphere) :
		sphere3 (sphere .radius (), sphere .center ())
	{ }

	///  Constructs a line of from @a radius and @a center.
	constexpr
	sphere3 (const Type & radius, const vector3 <Type> & center) :
		m_radius (radius),
		m_center (center)
	{ }

	///  @name Element access

	///  Sets the radius of this sphere.
	void
	radius (const Type & value)
	{ m_radius = value; }

	///  Returns the radius of this sphere.
	const Type &
	radius () const
	{ return m_radius; }

	///  Returns the center of this sphere.
	void
	center (const vector3 <Type> & value)
	{ m_center = value; }

	///  Returns the center of this sphere.
	const vector3 <Type> &
	center () const
	{ return m_center; }

	///  @name Intersection

	///  Returns true if the @a point intersects with this sphere.
	bool
	intersects (const vector3 <Type> & point) const
	{ return abs (point - center ()) <= radius (); }

	///  Returns true if the @a line intersects with this sphere.
	std::tuple <vector3 <Type>, vector3 <Type>, bool>
	intersects (const line3 <Type> & line) const;

	///  Returns true if the triangle of points @a A, @a B and @a C intersects with this sphere.
	bool
	intersects (vector3 <Type> A, vector3 <Type> B, vector3 <Type> C) const;


private:

	Type           m_radius;
	vector3 <Type> m_center;

};

template <class Type>
std::tuple <vector3 <Type>, vector3 <Type>, bool>
sphere3 <Type>::intersects (const line3 <Type> & line) const
{
	// https://github.com/Alexpux/Coin3D/blob/master/src/base/SbSphere.cpp
	const auto linepos = line .point ();
	const auto linedir = line .direction ();

	const auto scenter = center ();
	const auto r       = radius ();

	const auto b = 2 * (dot (linepos, linedir) - dot (scenter, linedir));
	const auto c = (linepos [0] * linepos [0] + linepos [1] * linepos [1] + linepos [2] * linepos [2]) +
	               (scenter [0] * scenter [0] + scenter [1] * scenter [1] + scenter [2] * scenter [2]) -
	               2 * dot (linepos, scenter) - r * r;

	const auto core = b * b - 4 * c;

	if (core >= 0)
	{
		auto t1 = (-b + std::sqrt (core)) / 2;
		auto t2 = (-b - std::sqrt (core)) / 2;

		if (t1 > t2)
			std::swap (t1, t2);

		const auto enter = linepos + t1 * linedir;
		const auto exit  = linepos + t2 * linedir;

		return std::tuple (enter, exit, true);
	}
	else
	{
		return std::tuple (vector3 <Type> (), vector3 <Type> (), false);
	}
}

// http://realtimecollisiondetection.net/blog/?p=103

template <class Type>
bool
sphere3 <Type>::intersects (vector3 <Type> A, vector3 <Type> B, vector3 <Type> C) const
{
	const auto P = center ();
	const auto r = radius ();

	A -= P;
	B -= P;
	C -= P;

	// Testing if sphere lies outside the triangle plane.
	const auto rr   = r * r;
	const auto V    = cross (B - A, C - A);
	const auto d    = dot (A, V);
	const auto e    = dot (V, V);
	const auto sep1 = d * d > rr * e;

	if (sep1)
		return false;

	// Testing if sphere lies outside a triangle vertex.
	const auto aa   = dot (A, A);
	const auto ab   = dot (A, B);
	const auto ac   = dot (A, C);
	const auto bb   = dot (B, B);
	const auto bc   = dot (B, C);
	const auto cc   = dot (C, C);
	const auto sep2 = (aa > rr) and (ab > aa) and (ac > aa);
	const auto sep3 = (bb > rr) and (ab > bb) and (bc > bb);
	const auto sep4 = (cc > rr) and (ac > cc) and (bc > cc);

	if (sep2 or sep3 or sep4)
		return false;

	// Testing if sphere lies outside a triangle edge.
	const auto AB   = B - A;
	const auto BC   = C - B;
	const auto CA   = A - C;
	const auto d1   = ab - aa;
	const auto d2   = bc - bb;
	const auto d3   = ac - cc;
	const auto e1   = dot (AB, AB);
	const auto e2   = dot (BC, BC);
	const auto e3   = dot (CA, CA);
	const auto Q1   = A * e1 - d1 * AB;
	const auto Q2   = B * e2 - d2 * BC;
	const auto Q3   = C * e3 - d3 * CA;
	const auto QC   = C * e1 - Q1;
	const auto QA   = A * e2 - Q2;
	const auto QB   = B * e3 - Q3;
	const auto sep5 = (dot (Q1, Q1) > rr * e1 * e1) and (dot (Q1, QC) > 0);
	const auto sep6 = (dot (Q2, Q2) > rr * e2 * e2) and (dot (Q2, QA) > 0);
	const auto sep7 = (dot (Q3, Q3) > rr * e3 * e3) and (dot (Q3, QB) > 0);

	if (sep5 or sep6 or sep7)
		return false;

	return true;
}

///  @relates sphere3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, sphere3 <Type> & sphere)
{
	Type radius;

	vector3 <Type> center;

	istream >> radius >> center;

	if (istream)
		sphere = sphere3 <Type> (radius, center);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const sphere3 <Type> & sphere)
{
	return ostream << sphere .radius () << "  " << sphere .center ();
}

extern template class sphere3 <float>;
extern template class sphere3 <double>;
extern template class sphere3 <long double>;

//
extern template std::istream & operator >> (std::istream &, sphere3 <float> &);
extern template std::istream & operator >> (std::istream &, sphere3 <double> &);
extern template std::istream & operator >> (std::istream &, sphere3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const sphere3 <float> &);
extern template std::ostream & operator << (std::ostream &, const sphere3 <double> &);
extern template std::ostream & operator << (std::ostream &, const sphere3 <long double> &);

} // math
} // titania

#endif
