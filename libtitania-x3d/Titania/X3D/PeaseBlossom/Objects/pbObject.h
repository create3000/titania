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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__

#include "../Bits/pbConstants.h"
#include "../Expressions/pbControlFlowException.h"
#include "../Objects/pbBaseObject.h"
#include "../Primitives/ptr.h"

#include <functional>
#include <map>
#include <memory>

#include <unordered_map>

namespace titania {
namespace pb {

class pbObject;
class pbFunction;

///  @relates pbObject
///  @name Type definitions

///  Type to represent a property descriptor.
class PropertyDescriptor
{
public:

	///  @name Construction

	PropertyDescriptor (pbObject* const,
	                    const var &,
		                 const PropertyFlagsType &,
		                 const ptr <pbFunction> &,
		                 const ptr <pbFunction> &);

	///  @name Member access

	void
	setValue (const var & value_)
	{
		value = value_;
		
		addValue ();
	}

	void
	setValue (var && value_)
	{
		value = std::move (value_);

		addValue ();
	}

	const var &
	getValue () const
	{ return value; }

	void
	setFlags (const PropertyFlagsType & value)
	{ flags = value; }

	const PropertyFlagsType &
	getFlags () const
	{ return flags; }

	void
	setGetter (const ptr <pbFunction> & value)
	{ getter = value; }

	const ptr <pbFunction> &
	getGetter () const
	{ return getter; }

	void
	setSetter (const ptr <pbFunction> & value)
	{ setter = value; }

	const ptr <pbFunction> &
	getSetter () const
	{ return setter; }

	~PropertyDescriptor ();

//private:

	void
	addValue ();

	///  @name Members

	pbObject* const   object;
	var               value;
	PropertyFlagsType flags;
	ptr <pbFunction>  getter;
	ptr <pbFunction>  setter;

};

///  Type to represent a property descriptor pointer.
using PropertyDescriptorPtr = std::shared_ptr <PropertyDescriptor>;

/**
 *  Class to represent a basic object.
 */
class pbObject :
	public pbBaseObject
{
public:

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const override
	{ return typeName; }

	///  @name Member access

	///  Checks wehter this object has a propterty @a name.
	bool
	hasProperty (const std::string & name) const
	noexcept (true)
	{ return hasProperty (getId (name)); }

	var
	setProperty (const std::string & name, var && value)
	throw (std::out_of_range,
	       pbException)
	{ return setProperty (getId (name), std::move (value)); }

	var
	getProperty (const std::string & name) const
	throw (std::out_of_range,
	       pbException)
	{ return getProperty (getId (name)); }

	///  Adds the named property described by the given descriptor to this object.
	void
	addPropertyDescriptor (const std::string & name,
	                       const var & value,
	                       const PropertyFlagsType flags = DEFAULT,
	                       const ptr <pbFunction> & getter = nullptr,
	                       const ptr <pbFunction> & setter = nullptr)
	throw (std::invalid_argument)
	{ addPropertyDescriptor (getId (name), value, flags, getter, setter); }

	///  Updates the named property described by the given descriptor to this object.
	void
	updatePropertyDescriptor (const std::string & name,
	                          const var & value,
	                          const PropertyFlagsType flags = DEFAULT,
	                          const ptr <pbFunction> & getter = nullptr,
	                          const ptr <pbFunction> & setter = nullptr)
	throw (std::invalid_argument)
	{ updatePropertyDescriptor (getId (name), value, flags, getter, setter); }

	///  Removes the property @a name from this object.
	void
	removePropertyDescriptor (const std::string & name)
	noexcept (true)
	{ removePropertyDescriptor (getId (name)); }

	virtual
	var
	getValue () const
	throw (pbException,
	       pbControlFlowException) final override
	{ return const_cast <pbObject*> (this); }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override;

	///  Destructs the pbObject.
	virtual
	~pbObject ();


protected:

	///  @name Friends

	friend class PropertyExpression;
	friend class VariableDeclaration;
	friend class VariableExpression;
	friend class Function;

	///  @name Member types

	using PropertyDescriptorIndex = std::unordered_map <size_t, PropertyDescriptorPtr>;
	using PropertyDescriptorArray = std::vector <std::pair <size_t, PropertyDescriptorPtr>>;

	///  @name Construction

	///  Constructs new pbObject.
	pbObject ();

	///  Creates a deep copy of this object.
	ptr <pbObject>
	copy (pbExecutionContext* executionContext, const ptr <pbObject> & object) const
	throw (pbException,
	       pbControlFlowException);

	///  @name Member access

	///  Checks wehter this object has a propterty @a id.
	bool
	hasProperty (const size_t id) const
	noexcept (true)
	{ return properties .count (id); }

	///  Removes the property @a id from this object.

	var
	setProperty (const size_t, var && value)
	throw (std::out_of_range,
	       pbException);

	var
	getProperty (const size_t) const
	throw (std::out_of_range,
	       pbException);

	void
	addPropertyDescriptor (const size_t id,
	                       const var & value,
	                       const PropertyFlagsType flags = DEFAULT,
	                       const ptr <pbFunction> & getter = nullptr,
	                       const ptr <pbFunction> & setter = nullptr)
	throw (std::invalid_argument);

	///  Updates the named property described by the given descriptor to this object.
	void
	updatePropertyDescriptor (const size_t id,
	                          const var & value,
	                          const PropertyFlagsType flags = DEFAULT,
	                          const ptr <pbFunction> & getter = nullptr,
	                          const ptr <pbFunction> & setter = nullptr)
	throw (std::invalid_argument);

	void
	removePropertyDescriptor (const size_t id)
	noexcept (true);

	///  Returns the property descriptor for a property id on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const size_t id) const
	throw (std::out_of_range);

	virtual
	var
	getDefaultValue (const ValueType preferedType) const
	throw (pbException) final override;

	var
	call (const size_t id, const std::vector <var> & arguments = { }) const
	throw (pbException);


private:

	///  @name Cache opeartions

	void
	addCachedPropertyDescriptor (const size_t, const PropertyDescriptorPtr &)
	noexcept (true);

	void
	removeCachedPropertyDescriptors (const size_t)
	noexcept (true);

	const PropertyDescriptorPtr &
	getCachedPropertyDescriptor (const size_t) const
	throw (std::out_of_range);

	///  @name Static members

	static constexpr size_t CACHE_SIZE = 32;

	static const std::string typeName;

	///  @name Members
	
	PropertyDescriptorIndex properties;
	PropertyDescriptorArray cachedProperties;

};

} // pb
} // titania

#endif
