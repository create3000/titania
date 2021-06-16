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

#ifndef __TITANIA_X3D_FIELDS_X3DSCALAR_H__
#define __TITANIA_X3D_FIELDS_X3DSCALAR_H__

#include "../Basic/X3DField.h"
#include "../InputOutput/Generator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

#ifdef __APPLE__
template <>
const std::string X3DField <bool>::typeName;

template <>
const std::string X3DField <double>::typeName;

template <>
const std::string X3DField <float>::typeName;

template <>
const std::string X3DField <int32_t>::typeName;

template <>
const FieldType X3DField <bool>::type;

template <>
const FieldType X3DField <double>::type;

template <>
const FieldType X3DField <float>::type;

template <>
const FieldType X3DField <int32_t>::type;
#endif

extern template class X3DField <bool>;
extern template class X3DField <double>;
extern template class X3DField <float>;
extern template class X3DField <int32_t>;

/**
 *  Template to represent scalar value fields. This is the base class for SFBool, SFDouble, SFFloat and SFInt32 fields.
 *
 *  Extern instantiations for bool, float, double and int32_t are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  InternalType  Type of the internal value of the field.
 */
template <class InternalType>
class X3DScalar :
	public X3DField <InternalType>
{
public:

	using value_type = InternalType;

	using X3DField <InternalType>::addInterest;
	using X3DField <InternalType>::addEvent;
	using X3DField <InternalType>::copy;
	using X3DField <InternalType>::operator =;
	using X3DField <InternalType>::getValue;
	using X3DField <InternalType>::getUnit;

	///  @name Construction

	X3DScalar () :
		X3DField <InternalType> ()
	{ }

	X3DScalar (const X3DScalar & field) :
		X3DField <InternalType> (field)
	{ }

	explicit
	X3DScalar (const InternalType & value) :
		X3DField <InternalType> (value)
	{ }

	virtual
	X3DScalar*
	create () const final override
	{ return new X3DScalar (); }

	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DScalar*
	copy (const CopyType) const final override
	{ return new X3DScalar (*this); }

	///  @name Interest service

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DScalar &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DScalar &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const InternalType), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const InternalType), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Arithmetic operations

	template <class T = InternalType>
	std::enable_if_t <
		std::is_arithmetic_v <T>,
		InternalType
	>
	operator ++ ()
	{
		++ get ();
		addEvent ();
		return getValue ();
	}

	template <class T = InternalType>
	std::enable_if_t <
		std::is_arithmetic_v <T>,
		InternalType
	>
	operator ++ (int)
	{
		const auto tmp = getValue ();
		++ get ();
		addEvent ();
		return tmp;
	}

	template <class T = InternalType>
	std::enable_if_t <
		std::is_arithmetic_v <T>,
		InternalType
	>
	operator -- ()
	{
		-- get ();
		addEvent ();
		return getValue ();
	}

	template <class T = InternalType>
	std::enable_if_t <
		std::is_arithmetic_v <T>,
		InternalType
	>
	operator -- (int)
	{
		const auto tmp = getValue ();
		-- get ();
		addEvent ();
		return tmp;
	}

	template <class T = InternalType>
	std::enable_if_t <
		std::is_integral_v <T>,
		X3DScalar &
	>
	operator <<= (const InternalType & value)
	{
		get () <<= value;
		addEvent ();
		return *this;
	}

	template <class T = InternalType>
	std::enable_if_t <
		std::is_integral_v <T>,
		X3DScalar &
	>
	operator >>= (const InternalType & value)
	{
		get () >>= value;
		addEvent ();
		return *this;
	}

	X3DScalar &
	operator += (const InternalType &);

	X3DScalar &
	operator -= (const InternalType &);

	X3DScalar &
	operator *= (const InternalType &);

	X3DScalar &
	operator /= (const InternalType &);

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream & istream) final override
	{ }

	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ VRMLGenerator::Encode (ostream, getValue (), getUnit ()); }

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override
	{ XMLGenerator::Encode (ostream, getValue (), getUnit ()); }

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override
	{ JSONGenerator::Encode (ostream, getValue (), getUnit ()); }


private:

	using X3DField <InternalType>::get;

};

template <class InternalType>
inline
X3DScalar <InternalType> &
X3DScalar <InternalType>::operator += (const InternalType & value)
{
	get () += value;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
X3DScalar <InternalType> &
X3DScalar <InternalType>::operator -= (const InternalType & value)
{
	get () -= value;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
X3DScalar <InternalType> &
X3DScalar <InternalType>::operator *= (const InternalType & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
X3DScalar <InternalType> &
X3DScalar <InternalType>::operator /= (const InternalType & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

template <>
void
X3DScalar <bool>::fromStream (std::istream & istream);

template <>
void
X3DScalar <double>::fromStream (std::istream & istream);

template <>
void
X3DScalar <float>::fromStream (std::istream & istream);

template <>
void
X3DScalar <int32_t>::fromStream (std::istream & istream);

extern template class X3DScalar <bool>;
extern template class X3DScalar <double>;
extern template class X3DScalar <float>;
extern template class X3DScalar <int32_t>;

using SFBool   = X3DScalar <bool>;
using SFDouble = X3DScalar <double>;
using SFFloat  = X3DScalar <float>;
using SFInt32  = X3DScalar <int32_t>;

template <class Type>
using SFEnum = X3DScalar <Type>;

} // X3D
} // titania

#endif
