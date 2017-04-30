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

#ifndef __TITANIA_X3D_FIELDS_SFTIME_H__
#define __TITANIA_X3D_FIELDS_SFTIME_H__

#include "../Basic/X3DField.h"
#include "../Fields/X3DScalar.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField;

/**
 *  Class to represent a X3D SFTime field.
 */
class SFTime :
	public X3DField <time_type>
{
public:

	using value_type = time_type;

	using X3DField <time_type>::addInterest;
	using X3DField <time_type>::operator =;

	///  @name Construction

	SFTime ();

	SFTime (const SFTime &);

	explicit
	SFTime (const time_type);

	virtual
	SFTime*
	create () const final override
	{ return new SFTime (); }

	SFTime*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>)
	{ return new SFTime (*this); }

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFTime &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFTime &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const time_type), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const time_type), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Common members

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	X3DConstants::FieldType
	getType () const final override
	{ return type; }

	///  @name Arithmetic operations

	SFTime &
	operator += (const SFTime &);

	template <class ValueType>
	SFTime &
	operator += (const ValueType &);

	SFTime &
	operator -= (const SFTime &);

	template <class ValueType>
	SFTime &
	operator -= (const ValueType &);

	SFTime &
	operator *= (const SFTime &);

	template <class ValueType>
	SFTime &
	operator *= (const ValueType &);

	SFTime &
	operator /= (const SFTime &);

	template <class ValueType>
	SFTime &
	operator /= (const ValueType &);

	///  @name Operations

	static
	time_type
	now ();

	bool
	fromUTCString (const std::string & string);

	std::string
	toUTCString () const;

	///  @name Input/Output

	///  Parses a date according to std::get_time with locale support.
	bool
	fromLocaleString (const std::string & string, const std::string & format, const std::locale & locale);

	///  Parses a date according to std::get_time.
	void
	fromStream (std::istream & istream, const std::string & format);

	///  Converts the value of this field to a time string specified by format according to std::put_time.
	std::string
	toLocaleString (const std::string & format, const std::locale & locale) const;

	///  Converts the value of this field to a time string specified by format according to std::put_time.
	void
	toStream (std::ostream & ostream, const std::string & format) const;

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

	friend class X3DArrayField <SFTime>;

	void
	toJSONStreamValue (std::ostream & ostream) const
	{ toJSONStream (ostream); }


private:

	static const std::string typeName;

	static const X3DConstants::FieldType type;

};

inline
SFTime &
SFTime::operator += (const SFTime & value)
{
	get () += value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFTime &
SFTime::operator += (const ValueType & value)
{
	get () += value;
	addEvent ();
	return *this;
}

inline
SFTime &
SFTime::operator -= (const SFTime & value)
{
	get () -= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFTime &
SFTime::operator -= (const ValueType & value)
{
	get () -= value;
	addEvent ();
	return *this;
}

inline
SFTime &
SFTime::operator *= (const SFTime & value)
{
	get () *= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFTime &
SFTime::operator *= (const ValueType & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

inline
SFTime &
SFTime::operator /= (const SFTime & value)
{
	get () /= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFTime &
SFTime::operator /= (const ValueType & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

} // X3D
} // titania

#endif
