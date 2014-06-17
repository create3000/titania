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

#include "jsValue.h"

#include "../Execution/GlobalObject.h"
#include "../Primitives/jsNumber.h"

namespace titania {
namespace pb {

constexpr double M_2_16 = 65536;
constexpr double M_2_31 = 2147483648;
constexpr double M_2_32 = 4294967296;

bool
jsValue::isPrimitive () const
{
	switch (getType ())
	{
		case UNDEFINED:
		case BOOLEAN:
		case NUMBER:
		case STRING:
		case NULL_OBJECT:
			return true;
		default:
			return false;
	}
}

uint16_t
jsValue::toUInt16 () const
{
	const double number = toNumber ();

	if (isNaN (number))
		return 0;

	if (number == jsNumber::POSITIVE_INFINITY ())
		return 0;

	if (number == jsNumber::NEGATIVE_INFINITY ())
		return 0;

	const double posInt   = std::copysign (std::floor (std::abs (number)), number);
	const double int16bit = std::fmod (posInt, M_2_16);

	return int16bit;
}

int32_t
jsValue::toInt32 () const
{
	const double int32bit = toInt32Bit ();

	if (int32bit >= M_2_31)
		return int32bit - M_2_32;

	return int32bit;
}

double
jsValue::toInt32Bit () const
{
	const double number = toNumber ();

	if (isNaN (number))
		return 0;

	if (number == jsNumber::POSITIVE_INFINITY ())
		return 0;

	if (number == jsNumber::NEGATIVE_INFINITY ())
		return 0;

	const double posInt   = std::copysign (std::floor (std::abs (number)), number);
	const double int32bit = std::fmod (posInt, M_2_32);

	return int32bit;
}

double
jsValue::toInteger () const
{
	const double number = toNumber ();

	return std::copysign (std::floor (std::abs (number)), number);
}

} // pb
} // titania
