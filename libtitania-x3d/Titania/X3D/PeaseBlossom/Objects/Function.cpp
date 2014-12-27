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
	           pbFunction (name),
	   pbExecutionContext (executionContext, executionContext -> getGlobalObject ()),
	     formalParameters (std::move (formalParameters)),
	       recursionDepth (0),
	    localObjectsStack (),
	   resolvedProperties ()
{
	addChildren (localObjectsStack);
}

ptr <pbBaseObject>
Function::copy (pbExecutionContext* executionContext) const
{
	const auto function = make_ptr <Function> (executionContext, getName (), std::vector <std::string> (formalParameters));

	function -> isStrict (isStrict ());
	function -> import (this);

	return pbObject::copy (executionContext, function);
}

void
Function::setExecutionContext (const ptr <pbExecutionContext> & executionContext)
{
	if (getExecutionContext () -> isRootContext ())
		return;

	if (executionContext not_eq getExecutionContext () -> getExecutionContext ())
		return;

	const auto & propertyDescriptors = getExecutionContext () -> getLocalObject () -> getPropertyDescriptors ();

	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		const auto pair = resolvedProperties .emplace (propertyDescriptor .first, propertyDescriptor .second -> value);

		if (pair .second)
			addValue (pair .first -> second);
	}

	pbExecutionContext::setExecutionContext (executionContext);
}

var
Function::call (const ptr <pbObject> & thisObject, const std::vector <var> & arguments)
{
	auto localObject = make_ptr <Object> ();

	localObject -> addProperty ("this", thisObject);

	for (size_t i = 0, size = formalParameters .size (), argc = arguments .size (); i < size; ++ i)
	{
		if (i < argc)
			localObject -> addProperty (formalParameters [i], arguments [i], WRITABLE | CONFIGURABLE);

		else
			localObject -> addProperty (formalParameters [i], var (), WRITABLE | CONFIGURABLE);
	}

	for (const auto & property : resolvedProperties)
	{
		try
		{
			localObject -> addProperty (property .first, property .second);
		}
		catch (const std::invalid_argument &)
		{ }
	}

	try
	{
		push (localObject);

		const auto value = run ();

		pop ();
		return value;
	}
	catch (...)
	{
		pop ();
		throw;
	}
}

void
Function::push (ptr <pbObject> && localObject)
{
	if (recursionDepth)
		localObjectsStack .emplace_back (std::move (getLocalObject ()));

	++ recursionDepth;

	setLocalObject (std::move (localObject));

	// As LAST step check recursion depth.

	if (recursionDepth > recursionLimit)
		throw RuntimeError ("Maximum recursion depth of " + basic::to_string (recursionLimit) + " exceeded while calling function '" + getName () + "'.");
}

void
Function::pop ()
{
	-- recursionDepth;

	if (recursionDepth)
	{
		setLocalObject (std::move (localObjectsStack .back ()));

		localObjectsStack .pop_back ();
	}
}

void
Function::dispose ()
{
	resolvedProperties .clear ();

	pbFunction::dispose ();
	pbExecutionContext::dispose ();
}

} // pb
} // titania
