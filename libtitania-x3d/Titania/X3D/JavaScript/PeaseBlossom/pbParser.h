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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_PB_PARSER_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_PB_PARSER_H__

#include "../../Parser/X3DParser.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

class Parser :
	public X3DParser
{
public:

	///  @name Construction

	Parser (std::istream & istream, X3DExecutionContext* const);

	///  @name Common members

	virtual
	const std::string &
	getComponentName () const final override
	{ return componentName; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Operations

	void
	parseIntoContext ();


private:

	void
	comments ();

	// A.1 Lexical Grammar

	bool
	identifier ();
	
	bool
	identifierName (std::string &);

	bool
	reservedWord (const std::string &);

	bool
	literal ();

	bool
	nullLiteral (std::istream &);

	bool
	booleanLiteral (std::istream &);

	bool
	numericLiteral ();

	bool
	decimalLiteral ();

	bool
	hexIntegerLiteral ();

	// A.2 Number Conversions

	// A.3 Expressions

	bool
	primaryExpression ();

	bool
	memberExpression ();

	bool
	newExpression ();

	bool
	leftHandSideExpression ();

	bool
	postfixExpression ();

	bool
	unaryExpression ();

	bool
	multiplicativeExpression ();

	bool
	additiveExpression ();

	bool
	shiftExpression ();

	bool
	relationalExpression ();

	bool
	equalityExpression ();

	bool
	bitwiseANDExpression ();

	bool
	bitwiseXORExpression ();

	bool
	bitwiseORExpression ();

	bool
	logicalANDExpression ();

	bool
	logicalORExpression ();

	bool
	conditionalExpression ();

	bool
	assignmentExpression ();

	bool
	expression ();

	// A.4 Statements

	bool
	statement ();

	bool
	expressionStatement ();

	// A.5 Functions and Programs

	bool
	functionDeclaration ();

	void
	program ();

	void
	sourceElements ();

	bool
	sourceElement ();

	///  @name Static members

	static const std::string componentName;
	static const std::string typeName;
	static const std::string containerField;

	///  @name Members

	std::istream & istream;
	
	std::string whitespaces;

};

} // peaseblossom
} // X3D
} // titania

#endif
