/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_MATH_GEOMETRY_SPHEROID3_H__
#define __TITANIA_MATH_GEOMETRY_SPHEROID3_H__

#include "../../LOG.h"

namespace titania {
namespace math {

struct inv_flattening_type { };

/**
 *  Template to represent a spheroid in 3D space.
 *
 *  Extern instantiations for float, double, and long double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of matrix values.
 */
template <class Type>
class spheroid3
{
public:

	///  Value typedef.
	typedef Type value_type;

	///  @name Constructors

	///  Default constructor.
	constexpr
	spheroid3 () :
		value ()
	{ }

	///  Copy constructor.
	template <class Up>
	constexpr
	spheroid3 (const spheroid3 <Up> & sphere) :
		value { sphere .a (), sphere .c () }

	{ }

	///  Constructs a spheroid of from @a a semi-major axis and @a c semi-minor axis.
	constexpr
	spheroid3 (const Type & a, const Type & c) :
		value { a, c }

	{ }

	///  Constructs a spheroid of from @a a semi-major axis and @a f_1 inverse flattening.
	constexpr
	spheroid3 (const Type & a, const Type & f_1, const inv_flattening_type &) :
		spheroid3 (a, a * (1 - 1 / f_1))
	{ }

	///  @name Element access

	///  Returns the semi-major axis of this spheroid.
	const Type &
	a () const
	{ return value .a; }

	///  Returns the semi-minor axis of this spheroid.
	const Type &
	c () const
	{ return value .c; }

	///  Returns the semi-minor axis of this spheroid.
	Type
	f () const
	{ return 1 - c () / a (); }


private:

	struct Value
	{
		Type a;
		Type c;
	};

	Value value;

};

///  @relates spheroid3
///  @name Input/Output operations

///  Extraction operator for vector values.
template <class CharT, class Traits, class Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, spheroid3 <Type> & spheroid)
{
	Type a, c;

	istream >> a >> c;

	if (istream)
		spheroid = spheroid3 <Type> (a, c);

	return istream;
}

///  Insertion operator for vector values.
template <class CharT, class Traits, class Type>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const spheroid3 <Type> & spheroid)
{
	return ostream << spheroid .a () << "  " << spheroid .c ();
}

extern template class spheroid3 <float>;
extern template class spheroid3 <double>;
extern template class spheroid3 <long double>;

//
extern template std::istream & operator >> (std::istream &, spheroid3 <float> &);
extern template std::istream & operator >> (std::istream &, spheroid3 <double> &);
extern template std::istream & operator >> (std::istream &, spheroid3 <long double> &);

//
extern template std::ostream & operator << (std::ostream &, const spheroid3 <float> &);
extern template std::ostream & operator << (std::ostream &, const spheroid3 <double> &);
extern template std::ostream & operator << (std::ostream &, const spheroid3 <long double> &);

} // math
} // titania

#endif
