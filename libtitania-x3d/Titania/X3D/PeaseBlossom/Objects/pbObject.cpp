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

#include "pbObject.h"

#include "../Objects/pbFunction.h"

namespace titania {
namespace pb {

PropertyDescriptor::~PropertyDescriptor ()
{ }

const std::string pbObject::typeName = "Object";

ptr <pbObject>
pbObject::clone (pbExecutionContext* const executionContext) const
{
	const ptr <pbObject> clone = create (executionContext);

	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		try
		{
			clone -> addProperty (propertyDescriptor .first,
			                      propertyDescriptor .second -> value,
			                      propertyDescriptor .second -> flags,
			                      propertyDescriptor .second -> get,
			                      propertyDescriptor .second -> set);
		}
		catch (const std::exception &)
		{ }
	}

	return clone;
}

ptr <pbObject>
pbObject::copy (pbExecutionContext* const executionContext) const
{
	const ptr <pbObject> copy = create (executionContext);

	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		try
		{
			copy -> addProperty (propertyDescriptor .first,
			                     propertyDescriptor .second -> value .copy (executionContext),
			                     propertyDescriptor .second -> flags,
			                     propertyDescriptor .second -> get,
			                     propertyDescriptor .second -> set);
		}
		catch (const std::exception &)
		{ }
	}

	return copy;
}

void
pbObject::addProperty (const size_t id,
                       const var & value,
                       const PropertyFlagsType flags,
                       const ptr <pbFunction> & get,
                       const ptr <pbFunction> & set)
throw (std::invalid_argument)
{
	const auto pair = propertyDescriptors .emplace (id, PropertyDescriptorPtr (new PropertyDescriptor { value, flags, get, set }));

	if (pair .second)
	{
		const auto & propertyDescriptor = pair .first -> second; 

		if (propertyDescriptor -> value .isObject ())
			propertyDescriptor -> value .getObject () .addParent (this);

		propertyDescriptor -> get .addParent (this);
		propertyDescriptor -> set .addParent (this);
	}
	else
		throw std::invalid_argument ("Property already exists.");
}

void
pbObject::updateProperty (const size_t id,
                          const var & value,
                          const PropertyFlagsType flags,
                          const ptr <pbFunction> & get,
                          const ptr <pbFunction> & set)
throw (std::invalid_argument)
{
	try
	{
		const auto & propertyDescriptor = propertyDescriptors .at (id);

		if (propertyDescriptor -> flags & WRITABLE and not (flags & LEAVE_VALUE))
		{
			auto & value_ = propertyDescriptor -> value;

			value_ = value;

			if (value_ .isObject ())
				value_ .getObject () .addParent (this);
		}

		if (propertyDescriptor -> flags & CONFIGURABLE)
		{
			propertyDescriptor -> flags = flags;

			if (get)
				propertyDescriptor -> get = get;

			if (set)
				propertyDescriptor -> set = set;
		}
	}
	catch (const std::out_of_range &)
	{
		addProperty (id, value, flags, get, set);
	}
}

void
pbObject::dispose ()
{
	propertyDescriptors .clear ();

	pbBaseObject::dispose ();
}

pbObject::~pbObject ()
{ }

} // pb
} // titania
