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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BITS_JS_CONSTANTS_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BITS_JS_CONSTANTS_H__

namespace titania {
namespace pb {

#undef NONE

/**
 *  Enum type for JavaScript primitives.
 */
enum ValueType
{
	// Standard object

	UNDEFINED,
	BOOLEAN,
	NUMBER,
	STRING,
	NULL_OBJECT,
	OBJECT,
	BOOLEAN_OBJECT,
	NUMBER_OBJECT,
	STRING_OBJECT,
	ARRAY_OBJECT,
	DATE_OBJECT,
	REGEX_OBJECT,
	FUNCTION_OBJECT,

	// Expressions

	ADDITION,
	DIVISION,
	FUNCTION_CALL,
	LEFT_SHIFT,
	MULTIPLICATION,
	REMAINDER,
	RETURN,
	SUBTRACTION,
	VARIABLE,
	VARIABLE_DECLARATION,

	// Exotic objects

	CUSTOM_OBJECT

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
		// Standard object

		case UNDEFINED:            ostream << "UNDEFINED";            break;
		case BOOLEAN:              ostream << "BOOLEAN";              break;
		case NUMBER:               ostream << "NUMBER";               break;
		case STRING:               ostream << "STRING";               break;
		case NULL_OBJECT:          ostream << "NULL_OBJECT";          break;
		case OBJECT:               ostream << "OBJECT";               break;
		case BOOLEAN_OBJECT:       ostream << "BOOLEAN_OBJECT";       break;
		case NUMBER_OBJECT:        ostream << "NUMBER_OBJECT";        break;
		case STRING_OBJECT:        ostream << "STRING_OBJECT";        break;
		case ARRAY_OBJECT:         ostream << "ARRAY_OBJECT";         break;
		case DATE_OBJECT:          ostream << "DATE_OBJECT";          break;
		case REGEX_OBJECT:         ostream << "REGEX_OBJECT";         break;
		case FUNCTION_OBJECT:      ostream << "FUNCTION_OBJECT";      break;

		// Expressions

		case ADDITION:             ostream << "ADDITION";             break;
		case DIVISION:             ostream << "DIVISION";             break;
		case FUNCTION_CALL:        ostream << "FUNCTION_CALL";        break;
		case SUBTRACTION:          ostream << "SUBTRACTION";          break;
		case LEFT_SHIFT:           ostream << "LEFT_SHIFT";           break;
		case MULTIPLICATION:       ostream << "MULTIPLICATION";       break;
		case REMAINDER:            ostream << "REMAINDER";            break;
		case RETURN:               ostream << "RETURN";               break;
		case VARIABLE:             ostream << "VARIABLE";             break;
		case VARIABLE_DECLARATION: ostream << "VARIABLE_DECLARATION"; break;

		// Exotic objects

		case CUSTOM_OBJECT:        ostream << "CUSTOM_OBJECT";        break;
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

using PropertyFlagsType = uint8_t;

constexpr PropertyFlagsType DEFAULT      = 0;
constexpr PropertyFlagsType WRITABLE     = 1 << 0;
constexpr PropertyFlagsType ENUMERABLE   = 1 << 1;
constexpr PropertyFlagsType CONFIGURABLE = 1 << 2;

} // pb
} // titania

#endif
