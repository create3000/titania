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

#include "GlobalObject.h"

#include "../Bits/pbMath.h"
#include "../Objects/Function.h"
#include "../Objects/NativeFunction.h"
#include "../Objects/Object.h"
#include "../Objects/Math.h"

namespace titania {
namespace pb {
namespace GlobalObject {

/// new Function ([arg1[, arg2[, ...argN]],] functionBody)
struct Function
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return new pb::Function (ec);

		return new pb::Function (ec);
	}

};

/// new Object ([ value ])
struct Object
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return new pb::Object (ec);

		return new pb::Object (ec);
	}

};

struct isNaN
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return true;

		return pb::isNaN (arguments [0] .toNumber ());
	}

};

struct parseInt
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return NaN ();

		return pb::parseInt (arguments [0] .toString ());
	}

};

struct parseFloat
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const ptr <pbObject> & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return NaN ();

		return pb::parseFloat (arguments [0] .toString ());
	}

};

static
void
addFunction (pbExecutionContext* const ec, const ptr <pbObject> & object)
{
	const auto standardFunction = ec -> getStandardFunction ();
	const auto constructor      = make_ptr <NativeFunction> (ec, "Function", Function { }, 1);

	constructor -> addPropertyDescriptor ("constructor", constructor,      NONE);
	constructor -> addPropertyDescriptor ("prototype",   standardFunction, NONE);

	object -> addPropertyDescriptor ("Function", constructor, NONE);

	standardFunction -> addPropertyDescriptor ("constructor", constructor, NONE);
	// standardFunction prototype remains undefined.
}

static
void
addObject (pbExecutionContext* const ec, const ptr <pbObject> & object)
{
	const auto standardObject = ec -> getStandardObject ();
	const auto constructor    = make_ptr <NativeFunction> (ec, "Object", Object { }, 1);

	object -> addPropertyDescriptor ("Object", constructor, NONE);

	standardObject -> addPropertyDescriptor ("constructor", constructor, NONE);
}

void
addStandardClasses (pbExecutionContext* const ec, const ptr <pbObject> & object)
{
	addFunction (ec, object);
	addObject (ec, object);

	// Properties

	object -> addPropertyDescriptor ("this",      object,               NONE);
	object -> addPropertyDescriptor ("NaN",       NaN (),               NONE);
	object -> addPropertyDescriptor ("Infinity",  POSITIVE_INFINITY (), NONE);
	object -> addPropertyDescriptor ("undefined", Undefined (),         NONE);
	object -> addPropertyDescriptor ("Math",      new Math (ec),        NONE);

	// Functions

	object -> addPropertyDescriptor ("isNaN",      new NativeFunction (ec, "isNaN",      isNaN { },      1), NONE);
	object -> addPropertyDescriptor ("parseInt",   new NativeFunction (ec, "parseInt",   parseInt { },   1), NONE);
	object -> addPropertyDescriptor ("parseFloat", new NativeFunction (ec, "parseFloat", parseFloat { }, 1), NONE);
}

} // GlobalObject
} // pb
} // titania
