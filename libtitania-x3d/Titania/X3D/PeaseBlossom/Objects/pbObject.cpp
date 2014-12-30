/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

PropertyDescriptor::PropertyDescriptor (pbChildObject* const object,
                                        const std::string & identifier,
                                        const var & value_,
                                        const PropertyFlagsType & flags,
                                        const ptr <pbFunction> & getter_,
                                        const ptr <pbFunction> & setter_) :
	    object (object),
	identifier (identifier),
	     value (value_),
	     flags (flags),
	    getter (getter_),
	    setter (setter_)
{
	addValue ();

	getter .addParent (object);
	setter .addParent (object);
}

void
PropertyDescriptor::addValue ()
{
	if (value .isObject ())
		const_cast <ptr <pbObject> &> (value .getObject ()) .addParent (object);
}

PropertyDescriptor::~PropertyDescriptor ()
{ }

const std::string pbObject::typeName = "Object";

pbObject::pbObject () :
	       pbChildObject (),
	pbOutputStreamObject (),
	          properties (),
	    cachedProperties (CACHE_SIZE, std::make_pair (-1, PropertyDescriptorPtr ()))
{ }

ptr <pbObject>
pbObject::copy (pbExecutionContext* const executionContext, const ptr <pbObject> & copy) const
noexcept (true)
{
	static const auto protoId = getId ("__proto__");

	for (const auto & property : properties)
	{
		if (property .first == protoId)
			continue;

		copy -> updatePropertyDescriptor (property .first,
		                                  property .second -> getIdentifier (),
		                                  property .second -> getValue () .copy (executionContext),
		                                  property .second -> getFlags (),
		                                  property .second -> getGetter (),
		                                  property .second -> getSetter ());
	}

	return copy;
}

var
pbObject::setProperty (const size_t id, var && value)
throw (std::out_of_range,
       pbException)
{
	const auto & property = getPropertyDescriptor (id);

	if (property -> getFlags () & WRITABLE)
	{
		if (property -> getSetter ())
			return property -> getSetter () -> call (this, { std::move (value) });

		property -> setValue (std::move (value));

		return property -> getValue ();
	}

	return Undefined ();
}

var
pbObject::getProperty (const size_t id) const
throw (std::out_of_range,
       pbException)
{
	const auto & property = getPropertyDescriptor (id);

	if (property -> getGetter ())
		return property -> getGetter () -> call (const_cast <pbObject*> (this));

	return property -> getValue ();
}

void
pbObject::addPropertyDescriptor (const size_t id,
                                 const std::string & identifier,
                                 const var & value,
                                 const PropertyFlagsType flags,
                                 const ptr <pbFunction> & getter,
                                 const ptr <pbFunction> & setter)
throw (std::invalid_argument)
{
	const auto pair = properties .emplace (id, std::make_shared <PropertyDescriptor> (this, identifier, value, flags, getter, setter));

	if (not pair .second)
		throw std::invalid_argument ("Property already exists.");
}

void
pbObject::updatePropertyDescriptor (const size_t id,
                                    const std::string & identifier,
                                    const var & value,
                                    const PropertyFlagsType flags,
                                    const ptr <pbFunction> & getter,
                                    const ptr <pbFunction> & setter)
throw (std::invalid_argument)
{
	try
	{
		const auto & property = getPropertyDescriptor (id);

		if (not (flags & LEAVE_VALUE))
			property -> setValue (value);

		property -> setFlags (flags);

		if (getter)
			property -> setGetter (getter);

		if (setter)
			property -> setSetter (setter);
	}
	catch (const std::out_of_range &)
	{
		addPropertyDescriptor (id, identifier, value, flags, getter, setter);
	}
}

void
pbObject::removePropertyDescriptor (const size_t id)
noexcept (true)
{
	removeCachedPropertyDescriptors (id);

	properties .erase (id);
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
		try
		{
			const auto & property = properties .at (id);

			const_cast <pbObject*> (this) -> addCachedPropertyDescriptor (id, property);

			return property;
		}
		catch (const std::out_of_range &)
		{
			static const auto protoId = getId ("__proto__");

			const auto & property = properties .at (protoId);
			const auto & value    = property -> getValue ();

			if (value .isObject ())
			{
				const auto & proto         = value .getObject ();
				const auto & protoProperty = proto -> getPropertyDescriptor (id);

				const_cast <pbObject*> (this) -> addPropertyDescriptor (id,
				                                                        protoProperty -> getIdentifier (),
		                                                              protoProperty -> getValue (),
		                                                              protoProperty -> getFlags (),
		                                                              protoProperty -> getGetter (),
		                                                              protoProperty -> getSetter ());

				return getPropertyDescriptor (id);
			}
			
			throw;
		}
	}
}

void
pbObject::addCachedPropertyDescriptor (const size_t id, const PropertyDescriptorPtr & property)
noexcept (true)
{
	cachedProperties [id % CACHE_SIZE] = std::make_pair (id, property);
}

void
pbObject::removeCachedPropertyDescriptors (const size_t id)
noexcept (true)
{
	auto & value = cachedProperties [id % CACHE_SIZE];

	if (value .first not_eq id)
		return;

	value .first = -1;
	value .second .reset ();
}

const PropertyDescriptorPtr &
pbObject::getCachedPropertyDescriptor (const size_t id) const
throw (std::out_of_range)
{
	const auto & value = cachedProperties [id % CACHE_SIZE];

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
		catch (const TypeError &)
		{ }

		try
		{
			return call (valueOf);
		}
		catch (const TypeError &)
		{ }
	}

	try
	{
		return call (valueOf);
	}
	catch (const TypeError &)
	{ }

	try
	{
		return call (toString);
	}
	catch (const TypeError &)
	{ }

	return this -> toString ();
}

var
pbObject::call (const size_t id, const std::vector <var> & arguments) const
throw (pbException)
{
	try
	{
		const auto & property = getProperty (id);

		if (property .isObject ())
		{
			const auto function = dynamic_cast <pbFunction*> (property .getObject () .get ());

			if (function)
				return function -> call (const_cast <pbObject*> (this), arguments);
		}

		throw TypeError ("'" + property .toString () + "' is not a function.");
	}
	catch (const std::out_of_range &)
	{
		throw TypeError ("'undefined' is not a function.");
	}
}

void
pbObject::dispose ()
{
	properties       .clear ();
	cachedProperties .clear ();

	pbChildObject::dispose ();
}

pbObject::~pbObject ()
{ }

} // pb
} // titania
