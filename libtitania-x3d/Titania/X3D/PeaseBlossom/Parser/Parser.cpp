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

#include "Parser.h"

#include "../Expressions.h"
#include "../Parser/Grammar.h"

#include <Titania/Math/Utility/strtol.h>

namespace titania {
namespace pb {

Parser::Parser (std::istream & istream, jsScope* const scope) :
	    istream (istream),
	whiteSpaces ()
{ }

void
Parser::parseIntoContext ()
{
	//__LOG__ << std::endl;

	try
	{
		istream .imbue (std::locale::classic ());

		program ();
	}
	catch (const jsException & error)
	{
		throw;
	}
}

void
Parser::comments ()
{
	Grammar::WhiteSpaces (istream, whiteSpaces);
}

// A.1 Lexical Grammar

bool
Parser::identifier ()
{
	//__LOG__ << std::endl;
	
	comments ();

	const auto state    = istream .rdstate ();
	const auto position = istream .tellg ();

	std::string _identifierName;

	if (identifierName (_identifierName))
	{
		if (not reservedWord (_identifierName))
		{
			return true;
		}

		istream .clear (state);
		istream .seekg (position - istream .tellg (), std::ios_base::cur);
	}

	return false;
}

bool
Parser::identifierName (std::string & _identifierName)
{
	//__LOG__ << std::endl;


	if (identifierStart (_identifierName))
	{
		while (identifierStart (_identifierName) or identifierPart (_identifierName))
			;

		return true;
	}

	return false;
}

bool
Parser::identifierStart (std::string & _identifierStart)
{
	//__LOG__ << std::endl;

	// ...

	static const io::sequence IdentifierStart ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_");

	return IdentifierStart (istream, _identifierStart);
}

bool
Parser::identifierPart (std::string & _identifierPart)
{
	//__LOG__ << std::endl;

	// ...

	static const io::sequence UnicodeDigit ("1234567890");

	bool result = false;

	while (identifierStart (_identifierPart) or UnicodeDigit (istream, _identifierPart))
		result = true;

	return result;
}

bool
Parser::reservedWord (const std::string & _string)
{
	//__LOG__ << std::endl;

	if (Grammar::Keyword .count (_string))
		return true;

	if (Grammar::FutureReservedWord .count (_string))
		return true;

	if (_string == "null")
		return true;

	if (_string == "false")
		return true;

	if (_string == "true")
		return true;

	return false;
}

bool
Parser::literal (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (nullLiteral (value))
		return true;

	if (booleanLiteral (value))
		return true;

	if (numericLiteral (value))
		return true;

	//if (stringLiteral ())
	//	return true;

	//if (regularExpressionLiteral ())
	//	return true;

	return false;
}

bool
Parser::nullLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_null (istream))
	{
		value .reset (new ObjectValue ());
		return true;
	}

	return false;
}

bool
Parser::booleanLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_true (istream))
	{
		value .reset (new Boolean (true));
		return true;
	}

	if (Grammar::_false (istream))
	{
		value .reset (new Boolean (false));
		return true;
	}

	return false;
}

bool
Parser::numericLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (binaryIntegerLiteral (value))
		return true;

	if (octalIntegerLiteral (value))
		return true;

	if (hexIntegerLiteral (value))
		return true;

	if (decimalLiteral (value))
		return true;

	return false;
}

bool
Parser::decimalLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	double _number;

	if (istream >> std::dec >> _number)
	{
		value .reset (new Number (_number));
		return true;
	}

	istream .clear ();

	return false;
}

bool
Parser::binaryIntegerLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::bin (istream) or Grammar::BIN (istream))
	{
		std::string _digits;
	
		if (Grammar::BinaryDigits (istream, _digits))
		{
			value .reset (new Number (math::strtoul (_digits .c_str (), 2)));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::octalIntegerLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::oct (istream) or Grammar::OCT (istream))
	{
		uint32_t _number;

		if (istream >> std::oct >> _number)
		{
			value .reset (new Number (_number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::hexIntegerLiteral (ValuePtr & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t _number;

		if (istream >> std::hex >> _number)
		{
			value .reset (new Number (_number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;
	
	comments ();

	if (Grammar::_this (istream))
	{
		// ...
		return true;
	}

	if (identifier ())
		return true;

	if (literal (value))
		return true;

	// ...

	if (Grammar::OpenParenthesis (istream))
	{
		if (expression (value))
		{
			comments ();

			if (Grammar::CloseParenthesis (istream))
				return true;

			throw Exception <SYNTAX_ERROR> ("Expected ')' after expression.");
		}

		throw Exception <SYNTAX_ERROR> ("Expected expression after '('.");
	}

	return false;
}

bool
Parser::memberExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (primaryExpression (value))
		return true;

	// ...

	return false;
}

bool
Parser::newExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (memberExpression (value))
		return true;

	// ...

	return false;
}

bool
Parser::leftHandSideExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (newExpression (value))
		return true;

	//if (callExpression ())
	//	return true;

	return false;
}

bool
Parser::postfixExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (leftHandSideExpression (value))
		return true;

	// ...

	return false;
}

bool
Parser::unaryExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (postfixExpression (value))
		return true;

	// ...

	return false;
}

bool
Parser::multiplicativeExpression (ValuePtr & lhs)
{
	//__LOG__ << std::endl;

	if (unaryExpression (lhs))
	{
		comments ();
	
		ValuePtr rhs = undefined ();

		if (Grammar::Multiplication (istream))
		{
			if (multiplicativeExpression (rhs))
			{
				lhs .reset (multiplication (lhs, rhs));
				return true;
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '*'.");
		}

		if (Grammar::Division (istream))
		{
			if (multiplicativeExpression (rhs))
			{
				lhs .reset (division (lhs, rhs));
				return true;
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '/'.");
		}

		if (Grammar::Remainder (istream))
		{
			if (multiplicativeExpression (rhs))
			{
				lhs .reset (remainder (lhs, rhs));
				return true;
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '/'.");
		}

		return true;
	}

	return false;
}

bool
Parser::additiveExpression (ValuePtr & lhs)
{
	//__LOG__ << std::endl;
	
	if (multiplicativeExpression (lhs))
	{
		comments ();
	
		ValuePtr rhs = undefined ();

		if (Grammar::Addition (istream))
		{
			if (additiveExpression (rhs))
			{
				lhs .reset (addition (lhs, rhs));
				return true;
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '+'.");
		}

		if (Grammar::Subtraction (istream))
		{
			if (additiveExpression (rhs))
			{
				lhs .reset (subtraction (lhs, rhs));
				return true;
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '-'.");
		}

		return true;
	}

	return false;
}

bool
Parser::shiftExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (additiveExpression (value))
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::LeftShift (istream))
			{
				if (additiveExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '<<'.");
			}

			if (Grammar::UnsignedRightShift (istream))
			{
				if (additiveExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '>>>'.");
			}

			if (Grammar::RightShift (istream))
			{
				if (additiveExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '>>'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::relationalExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (shiftExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LessEqual (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '<='.");
			}

			if (Grammar::GreaterEqual (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '>='.");
			}

			if (Grammar::Less (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '<'.");
			}

			if (Grammar::Greater (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '>'.");
			}

			if (Grammar::instanceof (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after 'instanceof'.");
			}

			if (Grammar::in (istream))
			{
				if (shiftExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after 'in'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::equalityExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (relationalExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::StrictEqual (istream))
			{
				if (relationalExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '==='.");
			}

			if (Grammar::StrictNotEqual (istream))
			{
				if (relationalExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '!=='.");
			}

			if (Grammar::Equal (istream))
			{
				if (relationalExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '=='.");
			}

			if (Grammar::NotEqual (istream))
			{
				if (relationalExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '!='.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseANDExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (equalityExpression (value))
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::LogicalAND .lookahead (istream))
				return true;

			if (Grammar::BitwiseAND (istream))
			{
				if (equalityExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseXORExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression (value))
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::BitwiseXOR (istream))
			{
				if (bitwiseANDExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '^'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseORExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR .lookahead (istream))
				return true;
		
			if (Grammar::BitwiseOR (istream))
			{
				if (bitwiseXORExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '|'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalANDExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalAND (istream))
			{
				if (bitwiseORExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '&&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalORExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (logicalANDExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR (istream))
			{
				if (logicalANDExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after '||'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::conditionalExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (logicalORExpression (value))
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			if (assignmentExpression (value))
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					if (assignmentExpression (value))
					{
						return true;
					}
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after ':'.");
			}

			throw Exception <SYNTAX_ERROR> ("Expected expression after '?'.");
		}
	
		return true;
	}

	return false;
}

bool
Parser::assignmentExpression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	const auto state    = istream .rdstate ();
	const auto position = istream .tellg ();

	if (leftHandSideExpression (value))
	{
		comments ();

		if (Grammar::Assignment (istream))
		{
			if (assignmentExpression (value))
				return true;
			
			throw Exception <SYNTAX_ERROR> ("Expected expression after '='.");
		}

		OperatorType type;

		if (assignmentOperator (type))
		{
			if (assignmentExpression (value))
				return true;
	
			throw Exception <SYNTAX_ERROR> ("Expected expression after '='."); // XXX
		}

		istream .clear (state);
		istream .seekg (position - istream .tellg (), std::ios_base::cur);
	}

	if (conditionalExpression (value))
		return true;

	return false;
}

bool
Parser::assignmentOperator (OperatorType & type)
{
	comments ();

	if (Grammar::MultiplicationAssigment (istream))
	{
		type = OperatorType::MULTIPLICATION_ASSIGMENT;
		return true;
	}

	if (Grammar::DivisionAssignment (istream))
	{
		type = OperatorType::DIVISION_ASSIGNMENT;
		return true;
	}

	if (Grammar::RemainderAssignment (istream))
	{
		type = OperatorType::REMAINDER_ASSIGNMENT;
		return true;
	}

	if (Grammar::AdditionAssignment (istream))
	{
		type = OperatorType::ADDITION_ASSIGNMENT;
		return true;
	}

	if (Grammar::SubtractionAssignment (istream))
	{
		type = OperatorType::SUBTRACTION_ASSIGNMENT;
		return true;
	}

	if (Grammar::LeftShiftAssignment (istream))
	{
		type = OperatorType::LEFT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::RightShiftAssignment (istream))
	{
		type = OperatorType::RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::UnsignedRightShiftAssignment (istream))
	{
		type = OperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseANDAssignment (istream))
	{
		type = OperatorType::BITWISE_AND_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseXORAssignment (istream))
	{
		type = OperatorType::BITWISE_XOR_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseORAssignment (istream))
	{
		type = OperatorType::BITWISE_OR_ASSIGNMENT;
		return true;
	}

	return false;
}

bool
Parser::expression (ValuePtr & value)
{
	//__LOG__ << std::endl;

	if (assignmentExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression (value))
				{
					continue;
				}

				throw Exception <SYNTAX_ERROR> ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

//bool
//Parser::expression (ValuePtr & value)
//{
//	//__LOG__ << std::endl;
//
//	if (assignmentExpression (value))
//	{
//		comments ();
//
//		if (Grammar::Comma (istream))
//		{
//			if (expression (value))
//				return true;
//
//			throw Exception <SYNTAX_ERROR> ("Expected expression after ','.");
//		}
//
//		return true;
//	}
//
//	return false;
//}

// A.4 Statements

bool
Parser::statement ()
{
	//__LOG__ << std::endl;
	
	if (expressionStatement ())
		return true;

	if (emptyStatement ())
		return true;

	return false;
}

bool
Parser::expressionStatement ()
{
	//__LOG__ << std::endl;

	// [lookahead ? {{, function}]

	ValuePtr value = undefined ();

	if (expression (value))
	{
		__LOG__ << value << std::endl;

		if (Grammar::Semicolon (istream))
			return true;

		throw Exception <SYNTAX_ERROR> ("Expected ';' after expression.");
	}

	return false;
}

bool
Parser::emptyStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::Semicolon (istream))
		return true;

	return false;
}

// A.5 Functions and Programs

bool
Parser::functionDeclaration ()
{
	//__LOG__ << std::endl;

	return false;
}

void
Parser::program ()
{
	sourceElements ();
}

void
Parser::sourceElements ()
{
	//__LOG__ << std::endl;

	while (sourceElement ())
		;
}

bool
Parser::sourceElement ()
{
	//__LOG__ << std::endl;

	if (statement ())
		return true;

	if (functionDeclaration ())
		return true;

	return false;
}

} // pb
} // titania
