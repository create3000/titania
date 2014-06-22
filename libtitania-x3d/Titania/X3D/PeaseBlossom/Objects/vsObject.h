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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_VS_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_OBJECTS_VS_OBJECT_H__

#include "../Primitives/vsValue.h"

#include <functional>
#include <map>
#include <memory>

namespace titania {
namespace pb {

class vsObject;
class vsFunction;

///  @relates vsObject
///  @name Type definitions

///  Type to represent a property descriptor.
struct PropertyDescriptor
{
	var value;
	PropertyFlagsType flags;
	basic_ptr <vsFunction> get;
	basic_ptr <vsFunction> set;

	~PropertyDescriptor ();

};

///  Type to represent a property descriptor pointer.
using PropertyDescriptorPtr = std::unique_ptr <PropertyDescriptor>;

/**
 *  Class to represent a basic object.
 */
class vsObject :
	virtual public vsValue
{
public:

	///  Creates a new default object.
	virtual
	var
	create (vsExecutionContext* const) const = 0;

	///  Creates a flat copy of this object.
	var
	clone (vsExecutionContext* const) const;

	///  Creates a deep copy of this object.
	virtual
	var
	copy (vsExecutionContext* const) const final override;

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const override
	{ return typeName; }

	///  Returns the type of the value. For basic objects this is »OBJECT«.
	virtual
	ValueType
	getType () const override
	{ return OBJECT; }

	///  @name Common operations

	///  Converts its input argument to a non-Object type.
	virtual
	var
	toPrimitive () const override
	{ return var (const_cast <vsObject*> (this)); }

	///  Returns true if the input argument is a non-Object type otherwise false.
	virtual
	bool
	isPrimitive () const final override
	{ return false; }

	///  Converts its argument to a value of type Boolean.
	virtual
	bool
	toBoolean () const override
	{ return true; }

	///  Converts its argument to an integral unsigned value of 16 bit.
	virtual
	uint16_t
	toUInt16 () const override
	{ return 1; }

	///  Converts its argument to an integral signed value of 32 bit.
	virtual
	int32_t
	toInt32 () const override
	{ return 1; }

	///  Converts its argument to an integral unsigned value of 32 bit.
	virtual
	uint32_t
	toUInt32 () const override
	{ return 1; }

	///  Converts its argument to a value of type Number.
	virtual
	double
	toNumber () const override
	{ return 1; }

	///  Converts its argument to a value of type Object.
	virtual
	var
	toObject () const
	throw (TypeError) override
	{ return var (const_cast <vsObject*> (this)); }

	///  Converts its input argument to either Primitive or Object type.
	virtual
	var
	toValue () const
	{ return var (const_cast <vsObject*> (this)); }

	///  @name Functions

	///  Checks wehter this object has a propterty @a name.
	bool
	hasProperty (const std::string & name) const
	noexcept (true)
	{ return propertyDescriptors .count (name); }

	///  Adds the named property described by the given descriptor to this object.
	void
	addProperty (const std::string & name,
	             const var & value,
	             const PropertyFlagsType flags = DEFAULT,
	             const var & get = var (),
	             const var & set = var ())
	throw (std::invalid_argument);

	///  Updates the named property described by the given descriptor to this object.
	void
	updateProperty (const std::string & name,
	                const var & value,
	                const PropertyFlagsType flags = DEFAULT,
	                const var & get = var (),
	                const var & set = var ())
	throw (std::invalid_argument);

	///  Removes the property @a name from this object.
	const var &
	getProperty (const std::string & name) const
	throw (std::out_of_range)
	{ return propertyDescriptors .at (name) -> value; }

	///  Removes the property @a name from this object.
	void
	removeProperty (const std::string & name)
	noexcept (true)
	{ propertyDescriptors .erase (name); }

	///  Returns the property descriptor for a named property on this object.
	const PropertyDescriptorPtr &
	getPropertyDescriptor (const std::string & name) const
	throw (std::out_of_range)
	{ return propertyDescriptors .at (name); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const override
	{ ostream << "[object " << getTypeName () << "]"; }

	///  @name Destruction

	///  Reclaims any resources consumed by this object, now or at any time in the future. If this object has already been
	///  disposed, further requests have no effect. Disposing an object does not remove the object itself.
	virtual
	void
	dispose () override;

	///  Destructs the vsObject.
	virtual
	~vsObject ();


protected:

	///  @name Construction

	///  Constructs new vsObject.
	vsObject () :
		            vsValue (),
		propertyDescriptors ()
	{ }

	///  Default initialize handler.
	virtual
	void
	initialize ()
	{ }


private:

	///  @name Static members

	static const std::string typeName;

	///  @name Members

	std::map <std::string, PropertyDescriptorPtr> propertyDescriptors;

};

} // pb
} // titania

#endif
