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

#include <algorithm>

namespace titania {
namespace pb {

template <class Type, class ... Args>
std::unique_ptr <Type>
make_unique (Args && ... args)
{
	return std::unique_ptr <Type> (new Type (std::forward <Args> (args) ...));
}

PropertyDescriptor::PropertyDescriptor (pbChildObject* const object,
                                        const Identifier & identifier,
                                        const var & value_,
                                        const PropertyFlagsType & flags,
                                        const ptr <pbFunction> & getter_,
                                        const ptr <pbFunction> & setter_) :
	      object (object),
	  identifier (identifier),
	       value (value_),
	       flags (flags),
	      getter (getter_),
	      setter (setter_),
	creationTime (clock::now () .time_since_epoch () .count ())
{
	addValue ();

	getter .addParent (object);
	setter .addParent (object);
}

PropertyDescriptor::PropertyDescriptor (pbChildObject* const object,
                                        const Identifier & identifier,
                                        var && value_,
                                        const PropertyFlagsType & flags,
                                        ptr <pbFunction> && getter_,
                                        ptr <pbFunction> && setter_) :
	      object (object),
	  identifier (identifier),
	       value (std::move (value_)),
	       flags (flags),
	      getter (std::move (getter_)),
	      setter (std::move (setter_)),
	creationTime (clock::now () .time_since_epoch () .count ())
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
const Callbacks   pbObject::defaultCallbacks;

pbObject::pbObject () :
	       pbChildObject (),
	pbOutputStreamObject (),
	          pbUserData (),
	          extensible (true),
	         constructor (),
	               proto (),
	          properties (),
	           callbacks (&defaultCallbacks)
{
	addChildren (constructor, proto);
}

const std::string &
pbObject::getClassName () const
noexcept (true)
{
	return constructor -> getTypeName ();
}

void
pbObject::setProto (const ptr <pbObject> & value)
throw (std::invalid_argument)
{
	proto = value;

	if (proto)
		addOwnProperty ("__proto__", proto, NONE);

	else
		addOwnProperty ("__proto__", nullptr, NONE);
}

const ptr <pbObject> &
pbObject::getProto () const
throw (std::out_of_range)
{
	if (proto)
		return proto;

	throw std::out_of_range ("pbObject::getProto");
}

bool
pbObject::enumerate (const EnumerateType type, std::string & propertyName, std::vector <void*> & userData)
noexcept (true)
{
	struct Data
	{
		bool user;
		ptr <pbObject> object;
		size_t index;
		std::vector <PropertyDescriptorPtr> properties;
		std::set <size_t> enumerated;
	};

	switch (type)
	{
		case ENUMERATE_BEGIN:
		{
			userData .resize (2);
		
			const auto data = new Data ();

			data -> user       = callbacks -> enumerate and callbacks -> enumerate (this, type, propertyName, userData [0]);
			data -> object     = this;
			data -> index      = 0;
			data -> properties = getOwnEnumerableProperties ();

			userData [1] = data;
			return true;
		}
		case ENUMERATE:
		{
			const auto data = static_cast <Data*> (userData [1]);

			if (data -> user)
			{
				if (callbacks -> enumerate (this, type, propertyName, userData [0]))
				{
					data -> enumerated .emplace (Identifier::hash (propertyName));
					return true;
				}
			}

			try
			{
				for (;;)
				{
					while (data -> index < data -> properties .size ())
					{
						const auto & identifier = data -> properties [data -> index] -> getIdentifier ();

						if (data -> object -> properties .count (identifier .getId ()))
						{
							if (data -> enumerated .emplace (identifier .getId ()) .second)
							{
								data -> index += 1;
								propertyName   = identifier .getName ();
								return true;
							}
						}

						data -> index += 1;
					}

					data -> object     = data -> object -> getProto ();
					data -> index      = 0;
					data -> properties = data -> object -> getOwnEnumerableProperties ();
				}
			}
			catch (const std::out_of_range &)
			{ }

			return false;
		}
		case ENUMERATE_END:
		{
			const auto & data = static_cast <Data*> (userData [1]);

			if (data -> user)
			{
				data -> user = false;
				callbacks -> enumerate (this, type, propertyName, userData [0]);
				return true;
			}
	
			delete data;
			return false;
		}
	}

	return false;
}

std::vector <PropertyDescriptorPtr>
pbObject::getOwnEnumerableProperties () const
{
	struct Comp
	{
		bool
		operator () (const PropertyDescriptorPtr & lhs, const PropertyDescriptorPtr & rhs) const
		{
			return std::make_pair (lhs -> getIndex (), lhs -> getCreationTime ()) < std::make_pair (rhs -> getIndex (), rhs -> getCreationTime ());
		}
	};

	static constexpr auto comp = Comp { };

	std::vector <PropertyDescriptorPtr> sorted;

	for (const auto & property : properties)
	{
		if (property .second -> isEnumerable ())
			sorted .emplace_back (property .second);
	}

	std::sort (sorted .begin (), sorted .end (), comp);

	return sorted;
}

bool
pbObject::hasProperty (const Identifier & identifier) const
noexcept (true)
{
	try
	{
		if (callbacks -> hasProperty)
		{
			if (callbacks -> hasProperty (const_cast <pbObject*> (this), identifier))
				return true;
		}
	}
	catch (const std::out_of_range &)
	{ }

	if (properties .count (identifier .getId ()))
		return true;

	try
	{
		return getProto () -> hasProperty (identifier);
	}
	catch (const std::out_of_range &)
	{
		return false;
	}
}

void
pbObject::put (const Identifier & identifier, const var & value, const PropertyFlagsType flags, const bool throw_)
throw (pbError,
       std::invalid_argument)
{
	try
	{
		if (callbacks -> setter)
			return callbacks -> setter (this, identifier, value);
	}
	catch (const std::out_of_range &)
	{ }

	try
	{
		const auto & ownDescriptor = getOwnProperty (identifier);

		if (ownDescriptor -> isWritable ())
			return ownDescriptor -> setValue (value);

		if (ownDescriptor -> getSetter ()) // isAccessorDescriptor
			ownDescriptor -> getSetter () -> call (this, { value });
	}
	catch (const std::out_of_range &)
	{
		try
		{
			const auto & descriptor = getProto () -> getProperty (identifier);

			if (descriptor -> getSetter ()) // isAccessorDescriptor
				descriptor -> getSetter () -> call (this, { value });
		}
		catch (const std::out_of_range &)
		{
			if (throw_)
				throw std::invalid_argument ("pbObject::put");

			pbObject::addOwnProperty (identifier, value, flags | (WRITABLE | CONFIGURABLE | ENUMERABLE), nullptr, nullptr, throw_);
		}
	}
}

var
pbObject::get (const Identifier & identifier) const
throw (pbError,
       std::out_of_range)
{
	try
	{
		if (callbacks -> getter)
			return callbacks -> getter (const_cast <pbObject*> (this), identifier);
	}
	catch (const std::out_of_range &)
	{ }

	const auto & descriptor = getProperty (identifier);

	if (descriptor -> isDataDescriptor ())
		return descriptor -> getValue ();

	if (descriptor -> getGetter ()) // isAccessorDescriptor
		return descriptor -> getGetter () -> call (const_cast <pbObject*> (this));

	return undefined;
}

ptr <pbObject>
pbObject::getObject (const Identifier & identifier) const
throw (pbError,
       std::out_of_range)
{
	const auto value = get (identifier);

	if (value .isObject ())
		return value .getObject ();

	throw TypeError ("Property '" + identifier .getName () + "' is not an object.");
}

const PropertyDescriptorPtr &
pbObject::getProperty (const Identifier & identifier) const
throw (std::out_of_range)
{
	try
	{
		return getOwnProperty (identifier);
	}
	catch (const std::out_of_range &)
	{
		try
		{
			return getProto () -> getProperty (identifier);
		}
		catch (const std::out_of_range &)
		{
			if (const_cast <pbObject*> (this) -> resolve (identifier))
				return getOwnProperty (identifier);

			throw;
		}
	}
}

bool
pbObject::resolve (const Identifier & identifier)
throw (pbError)
{
	return callbacks -> resolve and callbacks -> resolve (this, identifier);
}

void
pbObject::addOwnProperty (const Identifier & identifier,
                          const var & value,
                          const PropertyFlagsType flags,
                          const ptr <pbFunction> & getter,
                          const ptr <pbFunction> & setter,
                          const bool throw_)
throw (TypeError,
       std::invalid_argument)
{
	if (not isExtensible ())
	{
		if (throw_)
			throw TypeError ("Object is not extensible.");

		return;
	}

	if ((not value .isundefined () or flags & WRITABLE) and (getter or setter))
	{
		if (throw_)
			throw TypeError ("A property cannot both have accessors and be writable or have a value.");

		return;
	}

	if (properties .emplace (identifier .getId (), make_unique <PropertyDescriptor> (this, identifier, value, flags, getter, setter)) .second)
		return;

	throw std::invalid_argument ("Couldn't add property.");
}

void
pbObject::addOwnProperty (const Identifier & identifier,
                          var && value,
                          const PropertyFlagsType flags,
                          ptr <pbFunction> && getter,
                          ptr <pbFunction> && setter,
                          const bool throw_)
throw (TypeError,
       std::invalid_argument)
{
	if (not isExtensible ())
	{
		if (throw_)
			throw TypeError ("Object is not extensible.");

		return;
	}

	if ((not value .isundefined () or flags & WRITABLE) and (getter or setter))
	{
		if (throw_)
			throw TypeError ("A property cannot both have accessors and be writable or have a value.");

		return;
	}

	if (properties .emplace (identifier .getId (), make_unique <PropertyDescriptor> (this, identifier, std::move (value), flags, std::move (getter), std::move (setter))) .second)
		return;

	throw std::invalid_argument ("Couldn't add property.");
}

void
pbObject::defineOwnProperty (const Identifier & identifier,
                             const var & value,
                             const PropertyFlagsType flags,
                             const ptr <pbFunction> & getter,
                             const ptr <pbFunction> & setter,
                             const bool throw_)
throw (TypeError)
{
	try
	{
		const auto & descriptor = getOwnProperty (identifier);

		if (not descriptor -> isConfigurable ())
		{
			if (throw_)
				throw TypeError ("Property named '" + identifier .getName () + "' is not configurable.");

			return;
		}

		if ((not value .isundefined () or flags & WRITABLE) and (getter or setter))
		{
			if (throw_)
				throw TypeError ("A property cannot both have accessors and be writable or have a value.");

			return;
		}

		descriptor -> setValue (value);
		descriptor -> setFlags (flags);
		descriptor -> setGetter (getter);
		descriptor -> setSetter (setter);
	}
	catch (const std::out_of_range &)
	{
		pbObject::addOwnProperty (identifier, value, flags, getter, setter, throw_);
	}
}

void
pbObject::deleteProperty (const Identifier & identifier)
noexcept (true)
{
	properties .erase (identifier .getId ());
}

var
pbObject::getDefaultValue (const ValueType preferedType) const
throw (pbError)
{
	// All native ECMAScript objects except Date objects handle the absence of a hint as if the hint Number were given;
	// Date objects handle the absence of a hint as if the hint String were given. Host objects may handle the absence of
	// a hint in some other manner.

	//	if (preferedType == STRING)
	//		return toString () or valueOf () or throw TypeError ();
	//
	//	return valueOf () or toString () or throw TypeError ();

	static const Identifier toString = "toString";
	static const Identifier valueOf  = "valueOf";

	if (preferedType == STRING)
	{
		try
		{
			return call (toString, this);
		}
		catch (const std::invalid_argument &)
		{ }

		try
		{
			return call (valueOf, this);
		}
		catch (const std::invalid_argument &)
		{ }
	}

	try
	{
		return call (valueOf, this);
	}
	catch (const std::invalid_argument &)
	{ }

	try
	{
		return call (toString, this);
	}
	catch (const std::invalid_argument &)
	{ }

	throw TypeError ("can't convert object to string.");
}

var
pbObject::call (const Identifier & identifier, const var & object, const std::vector <var> & arguments) const
throw (pbError,
       std::invalid_argument)
{
	try
	{
		const auto property = get (identifier);

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
pbObject::toStream (std::ostream & ostream) const
{
	ostream << "[object " << getClassName () << "]";
}

void
pbObject::dispose ()
{
	if (callbacks -> dispose)
		callbacks -> dispose (this);

	properties .clear ();

	pbChildObject::dispose ();
}

pbObject::~pbObject ()
{ }

} // pb
} // titania
