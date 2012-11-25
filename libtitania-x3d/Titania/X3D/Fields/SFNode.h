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
 ******************************************************************************/

#ifndef __TITANIA_X3D_FIELDS_SFNODE_H__
#define __TITANIA_X3D_FIELDS_SFNODE_H__

#include "../Basic/X3DBasicNode.h"
#include "../Basic/X3DField.h"

namespace titania {
namespace X3D {

extern template class X3DField <X3DBasicNode*>;

template <class ValueType>
class SFNode :
	public X3DField <X3DBasicNode*>
{
public:

	typedef ValueType* value_type;
	typedef ValueType* scalar_type;
	typedef typename X3DField <X3DBasicNode*>::value_type basic_type;

	using X3DField <X3DBasicNode*>::addInterest;
	using X3DField <X3DBasicNode*>::setValue;
	using X3DField <X3DBasicNode*>::getValue;
	using X3DField <X3DBasicNode*>::operator =;

	///  @name Constructors

	SFNode ();

	SFNode (const SFNode &);

	template <class Up>
	SFNode (const SFNode <Up> &);

	//explicit
	SFNode (ValueType* const);

	template <class Up>
	SFNode (Up* const);

	///  @name Copy

	virtual
	SFNode*
	copy () const { return new SFNode (*this); }

	virtual
	SFNode*
	copy (X3DExecutionContext* const) const;

	///  @name Assignment Operator

	template <class Up>
	SFNode &
	operator = (const SFNode <Up> & field)
	{
		setValue (field);
		return *this;
	}

	template <class Up>
	SFNode &
	operator = (Up* const value)
	{
		setValue (value);
		return *this;
	}

	///  @name Node Services

	const basic::id &
	getNodeTypeName () const;

	const X3DType*
	getNodeType () const;

	void
	setNodeName (const basic::id &) const;

	const basic::id &
	getNodeName () const;

	const FieldDefinitionArray &
	getFieldDefinitions () const;

	X3DFieldDefinition*
	getField (const std::string &) const
	throw (Error <INVALID_NAME>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Set Value Services

	virtual
	void
	set (const basic_type &);

	///  @name Boolean Operator

	operator bool () const { return getLocalNode (); }

	///  @name Access Operators

	ValueType*
	operator -> () const { return getLocalNode (); }

	ValueType*
	operator * () const { return getLocalNode (); }

	///  @name 6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const SFNode &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFNode &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	void
	addInterest (void (* requester) (const SFNode &)) const
	{
		addInterest (requester, *this);
	}

	///  @name Object Functions

	virtual
	void
	dispose ();

	virtual
	~SFNode ();


private:

	using X3DField <X3DBasicNode*>::reset;

	void
	addNode (X3DBasicNode* const);

	void
	removeNode (X3DBasicNode* const);

	ValueType*
	getLocalNode () const
	{
		if (getValue ())
			return dynamic_cast <ValueType*> (getValue () -> getLocalNode ());

		return NULL;
	}

};

template <class ValueType>
SFNode <ValueType>::SFNode () :
	X3DField <X3DBasicNode*> ()
{ }

template <class ValueType>
SFNode <ValueType>::SFNode (const SFNode & field) :
	X3DField <X3DBasicNode*> (field)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
template <class Up>
SFNode <ValueType>::SFNode (const SFNode <Up> & field) :
	X3DField <X3DBasicNode*> (field)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
SFNode <ValueType>::SFNode (ValueType* const value) :
	X3DField <X3DBasicNode*> (value)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
template <class Up>
SFNode <ValueType>::SFNode (Up* const value) :
	X3DField <X3DBasicNode*> (value)
{
	if (getValue ())
		getValue () -> addParent (this);
}

template <class ValueType>
SFNode <ValueType>*
SFNode <ValueType>::copy (X3DExecutionContext* const executionContext) const
{
	if (getValue ())
		return new SFNode (getValue () -> copy (executionContext));

	else
		return new SFNode ();
}

template <class ValueType>
const basic::id &
SFNode <ValueType>::getNodeTypeName () const
{
	return getValue () -> getTypeName ();
}

template <class ValueType>
const X3DType*
SFNode <ValueType>::getNodeType () const
{
	return getValue () -> getType ();
}

template <class ValueType>
void
SFNode <ValueType>::setNodeName (const basic::id & name) const
{
	return getValue () -> setName (name);
}

template <class ValueType>
const basic::id &
SFNode <ValueType>::getNodeName () const
{
	return getValue () -> getName ();
}

template <class ValueType>
const FieldDefinitionArray &
SFNode <ValueType>::getFieldDefinitions () const
{
	return getValue () -> getFieldDefinitions ();
}

template <class ValueType>
X3DFieldDefinition*
SFNode <ValueType>::getField (const std::string & name) const
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return getValue () -> getField (name);
}

template <class ValueType>
void
SFNode <ValueType>::set (const basic_type & value)
{
	addNode (value);
	X3DField <X3DBasicNode*>::set (value);
}

template <class ValueType>
void
SFNode <ValueType>::addNode (X3DBasicNode* const value)
{
	if (getValue () not_eq value)
	{
		removeNode (getValue ());

		if (value)
			value -> addParent (this);
	}
}

template <class ValueType>
void
SFNode <ValueType>::removeNode (X3DBasicNode* const value)
{
	if (value)
	{
		reset ();

		value -> removeParent (this);
	}
}

template <class ValueType>
void
SFNode <ValueType>::dispose ()
{
	removeNode (getValue ());

	X3DField <X3DBasicNode*>::dispose ();
}

template <class ValueType>
SFNode <ValueType>::~SFNode ()
{
	removeNode (getValue ());
}

//
extern template class SFNode <X3DBasicNode>;

} // X3D
} // titania

#endif
