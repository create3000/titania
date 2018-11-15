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

#include "pbStatement.h"

#include "../Execution/pbExecutionContext.h"
#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

const std::string pbStatement::typeName = "Expression";

bool
pbStatement::isPrimitive () const
{
	switch (type)
	{
		case StatementType::UNDEFINED:
		case StatementType::BOOLEAN:
		case StatementType::NUMBER:
		case StatementType::BINARY_NUMBER:
		case StatementType::OCTAL_NUMBER:
		case StatementType::HEXAL_NUMBER:
		case StatementType::STRING:
		case StatementType::SINGLE_QUOTED_STRING:
		case StatementType::DOUBLE_QUOTED_STRING:
		case StatementType::NULL_OBJECT:
			return true;
		default:
			return false;
	}

	return false;
}

///  throws pbError
var
pbStatement::call (const ptr <pbExecutionContext> & executionContext, const std::vector <var> & arguments) const
{
	const auto value = getValue ();

	if (value .isObject ())
	{
		const auto function = dynamic_cast <pbFunction*> (value .getObject () .get ());

		if (function)
			return function -> call (undefined, arguments);
	}

	throw TypeError ("'" + value .toString () + "' is not a function.");
}

///  throws pbError
bool
pbStatement::deleteProperty () const
{
	getValue ();
	return true;
}

std::ostream &
operator << (std::ostream & ostream, const StatementsOutputType & value)
{
	bool          blankLine = true;
	StatementType lastType  = StatementType::EMPTY_STATEMENT;

	ostream << Generator::TidyBreak;

	for (const auto & statement : value .statements)
	{
		// Add blank line before special statements.

		if (not blankLine)
		{
			switch (statement -> getType ())
			{
				case StatementType::VARIABLE_STATEMENT :
					{
						if (lastType == StatementType::VARIABLE_STATEMENT)
							break;

						// Proceed with next case:
					}
				case StatementType::BLOCK_STATEMENT:
				case StatementType::IF_STATEMENT:
				case StatementType::FOR_IN_STATEMENT:
				case StatementType::FOR_VAR_IN_STATEMENT:
				case StatementType::FOR_STATEMENT:
				case StatementType::FOR_VAR_STATEMENT:
				case StatementType::RETURN_STATEMENT:
					ostream << Generator::TidyBreak;
					break;
				default:
					break;
			}
		}

		// Output statement.

		ostream
			<< toStream (statement);

		ostream << Generator::TidyBreak;

		// Add blank line after special statements.

		switch (statement -> getType ())
		{
			case StatementType::BLOCK_STATEMENT:
			case StatementType::IF_STATEMENT:
			case StatementType::FOR_IN_STATEMENT:
			case StatementType::FOR_VAR_IN_STATEMENT:
			case StatementType::FOR_STATEMENT:
			case StatementType::FOR_VAR_STATEMENT:
			case StatementType::RETURN_STATEMENT:
				ostream << Generator::TidyBreak;
				blankLine = true;
				break;
			default:
				blankLine = false;
				break;
		}

		lastType = statement -> getType ();
	}

	return ostream;
}

std::ostream &
operator << (std::ostream & ostream, const StatementOutputType & value)
{
	if (value .indent and value .statement -> getType () not_eq StatementType::BLOCK_STATEMENT)
		ostream << Generator::IncIndent;

	// Output statement.

	ostream
		<< Generator::Indent
		<< value .statement;

	// Add semicolon if needed.

	switch (value .statement -> getType ())
	{
		case StatementType::BLOCK_STATEMENT:
		case StatementType::IF_STATEMENT:
		case StatementType::FOR_IN_STATEMENT:
		case StatementType::FOR_VAR_IN_STATEMENT:
		case StatementType::FOR_STATEMENT:
		case StatementType::FOR_VAR_STATEMENT:
			break;
		default:
			ostream << ';';
			break;
	}

	if (value .indent and value .statement -> getType () not_eq StatementType::BLOCK_STATEMENT)
		ostream << Generator::DecIndent;

	return ostream;
}

std::ostream &
operator << (std::ostream & ostream, const ExpressionOutputType & value)
{
	// If statement has a higher precedence than parent, output parenthesises

	static const std::map <StatementType, size_t> precedences = { };

	return ostream
	       << value .statement;
}

} // pb
} // titania
