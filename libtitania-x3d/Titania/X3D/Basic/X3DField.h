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

#ifndef __TITANIA_X3D_BASIC_X3DFIELD_H__
#define __TITANIA_X3D_BASIC_X3DFIELD_H__

#include "../Basic/X3DFieldDefinition.h"
#include "../InputOutput/X3DGenerator.h"
#include "../Parser/Grammar.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DField :
	public X3DFieldDefinition
{
public:

	using internal_type = ValueType;

	using X3DFieldDefinition::addEvent;
	using X3DFieldDefinition::addInterest;
	using X3DFieldDefinition::processInterests;

	///  @name Construction

	virtual
	void
	copy (X3DExecutionContext* const, X3DFieldDefinition* field, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override
	{ static_cast <X3DField*> (field) -> set (value); }

	///  @name Assignment operators

	///  Default assignment operator.  Behaves the same as the 6.7.6 setValue service.
	X3DField &
	operator = (const X3DField & field)
	{
		setValue (field .getValue ());
		return *this;
	}

	///  Assignment operator for field values.  Behaves the same as the 6.7.6 setValue service.
	X3DField &
	operator = (const ValueType & value)
	{
		setValue (value);
		return *this;
	}

	/// @name Common members

	virtual
	X3DConstants::FieldType
	getType () const override
	{ return type; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) override
	{ return typeName; }

	///  @name Element access

	///  Returns true if this field has the default value for this field type, otherwise false.
	virtual
	bool
	isDefaultValue () const final override
	{
		static const ValueType defaultValue = ValueType ();

		return value == defaultValue;
	}

	///  Returns the value of this field.
	const ValueType &
	getValue () const
	{ return value; }

	///  Assigns @a value to this field and notifies its parents about a change.
	void
	setValue (const ValueType & value)
	{
		set (value);
		addEvent ();
	}

	///  DEPRECATED
	///  Deferredly assigns @a value to this field and notifies its parents about a change.
	void
	addEventObject (const X3DField & value)
	{
		const auto event = std::make_shared <Event> (&value);

		event -> sources .emplace (&value);

		X3DFieldDefinition::addEventObject (this, event);
	}

	///  Set @a value to this field without notifying this fields parents.
	void
	set (const X3DField & field)
	{ set (field .getValue ()); }

	///  Set @a value to this field without notifying this fields parents.
	virtual
	void
	set (const ValueType & value)
	{ this -> value = value; }

	///  Set @a field to this field without notifying this field.
	virtual
	void
	set (const X3DChildObject & field) override
	{
		set (static_cast <const X3DField &> (field) .getValue ());
	}

	///  Conversion operator.
	operator const ValueType & () const
	{ return value; }

	///  @name Logical operators

	///  Returns true if the type and the value of both fields are equal.
	virtual
	bool
	equals (const X3DFieldDefinition & field) const override
	{ return getValue () == static_cast <const X3DField &> (field) .getValue (); }

	///  @name Event handling

	///  6.7.7 Add field interest.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const ValueType &), Class* object) const
	{ addInterest (memberFunction, object, std::cref (value)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const ValueType &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (value)); }

	///  @name Destruction

	///  6.7.8 dispose
	virtual
	void
	dispose () override
	{
		X3DFieldDefinition::dispose ();
		reset (); // Reset as final step when parents empty.
	}

protected:

	/// @name Construction

	///  Default constructor.
	X3DField () :
		X3DFieldDefinition (),
		             value ()
	{ }

	///  Copy constructor.
	X3DField (const X3DField & field) :
		X3DFieldDefinition (),
		             value (field .getValue ())
	{ }

	///  Value constructor.
	explicit
	X3DField (const ValueType & value) :
		X3DFieldDefinition (),
		             value (value)
	{ }

	///  Move Value constructor.
	explicit
	X3DField (ValueType && other) :
		X3DFieldDefinition (),
		             value (std::move (other))
	{ }

	/// @name Member access

	///  Get a non const value.
	ValueType &
	get ()
	{ return value; }

	///  Set this field to its default value.
	virtual
	void
	reset ()
	{ value = ValueType (); }


private:

	/// @name Static members

	///  TypeName identifer for X3DFields.
	static const std::string   typeName;

	///  Type identifer for X3DFields.
	static const X3DConstants::FieldType type;

	/// @name Members

	///  The value for this field.
	ValueType value;

};

template <class ValueType>
const std::string X3DField <ValueType>::typeName = "X3DField";

template <class ValueType>
const X3DConstants::FieldType X3DField <ValueType>::type = X3DConstants::SFBool;

///  @relates X3DField
///  @name Comparision operations

///  Compares two X3DField numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class ValueType>
inline
bool
operator == (const X3DField <ValueType> & lhs, const X3DField <ValueType> & rhs)
{
	return lhs .getValue () == rhs .getValue ();
}

///  Compares two X3DField numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class ValueType>
inline
bool
operator == (const typename X3DField <ValueType>::internal_type & lhs, const X3DField <ValueType> & rhs)
{
	return lhs == rhs .getValue ();
}

///  Compares two X3DField numbers.
///  Returns true if @a lhs is equal to @a rhs.
template <class ValueType>
inline
bool
operator == (const X3DField <ValueType> & lhs, const typename X3DField <ValueType>::internal_type & rhs)
{
	return lhs .getValue () == rhs;
}

///  Compares two X3DField numbers.
///  Returns true if @a lhs is not equal to @a rhs.
template <class ValueType>
inline
bool
operator not_eq (const X3DField <ValueType> & lhs, const X3DField <ValueType> & rhs)
{
	return lhs .getValue () not_eq rhs .getValue ();
}

///  Compares two X3DField numbers.
///  Returns true if @a lhs is not equal to @a rhs.
template <class ValueType>
inline
bool
operator not_eq (const typename X3DField <ValueType>::internal_type & lhs, const X3DField <ValueType> & rhs)
{
	return lhs not_eq rhs .getValue ();
}

///  Compares two X3DField numbers.
///  Returns true if @a lhs is not equal to @a rhs.
template <class ValueType>
inline
bool
operator not_eq (const X3DField <ValueType> & lhs, const typename X3DField <ValueType>::internal_type & rhs)
{
	return lhs .getValue () not_eq rhs;
}

} // X3D
} // titania

#endif
