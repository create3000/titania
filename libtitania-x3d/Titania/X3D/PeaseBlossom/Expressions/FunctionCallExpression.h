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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_FUNCTION_CALL_EXPRESSION_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_EXPRESSIONS_FUNCTION_CALL_EXPRESSION_H__

#include "../Execution/pbExecutionContext.h"
#include "../Expressions/pbExpression.h"
#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript function call expression.
 */
class FunctionCallExpression :
	public pbExpression
{
public:

	///  @name Construction

	///  Constructs new FunctionCallExpression expression.
	FunctionCallExpression (pbExecutionContext* const executionContext, var && expression, std::vector <var> && expressions) :
		    pbExpression (),
		executionContext (executionContext),
		      expression (std::move (expression)),
		     expressions (std::move (expressions))
	{ construct (); }

//	///  Creates a copy of this object.
//	virtual
//	var
//	copy (pbExecutionContext* const executionContext) const final override
//	{
//		std::vector <var> expressions;
//
//		for (const auto & expression : this -> expressions)
//			expressions .emplace_back (expression -> copy (executionContext));
//
//		return make_var <FunctionCallExpression> (executionContext, expression -> copy (executionContext), std::move (expressions));
//	}

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	toPrimitive () const final override
	{
		const auto primitive = expression .toPrimitive ();

		if (primitive .isObject ())
		{
			const auto function  = dynamic_cast <pbFunction*> (primitive .getObject () .get ());

			if (function)
			{
				std::vector <var> arguments;

				for (const auto & value : expressions)
					arguments .emplace_back (value .toPrimitive ());

				return function -> call (executionContext -> getGlobalObject (), arguments);
			}
		}

		throw TypeError ("'" + primitive .toString () + "' is not a function.");
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{
		addChildren (executionContext, expression);

		for (const auto & value : expressions)
			addChild (value);
	}

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	const var                            expression;
	const std::vector <var>              expressions;

};

} // pb
} // titania

#endif
