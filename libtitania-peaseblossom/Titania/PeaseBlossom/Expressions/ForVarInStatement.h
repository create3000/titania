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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_FOR_VAR_IN_STATEMENT_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_FOR_VAR_IN_STATEMENT_H__

#include "../Expressions/PrimitiveExpression.h"
#include "../Expressions/VariableDeclaration.h"
#include "../Expressions/pbStatement.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript for in statement.
 */
class ForVarInStatement :
	public pbStatement
{
public:

	///  @name Construction

	///  Constructs new ForVarInStatement expression.
	ForVarInStatement (ptr <VariableDeclaration> && variableDeclaration, ptr <pbStatement> && expression, ptr <pbStatement> && statement) :
		       pbStatement (StatementType::FOR_VAR_IN_STATEMENT),
		variableDeclaration (std::move (variableDeclaration)),
		         expression (std::move (expression)),
		          statement (std::move (statement))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{
		return new ForVarInStatement (variableDeclaration -> copy (executionContext),
		                              expression -> copy (executionContext),
		                              statement -> copy (executionContext));
	}

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	///  throws pbError
	virtual
	CompletionType
	getValue () const final override
	{
		CompletionType result;

		const auto value = expression -> getValue ();

		if (value .getType () not_eq OBJECT)
			return undefined;

		const auto &        object = value .getObject ();
		std::string         propertyName;
		std::vector <void*> userData;

		if (object -> enumerate (ENUMERATE_BEGIN, propertyName, userData))
		{
			do
			{
				while (object -> enumerate (ENUMERATE, propertyName, userData))
				{
					variableDeclaration -> putValue (propertyName);

					const auto value = statement -> getValue ();

					if (value .getStatement ())
					{
						switch (value .getStatement () -> getType ())
						{
							case StatementType::BREAK_STATEMENT:
								return result;
							case StatementType::RETURN_STATEMENT:
								return value;
							default:
								continue;
						}
					}

					result = std::move (value);
				}
			}
			while (object -> enumerate (ENUMERATE_END, propertyName, userData));
		}

		return result;
	}

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		ostream
			<< "for"
			<< Generator::TidySpace
			<< '('
			<< "var"
			<< Generator::Space
			<< variableDeclaration
			<< Generator::Space
			<< "in"
			<< Generator::Space
			<< expression
			<< ')'
			<< Generator::TidyBreak
			<< pb::toStream (statement, true);
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (variableDeclaration, expression, statement); }

	///  @name Members

	const ptr <VariableDeclaration> variableDeclaration;
	const ptr <pbStatement>        expression;
	const ptr <pbStatement>        statement;

};

} // pb
} // titania

#endif
