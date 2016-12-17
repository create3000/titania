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

#ifndef __TITANIA_X3D_FIELDS_SFVEC3_H__
#define __TITANIA_X3D_FIELDS_SFVEC3_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

extern template class X3DField <Vector3d>;
extern template class X3DField <Vector3f>;

/**
 *  Template to represent vector3 fields. This is the base class for SFVec3d and SFVec3f fields.
 *
 *  Extern instantiations for float and double are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  ValueType  Type of the internal value of the field.
 */
template <class ValueType>
class SFVec3 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::value_type value_type;
	typedef typename ValueType::size_type  size_type;

	using X3DField <ValueType>::addInterest;
	using X3DField <ValueType>::addEvent;
	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	///  @name Construction

	/*
	 * These constructors are all trival and therefore made inline.
	 */

	SFVec3 () :
		X3DField <ValueType> ()
	{ }

	SFVec3 (const SFVec3 & field) :
		X3DField <ValueType> (field)
	{ }

	explicit
	SFVec3 (const ValueType & value) :
		X3DField <ValueType> (value)
	{ }

	SFVec3 (const value_type & x, const value_type & y, const value_type & z) :
		X3DField <ValueType> (ValueType (x, y, z))
	{ }

	/*
	 * These function are all trival and therefore made inline.
	 */

	virtual
	SFVec3*
	create () const final override
	{ return new SFVec3 (); }

	virtual
	SFVec3*
	copy (const CopyType) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) final override;

	///  @name Interest service

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFVec3 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFVec3 &)) const
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
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	///  @name Arithmetic operations

	SFVec3 &
	operator += (const SFVec3 &);

	SFVec3 &
	operator += (const ValueType &);

	SFVec3 &
	operator -= (const SFVec3 &);

	SFVec3 &
	operator -= (const ValueType &);

	SFVec3 &
	operator *= (const SFVec3 &);

	SFVec3 &
	operator *= (const ValueType &);

	SFVec3 &
	operator *= (const value_type &);

	SFVec3 &
	operator /= (const SFVec3 &);

	SFVec3 &
	operator /= (const ValueType &);

	SFVec3 &
	operator /= (const value_type &);

	SFVec3*
	negate () const;

	SFVec3*
	add (const SFVec3 &) const;

	SFVec3*
	subtract (const SFVec3 &) const;

	SFVec3*
	multiply (const SFVec3 &) const;

	SFVec3*
	multiply (const value_type &) const;

	SFVec3*
	divide (const SFVec3 &) const;

	SFVec3*
	divide (const value_type &) const;

	value_type
	dot (const SFVec3 &) const;

	SFVec3*
	cross (const SFVec3 &) const;

	SFVec3*
	normalize () const;

	value_type
	length () const;

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
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::copy (const CopyType) const
throw (Error <INVALID_NAME>,
	    Error <NOT_SUPPORTED>)
{
	return new SFVec3 (*this);
}

template <class ValueType>
void
SFVec3 <ValueType>::setX (const value_type & x)
{
	get () .x (x);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::getX () const
{
	return getValue () .x ();
}

template <class ValueType>
void
SFVec3 <ValueType>::setY (const value_type & y)
{
	get () .y (y);
	addEvent ();
}

template <class ValueType>
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::getY () const
{
	return getValue () .y ();
}

template <class ValueType>
void
SFVec3 <ValueType>::setZ (const value_type & z)
{
	get () .z (z);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::getZ () const
{
	return getValue () .z ();
}

template <class ValueType>
void
SFVec3 <ValueType>::set1Value (const size_type  & index, const value_type & value)
{
	get () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::get1Value (const size_type  & index) const
{
	return getValue () [index];
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator += (const SFVec3 & vector)
{
	get () += vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator += (const ValueType & vector)
{
	get () += vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator -= (const SFVec3 & vector)
{
	get () -= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator -= (const ValueType & vector)
{
	get () -= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator *= (const SFVec3 & vector)
{
	get () *= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator *= (const ValueType & vector)
{
	get () *= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator *= (const value_type & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator /= (const SFVec3 & vector)
{
	get () /= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator /= (const ValueType & vector)
{
	get () /= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType> &
SFVec3 <ValueType>::operator /= (const value_type & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::negate () const
{
	return new SFVec3 (-getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::add (const SFVec3 & field) const
{
	return new SFVec3 (getValue () + field .getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::subtract (const SFVec3 & field) const
{
	return new SFVec3 (getValue () - field .getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::multiply (const SFVec3 & field) const
{
	return new SFVec3 (getValue () * field .getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::multiply (const value_type & value) const
{
	return new SFVec3 (getValue () * value);
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::divide (const SFVec3 & field) const
{
	return new SFVec3 (getValue () / field .getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::divide (const value_type & value) const
{
	return new SFVec3 (getValue () / value);
}

template <class ValueType>
inline
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::dot (const SFVec3 & field) const
{
	return math::dot (getValue (), field .getValue ());
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::cross (const SFVec3 & field) const
{
	return new SFVec3 (math::cross (getValue (), field .getValue ()));
}

template <class ValueType>
inline
SFVec3 <ValueType>*
SFVec3 <ValueType>::normalize () const
{
	return new SFVec3 (math::normalize (getValue ()));
}

template <class ValueType>
inline
typename SFVec3 <ValueType>::value_type
SFVec3 <ValueType>::length () const
{
	return abs (getValue ());
}

template <class ValueType>
inline
void
SFVec3 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whiteSpaces;

	value_type x, y, z;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	if (Grammar::Number <value_type> (istream, x))
	{
		Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

		if (Grammar::Number <value_type> (istream, y))
		{
			Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

			if (Grammar::Number <value_type> (istream, z))
				setValue (ValueType (x, y, z));
	   }
	}
}

template <class ValueType>
inline
void
SFVec3 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::Precision <value_type> << getValue ();
}

template <class ValueType>
inline
void
SFVec3 <ValueType>::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

template <class ValueType>
inline
void
SFVec3 <ValueType>::toJSONStream (std::ostream & ostream) const
{
}

///  @relates SFVec3
///  @name Aritmetic operators.

template <class ValueType>
inline
ValueType
operator - (const SFVec3 <ValueType> & vector)
{
	return -vector .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const SFVec3 <ValueType> & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs .getValue () + rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const SFVec3 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () + rhs;
}

template <class ValueType>
inline
ValueType
operator + (const ValueType & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs + rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator - (const SFVec3 <ValueType> & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs .getValue () - rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator - (const SFVec3 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () - rhs;
}

template <class ValueType>
inline
ValueType
operator - (const ValueType & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs - rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFVec3 <ValueType> & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs .getValue () * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFVec3 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () * rhs;
}

template <class ValueType>
inline
ValueType
operator * (const ValueType & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFVec3 <ValueType> & lhs, const typename ValueType::value_type & rhs)
{
	return lhs .getValue () * rhs;
}

template <class ValueType>
inline
ValueType
operator * (const typename ValueType::value_type & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator / (const SFVec3 <ValueType> & lhs, const typename ValueType::value_type & rhs)
{
	return lhs .getValue () / rhs;
}

template <class ValueType>
inline
ValueType
operator / (const typename ValueType::value_type & lhs, const SFVec3 <ValueType> & rhs)
{
	return lhs / rhs .getValue ();
}

// SFVec3d and SFVec3f
extern template class SFVec3 <Vector3d>;
extern template class SFVec3 <Vector3f>;

typedef SFVec3 <Vector3d> SFVec3d;
typedef SFVec3 <Vector3f> SFVec3f;

} // X3D
} // titania

#endif
