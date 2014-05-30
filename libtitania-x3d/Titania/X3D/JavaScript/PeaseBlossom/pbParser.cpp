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

#include "pbParser.h"

#include "../../Execution/X3DExecutionContext.h"

// Grammar

#include <Titania/InputOutput.h>

namespace titania {
namespace X3D {
namespace pb {

class Grammar
{
public:

	///  @name General
	static const io::sequence whitespaces;

	// Keywords

	static const io::string _false;
	static const io::string _null;
	static const io::string _true;

	static const std::set <std::string> Keyword;

};

// General
const io::sequence Grammar::whitespaces ("\r\n \t,");

// Keywords
const io::string Grammar::_false ("false");
const io::string Grammar::_null ("null");
const io::string Grammar::_true ("true");

const std::set <std::string> Grammar::Keyword = {
	"break",    "do",       "instanceof", "typeof",
	"case",     "else",     "new",         "var",
	"catch",    "finally",  "return",      "void",
	"continue", "for",      "switch",      "while",
	"debugger", "function", "this",        "with",
	"default",  "if",       "throw",
	"delete",   "in",       "try"

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string Parser::componentName  = "PeaseBlossom";
const std::string Parser::typeName       = "Parser";
const std::string Parser::containerField = "parser";

Parser::Parser (std::istream & istream, X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DParser (),
	    istream (istream),
	whitespaces ()
{ }

void
Parser::parseIntoContext ()
{
	//__LOG__ << std::endl;

	try
	{
		program ();
	}
	catch (const X3DError & error)
	{
		__LOG__ << error .what () << std::endl;
	}
}

void
Parser::comments ()
{
	Grammar::whitespaces (istream, whitespaces);
}

// A.1 Lexical Grammar

bool
Parser::literal ()
{
	//__LOG__ << std::endl;

	if (nullLiteral ())
		return true;

	if (booleanLiteral ())
		return true;

	// ...

	return false;
}

bool
Parser::nullLiteral ()
{
	__LOG__ << std::endl;

	comments ();

	if (Grammar::_null (istream))
		return true;

	return false;
}

bool
Parser::identifier ()
{
	__LOG__ << std::endl;

	std::string _identifierName;

	if (identifierName (_identifierName))
	{
		if (reservedWord (_identifierName))
			throw Error <INVALID_X3D> ("reservedWord");
	}

	return false;
}

bool
Parser::identifierName (std::string & _identifierName)
{
	__LOG__ << std::endl;

	// ...

	return false;
}

bool
Parser::reservedWord (const std::string & _string)
{
	if (Grammar::Keyword .count (_string))
		return true;

	return false;
}

bool
Parser::booleanLiteral ()
{
	__LOG__ << std::endl;

	comments ();

	if (Grammar::_true (istream))
		return true;

	if (Grammar::_false (istream))
		return true;

	return false;
}

// A.2 Number Conversions

// A.3 Expressions

bool
Parser::primaryExpression ()
{
	//__LOG__ << std::endl;

	// ...

	if (identifier ())
		return true;

	if (literal ())
		return true;

	// ...

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
		return true;

	// ...

	return false;
}

bool
Parser::additiveExpression ()
{
	//__LOG__ << std::endl;

	if (multiplicativeExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::shiftExpression ()
{
	//__LOG__ << std::endl;

	if (additiveExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::relationalExpression ()
{
	//__LOG__ << std::endl;

	if (shiftExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::equalityExpression ()
{
	//__LOG__ << std::endl;

	if (relationalExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::bitwiseANDExpression ()
{
	//__LOG__ << std::endl;

	if (equalityExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::bitwiseXORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseANDExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::bitwiseORExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseXORExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::logicalANDExpression ()
{
	//__LOG__ << std::endl;

	if (bitwiseORExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::logicalORExpression ()
{
	//__LOG__ << std::endl;

	if (logicalANDExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::conditionalExpression ()
{
	//__LOG__ << std::endl;

	if (logicalORExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::assignmentExpression ()
{
	//__LOG__ << std::endl;

	if (conditionalExpression ())
		return true;

	// ...

	return false;
}

bool
Parser::expression ()
{
	//__LOG__ << std::endl;

	if (assignmentExpression ())
		return true;

	// ...

	return false;
}

// A.4 Statements

bool
Parser::statement ()
{
	//__LOG__ << std::endl;

	if (expressionStatement ())
		return true;

	return false;
}

bool
Parser::expressionStatement ()
{
	//__LOG__ << std::endl;

	// [lookahead ? {{, function}]

	if (expression ())
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
} // X3D
} // titania
