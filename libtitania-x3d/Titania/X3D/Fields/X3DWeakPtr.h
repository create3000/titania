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

#ifndef __TITANIA_X3D_FIELDS_X3DWEAK_SFNODE_H__
#define __TITANIA_X3D_FIELDS_X3DWEAK_SFNODE_H__

#include "X3DPtr.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DWeakPtr :
	public X3DField <ValueType*>, public X3DPtrBase
{
public:

	typedef ValueType* internal_type;
	typedef ValueType* value_type;

	using X3DField <ValueType*>::getParents;
	using X3DField <ValueType*>::addEvent;
	using X3DField <ValueType*>::addInterest;

	///  @name Constructors

	X3DWeakPtr () = delete;

	X3DWeakPtr (const X3DWeakPtr & field) :
		X3DWeakPtr (field .getValue ())
	{ }

	explicit
	X3DWeakPtr (const X3DPtrBase & field) :
		X3DWeakPtr (dynamic_cast <ValueType*> (field .getObject ()))
	{ }

	//explicit
	X3DWeakPtr (ValueType* const value) :
		X3DField <ValueType*> (value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> disposed () .addInterest (this, &X3DWeakPtr::set_disposed);
		}
	}

	template <class Up>
	explicit
	X3DWeakPtr (Up* const value) :
		X3DWeakPtr (dynamic_cast <ValueType*> (value))
	{ }

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

	///  @name X3DChildObject
	virtual
	bool
	hasRootedObjects (ChildObjectSet &) final override
	{
		// Weak pointers are no roots.
		return false;
	}

	///  @name Boolean operator

	operator bool () const
	{ return getValue () and getValue () -> getReferenceCount (); }

	///  @name 6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DWeakPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DWeakPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

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

	///  @name Dispose

	virtual
	void
	dispose () final override;

	virtual
	~X3DWeakPtr ();


private:

	template <class Up>
	friend class X3DWeakPtr;

	using X3DField <ValueType*>::isDefaultValue;
	using X3DField <ValueType*>::getValue;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::reset;
	using X3DField <ValueType*>::operator const value_type &;
	using X3DField <ValueType*>::operator =;
	using X3DField <ValueType*>::operator ==;
	using X3DField <ValueType*>::operator not_eq;

	///  @name Set value services

	virtual
	void
	set (const internal_type &) final override;

	virtual
	void
	write (const X3DChildObject &) final override;

	void
	addObject (ValueType* const);

	void
	removeObject (ValueType* const);

	virtual
	X3DChildObject*
	getObject () const
	throw (Error <DISPOSED>) final override
	{
		if (*this)
			return getValue ();

		throw Error <DISPOSED> ("X3DWeakPtr::getObject: value is already disposed!");
	}

	void
	set_disposed ();

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class ValueType>
const std::string X3DWeakPtr <ValueType>::typeName ("SFNode");

template <class ValueType>
void
X3DWeakPtr <ValueType>::set (const internal_type & value)
{
	addObject (value);
	X3DField <ValueType*>::set (value);
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::write (const X3DChildObject & field)
{
	X3DChildObject* const object = dynamic_cast <const X3DPtrBase &> (field) .getObject ();

	set (dynamic_cast <internal_type> (object));
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::addObject (ValueType* const value)
{
	if (getValue () not_eq value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> disposed () .addInterest (this, &X3DWeakPtr::set_disposed);
		}

		removeObject (getValue ());
	}
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::removeObject (ValueType* const value)
{
	if (value)
	{
		reset ();

		value -> removeWeakParent (this);
		value -> disposed () .removeInterest (this, &X3DWeakPtr::set_disposed);
	}
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::set_disposed ()
{
	reset ();
	addEvent ();
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::dispose ()
{
	removeObject (getValue ());

	X3DField <ValueType*>::dispose ();
}

template <class ValueType>
inline
X3DWeakPtr <ValueType>::~X3DWeakPtr ()
{
	removeObject (getValue ());
}

///  @relates X3DWeakPtr
///  @name Comparision operations

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator == (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator ==: not supported!");
}

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator not_eq (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator not_eq: not supported!");
}

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator < (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator <: not supported!");
}

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator <= (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator <=: not supported!");
}

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator > (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator >: not supported!");
}

///  NOT_SUPPORTED
template <class ValueType>
inline
bool
operator >= (const X3DWeakPtr <ValueType> & lhs, const X3DWeakPtr <ValueType> & rhs)
throw (Error <NOT_SUPPORTED>)
{
	throw Error <NOT_SUPPORTED> ("X3DWeakPtr::operator >=: not supported!");
}

} // X3D
} // titania

#endif
