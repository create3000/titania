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

#include "pbExecutionContext.h"

#include "../Expressions/ControlFlowException.h"
#include "../Objects/Function.h"
#include "../Parser/Parser.h"
#include "../Primitives.h"

#include <Titania/Utility/Range.h>

namespace titania {
namespace pb {

pbExecutionContext::pbExecutionContext (pbExecutionContext* const executionContext_) :
	             pbBlock (),
	 pbInputStreamObject (),
	          pbUserData (),
	    executionContext (executionContext_),
	        localObjects (),
	           functions (),
	              strict (executionContext_ -> isStrict ())
{
	addChildren (executionContext,
	             localObjects);
}

void
pbExecutionContext::addFunctionDeclaration (const ptr <pbFunction> & function)
throw (std::invalid_argument)
{
	const auto pair = functions .emplace (function -> getName (), function);

	if (pair .second)
		pair .first -> second .addParent (this);

	else
		throw std::invalid_argument ("Function already exists.");
}

void
pbExecutionContext::updateFunctionDeclaration (const ptr <pbFunction> & function)
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
pbExecutionContext::removeFunctionDeclaration (const std::string & identifier)
noexcept (true)
{
	functions .erase (identifier);
}

void
pbExecutionContext::import (const pbExecutionContext* const executionContext)
throw (pbError,
       pbControlFlowException)
{
	for (const auto & function : executionContext -> getFunctionDeclarations ())
		addFunctionDeclaration (function .second);

	pbBlock::import (this, executionContext);
}

var
pbExecutionContext::run ()
throw (pbError)
{
	try
	{
		for (const auto & function : functions)
		{
			getLocalObjects () .front () -> addOwnProperty (function .second -> getName (),
			                                                       isRootContext ()
			                                                       ? function .second
			                                                       : function .second -> copy (this),
			                                                       WRITABLE | CONFIGURABLE);
		}

		pbBlock::run ();

		return undefined;
	}
	catch (const ContinueException &)
	{
		throw SyntaxError ("Unlabelled continue must be inside loop.");
	}
	catch (const LabelledContinueException & error)
	{
		throw SyntaxError ("Label '' not found.");
	}
	catch (const BreakException &)
	{
		throw SyntaxError ("Unlabelled break must be inside loop or switch.");
	}
	catch (const LabelledBreakException & error)
	{
		throw SyntaxError ("Label '' not found.");
	}
	catch (const YieldException &)
	{
		throw Error ("Uncatched yield exception.");
	}
	catch (const ReturnException & exception)
	{
		return exception .getValue ();
	}
	catch (const pbError &)
	{
		throw;
	}
}

void
pbExecutionContext::fromStream (std::istream & istream)
throw (SyntaxError,
       ReferenceError)
{
	Parser (this, istream) .parseIntoContext ();
}

void
pbExecutionContext::dispose ()
{
	functions .clear ();

	pbBlock::dispose ();
}

} // pb
} // titania
