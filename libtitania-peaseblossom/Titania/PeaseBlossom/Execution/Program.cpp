/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraße 31a, Leipzig, Germany 2011.
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
 * Copyright 1999, 2016 Holger Seelig <holger.seelig@yahoo.de>.
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

#include "Program.h"

#include "../Expressions/VariableDeclaration.h"

#include "../Objects/NativeFunction.h"
#include "../Objects/Function.h"
#include "../Objects/Object.h"

#include "../Standard/Function.h"
#include "../Standard/Global.h"
#include "../Standard/Object.h"
#include "../Standard/Array.h"
#include "../Standard/Date.h"
#include "../Standard/String.h"

namespace titania {
namespace pb {

const std::string Program::typeName = "Program";

///  Constructs new Program.
Program::Program () :
	pbExecutionContext (this),
	    standardObject (new Object (nullptr)),
	  standardFunction (new Function (this, nullptr)),
	   standardClasses ((size_t) StandardClassType::SIZE)
{
	addChildren (standardObject, standardFunction, standardClasses);
}

void
Program::initialize ()
noexcept (true)
{
	using namespace std::placeholders;

	const auto functionClass = Standard::Function::initialize (this);
	standardClasses [(size_t) StandardClassType::Function] = functionClass;
	Standard::Function::realize (this);

	const auto objectClass = Standard::Object::initialize (this, functionClass);
	standardClasses [(size_t) StandardClassType::Object] = objectClass;

	// Add global object.
	getVariableObjects () .emplace_back (new Standard::GlobalObject (this, std::bind (&Program::resolve, this, _1)));

	getGlobalObject () -> addOwnProperty ("Function", functionClass, WRITABLE | CONFIGURABLE);
	getGlobalObject () -> addOwnProperty ("Object",   objectClass,   WRITABLE | CONFIGURABLE);
}

bool
Program::resolve (const Identifier & identifier)
{
	static const std::map <pb::Identifier, StandardClassType> types = {
		std::pair ("Array",  StandardClassType::Array),
		std::pair ("Date",   StandardClassType::Date),
		std::pair ("String", StandardClassType::String),
	};

	const auto iter = types .find (identifier);

	if (iter == types .end ())
		return false;

	getGlobalObject () -> addOwnProperty (identifier, getStandardClass (iter -> second), WRITABLE | CONFIGURABLE);
	return true;
}

///  throws std::out_of_range
const ptr <pbFunction> &
Program::getStandardClass (const StandardClassType type) const
{
	using Initialize = std::function <ptr <NativeFunction> (pbExecutionContext* const)>;

	static const std::map <StandardClassType, Initialize> functions = {
		std::pair (StandardClassType::Array,  Standard::Array::initialize),
		std::pair (StandardClassType::Date,   Standard::Date::initialize),
		std::pair (StandardClassType::String, Standard::String::initialize),
	};

	auto & standardClass = standardClasses .at (size_t (type));

	if (standardClass)
		return standardClass;

	return standardClass = functions .at (type) (const_cast <Program*> (this));
}

///  throws pbError
var
Program::run ()
{
	for (const auto & function : getFunctionDeclarations ())
		getGlobalObject () -> defineOwnProperty (function .second -> getName (), function .second, WRITABLE | CONFIGURABLE);

	for (const auto & variable : getVariableDeclarations ())
		getGlobalObject () -> defineOwnProperty (variable -> getIdentifier (), undefined, WRITABLE | ENUMERABLE);

	var result;

	for (const auto & statement : getStatements ())
	{
		auto value = statement -> getValue ();

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

		result = std::move (value);
	}

	return result;
}

void
Program::dispose ()
{
	//__LOG__ << std::endl;
	pbExecutionContext::dispose ();
}

ptr <Program>
createProgram ()
{
	const auto program = ptr <Program> (new Program ());

	program -> initialize ();

	return program;
}

} // pb
} // titania
