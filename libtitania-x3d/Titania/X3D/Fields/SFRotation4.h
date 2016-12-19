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
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class SFRotation4 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type            value_type;
	typedef SFVec3 <typename ValueType::vector3_type> vector3_type;
	typedef typename ValueType::size_type             size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::addEvent;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	///  @name Construction

	SFRotation4 ();

	SFRotation4 (const SFRotation4 &);

	explicit
	SFRotation4 (const ValueType &);

	SFRotation4 (const value_type &, const value_type &, const value_type &, const value_type &);

	SFRotation4 (const vector3_type &, const value_type &);

	SFRotation4 (const vector3_type &, const vector3_type &);

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
	addInterest (Class* const object, void (Class::* memberFunction) (const SFRotation4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFRotation4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Member access

	void
	setX (const value_type &);

	value_type
	getX () const;

	void
	setY (const value_type &);

	value_type
	getY () const;

	void
	setZ (const value_type &);

	value_type
	getZ () const;

	void
	setAxis (const vector3_type &);

	vector3_type*
	getAxis () const;

	void
	setAngle (const value_type &);

	value_type
	getAngle () const;

	void
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	void
	getValue (value_type &, value_type &, value_type &, value_type &) const;

	///  @name Arithmetic operations

	SFRotation4 &
	operator *= (const SFRotation4 &);

	SFRotation4 &
	operator *= (const ValueType &);

	SFRotation4*
	inverse () const;

	SFRotation4*
	multiply (const SFRotation4 &) const;

	vector3_type*
	multVec (const vector3_type &) const;

	SFRotation4*
	slerp (const SFRotation4 &, const value_type &) const;

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


private:

	using X3DField <ValueType>::get;

};

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 () :
	X3DField <ValueType> ()
{ }

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 (const SFRotation4 & field) :
	X3DField <ValueType> (field)
{ }

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 (const ValueType & value) :
	X3DField <ValueType> (value)
{ }

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 (const value_type & x, const value_type & y, const value_type & z, const value_type & angle) :
	X3DField <ValueType> (ValueType (x, y, z, angle))
{ }

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 (const vector3_type & vector, const value_type & angle) :
	X3DField <ValueType> (ValueType (vector .getValue (), angle))
{ }

template <class ValueType>
SFRotation4 <ValueType>::SFRotation4 (const vector3_type & fromVector, const vector3_type & toVector) :
	X3DField <ValueType> (ValueType (fromVector .getValue (), toVector .getValue ()))
{ }

template <class ValueType>
inline
SFRotation4 <ValueType>*
SFRotation4 <ValueType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFRotation4 (*this);
}

template <class ValueType>
void
SFRotation4 <ValueType>::setX (const value_type & x)
{
	get () .x (x);
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::value_type
SFRotation4 <ValueType>::getX () const
{
	return getValue () .x ();
}

template <class ValueType>
void
SFRotation4 <ValueType>::setY (const value_type & y)
{
	get () .y (y);
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::value_type
SFRotation4 <ValueType>::getY () const
{
	return getValue () .y ();
}

template <class ValueType>
void
SFRotation4 <ValueType>::setZ (const value_type & z)
{
	get () .z (z);
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::value_type
SFRotation4 <ValueType>::getZ () const
{
	return getValue () .z ();
}

template <class ValueType>
void
SFRotation4 <ValueType>::setAxis (const vector3_type & axis)
{
	get () .axis (axis .getValue ());
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::vector3_type *
SFRotation4 <ValueType>::getAxis () const
{
	return new vector3_type (getValue () .axis ());
}

template <class ValueType>
void
SFRotation4 <ValueType>::setAngle (const value_type & angle)
{
	get () .angle (angle);
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::value_type
SFRotation4 <ValueType>::getAngle () const
{
	return getValue () .angle ();
}

template <class ValueType>
void
SFRotation4 <ValueType>::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::value_type
SFRotation4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class ValueType>
inline
void
SFRotation4 <ValueType>::getValue (value_type & x, value_type & y, value_type & z, value_type & angle) const
{
	getValue () .get (x, y, z, angle);
}

template <class ValueType>
SFRotation4 <ValueType> &
SFRotation4 <ValueType>::operator *= (const SFRotation4 & rotation)
{
	get () *= rotation .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
SFRotation4 <ValueType> &
SFRotation4 <ValueType>::operator *= (const ValueType & rotation)
{
	get () *= rotation;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFRotation4 <ValueType>*
SFRotation4 <ValueType>::inverse () const
{
	return new SFRotation4 (~getValue ());
}

template <class ValueType>
inline
SFRotation4 <ValueType>*
SFRotation4 <ValueType>::multiply (const SFRotation4 & value) const
{
	return new SFRotation4 (getValue () * value .getValue ());
}

template <class ValueType>
inline
typename SFRotation4 <ValueType>::vector3_type *
SFRotation4 <ValueType>::multVec (const vector3_type &value) const
{
	return new vector3_type (value .getValue () * getValue ());
}

template <class ValueType>
inline
SFRotation4 <ValueType>*
SFRotation4 <ValueType>::slerp (const SFRotation4 & dest, const value_type & t) const
{
	return new SFRotation4 (math::slerp (getValue (), dest .getValue (), t));
}

template <class ValueType>
inline
void
SFRotation4 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whiteSpaces;

	value_type x, y, z, angle;

	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	if (Grammar::Number <value_type> (istream, x))
	{
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <value_type> (istream, y))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <value_type> (istream, z))
			{
				Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

				if (Grammar::Number <value_type> (istream, angle))
					setValue (ValueType (x, y, z, angle));
			}
	   }
	}
}

template <class ValueType>
inline
void
SFRotation4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::Precision <value_type> << getValue ();
}

template <class ValueType>
inline
void
SFRotation4 <ValueType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

template <class ValueType>
inline
void
SFRotation4 <ValueType>::toJSONStream (std::ostream & ostream) const
{
	value_type x, y, z, angle;

	getValue () .get (x, y, z, angle);

	ostream
		<< X3DGenerator::Precision <value_type>
		<< '['
		<< X3DGenerator::TidySpace
		<< x
		<< ','
		<< X3DGenerator::TidySpace
		<< y
		<< ','
		<< X3DGenerator::TidySpace
		<< z
		<< ','
		<< X3DGenerator::TidySpace
		<< angle
		<< X3DGenerator::TidySpace
		<< ']';
}

///  @relates SFRotation4
///  @name Aritmetic operators.

template <class ValueType>
inline
ValueType
operator ~ (const SFRotation4 <ValueType> & rotation)
{
	return ~rotation .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFRotation4 <ValueType> & a, const SFRotation4 <ValueType> & b)
{
	return a .getValue () * b .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFRotation4 <ValueType> & a, const ValueType & b)
{
	return a .getValue () * b;
}

template <class ValueType>
inline
ValueType
operator * (const ValueType & a, const SFRotation4 <ValueType> & b)
{
	return a * b .getValue ();
}

template <class ValueType>
inline
typename ValueType::vector3_type
operator * (const typename ValueType::vector3_type & vector, const SFRotation4 <ValueType> & rotation)
{
	return vector * rotation .getValue ();
}

template <class ValueType>
inline
typename ValueType::vector3_type
operator * (const SFRotation4 <ValueType> & rotation, const typename ValueType::vector3_type & vector)
{
	return rotation .getValue () * vector;
}

extern template class SFRotation4 <Rotation4d>;
extern template class SFRotation4 <Rotation4f>;

typedef SFRotation4 <Rotation4d> SFRotation4d;
typedef SFRotation4 <Rotation4f> SFRotation4f;
typedef SFRotation4 <Rotation4d> SFRotation;

} // X3D
} // titania

#endif
