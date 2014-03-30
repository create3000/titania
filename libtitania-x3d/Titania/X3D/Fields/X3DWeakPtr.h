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
	using X3DField <ValueType*>::operator =;
	using X3DField <ValueType*>::addInterest;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::getValue;

	///  @name Constructors

	X3DWeakPtr () :
		X3DField <ValueType*> (nullptr)
	{ }

	X3DWeakPtr (const X3DWeakPtr & field) :
		X3DWeakPtr (field .getValue ())
	{ }

	explicit
	X3DWeakPtr (const X3DPtrBase & field) :
		X3DWeakPtr (dynamic_cast <ValueType*> (field .getBaseNode ()))
	{ }

	//explicit
	X3DWeakPtr (ValueType* const value) :
		X3DField <ValueType*> (value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> X3DBaseNode::shutdown () .addInterest (this, &X3DWeakPtr::set_shutdown);
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
	{ return new X3DWeakPtr (); }

	virtual
	X3DWeakPtr*
	clone () const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new X3DWeakPtr (*this); }

	virtual
	X3DWeakPtr*
	clone (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	virtual
	void
	clone (X3DExecutionContext* const, X3DFieldDefinition*) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	/// @name Assignment operators

	///  Default assignment operator.  Behaves the same as the 6.7.6 setValue service.
	X3DWeakPtr &
	operator = (const X3DWeakPtr &);

	X3DWeakPtr &
	operator = (const X3DPtrBase &);

	template <class Up>
	X3DWeakPtr &
	operator = (Up* const);

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

	///  @name Node services

	const std::string &
	getNodeTypeName () const
	{ return getValue () -> getTypeName (); }

	const X3DBaseNode*
	getNodeType () const
	{ return getValue () -> getType (); }

	const std::string &
	getNodeName () const
	{ return getValue () -> getName (); }

	const FieldDefinitionArray &
	getFieldDefinitions () const
	{ return getValue () -> getFieldDefinitions (); }

	X3DFieldDefinition*
	getField (const std::string & name) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>)
	{ return getValue () -> getField (name); }

	///  @name X3DChildObject
	virtual
	bool
	hasRoots (ChildObjectSet &) final override;

	///  @name Set value services

	virtual
	void
	set (const internal_type &) final override;

	virtual
	void
	write (const X3DChildObject &) final override;

	virtual
	X3DBaseNode*
	getBaseNode () const final override
	{ return getValue (); }

	///  @name Boolean operator

	operator bool () const
	{ return getValue (); }

	///  @name Access operators

	ValueType*
	operator -> () const
	{ return getValue (); }

	ValueType &
	operator * () const
	{ return *getValue (); }

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
	       Error <DISPOSED>) final override;

	virtual
	void
	toStream (std::ostream &) const final override;

	virtual
	void
	toXMLStream (std::ostream &) const final override;

	///  @name Dispose

	virtual
	void
	dispose () final override;

	virtual
	~X3DWeakPtr ();


private:

	template <class Up>
	friend class X3DWeakPtr;

	using X3DField <ValueType*>::reset;

	void
	addNode (ValueType* const);

	void
	removeNode (ValueType* const);

	void
	set_shutdown ();

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class ValueType>
const std::string X3DWeakPtr <ValueType>::typeName ("SFNode");

template <class ValueType>
X3DWeakPtr <ValueType>*
X3DWeakPtr <ValueType>::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DWeakPtr* const field = new X3DWeakPtr ();

	clone (executionContext, field);

	return field;
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DWeakPtr* const field = static_cast <X3DWeakPtr*> (fieldDefinition);

	if (getValue ())
		field -> set (dynamic_cast <ValueType*> (getValue () -> clone (executionContext)));

	else
		field -> set (nullptr);
}

template <class ValueType>
inline
X3DWeakPtr <ValueType> &
X3DWeakPtr <ValueType>::operator = (const X3DWeakPtr & field)
{
	X3DField <ValueType*>::operator = (field);
	return *this;
}

template <class ValueType>
inline
X3DWeakPtr <ValueType> &
X3DWeakPtr <ValueType>::operator = (const X3DPtrBase & field)
{
	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (field .getBaseNode ()));
	return *this;
}

template <class ValueType>
template <class Up>
inline
X3DWeakPtr <ValueType> &
X3DWeakPtr <ValueType>::operator = (Up* const value)
{
	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (value));
	return *this;
}

template <class ValueType>
bool
X3DWeakPtr <ValueType>::hasRoots (ChildObjectSet & seen)
{
	// Weak pointers are no roots.
	return false;
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::set (const internal_type & value)
{
	addNode (value);
	X3DField <ValueType*>::set (value);
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::write (const X3DChildObject & field)
{
	X3DBaseNode* baseNode = dynamic_cast <const X3DPtrBase &> (field) .getBaseNode ();

	set (dynamic_cast <internal_type> (baseNode));
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::addNode (ValueType* const value)
{
	if (getValue () not_eq value)
	{
		if (value)
		{
			value -> addWeakParent (this);
			value -> X3DBaseNode::shutdown () .addInterest (this, &X3DWeakPtr::set_shutdown);
		}

		removeNode (getValue ());
	}
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::removeNode (ValueType* const value)
{
	if (value)
	{
		reset ();

		value -> removeWeakParent (this);
		value -> X3DBaseNode::shutdown () .removeInterest (this, &X3DWeakPtr::set_shutdown);
	}
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::set_shutdown ()
{
	reset ();
	addEvent ();
}

template <class ValueType>
inline
void
X3DWeakPtr <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
inline
void
X3DWeakPtr <ValueType>::toStream (std::ostream & ostream) const
{
	if (getValue ())
		getValue () -> toStream (ostream);

	else
		ostream << "NULL";
}

template <class ValueType>
inline
void
X3DWeakPtr <ValueType>::toXMLStream (std::ostream & ostream) const
{
	if (getValue ())
		getValue () -> toXMLStream (ostream);

	else
		ostream << "NULL";
}

template <class ValueType>
void
X3DWeakPtr <ValueType>::dispose ()
{
	removeNode (getValue ());

	X3DField <ValueType*>::dispose ();
}

template <class ValueType>
inline
X3DWeakPtr <ValueType>::~X3DWeakPtr ()
{
	removeNode (getValue ());
}

template <class Type>
inline
bool
operator < (const X3DWeakPtr <Type> & lhs, const X3DWeakPtr <Type> & rhs)
{
	return lhs .getValue () < rhs .getValue ();
}

} // X3D
} // titania

#endif
