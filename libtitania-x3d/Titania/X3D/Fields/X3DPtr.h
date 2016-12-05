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

#ifndef __TITANIA_X3D_FIELDS_X3DPTR_H__
#define __TITANIA_X3D_FIELDS_X3DPTR_H__

#include "../Basic/X3DField.h"

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

/**
 *  Base class for X3DPtr and X3DWeakPtr.
 */
class X3DPtrBase :
	virtual public X3DBase
{
public:

	///  @name Destruction

	virtual
	~X3DPtrBase ()
	{ }


protected:

	///  @name Fiends

	template <class Up>
	friend class X3DPtr;

	template <class Up>
	friend class X3DWeakPtr;

	///  @name Construction

	X3DPtrBase () :
		X3DBase ()
	{ }

	///  @name Operations

	virtual
	X3DChildObject*
	getObject () const = 0;

};

/**
 *  Template to represent a pointer that can handle circular references and that does
 *  automatic garbage collection.
 *
 *  Extern instantiations for X3DBaseNode are part of the
 *  library.  Results with any other X3DChildObject type are not guaranteed.
 *
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class X3DPtr :
	public X3DField <ValueType*>, public X3DPtrBase
{
public:

	///  @name Member types

	typedef ValueType* internal_type;
	typedef ValueType* value_type;

	using X3DField <ValueType*>::addEvent;
	using X3DField <ValueType*>::operator =;
	using X3DField <ValueType*>::addInterest;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::getValue;

	///  @name Construction

	///  Constructs new X3DPtr.
	X3DPtr () :
		X3DField <ValueType*> (nullptr),
		           cloneCount (0)
	{ }

	///  Constructs new X3DPtr.
	X3DPtr (const X3DPtr & field) :
		X3DPtr (field .getValue ())
	{ }

	///  Constructs new X3DPtr.
	explicit
	X3DPtr (const X3DPtrBase & field) :
		X3DPtr (dynamic_cast <ValueType*> (field .getObject ()))
	{ }

	///  Constructs new X3DPtr.
	X3DPtr (X3DPtr && field) :
		X3DPtr ()
	{ moveObject (field); }

	///  Constructs new X3DPtr.
	template <class Up>
	explicit
	X3DPtr (X3DPtr <Up> && field) :
		X3DPtr ()
	{ moveObject (field); }

	///  Constructs new X3DPtr.
	//explicit
	X3DPtr (ValueType* const value) :
		X3DField <ValueType*> (value),
		           cloneCount (0)
	{ addObject (value); }

	///  Constructs new X3DPtr.
	template <class Up>
	explicit
	X3DPtr (Up* const value) :
		X3DPtr (dynamic_cast <ValueType*> (value))
	{ }

	///  Constructs new X3DPtr.
	virtual
	X3DPtr*
	create () const final override
	{ return new X3DPtr (); }

	///  Constructs new X3DPtr.
	virtual
	X3DPtr*
	copy (const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{
		if (type == FLAT_COPY)
			return new X3DPtr (getValue ());

		if (getValue ())
			return new X3DPtr (getValue () -> copy (type));

		return new X3DPtr ();
	}

	///  Constructs new X3DPtr.
	virtual
	X3DPtr*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{
		X3DPtr* const field = new X3DPtr ();

		copy (executionContext, field, type);

		return field;
	}

	///  Constructs new X3DPtr.
	virtual
	void
	copy (X3DExecutionContext* const executionContext, X3DFieldDefinition* const fieldDefinition, const CopyType type) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{
		X3DPtr* const field = static_cast <X3DPtr*> (fieldDefinition);

		if (type == FLAT_COPY)
		{
			field -> set (getValue ());
		}
		else
		{
			if (getValue ())
				field -> set (dynamic_cast <ValueType*> (getValue () -> copy (executionContext, type)));

			else
				field -> set (nullptr);
		}
	}

	///  @name Assignment operators

	///  Assigns the X3DPtr and propagates an event.
	X3DPtr &
	operator = (const X3DPtr & field)
	{
		setValue (field);
		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	X3DPtr &
	operator = (const X3DPtrBase & field)
	{
		setValue (dynamic_cast <ValueType*> (field .getObject ()));
		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	X3DPtr &
	operator = (X3DPtr && other)
	{
		if (&other == this)
			return *this;

		if (moveObject (other))
			addEvent ();

		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	template <class Up>
	X3DPtr &
	operator = (X3DPtr <Up> && other)
	{
		if (static_cast <X3DFieldDefinition*> (&other) == static_cast <X3DFieldDefinition*> (this))
			return *this;

		if (moveObject (other))
			addEvent ();

		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	template <class Up>
	X3DPtr &
	operator = (Up* const value)
	{
		setValue (dynamic_cast <ValueType*> (value));
		return *this;
	}

	///  @name Modifiers

	///  Assigns the X3DPtr without propagating an event.
	virtual
	void
	set (const internal_type & value) final override
	{
		if (value == getValue ())
			return;

		// FIRST ADD OBJECT TO AVOID DISPOSE!!!
		addObject (value);
		removeObject (getValue ());
		setObject (value);
	}

	///  Assigns the X3DPtr without propagating an event.
	virtual
	void
	set (const X3DChildObject & field) final override
	{
		X3DChildObject* const object = dynamic_cast <const X3DPtrBase &> (field) .getObject ();

		set (dynamic_cast <internal_type> (object));
	}

	///  @name Observers

	ValueType*
	operator -> () const
	throw (Error <DISPOSED>)
	{
	   const auto value = getValue ();

	   if (value)
			return value;

		//__LOG__ << "X3DPtr::operator -> ()\n\n" << std::endl;
		//__LOG__ << backtrace_symbols () << std::endl;

		throw Error <DISPOSED> ("X3DPtr::operator -> ()\n\n" + backtrace_symbols ());
	}

	ValueType &
	operator * () const
	throw (Error <DISPOSED>)
	{
	   const auto value = getValue ();

	   if (value)
			return *value;

		throw Error <DISPOSED> ("X3DPtr::operator * ()\n\n" + backtrace_symbols ());
	}

	operator bool () const
	{ return getValue (); }

	///  @name Common members

	///  Returns the type name of the object.
	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	///  Returns the type of the object.
	virtual
	X3DConstants::FieldType
	getType () const final override
	{ return X3DConstants::SFNode; }

	///  @name Clone handling

	///  Increments the clone count of its child by @a count.
	virtual
	void
	addClones (const size_t count) final override
	{
		cloneCount += count;

		if (getValue ())
			getValue () -> addClones (count);
	}

	///  Decrements the clone count of its child by @a count.
	virtual
	void
	removeClones (const size_t count) final override
	{
		cloneCount -= count;

		if (getValue ())
			getValue () -> removeClones (count);
	}
	
	///  @name Boolean operations

	///  Returns true if the contents of the arrays are equal, false otherwise.
	virtual
	bool
	operator == (const X3DFieldDefinition & field) const final override
	{
		const auto rhs = dynamic_cast <const X3DPtrBase &> (field) .getObject ();

		const size_t a = getValue () ? getValue () -> getId () : 0;
		const size_t b = rhs ? rhs -> getId () : 0;

		return a == b;
	}

	///  Returns true if the contents of the arrays are not equal, false otherwise.
	virtual
	bool
	operator not_eq (const X3DFieldDefinition & field) const final override
	{
		const auto rhs = dynamic_cast <const X3DPtrBase &> (field) .getObject ();

		const size_t a = getValue () ? getValue () -> getId () : 0;
		const size_t b = rhs ? rhs -> getId () : 0;

		return a not_eq b;
	}

	///  @name Interest service

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Input/Output

	///  Not supported.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ }

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (getValue ())
			getValue () -> toStream (ostream);

		else
			ostream << "NULL";
	}

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream & ostream) const final override
	{
		if (getValue ())
			getValue () -> toXMLStream (ostream);

		else
			ostream << "NULL";
	}

	///  @name Destruction

	///  Destructs the owned object if no more X3DPtr link to it
	virtual
	void
	dispose () final override
	{
		removeObject (getValue ());

		X3DField <ValueType*>::dispose ();
	}

	///  Destructs the owned object if no more X3DPtr link to it
	virtual
	~X3DPtr ()
	{ removeObject (getValue ()); }


private:

	///  @name Fiends

	template <class Up>
	friend class X3DPtr;

	using X3DField <ValueType*>::reset;

	///  @name Operations

	void
	addObject (ValueType* const value)
	{
		if (value)
		{
			value -> addParent (this);
			value -> addClones (cloneCount);
			value -> X3DInput::disposed () .addInterest (this, &X3DPtr::set_disposed);
		}
	}

	bool
	moveObject (X3DPtr & other)
	{
		const auto value = other .getValue ();
		
		if (value == getValue ())
		{
			// If both values are NULL or equal, only set other to NULL.
			other = nullptr;
			return false;
		}

		if (value)
		{
			value -> replaceParent (&other, this);
			value -> addClones (cloneCount);
			value -> removeClones (other .cloneCount);
			value -> X3DInput::disposed () .removeInterest (other, &X3DPtr::set_disposed);
			value -> X3DInput::disposed () .addInterest (this, &X3DPtr::set_disposed);
			other .setObject (nullptr);
			other .addEvent ();
		}

		removeObject (getValue ());
		setObject (value);
		return true;
	}

	template <class Up>
	bool
	moveObject (X3DPtr <Up> & other)
	{
		const auto value = dynamic_cast <ValueType*> (other .getValue ());

		if (value == getValue ())
		{
			// If both values are NULL or equal, only set other to NULL.
			other = nullptr;
			return false;
		}

		if (value)
		{
			value -> replaceParent (&other, this);
			value -> addClones (cloneCount);
			value -> removeClones (other .cloneCount);
			value -> X3DInput::disposed () .removeInterest (other, &X3DPtr <Up>::set_disposed);
			value -> X3DInput::disposed () .addInterest (this, &X3DPtr::set_disposed);
			other .setObject (nullptr);
			other .addEvent ();
		}
		else
			other = nullptr;

		removeObject (getValue ());
		setObject (value);
		return true;
	}

	void
	removeObject (ValueType* const value)
	{
		if (value)
		{
			setObject (nullptr);

			value -> X3DInput::disposed () .removeInterest (this, &X3DPtr::set_disposed);
			value -> removeClones (cloneCount);
			value -> removeParent (this);
		}
	}

	void
	setObject (ValueType* const value)
	{ X3DField <ValueType*>::set (value); }

	virtual
	X3DChildObject*
	getObject () const final override
	{ return getValue (); }

	void
	set_disposed ()
	{
		setObject (nullptr);
		addEvent ();
	}

	///  @name Static members

	static const std::string typeName;

	///  @name Members

	size_t cloneCount;

};

template <class ValueType>
const std::string X3DPtr <ValueType>::typeName ("SFNode");

///  @relates X3DPtr
///  @name Comparision operations

///  Compares two X3DPtr.
///  Returns true if @a lhs is equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator == (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a == b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs is not equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator not_eq (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a not_eq b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs less than @a rhs.
template <class LHS, class RHS>
inline
bool
operator < (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a < b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator <= (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a <= b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs greater than @a rhs.
template <class LHS, class RHS>
inline
bool
operator > (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a > b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator >= (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a >= b;
}

} // X3D
} // titania

#endif
