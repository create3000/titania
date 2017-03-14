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

template <class ValueType>
class X3DPtr;

template <class ValueType>
class X3DBasePtrArray;

template <class ValueType>
class X3DWeakPtr :
	public X3DField <ValueType*>,
	public X3DPtrObject
{
public:

	using internal_type = ValueType*;
	using value_type    = ValueType;

	using X3DField <ValueType*>::getParents;
	using X3DField <ValueType*>::addEvent;
	using X3DField <ValueType*>::addInterest;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::getValue;

	///  @name Constructors

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr () :
		X3DField <ValueType*> ()
	{ }

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr (std::nullptr_t) :
		X3DField <ValueType*> ()
	{ }

	///  Constructs new X3DWeakPtr.
	X3DWeakPtr (const X3DWeakPtr & other) :
		X3DWeakPtr (other .getValue ())
	{ }

	///  Constructs new X3DWeakPtr.
	template <class Up>
	X3DWeakPtr (const X3DPtr <Up> & other) :
		X3DWeakPtr (other .getValue ())
	{ }

	///  Constructs new X3DWeakPtr.
	explicit
	X3DWeakPtr (ValueType* const value) :
		X3DField <ValueType*> (value)
	{ addObject (value); }

	///  Constructs new X3DWeakPtr.
	template <class Up>
	explicit
	X3DWeakPtr (Up* const value) :
		X3DWeakPtr (dynamic_cast <ValueType*> (value))
	{ }

	///  @name Field services

	virtual
	X3DConstants::FieldType
	getType () const final override
	{ return X3DConstants::SFNode; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	///  @name Observers

	ValueType*
	operator -> () const
	throw (Error <DISPOSED>)
	{
	   const auto value = getValue ();

	   if (value)
			return value;

		//__LOG__ << "X3DPtr::operator -> ()\n\n" << std::endl;
		//backtrace ();

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

	X3DPtr <ValueType>
	getLock () const
	{ return X3DPtr <ValueType> (getValue ()); }

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

		X3DField <ValueType*>::dispose ();
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

	friend class X3DBasePtrArray <X3DWeakPtr <ValueType>>;

	friend class X3DArrayField <X3DWeakPtr <ValueType>>;

	///  @name X3DChildObject

	virtual
	bool
	hasRootedObjects (ChildObjectSet &) final override
	{
		// Weak pointers are no roots.
		return false;
	}

	void
	toJSONStreamValue (std::ostream &) const
	{ }


private:

	///  @name Construction

	virtual
	X3DWeakPtr*
	create () const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::create: not supported!"); }

	virtual
	X3DWeakPtr*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	virtual
	X3DWeakPtr*
	copy (X3DExecutionContext* const, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	virtual
	void
	copy (X3DExecutionContext* const, X3DFieldDefinition*, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::copy: not supported!"); }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::fromStream: not supported!"); }

	virtual
	void
	toStream (std::ostream &) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::toStream: not supported!"); }

	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ throw Error <NOT_SUPPORTED> ("X3DWeakPtr::toXMLStream: not supported!"); }

	virtual
	void
	toJSONStream (std::ostream &) const final override
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
	addObject (ValueType* const value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> X3DInput::disposed () .addInterest (&X3DWeakPtr::set_disposed, this);
		}
	}

	void
	removeObject (ValueType* const value)
	{
		if (value)
		{
			setObject (nullptr);

			value -> X3DInput::disposed () .removeInterest (&X3DWeakPtr::set_disposed, this);
			value -> removeWeakParent (this);
		}
	}

	void
	setObject (ValueType* const value)
	{ X3DField <ValueType*>::set (value); }

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

template <class ValueType>
const std::string X3DWeakPtr <ValueType>::typeName ("SFNode");

///  @relates X3DWeakPtr
///  @name Comparision operations

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs is equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator == (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a == b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs is not equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator not_eq (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a not_eq b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs less than @a rhs.
template <class LHS, class RHS>
inline
bool
operator < (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a < b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator <= (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a <= b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs greater than @a rhs.
template <class LHS, class RHS>
inline
bool
operator > (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a > b;
}

///  Compares two X3DWeakPtr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator >= (const X3DWeakPtr <LHS> & lhs, const X3DWeakPtr <RHS> & rhs)
throw (Error <DISPOSED>)
{
	const size_t a = lhs ? lhs -> getId () : 0;
	const size_t b = rhs ? rhs -> getId () : 0;

	return a >= b;
}

} // X3D
} // titania

#endif
