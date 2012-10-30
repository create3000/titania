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
 ******************************************************************************/

#ifndef __TITANIA_MATH_GEOMETRY_SPHERE3_H__
#define __TITANIA_MATH_GEOMETRY_SPHERE3_H__

#include "../Numbers/Vector3.h"

namespace titania {
namespace math {

template <class Type>
class sphere3
{
public:

	constexpr
	sphere3 () :
		value ()
	{ }

	constexpr
	sphere3 (const sphere3 & sphere) :
		value { sphere .radius (), sphere .center () }

	{ }

	constexpr
	sphere3 (const Type & radius, const vector3 <Type> & center) :
		value { radius, center }

	{ }

	const Type &
	radius () const { return value .radius; }

	const vector3 <Type> &
	center () const { return value .center; }


private:

	struct Value
	{
		Type radius;
		vector3 <Type> center;
	};

	Value value;

};

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
