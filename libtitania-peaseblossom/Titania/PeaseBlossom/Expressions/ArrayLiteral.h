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

#ifndef __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_ARRAY_LITERAL_H__
#define __TITANIA_PEASE_BLOSSOM_EXPRESSIONS_ARRAY_LITERAL_H__

#include "../Expressions/pbStatement.h"
#include "../Objects/Array.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace pb {

/**
 *  Class to represent a ECMAScript array literal expression.
 */
class ArrayLiteral :
	public pbStatement
{
public:

	///  @name Construction

	///  Constructs new ArrayLiteral expression.
	ArrayLiteral (pbExecutionContext* const executionContext) :
		    pbStatement (StatementType::ARRAY_LITERAL),
		executionContext (executionContext),
		     identifiers (),
		     expressions ()
	{ construct (); }

	///  Creates a copy of this object.
	virtual
	ptr <pbStatement>
	copy (pbExecutionContext* const executionContext) const
	noexcept (true) final override
	{
		const auto copy = new ArrayLiteral (executionContext);

		for (const auto & expression : expressions)
			copy -> addExpression (expression -> copy (executionContext));

		return copy;
	}

	///  @name Operations

	void
	addExpression (ptr <pbStatement> && value)
	{
		identifiers .emplace_back (basic::to_string (identifiers .size (), std::locale::classic ()));
		expressions .emplace_back (std::move (value));
	}

	///  Converts its input argument to either Primitive or Object type.
	///  throws pbError
	virtual
	CompletionType
	getValue () const final override
	{
		const auto array = new Array (executionContext .get ());

		for (uint32_t index = 0, size = expressions .size (); index < size; ++ index)
			array -> put (identifiers [index], expressions [index] -> getValue (), false);

		return array;
	}

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (expressions .empty ())
		{
			ostream
				<< '['
				<< Generator::TidySpace
				<< ']';
		}
		else
		{
			ostream
				<< '['
				<< Generator::IncIndent
				<< Generator::TidyBreak;

			for (const auto & expression : std::pair (expressions .begin (), expressions .end () - 1))
			{
				ostream << Generator::Indent;

				if (expression -> getType () not_eq StatementType::UNDEFINED)
				{
					ostream << expression;
				}

				ostream
					<< ','
					<< Generator::TidyBreak;
			}

			if (expressions .back () -> getType () not_eq StatementType::UNDEFINED)
			{
				ostream
					<< Generator::Indent
					<< expressions .back ();
			}
			else
			{
				ostream
					<< Generator::Indent
					<< ',';
			}

			ostream
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';
		}
	}

private:

	///  @name Construction

	///  Performs neccessary operations after construction.
	void
	construct ()
	{ addChildren (executionContext, expressions); }

	///  @name Members

	const ptr <pbExecutionContext> executionContext;
	std::vector <Identifier>       identifiers;
	array <ptr <pbStatement>>     expressions;

};

} // pb
} // titania

#endif
