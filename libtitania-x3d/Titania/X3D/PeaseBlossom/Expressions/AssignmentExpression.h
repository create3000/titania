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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ASSIGNMENT_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_ASSIGNMENT_EXPRESSION_H__

#include "../Execution/pbExecutionContext.h"
#include "../Expressions/AssignmentOperatorType.h"
#include "../Expressions/pbExpression.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript identifier expression.
 */
class AssignmentExpression :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new AssignmentExpression expression.
	AssignmentExpression (pbExecutionContext* const executionContext, var && lhs, var && rhs, AssignmentOperatorType type) :
		    pbExpression (),
		executionContext (executionContext),
		             lhs (lhs .isObject () ? lhs .getObject () : nullptr),
		             rhs (std::move (rhs)),
		            type (type)
	{ construct (); }

//	///  Creates a copy of this object.
//	virtual
//	var
//	copy (pbExecutionContext* const executionContext) const final override
//	{ return make_var <AssignmentExpression> (executionContext, lhs -> copy (executionContext), rhs -> copy (executionContext), type); }

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	toPrimitive () const final override
	{
		switch (type)
		{
			case AssignmentOperatorType::ASSIGNMENT:
				return lhs -> setValue (rhs .toPrimitive ());

			case AssignmentOperatorType::MULTIPLICATION_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toNumber () * rhs .toNumber ());
			}
			case AssignmentOperatorType::DIVISION_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toNumber () / rhs .toNumber ());
			}
			case AssignmentOperatorType::REMAINDER_ASSIGNMENT:
			{
				return lhs -> setValue (std::fmod (lhs -> toPrimitive () .toNumber (), rhs .toNumber ()));
			}
			case AssignmentOperatorType::ADDITION_ASSIGNMENT:
			{
				if (lhs -> toPrimitive () .getType () == STRING or rhs .getType () == STRING)
					return lhs -> setValue (lhs -> toPrimitive () .toString () + rhs .toString ());

				return lhs -> setValue (lhs -> toPrimitive () .toNumber () + rhs .toNumber ());
			}
			case AssignmentOperatorType::SUBTRACTION_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toNumber () - rhs .toNumber ());
			}
			case AssignmentOperatorType::LEFT_SHIFT_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toInt32 () << (rhs .toUInt32 () & 0x1f));
			}
			case AssignmentOperatorType::RIGHT_SHIFT_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toInt32 () >> (rhs .toUInt32 () & 0x1f));
			}
			case AssignmentOperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toUInt32 () >> (rhs .toUInt32 () & 0x1f));
			}
			case AssignmentOperatorType::BITWISE_AND_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toInt32 () & rhs .toInt32 ());
			}
			case AssignmentOperatorType::BITWISE_XOR_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toInt32 () ^ rhs .toInt32 ());
			}
			case AssignmentOperatorType::BITWISE_OR_ASSIGNMENT:
			{
				return lhs -> setValue (lhs -> toPrimitive () .toInt32 () | rhs .toInt32 ());
			}
		}

		return var ();
	}


private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{
		if (not lhs)
			throw ReferenceError ("Invalid assignment left-hand side.");

		addChildren (executionContext, lhs, rhs);
	}

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	const ptr <pbExpression>       lhs;
	const var                            rhs;
	const AssignmentOperatorType         type;

};

} // pb
} // titania

#endif
