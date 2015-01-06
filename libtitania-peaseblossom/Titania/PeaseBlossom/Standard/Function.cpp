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
#include "../Objects/Function.h"
#include "../Objects/NativeFunction.h"

namespace titania {
namespace pb {
namespace Standard {
namespace Function {

/// new Function ([arg1[, arg2[, ...argN]],] functionBody)
struct Constructor
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & arguments)
	{
		if (arguments .empty ())
			return new pb::Function (ec);

		return new pb::Function (ec);
	}

};

struct toString
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & arguments)
	{
		if (object .getType () == OBJECT)
		{
			if (dynamic_cast <pbFunction*> (object .getObject () .get ()))
				return object .getObject () -> toString ();
		}

		throw TypeError ("Function.prototype.toString is not generic.");
	}

};

ptr <NativeFunction>
initialize (pbExecutionContext* const ec)
{
	const auto & standardFunction = ec -> getStandardFunction ();
	const auto   constructor      = make_ptr <NativeFunction> (ec, "Function", Constructor { }, 1);

	constructor -> setConstructor (constructor);
	constructor -> setProto (standardFunction);
	constructor -> addPropertyDescriptor ("prototype", standardFunction, WRITABLE | CONFIGURABLE);

	// standardFunction prototype remains undefined.
	standardFunction -> setConstructor (constructor);
	standardFunction -> addPropertyDescriptor ("constructor", constructor,                                          WRITABLE | CONFIGURABLE);
	standardFunction -> addPropertyDescriptor ("toString",    new NativeFunction (ec, "toString", toString { }, 0), WRITABLE | CONFIGURABLE);

	return constructor;
}

} // Function
} // Standard
} // pb
} // titania
