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

#include "../../Execution/X3DExecutionContext.h"

#include "Parser.h"

#include "../Parser/Grammar.h"

#include <Titania/Math/Utility/strtol.h>

namespace titania {
namespace pb {

const std::string Parser::componentName  = "PeaseBlossom";
const std::string Parser::typeName       = "Parser";
const std::string Parser::containerField = "parser";

Parser::Parser (std::istream & istream, X3D::X3DExecutionContext* const executionContext) :
	X3D::X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3D::X3DParser (),
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
	catch (const X3D::X3DError & error)
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
Parser::literal (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	if (nullLiteral (_value))
		return true;

	if (booleanLiteral (_value))
		return true;

	if (numericLiteral (_value))
		return true;

	//if (stringLiteral ())
	//	return true;

	//if (regularExpressionLiteral ())
	//	return true;

	return false;
}

bool
Parser::nullLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_null (istream))
	{
		_value .reset (new ObjectValue ());
		return true;
	}

	return false;
}

bool
Parser::booleanLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::_true (istream))
	{
		_value .reset (new Boolean (true));
		return true;
	}

	if (Grammar::_false (istream))
	{
		_value .reset (new Boolean (false));
		return true;
	}

	return false;
}

bool
Parser::numericLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	if (binaryIntegerLiteral (_value))
		return true;

	if (octalIntegerLiteral (_value))
		return true;

	if (hexIntegerLiteral (_value))
		return true;

	if (decimalLiteral (_value))
		return true;

	return false;
}

bool
Parser::decimalLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	double _number;

	if (istream >> std::dec >> _number)
	{
		_value .reset (new Number (_number));
		return true;
	}

	istream .clear ();

	return false;
}

bool
Parser::binaryIntegerLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::bin (istream) or Grammar::BIN (istream))
	{
		std::string _digits;
	
		if (Grammar::BinaryDigits (istream, _digits))
		{
			_value .reset (new Number (math::strtoul (_digits .c_str (), 2)));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::octalIntegerLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::oct (istream) or Grammar::OCT (istream))
	{
		uint32_t _number;

		if (istream >> std::oct >> _number)
		{
			_value .reset (new Number (_number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::hexIntegerLiteral (ValuePtr & _value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t _number;

		if (istream >> std::hex >> _number)
		{
			_value .reset (new Number (_number));
			return true;
		}

		istream .clear ();
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression ()
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

	ValuePtr _value;

	if (literal (_value))
	{
		if (_value)
			__LOG__ << *_value << std::endl;
		else
			__LOG__ << "undefined" << std::endl;

		return true;
	}

	// ...

	if (Grammar::OpenParenthesis (istream))
	{
		if (expression ())
		{
			comments ();

			if (Grammar::CloseParenthesis (istream))
				return true;

			throw X3D::Error <X3D::INVALID_X3D> ("Expected ')' after expression.");
		}

		throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '('.");
	}

	return false;
}

bool
Parser::memberExpression ()
{
	//__LOG__ << std::endl;

	if (primaryExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::newExpression ()
{
	//__LOG__ << std::endl;

	if (memberExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::leftHandSideExpression ()
{
	//__LOG__ << std::endl;

	if (newExpression ())
		return true;

	//if (callExpression ())
	//	return true;

	return false;
}

bool
Parser::postfixExpression ()
{
	//__LOG__ << std::endl;

	if (leftHandSideExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::unaryExpression ()
{
	//__LOG__ << std::endl;

	if (postfixExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::multiplicativeExpression ()
{
	//__LOG__ << std::endl;

	if (unaryExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::Multiplication (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '*'.");
			}

			if (Grammar::Division (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '/'.");
			}

			if (Grammar::Remainder (istream))
			{
				if (unaryExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '/'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::additiveExpression ()
{
	//__LOG__ << std::endl;

	if (multiplicativeExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::Addition (istream))
			{
				if (multiplicativeExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '/+'.");
			}

			if (Grammar::Subtraction (istream))
			{
				if (multiplicativeExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '-'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::shiftExpression ()
{
	//__LOG__ << std::endl;

	if (additiveExpression ())
	{
		for (;;)
		{
			comments ();
		
			if (Grammar::LeftShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '<<'.");
			}

			if (Grammar::UnsignedRightShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '>>>'.");
			}

			if (Grammar::RightShift (istream))
			{
				if (additiveExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '>>'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::relationalExpression ()
{
	//__LOG__ << std::endl;

	if (shiftExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LessEqual (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '<='.");
			}

			if (Grammar::GreaterEqual (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '>='.");
			}

			if (Grammar::Less (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '<'.");
			}

			if (Grammar::Greater (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '>'.");
			}

			if (Grammar::instanceof (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after 'instanceof'.");
			}

			if (Grammar::in (istream))
			{
				if (shiftExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after 'in'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::equalityExpression ()
{
	//__LOG__ << std::endl;

	if (relationalExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::StrictEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '==='.");
			}

			if (Grammar::StrictNotEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '!=='.");
			}

			if (Grammar::Equal (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '=='.");
			}

			if (Grammar::NotEqual (istream))
			{
				if (relationalExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '!='.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseANDExpression ()
{
	//__LOG__ << std::endl;

	if (equalityExpression ())
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::LogicalAND .lookahead (istream))
				return true;

			if (Grammar::BitwiseAND (istream))
			{
				if (equalityExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseXORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression ())
	{
		for (;;)
		{
			comments ();
			
			if (Grammar::BitwiseXOR (istream))
			{
				if (bitwiseANDExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '^'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR .lookahead (istream))
				return true;
		
			if (Grammar::BitwiseOR (istream))
			{
				if (bitwiseXORExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '|'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalANDExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalAND (istream))
			{
				if (bitwiseORExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '&&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalORExpression ()
{
	//__LOG__ << std::endl;

	if (logicalANDExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR (istream))
			{
				if (logicalANDExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '||'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::conditionalExpression ()
{
	//__LOG__ << std::endl;

	if (logicalORExpression ())
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			if (assignmentExpression ())
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					if (assignmentExpression ())
					{
						return true;
					}
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after ':'.");
			}

			throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '?'.");
		}
	
		return true;
	}

	return false;
}

bool
Parser::assignmentExpression ()
{
	//__LOG__ << std::endl;

	const auto state    = istream .rdstate ();
	const auto position = istream .tellg ();

	if (leftHandSideExpression ())
	{
		comments ();

		if (Grammar::Assignment (istream))
		{
			if (assignmentExpression ())
				return true;
			
			throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '='.");
		}

		OperatorType _assignmentOperator;

		if (assignmentOperator (_assignmentOperator))
		{
			if (assignmentExpression ())
				return true;
	
			throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after '='."); // XXX
		}

		istream .clear (state);
		istream .seekg (position - istream .tellg (), std::ios_base::cur);
	}

	if (conditionalExpression ())
		return true;

	return false;
}

bool
Parser::assignmentOperator (OperatorType & _assignmentOperator)
{
	comments ();

	if (Grammar::MultiplicationAssigment (istream))
	{
		_assignmentOperator = OperatorType::MULTIPLICATION_ASSIGMENT;
		return true;
	}

	if (Grammar::DivisionAssignment (istream))
	{
		_assignmentOperator = OperatorType::DIVISION_ASSIGNMENT;
		return true;
	}

	if (Grammar::RemainderAssignment (istream))
	{
		_assignmentOperator = OperatorType::REMAINDER_ASSIGNMENT;
		return true;
	}

	if (Grammar::AdditionAssignment (istream))
	{
		_assignmentOperator = OperatorType::ADDITION_ASSIGNMENT;
		return true;
	}

	if (Grammar::SubtractionAssignment (istream))
	{
		_assignmentOperator = OperatorType::SUBTRACTION_ASSIGNMENT;
		return true;
	}

	if (Grammar::LeftShiftAssignment (istream))
	{
		_assignmentOperator = OperatorType::LEFT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::RightShiftAssignment (istream))
	{
		_assignmentOperator = OperatorType::RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::UnsignedRightShiftAssignment (istream))
	{
		_assignmentOperator = OperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseANDAssignment (istream))
	{
		_assignmentOperator = OperatorType::BITWISE_AND_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseXORAssignment (istream))
	{
		_assignmentOperator = OperatorType::BITWISE_XOR_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseORAssignment (istream))
	{
		_assignmentOperator = OperatorType::BITWISE_OR_ASSIGNMENT;
		return true;
	}

	return false;
}

bool
Parser::expression ()
{
	//__LOG__ << std::endl;

	if (assignmentExpression ())
	{
		for (;;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression ())
				{
					continue;
				}

				throw X3D::Error <X3D::INVALID_X3D> ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

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

	if (expression ())
	{
		if (Grammar::Semicolon (istream))
			return true;

		throw X3D::Error <X3D::INVALID_X3D> ("Expected ';' after expression.");
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
