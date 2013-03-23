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

#ifndef __TITANIA_MATH_GEOMETRY_SPHERE3_H__
#define __TITANIA_MATH_GEOMETRY_SPHERE3_H__

#include "../Numbers/Vector3.h"
#include "../Numbers/Matrix4.h"
#include "../Geometry/Line3.h"

#include "../../LOG.h"

namespace titania {
namespace math {

template <class Type>
class sphere3
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor.
	constexpr
	sphere3 () :
		value ()
	{ }

	///  Copy constructor.
	constexpr
	sphere3 (const sphere3 & sphere) :
		value { sphere .radius (), sphere .center () }

	{ }

	///  Constructs a line of from @a radius and @a center.
	constexpr
	sphere3 (const Type & radius, const vector3 <Type> & center) :
		value { radius, center }

	{ }
	
	///  @name Element access

	///  Returns the radius of this sphere.
	const Type &
	radius () const { return value .radius; }

	///  Returns the center of this sphere.
	const vector3 <Type> &
	center () const { return value .center; }
	
	///  @name Intersection
	
	///  Returns true if the @a line intersects with this sphere.
	bool
	intersect (const line3 <Type> &, vector3 <Type> &, vector3 <Type> &) const;

	///  Returns true if the triangle of points @a A, @a B and @a C intersects with this sphere.
	bool
	intersect (vector3 <Type>, vector3 <Type>, vector3 <Type>) const;


private:

	struct Value
	{
		Type radius;
		vector3 <Type> center;
	};

	Value value;

};

template <class Type>
bool
sphere3 <Type>::intersect (const line3 <Type> & line, vector3 <Type> & intersection1, vector3 <Type> & intersection2) const
{
	auto L   = center () - line .origin ();
	Type tca = dot (L, line .direction ());

	if (tca < 0)
		// there is no intersection
		return false;

	Type d2 = dot (L, L) - std::pow (tca, 2);
	Type r2 = std::pow (radius (), 2);

	if (d2 > r2)
		return false;
		
	Type thc = std::sqrt (r2 - d2);
	
	Type t1 = tca - thc;
	Type t2 = tca + thc;

	intersection1 = line .origin () + line .direction () * t1;
	intersection2 = line .origin () + line .direction () * t2;
	
	return true;
}

// http://realtimecollisiondetection.net/blog/?p=103

template <class Type>
bool
sphere3 <Type>::intersect (vector3 <Type> A, vector3 <Type> B, vector3 <Type> C) const
{
	auto P = center ();
	auto r = radius ();

	A = A - P;
	B = B - P;
	C = C - P;

	// Testing if sphere lies outside the triangle plane.
	auto rr   = r * r;
	auto V    = cross (B - A, C - A);
	auto d    = dot (A, V);
	auto e    = dot (V, V);
	auto sep1 = d * d > rr * e;

	if (sep1)
		return false;

	// Testing if sphere lies outside a triangle vertex.
	auto aa   = dot (A, A);
	auto ab   = dot (A, B);
	auto ac   = dot (A, C);
	auto bb   = dot (B, B);
	auto bc   = dot (B, C);
	auto cc   = dot (C, C);
	auto sep2 = (aa > rr) and (ab > aa) and (ac > aa);
	auto sep3 = (bb > rr) and (ab > bb) and (bc > bb);
	auto sep4 = (cc > rr) and (ac > cc) and (bc > cc);

	if (sep2 or sep3 or sep4)
		return false;

	// Testing if sphere lies outside a triangle edge.
	auto AB   = B - A;
	auto BC   = C - B;
	auto CA   = A - C;
	auto d1   = ab - aa;
	auto d2   = bc - bb;
	auto d3   = ac - cc;
	auto e1   = dot (AB, AB);
	auto e2   = dot (BC, BC);
	auto e3   = dot (CA, CA);
	auto Q1   = A * e1 - d1 * AB;
	auto Q2   = B * e2 - d2 * BC;
	auto Q3   = C * e3 - d3 * CA;
	auto QC   = C * e1 - Q1;
	auto QA   = A * e2 - Q2;
	auto QB   = B * e3 - Q3;
	auto sep5 = (dot (Q1, Q1) > rr * e1 * e1) and (dot (Q1, QC) > 0);
	auto sep6 = (dot (Q2, Q2) > rr * e2 * e2) and (dot (Q2, QA) > 0);
	auto sep7 = (dot (Q3, Q3) > rr * e3 * e3) and (dot (Q3, QB) > 0);

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
