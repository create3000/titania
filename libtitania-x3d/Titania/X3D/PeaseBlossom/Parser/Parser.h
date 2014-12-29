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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PARSER_PARSER_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PARSER_PARSER_H__

#include "../Bits/Exception.h"
#include "../Expressions/AssignmentOperatorType.h"
#include "../Objects/Object.h"
#include "../Primitives/ptr.h"

#include <iostream>
#include <stack>
#include <tuple>

namespace titania {
namespace pb {

class pbExecutionContext;
class pbBlock;

class Parser
{
protected:

	///  @name Friends

	friend class pbExecutionContext;

	///  @name Construction

	///  Constructs new StringObject.
	Parser (pbExecutionContext* const, std::istream & istream);

	///  @name Operations

	void
	parseIntoContext ()
	throw (SyntaxError,
	       ReferenceError);


private:

	using State = std::tuple <std::ios_base::iostate, std::istream::pos_type>;

	// Operations

	pbExecutionContext*
	getRootContext () const
	{ return rootContext; }

	void
	pushExecutionContext (pbExecutionContext* const executionContext);

	void
	popExecutionContext ();

	pbExecutionContext*
	getExecutionContext () const
	{ return executionContexts .top (); }

	void
	pushBlock (pbBlock* const block)
	{ blocks .emplace (block); }

	void
	popBlock ()
	{ blocks .pop (); }

	pbBlock*
	getBlock () const
	{ return blocks .top (); }

	void
	setState (const State & value);

	State
	getState ();

	// A.1 Lexical Grammar

	void
	comments ();

	bool
	comment ();

	void
	commentsNoLineTerminator ();

	bool
	commentNoLineTerminator ();

	bool
	identifier (std::string &);

	bool
	identifierName (std::string &);

	bool
	identifierStart (std::string &);

	bool
	identifierPart (std::string &);

	bool
	reservedWord (const std::string &);

	bool
	literal (ptr <pbExpression> &);

	bool
	nullLiteral (ptr <pbExpression> &);

	bool
	booleanLiteral (ptr <pbExpression> &);

	bool
	numericLiteral (ptr <pbExpression> &);

	bool
	decimalLiteral (ptr <pbExpression> &);

	bool
	binaryIntegerLiteral (ptr <pbExpression> &);

	bool
	octalIntegerLiteral (ptr <pbExpression> &);

	bool
	hexIntegerLiteral (ptr <pbExpression> &);

	bool
	stringLiteral (ptr <pbExpression> &);

	// A.2 Number Conversions

	// A.3 Expressions

	bool
	primaryExpression (ptr <pbExpression> &);

	bool
	objectLiteral (ptr <pbExpression> &);

	bool
	propertyDefinitionList (const ptr <Object> &);

	bool
	propertyDefinition (const ptr <Object> &);

	bool
	propertyName (ptr <pbExpression> & value);

	bool
	propertySetParameterList (std::vector <std::string> &);

	bool
	memberExpression (ptr <pbExpression> &);

	bool
	newExpression (ptr <pbExpression> &);

	bool
	callExpression (ptr <pbExpression> &);

	bool
	arguments (array <ptr <pbExpression>> &);

	bool
	argumentList (array <ptr <pbExpression>> &);

	bool
	leftHandSideExpression (ptr <pbExpression> &);

	bool
	postfixExpression (ptr <pbExpression> &);

	bool
	unaryExpression (ptr <pbExpression> &);

	bool
	multiplicativeExpression (ptr <pbExpression> &);

	bool
	additiveExpression (ptr <pbExpression> &);

	bool
	shiftExpression (ptr <pbExpression> &);

	bool
	relationalExpression (ptr <pbExpression> &);

	bool
	equalityExpression (ptr <pbExpression> &);

	bool
	bitwiseANDExpression (ptr <pbExpression> &);

	bool
	bitwiseXORExpression (ptr <pbExpression> &);

	bool
	bitwiseORExpression (ptr <pbExpression> &);

	bool
	logicalANDExpression (ptr <pbExpression> &);

	bool
	logicalORExpression (ptr <pbExpression> &);

	bool
	conditionalExpression (ptr <pbExpression> &);

	bool
	assignmentExpression (ptr <pbExpression> &);

	bool
	assignmentOperator (AssignmentOperatorType &);

	bool
	expression (ptr <pbExpression> &);

	// A.4 Statements

	bool
	statement ();
	
	bool
	block ();
	
	bool
	statementList ();

	bool
	variableStatement ();

	bool
	variableDeclarationList ();

	bool
	variableDeclaration ();

	bool
	initialiser (ptr <pbExpression> &);

	bool
	emptyStatement ();

	bool
	expressionStatement ();

	bool
	ifStatement ();

	bool
	iterationStatement ();

	bool
	returnStatement ();

	// A.5 Functions and Programs

	bool
	functionDeclaration ();

	bool
	functionExpression (ptr <pbExpression> &);

	bool
	formalParameterList (std::vector <std::string> &);

	void
	functionBody ();

	void
	program ();

	void
	sourceElements ();

	bool
	sourceElement ();

	///  @name Members

	pbExecutionContext* const        rootContext;
	std::stack <pbExecutionContext*> executionContexts;
	std::stack <pbBlock*>            blocks;
	std::istream &                   istream;
	std::string                      whiteSpaces;
	std::string                      commentCharacters;
	std::vector <bool>               isLeftHandSideExressions;

};

} // pb
} // titania

#endif
