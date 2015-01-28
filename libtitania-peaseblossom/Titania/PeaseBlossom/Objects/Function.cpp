/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Function.h"

#include "../Expressions/VariableDeclaration.h"
#include "../Objects/Object.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace pb {

std::atomic <size_t> Function::recursionLimit (100000);

Function::Function (pbExecutionContext* const executionContext, const std::string & name, std::vector <Identifier> && formalParameters) :
	          pbFunction (executionContext, name, formalParameters .size ()),
	  pbExecutionContext (executionContext),
	    formalParameters (std::move (formalParameters)),
	variableObjectsStack (),
	      recursionDepth (0)
{
	const auto prototype = new Object (executionContext);

	addOwnProperty ("prototype", prototype, WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("constructor", this, WRITABLE | CONFIGURABLE);

	addChildren (variableObjectsStack);

	getVariableObjects () .resize (1);

	addLocalObjects (getExecutionContext ());
}

Function::Function (pbExecutionContext* const executionContext, const std::nullptr_t) :
	          pbFunction (executionContext, nullptr),
	  pbExecutionContext (executionContext),
	    formalParameters (),
	variableObjectsStack (),
	      recursionDepth (0)
{
	addChildren (variableObjectsStack, variableObjects);
}

ptr <pbFunction>
Function::copy (pbExecutionContext* const executionContext) const
noexcept (true)
{
	const auto function = make_ptr <Function> (executionContext, getName (), std::vector <Identifier> (formalParameters));

	function -> import (this);

	return function;
}

void
Function::addLocalObjects (const ptr <pbExecutionContext> & executionContext)
{
	getVariableObjects () .emplace_back (executionContext -> getVariableObject ());

	if (executionContext -> isRootContext ())
		return;

	addLocalObjects (executionContext -> getExecutionContext ());
}

const ptr <pbObject> &
Function::getLocalObjects () const
{
	if (variableObjectsStack .size () <= recursionDepth)
		variableObjectsStack .emplace_back ();

	auto & variableObject = variableObjectsStack [recursionDepth];

	if (not variableObject or variableObject -> getReferenceCount () > 1)
		variableObject = new Object (nullptr);

	return variableObject;
}

var
Function::construct (pbObject* const object, const std::vector <var> & arguments)
throw (pbError)
{
	return call (object, arguments);
}

var
Function::call (pbObject* const object, const std::vector <var> & arguments)
throw (pbError)
{
	static const Identifier this_ ("this");

	const auto & variableObject = getLocalObjects ();

	getVariableObjects () .front () = variableObject;

	const auto & thisDescriptor = variableObject -> getOwnProperty (this_);

	if (thisDescriptor)
		const_cast <ptr <pbObject> &> (thisDescriptor -> getValue () .getObject ()) .reset (object);
	else
		variableObject -> addOwnProperty (this_, object, CONFIGURABLE);

	//variableObject -> defineOwnProperty ("arguments", arguments, CONFIGURABLE);

	for (const auto & function : getFunctionDeclarations ())
		variableObject -> defineOwnProperty (function .second -> getName (), function .second -> copy (this), WRITABLE | CONFIGURABLE);

	for (const auto & variable : getVariableDeclarations ())
		variable -> setup ();

	for (size_t i = 0, size = formalParameters .size (), argc = arguments .size (); i < size; ++ i)
	{
		const auto & descriptor = variableObject -> getOwnProperty (formalParameters [i]);

		if (descriptor)
			descriptor -> setValue (i < argc ? arguments [i] : undefined);

		else
			variableObject -> addOwnProperty (formalParameters [i], i < argc ? arguments [i] : undefined, WRITABLE | CONFIGURABLE);
	}

	// Evaluate statements

	StackGuard guard (this);

	for (const auto & statement : getStatements ())
	{
		const auto value = statement -> getValue ();

		if (value .getStatement ())
		{
			switch (value .getStatement () -> getType ())
			{
				case StatementType::RETURN_STATEMENT:
					return value;
				default:
					continue;
			}
		}
	}

	return undefined;
}

void
Function::enter ()
{
	// As LAST step check recursion depth.

	++ recursionDepth;

	if (recursionDepth > recursionLimit)
		throw RuntimeError ("Maximum recursion depth of " + basic::to_string (recursionLimit) + " exceeded while calling function '" + getName () + "'.");
}

void
Function::leave ()
{
	-- recursionDepth;

	if (recursionDepth)
		getVariableObjects () [0] = variableObjectsStack [recursionDepth - 1];
	else
		getVariableObjects () [0] = nullptr;
}

void
Function::toStream (std::ostream & ostream) const
{
	ostream
		<< Generator::Indent
		<< "function"
		<< Generator::Space
		<< getName ()
		<< Generator::TidySpace
		<< '(';

	if (not formalParameters .empty ())
	{
		for (const auto parameter : std::make_pair (formalParameters .begin (), formalParameters .end () - 1))
		{
			ostream
				<< parameter
				<< ','
				<< Generator::TidySpace;
		}

		ostream << formalParameters .back ();
	}

	ostream << ')';

	if (getStatements () .empty ())
	{
		ostream
			<< Generator::TidySpace
			<< '{'
			<< Generator::TidySpace
			<< '}';
	}
	else
	{
		ostream
			<< Generator::TidyBreak
			<< Generator::Indent
			<< Generator::IncIndent
			<< '{';

		pbExecutionContext::toStream (ostream);

		ostream
			<< Generator::DecIndent
			<< Generator::Indent
			<< '}';
	}
}

void
Function::dispose ()
{
	pbFunction::dispose ();
	pbExecutionContext::dispose ();
}

} // pb
} // titania
