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

#ifndef __TITANIA_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__
#define __TITANIA_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__

#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Base/pbUserData.h"
#include "../Bits/Identifier.h"
#include "../Bits/pbConstants.h"
#include "../Expressions/pbControlFlowException.h"
#include "../Primitives/ptr.h"
#include "../Primitives/var.h"

#include "../Standard/Function.h"
#include "../Standard/Object.h"
#include "../Standard/Array.h"

#include <Titania/String/to_string.h>
#include <functional>
#include <memory>
#include <unordered_map>

namespace titania {
namespace pb {

class pbFunction;
class pbObject;

///  @relates pbObject
///  @name Type definitions

///  Type to represent a property descriptor.
class PropertyDescriptor
{
public:

	///  @name Construction

	PropertyDescriptor (pbChildObject* const,
	                    const Identifier &,
	                    const var &,
	                    const PropertyFlagsType &,
	                    const ptr <pbFunction> &,
	                    const ptr <pbFunction> &);

	PropertyDescriptor (pbChildObject* const,
	                    const Identifier &,
	                    var &&,
	                    const PropertyFlagsType &,
	                    ptr <pbFunction> &&,
	                    ptr <pbFunction> &&);

	///  @name Member access

	const Identifier &
	getIdentifier () const
	{ return identifier; }

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


private:

	void
	addValue ();

	///  @name Members

	pbChildObject* const object;
	const Identifier     identifier;
	var                  value;
	PropertyFlagsType    flags;
	ptr <pbFunction>     getter;
	ptr <pbFunction>     setter;

};

///  Type to represent a property descriptor pointer.
using PropertyDescriptorPtr = std::shared_ptr <PropertyDescriptor>;

///  Type to represent a resolve callback function.
using PropertyGetter = std::function <var (pbObject* const, const Identifier &)>;

///  Type to represent a resolve callback function.
using PropertySetter = std::function <void (pbObject* const, const Identifier &, const var &)>;

///  Type to represent a resolve callback function.
using IndexedGetter = std::function <var (pbObject* const, const uint32_t)>;

///  Type to represent a resolve callback function.
using IndexedSetter = std::function <void (pbObject* const, const uint32_t, const var &)>;

///  Type to represent a resolve callback function.
using ResolveCallback = std::function <bool (pbObject* const, const Identifier &)>;

///  Type to represent a dispose callback function.
using DisposeCallback = std::function <void (pbObject* const)>;

struct Callbacks
{
	PropertyGetter  getter;
	PropertySetter  setter;
	IndexedGetter   indexedGetter;
	IndexedSetter   indexedSetter;
	ResolveCallback resolve;
	DisposeCallback dispose;

};

/**
 *  Class to represent a basic object.
 */
class pbObject :
	virtual public pbChildObject,
	virtual public pbOutputStreamObject,
	virtual public pbUserData
{
public:

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const
	noexcept (true) override
	{ return typeName; }

	virtual
	const std::string &
	getClassName () const
	noexcept (true);

	bool
	isExtensible () const
	noexcept (true)
	{ return extensible; }

	void
	isExtensible (const bool value)
	noexcept (true)
	{ extensible = value; }

	const ptr <pbFunction> &
	getConstructor () const
	noexcept (true)
	{ return constructor; }

	const ptr <pbObject> &
	getProto () const
	throw (std::out_of_range);
	
	void
	setCallbacks (const Callbacks & value)
	noexcept (true)
	{ callbacks = &value; }

	///  @name Property access

	///  Checks wehter this object has a propterty @a id.
	bool
	hasProperty (const Identifier & identifier) const
	noexcept (true)
	{ return properties .count (identifier .getId ()); }

	void
	setProperty (const Identifier & identifier, const var & value)
	throw (std::out_of_range,
	       pbException);

	var
	getProperty (const Identifier & identifier) const
	throw (std::out_of_range,
	       pbException);

	ptr <pbObject>
	getObject (const Identifier & identifier) const
	throw (std::out_of_range,
	       TypeError);

	///  @name Property descriptor access

	///  Adds the named property described by the given descriptor to this object.
	void
	addPropertyDescriptor (const Identifier & identifier,
	                       const var & value,
	                       const PropertyFlagsType flags = DEFAULT,
	                       const ptr <pbFunction> & getter = nullptr,
	                       const ptr <pbFunction> & setter = nullptr)
	throw (std::invalid_argument);

	///  Adds the named property described by the given descriptor to this object.
	void
	addPropertyDescriptor (const Identifier &identifier,
	                       var && value,
	                       const PropertyFlagsType flags = DEFAULT,
	                       ptr <pbFunction> && getter = nullptr,
	                       ptr <pbFunction> && setter = nullptr)
	throw (std::invalid_argument);

	///  Updates the named property described by the given descriptor to this object.
	void
	updatePropertyDescriptor (const Identifier & identifier,
	                          const var & value,
	                          const PropertyFlagsType flags = DEFAULT,
	                          const ptr <pbFunction> & getter = nullptr,
	                          const ptr <pbFunction> & setter = nullptr)
	noexcept (true);

	void
	removePropertyDescriptor (const Identifier & identifier)
	noexcept (true);

	///  @name Indexed property access

	virtual
	bool
	hasIndexedProperties ()
	noexcept (true)
	{ return callbacks -> indexedGetter and callbacks -> indexedSetter; }

	virtual
	void
	setIndexedProperty (const uint32_t index, const var & value)
	throw (pbException);

	virtual
	var
	getIndexedProperty (const uint32_t index) const
	throw (std::out_of_range,
	       pbException);

	///  @name Operations

	virtual
	bool
	hasInstance (const var & value)
	throw (TypeError)
	{ throw TypeError ("pbObject::hasInstance"); }

	var
	getDefaultValue (const ValueType) const
	throw (pbException);

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream &) const override;

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

	friend class ArrayIndexExpression;
	friend class PropertyExpression;

	friend
	ptr <NativeFunction>
	Standard::Function::initialize (pbExecutionContext* const);

	friend
	ptr <NativeFunction>
	Standard::Object::initialize (pbExecutionContext* const, const ptr <NativeFunction> &);

	friend
	ptr <NativeFunction>
	Standard::Array::initialize (pbExecutionContext* const);

	///  @name Construction

	///  Constructs new pbObject.
	pbObject ();

	///  @name Member access

	void
	setConstructor (const ptr <pbFunction> & value)
	{ constructor = value; }

	void
	setProto (const ptr <pbObject> & value)
	throw (std::invalid_argument);

	///  @name Operations
	
	virtual
	bool
	resolve (const Identifier & identifier);

	var
	apply (const Identifier & identifier, const var & object, const std::vector <var> & args = { }) const
	throw (pbException,
	       std::invalid_argument);

	virtual
	var
	apply (const uint32_t index, const var & object, const std::vector <var> & args = { }) const
	throw (pbException,
	       std::invalid_argument)
	{ return apply (basic::to_string (index), object, args); }


private:

	///  @name Member types

	using PropertyDescriptorIndex = std::unordered_map <size_t, PropertyDescriptorPtr>;
	using PropertyDescriptorArray = std::vector <std::pair <size_t, PropertyDescriptorPtr>>;

	///  @name Cache operations

	///  Returns the property descriptor for a property id on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const Identifier &, const bool) const
	throw (std::out_of_range);

	///  Returns the property descriptor for a property id on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const size_t) const
	throw (std::out_of_range);

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

	static constexpr size_t CACHE_SIZE = 16;

	static const std::string typeName;
	static const Callbacks   defaultCallbacks;

	///  @name Members

	bool                    extensible;
	ptr <pbFunction>        constructor;
	ptr <pbObject>          proto;
	PropertyDescriptorIndex properties;
	PropertyDescriptorArray cachedProperties;
	const Callbacks*        callbacks;

};

} // pb
} // titania

#endif
