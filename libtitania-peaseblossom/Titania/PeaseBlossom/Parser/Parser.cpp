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

#include "Parser.h"

#include "../Execution/pbExecutionContext.h"
#include "../Objects.h"
#include "../Parser/Grammar.h"
#include "../Primitives.h"
#include "../Statements.h"

#include <Titania/Backtrace.h>
#include <Titania/Math/Utility/strtol.h>

namespace titania {
namespace pb {

Parser::Parser (pbExecutionContext* const executionContext, std::istream & istream) :
	             rootContext (executionContext),
	       executionContexts ({ executionContext }),
	                 istream (istream),
	                position (-1),
	              lineNumber (1),
	             whiteSpaces (),
	       commentCharacters (),
	                 newLine (false),
	isLeftHandSideExressions (),
	                    noIn (false),
	                forLevel (0),
	             switchLevel (0)
{ }

///  throws pbError
void
Parser::parseIntoContext ()
{
	//__LOG__ << (char) istream .peek () << std::endl;

	try
	{
		istream .imbue (std::locale::classic ());

		program ();
	}
	catch (pbError & error)
	{
		setError (error);
		throw;
	}
}

void
Parser::setError (pbError & error)
{
	__LOG__ << this << " " << std::endl;
	__LOG__ << this << " " << istream .peek () << std::endl;

	istream .clear ();

	std::string rest   = getline ();
	std::string line   = rgetline ();
	size_t      column = line .size () - rest .size ();

	//FilterControlCharacters (line);
	//FilterBadUTF8Characters (line);

	//	if (line .size () > 80)
	//	{
	//		line    = line .substr (linePos - 40, 80);
	//		preLine = std::string ();
	//		linePos = 40;
	//	}

	error .setFilename ("filename");
	error .setLineNumber (lineNumber);
	error .setColumn (column);
	error .setSourceLine (line);
}

std::string
Parser::getline ()
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for (;;)
	{
		char c = istream .get ();

		if (istream)
		{
			if (c == '\n' or c == '\r')
			{
				istream .unget ();
				break;
			}

			else
				string .push_back (c);
		}
		else
			break;
	}

	istream .clear ();

	return string;
}

std::string
Parser::rgetline ()
{
	//__LOG__ << this << " " << std::endl;

	std::string string;

	for (;;)
	{
		istream .unget ();

		char c = istream .peek ();

		if (istream)
		{
			if (c == '\n' or c == '\r')
				break;

			else
				string .push_back (c);
		}
		else
			return "";
	}

	istream .clear ();

	return std::string (string .rbegin (), string .rend ());
}

void
Parser::setState (const State & value)
{
	istream .clear (std::get <0> (value));

	for (size_t i = 0, size = istream .tellg () - std::get <1> (value) ; i < size; ++ i)
		istream .unget ();
}

Parser::State
Parser::getState ()
{
	return State (istream .rdstate (), istream .tellg ());
}

void
Parser::lines (const std::string & string)
{
	//__LOG__ << this << " " << std::endl;

	lineNumber += std::count (string .begin (), string .end (), '\n');
}

bool
Parser::haveAutomaticSemicolon () const
{
	if (newLine)
		return true;

	if (not istream or istream .peek () == -1)
		return true;

	if (istream .peek () == '}')
		return true;

	return false;
}

// A.1 Lexical Grammar

bool
Parser::comments ()
{
	//__LOG__ << this << " " << std::endl;

	const auto currentPosition = istream .tellg ();

	if (currentPosition == position)
		return false;

	const auto currentLineNumber = lineNumber;

	while (comment ())
		;

	position = istream .tellg ();

	const bool comments = position not_eq currentPosition;

	if (comments)
		newLine = lineNumber not_eq currentLineNumber;

	return comments;
}

bool
Parser::comment ()
{
	//__LOG__ << this << " " << std::endl;

	Grammar::WhiteSpaces (istream, whiteSpaces);

	lines (whiteSpaces);

	whiteSpaces .clear ();

	if (Grammar::MultiLineComment (istream, commentCharacters))
	{
		lines (commentCharacters);
		commentCharacters .clear ();
		return true;
	}

	if (Grammar::SingleLineComment (istream, commentCharacters))
	{
		commentCharacters .clear ();
		return true;
	}

	return false;
}

bool
Parser::identifier (std::string & identifierCharacters)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
	//__LOG__ << (char) istream .peek () << std::endl;

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
	//__LOG__ << (char) istream .peek () << std::endl;

	// ...

	static const io::sequence IdentifierStart ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$_");

	return IdentifierStart (istream, identifierStartCharacters);
}

bool
Parser::identifierPart (std::string & identifierPartCharacters)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
	//__LOG__ << (char) istream .peek () << std::endl;

	return Grammar::Keyword .count (string);
}

bool
Parser::literal (ptr <pbStatement> & value)
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
Parser::nullLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::null (istream))
	{
		value = new PrimitiveExpression (nullptr, StatementType::NULL_OBJECT);
		return true;
	}

	return false;
}

bool
Parser::booleanLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::true_ (istream))
	{
		value = new PrimitiveExpression (true, StatementType::BOOLEAN);
		return true;
	}

	if (Grammar::false_ (istream))
	{
		value = new PrimitiveExpression (false, StatementType::BOOLEAN);
		return true;
	}

	return false;
}

bool
Parser::numericLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
Parser::decimalLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	const auto state = getState ();

	comments ();

	double number = 0;

	if (istream >> std::dec >> number)
	{
		value = new PrimitiveExpression (number, StatementType::NUMBER);
		return true;
	}

	setState (state);

	return false;
}

bool
Parser::binaryIntegerLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::bin (istream) or Grammar::BIN (istream))
	{
		std::string digits;

		if (Grammar::BinaryDigits (istream, digits))
		{
			value = new PrimitiveExpression ((double) math::strtoul (digits .c_str (), 2), StatementType::BINARY_NUMBER);
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::octalIntegerLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::oct (istream) or Grammar::OCT (istream))
	{
		uint32_t number = 0;

		if (istream >> std::oct >> number)
		{
			value = new PrimitiveExpression (number, StatementType::OCTAL_NUMBER);
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::hexIntegerLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::hex (istream) or Grammar::HEX (istream))
	{
		uint32_t number = 0;

		if (istream >> std::hex >> number)
		{
			value = new PrimitiveExpression (number, StatementType::HEXAL_NUMBER);
			return true;
		}

		istream .clear ();
	}

	return false;
}

bool
Parser::stringLiteral (ptr <pbStatement> & value)
{
	static const io::quoted_string doubleQuotedString ('"');
	static const io::quoted_string singleQuotedString ('\'');

	comments ();

	std::string characters;

	if (doubleQuotedString (istream, characters))
	{
		lines (characters);

		value = new PrimitiveExpression (std::move (characters), StatementType::DOUBLE_QUOTED_STRING);
		return true;
	}

	if (singleQuotedString (istream, characters))
	{
		lines (characters);

		value = new PrimitiveExpression (std::move (characters), StatementType::SINGLE_QUOTED_STRING);
		return true;
	}

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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

	if (arrayLiteral (value))
		return true;

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
Parser::arrayLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::OpenBracket (istream))
	{
		auto arrayLiteral = make_ptr <ArrayLiteral> (getExecutionContext ());

		if (elision (arrayLiteral))
		{
			comments ();

			if (Grammar::CloseBracket (istream))
			{
				value = std::move (arrayLiteral);
				return true;
			}
		}

		comments ();

		if (Grammar::CloseBracket (istream))
		{
			value = std::move (arrayLiteral);
			return true;
		}

		if (elementList (arrayLiteral))
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				elision (arrayLiteral);

				comments ();
			}

			if (Grammar::CloseBracket (istream))
			{
				value = std::move (arrayLiteral);
				return true;
			}

			throw SyntaxError ("Expected a ']' after array element list.");
		}
	}

	return false;
}

bool
Parser::elementList (const ptr <ArrayLiteral> & arrayLiteral)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	elision (arrayLiteral);

	ptr <pbStatement> value;

	if (assignmentExpression (value))
	{
		arrayLiteral -> addExpression (std::move (value));

		for (;;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				elision (arrayLiteral);

				if (assignmentExpression (value))
				{
					arrayLiteral -> addExpression (std::move (value));
					continue;
				}
			}

			return true;
		}
	}

	return false;
}

bool
Parser::elision (const ptr <ArrayLiteral> & arrayLiteral)
{
	comments ();

	if (Grammar::Comma (istream))
	{
		arrayLiteral -> addExpression (new PrimitiveExpression (undefined, StatementType::UNDEFINED));

		comments ();

		while (Grammar::Comma (istream))
		{
			arrayLiteral -> addExpression (new PrimitiveExpression (undefined, StatementType::UNDEFINED));

			comments ();
		}

		return true;
	}

	return false;
}

bool
Parser::objectLiteral (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
	//__LOG__ << (char) istream .peek () << std::endl;

	if (propertyDefinition (objectLiteral))
	{
		for (;;)
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
	//__LOG__ << (char) istream .peek () << std::endl;

	ptr <pbStatement> propertyNameValue;
	std::string       propertyNameCharacters;

	if (propertyName (propertyNameValue))
	{
		propertyNameCharacters = propertyNameValue -> getValue () .toString ();

		comments ();

		if (Grammar::Colon (istream))
		{
			ptr <pbStatement> value;

			if (assignmentExpression (value))
			{
				objectLiteral -> defineOwnProperty (std::move (propertyNameCharacters), std::move (value), WRITABLE | CONFIGURABLE | ENUMERABLE);
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
							objectLiteral -> defineOwnProperty (propertyNameValue -> getValue () .toString (), nullptr, CONFIGURABLE | ENUMERABLE, std::move (function));
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
				std::vector <Identifier> formalParameters;

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
								objectLiteral -> defineOwnProperty (propertyNameValue -> getValue () .toString (), nullptr, CONFIGURABLE | ENUMERABLE, nullptr, std::move (function));
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
Parser::propertyName (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	std::string propertyNameCharacters;

	if (identifierName (propertyNameCharacters))
	{
		value = new PrimitiveExpression (std::move (propertyNameCharacters), StatementType::STRING);
		return true;
	}

	if (stringLiteral (value))
		return true;

	if (numericLiteral (value))
		return true;

	return false;
}

bool
Parser::propertySetParameterList (std::vector <Identifier> & formalParameters)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		formalParameters .emplace_back (std::move (identifierCharacters));
		return true;
	}

	return false;
}

bool
Parser::memberExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (primaryExpression (value) or functionExpression (value) or newExpression (value))
	{
		for (;;)
		{
			comments ();

			if (Grammar::OpenBracket (istream))
			{
				ptr <pbStatement> identifier;

				if (expression (identifier))
				{
					comments ();

					if (Grammar::CloseBracket (istream))
					{
						value = new ComputedMemberAccessExpression (getExecutionContext (), std::move (value), std::move (identifier));
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
					value = new MemberAccessExpression (getExecutionContext (), std::move (value), std::move (identifierNameCharacters));
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
Parser::newExpression (ptr <pbStatement> & value)
{
	if (Grammar::new_ (istream))
	{
		if (memberExpression (value))
		{
			array <ptr <pbStatement>>  argumentListExpressions;

			arguments (argumentListExpressions);

			value = new NewExpression (getExecutionContext (), std::move (value), std::move (argumentListExpressions));
			return true;
		}

		throw SyntaxError ("Expected a expression after new.");
	}

	return false;
}

//bool
//Parser::newExpression (ptr <pbStatement> & value)
//{
//	//__LOG__ << (char) istream .peek () << std::endl;
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
//			value = new NewExpression (std::move (value), std::vector <ptr <pbStatement>> ());
//			return true;
//		}
//	}
//
//	return false;
//}

bool
Parser::callExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (value or memberExpression (value))
	{
		for (;;)
		{
			array <ptr <pbStatement>>  argumentListExpressions;

			if (arguments (argumentListExpressions))
			{
				value = new FunctionCallExpression (getExecutionContext (), std::move (value), std::move (argumentListExpressions));
				continue;
			}

			if (Grammar::OpenBracket (istream))
			{
				ptr <pbStatement> identifier;

				if (expression (identifier))
				{
					comments ();

					if (Grammar::CloseBracket (istream))
					{
						value = new ComputedMemberAccessExpression (getExecutionContext (), std::move (value), std::move (identifier));
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
					value = new MemberAccessExpression (getExecutionContext (), std::move (value), std::move (identifierNameCharacters));
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
Parser::arguments (array <ptr <pbStatement>> & argumentListExpressions)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
Parser::argumentList (array <ptr <pbStatement>> & argumentListExpressions)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	ptr <pbStatement> value;

	if (assignmentExpression (value))
	{
		argumentListExpressions .emplace_back (std::move (value));

		for (;;)
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
Parser::leftHandSideExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
Parser::postfixExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (leftHandSideExpression (value))
	{
		comments ();

		if (Grammar::Increment (istream))
		{
			isLeftHandSideExressions .back () = false;

			value = new PostIncrementExpression (std::move (value));
			return true;
		}

		if (Grammar::Decrement (istream))
		{
			isLeftHandSideExressions .back () = false;

			value = new PostDecrementExpression (std::move (value));
			return true;
		}

		return true;
	}

	return false;
}

bool
Parser::unaryExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (postfixExpression (value))
		return true;

	if (Grammar::delete_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new DeleteExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after delete.");
	}

	if (Grammar::void_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new VoidExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after void.");
	}

	if (Grammar::typeof_ (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new TypeOfExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after typeof.");
	}

	if (Grammar::Increment (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new PreIncrementExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after ++.");
	}

	if (Grammar::Decrement (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new PreDecrementExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after --.");
	}

	if (Grammar::PlusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new ToNumberExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after +.");
	}

	if (Grammar::MinusSign (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new NegateExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after -.");
	}

	if (Grammar::Tilde (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new BitwiseNotExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after ~.");
	}

	if (Grammar::ExclamationMark (istream))
	{
		isLeftHandSideExressions .back () = false;

		if (unaryExpression (value))
		{
			value = new LogicalNotExpression (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected expression after !.");
	}

	return false;
}

bool
Parser::multiplicativeExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (unaryExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::Multiplication (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::Multiplication .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (unaryExpression (rhs))
				{
					lhs = createMultiplicationExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '*'.");
			}

			if (Grammar::Division (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::Division .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (unaryExpression (rhs))
				{
					lhs = createDivisionExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '/'.");
			}

			if (Grammar::Remainder (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::Remainder .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (unaryExpression (rhs))
				{
					lhs = createRemainderExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '%'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::additiveExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (multiplicativeExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::Addition (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::Addition .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (multiplicativeExpression (rhs))
				{
					lhs = createAdditionExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '+'.");
			}

			if (Grammar::Subtraction (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::Subtraction .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (multiplicativeExpression (rhs))
				{
					lhs = createSubtractionExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '-'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::shiftExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (additiveExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LeftShift (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::LeftShift .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (additiveExpression (rhs))
				{
					lhs = createLeftShiftExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '<<'.");
			}

			if (Grammar::UnsignedRightShift (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::UnsignedRightShift .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (additiveExpression (rhs))
				{
					//lhs = make_unsigned_right_shift (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '>>>'.");
			}

			if (Grammar::RightShift (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::RightShift .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (additiveExpression (rhs))
				{
					//lhs = make_right_shift (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '>>'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::relationalExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (shiftExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LessEqual (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::LessEqual .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (shiftExpression (rhs))
				{
					lhs = createLessEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '<='.");
			}

			if (Grammar::GreaterEqual (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::GreaterEqual .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (shiftExpression (rhs))
				{
					lhs = createGreaterEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '>='.");
			}

			if (Grammar::Less (istream))
			{
				if (Grammar::LessEqual .lookahead (istream))
					return Grammar::Less .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (shiftExpression (rhs))
				{
					lhs = createLessExpression (std::move (lhs), std::move (rhs));
					continue;
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

				ptr <pbStatement> rhs;

				if (shiftExpression (rhs))
				{
					lhs = createGreaterExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '>'.");
			}

			const auto state = getState ();

			std::string identifierCharacters;

			if (identifierName (identifierCharacters))
			{
				if (identifierCharacters == Grammar::instanceof ())
				{
					isLeftHandSideExressions .back () = false;

					ptr <pbStatement> rhs;

					if (shiftExpression (rhs))
					{
						lhs = new InstanceOfExpression (std::move (lhs), std::move (rhs));
						continue;
					}

					throw SyntaxError ("Expected expression after 'instanceof'.");
				}

				if (not noIn)
				{
					if (identifierCharacters == Grammar::in ())
					{
						isLeftHandSideExressions .back () = false;

						ptr <pbStatement> rhs;

						if (shiftExpression (rhs))
						{
							lhs = new InExpression (std::move (lhs), std::move (rhs));
							continue;
						}

						throw SyntaxError ("Expected expression after 'in'.");
					}
				}

				setState (state);
			}

			return true;
		}
	}

	return false;
}

bool
Parser::equalityExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (relationalExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::StrictEqual (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (relationalExpression (rhs))
				{
					lhs = createStrictEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '==='.");
			}

			if (Grammar::StrictNotEqual (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (relationalExpression (rhs))
				{
					lhs = createStrictNotEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '!=='.");
			}

			if (Grammar::Equal (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (relationalExpression (rhs))
				{
					lhs = createEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '=='.");
			}

			if (Grammar::NotEqual (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (relationalExpression (rhs))
				{
					lhs = createNotEqualExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '!='.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseANDExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (equalityExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalAND .lookahead (istream))
				return true;

			if (Grammar::BitwiseAND (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::BitwiseAND .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (equalityExpression (rhs))
				{
					lhs = createBitwiseAndExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseXORExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (bitwiseANDExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::BitwiseXOR (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::BitwiseXOR .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (bitwiseANDExpression (rhs))
				{
					//lhs = make_bitwise_xor (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '^'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::bitwiseORExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (bitwiseXORExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR .lookahead (istream))
				return true;

			if (Grammar::BitwiseOR (istream))
			{
				if (Grammar::Assignment .lookahead (istream))
					return Grammar::BitwiseOR .rewind (istream);

				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (bitwiseXORExpression (rhs))
				{
					//lhs = make_bitwise_or (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '|'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalANDExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (bitwiseORExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalAND (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (bitwiseORExpression (rhs))
				{
					lhs = createLogicalAndExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '&&'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::logicalORExpression (ptr <pbStatement> & lhs)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (logicalANDExpression (lhs))
	{
		for (;;)
		{
			comments ();

			if (Grammar::LogicalOR (istream))
			{
				isLeftHandSideExressions .back () = false;

				ptr <pbStatement> rhs;

				if (logicalANDExpression (rhs))
				{
					lhs = createLogicalOrExpression (std::move (lhs), std::move (rhs));
					continue;
				}

				throw SyntaxError ("Expected expression after '||'.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::conditionalExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	if (logicalORExpression (value))
	{
		comments ();

		if (Grammar::QuestionMark (istream))
		{
			isLeftHandSideExressions .back () = false;

			ptr <pbStatement> thenExpression;

			if (assignmentExpression (thenExpression))
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					ptr <pbStatement> elseExpression;

					if (assignmentExpression (elseExpression))
					{
						value = new ConditionalExpression (std::move (value), std::move (thenExpression), std::move (elseExpression));
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
Parser::assignmentExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

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
				ptr <pbStatement> expression;

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
				ptr <pbStatement> expression;

				if (assignmentExpression (expression))
				{
					value = new AssignmentExpression (getExecutionContext (), std::move (value), std::move (expression), type);
					return true;
				}

				throw SyntaxError ("Expected expression after '" + to_string (type) + "'.");
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
Parser::expression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	array <ptr <pbStatement>>  expressions;

	if (assignmentExpression (value))
	{
		expressions .emplace_back (std::move (value));

		for (;;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (assignmentExpression (value))
				{
					expressions .emplace_back (std::move (value));
					continue;
				}

				throw SyntaxError ("Expected expression after ','.");
			}

			value = createExpression (std::move (expressions));
			return true;
		}
	}

	return false;
}

// A.4 Statements

bool
Parser::statement (ptr <pbStatement> & value)
{
	//__LOG__ << istream .tellg () << std::endl;

	if (block (value))
		return true;

	if (variableStatement (value))
		return true;

	if (emptyStatement (value))
		return true;

	if (expressionStatement (value))
		return true;

	if (ifStatement (value))
		return true;

	if (iterationStatement (value))
		return true;

	if (continueStatement (value))
		return true;

	if (breakStatement (value))
		return true;

	if (returnStatement (value))
		return true;

	//if (withStatement (value))
	//	return true;

	//if (labelledStatement ())
	//	return true;

	if (switchStatement (value))
		return true;

	//if (throwStatement (value))
	//	return true;

	//if (tryStatement (value))
	//	return true;

	//if (debuggerStatement (value))
	//	return true;

	return false;
}

bool
Parser::block (ptr <pbStatement> & value)
{
	comments ();

	if (Grammar::OpenBrace (istream))
	{
		array <ptr <pbStatement>>  statements;

		statementList (statements);

		comments ();

		if (Grammar::CloseBrace (istream))
		{
			value = new BlockStatement (std::move (statements));
			return true;
		}

		throw SyntaxError ("Expected a '}' at end of block statement.");
	}

	return false;
}

bool
Parser::statementList (array <ptr <pbStatement>> & statements)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	ptr <pbStatement> value;

	if (statement (value))
	{
		statements .emplace_back (std::move (value));

		for (;;)
		{
			if (statement (value))
			{
				statements .emplace_back (std::move (value));
				continue;
			}

			return true;
		}
	}

	return false;
}

bool
Parser::variableStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	const auto state = getState ();

	if (Grammar::var (istream))
	{
		if (comments ())
		{
			array <ptr <VariableDeclaration>>  variableDeclarations;

			if (variableDeclarationList (variableDeclarations))
			{
				comments ();

				if (Grammar::Semicolon (istream) or haveAutomaticSemicolon ())
				{
					value = new VariableStatement (std::move (variableDeclarations));
					return true;
				}

				throw SyntaxError ("Expected a ';' after variable declaration.");
			}

			throw SyntaxError ("Expected variable name after 'var'.");
		}

		setState (state);
	}

	return false;
}

bool
Parser::variableDeclarationList (array <ptr <VariableDeclaration>> & variableDeclarations)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	ptr <VariableDeclaration> value;

	if (variableDeclaration (value))
	{
		variableDeclarations .emplace_back (std::move (value));

		for (;;)
		{
			comments ();

			if (Grammar::Comma (istream))
			{
				if (variableDeclaration (value))
				{
					variableDeclarations .emplace_back (std::move (value));
					continue;
				}

				throw SyntaxError ("Expected variable name after ','.");
			}

			return true;
		}
	}

	return false;
}

bool
Parser::variableDeclaration (ptr <VariableDeclaration> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		ptr <pbStatement> initialiserExpression;

		initialiser (initialiserExpression);

		value = new VariableDeclaration (getExecutionContext (), std::move (identifierCharacters), std::move (initialiserExpression));
		return true;
	}

	return false;
}

bool
Parser::initialiser (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::Assignment (istream))
	{
		if (assignmentExpression (value))
			return true;
	}

	return false;
}

bool
Parser::emptyStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::Semicolon (istream))
	{
		value = new EmptyStatement ();
		return true;
	}

	return false;
}

bool
Parser::expressionStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::OpenBrace .lookahead (istream))
		return false;

	if (Grammar::function .lookahead (istream))
		return false;

	if (expression (value))
	{
		comments ();

		if (Grammar::Semicolon (istream) or haveAutomaticSemicolon ())
			return true;

		throw SyntaxError ("Expected ';' after expression.");
	}

	return false;
}

bool
Parser::ifStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::if_ (istream))
	{
		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			ptr <pbStatement> booleanExpression;

			if (expression (booleanExpression))
			{
				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					ptr <pbStatement> thenStatement;
					ptr <pbStatement> elseStatement;

					if (statement (thenStatement))
					{
						comments ();

						if (Grammar::else_ (istream))
							statement (elseStatement);

						value = new IfStatement (std::move (booleanExpression), std::move (thenStatement), std::move (elseStatement));
						return true;
					}

					throw SyntaxError ("Expected a statement after 'if'.");
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
Parser::iterationStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	// ...

	if (Grammar::for_ (istream))
	{
		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			comments ();

			// for (var VariableDeclarationNoIn in Expression) Statement
			// for (var VariableDeclarationListNoIn; Expression Opt; Expression Opt) Statement

			const auto state = getState ();

			if (Grammar::var (istream))
			{
				if (comments ())
				{
					noIn = true;

					array <ptr <VariableDeclaration>>  variableDeclarations;

					if (variableDeclarationList (variableDeclarations))
					{
						noIn = false;

						if (variableDeclarations .size () == 1)
						{
							// for (LeftHandSideExpresssion in Expression) Statement

							comments ();

							if (Grammar::in (istream))
							{
								if (comments ())
								{
									ptr <pbStatement> objectExpression;

									if (expression (objectExpression))
									{
										comments ();

										if (Grammar::CloseParenthesis (istream))
										{
											++ forLevel;
										
											ptr <pbStatement> body;

											if (statement (body))
											{
												-- forLevel;
											
												value = new ForVarInStatement (std::move (variableDeclarations .back ()), std::move (objectExpression), std::move (body));
												return true;
											}

											throw SyntaxError ("Expected a statement after 'for'.");
										}

										throw SyntaxError ("Expected a ')'.");
									}

									throw SyntaxError ("Expected expression after in.");
								}

								Grammar::in .rewind (istream);

								throw SyntaxError ("Unexpected identifier.");
							}
						}

						comments ();

						if (Grammar::Semicolon (istream))
						{
							ptr <pbStatement> booleanExpression;

							expression (booleanExpression);

							comments ();

							if (Grammar::Semicolon (istream))
							{
								ptr <pbStatement> iterationExpression;

								expression (iterationExpression);

								comments ();

								if (Grammar::CloseParenthesis (istream))
								{
									++ forLevel;
										
									ptr <pbStatement> body;

									if (statement (body))
									{
										-- forLevel;

										value = new ForVarStatement (std::move (variableDeclarations), std::move (booleanExpression), std::move (iterationExpression), std::move (body));
										return true;
									}

									throw SyntaxError ("Expected a statement after 'for'.");
								}

								throw SyntaxError ("Expected a ')'.");
							}

							throw SyntaxError ("Expected a ';'.");
						}

						throw SyntaxError ("Expected a ';'.");
					}

					noIn = false;

					throw SyntaxError ("Expected variable declaration after var.");
				}

				setState (state);
			}

			// first parse for (LeftHandSideExpression in Expression) Statement
			// if not parse in, use LeftHandSideExpression as expression for the next step.

			ptr <pbStatement> variableExpression;

			noIn = true;

			expression (variableExpression);

			noIn = false;

			comments ();

			if (Grammar::Semicolon (istream))
			{
				ptr <pbStatement> booleanExpression;

				expression (booleanExpression);

				comments ();

				if (Grammar::Semicolon (istream))
				{
					ptr <pbStatement> iterationExpression;

					expression (iterationExpression);

					comments ();

					if (Grammar::CloseParenthesis (istream))
					{
						++ forLevel;
										
						ptr <pbStatement> body;

						if (statement (body))
						{
							-- forLevel;

							value = new ForStatement (std::move (variableExpression), std::move (booleanExpression), std::move (iterationExpression), std::move (body));
							return true;
						}

						throw SyntaxError ("Expected a statement after 'for'.");
					}

					throw SyntaxError ("Expected a ')'.");
				}

				throw SyntaxError ("Expected a ';'.");
			}

			throw SyntaxError ("Expected a ';'.");
		}

		throw SyntaxError ("Expected a '(' after 'for'.");
	}

	// ...

	return false;
}

bool
Parser::continueStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::continue_ (istream))
	{
		if (forLevel)
		{
			std::string identifierCharacters;

			comments ();

			if (not newLine)
			{
				identifier (identifierCharacters);

				comments ();
			}

			if (Grammar::Semicolon (istream) or haveAutomaticSemicolon ())
			{
				value = new ContinueStatement (std::move (identifierCharacters));
				return true;
			}

			throw SyntaxError ("Expected a ';' after expression.");
		}

		throw SyntaxError ("Illegal continue statement");
	}

	return false;
}

bool
Parser::breakStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::break_ (istream))
	{
		if (forLevel or switchLevel)
		{
			std::string identifierCharacters;

			comments ();

			if (not newLine)
			{
				identifier (identifierCharacters);

				comments ();
			}

			if (Grammar::Semicolon (istream) or haveAutomaticSemicolon ())
			{
				value = new BreakStatement (std::move (identifierCharacters));
				return true;
			}

			throw SyntaxError ("Expected a ';' after expression.");
		}

		throw SyntaxError ("Illegal break statement");
	}

	return false;
}

bool
Parser::returnStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::return_ (istream))
	{
		comments ();

		if (not newLine)
		{
			expression (value);

			comments ();
		}

		if (Grammar::Semicolon (istream) or haveAutomaticSemicolon ())
		{
			value = new ReturnStatement (std::move (value));
			return true;
		}

		throw SyntaxError ("Expected a ';' after expression.");
	}

	return false;
}

bool
Parser::switchStatement (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::switch_ (istream))
	{
		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			ptr <pbStatement> switchExpression;

			if (expression (switchExpression))
			{
				comments ();

				if (Grammar::CloseParenthesis (istream))
				{
					++ switchLevel;
										
					array <ptr <pbStatement>>           clauseSelectors;
					array <array <ptr <pbStatement>>>   clauseStatements;
					array <ptr <pbStatement>>           defaultStatements;

					if (caseBlock (clauseSelectors, clauseStatements, defaultStatements))
					{
						-- switchLevel;

						value = new SwitchStatement (std::move (switchExpression),
						                             std::move (clauseSelectors),
						                             std::move (clauseStatements),
						                             std::move (defaultStatements));
						return true;
					}

					throw SyntaxError ("Expected a case block after 'switch'.");
				}

				throw SyntaxError ("Expected a ')' after expression.");
			}

			throw SyntaxError ("Expected an expression after '('.");
		}

		throw SyntaxError ("Expected a '(' after 'switch'.");
	}

	return false;
}

bool
Parser::caseBlock (array <ptr <pbStatement>> & clauseSelectors,
                   array <array <ptr <pbStatement>>> & clauseStatements,
                   array <ptr <pbStatement>> & defaultStatements)
{
	comments ();

	if (Grammar::OpenBrace (istream))
	{
		comments ();

		caseClauses (clauseSelectors, clauseStatements);

		defaultClause (defaultStatements, clauseSelectors, clauseStatements);

		caseClauses (clauseSelectors, clauseStatements);

		if (Grammar::CloseBrace (istream))
		{
			return true;
		}

		throw SyntaxError ("Expected a '}' at end of case block.");
	}

	return false;
}

void
Parser::caseClauses (array <ptr <pbStatement>> & clauseSelectors,
                     array <array <ptr <pbStatement>>> & clauseStatements)
{
	while (caseClause (clauseSelectors, clauseStatements))
		;
}

bool
Parser::caseClause (array <ptr <pbStatement>> & clauseSelectors,
                    array <array <ptr <pbStatement>>> & clauseStatements)
{
	comments ();

	if (Grammar::case_ (istream))
	{
		if (comments ())
		{
			ptr <pbStatement> clauseSelector;

			if (expression (clauseSelector))
			{
				comments ();

				if (Grammar::Colon (istream))
				{
					array <ptr <pbStatement>>  statements;

					statementList (statements);

					clauseSelectors .emplace_back (std::move (clauseSelector));
					clauseStatements .emplace_back (std::move (statements));
					return true;
				}

				throw SyntaxError ("Expected a ':' after case expression.");
			}

			throw SyntaxError ("Expected a expression after 'case'.");
		}

		throw SyntaxError ("Unexpected identifier.");
	}

	return false;
}

bool
Parser::defaultClause (array <ptr <pbStatement>> & defaultStatements,
                       array <ptr <pbStatement>> & clauseSelectors,
                       array <array <ptr <pbStatement>>> & clauseStatements)
{
	comments ();

	if (Grammar::default_ (istream))
	{
		comments ();

		if (Grammar::Colon (istream))
		{
			statementList (defaultStatements);

			clauseSelectors .emplace_back ();
			clauseStatements .emplace_back (defaultStatements);

			return true;
		}

		throw SyntaxError ("Expected a ':' after 'default'.");
	}

	return false;
}

// A.5 Functions and Programs

bool
Parser::functionDeclaration ()
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		if (identifier (name))
		{
			comments ();

			if (Grammar::OpenParenthesis (istream))
			{
				std::vector <Identifier> formalParameters;

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
Parser::functionExpression (ptr <pbStatement> & value)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	comments ();

	if (Grammar::function (istream))
	{
		std::string name;

		identifier (name);

		comments ();

		if (Grammar::OpenParenthesis (istream))
		{
			std::vector <Identifier> formalParameters;

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
Parser::formalParameterList (std::vector <Identifier> & formalParameters)
{
	//__LOG__ << (char) istream .peek () << std::endl;

	std::string identifierCharacters;

	if (identifier (identifierCharacters))
	{
		formalParameters .emplace_back (std::move (identifierCharacters));

		for (;;)
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
	//__LOG__ << (char) istream .peek () << std::endl;

	sourceElements ();
}

void
Parser::program ()
{
	//__LOG__ << (char) istream .peek () << std::endl;

	sourceElements ();

	if (istream .peek () not_eq std::char_traits <char>::eof ())
		throw SyntaxError ("Unexpected statement.");
}

void
Parser::sourceElements ()
{
	//__LOG__ << (char) istream .peek () << std::endl;

	ptr <pbStatement> value;

	while (sourceElement (value))
	{
		if (value)
			getExecutionContext () -> addStatement (std::move (value));
	}
}

bool
Parser::sourceElement (ptr <pbStatement> & value)
{
	//__LOG__ << istream .tellg () << std::endl;

	if (statement (value))
		return true;

	if (functionDeclaration ())
		return true;

	return false;
}

//	throw SyntaxError ("Unlabelled continue must be inside loop.");
//	throw SyntaxError ("Label '' not found.");
//	throw SyntaxError ("Unlabelled break must be inside loop or switch.");
//	throw SyntaxError ("Label '' not found.");
//	throw Error ("Uncatched yield exception.");

} // pb
} // titania
