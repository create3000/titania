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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_SWITCH_STATEMENT_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_SWITCH_STATEMENT_H__

#include "../Expressions/pbStatement.h"
#include "../Expressions/StrictEqualExpression.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript switch statement.
 */
class SwitchStatement :
	public pbStatement
{
public:

	///  @name Construction

	///  Constructs new SwitchStatement expression.
	SwitchStatement (ptr <pbStatement> && expression,
	                 array <ptr <pbStatement>> && clauseSelectors,
	                 array <array <ptr <pbStatement>>> && clauseStatements,
	                 array <ptr <pbStatement>> && defaultStatements) :
		      pbStatement (StatementType::SWTICH_STATEMENT),
		       expression (std::move (expression)),
		  clauseSelectors (std::move (clauseSelectors)),
		   clauseStatements (std::move (clauseStatements)),
		defaultStatements (std::move (defaultStatements))
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{
		array <ptr <pbStatement>>         clauseSelectors;
		array <array <ptr <pbStatement>>> clauseStatements;
		size_t                            defaultSelector = -1L;
		array <ptr <pbStatement>>         defaultStatements;
		size_t                            i = 0;

		for (const auto & clauseSelector : this -> clauseSelectors)
		{
			if (clauseSelector)
				clauseSelectors .emplace_back (clauseSelector -> copy (executionContext));
			else
			{
				defaultSelector = i;
				clauseSelectors .emplace_back ();
			}

			++ i;
		}

		for (const auto & statements : this -> clauseStatements)
		{
			clauseStatements .emplace_back ();

			for (const auto & statement : statements)
				clauseStatements .back () .emplace_back (statement -> copy (executionContext));
		}

		if (defaultSelector not_eq size_t (-1L))
			defaultStatements = clauseStatements [defaultSelector];

		return new SwitchStatement (expression -> copy (executionContext),
		                            std::move (clauseSelectors),
		                            std::move (clauseStatements),
		                            std::move (defaultStatements));
	}

	///  @name Operations

	///  Converts its input argument to either Primitive or Object type.
	///  throws pbError
	virtual
	CompletionType
	getValue () const final override
	{
		CompletionType result (this, undefined);

		const auto input = expression -> getValue ();
		bool       found = false;

		for (size_t i = 0, size = clauseSelectors .size (); i < size; ++ i)
		{
			if (found || (clauseSelectors [i] and StrictEqualExpression::evaluate (input, clauseSelectors [i] -> getValue ())))
			{
				found = true;
	
				for (const auto & statement : clauseStatements [i])
				{
					const auto value = statement -> getValue ();
					
					if (value .getStatement ())
					{
						switch (value .getStatement () -> getType ())
						{
							case StatementType::BREAK_STATEMENT:
								return result;
							case StatementType::CONTINUE_STATEMENT:
							case StatementType::RETURN_STATEMENT:
								return value;
							default:
								continue;
						}
					}

					result = std::move (value);
				}
			}
		}

		if (not found)
		{
			for (const auto & statement : defaultStatements)
			{
				const auto value = statement -> getValue ();
				
				if (value .getStatement ())
				{
					switch (value .getStatement () -> getType ())
					{
						case StatementType::BREAK_STATEMENT:
							return result;
						case StatementType::CONTINUE_STATEMENT:
						case StatementType::RETURN_STATEMENT:
							return value;
						default:
							continue;
					}
				}

				result = std::move (value);
			}
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
			<< "switch"
			<< Generator::TidySpace
			<< '('
			<< expression
			<< ')';

		if (clauseSelectors .empty () and defaultStatements .empty ())
		{
			ostream
				<< Generator::TidySpace
				<< '{'
				<< Generator::TidySpace
				<< '}';
		}
		else
		{
			ostream
				<< Generator::TidyBreak
				<< Generator::Indent
				<< '{'
				<< Generator::IncIndent
				<< Generator::TidyBreak;

			for (size_t i = 0, size = clauseSelectors .size (); i < size; ++ i)
			{
				if (clauseSelectors [i])
				{
					ostream
						<< Generator::Indent
						<< "case"
						<< Generator::Space
						<< clauseSelectors [i]
						<< ':'
						<< Generator::TidyBreak;
				}
				else
				{
					ostream
						<< Generator::Indent
						<< "default:"
						<< Generator::TidyBreak;
				}

				for (const auto & statement : clauseStatements [i])
				{
					ostream
						<< pb::toStream (statement, true)
						<< Generator::TidyBreak;
				}
			}

			ostream
				<< Generator::DecIndent
				<< Generator::Indent
				<< '}';
		}
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (expression, clauseSelectors, clauseStatements, defaultStatements); }

	///  @name Members

	const ptr <pbStatement>                   expression;
	const array <ptr <pbStatement>>           clauseSelectors;
	const array <array <ptr <pbStatement>>>   clauseStatements;
	const array <ptr <pbStatement>>           defaultStatements;

};

} // pb
} // titania

#endif
