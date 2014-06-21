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

#include "vsObject.h"

#include "../Objects/vsFunction.h"

namespace titania {
namespace pb {

PropertyDescriptor::~PropertyDescriptor ()
{ }

const std::string vsObject::typeName = "Object";

var
vsObject::clone (vsExecutionContext* const executionContext) const
{
	const basic_ptr <vsObject> clone = create (executionContext);

	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		try
		{
			clone -> addProperty (propertyDescriptor .first,
			                      propertyDescriptor .second .value,
			                      propertyDescriptor .second .flags,
			                      propertyDescriptor .second .get,
			                      propertyDescriptor .second .set);
		}
		catch (const std::exception &)
		{ }
	}

	return clone;
}

var
vsObject::copy (vsExecutionContext* const executionContext) const
{
	const basic_ptr <vsObject> copy = create (executionContext);

	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		try
		{
			copy -> addProperty (propertyDescriptor .first,
			                     propertyDescriptor .second .value -> copy (executionContext),
			                     propertyDescriptor .second .flags,
			                     propertyDescriptor .second .get,
			                     propertyDescriptor .second .set);
		}
		catch (const std::exception &)
		{ }
	}

	return copy;
}

void
vsObject::addProperty (const std::string & name,
                       const var & value,
                       const PropertyFlagsType flags,
                       const var & get,
                       const var & set)
throw (std::invalid_argument)
{
	const auto pair = propertyDescriptors .emplace (name, PropertyDescriptor { value ? value : make_var <Undefined> (), flags, get, set });

	if (pair .second)
	{
		pair .first -> second .value .addParent (this);
		pair .first -> second .get   .addParent (this);
		pair .first -> second .set   .addParent (this);
	}
	else
		throw std::invalid_argument ("Property already exists.");
}

void
vsObject::updateProperty (const std::string & name,
                          const var & value,
                          const PropertyFlagsType flags,
                          const var & get,
                          const var & set)
throw (std::invalid_argument)
{
	try
	{
		auto & propertyDescriptor = propertyDescriptors .at (name);

		if (propertyDescriptor .flags & WRITABLE)
		{
			if (value)
				propertyDescriptor .value = value;
		}

		if (propertyDescriptor .flags & CONFIGURABLE)
		{
			if (not (propertyDescriptor .flags & SKIP))
				propertyDescriptor .flags = flags;

			if (get)
				propertyDescriptor .get = get;

			if (set)
				propertyDescriptor .set = set;
		}
	}
	catch (const std::out_of_range &)
	{
		addProperty (name, value, flags, get, set);
	}
}

void
vsObject::dispose ()
{
	propertyDescriptors .clear ();

	vsValue::dispose ();
}

vsObject::~vsObject ()
{ }

} // pb
} // titania
