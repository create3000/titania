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

#ifndef __TITANIA_X3D_FIELDS_X3DPTR_ARRAY_H__
#define __TITANIA_X3D_FIELDS_X3DPTR_ARRAY_H__

#include "../Basic/X3DArrayField.h"
#include "../Fields/X3DPtr.h"
#include "../InputOutput/Generator.h"

#include <Titania/Utility/Range.h>

namespace titania {
namespace X3D {

template <class ValueType>
class X3DPtrArray :
	public X3DArrayField <X3DPtr <ValueType>> 
{
private:

	typedef X3DArrayField <X3DPtr <ValueType>>  ArrayField;


public:

	/***
	 *  @name Member types
	 */

	typedef typename ArrayField::value_type value_type;

	using X3DArrayField <X3DPtr <ValueType>> ::operator =;
	using X3DArrayField <X3DPtr <ValueType>> ::getValue;
	using X3DArrayField <X3DPtr <ValueType>> ::front;
	using X3DArrayField <X3DPtr <ValueType>> ::back;
	using X3DArrayField <X3DPtr <ValueType>> ::cbegin;
	using X3DArrayField <X3DPtr <ValueType>> ::cend;
	using X3DArrayField <X3DPtr <ValueType>> ::empty;
	using X3DArrayField <X3DPtr <ValueType>> ::size;
	using X3DArrayField <X3DPtr <ValueType>> ::addInterest;

	/***
	 *  @name Construction
	 */

	///  Constructs new X3DPtrArray.
	X3DPtrArray () :
		ArrayField (),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	X3DPtrArray (const X3DPtrArray & field) :
		ArrayField (field),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	X3DPtrArray (X3DPtrArray && field) :
		ArrayField (std::move (field)),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	template <class Up>
	explicit
	X3DPtrArray (const X3DPtrArray <Up> & field) :
		ArrayField (field .begin (), field .end ()),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	template <class Up>
	explicit
	X3DPtrArray (X3DPtrArray <Up> &&);

	///  Constructs new X3DPtrArray.
	X3DPtrArray (std::initializer_list <X3DPtr <ValueType>>  initializer_list) :
		ArrayField (initializer_list),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	X3DPtrArray (std::initializer_list <const typename X3DPtr <ValueType>::internal_type> initializer_list) :
		ArrayField (initializer_list),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	template <class InputIterator>
	X3DPtrArray (InputIterator first, InputIterator last) :
		ArrayField (first, last),
		cloneCount (0)
	{ }

	///  Constructs new X3DPtrArray.
	virtual
	X3DPtrArray*
	create () const final override
	{ return new X3DPtrArray (); }

	///  Constructs new X3DPtrArray.
	virtual
	X3DPtrArray*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  Constructs new X3DPtrArray.
	virtual
	X3DPtrArray*
	copy (X3DExecutionContext* const executionContext, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  Constructs new X3DPtrArray.
	virtual
	void
	copy (X3DExecutionContext* const, X3DFieldDefinition* const, const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	/***
	 *  @name Assignment operators
	 */

	///  Assigns the X3DPtrArray and propagates an event.
	X3DPtrArray &
	operator = (const X3DPtrArray & field)
	{
		ArrayField::operator = (field);
		return *this;
	}

	///  Assigns the X3DPtrArray and propagates an event.
	X3DPtrArray &
	operator = (X3DPtrArray && field)
	{
		ArrayField::operator = (std::move (field));
		return *this;
	}

	///  Assigns the X3DPtrArray and propagates an event.
	template <class Up>
	X3DPtrArray &
	operator = (X3DPtrArray <Up> &&);

	/**
	 *  @name Common members
	 */

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
	{ return X3DConstants::MFNode; }

	/**
	 *  @name Clone handling
	 */

	virtual
	void
	addClones (const size_t count) final override
	{
		cloneCount += count;

		for (const auto & value : getValue ())
			value -> addClones (count);
	}

	virtual
	void
	removeClones (const size_t count) final override
	{
		cloneCount -= count;

		for (const auto & value : getValue ())
			value -> removeClones (count);
	}

	/**
	 *  @name Interest service
	 */

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DPtrArray &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DPtrArray &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	/**
	 *  @name Input/Output
	 */

	///  Not supported.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream &) const final override;

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream &) const final override;


private:

	using X3DArrayField <X3DPtr <ValueType>> ::get;

	/***
	 *  @name Element handling
	 */

	virtual
	void
	addChild (value_type* value) final override
	{
		ArrayField::addChild (value);

		value -> addClones (cloneCount);
	}

	virtual
	void
	moveChild (ArrayField & other, value_type* value) final override
	{
		ArrayField::moveChild (other, value);

		value -> addClones (cloneCount);
		value -> removeClones (other .getCloneCount ());
	}

	virtual
	void
	removeChild (value_type* value) final override
	{
		value -> removeClones (cloneCount);

		ArrayField::removeChild (value);
	}

	virtual
	size_t
	getCloneCount () const final override
	{ return cloneCount; }


private:

	/**
	 *  @name Static members
	 */

	static const std::string typeName;

	/**
	 *  @name Members
	 */

	size_t cloneCount;

};

template <class ValueType>
const std::string X3DPtrArray <ValueType>::typeName ("MFNode");

template <class ValueType>
template <class Up>
X3DPtrArray <ValueType>::X3DPtrArray (X3DPtrArray <Up> && field) :
	ArrayField (),
	cloneCount (0)
{
	auto       first = field .begin ();
	const auto last  = field .end ();

	// Insert at end

	for (; first not_eq last; ++ first)
	{
		ValueType* const field = new ValueType (std::move (*first));

		get () .emplace_back (field);

		addChild (field);
	}

	field .clear ();
}

template <class ValueType>
X3DPtrArray <ValueType>*
X3DPtrArray <ValueType>::copy (const CopyType type) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	if (type == FLAT_COPY)
		return new X3DPtrArray (*this);

	const auto field = new X3DPtrArray ();

	for (const auto & value : *this)
	{
		if (value)
			field -> emplace_back (value -> copy (type));
		else
			field -> emplace_back ();
	}

	return field;
}

template <class ValueType>
X3DPtrArray <ValueType>*
X3DPtrArray <ValueType>::copy (X3DExecutionContext* const executionContext, const CopyType type) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DPtrArray* const field = new X3DPtrArray ();

	copy (executionContext, field, type);

	return field;
}

template <class ValueType>
void
X3DPtrArray <ValueType>::copy (X3DExecutionContext* const executionContext, X3DFieldDefinition* const fieldDefinition, const CopyType type) const
throw (Error <INVALID_NAME>,
       Error <NOT_SUPPORTED>)
{
	X3DPtrArray* const field = static_cast <X3DPtrArray*> (fieldDefinition);

	if (type == FLAT_COPY)
	{
		*field = *this;
	}
	else
	{
		for (const auto & value : *this)
		{
			if (value)
				field -> emplace_back (value -> copy (executionContext, type));

			else
				field -> emplace_back ();
		}
	}
}

template <class ValueType>
template <class Up>
X3DPtrArray <ValueType> &
X3DPtrArray <ValueType>::operator = (X3DPtrArray <Up> && field)
{
	auto       first = field .begin ();
	const auto last  = field .end ();

	auto current = this -> begin ();

	for (const auto end = this -> end (); first not_eq last && current not_eq end; ++ current, ++ first)
		*current = std::move (*first);

	if (first == last)
	{
		// Remove trailing fields

		const size_t count = current - this -> begin ();

		removeChildren (get () .begin () + count, get () .end ());

		get () .resize (count);
	}
	else
	{
		// Insert at end

		for (; first not_eq last; ++ first)
		{
			ValueType* const field = new ValueType (std::move (*first));

			get () .emplace_back (field);

			addChild (field);
		}
	}

	field .clear ();

	return *this;
}

template <class ValueType>
void
X3DPtrArray <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

template <class ValueType>
void
X3DPtrArray <ValueType>::toStream (std::ostream & ostream) const
{
	switch (size ())
	{
		case 0:
		{
			ostream << Generator::EmptyBrackets;
			return;
		}
		case 1:
		{
			Generator::PushContext ();
			ostream << front ();
			Generator::PopContext ();

			return;
		}
		default:
		{
			Generator::PushContext ();

			ostream
				<< '['
				<< Generator::TidyBreak
				<< Generator::IncIndent;

			for (const auto & field : std::make_pair (cbegin (), cend () - 1))
			{
				ostream
					<< Generator::Indent
					<< field
					<< Generator::Break;
			}

			ostream
				<< Generator::Indent
				<< back ()
				<< Generator::TidyBreak
				<< Generator::DecIndent
				<< Generator::Indent
				<< ']';

			Generator::PopContext ();

			return;
		}
	}
}

template <class ValueType>
void
X3DPtrArray <ValueType>::toXMLStream (std::ostream & ostream) const
{
	if (not empty ())
	{
		Generator::PushContext ();

		for (const auto & value : std::make_pair (cbegin (), cend () - 1))
		{
			if (value)
			{
				ostream
					<< XMLEncode (value)
					<< Generator::Break;
			}
			else
			{
				ostream
					<< Generator::Indent
					<< "<!-- NULL -->"
					<< Generator::Break;
			}
		}

		if (back ())
			ostream << XMLEncode (back ());

		else
		{
			ostream
				<< Generator::Indent
				<< "<!-- NULL -->";
		}

		Generator::PopContext ();
	}
}

} // X3D
} // titania

#endif
