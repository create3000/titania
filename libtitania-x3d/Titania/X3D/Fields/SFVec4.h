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

#ifndef __TITANIA_X3D_FIELDS_SFVEC4_H__
#define __TITANIA_X3D_FIELDS_SFVEC4_H__

#include "../Basic/X3DField.h"
#include "../InputOutput/VRMLGenerator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Parser/MiniParser.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

extern template class X3DField <Vector4d>;
extern template class X3DField <Vector4f>;

/**
 *  Template to represent vector4 fields. This is the base class for SFVec4d and SFVec4f fields.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  InternalType  Type of the internal value of the field.
 */
template <class InternalType>
class SFVec4 :
	public X3DField <InternalType>
{
public:

	using value_type = typename InternalType::value_type;
	using size_type  = typename InternalType::size_type;

	using X3DField <InternalType>::addInterest;
	using X3DField <InternalType>::addEvent;
	using X3DField <InternalType>::setValue;
	using X3DField <InternalType>::getValue;
	using X3DField <InternalType>::getUnit;
	using X3DField <InternalType>::operator =;

	///  @name Construction

	/*
	 * These constructors are all trival and therefore made inline.
	 */

	SFVec4 () :
		X3DField <InternalType> ()
	{ }

	SFVec4 (const SFVec4 & other) :
		X3DField <InternalType> (other)
	{ }

	explicit
	SFVec4 (const InternalType & value) :
		X3DField <InternalType> (value)
	{ }

	SFVec4 (const value_type & x, const value_type & y, const value_type & z, const value_type & w) :
		X3DField <InternalType> (InternalType (x, y, z, w))
	{ }

	/*
	 * These function are all trival and therefore made inline.
	 */

	virtual
	SFVec4*
	create () const final override
	{ return new SFVec4 (); }

	virtual
	SFVec4*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFVec4 &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFVec4 &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Member access

	void
	setX (const value_type & value);

	value_type
	getX () const;

	void
	setY (const value_type & value);

	value_type
	getY () const;

	void
	setZ (const value_type & value);

	value_type
	getZ () const;

	void
	setW (const value_type & value);

	value_type
	getW () const;

	void
	set1Value (const size_type & index, const value_type & value);

	value_type
	get1Value (const size_type & index) const;

	value_type
	at (const size_type & index) const;

	value_type
	operator [ ] (const size_type & index) const;

	///  @name Arithmetic operations

	SFVec4 &
	operator += (const InternalType & vector);

	SFVec4 &
	operator -= (const InternalType & vector);

	SFVec4 &
	operator *= (const InternalType & vector);

	SFVec4 &
	operator *= (const value_type & vector);

	SFVec4 &
	operator /= (const InternalType & vector);

	SFVec4 &
	operator /= (const value_type & vector);

	InternalType
	negate () const;

	InternalType
	add (const InternalType & vector) const;

	InternalType
	subtract (const InternalType & vector) const;

	InternalType
	multiply (const InternalType & vector) const;

	InternalType
	multiply (const value_type & value) const;

	InternalType
	divide (const InternalType & vector) const;

	InternalType
	divide (const value_type & value) const;

	value_type
	dot (const InternalType & vector) const;

	InternalType
	normalize () const;

	value_type
	length () const;

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream & istream)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	virtual
	void
	toStream (std::ostream & ostream) const final override;

	virtual
	void
	toXMLStream (std::ostream & ostream) const final override;

	virtual
	void
	toJSONStream (std::ostream & ostream) const final override;


private:

	using X3DField <InternalType>::get;

};

template <class InternalType>
inline
SFVec4 <InternalType>*
SFVec4 <InternalType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFVec4 (*this);
}

template <class InternalType>
void
SFVec4 <InternalType>::setX (const value_type & x)
{
	get () .x (x);
	addEvent ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::getX () const
{
	return getValue () .x ();
}

template <class InternalType>
void
SFVec4 <InternalType>::setY (const value_type & y)
{
	get () .y (y);
	addEvent ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::getY () const
{
	return getValue () .y ();
}

template <class InternalType>
void
SFVec4 <InternalType>::setZ (const value_type & z)
{
	get () .z (z);
	addEvent ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::getZ () const
{
	return getValue () .z ();
}

template <class InternalType>
void
SFVec4 <InternalType>::setW (const value_type & w)
{
	get () .w (w);
	addEvent ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::getW () const
{
	return getValue () .w ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::at (const size_type & index) const
{
	return getValue () .at (index);
}

template <class InternalType>
void
SFVec4 <InternalType>::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = value;
	addEvent ();
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::operator [ ] (const size_type & index) const
{
	return getValue () [index];
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator += (const InternalType & vector)
{
	get () += vector;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator -= (const InternalType & vector)
{
	get () -= vector;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator *= (const InternalType & vector)
{
	get () *= vector;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator *= (const value_type & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator /= (const InternalType & vector)
{
	get () /= vector;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
SFVec4 <InternalType> &
SFVec4 <InternalType>::operator /= (const value_type & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::negate () const
{
	return math::negate (getValue ());
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::add (const InternalType & vector) const
{
	return getValue () + vector;
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::subtract (const InternalType & vector) const
{
	return getValue () - vector;
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::multiply (const InternalType & vector) const
{
	return getValue () * vector;
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::multiply (const value_type & value) const
{
	return getValue () * value;
}

template <class InternalType>
InternalType
SFVec4 <InternalType>::divide (const InternalType & vector) const
{
	return getValue () / vector;
}

template <class InternalType>
InternalType
SFVec4 <InternalType>::divide (const value_type & value) const
{
	return getValue () / value;
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::dot (const InternalType & vector) const
{
	return math::dot (getValue (), vector);
}

template <class InternalType>
inline
InternalType
SFVec4 <InternalType>::normalize () const
{
	return math::normalize (getValue ());
}

template <class InternalType>
inline
typename SFVec4 <InternalType>::value_type
SFVec4 <InternalType>::length () const
{
	return abs (getValue ());
}

template <class InternalType>
inline
void
SFVec4 <InternalType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	InternalType value;

	if (MiniParser::Decode (istream, value))
		setValue (value);
}

template <class InternalType>
inline
void
SFVec4 <InternalType>::toStream (std::ostream & ostream) const
{
	VRMLGenerator::Encode (ostream, getValue (), getUnit ());
}

template <class InternalType>
inline
void
SFVec4 <InternalType>::toXMLStream (std::ostream & ostream) const
{
	XMLGenerator::Encode (ostream, getValue (), getUnit ());
}

template <class InternalType>
inline
void
SFVec4 <InternalType>::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< Generator::TidySpace;

	JSONGenerator::Encode (ostream, getValue (), getUnit ());

	ostream
		<< Generator::TidySpace
		<< ']';
}

///  @relates SFVec4
///  @name Comparision operations

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than @a rhs.
template <class InternalType>
inline
bool
operator < (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () < rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than @a rhs.
template <class InternalType>
inline
bool
operator < (const InternalType& lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs < rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than @a rhs.
template <class InternalType>
inline
bool
operator < (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () < rhs;
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than equal to @a rhs.
template <class InternalType>
inline
bool
operator > (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () > rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than equal to @a rhs.
template <class InternalType>
inline
bool
operator > (const InternalType& lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs > rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs less than equal to @a rhs.
template <class InternalType>
inline
bool
operator > (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () > rhs;
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than @a rhs.
template <class InternalType>
inline
bool
operator <= (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () <= rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than @a rhs.
template <class InternalType>
inline
bool
operator <= (const InternalType& lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs <= rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than @a rhs.
template <class InternalType>
inline
bool
operator <= (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () <= rhs;
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class InternalType>
inline
bool
operator >= (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () >= rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class InternalType>
inline
bool
operator >= (const InternalType& lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs >= rhs .getValue ();
}

///  Compares two SFVec4 vectors.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class InternalType>
inline
bool
operator >= (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () >= rhs;
}

///  @relates SFVec4
///  @name Aritmetic operators.

template <class InternalType>
inline
InternalType
operator - (const SFVec4 <InternalType> & vector)
{
	return math::negate (vector .getValue ());
}

template <class InternalType>
inline
InternalType
operator + (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () + rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator + (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () + rhs;
}

template <class InternalType>
inline
InternalType
operator + (const InternalType & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs + rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator - (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () - rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator - (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () - rhs;
}

template <class InternalType>
inline
InternalType
operator - (const InternalType & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs - rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator * (const SFVec4 <InternalType> & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs .getValue () * rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator * (const SFVec4 <InternalType> & lhs, const InternalType & rhs)
{
	return lhs .getValue () * rhs;
}

template <class InternalType>
inline
InternalType
operator * (const InternalType & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs * rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator * (const SFVec4 <InternalType> & lhs, const typename InternalType::value_type & rhs)
{
	return lhs .getValue () * rhs;
}

template <class InternalType>
inline
InternalType
operator * (const typename InternalType::value_type & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs * rhs .getValue ();
}

template <class InternalType>
inline
InternalType
operator / (const SFVec4 <InternalType> & lhs, const typename InternalType::value_type & rhs)
{
	return lhs .getValue () / rhs;
}

template <class InternalType>
inline
InternalType
operator / (const typename InternalType::value_type & lhs, const SFVec4 <InternalType> & rhs)
{
	return lhs / rhs .getValue ();
}

// SFVec4d and SFVec4f
extern template class SFVec4 <Vector4d>;
extern template class SFVec4 <Vector4f>;

using SFVec4d = SFVec4 <Vector4d>;
using SFVec4f = SFVec4 <Vector4f>;

} // X3D
} // titania

#endif
