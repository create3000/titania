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

pbObject::pbObject () :
                pbBaseObject (),
	      propertyDescriptors (),
	cachedPropertyDescriptors (CACHE_SIZE, std::make_pair (-1, PropertyDescriptorPtr ()))
{ }

ptr <pbObject>
pbObject::copy (pbExecutionContext* const executionContext, const ptr <pbObject> & copy) const
throw (pbException,
       pbControlFlowException)
{
	for (const auto & propertyDescriptor : propertyDescriptors)
	{
		copy -> updateProperty (propertyDescriptor .first,
		                        propertyDescriptor .second -> value .copy (executionContext) .getValue (),
		                        propertyDescriptor .second -> flags,
		                        propertyDescriptor .second -> get,
		                        propertyDescriptor .second -> set);
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

		addValue (propertyDescriptor -> value);

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
		const auto & propertyDescriptor = getPropertyDescriptor (id);

		if (not (flags & LEAVE_VALUE))
		{
			auto & value_ = propertyDescriptor -> value;

			value_ = value;

			addValue (value_);
		}

		propertyDescriptor -> flags = flags;

		if (get)
			propertyDescriptor -> get = get;

		if (set)
			propertyDescriptor -> set = set;
	}
	catch (const std::out_of_range &)
	{
		addProperty (id, value, flags, get, set);
	}
}

void
pbObject::removeProperty (const size_t id)
noexcept (true)
{
	removeCachedPropertyDescriptors (id);

	propertyDescriptors .erase (id);
}

const PropertyDescriptorPtr &
pbObject::getPropertyDescriptor (const size_t id) const
throw (std::out_of_range)
{
	try
	{
		return getCachedPropertyDescriptor (id);
	}
	catch (const std::out_of_range &)
	{
		const auto & propertyDescriptor = propertyDescriptors .at (id);
		
		const_cast <pbObject*> (this) -> addCachedPropertyDescriptor (id, propertyDescriptor);

		return propertyDescriptor;
	}
}

void
pbObject::addCachedPropertyDescriptor (const size_t id, const PropertyDescriptorPtr & propertyDescriptor)
noexcept (true)
{
	cachedPropertyDescriptors [id % CACHE_SIZE] = std::make_pair (id, propertyDescriptor);
}

void
pbObject::removeCachedPropertyDescriptors (const size_t id)
noexcept (true)
{
	auto & value = cachedPropertyDescriptors [id % CACHE_SIZE];
	
	if (value .first not_eq id)
		return;

	value .first = -1;
	value .second .reset ();
}

const PropertyDescriptorPtr &
pbObject::getCachedPropertyDescriptor (const size_t id) const
throw (std::out_of_range)
{
	const auto & value = cachedPropertyDescriptors [id % CACHE_SIZE];

	if (value .first == id)
		return value .second;

	throw std::out_of_range ("getCachedPropertyDescriptor");
}

var
pbObject::getDefaultValue (const ValueType preferedType) const
throw (pbException)
{
	// All native ECMAScript objects except Date objects handle the absence of a hint as if the hint Number were given;
	// Date objects handle the absence of a hint as if the hint String were given. Host objects may handle the absence of
	// a hint in some other manner.

	//	if (preferedType == STRING)
	//		return toString () or valueOf () or throw TypeError ();
	//
	//	return valueOf () or toString () or throw TypeError ();

	static const auto toString = getId ("toString");
	static const auto valueOf  = getId ("valueOf");
	
	if (preferedType == STRING)
	{
		try
		{
			return call (toString);
		}
		catch (const std::out_of_range &)
		{
			__LOG__ << std::endl;
		}

		try
		{
			return call (valueOf);
		}
		catch (const std::out_of_range &)
		{
			__LOG__ << std::endl;
		}
	}

	try
	{
		return call (valueOf);
	}
	catch (const std::out_of_range &)
	{
		__LOG__ << std::endl;
	}

	try
	{
		return call (toString);
	}
	catch (const std::out_of_range &)
	{
		__LOG__ << std::endl;
	}

	return this -> toString ();
}

var
pbObject::call (const size_t id, const std::vector <var> & arguments) const
throw (std::exception)
{
	const auto & property = getProperty (id);

	if (property .isObject ())
	{
		const auto function = dynamic_cast <pbFunction*> (property .getObject () .get ());

		if (function)
			return function -> call (const_cast <pbObject*> (this));
	}

	throw std::out_of_range ("call");
}

void
pbObject::dispose ()
{
	propertyDescriptors       .clear ();
	cachedPropertyDescriptors .clear ();

	pbBaseObject::dispose ();
}

pbObject::~pbObject ()
{ }

} // pb
} // titania
