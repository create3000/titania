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
#include "../Primitives/ptr.h"
#include "../Primitives/var.h"

#include "../Standard/Function.h"
#include "../Standard/Object.h"
#include "../Standard/Array.h"

#include <Titania/String/to_string.h>
#include <functional>
#include <memory>
#include <map>
#include <chrono>

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

	///  @name Member type
	
	using clock = std::chrono::high_resolution_clock;

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

	uint32_t
	getIndex () const
	{ return flags & PROPERTY; }

	bool
	isDefined () const
	{ return isDataDescriptor () or isAccessorDescriptor (); }

	bool
	isAccessorDescriptor () const
	{ return getter or setter; }
	
	bool
	isDataDescriptor () const
	{ return not value .isundefined () or isWritable (); }

	bool
	isGenericDescriptor () const
	{ return not (isAccessorDescriptor () or isDataDescriptor ()); }

	void
	isWritable (const bool value)
	{
		if (value)
			flags |= WRITABLE;
		else
			flags &= ~WRITABLE;
	}

	bool
	isWritable () const
	{ return flags & WRITABLE; }

	void
	isConfigurable (const bool value)
	{
		if (value)
			flags |= CONFIGURABLE;
		else
			flags &= ~CONFIGURABLE;
	}

	bool
	isConfigurable () const
	{ return flags & CONFIGURABLE; }

	void
	isEnumerable (const bool value)
	{
		if (value)
			flags |= ENUMERABLE;
		else
			flags &= ~ENUMERABLE;
	}

	bool
	isEnumerable () const
	{ return flags & ENUMERABLE; }

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

	const clock::duration &
	getCreationTime () const
	{ return creationTime; }

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
	clock::duration      creationTime;

};

///  Type to represent a property descriptor pointer.
using PropertyDescriptorPtr = std::shared_ptr <PropertyDescriptor>;

///  Type to represent a enumerate callback function.
using EnumerateCallback = std::function <bool (pbObject* const, const EnumerateType, std::string &, void* &)>;

///  Type to represent a hasProperty callback function.
using HasPropertyCallback = std::function <bool (pbObject* const, const Identifier &)>;

///  Type to represent a property getter callback function.
using PropertyGetter = std::function <var (pbObject* const, const Identifier &)>;

///  Type to represent a property setter callback function.
using PropertySetter = std::function <void (pbObject* const, const Identifier &, const var &)>;

///  Type to represent a resolve callback function.
using ResolveCallback = std::function <bool (pbObject* const, const Identifier &)>;

///  Type to represent a dispose callback function.
using DisposeCallback = std::function <void (pbObject* const)>;

struct Callbacks
{
	EnumerateCallback   enumerate;
	HasPropertyCallback hasProperty;
	PropertyGetter      getter;
	PropertySetter      setter;
	ResolveCallback     resolve;
	DisposeCallback     dispose;

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

	///  Checks whether this object has a property @a identifier.
	bool
	hasProperty (const Identifier & identifier) const
	noexcept (true);

	///  Sets the value of the property for @a identifier.
	virtual
	void
	put (const Identifier & identifier, const var & value, const bool throw_)
	throw (pbError,
	       std::invalid_argument)
	{ put (identifier, value, NONE, throw_); }

	///  Returns the value of the property for @a identifier.
	var
	get (const Identifier & identifier) const
	throw (pbError,
	       std::out_of_range);

	ptr <pbObject>
	getObject (const Identifier & identifier) const
	throw (pbError,
	       std::out_of_range);

	///  Returns the property descriptor for @a identifier.
	const PropertyDescriptorPtr &
	getProperty (const Identifier & identifier) const
	throw (std::out_of_range);

	///  Returns the own property descriptor for @a identifier.
	const PropertyDescriptorPtr &
	getOwnProperty (const Identifier & identifier) const
	throw (std::out_of_range)
	{ return properties .at (identifier .getId ()); }

	///  Adds the named property described by the given descriptor for this object.
	virtual
	void
	addOwnProperty (const Identifier & identifier,
	                const var & value,
	                const PropertyFlagsType flags = DEFAULT,
	                const ptr <pbFunction> & getter = nullptr,
	                const ptr <pbFunction> & setter = nullptr,
	                const bool throw_ = true)
	throw (TypeError,
	       std::invalid_argument);

	///  Adds the named property described by the given descriptor for this object.
	virtual
	void
	addOwnProperty (const Identifier & identifier,
	                var && value,
	                const PropertyFlagsType flags = DEFAULT,
	                ptr <pbFunction> && getter = nullptr,
	                ptr <pbFunction> && setter = nullptr,
	                const bool throw_ = true)
	throw (TypeError,
	       std::invalid_argument);

	///  Updates the named property described by the given descriptor for this object.
	virtual
	void
	defineOwnProperty (const Identifier & identifier,
	                   const var & value,
	                   const PropertyFlagsType flags = DEFAULT,
	                   const ptr <pbFunction> & getter = nullptr,
	                   const ptr <pbFunction> & setter = nullptr,
	                   const bool throw_ = true)
	throw (TypeError);

	void
	deleteProperty (const Identifier & identifier)
	noexcept (true);

	///  @name Operations

	virtual
	bool
	hasInstance (const var & value)
	throw (TypeError)
	{ throw TypeError ("pbObject::hasInstance"); }

	var
	getDefaultValue (const ValueType) const
	throw (pbError);

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
	friend class ForInStatement;
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

	bool
	enumerate (const EnumerateType type, std::string & identifier, std::vector <void*> & userData)
	noexcept (true);

	///  Sets the value of the property for @a identifier.
	void
	put (const Identifier & identifier, const var & value, const PropertyFlagsType flags, const bool throw_)
	throw (pbError,
	       std::invalid_argument);

	virtual
	bool
	resolve (const Identifier & identifier)
	throw (pbError);

	var
	call (const Identifier & identifier, const var & object, const std::vector <var> & args = { }) const
	throw (pbError,
	       std::invalid_argument);


private:

	///  @name Member access

	std::vector <PropertyDescriptorPtr>
	getOwnEnumerableProperties () const;

	///  @name Member types

	using PropertyDescriptorIndex = std::map <size_t, PropertyDescriptorPtr>;

	///  @name Static members

	static const std::string typeName;
	static const Callbacks   defaultCallbacks;

	///  @name Members

	bool                    extensible;
	ptr <pbFunction>        constructor;
	ptr <pbObject>          proto;
	PropertyDescriptorIndex properties;
	const Callbacks*        callbacks;

};

} // pb
} // titania

#endif
