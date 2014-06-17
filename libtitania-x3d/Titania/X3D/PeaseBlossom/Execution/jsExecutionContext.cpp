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

#include "jsExecutionContext.h"

#include "../Objects/Function.h"
#include "../Parser/Parser.h"
#include "../Primitives.h"

namespace titania {
namespace pb {

jsExecutionContext::jsExecutionContext (jsExecutionContext* const executionContext, const basic_ptr <jsObject> & globalObject) :
	      jsChildObject (),
	jsInputStreamObject (),
	   executionContext (executionContext),
	       globalObject (globalObject),
	     defaultObjects (),
	        expressions (),
	          functions ()
{
	addChildren (this -> executionContext,
	             this -> globalObject);

	addDefaultObject (globalObject);
}

void
jsExecutionContext::addDefaultObject (const basic_ptr <jsObject> & object)
{
	defaultObjects .emplace_back (object);
	defaultObjects .back () .addParent (this);
}

const var &
jsExecutionContext::getProperty (const std::string & name) const
throw (ReferenceError)
{
	for (const auto & object : basic::reverse_adapter (defaultObjects))
	{
		try
		{
			return object -> getOwnProperty (name);
		}
		catch (const std::out_of_range &)
		{ }
	}

	throw ReferenceError (name + " is not defined.");
}

void
jsExecutionContext::addExpression (var && expression)
{
	expressions .emplace_back (std::move (expression));
	expressions .back () .addParent (this);
}

void
jsExecutionContext::defineFunction (const basic_ptr <jsFunction> & function)
{
	try
	{
		functions .at (function -> getName ()) = function;
	}
	catch (const std::out_of_range &)
	{
		functions .emplace (function -> getName (), function) .first -> second .addParent (this);
	}
}

var
jsExecutionContext::run ()
{
	for (const auto & function : functions)
		getDefaultObject () -> defineProperty (function .second -> getName (), function .second);

	__LOG__ << expressions .size () << std::endl;

	var result = getUndefined ();

	for (const auto & expression : expressions)
		__LOG__ << (result = expression -> toValue ()) << std::endl;

	return result;
}

void
jsExecutionContext::fromStream (std::istream & istream)
throw (SyntaxError)
{
	Parser (this, istream) .parseIntoContext ();
}

void
jsExecutionContext::dispose ()
{
	functions      .clear ();
	defaultObjects .clear ();
	expressions    .clear ();

	jsChildObject::dispose ();
}

} // pb
} // titania
