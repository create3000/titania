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

#include "Array.h"

#include "../Execution/pbExecutionContext.h"
#include "../Objects/NativeFunction.h"
#include "../Objects/Object.h"

#include <Titania/Utility/Range.h>

namespace titania {
namespace pb {

Array::Array (pbExecutionContext* const executionContext) :
	Array (executionContext, nullptr)
{
	const auto & constructor = executionContext -> getStandardClass (StandardClassType::Array);

	setConstructor (constructor);
	setProto (constructor -> getObject ("prototype"));
}

Array::Array (pbExecutionContext* const executionContext, const std::nullptr_t) :
	pbObject (),
	   array ()
{
	addOwnProperty ("length",
	                Undefined,
	                pb::NONE,
	                new pb::NativeFunction (executionContext, "length", getLength, 0),
	                new pb::NativeFunction (executionContext, "length", setLength, 1));
}

void
Array::setIndexedProperty (const uint32_t index, const var & value)
throw (pbException)
{
	try
	{
		if (index >= array .size ())
			array .resize (index + 1);

		addElement (array [index] = value);
	}
	catch (const std::bad_alloc &)
	{
		throw RuntimeError ("Array: out of memory.");
	}
}

var
Array::getIndexedProperty (const uint32_t index) const
throw (std::out_of_range,
       pbException)
{
	return array .at (index);
}

pb::var
Array::setLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto array = dynamic_cast <Array*> (object .getObject () .get ());
	const auto size  = args [0] .toUInt32 ();

	if (size == M_2_32)
		return Undefined; /// XXX

	array -> array .resize (size);

	return Undefined;
}

pb::var
Array::getLength (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	const auto array = dynamic_cast <Array*> (object .getObject () .get ());

	return array -> array .size ();
}

var
Array::call (const uint32_t index, const var & object, const std::vector <var> & arguments) const
throw (pbException,
       std::invalid_argument)
{
	try
	{
		const auto & property = array .at (index);

		if (property .isObject ())
		{
			const auto function = dynamic_cast <pbFunction*> (property .getObject () .get ());

			if (function)
				return function -> call (object, arguments);
		}
	}
	catch (const std::out_of_range &)
	{ }

	throw std::invalid_argument ("pbObject::apply");
}

void
Array::toStream (std::ostream & ostream) const
{
	if (array .empty ())
		return;

	for (const auto & element : std::make_pair (array .begin (), array .end () - 1))
	{
		if (element .isUndefined ())
			ostream << ',';
		else
			ostream << element << ',';
	}

	if (not array .back () .isUndefined ())
		ostream << array .back ();
}

void
Array::dispose ()
{
	array .clear ();

	pbObject::dispose ();
}

} // pb
} // titania
