///* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
// *******************************************************************************
// *
// * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// *
// * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
// *
// * All rights reserved. Holger Seelig <holger.seelig@yahoo.de>.
// *
// * THIS IS UNPUBLISHED SOURCE CODE OF create3000.
// *
// * The copyright notice above does not evidence any actual of intended
// * publication of such source code, and is an unpublished work by create3000.
// * This material contains CONFIDENTIAL INFORMATION that is the property of
// * create3000.
// *
// * No permission is granted to copy, distribute, or create derivative works from
// * the contents of this software, in whole or in part, without the prior written
// * permission of create3000.
// *
// * NON-MILITARY USE ONLY
// *
// * All create3000 software are effectively free software with a non-military use
// * restriction. It is free. Well commented source is provided. You may reuse the
// * source in any way you please with the exception anything that uses it must be
// * marked to indicate is contains 'non-military use only' components.
// *
// * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
// *
// * Copyright 1999, 2012 Holger Seelig <holger.seelig@yahoo.de>.
// *
// * This file is part of the Titania Project.
// *
// * Titania is free software: you can redistribute it and/or modify it under the
// * terms of the GNU General Public License version 3 only, as published by the
// * Free Software Foundation.
// *
// * Titania is distributed in the hope that it will be useful, but WITHOUT ANY
// * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// * A PARTICULAR PURPOSE. See the GNU General Public License version 3 for more
// * details (a copy is included in the LICENSE file that accompanied this code).
// *
// * You should have received a copy of the GNU General Public License version 3
// * along with Titania.  If not, see <http://www.gnu.org/licenses/gpl.html> for a
// * copy of the GPLv3 License.
// *
// * For Silvio, Joy and Adi.
// *
// ******************************************************************************/
//
//#ifndef __TITANIA_X3D_FIELDS_X3DPTR_H__
//#define __TITANIA_X3D_FIELDS_X3DPTR_H__
//
//#include "../Basic/X3DField.h"
//
//namespace titania {
//namespace X3D {
//
//class X3DPtrBase :
//	virtual public X3DBase
//{
//public:
//
//	///  @name Operations
//
//	virtual
//	X3DChildObject*
//	getObject () const
//	throw (Error <DISPOSED>) = 0;
//
//
//protected:
//
//	X3DPtrBase () :
//		X3DBase ()
//	{ }
//
//};
//
//template <class ValueType>
//class X3DPtr :
//	public X3DField <ValueType*>, public X3DPtrBase
//{
//public:
//
//	typedef ValueType* internal_type;
//	typedef ValueType* value_type;
//
//	using X3DField <ValueType*>::addEvent;
//	using X3DField <ValueType*>::operator =;
//	using X3DField <ValueType*>::addInterest;
//	using X3DField <ValueType*>::setValue;
//	using X3DField <ValueType*>::getValue;
//
//	///  @name Constructors
//
//	X3DPtr () :
//		X3DField <ValueType*> (nullptr)
//	{ }
//
//	X3DPtr (const X3DPtr & field) :
//		X3DPtr (field .getValue ())
//	{ }
//
//	explicit
//	X3DPtr (const X3DPtrBase & field) :
//		X3DPtr (dynamic_cast <ValueType*> (field .getObject ()))
//	{ }
//
//	X3DPtr (X3DPtr && field) :
//		X3DField <ValueType*> (field .getValue ())
//	{
//		if (getValue ())
//		{
//			getValue () -> replaceParent (&field, this);
//			field .reset ();
//			field .addEvent ();
//		}
//	}
//
//	template <class Up>
//	explicit
//	X3DPtr (X3DPtr <Up>&& field) :
//		X3DField <ValueType*> (dynamic_cast <ValueType*> (field .getValue ()))
//	{
//		if (getValue ())
//		{
//			getValue () -> replaceParent (&field, this);
//			field .reset ();
//			field .addEvent ();
//		}
//		else
//			field = nullptr;
//	}
//
//	//explicit
//	X3DPtr (ValueType* const value) :
//		X3DField <ValueType*> (value)
//	{
//		if (value)
//			value -> addParent (this);
//	}
//
//	template <class Up>
//	explicit
//	X3DPtr (Up* const value) :
//		X3DPtr (dynamic_cast <ValueType*> (value))
//	{ }
//
//	///  @name Construction
//
//	virtual
//	X3DPtr*
//	create () const final override
//	{ return new X3DPtr (); }
//
//	virtual
//	X3DPtr*
//	clone () const
//	throw (Error <INVALID_NAME>,
//	       Error <NOT_SUPPORTED>) final override
//	{ return new X3DPtr (*this); }
//
//	virtual
//	X3DPtr*
//	clone (X3DExecutionContext* const) const
//	throw (Error <INVALID_NAME>,
//	       Error <NOT_SUPPORTED>) final override;
//
//	virtual
//	void
//	clone (X3DExecutionContext* const, X3DFieldDefinition*) const
//	throw (Error <INVALID_NAME>,
//	       Error <NOT_SUPPORTED>) final override;
//
//	/// @name Assignment operators
//
//	///  Default assignment operator.  Behaves the same as the 6.7.6 setValue service.
//	X3DPtr &
//	operator = (const X3DPtr &);
//
//	X3DPtr &
//	operator = (const X3DPtrBase &);
//
//	///  Move assignment operator.  Behaves the same as the 6.7.6 setValue service.
//	X3DPtr &
//	operator = (X3DPtr &&);
//
//	template <class Up>
//	X3DPtr &
//	operator = (X3DPtr <Up>&&);
//
//	template <class Up>
//	X3DPtr &
//	operator = (Up* const);
//
//	///  @name Field services
//
//	virtual
//	X3DConstants::FieldType
//	getType () const final override
//	{ return X3DConstants::SFNode; }
//
//	virtual
//	const std::string &
//	getTypeName () const
//	throw (Error <DISPOSED>) final override
//	{ return typeName; }
//
//	///  @name Set value services
//
//	virtual
//	void
//	set (const internal_type &) final override;
//
//	virtual
//	void
//	write (const X3DChildObject &) final override;
//
//	virtual
//	X3DChildObject*
//	getObject () const
//	throw (Error <DISPOSED>) final override
//	{ return getValue (); }
//
//	///  @name Boolean operator
//
//	operator bool () const
//	{ return getValue (); }
//
//	///  @name Access operators
//
//	ValueType*
//	operator -> () const
//	{ return getValue (); }
//
//	ValueType &
//	operator * () const
//	{ return *getValue (); }
//
//	///  @name 6.7.7 Add field interest.
//
//	template <class Class>
//	void
//	addInterest (Class* const object, void (Class::* memberFunction) (const X3DPtr &)) const
//	{ addInterest (object, memberFunction, std::cref (*this)); }
//
//	template <class Class>
//	void
//	addInterest (Class & object, void (Class::* memberFunction) (const X3DPtr &)) const
//	{ addInterest (object, memberFunction, std::cref (*this)); }
//
//	///  @name Input/Output
//	virtual
//	void
//	fromStream (std::istream &)
//	throw (Error <INVALID_X3D>,
//	       Error <NOT_SUPPORTED>,
//	       Error <INVALID_OPERATION_TIMING>,
//	       Error <DISPOSED>) final override;
//
//	virtual
//	void
//	toStream (std::ostream &) const final override;
//
//	virtual
//	void
//	toXMLStream (std::ostream &) const final override;
//
//	///  @name Dispose
//
//	virtual
//	void
//	dispose () final override;
//
//	virtual
//	~X3DPtr ();
//
//
//private:
//
//	template <class Up>
//	friend class X3DPtr;
//
//	using X3DField <ValueType*>::reset;
//
//	void
//	addObject (ValueType* const);
//
//	void
//	removeObject (ValueType* const);
//
//	///  TypeName identifer for X3DFields.
//	static const std::string typeName;
//
//};
//
//template <class ValueType>
//const std::string X3DPtr <ValueType>::typeName ("SFNode");
//
//template <class ValueType>
//X3DPtr <ValueType>*
//X3DPtr <ValueType>::clone (X3DExecutionContext* const executionContext) const
//throw (Error <INVALID_NAME>,
//       Error <NOT_SUPPORTED>)
//{
//	X3DPtr* const field = new X3DPtr ();
//
//	clone (executionContext, field);
//
//	return field;
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::clone (X3DExecutionContext* const executionContext, X3DFieldDefinition* fieldDefinition) const
//throw (Error <INVALID_NAME>,
//       Error <NOT_SUPPORTED>)
//{
//	X3DPtr* const field = static_cast <X3DPtr*> (fieldDefinition);
//
//	if (getValue ())
//		field -> set (dynamic_cast <ValueType*> (getValue () -> clone (executionContext)));
//
//	else
//		field -> set (nullptr);
//}
//
//template <class ValueType>
//inline
//X3DPtr <ValueType> &
//X3DPtr <ValueType>::operator = (const X3DPtr & field)
//{
//	X3DField <ValueType*>::operator = (field);
//	return *this;
//}
//
//template <class ValueType>
//inline
//X3DPtr <ValueType> &
//X3DPtr <ValueType>::operator = (const X3DPtrBase & field)
//{
//	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (field .getObject ()));
//	return *this;
//}
//
//template <class ValueType>
//inline
//X3DPtr <ValueType> &
//X3DPtr <ValueType>::operator = (X3DPtr && field)
//{
//	if (&field == this)
//		return *this;
//
//	removeObject (getValue ());
//
//	X3DField <ValueType*>::set (field .getValue ());
//
//	if (getValue ())
//	{
//		getValue () -> replaceParent (&field, this);
//		field .reset ();
//		field .addEvent ();
//	}
//
//	addEvent ();
//
//	return *this;
//}
//
//template <class ValueType>
//template <class Up>
//inline
//X3DPtr <ValueType> &
//X3DPtr <ValueType>::operator = (X3DPtr <Up>&& field)
//{
//	if (&field == this)
//		return *this;
//
//	removeObject (getValue ());
//
//	X3DField <ValueType*>::set (dynamic_cast <ValueType*> (field .getValue ()));
//
//	if (getValue ())
//	{
//		getValue () -> replaceParent (&field, this);
//		field .reset ();
//		field .addEvent ();
//	}
//	else
//		field = nullptr;
//
//	addEvent ();
//
//	return *this;
//}
//
//template <class ValueType>
//template <class Up>
//inline
//X3DPtr <ValueType> &
//X3DPtr <ValueType>::operator = (Up* const value)
//{
//	X3DField <ValueType*>::operator = (dynamic_cast <ValueType*> (value));
//	return *this;
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::set (const internal_type & value)
//{
//	addObject (value);
//	X3DField <ValueType*>::set (value);
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::write (const X3DChildObject & field)
//{
//	X3DChildObject* const object = dynamic_cast <const X3DPtrBase &> (field) .getObject ();
//
//	set (dynamic_cast <internal_type> (object));
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::addObject (ValueType* const value)
//{
//	if (getValue () not_eq value)
//	{
//		if (value)
//			value -> addParent (this);
//
//		removeObject (getValue ());
//	}
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::removeObject (ValueType* const value)
//{
//	if (value)
//	{
//		reset ();
//
//		value -> removeParent (this);
//	}
//}
//
//template <class ValueType>
//inline
//void
//X3DPtr <ValueType>::fromStream (std::istream & istream)
//throw (Error <INVALID_X3D>,
//       Error <NOT_SUPPORTED>,
//       Error <INVALID_OPERATION_TIMING>,
//       Error <DISPOSED>)
//{ }
//
//template <class ValueType>
//inline
//void
//X3DPtr <ValueType>::toStream (std::ostream & ostream) const
//{
//	if (getValue ())
//		getValue () -> toStream (ostream);
//
//	else
//		ostream << "NULL";
//}
//
//template <class ValueType>
//inline
//void
//X3DPtr <ValueType>::toXMLStream (std::ostream & ostream) const
//{
//	if (getValue ())
//		getValue () -> toXMLStream (ostream);
//
//	else
//		ostream << "NULL";
//}
//
//template <class ValueType>
//void
//X3DPtr <ValueType>::dispose ()
//{
//	removeObject (getValue ());
//
//	X3DField <ValueType*>::dispose ();
//}
//
//template <class ValueType>
//inline
//X3DPtr <ValueType>::~X3DPtr ()
//{
//	removeObject (getValue ());
//}
//
/////  @relates X3DPtr
/////  @name Comparision operations
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs is equal to @a rhs.
//template <class ValueType>
//inline
//bool
//operator == (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a == b;
//}
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs is not equal to @a rhs.
//template <class ValueType>
//inline
//bool
//operator not_eq (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a not_eq b;
//}
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs less than @a rhs.
//template <class ValueType>
//inline
//bool
//operator < (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a < b;
//}
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs less than equal to @a rhs.
//template <class ValueType>
//inline
//bool
//operator <= (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a <= b;
//}
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs greater than @a rhs.
//template <class ValueType>
//inline
//bool
//operator > (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a > b;
//}
//
/////  Compares two X3DPtr.
/////  Returns true if @a lhs greater than equal to @a rhs.
//template <class ValueType>
//inline
//bool
//operator >= (const X3DPtr <ValueType> & lhs, const X3DPtr <ValueType> & rhs)
//{
//	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
//	X3DBase* const b = rhs ? rhs -> getId () : nullptr;
//
//	return a >= b;
//}
//
//} // X3D
//} // titania
//
//#endif















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

#ifndef __TITANIA_X3D_FIELDS_X3DPTR_H__
#define __TITANIA_X3D_FIELDS_X3DPTR_H__

#include "../Basic/X3DField.h"

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
 *  library.  Results with any other X3DChildObject type are guaranteed.
 *
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class X3DPtr :
	public X3DField <ValueType*>, public X3DPtrBase
{
public:

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
		X3DField <ValueType*> (nullptr)
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
		X3DField <ValueType*> ()
	{ moveObject (field); }

	///  Constructs new X3DPtr.
	template <class Up>
	explicit
	X3DPtr (X3DPtr <Up> && field) :
		X3DField <ValueType*> ()
	{ moveObject (field); }

	///  Constructs new X3DPtr.
	//explicit
	X3DPtr (ValueType* const value) :
		X3DField <ValueType*> (value)
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

	/// @name Assignment operators

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
	operator = (X3DPtr && field)
	{
		if (&field == this)
			return *this;

		removeObject (getValue ());
		moveObject (field);
		addEvent ();

		return *this;
	}

	///  Assigns the X3DPtr and propagates an event.
	template <class Up>
	X3DPtr &
	operator = (X3DPtr <Up> && field)
	{
		if (&field == this)
			return *this;

		removeObject (getValue ());
		moveObject (field);
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
		if (value not_eq getValue ())
		{
			// First add Object to avoid dispose.
			addObject (value);
			removeObject (getValue ());
		}

		setObject (value);
	}

	///  Assigns the X3DPtr without propagating an event.
	virtual
	void
	write (const X3DChildObject & field) final override
	{
		X3DChildObject* const object = dynamic_cast <const X3DPtrBase &> (field) .getObject ();

		set (dynamic_cast <internal_type> (object));
	}

	///  @name Observers

	ValueType*
	operator -> () const
	{ return getValue (); }

	ValueType &
	operator * () const
	{ return *getValue (); }

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

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DPtr &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

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

	///  Inserts this object as Classic VRML into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (getValue ())
			getValue () -> toStream (ostream);

		else
			ostream << "NULL";
	}

	///  Inserts this object as XML into the output stream @a ostream.
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
			value -> disposed () .addInterest (this, &X3DPtr::set_disposed);
		}
	}

	void
	moveObject (X3DPtr & field)
	{
		setObject (field .getValue ());

		if (getValue ())
		{
			field .get () -> replaceParent (&field, this);
			field .get () -> disposed () .addInterest (this, &X3DPtr::set_disposed);
			field .setObject (nullptr);
			field .addEvent ();
		}
	}

	template <class Up>
	void
	moveObject (X3DPtr <Up> & field)
	{
		setObject (dynamic_cast <ValueType*> (field .getValue ()));

		if (getValue ())
		{
			field .get () -> replaceParent (&field, this);
			field .get () -> disposed () .addInterest (this, &X3DPtr::set_disposed);
			field .setObject (nullptr);
			field .addEvent ();
		}
		else
			field = nullptr;
	}

	void
	removeObject (ValueType* const value)
	{
		if (value)
		{
			setObject (nullptr);

			value -> removeParent (this);
			value -> disposed () .removeInterest (this, &X3DPtr::set_disposed);
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
	{ setObject (nullptr); }

	///  @name Static members

	static const std::string typeName;

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
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a == b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs is not equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator not_eq (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a not_eq b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs less than @a rhs.
template <class LHS, class RHS>
inline
bool
operator < (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a < b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator <= (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a <= b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs greater than @a rhs.
template <class LHS, class RHS>
inline
bool
operator > (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a > b;
}

///  Compares two X3DPtr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class LHS, class RHS>
inline
bool
operator >= (const X3DPtr <LHS> & lhs, const X3DPtr <RHS> & rhs)
{
	X3DBase* const a = lhs ? lhs -> getId () : nullptr;
	X3DBase* const b = rhs ? rhs -> getId () : nullptr;

	return a >= b;
}

} // X3D
} // titania

#endif
