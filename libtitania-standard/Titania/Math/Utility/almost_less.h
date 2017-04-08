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

#ifndef __TITANIA_MATH_UTILITY_ALMOST_LESS_H__
#define __TITANIA_MATH_UTILITY_ALMOST_LESS_H__

#include "almost_equal.h"

#include <tuple>

namespace titania {
namespace math {

template <class Type>
struct fuzzy_value
{
	constexpr
	fuzzy_value (const Type value, const Type epsilon) :
		  value (value),
		epsilon (epsilon)
	{ }

	constexpr
	bool
	operator < (const fuzzy_value & fvalue) const
	{ return value < fvalue .value and not almost_equal (value, fvalue .value, epsilon); }

	const Type & value;
	const Type & epsilon;

};

template <class Type>
struct almost_less :
	public std::binary_function <Type, Type, bool>
{

	constexpr
	almost_less (const Type & epsilon) :
		epsilon (epsilon)
	{ }

	constexpr
	bool
	operator () (const Type & lhs, const Type & rhs) const
	{
		return fuzzy_value <Type> (lhs, epsilon) < fuzzy_value <Type> (rhs, epsilon);
	}

	const double epsilon;

};

template <>
struct almost_less <vector3 <double>> :
	public std::binary_function <vector3 <double>, vector3 <double>, bool>
{

	constexpr
	almost_less (const double & epsilon) :
		epsilon (epsilon)
	{ }

	bool
	operator () (const vector3 <double> & lhs, const vector3 <double> & rhs) const
	{
		using fvalue = fuzzy_value <double>;

		return std::make_tuple (fvalue (lhs .x (), epsilon), fvalue (lhs .y (), epsilon), fvalue (lhs .z (), epsilon)) <
		       std::make_tuple (fvalue (rhs .x (), epsilon), fvalue (rhs .y (), epsilon), fvalue (rhs .z (), epsilon));
	}

	const double epsilon;

};

} // math
} // titania

#endif
