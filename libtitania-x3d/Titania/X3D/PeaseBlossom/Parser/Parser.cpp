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

#include "../Execution/pbExecutionContext.h"
#include "../Expressions.h"
#include "../Objects.h"
#include "../Parser/Grammar.h"
#include "../Primitives.h"

#include <Titania/Backtrace.h>
#include <Titania/Math/Utility/strtol.h>

namespace titania {
namespace pb {

Parser::Parser (pbExecutionContext* const executionContext, std::istream & istream) :
	             rootContext (executionContext),
	       executionContexts ({ executionContext }),
	                  blocks ({ executionContext }),
	                 istream (istream),
	             whiteSpaces (),
	       commentCharacters (),
	isLeftHandSideExressions ()
{ }

void
Parser::parseIntoContext ()
throw (SyntaxError,
       ReferenceError)
{
	//__LOG__ << std::endl;

	try
	{
		istream .imbue (std::locale::classic ());

		program ();
	}
	catch (const pbException & error)
	{
		__LOG__ << ">>>" << istream .rdbuf () << "<<<" << std::endl;
		throw;
	}
}

void
Parser::pushExecutionContext (pbExecutionContext* const executionContext)
{
	pushBlock (executionContext);
	executionContexts .emplace (executionContext);
}

void
Parser::popExecutionContext ()
{
	popBlock ();
	executionContexts .pop ();
}

void
Parser::setState (const State & value)
{
	istream .clear (std::get <0> (value));
	istream .seekg (std::get <1> (value) - istream .tellg (), std::ios_base::cur);
}

Parser::State
Parser::getState ()
{
	return State (istream .rdstate (), istream .tellg ());
}

// A.1 Lexical Grammar

void
Parser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	while (comment ())
		;
}

bool
Parser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaces);

	if (Grammar::MultiLineComment (istream, commentCharacters))
		return true;

	if (Grammar::SingleLineComment (istream, commentCharacters))
		return true;

	return false;
}

void
Parser::commentsNoLineTerminator ()
{
	//__LOG__ << this << " " << std::endl;

	while (commentNoLineTerminator ())
		;
}

bool
Parser::commentNoLineTerminator ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpacesNoLineTerminator (istream, whiteSpaces);

	if (Grammar::MultiLineComment (istream, commentCharacters))
		return true;

	if (Grammar::SingleLineComment (istream, commentCharacters))
		return true;

	return false;
}

bool
Parser::identifier (std::string & identifierCharacters)
{
	//__LOG__ << std::endl;

	const auto state = getState ();

	if (identifierName (identifierCharacters))
	{
		if (not reservedWord (identifierCharacters))
			return true;

		setState (state);
	}

	return false;
}

bool
Parser::identifierName (std::string & identifierNameCharacters)
{
	//__LOG__ << std::endl;

	comments ();

	if (identifierStart (identifierNameCharacters))
	{
		while (identifierStart (identifierNameCharacters) or identifierPart (identifierNameCharacters))
			;

		return true;
	}

	return false;
}

bool
Parser::identifierStart (std::string & identifierStartCharacters)
{
	//__LOG__ << std::endl;

	// ...

	static const io::sequence IdentifierStart ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_");

	return IdentifierStart (istream, identifierStartCharacters);
}

bool
Parser::identifierPart (std::string & identifierPartCharacters)
{
	//__LOG__ << std::endl;

	static const io::sequence UnicodeDigit ("1234567890");

	// ...

	const auto size = identifierPartCharacters .size ();

	while (identifierStart (identifierPartCharacters) or UnicodeDigit (istream, identifierPartCharacters))
		;

	return identifierPartCharacters .size () not_eq size;
}

bool
Parser::reservedWord (const std::string & string)
{
	//__LOG__ << std::endl;

	if (Grammar::Keyword .count (string))
		return true;

	if (Grammar::FutureReservedWord .count (string))
		return true;

	static const std::set <std::string> primitives = {
		Grammar::null (),
		Grammar::true_ (),
		Grammar::false_ ()
	};

	if (primitives .count (string))
		return true;

	return false;
}

bool
Parser::literal (ptr <pbExpression> & value)
{
	if (nullLiteral (value))
		return true;

	if (booleanLiteral (value))
		return true;

	if (numericLiteral (value))
		return true;

	if (stringLiteral (value))
		return true;

	//if (regularExpressionLiteral ())
	//	return true;

	return false;
}

bool
Parser::nullLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::null (istream))
	{
		value = new PrimitiveExpression (nullptr);
		return true;
	}

	return false;
}

bool
Parser::booleanLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::true_ (istream))
	{
		value = new PrimitiveExpression (true);
		return true;
	}

	if (Grammar::false_ (istream))
	{
		value = new PrimitiveExpression (false);
		return true;
	}

	return false;
}

bool
Parser::numericLiteral (ptr <pbExpression> & value)
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
Parser::decimalLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	const auto state = getState ();

	comments ();

	double number = 0;

	if (istream >> std::dec >> number)
	{
		value = new PrimitiveExpression (number);
		return true;
	}

	setState (state);

	return false;
}

bool
Parser::binaryIntegerLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::bin (istream) or Grammar::BIN (istream))
	{
		std::string digits;

		if (Grammar::BinaryDigits (istream, digits))
		{
			value = new PrimitiveExpression ((double) math::strtoul (digits .c_str (), 2));
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::octalIntegerLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::oct (istream) or Grammar::OCT (istream))
	{
		uint32_t number = 0;

		if (istream >> std::oct >> number)
		{
			value = new PrimitiveExpression (number);
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::hexIntegerLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t number = 0;

		if (istream >> std::hex >> number)
		{
			value = new PrimitiveExpression (number);
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::stringLiteral (ptr <pbExpression> & value)
{
	static const io::quoted_string doubleQuotedString ('"');
	static const io::quoted_string singleQuotedString ('\'');

	comments ();

	std::string characters;

	if (doubleQuotedString (istream, characters))
	{
		value = new PrimitiveExpression (std::move (characters));
		return true;
	}

	if (singleQuotedString (istream, characters))
	{
		value = new PrimitiveExpression (std::move (characters));
		return true;
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::this_ (istream))
	{
		value = new VariableExpression (getExecutionContext (), std::string (Grammar::this_ ()));
		//value = new ThisExpression (getExecutionContext ());
		return true;
	}

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		value = new VariableExpression (getExecutionContext (), std::move (identifierCharacters));
		return true;
	}

	if (literal (value))
		return true;

	//if (arrayLiteral (value))
	//	return true;

	if (objectLiteral (value))
		return true;

	if (Grammar::OpenParenthesis (istream))
	{
		if (expression (value))
		{
			comments ();

			if (Grammar::CloseParenthesis (istream))
				return true;

			throw SyntaxError ("Expected ')' after expression.");
		}

		throw SyntaxError ("Expected expression after '('.");
	}

	return false;
}

bool
Parser::objectLiteral (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::OpenBrace (istream))
	{
		comments ();

		if (Grammar::CloseBrace (istream))
		{
			value = new ObjectLiteral (getExecutionContext ());
			return true;
		}

		auto objectLiteral = make_ptr <ObjectLiteral> (getExecutionContext ());

		if (propertyDefinitionList (objectLiteral))
		{
			comments ();

			if (Grammar::CloseBrace (istream))
			{
				value = std::move (objectLiteral);
				return true;
			}

			throw SyntaxError ("Expected a '}' after property definitions.");
		}
	}

	return false;
}

bool
Parser::propertyDefinitionList (const ptr <ObjectLiteral> & objectLiteral)
{
	//__LOG__ << std::endl;

	if (propertyDefinition (objectLiteral))
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (propertyDefinition (objectLiteral))
					continue;
			}

			return true;
		}
	}

	return false;
}

bool
Parser::propertyDefinition (const ptr <ObjectLiteral> & objectLiteral)
{
	//__LOG__ << std::endl;

	ptr <pbExpression> propertyNameValue;
	std::string        propertyNameCharacters;

	if (propertyName (propertyNameValue))
	{
		propertyNameCharacters = propertyNameValue -> getValue () .toString ();

		comments ();

		if (Grammar::Colon (istream))
		{
			ptr <pbExpression> value;

			if (assignmentExpression (value))
			{
				objectLiteral -> updatePropertyDescriptor (std::move (propertyNameCharacters), std::move (value));
				return true;
			}

			throw SyntaxError ("Expected expression after property name.");
		}

		if (propertyNameCharacters not_eq Grammar::get () and propertyNameCharacters not_eq Grammar::set ())
			throw SyntaxError ("Expected a ':' after property name.");
	}

	comments ();

	if (propertyNameCharacters == Grammar::get () or Grammar::get (istream))
	{
		if (propertyName (propertyNameValue))
		{
			comments ();

			if (Grammar::OpenParenthesis (istream))
			{
				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					comments ();

					if (Grammar::OpenBrace (istream))
					{
						auto function = make_ptr <Function> (getExecutionContext ());

						pushExecutionContext (function .get ());

						functionBody ();

						popExecutionContext ();

						comments ();

						if (Grammar::CloseBrace (istream))
						{
							objectLiteral -> updatePropertyDescriptor (propertyNameValue -> getValue () .toString (), nullptr, std::move (function));
							return true;
						}

						throw SyntaxError ("Expected a '}' after function body.");
					}

					throw SyntaxError ("Expected a '{' before function body.");
				}

				throw SyntaxError ("Expected a ')'.");
			}

			throw SyntaxError ("Expected a '(' after property name.");
		}

		throw SyntaxError ("Expected property name after 'get'.");
	}

	if (propertyNameCharacters == Grammar::set () or Grammar::set (istream))
	{
		if (propertyName (propertyNameValue))
		{
			comments ();

			if (Grammar::OpenParenthesis (istream))
			{
				std::vector <std::string> formalParameters;

				if (propertySetParameterList (formalParameters))
				{
					comments ();

					if (Grammar::CloseParenthesis (istream))
					{
						comments ();

						if (Grammar::OpenBrace (istream))
						{
							auto function = make_ptr <Function> (getExecutionContext (), "", std::move (formalParameters));

							pushExecutionContext (function .get ());

							functionBody ();

							popExecutionContext ();

							comments ();

							if (Grammar::CloseBrace (istream))
							{
								objectLiteral -> updatePropertyDescriptor (propertyNameValue -> getValue () .toString (), nullptr, nullptr, std::move (function));
								return true;
							}

							throw SyntaxError ("Expected a '}' after function body.");
						}

						throw SyntaxError ("Expected a '{' before function body.");
					}

					throw SyntaxError ("Expected a ')'.");
				}

				throw SyntaxError ("Expected a property set parameter list.");
			}

			throw SyntaxError ("Expected a '(' after property name.");
		}

		throw SyntaxError ("Expected property name after 'set'.");
	}

	return false;
}

bool
Parser::propertyName (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	std::string propertyNameCharacters;

	if (identifierName (propertyNameCharacters))
	{
		value = new PrimitiveExpression (std::move (propertyNameCharacters));
		return true;
	}

	if (stringLiteral (value))
		return true;

	if (numericLiteral (value))
		return true;

	return false;
}

bool
Parser::propertySetParameterList (std::vector <std::string> & formalParameters)
{
	//__LOG__ << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		formalParameters .emplace_back (std::move (identifierCharacters));
		return true;
	}

	return false;
}

bool
Parser::memberExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (primaryExpression (value) or functionExpression (value) or newExpression (value))
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::OpenBracket (istream))
			{
				ptr <pbExpression> arrayIndexExpressions;

				if (expression (arrayIndexExpressions))
				{
					comments ();

					if (Grammar::CloseBracket (istream))
					{
						//value = new ArrayIndexExpression (std::move (value), std::move (arrayIndexExpressions));
						continue;
					}

					throw SyntaxError ("Expected a ']' after expression.");
				}

				throw SyntaxError ("Expected expression after '['.");
			}

			if (Grammar::Period (istream))
			{
				std::string identifierNameCharacters;

				if (identifierName (identifierNameCharacters))
				{
					value = new PropertyExpression (std::move (value), std::move (identifierNameCharacters));
					continue;
				}

				throw SyntaxError ("Expected a identifier name after '.'.");
			}

			break;
		}

		return true;
	}

	return false;
}

bool
Parser::newExpression (ptr <pbExpression> & value)
{
	if (Grammar::new_ (istream))
	{
		if (memberExpression (value))
		{
			array <ptr <pbExpression>> argumentListExpressions;

			arguments (argumentListExpressions);

			value = new NewExpression (std::move (value), std::move (argumentListExpressions));
			return true;
		}

		throw SyntaxError ("Expected a expression after new.");
	}

	return false;
}

//bool
//Parser::newExpression (ptr <pbExpression> & value)
//{
//	//__LOG__ << std::endl;
//
//	if (memberExpression (value))
//		return true;
//
//	// Never reached.
//
//	if (Grammar::new_ (istream))
//	{
//		if (newExpression (value))
//		{
//			value = new NewExpression (std::move (value), std::vector <ptr <pbExpression>> ());
//			return true;
//		}
//	}
//
//	return false;
//}

bool
Parser::callExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (value or memberExpression (value))
	{
		for ( ; ;)
		{
			array <ptr <pbExpression>> argumentListExpressions;

			if (arguments (argumentListExpressions))
			{
				value = new FunctionCallExpression (getExecutionContext (), std::move (value), std::move (argumentListExpressions));
				continue;
			}

			if (Grammar::OpenBracket (istream))
			{
				ptr <pbExpression> arrayIndexExpressions;

				if (expression (arrayIndexExpressions))
				{
					comments ();

					if (Grammar::CloseBracket (istream))
					{
						//value = new ArrayIndexExpression (std::move (value), std::move (arrayIndexExpressions));
						continue;
					}

					throw SyntaxError ("Expected a ']' after expression.");
				}

				throw SyntaxError ("Expected expression after '['.");
			}

			if (Grammar::Period (istream))
			{
				std::string identifierNameCharacters;

				if (identifierName (identifierNameCharacters))
				{
					value = new PropertyExpression (std::move (value), std::move (identifierNameCharacters));
					continue;
				}

				throw SyntaxError ("Expected a identifier name after '.'.");
			}

			break;
		}

		return true;
	}

	return false;
}

bool
Parser::arguments (array <ptr <pbExpression>> & argumentListExpressions)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::OpenParenthesis (istream))
	{
		comments ();

		if (Grammar::CloseParenthesis (istream))
			return true;

		argumentList (argumentListExpressions);

		comments ();

		if (Grammar::CloseParenthesis (istream))
			return true;

		throw SyntaxError ("Expected a ')' after argument list.");
	}

	return false;
}

bool
Parser::argumentList (array <ptr <pbExpression>> & argumentListExpressions)
{
	//__LOG__ << std::endl;

	ptr <pbExpression> value;

	if (assignmentExpression (value))
	{
		argumentListExpressions .emplace_back (std::move (value));

		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression (value))
				{
					argumentListExpressions .emplace_back (std::move (value));
					continue;
				}

				throw SyntaxError ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::leftHandSideExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (memberExpression (value))
	{
		comments ();

		if (not Grammar::OpenParenthesis .lookahead (istream))
		{
			isLeftHandSideExressions .back () = true;
			return true;
		}
	}

	if (callExpression (value))
	{
		isLeftHandSideExressions .back () = true;
		return true;
	}

	return false;
}

bool
Parser::postfixExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (leftHandSideExpression (value))
		return true;

	// ...
	//		isLeftHandSideExressions .back () = false;

	return false;
}

bool
Parser::unaryExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (postfixExpression (value))
		return true;

	if (Grammar::delete_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new DeleteExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after delete.");
	}

	if (Grammar::void_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new VoidExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after void.");
	}

	if (Grammar::typeof_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new TypeOfExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after typeof.");
	}

	if (Grammar::Increment (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new PostIncrementExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after ++.");
	}

	if (Grammar::Decrement (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new PostDecrementExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after --.");
	}

	if (Grammar::PlusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new ToNumberExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after +.");
	}

	if (Grammar::MinusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new NegateExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after -.");
	}

	if (Grammar::Tilde (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new BitwiseNOTExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after ~.");
	}

	if (Grammar::ExclamationMark (istream))
	{
		isLeftHandSideExressions .back () = false;

		ptr <pbExpression> expression;

		if (unaryExpression (expression))
		{
			//value = new LogicalNOTExpression (std::move (expression));
			return true;
		}

		throw SyntaxError ("Expected expression after !.");
	}

	return false;
}

bool
Parser::multiplicativeExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (unaryExpression (lhs))
	{
		comments ();

		if (Grammar::Multiplication (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Multiplication .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (multiplicativeExpression (rhs))
			{
				lhs = createMultiplicationExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '*'.");
		}

		if (Grammar::Division (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Division .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (multiplicativeExpression (rhs))
			{
				lhs = createDivisionExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '/'.");
		}

		if (Grammar::Remainder (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Remainder .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (multiplicativeExpression (rhs))
			{
				lhs = createRemainderExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '%'.");
		}

		return true;
	}

	return false;
}

bool
Parser::additiveExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (multiplicativeExpression (lhs))
	{
		comments ();

		if (Grammar::Addition (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Addition .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (additiveExpression (rhs))
			{
				lhs = createAdditionExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '+'.");
		}

		if (Grammar::Subtraction (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::Subtraction .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (additiveExpression (rhs))
			{
				lhs = createSubtractionExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '-'.");
		}

		return true;
	}

	return false;
}

bool
Parser::shiftExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (additiveExpression (lhs))
	{
		comments ();

		if (Grammar::LeftShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::LeftShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (shiftExpression (rhs))
			{
				lhs = createLeftShiftExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<<'.");
		}

		if (Grammar::UnsignedRightShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::UnsignedRightShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (shiftExpression (rhs))
			{
				//lhs = make_unsigned_right_shift (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>>>'.");
		}

		if (Grammar::RightShift (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::RightShift .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (shiftExpression (rhs))
			{
				//lhs = make_right_shift (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>>'.");
		}

		return true;
	}

	return false;
}

bool
Parser::relationalExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (shiftExpression (lhs))
	{
		comments ();

		if (Grammar::LessEqual (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::LessEqual .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				//lhs = make_less_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<='.");
		}

		if (Grammar::GreaterEqual (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::GreaterEqual .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				//lhs = make_greater_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>='.");
		}

		if (Grammar::Less (istream))
		{
			if (Grammar::LessEqual .lookahead (istream))
				return Grammar::Less .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				lhs = createLessExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '<'.");
		}

		if (Grammar::Greater (istream))
		{
			if (Grammar::RightShift .lookahead (istream))
				return Grammar::Greater .rewind (istream);

			if (Grammar::GreaterEqual .lookahead (istream))
				return Grammar::Greater .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				//lhs = make_greater (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '>'.");
		}

		if (Grammar::instanceof (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::instanceof .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				//lhs = make_instanceof (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after 'instanceof'.");
		}

		if (Grammar::in (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::in .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (relationalExpression (rhs))
			{
				//lhs = make_in (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after 'in'.");
		}

		return true;
	}

	return false;
}

bool
Parser::equalityExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (relationalExpression (lhs))
	{
		comments ();

		if (Grammar::StrictEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (equalityExpression (rhs))
			{
				lhs = createStrictEqualExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '==='.");
		}

		if (Grammar::StrictNotEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (equalityExpression (rhs))
			{
				//lhs = make_strict_not_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '!=='.");
		}

		if (Grammar::Equal (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (equalityExpression (rhs))
			{
				lhs = createEqualExpression (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '=='.");
		}

		if (Grammar::NotEqual (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (equalityExpression (rhs))
			{
				//lhs = make_not_equal (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '!='.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseANDExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (equalityExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalAND .lookahead (istream))
			return true;

		if (Grammar::BitwiseAND (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseAND .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (bitwiseANDExpression (rhs))
			{
				//lhs = make_bitwise_and (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '&'.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseXORExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression (lhs))
	{
		comments ();

		if (Grammar::BitwiseXOR (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseXOR .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (bitwiseXORExpression (rhs))
			{
				//lhs = make_bitwise_xor (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '^'.");
		}

		return true;
	}

	return false;
}

bool
Parser::bitwiseORExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalOR .lookahead (istream))
			return true;

		if (Grammar::BitwiseOR (istream))
		{
			if (Grammar::Assignment .lookahead (istream))
				return Grammar::BitwiseOR .rewind (istream);

			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (bitwiseORExpression (rhs))
			{
				//lhs = make_bitwise_or (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '|'.");
		}

		return true;
	}

	return false;
}

bool
Parser::logicalANDExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalAND (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (logicalANDExpression (rhs))
			{
				//lhs = make_logical_and (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '&&'.");
		}

		return true;
	}

	return false;
}

bool
Parser::logicalORExpression (ptr <pbExpression> & lhs)
{
	//__LOG__ << std::endl;

	if (logicalANDExpression (lhs))
	{
		comments ();

		if (Grammar::LogicalOR (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> rhs;

			if (logicalORExpression (rhs))
			{
				//lhs = make_logical_or (std::move (lhs), std::move (rhs));
				return true;
			}

			throw SyntaxError ("Expected expression after '||'.");
		}

		return true;
	}

	return false;
}

bool
Parser::conditionalExpression (ptr <pbExpression> & first)
{
	//__LOG__ << std::endl;

	if (logicalORExpression (first))
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbExpression> second;

			if (assignmentExpression (second))
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					ptr <pbExpression> third;

					if (assignmentExpression (third))
					{
						//first = make_conditional (std::move (first), std::move (second), std::move (third));
						return true;
					}
				}

				throw SyntaxError ("Expected expression after ':'.");
			}

			throw SyntaxError ("Expected expression after '?'.");
		}

		return true;
	}

	return false;
}

bool
Parser::assignmentExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	isLeftHandSideExressions .push_back (false);

	if (conditionalExpression (value))
	{
		const bool isLeftHandSideExression = isLeftHandSideExressions .back ();

		isLeftHandSideExressions .pop_back ();

		if (isLeftHandSideExression)
		{
			comments ();

			if (Grammar::Assignment (istream))
			{
				ptr <pbExpression> expression;

				if (assignmentExpression (expression))
				{
					value = new AssignmentExpression (getExecutionContext (), std::move (value), std::move (expression), AssignmentOperatorType::ASSIGNMENT);
					return true;
				}

				throw SyntaxError ("Expected expression after '='.");
			}

			AssignmentOperatorType type;

			if (assignmentOperator (type))
			{
				ptr <pbExpression> expression;

				if (assignmentExpression (expression))
				{
					value = new AssignmentExpression (getExecutionContext (), std::move (value), std::move (expression), type);
					return true;
				}

				throw SyntaxError ("Expected expression after '" + to_ustring (type) + "'.");
			}
		}

		return true;
	}

	isLeftHandSideExressions .pop_back ();

	return false;
}

bool
Parser::assignmentOperator (AssignmentOperatorType & type)
{
	comments ();

	if (Grammar::MultiplicationAssigment (istream))
	{
		type = AssignmentOperatorType::MULTIPLICATION_ASSIGNMENT;
		return true;
	}

	if (Grammar::DivisionAssignment (istream))
	{
		type = AssignmentOperatorType::DIVISION_ASSIGNMENT;
		return true;
	}

	if (Grammar::RemainderAssignment (istream))
	{
		type = AssignmentOperatorType::REMAINDER_ASSIGNMENT;
		return true;
	}

	if (Grammar::AdditionAssignment (istream))
	{
		type = AssignmentOperatorType::ADDITION_ASSIGNMENT;
		return true;
	}

	if (Grammar::SubtractionAssignment (istream))
	{
		type = AssignmentOperatorType::SUBTRACTION_ASSIGNMENT;
		return true;
	}

	if (Grammar::LeftShiftAssignment (istream))
	{
		type = AssignmentOperatorType::LEFT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::RightShiftAssignment (istream))
	{
		type = AssignmentOperatorType::RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::UnsignedRightShiftAssignment (istream))
	{
		type = AssignmentOperatorType::UNSIGNED_RIGHT_SHIFT_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseANDAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_AND_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseXORAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_XOR_ASSIGNMENT;
		return true;
	}

	if (Grammar::BitwiseORAssignment (istream))
	{
		type = AssignmentOperatorType::BITWISE_OR_ASSIGNMENT;
		return true;
	}

	return false;
}

bool
Parser::expression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	if (assignmentExpression (value))
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression (value))
					continue;

				throw SyntaxError ("Expected expression after ','.");
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
	//__LOG__ << istream .tellg () << std::endl;

	if (block ())
		return true;

	if (variableStatement ())
		return true;

	if (emptyStatement ())
		return true;

	if (expressionStatement ())
		return true;

	if (ifStatement ())
		return true;

	if (iterationStatement ())
		return true;

	//if (continueStatement ())
	//	return true;

	//if (breakStatement ())
	//	return true;

	if (returnStatement ())
		return true;

	//if (withStatement ())
	//	return true;

	//if (labelledStatement ())
	//	return true;

	//if (switchStatement ())
	//	return true;

	//if (throwStatement ())
	//	return true;

	//if (tryStatement ())
	//	return true;

	//if (sDebuggerStatement ())
	//	return true;

	return false;
}

bool
Parser::block ()
{
	comments ();

	if (Grammar::OpenBrace (istream))
	{
		statementList ();

		comments ();

		if (Grammar::CloseBrace (istream))
			return true;

		throw SyntaxError ("Expected a '}' at end of block statement.");
	}

	return false;
}

bool
Parser::statementList ()
{
	//__LOG__ << std::endl;

	if (statement ())
	{
		for ( ; ;)
		{
			if (statement ())
				continue;

			return true;
		}
	}

	return false;
}

bool
Parser::variableStatement ()
{
	comments ();

	if (Grammar::var (istream))
	{
		if (variableDeclarationList ())
		{
			comments ();

			if (Grammar::Semicolon (istream))
				return true;

			throw SyntaxError ("Expected a ';' after variable declaration.");
		}

		throw SyntaxError ("Expected variable name after 'var'.");
	}

	return false;
}

bool
Parser::variableDeclarationList ()
{
	//__LOG__ << std::endl;

	if (variableDeclaration ())
	{
		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (variableDeclaration ())
					continue;

				throw SyntaxError ("Expected variable name after ','.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::variableDeclaration ()
{
	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		ptr <pbExpression> value;

		initialiser (value);

		getBlock () -> addExpression (new VariableDeclaration (getExecutionContext (), std::move (identifierCharacters), std::move (value)));

		return true;
	}

	return false;
}

bool
Parser::initialiser (ptr <pbExpression> & value)
{
	comments ();

	if (Grammar::Assignment (istream))
	{
		if (assignmentExpression (value))
			return true;
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

bool
Parser::expressionStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::OpenBrace .lookahead (istream))
		return false;

	if (Grammar::function .lookahead (istream))
		return false;

	ptr <pbExpression> value;

	if (expression (value))
	{
		comments ();

		if (Grammar::Semicolon (istream))
		{
			getBlock () -> addExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected ';' after expression.");
	}

	return false;
}

bool
Parser::ifStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::if_ (istream))
	{
		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			ptr <pbExpression> booleanExpression;

			if (expression (booleanExpression))
			{
				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					auto value = new IfStatement (std::move (booleanExpression));

					pushBlock (value -> getThenBlock () .get ());

					statement ();

					popBlock ();

					comments ();

					if (Grammar::else_ (istream))
					{
						pushBlock (value -> getElseBlock () .get ());

						statement ();

						popBlock ();
					}

					getBlock () -> addExpression (std::move (value));

					return true;
				}

				throw SyntaxError ("Expected a ')'.");
			}

			throw SyntaxError ("Expected boolean expression after '('.");
		}

		throw SyntaxError ("Expected a '(' after 'if'.");
	}

	return false;
}

bool
Parser::iterationStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	// ...

	if (Grammar::for_ (istream))
	{
		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			comments ();

			if (Grammar::var (istream))
			{
				if (variableDeclarationList ())
				{
					comments ();

					if (Grammar::Semicolon (istream))
					{
						ptr <pbExpression> booleanExpression;

						expression (booleanExpression);

						comments ();

						if (Grammar::Semicolon (istream))
						{
							ptr <pbExpression> iterationExpression;

							expression (iterationExpression);

							comments ();

							if (Grammar::CloseParenthesis (istream))
							{
								auto value = new ForStatement (std::move (booleanExpression), std::move (iterationExpression));

								pushBlock (value -> getBlock () .get ());

								statement ();

								popBlock ();

								getBlock () -> addExpression (std::move (value));

								return true;
							}

							throw SyntaxError ("Expected a ')'.");
						}

						throw SyntaxError ("Expected a ';'.");
					}

					throw SyntaxError ("Expected a ';'.");
				}
			}
		}

		throw SyntaxError ("Expected a '(' after 'for'.");
	}

	// ...

	return false;
}

bool
Parser::returnStatement ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::return_ (istream))
	{
		commentsNoLineTerminator ();

		ptr <pbExpression> value;

		if (expression (value))
			comments ();

		if (Grammar::Semicolon (istream))
		{
			getBlock () -> addExpression (new ReturnStatement (getExecutionContext (), std::move (value)));
			return true;
		}

		throw SyntaxError ("Expected a ';' after expression.");
	}

	return false;
}

// A.5 Functions and Programs

bool
Parser::functionDeclaration ()
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		if (identifier (name))
		{
			comments ();

			if (Grammar::OpenParenthesis (istream))
			{
				std::vector <std::string> formalParameters;

				formalParameterList (formalParameters);

				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					comments ();

					if (Grammar::OpenBrace (istream))
					{
						auto function = make_ptr <Function> (getExecutionContext (), name, std::move (formalParameters));

						pushExecutionContext (function .get ());

						functionBody ();

						popExecutionContext ();

						comments ();

						if (Grammar::CloseBrace (istream))
						{
							getExecutionContext () -> updateFunctionDeclaration (std::move (function));
							return true;
						}

						throw SyntaxError ("Expected a '}' after function body.");
					}

					throw SyntaxError ("Expected a '{' after parameter list.");
				}

				throw SyntaxError ("Expected a ')' after formal parameters.");
			}

			throw SyntaxError ("Expected a '(' after function name.");
		}

		throw SyntaxError ("Function statement requires a name.");
	}

	return false;
}

bool
Parser::functionExpression (ptr <pbExpression> & value)
{
	//__LOG__ << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		identifier (name);

		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			std::vector <std::string> formalParameters;

			formalParameterList (formalParameters);

			comments ();

			if (Grammar::CloseParenthesis (istream))
			{
				comments ();

				if (Grammar::OpenBrace (istream))
				{
					auto function = make_ptr <Function> (getExecutionContext (), name, std::move (formalParameters));

					pushExecutionContext (function .get ());

					functionBody ();

					popExecutionContext ();

					comments ();

					if (Grammar::CloseBrace (istream))
					{
						value = new FunctionExpression (getExecutionContext (), std::move (function));
						return true;
					}

					throw SyntaxError ("Expected a '}' after function body.");
				}

				throw SyntaxError ("Expected a '{' after parameter list.");
			}

			throw SyntaxError ("Expected a ')' after formal parameters.");
		}

		throw SyntaxError ("Expected a '(' before formal parameters.");
	}

	return false;
}

bool
Parser::formalParameterList (std::vector <std::string> & formalParameters)
{
	//__LOG__ << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		formalParameters .emplace_back (std::move (identifierCharacters));

		for ( ; ;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				std::string identifierCharacters;

				if (identifier (identifierCharacters))
				{
					formalParameters .emplace_back (std::move (identifierCharacters));
					continue;
				}

				throw SyntaxError ("Expected expression after ','.");
			}

			return true;
		}
	}

	return false;
}

void
Parser::functionBody ()
{
	//__LOG__ << std::endl;

	sourceElements ();
}

void
Parser::program ()
{
	//__LOG__ << std::endl;

	sourceElements ();

	if (istream .peek () not_eq std::char_traits <char>::eof ())
		throw SyntaxError ("Unexpected statement.");
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
	//__LOG__ << istream .tellg () << std::endl;

	if (statement ())
		return true;

	if (functionDeclaration ())
		return true;

	return false;
}

} // pb
} // titania
