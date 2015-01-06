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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_PB_OBJECT_H__

#include "../Base/pbUserData.h"
#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Bits/Identifier.h"
#include "../Bits/pbConstants.h"
#include "../Expressions/pbControlFlowException.h"
#include "../Primitives/ptr.h"
#include "../Primitives/var.h"

#include "../Standard/Function.h"
#include "../Standard/Object.h"

#include <functional>
#include <map>
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
using ResolveType = std::function <bool (pbObject* const, const Identifier &)>;

///  Type to represent a dispose callback function.
using DisposeType = std::function <void (pbObject* const)>;

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
	{ return extensible; }

	void
	isExtensible (const bool value)
	{ extensible = value; }

	const ptr <pbFunction> &
	getConstructor () const
	{ return constructor; }

	const ptr <pbObject> &
	getProto () const
	throw (std::out_of_range);

	///  @name Property access

	///  Checks wehter this object has a propterty @a id.
	bool
	hasProperty (const Identifier & identifier) const
	noexcept (true)
	{ return properties .count (identifier .getId ()); }

	void
	setProperty (const Identifier &, const var &)
	throw (std::out_of_range,
	       pbException);

	var
	getProperty (const Identifier &) const
	throw (std::out_of_range,
	       pbException);

	ptr <pbObject>
	getObject (const Identifier &) const
	throw (std::out_of_range,
	       TypeError);

	///  Adds the named property described by the given descriptor to this object.
	void
	addPropertyDescriptor (const Identifier &,
	                       const var &,
	                       const PropertyFlagsType = DEFAULT,
	                       const ptr <pbFunction> & = nullptr,
	                       const ptr <pbFunction> & = nullptr)
	throw (std::invalid_argument);

	///  Adds the named property described by the given descriptor to this object.
	void
	addPropertyDescriptor (const Identifier &,
	                       var &&,
	                       const PropertyFlagsType = DEFAULT,
	                       ptr <pbFunction> && = nullptr,
	                       ptr <pbFunction> && = nullptr)
	throw (std::invalid_argument);

	///  Updates the named property described by the given descriptor to this object.
	void
	updatePropertyDescriptor (const Identifier &,
	                          const var &,
	                          const PropertyFlagsType = DEFAULT,
	                          const ptr <pbFunction> & = nullptr,
	                          const ptr <pbFunction> & = nullptr)
	noexcept (true);

	void
	removePropertyDescriptor (const Identifier &)
	noexcept (true);

	void
	setResolveCallback (const ResolveType & value)
	{ resolveFunction = value; }
	
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

	void
	setDisposeCallback (const DisposeType & value)
	{ disposeFunction = value; }

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
	
	friend
	ptr <NativeFunction>
	Standard::Function::initialize (pbExecutionContext* const);
	
	friend
	ptr <NativeFunction>
	Standard::Object::initialize (pbExecutionContext* const, const ptr <NativeFunction> &);

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

	var
	apply (const Identifier &, const var &, const std::vector <var> & = { }) const
	throw (pbException,
	       std::invalid_argument);


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

	///  @name Members

	bool                    extensible;
	ptr <pbFunction>        constructor;
	ptr <pbObject>          proto;
	PropertyDescriptorIndex properties;
	PropertyDescriptorArray cachedProperties;
	ResolveType             resolveFunction;
	DisposeType             disposeFunction;

};

} // pb
} // titania

#endif
