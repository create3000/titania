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
	                    const AttributeType &,
	                    const ptr <pbFunction> &,
	                    const ptr <pbFunction> &);

	PropertyDescriptor (pbChildObject* const,
	                    const Identifier &,
	                    var &&,
	                    const AttributeType &,
	                    ptr <pbFunction> &&,
	                    ptr <pbFunction> &&);

	///  @name Member access

	const Identifier &
	getIdentifier () const
	{ return identifier; }

	uint32_t
	getIndex () const
	{ return attributes & PROPERTY; }

	bool
	isDefined () const
	{ return isDataDescriptor () or isAccessorDescriptor (); }

	bool
	isAccessorDescriptor () const
	{ return getter or setter; }
	
	bool
	isDataDescriptor () const
	{ return not value .isUndefined () or isWritable (); }

	bool
	isGenericDescriptor () const
	{ return not (isAccessorDescriptor () or isDataDescriptor ()); }

	void
	isWritable (const bool value)
	{
		if (value)
			attributes |= WRITABLE;
		else
			attributes &= ~WRITABLE;
	}

	bool
	isWritable () const
	{ return attributes & WRITABLE; }

	void
	isConfigurable (const bool value)
	{
		if (value)
			attributes |= CONFIGURABLE;
		else
			attributes &= ~CONFIGURABLE;
	}

	bool
	isConfigurable () const
	{ return attributes & CONFIGURABLE; }

	void
	isEnumerable (const bool value)
	{
		if (value)
			attributes |= ENUMERABLE;
		else
			attributes &= ~ENUMERABLE;
	}

	bool
	isEnumerable () const
	{ return attributes & ENUMERABLE; }

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
	setFlags (const AttributeType & value)
	{ attributes = value; }

	const AttributeType &
	getFlags () const
	{ return attributes; }

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

	pbChildObject* const  object;
	const Identifier      identifier;
	var                   value;
	AttributeType         attributes;
	ptr <pbFunction>      getter;
	ptr <pbFunction>      setter;
	const clock::duration creationTime;

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

extern const ptr <pbFunction> DefaultGetter;
extern const ptr <pbFunction> DefaultSetter;

/**
 *  Class to represent a basic object.
 */
class pbObject :
	virtual public pbChildObject,
	virtual public pbUserData,
	virtual public pbOutputStreamObject
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
	noexcept (true)
	{ return proto; }

	void
	setCallbacks (const Callbacks & value)
	noexcept (true)
	{ callbacks = &value; }

	const Callbacks &
	getCallbacks () const
	noexcept (true)
	{ return *callbacks; }

	///  @name Property access

	///  Checks whether this object has a property @a identifier.
	bool
	hasProperty (const Identifier & identifier) const
	noexcept (true);

	///  throws pbError, std::out_of_range, std::invalid_argument
	var
	call (const Identifier & identifier, const std::vector <var> & args = { }) const;

	///  Sets the value of the property for @a identifier.
	///  throws pbError, std::out_of_range
	virtual
	void
	put (const Identifier & identifier, const var & value, const bool throw_ = false)
	{ return put (identifier, value, NONE, throw_); }

	///  Returns the value of the property for @a identifier.
	///  throws pbError, std::out_of_range
	virtual
	var
	get (const Identifier & identifier) const;

	///  Returns the property descriptor for @a identifier.
	///  throws std::out_of_range
	const PropertyDescriptorPtr &
	getProperty (const Identifier & identifier) const;

	///  Returns the own property descriptor for @a identifier.
	///  throws std::out_of_range
	const PropertyDescriptorPtr &
	getOwnProperty (const Identifier & identifier) const
	{ return properties .at (identifier .getId ()); }

	///  Adds the named property described by the given descriptor for this object.
	///  throws TypeError, std::invalid_argument
	void
	addOwnProperty (const Identifier & identifier,
	                const var & value,
	                const AttributeType attributes = DEFAULT,
	                const ptr <pbFunction> & getter = DefaultGetter,
	                const ptr <pbFunction> & setter = DefaultSetter,
	                const bool throw_ = false);

	///  Adds the named property described by the given descriptor for this object.
	///  throws TypeError, std::invalid_argument
	virtual
	void
	addOwnProperty (const Identifier & identifier,
	                var && value,
	                const AttributeType attributes = DEFAULT,
	                ptr <pbFunction> && getter = nullptr,
	                ptr <pbFunction> && setter = nullptr,
	                const bool throw_ = false);

	///  Updates the named property described by the given descriptor for this object.
	///  throws TypeError
	virtual
	void
	defineOwnProperty (const Identifier & identifier,
	                   const var & value,
	                   const AttributeType attributes = DEFAULT,
	                   const ptr <pbFunction> & getter = DefaultGetter,
	                   const ptr <pbFunction> & setter = DefaultSetter,
	                   const bool throw_ = false);

	///  throws TypeError, std::out_of_range
	bool
	deleteOwnProperty (const Identifier & identifier, const bool throw_ = false);

	///  @name Operations

	///  throws TypeError
	virtual
	bool
	hasInstance (const var & value)
	{ throw TypeError ("pbObject::hasInstance"); }

	///  throws pbError
	var
	getDefaultValue (const ValueType preferedType) const;

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

	friend class ForInStatement;
	friend class ForVarInStatement;

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
	
	void
	prepare ();

	///  @name Member access

	void
	setConstructor (pbFunction* const value)
	{ constructor = value; }

	///  throws std::invalid_argument
	void
	setProto (pbObject* const value);

	///  @name Operations

	bool
	enumerate (const EnumerateType type, std::string & identifier, std::vector <void*> & userData)
	noexcept (true);

	///  Sets the value of the property for @a identifier.
	///  throws pbError, std::out_of_range
	void
	put (const Identifier & identifier, const var & value, const AttributeType attributes, const bool throw_);

	///  throws pbError
	virtual
	bool
	resolve (const Identifier & identifier);


private:

	///  @name Member access

	std::vector <PropertyDescriptorPtr>
	getOwnEnumerableProperties () const
	noexcept (true);

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
