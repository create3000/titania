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

#include "vsExecutionContext.h"

#include "../Expressions/ControlFlowException.h"
#include "../Objects/Function.h"
#include "../Parser/Parser.h"
#include "../Primitives.h"

namespace titania {
namespace pb {

vsExecutionContext::vsExecutionContext (vsExecutionContext* const executionContext, const basic_ptr <vsObject> & globalObject) :
	      vsChildObject (),
	vsInputStreamObject (),
	             strict (false),
	   executionContext (executionContext),
	       globalObject (globalObject),
	     defaultObjects (),
	        expressions (),
	          functions ()
{ construct (); }

void
vsExecutionContext::construct ()
{
	addChildren (executionContext,
	             globalObject);

	addDefaultObject (globalObject);
}

void
vsExecutionContext::addDefaultObject (const basic_ptr <vsObject> & object)
{
	defaultObjects .emplace_back (object);
	defaultObjects .back () .addParent (this);
}

void
vsExecutionContext::addExpression (var && expression)
{
	expressions .emplace_back (std::move (expression));
	expressions .back () .addParent (this);
}

void
vsExecutionContext::addFunctionDeclaration (const basic_ptr <vsFunction> & function)
throw (std::invalid_argument)
{
	const auto pair = functions .emplace (function -> getName (), function);
	
	if (pair .second)
		pair .first -> second .addParent (this);

	else
		throw std::invalid_argument ("Function already exists.");
}

void
vsExecutionContext::updateFunctionDeclaration (const basic_ptr <vsFunction> & function)
throw (std::invalid_argument)
{
	try
	{
		functions .at (function -> getName ()) = function;
	}
	catch (const std::out_of_range &)
	{
		addFunctionDeclaration (function);
	}
}

void
vsExecutionContext::removeFunctionDeclaration (const std::string & name)
noexcept (true)
{
	functions .erase (name);
}

void
vsExecutionContext::import (const vsExecutionContext* const executionContext)
{
	for (const auto & function : functions)
		addFunctionDeclaration (function .second -> copy (this));

	for (const auto & expression : executionContext -> getExpressions ())
		addExpression (expression -> copy (this));
}

var
vsExecutionContext::run ()
{
	try
	{
		for (const auto & function : functions)
			getDefaultObject () -> updateProperty (function .second -> getName (), function .second);

		for (const auto & expression : expressions)
			expression -> toValue ();

		return getUndefined ();
	}
	catch (const ReturnException & exception)
	{
		return exception .toValue ();
	}
	catch (const BreakException &)
	{
		throw SyntaxError ("Unlabeled break must be inside loop or switch.");
	}
	catch (const ContinueException &)
	{
		throw SyntaxError ("continue must be inside loop.");
	}
	catch (const GotoException &)
	{
		throw Error ("Uncatched goto exception.");
	}
	catch (const YieldException &)
	{
		throw Error ("Uncatched yield exception.");
	}
	catch (...)
	{
		throw;
	}
}

void
vsExecutionContext::fromStream (std::istream & istream)
throw (SyntaxError)
{
	Parser (this, istream) .parseIntoContext ();
}

void
vsExecutionContext::dispose ()
{
	functions      .clear ();
	defaultObjects .clear ();
	expressions    .clear ();

	vsChildObject::dispose ();
}

} // pb
} // titania
