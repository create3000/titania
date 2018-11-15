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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PARSER_PARSER_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PARSER_PARSER_H__

#include "../Bits/Exception.h"
#include "../Expressions/AssignmentOperatorType.h"
#include "../Objects/Object.h"
#include "../Primitives/array.h"
#include "../Primitives/ptr.h"

#include <iostream>
#include <stack>
#include <tuple>

namespace titania {
namespace pb {

class ArrayLiteral;
class ObjectLiteral;
class VariableDeclaration;
class pbSource;
class pbExecutionContext;
class pbStatement;

class Parser
{
protected:

	///  @name Friends

	friend class pbExecutionContext;

	///  @name Construction

	///  Constructs new Parser.
	Parser (pbExecutionContext* const, std::istream & istream);

	///  @name Operations

	///  throws pbError
	void
	parseIntoContext ();


private:

	using State = std::tuple <std::ios_base::iostate, std::istream::pos_type>;

	// Operations

	void
	setError (pbError & error);
	
	std::string
	getline ();

	std::string
	rgetline ();

	pbExecutionContext*
	getRootContext () const
	{ return rootContext; }

	void
	pushExecutionContext (pbExecutionContext* const executionContext)
	{ executionContexts .emplace (executionContext); }

	void
	popExecutionContext ()
	{ executionContexts .pop (); }

	pbExecutionContext*
	getExecutionContext () const
	{ return executionContexts .top (); }

	void
	setState (const State & value);

	State
	getState ();

	void
	lines (const std::string & string);

	bool
	haveAutomaticSemicolon () const;

	// A.1 Lexical Grammar

	bool
	comments ();

	bool
	comment ();

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
	literal (ptr <pbStatement> &);

	bool
	nullLiteral (ptr <pbStatement> &);

	bool
	booleanLiteral (ptr <pbStatement> &);

	bool
	numericLiteral (ptr <pbStatement> &);

	bool
	decimalLiteral (ptr <pbStatement> &);

	bool
	binaryIntegerLiteral (ptr <pbStatement> &);

	bool
	octalIntegerLiteral (ptr <pbStatement> &);

	bool
	hexIntegerLiteral (ptr <pbStatement> &);

	bool
	stringLiteral (ptr <pbStatement> &);

	// A.2 Number Conversions

	// A.3 Expressions

	bool
	primaryExpression (ptr <pbStatement> &);

	bool
	arrayLiteral (ptr <pbStatement> &);

	bool
	elementList (const ptr <ArrayLiteral> &);

	bool
	elision (const ptr <ArrayLiteral> & arrayLiteral);

	bool
	objectLiteral (ptr <pbStatement> &);

	bool
	propertyDefinitionList (const ptr <ObjectLiteral> &);

	bool
	propertyDefinition (const ptr <ObjectLiteral> &);

	bool
	propertyName (ptr <pbStatement> & value);

	bool
	propertySetParameterList (std::vector <Identifier> &);

	bool
	memberExpression (ptr <pbStatement> &);

	bool
	newExpression (ptr <pbStatement> &);

	bool
	callExpression (ptr <pbStatement> &);

	bool
	arguments (array <ptr <pbStatement>> &);

	bool
	argumentList (array <ptr <pbStatement>> &);

	bool
	leftHandSideExpression (ptr <pbStatement> &);

	bool
	postfixExpression (ptr <pbStatement> &);

	bool
	unaryExpression (ptr <pbStatement> &);

	bool
	multiplicativeExpression (ptr <pbStatement> &);

	bool
	additiveExpression (ptr <pbStatement> &);

	bool
	shiftExpression (ptr <pbStatement> &);

	bool
	relationalExpression (ptr <pbStatement> &);

	bool
	equalityExpression (ptr <pbStatement> &);

	bool
	bitwiseANDExpression (ptr <pbStatement> &);

	bool
	bitwiseXORExpression (ptr <pbStatement> &);

	bool
	bitwiseORExpression (ptr <pbStatement> &);

	bool
	logicalANDExpression (ptr <pbStatement> &);

	bool
	logicalORExpression (ptr <pbStatement> &);

	bool
	conditionalExpression (ptr <pbStatement> &);

	bool
	assignmentExpression (ptr <pbStatement> &);

	bool
	assignmentOperator (AssignmentOperatorType &);

	bool
	expression (ptr <pbStatement> &);

	// A.4 Statements

	bool
	statement (ptr <pbStatement> &);
	
	bool
	block (ptr <pbStatement> &);
	
	bool
	statementList (array <ptr <pbStatement>> &);

	bool
	variableStatement (ptr <pbStatement> &);

	bool
	variableDeclarationList (array <ptr <VariableDeclaration>> &);

	bool
	variableDeclaration (ptr <VariableDeclaration> &);

	bool
	initialiser (ptr <pbStatement> &);

	bool
	emptyStatement (ptr <pbStatement> &);

	bool
	expressionStatement (ptr <pbStatement> &);

	bool
	ifStatement (ptr <pbStatement> &);

	bool
	iterationStatement (ptr <pbStatement> &);

	bool
	continueStatement (ptr <pbStatement> &);

	bool
	breakStatement (ptr <pbStatement> &);

	bool
	returnStatement (ptr <pbStatement> &);
	
	bool
	switchStatement (ptr <pbStatement> &);
	
	bool
	caseBlock (array <ptr <pbStatement>> &, array <array <ptr <pbStatement>>> &, array <ptr <pbStatement>> &);

	void
	caseClauses (array <ptr <pbStatement>> &, array <array <ptr <pbStatement>>> &);

	bool
	caseClause (array <ptr <pbStatement>> &, array <array <ptr <pbStatement>>> &);

	bool
	defaultClause (array <ptr <pbStatement>> &, array <ptr <pbStatement>> &, array <array <ptr <pbStatement>>> &);

	// A.5 Functions and Programs

	bool
	functionDeclaration ();

	bool
	functionExpression (ptr <pbStatement> &);

	bool
	formalParameterList (std::vector <Identifier> &);

	void
	functionBody ();

	void
	program ();

	void
	sourceElements ();

	bool
	sourceElement (ptr <pbStatement> &);

	///  @name Members

	pbExecutionContext* const        rootContext;
	std::stack <pbExecutionContext*> executionContexts;
	std::istream &                   istream;
	std::istream::pos_type           position;
	size_t                           lineNumber;
	std::string                      whiteSpaces;
	std::string                      commentCharacters;
	bool                             newLine;
	std::vector <bool>               isLeftHandSideExressions;
	bool                             noIn;
	size_t                           forLevel;
	size_t                           switchLevel;

};

} // pb
} // titania

#endif
