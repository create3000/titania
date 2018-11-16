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
#include "../Fields/X3DPtrObject.h"

#include <Titania/Backtrace.h>

namespace titania {
namespace X3D {

template <class InternalType>
class X3DWeakPtr;

template <class InternalType>
class X3DArrayField;

/**
 *  Template to represent a pointer that can handle circular references and that does
 *  automatic garbage collection.
 *
 *  Extern instantiations for X3DBaseNode are part of the
 *  library.  Results with any other X3DChildObject type are not guaranteed.
 *
 *  @param  InternalType  Type of the internal value of the field.
 */
template <class InternalType>
class X3DPtr :
	public X3DField <InternalType*>,
	public X3DPtrObject
{
public:

	///  @name Member types

	using internal_type = InternalType*;
	using value_type    = InternalType;

	using X3DField <InternalType*>::addEvent;
	using X3DField <InternalType*>::operator =;
	using X3DField <InternalType*>::addInterest;
	using X3DField <InternalType*>::setValue;
	using X3DField <InternalType*>::getValue;

	///  @name Construction

	///  Constructs new X3DPtr.
	X3DPtr () :
		X3DField <InternalType*> (nullptr),
		           cloneCount (0)
	{ }

	///  Constructs new X3DPtr.
	X3DPtr (std::nullptr_t) :
		X3DPtr ()
	{ }

	///  Constructs new X3DPtr.
	X3DPtr (const X3DPtr & other) :
		X3DPtr (other .getValue ())
	{ }

	///  Constructs new X3DPtr.
	template <class Up, std::enable_if_t <std::is_base_of_v <InternalType, Up>, bool> = false>
	X3DPtr (const X3DPtr <Up> & other) :
		X3DPtr (other .getValue ())
	{ }

	///  Constructs new X3DPtr.
	template <class Up, std::enable_if_t <not std::is_base_of_v <InternalType, Up>, bool> = true>
	explicit
	X3DPtr (const X3DPtr <Up> & other) :
		X3DPtr (dynamic_cast <InternalType*> (other .getValue ()))
	{ }

	///  Constructs new X3DPtr.
	template <class Up>
	explicit
	X3DPtr (const X3DWeakPtr <Up> & other) :
		X3DPtr (other .getValue ())
	{ }

	///  Constructs new X3DPtr.
	X3DPtr (X3DPtr && other) :
		X3DPtr ()
	{ moveObject (other); }

	///  Constructs new X3DPtr.
	template <class Up, std::enable_if_t <std::is_base_of_v <InternalType, Up>, bool> = false>
	X3DPtr (X3DPtr <Up> && other) :
		X3DPtr ()
	{ moveObject (other); }

	///  Constructs new X3DPtr.
	template <class Up, std::enable_if_t <not std::is_base_of_v <InternalType, Up>, bool> = true>
	explicit
	X3DPtr (X3DPtr <Up> && other) :
		X3DPtr ()
	{ moveObject (other); }

	///  Constructs new X3DPtr.
	explicit
	X3DPtr (InternalType* const value) :
		X3DField <InternalType*> (value),
		           cloneCount (0)
	{ addObject (value); }

	///  Constructs new X3DPtr.
	template <class Up>
	explicit
	X3DPtr (Up* const value) :
		X3DPtr (dynamic_cast <InternalType*> (value))
	{ }

	///  Constructs new X3DPtr.
	virtual
	X3DPtr*
	create () const final override
	{ return new X3DPtr (); }

	///  Constructs new X3DPtr.
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DPtr*
	copy (const CopyType type) const final override
	{
		if (type == FLAT_COPY)
			return new X3DPtr (getValue ());

		if (getValue ())
			return new X3DPtr (getValue () -> copy (type));

		return new X3DPtr ();
	}

	///  Constructs new X3DPtr.
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DPtr*
	copy (X3DExecutionContext* const executionContext, const CopyType type) const final override
	{
		const auto field = new X3DPtr ();

		copy (executionContext, field, type);

		return field;
	}

	///  Constructs new X3DPtr.
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	void
	copy (X3DExecutionContext* const executionContext, X3DFieldDefinition* const fieldDefinition, const CopyType type) const final override
	{
		const auto field = static_cast <X3DPtr*> (fieldDefinition);

		if (type == FLAT_COPY)
		{
			field -> set (getValue ());
		}
		else
		{
			if (getValue ())
				field -> set (dynamic_cast <InternalType*> (getValue () -> copy (executionContext, type)));

			else
				field -> set (nullptr);
		}
	}

	///  @name Assignment operators

	///  Assigns the nullptr and propagates an event.
	X3DPtr &
	operator = (std::nullptr_t)
	{
		setValue (nullptr);
		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	X3DPtr &
	operator = (const X3DPtr & other)
	{
		setValue (other .getValue ());
		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	template <class Up>
	X3DPtr &
	operator = (const X3DPtr <Up> & other)
	{
		setValue (dynamic_cast <InternalType*> (other .getValue ()));
		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	X3DPtr &
	operator = (X3DPtr && other)
	{
		if (&other == this)
		{
			addEvent ();
			return *this;
		}

		moveObject (other);
		addEvent ();

		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	template <class Up>
	X3DPtr &
	operator = (X3DPtr <Up> && other)
	{
		if (static_cast <X3DFieldDefinition*> (&other) == static_cast <X3DFieldDefinition*> (this))
		{
			addEvent ();
			return *this;
		}

		moveObject (other);
		addEvent ();

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
	set (const X3DChildObject & other) final override
	{
		X3DChildObject* const object = dynamic_cast <const X3DPtrObject &> (other) .getObject ();

		set (dynamic_cast <internal_type> (object));
	}

	///  @name Observers

	///  throws Error <DISPOSED>
	InternalType*
	operator -> () const
	{
	   const auto value = getValue ();

	   if (value)
			return value;

		//__LOG__ << "X3DPtr::operator -> ()\n\n" << std::endl;
		//backtrace ();

		throw Error <DISPOSED> ("X3DPtr::operator -> ()\n\n" + backtrace_symbols ());
	}

	///  throws Error <DISPOSED>
	InternalType &
	operator * () const
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
	getTypeName () const final override
	{ return typeName; }

	///  Returns the type of the object.
	virtual
	FieldType
	getType () const final override
	{ return X3DConstants::SFNode; }

	///  @name Clone handling

	///  Increments the clone count of its child by @a count.
	virtual
	void
	addCloneCount (const size_t count) final override
	{
		cloneCount += count;

		if (getValue ())
			getValue () -> addCloneCount (count);
	}

	///  Decrements the clone count of its child by @a count.
	virtual
	void
	removeCloneCount (const size_t count) final override
	{
		cloneCount -= count;

		if (getValue ())
			getValue () -> removeCloneCount (count);
	}
	
	///  @name Boolean operations

	///  Returns true if the contents of the arrays are equal, false otherwise.
	virtual
	bool
	equals (const X3DFieldDefinition & field) const final override
	{
		const auto rhs = dynamic_cast <const X3DPtrObject &> (field) .getObject ();

		const size_t a = getValue () ? getValue () -> getId () : 0;
		const size_t b = rhs ? rhs -> getId () : 0;

		return a == b;
	}

	///  @name Interest service

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DPtr &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DPtr &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Input/Output

	///  Not supported.
	virtual
	void
	fromStream (std::istream & istream) final override
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

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toJSONStream (std::ostream & ostream) const final override
	{
		if (getValue ())
			getValue () -> toJSONStream (ostream);

		else
			ostream << "null";
	}

	///  @name Destruction

	///  Destructs the owned object if no more X3DPtr link to it
	virtual
	void
	dispose () final override
	{
		removeObject (getValue ());

		X3DField <InternalType*>::dispose ();
	}

	///  Destructs the owned object if no more X3DPtr link to it
	virtual
	~X3DPtr () final override
	{ removeObject (getValue ()); }


protected:

	using X3DField <InternalType*>::reset;

	friend class X3DArrayField <X3DPtr <InternalType>>;

	void
	toJSONStreamValue (std::ostream & ostream) const
	{ }


private:

	///  @name Fiends

	template <class Up>
	friend class X3DPtr;

	///  @name Operations

	void
	addObject (InternalType* const value)
	{
		if (value)
		{
			value -> addParent (this);
			value -> addCloneCount (cloneCount);
			value -> X3DInput::disposed () .addInterest (&X3DPtr::set_disposed, this);
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
			value -> addCloneCount (cloneCount);
			value -> removeCloneCount (other .cloneCount);
			value -> X3DInput::disposed () .removeInterest (&X3DPtr::set_disposed, other);
			value -> X3DInput::disposed () .addInterest (&X3DPtr::set_disposed, this);
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
		const auto value = dynamic_cast <InternalType*> (other .getValue ());

		if (value == getValue ())
		{
			// If both values are NULL or equal, only set other to NULL.
			other = nullptr;
			return false;
		}

		if (value)
		{
			value -> replaceParent (&other, this);
			value -> addCloneCount (cloneCount);
			value -> removeCloneCount (other .cloneCount);
			value -> X3DInput::disposed () .removeInterest (&X3DPtr <Up>::set_disposed, other);
			value -> X3DInput::disposed () .addInterest (&X3DPtr::set_disposed, this);
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
	removeObject (InternalType* const value)
	{
		if (value)
		{
			setObject (nullptr);

			value -> X3DInput::disposed () .removeInterest (&X3DPtr::set_disposed, this);
			value -> removeCloneCount (cloneCount);
			value -> removeParent (this);
		}
	}

	void
	setObject (InternalType* const value)
	{ X3DField <InternalType*>::set (value); }

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

template <class InternalType>
const std::string X3DPtr <InternalType>::typeName = "SFNode";

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

///  @relates X3DPtr
///  @name Utiliy functions

///  Constructs an object of type InternalType and wraps it in a X3DPtr using
///  @a args as the parameter list for the constructor of Type.
template <class InternalType, class ... Args>
X3DPtr <InternalType>
MakePtr (Args && ... args)
{
	return X3DPtr <InternalType> (new InternalType (std::forward <Args> (args) ...));
}

///  Constructs an object of type InternalType and wraps it in a X3DPtr using
///  value as the parameter for the X3DPtr.
template <class InternalType, class ... Args>
X3DPtr <InternalType>
MakePtr (InternalType* const value)
{
	return X3DPtr <InternalType> (value);
}

} // X3D
} // titania

#endif
