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

#include "Array.h"

#include "../Execution/pbExecutionContext.h"
#include "../Objects/NativeFunction.h"

#include <Titania/Utility/Range.h>

namespace titania {
namespace pb {

Array::Array (pbExecutionContext* const executionContext) :
	Array (executionContext, nullptr)
{
	const auto & constructor = executionContext -> getStandardClass (StandardClassType::Array);

	setConstructor (constructor);
	setProto (constructor -> getPrototype ());
}

Array::Array (pbExecutionContext* const executionContext, const std::nullptr_t) :
	pbObject (),
	  length (0)
{
	addOwnProperty ("length",
	                undefined,
	                pb::NONE,
	                new pb::NativeFunction (executionContext, "length", getLength, 0),
	                new pb::NativeFunction (executionContext, "length", setLength, 1));
}

///  throws pbError, std::out_of_range
void
Array::put (const Identifier & identifier, const var & value, const bool throw_)
{
	const auto index = identifier .toUInt32 ();
	
	if (index == PROPERTY)
		return pbObject::put (identifier, value, PROPERTY, throw_);

	pbObject::put (identifier, value, index, throw_);

	if (index < length)
		return;

	length = index + 1;
}

pb::var
Array::setLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto array = dynamic_cast <Array*> (object .getObject () .get ());
	const auto size  = args [0] .toUInt32 ();

	if (basic::to_string (size, std::locale::classic ()) not_eq args [0] .toString ())
		throw RangeError ("Invalid array length.");

	for (uint32_t index = size; index < array -> length; ++ index)
		array -> deleteOwnProperty (basic::to_string (index, std::locale::classic ()));

	array -> length = size;

	return undefined;
}

pb::var
Array::getLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto array = dynamic_cast <Array*> (object .getObject () .get ());

	return array -> length;
}

void
Array::toStream (std::ostream & ostream) const
{
	if (length == 0)
		return;

	for (uint32_t index = 0, size = length - 1; index < size; ++ index)
	{
		try
		{
			const var value = get (basic::to_string (index, std::locale::classic ()));
		
			if (value .isUndefined ())
				ostream << ',';
			else
				ostream << value << ',';
		}
		catch (const std::out_of_range &)
		{
			ostream << ',';
		}
	}

	try
	{
		const var value = get (basic::to_string (length - 1, std::locale::classic ()));

		if (not value .isUndefined ())
			ostream << value;
	}
	catch (const std::out_of_range &)
	{ }
}

void
Array::dispose ()
{
	pbObject::dispose ();
}

} // pb
} // titania
