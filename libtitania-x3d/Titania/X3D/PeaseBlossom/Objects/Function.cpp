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

namespace titania {
namespace pb {

var
Function::create (vsExecutionContext* const executionContext) const
{
	const auto function = make_ptr <Function> (executionContext, getName (), std::vector <std::string> (formalParameters));

	function -> isStrict (this -> isStrict ());
	function -> import (this);
	
	return function;
}

void
Function::construct ()
{
	addClosure (getExecutionContext ());
}

void
Function::addClosure (const basic_ptr <vsExecutionContext> & executionContext)
{
	const auto & defaultObjects = executionContext -> getDefaultObjects ();

	if (defaultObjects .empty ())
		return;

	const auto pair = closures .emplace (executionContext .get (), executionContext -> getLocalObject ());

	pair .first -> second .addParent (this);
	
	for (const auto & object : basic::adapter (defaultObjects .rbegin (), defaultObjects .rend () - 1))
	{
		if (object == executionContext -> getLocalObject ())
			break;

		getDefaultObjects () .emplace_front (object);
	}

	if (executionContext -> isRootContext ())
		return;

	addClosure (executionContext -> getExecutionContext ());
}

void
Function::resolve (const basic_ptr <vsExecutionContext> & executionContext)
{
	if (getExecutionContext () -> isRootContext ())
		return;

	const auto iter = closures .find (executionContext .get ());

	if (iter not_eq closures .end ())
	{
		getDefaultObjects () .emplace_front (iter -> second -> clone (this));

		closures .erase (iter);
		setExecutionContext (getExecutionContext () -> getExecutionContext ());
	}
}

var
Function::call (const basic_ptr <vsObject> & thisObject, const std::vector <var> & arguments)
{
	auto localObject = make_ptr <Object> ();

	localObject -> addProperty ("this", thisObject);

	for (size_t i = 0, size = formalParameters .size (), argc = arguments .size (); i < size; ++ i)
	{
		if (i < argc)
			localObject -> addProperty (formalParameters [i], arguments [i], WRITABLE | CONFIGURABLE);

		else
			localObject -> addProperty (formalParameters [i], make_var <Undefined> (), WRITABLE | CONFIGURABLE);
	}

	setLocalObject (localObject);
	getDefaultObjects () .emplace_back (std::move (localObject));

	const auto value = run ();

	getDefaultObjects () .pop_back ();
	return value;
}

void
Function::dispose ()
{
	closures .clear ();

	vsFunction::dispose ();
	vsExecutionContext::dispose ();
}

} // pb
} // titania
