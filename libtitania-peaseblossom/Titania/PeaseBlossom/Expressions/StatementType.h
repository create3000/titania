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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_EXPRESSION_TYPE_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_EXPRESSION_TYPE_H__

#include <cstdint>

namespace titania {
namespace pb {

enum class StatementType :
	uint8_t
{
	UNDEFINED,
	BOOLEAN,
	NUMBER,
	BINARY_NUMBER,
	OCTAL_NUMBER,
	HEXAL_NUMBER,
	STRING,
	SINGLE_QUOTED_STRING,
	DOUBLE_QUOTED_STRING,
	NULL_OBJECT,

	ADDITION_EXPRESSION,
	ARRAY_LITERAL,
	ASSIGNMENT_EXPRESSION,
	BITWISE_AND_EXPRESSION,
	BITWISE_NOT_EXPRESSION,
	BLOCK_STATEMENT,
	BREAK_STATEMENT,
	COMPUTED_MEMBER_ACCESS_EXPRESSION,
	CONDITIONAL_EXPRESSION,
	CONTINUE_STATEMENT,
	DELETE_EXPRESSION,
	DIVISION_EXPRESSION,
	EMPTY_STATEMENT,
	EQUAL_EXPRESSION,
	EXPRESSION,
	FOR_IN_STATEMENT,
	FOR_STATEMENT,
	FOR_VAR_IN_STATEMENT,
	FOR_VAR_STATEMENT,
	FUNCTION_CALL_EXPRESSION,
	FUNCTION_EXPRESSION,
	GREATER_EQUAL_EXPRESSION,
	GREATER_EXPRESSION,
	IF_STATEMENT,
	IN_EXPRESSION,
	INSTANCE_OF_EXPRESSION,
	LEFT_SHIFT_EXPRESSION,
	LESS_EQUAL_EXPRESSION,
	LESS_EXPRESSION,
	LOGICAL_AND_EXPRESSION,
	LOGICAL_NOT_EXPRESSION,
	LOGICAL_OR_EXPRESSION,
	MEMBER_ACCESS_EXPRESSION,
	MULTIPLICATION_EXPRESSION,
	NEGATE_EXPRESSION,
	NEW_EXPRESSION,
	NOT_EQUAL_EXPRESSION,
	OBJECT_LITERAL,
	POST_DECREMENT_EXPRESSION,
	POST_INCREMENT_EXPRESSION,
	PRE_DECREMENT_EXPRESSION,
	PRE_INCREMENT_EXPRESSION,
	REMAINDER_EXPRESSION,
	RETURN_STATEMENT,
	STRICT_EQUAL_EXPRESSION,
	STRICT_NOT_EQUAL_EXPRESSION,
	SUBTRACTION_EXPRESSION,
	SWTICH_STATEMENT,
	TO_NUMBER_EXPRESSION,
	TYPEOF_EXPRESSION,
	VARIABLE_EXPRESSION,
	VARIABLE_DECLARATION,
	VARIABLE_STATEMENT,
	VOID_EXPRESSION,

};

} // pb
} // titania

#endif
