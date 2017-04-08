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

#ifndef __TITANIA_MATH_UTILITY_ALMOST_EQUAL_H__
#define __TITANIA_MATH_UTILITY_ALMOST_EQUAL_H__

#include "../Numbers/Color3.h"
#include "../Numbers/Color4.h"
#include "../Numbers/Quaternion.h"
#include "../Numbers/Rotation4.h"
#include "../Numbers/Vector2.h"
#include "../Numbers/Vector3.h"
#include "../Numbers/Vector4.h"

namespace titania {
namespace math {

template <class Type>
constexpr
bool
almost_equal (const Type & lhs, const Type & rhs, const Type & epsilon)
{
	return std::abs (lhs - rhs) < epsilon;
}

template <class Type>
constexpr
bool
almost_equal (const color3 <Type> & lhs, const color3 <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .r (), rhs .r (), epsilon) and
	       almost_equal (lhs .g (), rhs .g (), epsilon) and
	       almost_equal (lhs .b (), rhs .b (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const color4 <Type> & lhs, const color4 <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .r (), rhs .r (), epsilon) and
	       almost_equal (lhs .g (), rhs .g (), epsilon) and
	       almost_equal (lhs .b (), rhs .b (), epsilon) and
	       almost_equal (lhs .a (), rhs .a (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const vector2 <Type> & lhs, const vector2 <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .x (), rhs .x (), epsilon) and
	       almost_equal (lhs .y (), rhs .y (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const vector3 <Type> & lhs, const vector3 <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .x (), rhs .x (), epsilon) and
	       almost_equal (lhs .y (), rhs .y (), epsilon) and
	       almost_equal (lhs .z (), rhs .z (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const vector4 <Type> & lhs, const vector4 <Type> & rhs, const int epsilon)
{
	return almost_equal (lhs .x (), rhs .x (), epsilon) and
	       almost_equal (lhs .y (), rhs .y (), epsilon) and
	       almost_equal (lhs .z (), rhs .z (), epsilon) and
	       almost_equal (lhs .w (), rhs .w (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const quaternion <Type> & lhs, const quaternion <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .x (), rhs .x (), epsilon) and
	       almost_equal (lhs .y (), rhs .y (), epsilon) and
	       almost_equal (lhs .z (), rhs .z (), epsilon) and
	       almost_equal (lhs .w (), rhs .w (), epsilon);
}

template <class Type>
constexpr
bool
almost_equal (const rotation4 <Type> & lhs, const rotation4 <Type> & rhs, const Type & epsilon)
{
	return almost_equal (lhs .quat (), rhs .quat (), epsilon)
	       or
	       almost_equal (-lhs .quat (), rhs .quat (), epsilon);
}

} // math
} // titania

#endif
