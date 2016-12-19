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

#ifndef __TITANIA_X3D_FIELDS_SFSTRING_H__
#define __TITANIA_X3D_FIELDS_SFSTRING_H__

#include "../Basic/X3DField.h"
#include "../Types/String.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

extern template class X3DField <String>;

/**
 *  Class to represent a X3D SFString field.
 */
class SFString :
	public X3DField <String>
{
public:

	typedef std::string::value_type   char_type;
	typedef internal_type::value_type value_type;
	typedef internal_type::size_type  size_type;

	using X3DField <String>::addInterest;
	using X3DField <String>::operator =;

	///  @name Construction

	SFString ();

	SFString (const SFString &);

	SFString (SFString &&);

	explicit
	SFString (const String &);

	explicit
	SFString (String &&);

	explicit
	SFString (const char_type* const);

	virtual
	SFString*
	create () const final override
	{ return new SFString (); }

	virtual
	SFString*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override
	{ return new SFString (*this); }

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFString &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFString &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Assignment operators

	SFString &
	operator = (const SFString & field)
	{
		setValue (field);
		return *this;
	}

	SFString &
	operator = (SFString &&);

	SFString &
	operator = (const String & value)
	{
		setValue (value);
		return *this;
	}

	SFString &
	operator = (String &&);

	SFString &
	operator = (const char_type* const);

	operator const std::string & () const;

	///  @name Element access

	const std::string &
	str () const;

	const char_type*
	c_str () const;

	///  @name Capacity

	bool
	empty () const;

	size_type
	length () const;

	///  @name Operations

	void
	clear ();

	SFString &
	append (const String &);

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

	virtual
	void
	toJSONStream (std::ostream &) const final override;


protected:

	friend class X3DArrayField <SFString>;

	void
	toJSONStreamValue (std::ostream & ostream) const
	{ toJSONStream (ostream); }


private:

	using X3DField <String>::get;

};


///  @relates SFString
///  @name Comparision operations

///  Compares two SFString values.
///  Returns true if @a lhs is equal to @a rhs.
inline
bool
operator == (const SFString & lhs, const char* rhs)
{
	return lhs .getValue () == rhs;
}

///  Compares two SFString values.
///  Returns true if @a lhs is equal to @a rhs.
inline
bool
operator == (const char* lhs, const SFString & rhs)
{
	return lhs == rhs .getValue ();
}

///  Compares two SFString values.
///  Returns true if @a lhs is not equal to @a rhs.
inline
bool
operator not_eq (const SFString & lhs, const char* rhs)
{
	return lhs .getValue () not_eq rhs;
}

///  Compares two SFString values.
///  Returns true if @a lhs is not equal to @a rhs.
inline
bool
operator not_eq (const char* lhs, const SFString & rhs)
{
	return lhs not_eq rhs .getValue ();
}


} // X3D
} // titania

#endif
