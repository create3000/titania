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

#ifndef __TITANIA_X3D_FIELDS_X3DSCALAR_H__
#define __TITANIA_X3D_FIELDS_X3DSCALAR_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

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
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class X3DScalar :
	public X3DField <ValueType>
{
public:

	typedef ValueType value_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::addEvent;
	using X3DField <ValueType>::operator =;
	using X3DField <ValueType>::getValue;

	///  @name Construction

	X3DScalar () :
		X3DField <ValueType> () { }

	X3DScalar (const X3DScalar & field) :
		X3DField <ValueType> (field) { }

	explicit
	X3DScalar (const ValueType & value) :
		X3DField <ValueType> (value) { }

	virtual
	X3DScalar*
	create () const final override
	{ return new X3DScalar (); }

	virtual
	X3DScalar*
	clone () const
	throw (Error <NOT_SUPPORTED>) final override
	{ return new X3DScalar (*this); }

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DScalar &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DScalar &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (ValueType)) const
	{ addInterest (object, memberFunction, std::cref (this -> getValue ())); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (ValueType)) const
	{ addInterest (object, memberFunction, std::cref (this -> getValue ())); }

	///  @name Arithmetic operations

	X3DScalar &
	operator += (const X3DScalar &);

	X3DScalar &
	operator += (const ValueType &);

	X3DScalar &
	operator -= (const X3DScalar &);

	X3DScalar &
	operator -= (const ValueType &);

	X3DScalar &
	operator *= (const X3DScalar &);

	X3DScalar &
	operator *= (const ValueType &);

	X3DScalar &
	operator /= (const X3DScalar &);

	X3DScalar &
	operator /= (const ValueType &);

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ }

	virtual
	void
	toStream (std::ostream &) const final override
	{ }

	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ }


private:

	using X3DField <ValueType>::get;

};

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator += (const X3DScalar & value)
{
	get () += value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator += (const ValueType & value)
{
	get () += value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator -= (const X3DScalar & value)
{
	get () -= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator -= (const ValueType & value)
{
	get () -= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator *= (const X3DScalar & value)
{
	get () *= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator *= (const ValueType & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator /= (const X3DScalar & value)
{
	get () /= value .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
X3DScalar <ValueType> &
X3DScalar <ValueType>::operator /= (const ValueType & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

///  @relates X3DScalar
///  @name Aritmetic operators.

template <class ValueType>
inline
ValueType
operator - (const X3DScalar <ValueType> & scalar)
{
	return -scalar .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const X3DScalar <ValueType> & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs .getValue () + rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const X3DScalar <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () + rhs;
}

template <class ValueType>
inline
ValueType
operator + (const ValueType & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs + rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator - (const X3DScalar <ValueType> & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs .getValue () - rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator - (const X3DScalar <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () - rhs;
}

template <class ValueType>
inline
ValueType
operator - (const ValueType & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs - rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const X3DScalar <ValueType> & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs .getValue () * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const X3DScalar <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () * rhs;
}

template <class ValueType>
inline
ValueType
operator * (const ValueType & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator / (const X3DScalar <ValueType> & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs .getValue () / rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator / (const X3DScalar <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () / rhs;
}

template <class ValueType>
inline
ValueType
operator / (const ValueType & lhs, const X3DScalar <ValueType> & rhs)
{
	return lhs / rhs .getValue ();
}

template <>
void
X3DScalar <bool>::fromStream (std::istream &)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>);

template <>
void
X3DScalar <double>::fromStream (std::istream &)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>);

template <>
void
X3DScalar <float>::fromStream (std::istream &)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>);

template <>
void
X3DScalar <int32_t>::fromStream (std::istream &)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>);

template <>
void
X3DScalar <bool>::toStream (std::ostream &) const;

template <>
void
X3DScalar <double>::toStream (std::ostream &) const;

template <>
void
X3DScalar <float>::toStream (std::ostream &) const;

template <>
void
X3DScalar <int32_t>::toStream (std::ostream &) const;

template <>
void
X3DScalar <bool>::toXMLStream (std::ostream &) const;

template <>
void
X3DScalar <double>::toXMLStream (std::ostream &) const;

template <>
void
X3DScalar <float>::toXMLStream (std::ostream &) const;

template <>
void
X3DScalar <int32_t>::toXMLStream (std::ostream &) const;

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
