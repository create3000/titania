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

#ifndef __TITANIA_MATH_UTILITY_ALMOST_EQUAL_H__
#define __TITANIA_MATH_UTILITY_ALMOST_EQUAL_H__

#include "../Numbers/Color3.h"
#include "../Numbers/Color4.h"
#include "../Numbers/Quaternion.h"
#include "../Numbers/Rotation4.h"
#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"
#include "../Numbers/Vector4.h"

#include <limits>
#include <type_traits>

namespace titania {
namespace math {

template <class T>
typename std::enable_if <not std::numeric_limits <T>::is_integer, bool>::type
almost_equal (const T & lhs, const T & rhs, const int unitsInTheLastPlace)
{
	// The machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place).
	// http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon

	const T difference = std::abs (lhs - rhs);

	return difference < std::numeric_limits <T>::epsilon () * std::abs (lhs + rhs) * unitsInTheLastPlace
	       // unless the result is subnormal
	       or difference < std::numeric_limits <T>::min ();
}

template <class Type>
bool
almost_equal (const color3 <Type> & lhs, const color3 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .r (), rhs .r (), unitsInTheLastPlace) and
	       almost_equal (lhs .g (), rhs .g (), unitsInTheLastPlace) and
	       almost_equal (lhs .b (), rhs .b (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const color4 <Type> & lhs, const color4 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .r (), rhs .r (), unitsInTheLastPlace) and
	       almost_equal (lhs .g (), rhs .g (), unitsInTheLastPlace) and
	       almost_equal (lhs .b (), rhs .b (), unitsInTheLastPlace) and
	       almost_equal (lhs .a (), rhs .a (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const vector2 <Type> & lhs, const vector2 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .x (), rhs .x (), unitsInTheLastPlace) and
	       almost_equal (lhs .y (), rhs .y (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const vector3 <Type> & lhs, const vector3 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .x (), rhs .x (), unitsInTheLastPlace) and
	       almost_equal (lhs .y (), rhs .y (), unitsInTheLastPlace) and
	       almost_equal (lhs .z (), rhs .z (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const vector4 <Type> & lhs, const vector4 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .x (), rhs .x (), unitsInTheLastPlace) and
	       almost_equal (lhs .y (), rhs .y (), unitsInTheLastPlace) and
	       almost_equal (lhs .z (), rhs .z (), unitsInTheLastPlace) and
	       almost_equal (lhs .w (), rhs .w (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const quaternion <Type> & lhs, const quaternion <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .x (), rhs .x (), unitsInTheLastPlace) and
	       almost_equal (lhs .y (), rhs .y (), unitsInTheLastPlace) and
	       almost_equal (lhs .z (), rhs .z (), unitsInTheLastPlace) and
	       almost_equal (lhs .w (), rhs .w (), unitsInTheLastPlace);
}

template <class Type>
bool
almost_equal (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs, const int unitsInTheLastPlace)
{
	return almost_equal (lhs .quat (), rhs .quat (), unitsInTheLastPlace)
	       or
	       almost_equal (-lhs .quat (), rhs .quat (), unitsInTheLastPlace);
}

} // math
} // titania

#endif
