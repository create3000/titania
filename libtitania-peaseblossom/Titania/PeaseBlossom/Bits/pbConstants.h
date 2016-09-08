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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BITS_VS_CONSTANTS_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BITS_VS_CONSTANTS_H__

#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>

#undef NONE

namespace titania {
namespace pb {

/**
 *  Enum type for ECMAScript primitives.
 */
enum ValueType :
	uint8_t
{
	// Standard object

	UNDEFINED,
	BOOLEAN,
	NUMBER,
	STRING,
	NULL_OBJECT,
	OBJECT,

};

///  @relates ValueType
///  @name Input/Output operators.

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const ValueType type)
noexcept (true)
{
	switch (type)
	{
		case UNDEFINED:   ostream << "UNDEFINED"; break;
		case BOOLEAN:     ostream << "BOOLEAN";   break;
		case NUMBER:      ostream << "NUMBER";    break;
		case STRING:      ostream << "STRING";    break;
		case NULL_OBJECT: ostream << "NULL";      break;
		case OBJECT:      ostream << "OBJECT";    break;
	}

	return ostream;
}

inline
std::string
to_string (const ValueType type)
noexcept (true)
{
	std::ostringstream osstream;

	osstream << type;

	return osstream .str ();
}

using AttributeType = uint64_t;

constexpr AttributeType NONE         = 0ULL;
constexpr AttributeType WRITABLE     = 1ULL << 32;
constexpr AttributeType ENUMERABLE   = 1ULL << 33;
constexpr AttributeType CONFIGURABLE = 1ULL << 34;
constexpr AttributeType PROPERTY     = 0xffffffff;
constexpr AttributeType DEFAULT      = WRITABLE | CONFIGURABLE | ENUMERABLE;

enum EnumerateType :
	uint8_t
{
	// Enumerate cases:

	ENUMERATE_BEGIN,
	ENUMERATE,
	ENUMERATE_END

};

} // pb
} // titania

#endif
