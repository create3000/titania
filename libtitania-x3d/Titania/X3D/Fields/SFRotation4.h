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

#ifndef __TITANIA_X3D_FIELDS_SFROTATION4_H__
#define __TITANIA_X3D_FIELDS_SFROTATION4_H__

#include "../Basic/X3DField.h"
#include "../InputOutput/VRMLGenerator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Parser/MiniParser.h"
#include "../Types/Numbers.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {

extern template class X3DField <Rotation4d>;
extern template class X3DField <Rotation4f>;

/**
 *  Template to represent an arbitary rotation about an axis. This is the base class for the SFRotation field.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  InternalType  Type of the internal value of the field.
 */
template <class InternalType>
class SFRotation4 :
	public X3DField <InternalType>
{
public:

	using value_type  = typename InternalType::value_type;
	using vector_type = SFVec3 <typename InternalType::vector_type>;
	using size_type   = typename InternalType::size_type;

	using X3DField <InternalType>::addInterest;
	using X3DField <InternalType>::addEvent;
	using X3DField <InternalType>::getUnit;
	using X3DField <InternalType>::setValue;
	using X3DField <InternalType>::getValue;
	using X3DField <InternalType>::operator =;

	///  @name Construction

	SFRotation4 ();

	SFRotation4 (const SFRotation4 & other);

	explicit
	SFRotation4 (const InternalType & value);

	SFRotation4 (const value_type & x, const value_type & y, const value_type & z, const value_type & angle);

	SFRotation4 (const typename vector_type::internal_type & axis, const value_type & angle);

	SFRotation4 (const typename vector_type::internal_type & fromVec, const typename vector_type::internal_type & toVec);

	virtual
	SFRotation4*
	create () const final override
	{ return new SFRotation4 (); }

	virtual
	SFRotation4*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFRotation4 &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const SFRotation4 &), Class & object) const
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
	setAxis (const typename vector_type::internal_type & value);

	typename vector_type::internal_type
	getAxis () const;

	void
	setAngle (const value_type & value);

	value_type
	getAngle () const;

	void
	set1Value (const size_type & index, const value_type & value);

	value_type
	get1Value (const size_type & index) const;

	value_type
	at (const size_type & index) const;

	value_type
	operator [ ] (const size_type & index) const;

	///  @name Arithmetic operations

	SFRotation4 &
	operator *= (const InternalType & rotation);

	InternalType
	inverse () const;

	InternalType
	multiply (const InternalType & rotation) const;

	typename vector_type::internal_type
	multVec (const typename vector_type::internal_type & vector) const;

	InternalType
	normalize () const;

	InternalType
	slerp (const InternalType & dest, const value_type & t) const;

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


protected:

	using X3DField <InternalType>::get;

};

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 () :
	X3DField <InternalType> ()
{ }

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 (const SFRotation4 & other) :
	X3DField <InternalType> (other)
{ }

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 (const InternalType & value) :
	X3DField <InternalType> (value)
{ }

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 (const value_type & x, const value_type & y, const value_type & z, const value_type & angle) :
	X3DField <InternalType> (InternalType (x, y, z, angle))
{ }

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 (const typename vector_type::internal_type & vector, const value_type & angle) :
	X3DField <InternalType> (InternalType (vector, angle))
{ }

template <class InternalType>
SFRotation4 <InternalType>::SFRotation4 (const typename vector_type::internal_type & fromVector, const typename vector_type::internal_type & toVector) :
	X3DField <InternalType> (InternalType (fromVector, toVector))
{ }

template <class InternalType>
inline
SFRotation4 <InternalType>*
SFRotation4 <InternalType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFRotation4 (*this);
}

template <class InternalType>
void
SFRotation4 <InternalType>::setX (const value_type & x)
{
	get () .x (x);
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::getX () const
{
	return getValue () .x ();
}

template <class InternalType>
void
SFRotation4 <InternalType>::setY (const value_type & y)
{
	get () .y (y);
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::getY () const
{
	return getValue () .y ();
}

template <class InternalType>
void
SFRotation4 <InternalType>::setZ (const value_type & z)
{
	get () .z (z);
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::getZ () const
{
	return getValue () .z ();
}

template <class InternalType>
void
SFRotation4 <InternalType>::setAxis (const typename vector_type::internal_type & axis)
{
	get () .axis (axis);
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::vector_type::internal_type
SFRotation4 <InternalType>::getAxis () const
{
	return getValue () .axis ();
}

template <class InternalType>
void
SFRotation4 <InternalType>::setAngle (const value_type & angle)
{
	get () .angle (angle);
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::getAngle () const
{
	return getValue () .angle ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::at (const size_type & index) const
{
	return getValue () .at (index);
}

template <class InternalType>
void
SFRotation4 <InternalType>::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = value;
	addEvent ();
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::value_type
SFRotation4 <InternalType>::operator [ ] (const size_type & index) const
{
	return getValue () [index];
}

template <class InternalType>
SFRotation4 <InternalType> &
SFRotation4 <InternalType>::operator *= (const InternalType & rotation)
{
	get () *= rotation;
	addEvent ();
	return *this;
}

template <class InternalType>
inline
InternalType
SFRotation4 <InternalType>::inverse () const
{
	return math::inverse (getValue ());
}

template <class InternalType>
inline
InternalType
SFRotation4 <InternalType>::multiply (const InternalType & value) const
{
	return getValue () * value;
}

template <class InternalType>
inline
typename SFRotation4 <InternalType>::vector_type::internal_type
SFRotation4 <InternalType>::multVec (const typename vector_type::internal_type &value) const
{
	return value * getValue ();
}

template <class InternalType>
inline
InternalType
SFRotation4 <InternalType>::normalize () const
{
	return math::normalize (getValue ());
}

template <class InternalType>
inline
InternalType
SFRotation4 <InternalType>::slerp (const InternalType & dest, const value_type & t) const
{
	return math::slerp (getValue (), dest, t);
}

template <class InternalType>
inline
void
SFRotation4 <InternalType>::fromStream (std::istream & istream)
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
SFRotation4 <InternalType>::toStream (std::ostream & ostream) const
{
	VRMLGenerator::Encode (ostream, getValue (), getUnit ());
}

template <class InternalType>
inline
void
SFRotation4 <InternalType>::toXMLStream (std::ostream & ostream) const
{
	XMLGenerator::Encode (ostream, getValue (), getUnit ());
}

template <class InternalType>
inline
void
SFRotation4 <InternalType>::toJSONStream (std::ostream & ostream) const
{
	ostream
		<< '['
		<< Generator::TidySpace;

	JSONGenerator::Encode (ostream, getValue (), getUnit ());

	ostream
		<< Generator::TidySpace
		<< ']';
}

///  @relates SFRotation4
///  @name Aritmetic operators.

template <class InternalType>
inline
InternalType
operator ~ (const SFRotation4 <InternalType> & rotation)
{
	return math::inverse (rotation .getValue ());
}

template <class InternalType>
inline
InternalType
operator * (const SFRotation4 <InternalType> & a, const SFRotation4 <InternalType> & b)
{
	return a .getValue () * b .getValue ();
}

template <class InternalType>
inline
InternalType
operator * (const SFRotation4 <InternalType> & a, const InternalType & b)
{
	return a .getValue () * b;
}

template <class InternalType>
inline
InternalType
operator * (const InternalType & a, const SFRotation4 <InternalType> & b)
{
	return a * b .getValue ();
}

template <class InternalType>
inline
typename InternalType::vector_type
operator * (const typename InternalType::vector_type & vector, const SFRotation4 <InternalType> & rotation)
{
	return vector * rotation .getValue ();
}

template <class InternalType>
inline
typename InternalType::vector_type
operator * (const SFRotation4 <InternalType> & rotation, const typename InternalType::vector_type & vector)
{
	return rotation .getValue () * vector;
}

extern template class SFRotation4 <Rotation4d>;
extern template class SFRotation4 <Rotation4f>;

using SFRotation = SFRotation4 <Rotation4d>;

} // X3D
} // titania

#endif
