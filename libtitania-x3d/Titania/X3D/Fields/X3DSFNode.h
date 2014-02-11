/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_FIELDS_X3DSFNODE_H__
#define __TITANIA_X3D_FIELDS_X3DSFNODE_H__

#include "../Basic/X3DBaseNode.h"
#include "../Basic/X3DField.h"

namespace titania {
namespace X3D {

class X3DSFNodeBase :
	virtual public X3DBase
{
public:

	///  @name Operations
	
	virtual
	X3DBaseNode*
	getBaseValue () const = 0;


protected:

	X3DSFNodeBase () :
		X3DBase ()
	{ }

};

template <class ValueType>
class X3DSFNode :
	public X3DField <ValueType*>, public X3DSFNodeBase
{
public:

	typedef ValueType* internal_type;
	typedef ValueType* value_type;

	using X3DField <ValueType*>::operator =;
	using X3DField <ValueType*>::addInterest;
	using X3DField <ValueType*>::setValue;
	using X3DField <ValueType*>::getValue;

	///  @name Constructors

	X3DSFNode () :
		X3DField <ValueType*> (nullptr)
	{ }

	X3DSFNode (const X3DSFNode & field) :
		X3DSFNode (field .getValue ())
	{ }

	template <class Up>
	explicit
	X3DSFNode (const X3DSFNode <Up> & field) :
		X3DSFNode (dynamic_cast <ValueType*> (field .getValue ()))
	{ }

	X3DSFNode (X3DSFNode && field) :
		X3DSFNode (field .getValue ())
	{
		field = nullptr;
	}

	template <class Up>
	explicit
	X3DSFNode (X3DSFNode <Up> && field) :
		X3DSFNode (dynamic_cast <ValueType*> (field .getValue ()))
	{
		field = nullptr;
	}

	//explicit
	X3DSFNode (ValueType* const value) :
		X3DField <ValueType*> (value)
	{
		if (getValue ())
			getValue () -> addParent (this);
	}

	template <class Up>
	explicit
	X3DSFNode (Up* const value) :
		X3DSFNode (dynamic_cast <ValueType*> (value))
	{ }

	///  @name Copy

	virtual
	X3DSFNode*
	clone () const
	throw (Error <INVALID_NAME>,
          Error <NOT_SUPPORTED>) final override
	{ return new X3DSFNode (*this); }

	virtual
	X3DSFNode*
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
	X3DSFNode &
	operator = (const X3DSFNode &);

	template <class Up>
	X3DSFNode &
	operator = (const X3DSFNode <Up> &);

	///  Move assignment operator.  Behaves the same as the 6.7.6 setValue service.
	X3DSFNode &
	operator = (X3DSFNode &&);

	template <class Up>
	X3DSFNode &
	operator = (X3DSFNode <Up> &&);

	template <class Up>
	X3DSFNode &
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

	///  @name Set value services

	virtual
	void
	set (const internal_type &) final override;

	virtual
	void
	write (const X3DChildObject &) final override;
	
	virtual
	X3DBaseNode*
	getBaseValue () const final override
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
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DSFNode &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DSFNode &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const final override;

	///  @name Dispose

	virtual
	void
	dispose () final override;

	virtual
	~X3DSFNode ();


private:

	using X3DField <ValueType*>::reset;

	void
	addNode (ValueType* const);

	void
	removeNode (ValueType* const);

	///  TypeName identifer for X3DFields.
	static const std::string typeName;

};

template <class ValueType>
const std::string X3DSFNode <ValueType>::typeName ("SFNode");

template <class ValueType>
X3DSFNode <ValueType>*
X3DSFNode <ValueType>::clone (X3DExecutionContext* const executionContext) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DSFNode* const field = new X3DSFNode ();

	clone (executionContext, field);

	return field;
}

template <class ValueType>
void
X3DSFNode <ValueType>::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DSFNode* const field = static_cast <X3DSFNode*> (fieldDefinition);

	if (getValue ())
		field -> set (dynamic_cast <ValueType*> (getValue () -> clone (executionContext)));

	else
		field -> set (nullptr);
}

template <class ValueType>
inline
X3DSFNode <ValueType> &
X3DSFNode <ValueType>::operator = (const X3DSFNode & field)
{
	X3DField <ValueType*>::operator = (field);
	return *this;
}

template <class ValueType>
template <class Up>
inline
X3DSFNode <ValueType> &
X3DSFNode <ValueType>::operator = (const X3DSFNode <Up> & field)
{
	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (field .getValue ()));
	return *this;
}

template <class ValueType>
inline
X3DSFNode <ValueType> &
X3DSFNode <ValueType>::operator = (X3DSFNode && field)
{
	X3DField <ValueType*>::operator = (field);
	field = nullptr;
	return *this;
}

template <class ValueType>
template <class Up>
inline
X3DSFNode <ValueType> &
X3DSFNode <ValueType>::operator = (X3DSFNode <Up> && field)
{
	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (field .getValue ()));
	field = nullptr;
	return *this;
}

template <class ValueType>
template <class Up>
inline
X3DSFNode <ValueType> &
X3DSFNode <ValueType>::operator = (Up* const value)
{
	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (value));
	return *this;
}

template <class ValueType>
void
X3DSFNode <ValueType>::set (const internal_type & value)
{
	addNode (value);
	X3DField <ValueType*>::set (value);
}

template <class ValueType>
void
X3DSFNode <ValueType>::write (const X3DChildObject & field)
{
	X3DBaseNode* baseNode = dynamic_cast <const X3DSFNodeBase &> (field) .getBaseValue ();

	set (dynamic_cast <internal_type> (baseNode));
}

template <class ValueType>
void
X3DSFNode <ValueType>::addNode (ValueType* const value)
{
	if (getValue () not_eq value)
	{
		if (value)
			value -> addParent (this);

		removeNode (getValue ());
	}
}

template <class ValueType>
void
X3DSFNode <ValueType>::removeNode (ValueType* const value)
{
	if (value)
	{
		reset ();

		value -> removeParent (this);
	}
}

template <class ValueType>
inline
void
X3DSFNode <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
inline
void
X3DSFNode <ValueType>::toStream (std::ostream & ostream) const
{
	if (getValue ())
		getValue () -> toStream (ostream);

	else
		ostream << "NULL";
}

template <class ValueType>
void
X3DSFNode <ValueType>::dispose ()
{
	removeNode (getValue ());

	X3DField <ValueType*>::dispose ();
}

template <class ValueType>
inline
X3DSFNode <ValueType>::~X3DSFNode ()
{
	removeNode (getValue ());
}

template <class Type>
inline
bool
operator < (const X3DSFNode <Type> & lhs, const X3DSFNode <Type> & rhs)
{
	return lhs .getValue () < rhs .getValue ();
}

} // X3D
} // titania

#endif
