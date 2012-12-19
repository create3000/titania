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
 ******************************************************************************/

#ifndef __TITANIA_X3D_BASIC_X3DFIELD_H__
#define __TITANIA_X3D_BASIC_X3DFIELD_H__

#include "../Basic/X3DFieldDefinition.h"
#include "../InputOutput/Generator.h"
#include <cassert>

namespace titania {
namespace X3D {

template <class ValueType>
class X3DField :
	public X3DFieldDefinition
{
public:

	typedef ValueType value_type;

	using X3DFieldDefinition::addInterest;
	
	/// @name Copy

	///  Returns a copy of this field.
	virtual 
	X3DField <ValueType>*
	clone () const;
	
	/// @name Assignment operators

	///  Default assignment opeator.  Behaves the same as the 6.7.6 setValue service.
	X3DField &
	operator = (const X3DField &);

	///  Assignment operator for field values.  Behaves the same as the 6.7.6 setValue service.
	X3DField &
	operator = (const ValueType &);

	/// @name Elements access

	virtual
	X3DConstants::FieldType
	getType () const { return type; }

	virtual
	const basic::id &
	getTypeName () const { return typeName; }

	///  6.7.5 getValue service.
	const ValueType &
	getValue () const { return value; }

	///  6.7.6 setValue service.
	virtual
	void
	setValue (const ValueType &);

	///  Set @a value to this field without notifying this field.
	virtual
	void
	set (const ValueType &);

	///  Conversion operator.
	operator const ValueType & () const { return value; }
	
	///  Returns true if the type and the value of both fields are equal.
	virtual
	bool
	operator == (const X3DFieldDefinition &) const;

	///  Returns true if the type or the value of both fields are not equal.
	virtual
	bool
	operator not_eq (const X3DFieldDefinition & field) const { return not (*this == field); }

	//@{
	///  6.7.7 Add field interest.
	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const X3DField &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DField &)) const
	{
		addInterest (object, memberFunction, *this);
	}

	void
	addInterest (void (* requester) (const X3DField &)) const
	{
		addInterest (requester, *this);
	}

	///  6.7.7 Add field interest.
	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const ValueType &)) const
	{
		addInterest (object, memberFunction, value);
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const ValueType &)) const
	{
		addInterest (object, memberFunction, value);
	}

	void
	addInterest (void (* requester) (const ValueType &)) const
	{
		addInterest (requester, value);
	}
	//@}

	///  Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  Output operator.
	virtual
	void
	toStream (std::ostream &) const;

	///  6.7.8 dispose
	virtual
	void
	dispose ();


protected:

	/// @name Constructors
	
	///  Default constructor.
	X3DField ();

	///  Copy constructor.
	X3DField (const X3DField &);

	///  Value constructor.
	explicit
	X3DField (const ValueType &);
	
	/// @name Element access

	///  Get a non const value.
	ValueType &
	get () { return value; }

	///  Set @a field to this field without notifying this field.
	virtual
	void
	write (const X3DChildObject &);

	///  Set this field to its default value.
	void
	reset ();


private:

	///  The value for this field.
	ValueType value;

	///  TypeName identifer for X3DFields.
	static const basic::id typeName;

	///  Type identifer for X3DFields.
	static const X3DConstants::FieldType type;

};

template <class ValueType>
const basic::id X3DField <ValueType>::typeName = "X3DField";

template <class ValueType>
const X3DConstants::FieldType X3DField <ValueType>::type = X3DConstants::SFBool;

template <class ValueType>
X3DField <ValueType>::X3DField () :
	X3DFieldDefinition (),
	value ()
{ }

template <class ValueType>
X3DField <ValueType>::X3DField (const X3DField & field) :
	X3DFieldDefinition (),
	value (field .getValue ())
{ }

template <class ValueType>
X3DField <ValueType>::X3DField (const ValueType & value) :
	X3DFieldDefinition (),
	value (value)
{ }

template <class ValueType>
X3DField <ValueType>*
X3DField <ValueType>::clone () const
{
	return new X3DField <ValueType> (*this);
}

template <class ValueType>
X3DField <ValueType> &
X3DField <ValueType>::operator = (const X3DField & value)
{
	setValue (value .getValue ());
	return *this;
}

template <class ValueType>
X3DField <ValueType> &
X3DField <ValueType>::operator = (const ValueType & value)
{
	setValue (value);
	return *this;
}

template <class ValueType>
void
X3DField <ValueType>::setValue (const ValueType & value)
{
	set (value);
	notifyParents ();
}

template <class ValueType>
inline
void
X3DField <ValueType>::set (const ValueType & value)
{
	this -> value = value;
}

template <class ValueType>
void
X3DField <ValueType>::write (const X3DChildObject & field)
{
	//assert (getType () == field .getType ());

	set (static_cast <const X3DField &> (field) .getValue ());
}

template <class ValueType>
bool
X3DField <ValueType>::operator == (const X3DFieldDefinition & field) const
{
	if (getType () == field .getType ())
		return getValue () == static_cast <const X3DField &> (field) .getValue ();

	return false;
}

template <class ValueType>
inline
void
X3DField <ValueType>::reset ()
{
	value = ValueType ();
}

template <class ValueType>
inline
void
X3DField <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	//	istream >> value;
}

template <class ValueType>
inline
void
X3DField <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << value;
}

template <class ValueType>
inline
void
X3DField <ValueType>::dispose ()
{
	X3DFieldDefinition::dispose ();

	// Reset as final step when parents empty.
	X3DField::reset ();
}

//@{
template <class ValueType>
inline
bool
operator == (const X3DField <ValueType> & lhs, const X3DField <ValueType> & rhs)
{
	return lhs .getValue () == rhs .getValue ();
}

template <class ValueType>
inline
bool
operator == (const typename X3DField <ValueType>::value_type & lhs, const X3DField <ValueType> & rhs)
{
	return lhs == rhs .getValue ();
}

template <class ValueType>
inline
bool
operator == (const X3DField <ValueType> & lhs, const typename X3DField <ValueType>::value_type & rhs)
{
	return lhs .getValue () == rhs;
}
//@}

//@{
template <class ValueType>
inline
bool
operator not_eq (const X3DField <ValueType> & lhs, const X3DField <ValueType> & rhs)
{
	return lhs .getValue () not_eq rhs .getValue ();
}

template <class ValueType>
inline
bool
operator not_eq (const typename X3DField <ValueType>::value_type & lhs, const X3DField <ValueType> & rhs)
{
	return lhs not_eq rhs .getValue ();
}

template <class ValueType>
bool
operator not_eq (const X3DField <ValueType> & lhs, const typename X3DField <ValueType>::value_type & rhs)
{
	return lhs .getValue () not_eq rhs;
}
//@}

} // X3D
} // titania

#endif
