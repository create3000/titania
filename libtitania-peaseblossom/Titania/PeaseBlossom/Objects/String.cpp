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

#include "String.h"

#include "../Execution/pbExecutionContext.h"
#include "../Objects/NativeFunction.h"

namespace titania {
namespace pb {

String::String (pbExecutionContext* const executionContext, const Glib::ustring & value) :
	pbObject (),
	   value (value)
{
	const auto & constructor = executionContext -> getStandardClass (StandardClassType::String);

	setConstructor (constructor);
	setProto (constructor -> getPrototype ());
	addProperties (executionContext);
}

String::String (pbExecutionContext* const executionContext, const std::nullptr_t) :
	pbObject (),
	   value ()
{
	addProperties (executionContext);
}

void
String::addProperties (pbExecutionContext* const ec)
{
	addOwnProperty ("length",
	                undefined,
	                pb::NONE,
	                new pb::NativeFunction (ec, "length", getLength, 0),
	                DefaultSetter);
}

///  throws pbError, std::out_of_range
void
String::put (const Identifier & identifier, const var & value, const bool throw_)
{
	const auto index = identifier .toUInt32 ();

	if (index == PROPERTY)
		return pbObject::put (identifier, value, throw_);

	if (index >= this -> value .length ())
		return pbObject::put (identifier, value, throw_);
}

///  throws pbError, std::out_of_range
var
String::get (const Identifier & identifier) const
{
	const auto index = identifier .toUInt32 ();

	if (index == PROPERTY)
		return pbObject::get (identifier);

	if (index >= value .length ())
		return pbObject::get (identifier);

	return Glib::ustring (1, value [index]);
}

pb::var
String::getLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto string = dynamic_cast <String*> (object .getObject () .get ());

	return string -> value .length ();
}

} // pb
} // titania
