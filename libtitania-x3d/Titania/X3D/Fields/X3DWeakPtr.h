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

#ifndef __TITANIA_X3D_FIELDS_X3DWEAK_SFNODE_H__
#define __TITANIA_X3D_FIELDS_X3DWEAK_SFNODE_H__

#include "../Basic/X3DField.h"
#include "../Fields/X3DPtrObject.h"

namespace titania {
namespace X3D {

template <class InternalType>
class X3DPtr;

template <class InternalType>
class X3DBasePtrArray;

template <class InternalType>
class X3DWeakPtr :
	public X3DField <InternalType*>,
	public X3DPtrObject
{
public:

	using internal_type = InternalType*;
	using value_type    = InternalType;

	using X3DField <InternalType*>::getParents;
	using X3DField <InternalType*>::addEvent;
	using X3DField <InternalType*>::addInterest;
	using X3DField <InternalType*>::setValue;
	using X3DField <InternalType*>::getValue;

	///  @name Constructors

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr () :
		X3DField <InternalType*> ()
	{ }

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr (std::nullptr_t) :
		X3DField <InternalType*> ()
	{ }

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr (const X3DWeakPtr & other) :
		X3DWeakPtr (other .getValue ())
	{ }

	///  Constructs new X3DWeakPtr.
	template <class Up, std::enable_if_t <std::is_base_of_v <InternalType, Up>, bool> = false>
	X3DWeakPtr (const X3DWeakPtr <Up> & other) :
		X3DWeakPtr (other .getValue ())
	{ }

	///  Constructs new X3DWeakPtr.
	template <class Up, std::enable_if_t <not std::is_base_of_v <InternalType, Up>, bool> = true>
	explicit
	X3DWeakPtr (const X3DWeakPtr <Up> & other) :
		X3DWeakPtr (dynamic_cast <InternalType*> (other .getValue ()))
	{ }

	///  Constructs new X3DWeakPtr.
	template <class Up>
	X3DWeakPtr (const X3DPtr <Up> & other) :
		X3DWeakPtr (other .getValue ())
	{ }

	///  Constructs new X3DWeakPtr.
	explicit
	X3DWeakPtr (InternalType* const value) :
		X3DField <InternalType*> (value)
	{ addObject (value); }

	///  Constructs new X3DWeakPtr.
	template <class Up>
	explicit
	X3DWeakPtr (Up* const value) :
		X3DWeakPtr (dynamic_cast <InternalType*> (value))
	{ }

	///  @name Field services

	virtual
	FieldType
	getType () const final override
	{ return X3DConstants::SFNode; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	///  @name Observers

	///  Error <DISPOSED>
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

	X3DPtr <InternalType>
	getLock () const
	{ return X3DPtr <InternalType> (getValue ()); }

	///  @name 6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DWeakPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DWeakPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Dispose

	virtual
	void
	dispose () final override
	{
		removeObject (getValue ());

		X3DField <InternalType*>::dispose ();
	}

	virtual
	~X3DWeakPtr () final override
	{ removeObject (getValue ()); }


protected:

	///  @name Friends

	template <class Up>
	friend class X3DPtr;

	template <class Up>
	friend class X3DWeakPtr;

	friend class X3DBasePtrArray <X3DWeakPtr <InternalType>>;

	friend class X3DArrayField <X3DWeakPtr <InternalType>>;

	///  @name X3DChildObject

	virtual
	bool
	hasRootedObjects (ChildObjectSet & seen) final override
	{
		// Weak pointers are no roots.
		return false;
	}

	void
	toJSONStreamValue (std::ostream & ostream) const
	{ }


private:

	///  @name Construction

	///  throws Error <NOT_SUPPORTED>
	virtual
	X3DWeakPtr*
	create () const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::create: not supported!"); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DWeakPtr*
	copy (const CopyType) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DWeakPtr*
	copy (X3DExecutionContext* const, const CopyType) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	void
	copy (X3DExecutionContext* const, X3DFieldDefinition*, const CopyType) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	///  @name Input/Output

	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	void
	fromStream (std::istream & istream) final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::fromStream: not supported!"); }

	///  throws Error <NOT_SUPPORTED>
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::toStream: not supported!"); }

	///  throws Error <NOT_SUPPORTED>
	virtual
	void
	toXMLStream (std::ostream & ostream) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::toXMLStream: not supported!"); }

	///  throws Error <NOT_SUPPORTED>
	virtual
	void
	toJSONStream (std::ostream & ostream) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::toJSONtream: not supported!"); }


	///  @name Set value services

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

	virtual
	void
	set (const X3DChildObject & field) final override
	{
		X3DChildObject* const object = dynamic_cast <const X3DPtrObject &> (field) .getObject ();

		set (dynamic_cast <internal_type> (object));
	}

	void
	addObject (InternalType* const value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> X3DInput::disposed () .addInterest (&X3DWeakPtr::set_disposed, this);
		}
	}

	void
	removeObject (InternalType* const value)
	{
		if (value)
		{
			setObject (nullptr);

			value -> X3DInput::disposed () .removeInterest (&X3DWeakPtr::set_disposed, this);
			value -> removeWeakParent (this);
		}
	}

	void
	setObject (InternalType* const value)
	{ X3DField <InternalType*>::set (value); }

	virtual
	X3DChildObject*
	getObject () const final override
	{
		if (*this)
			return getValue ();

		return nullptr;
	}

	void
	set_disposed ()
	{
		setObject (nullptr);
		addEvent ();
	}

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class InternalType>
const std::string X3DWeakPtr <InternalType>::typeName = "SFNode";

///  @relates X3DWeakPtr
///  @name Comparision operations

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs is equal to @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator == (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a == b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs is not equal to @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator not_eq (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a not_eq b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs less than @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator < (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a < b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs less than equal to @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator <= (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a <= b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs greater than @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator > (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a > b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs greater than equal to @a rhs.
///  throws  Error <DISPOSED>
template <class LHS, class RHS>
inline
bool
operator >= (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a >= b;
}

} // X3D
} // titania

#endif
