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

#include "Function.h"

#include "../Objects/Object.h"

#include <Titania/String/to_string.h>

namespace titania {
namespace pb {

std::atomic <size_t> Function::recursionLimit (100000);

Function::Function (pbExecutionContext* const executionContext, const std::string & name, std::vector <std::string> && formalParameters) :
	           pbFunction (executionContext, name, formalParameters .size ()),
	   pbExecutionContext (executionContext),
	     formalParameters (std::move (formalParameters)),
	         localObjects (),
	    localObjectsStack (),
	       recursionDepth (0)
{
	addChildren (localObjectsStack, localObjects);

	addLocalObjects (getExecutionContext ());
}

Function::Function (pbExecutionContext* const executionContext, const std::nullptr_t) :
	           pbFunction (executionContext, nullptr),
	   pbExecutionContext (executionContext),
	     formalParameters (),
	         localObjects (),
	    localObjectsStack (),
	       recursionDepth (0)
{
	addChildren (localObjectsStack, localObjects);
}

ptr <pbFunction>
Function::copy (pbExecutionContext* const executionContext) const
noexcept (true)
{
	const auto function = make_ptr <Function> (executionContext, getName (), std::vector <std::string> (formalParameters));

	function -> import (this);

	return function;
}

void
Function::addLocalObjects (const ptr <pbExecutionContext> & executionContext)
{
	localObjects .append (executionContext -> getLocalObjects ());

	if (executionContext -> isRootContext ())
		return;

	addLocalObjects (executionContext -> getExecutionContext ());
}

var
Function::construct (const ptr <pbExecutionContext> &, const var & object, const std::vector <var> & arguments)
throw (pbException)
{
	return apply (object, arguments);
}

var
Function::apply (const var & object, const std::vector <var> & arguments)
throw (pbException)
{
	const auto localObject = new Object (static_cast <pbExecutionContext*> (this));

	localObject -> addPropertyDescriptor ("this", object, NONE);

	for (size_t i = 0, size = formalParameters .size (), argc = arguments .size (); i < size; ++ i)
		localObject -> addPropertyDescriptor (formalParameters [i], i < argc ? arguments [i] : var (), WRITABLE | CONFIGURABLE);

	StackGuard guard (this, localObject);

	return run ();
}

void
Function::push (pbObject* const localObject)
{
	if (recursionDepth)
		localObjectsStack .append (std::move (getLocalObjects ()));

	++ recursionDepth;

	getLocalObjects () .emplace_back (localObject);
	getLocalObjects () .append (localObjects);

	// As LAST step check recursion depth.

	if (recursionDepth > recursionLimit)
		throw RuntimeError ("Maximum recursion depth of " + basic::to_string (recursionLimit) + " exceeded while calling function '" + getName () + "'.");
}

void
Function::pop ()
{
	getLocalObjects () .clear ();

	-- recursionDepth;

	if (recursionDepth)
	{
		const auto size = localObjects .size () + 1;
	
		for (auto localObject : std::make_pair (localObjectsStack .end () - size, localObjectsStack .end ()))
			getLocalObjects () .emplace_back (std::move (localObject));

		localObjectsStack .resize (localObjectsStack .size () - size);
	}
}

void
Function::toStream (std::ostream & ostream) const
{
	ostream << "function " << getName () << " (";
	
	if (not formalParameters .empty ())
	{
		for (const auto parameter : std::make_pair (formalParameters .begin (), formalParameters .end () - 1))
			ostream << parameter << ", ";

		ostream << formalParameters .back ();
	}

	ostream << ") { }";
}

void
Function::dispose ()
{
	pbFunction::dispose ();
	pbExecutionContext::dispose ();
}

} // pb
} // titania
