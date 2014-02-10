/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_FIELDS_SFVEC4_H__
#define __TITANIA_X3D_FIELDS_SFVEC4_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

extern template class X3DField <Vector4d>;
extern template class X3DField <Vector4f>;

template <class ValueType>
class SFVec4 :
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

	/*
	 * These constructors are all trival and therefore made inline.
	 */

	SFVec4 () :
		X3DField <ValueType> ()
	{ }

	SFVec4 (const SFVec4 & field) :
		X3DField <ValueType> (field)
	{ }

	explicit
	SFVec4 (const ValueType & value) :
		X3DField <ValueType> (value)
	{ }

	SFVec4 (const value_type & x, const value_type & y, const value_type & z, const value_type & w) :
		X3DField <ValueType> (ValueType (x, y, z, w))
	{ }

	/*
	 * These function are all trival and therefore made inline.
	 */

	virtual
	SFVec4*
	clone () const
	throw (Error <NOT_SUPPORTED>) final override;

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const SFVec4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const SFVec4 &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Functions

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
	setW (const value_type &);

	value_type
	getW () const;

	void
	set1Value (const size_type &, const value_type &);

	value_type
	get1Value (const size_type &) const;

	void
	setValue (const value_type &, const value_type &, const value_type &, const value_type &);

	void
	getValue (value_type &, value_type &, value_type &, value_type &) const;

	SFVec4 &
	operator += (const SFVec4 &);

	SFVec4 &
	operator += (const ValueType &);

	SFVec4 &
	operator -= (const SFVec4 &);

	SFVec4 &
	operator -= (const ValueType &);

	SFVec4 &
	operator *= (const SFVec4 &);

	SFVec4 &
	operator *= (const ValueType &);

	SFVec4 &
	operator *= (const value_type &);

	SFVec4 &
	operator /= (const SFVec4 &);

	SFVec4 &
	operator /= (const ValueType &);

	SFVec4 &
	operator /= (const value_type &);

	SFVec4*
	negate () const;

	SFVec4*
	add (const SFVec4 &) const;

	SFVec4*
	subtract (const SFVec4 &) const;

	SFVec4*
	multiply (const SFVec4 &) const;

	SFVec4*
	multiply (const value_type &) const;

	SFVec4*
	divide (const SFVec4 &) const;

	SFVec4*
	divide (const value_type &) const;

	value_type
	dot (const SFVec4 &) const;

	SFVec4*
	normalize () const;

	value_type
	length () const;

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const final override;


private:

	using X3DField <ValueType>::get;

};

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::clone () const
throw (Error <NOT_SUPPORTED>)
{
	return new SFVec4 (*this);
}

template <class ValueType>
void
SFVec4 <ValueType>::setX (const value_type & x)
{
	get () .x (x);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::getX () const
{
	return getValue () .x ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setY (const value_type & y)
{
	get () .y (y);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::getY () const
{
	return getValue () .y ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setZ (const value_type & z)
{
	get () .z (z);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::getZ () const
{
	return getValue () .z ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setW (const value_type & w)
{
	get () .w (w);
	addEvent ();
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::getW () const
{
	return getValue () .w ();
}

template <class ValueType>
void
SFVec4 <ValueType>::set1Value (const size_type & index, const value_type & value)
{
	get () [index] = value;
	addEvent ();
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class ValueType>
inline
void
SFVec4 <ValueType>::setValue (const value_type & x, const value_type & y, const value_type & z, const value_type & w)
{
	setValue (ValueType (x, y, z, w));
}

template <class ValueType>
void
SFVec4 <ValueType>::getValue (value_type & x, value_type & y, value_type & z, value_type & w) const
{
	x = getValue () .x ();
	y = getValue () .y ();
	z = getValue () .z ();
	w = getValue () .w ();
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator += (const SFVec4 & vector)
{
	get () += vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator += (const ValueType & vector)
{
	get () += vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator -= (const SFVec4 & vector)
{
	get () -= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator -= (const ValueType & vector)
{
	get () -= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator *= (const SFVec4 & vector)
{
	get () *= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator *= (const ValueType & vector)
{
	get () *= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator *= (const value_type & value)
{
	get () *= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator /= (const SFVec4 & vector)
{
	get () /= vector .getValue ();
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator /= (const ValueType & vector)
{
	get () /= vector;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType> &
SFVec4 <ValueType>::operator /= (const value_type & value)
{
	get () /= value;
	addEvent ();
	return *this;
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::negate () const
{
	return new SFVec4 (-getValue ());
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::add (const SFVec4 & field) const
{
	return new SFVec4 (getValue () + field .getValue ());
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::subtract (const SFVec4 & field) const
{
	return new SFVec4 (getValue () - field .getValue ());
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::multiply (const SFVec4 & field) const
{
	return new SFVec4 (getValue () * field .getValue ());
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::multiply (const value_type & value) const
{
	return new SFVec4 (getValue () * value);
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::divide (const SFVec4 & field) const
{
	return new SFVec4 (getValue () / field .getValue ());
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::divide (const value_type & value) const
{
	return new SFVec4 (getValue () / value);
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::dot (const SFVec4 & field) const
{
	return math::dot (getValue (), field .getValue ());
}

template <class ValueType>
inline
SFVec4 <ValueType>*
SFVec4 <ValueType>::normalize () const
{
	return new SFVec4 (math::normalize (getValue ()));
}

template <class ValueType>
inline
typename SFVec4 <ValueType>::value_type
SFVec4 <ValueType>::length () const
{
	return abs (getValue ());
}

template <class ValueType>
inline
void
SFVec4 <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	std::string whitespaces;
	
	Grammar::whitespaces (istream, whitespaces);

	istream >> get ();

	if (istream)
		addEvent ();
}

template <class ValueType>
inline
void
SFVec4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <value_type> << getValue ();
}

// Aritmetic operators.

//@{
template <class ValueType>
inline
ValueType
operator - (const SFVec4 <ValueType> & vector)
{
	return -vector .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator + (const SFVec4 <ValueType> & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs .getValue () + rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const SFVec4 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () + rhs;
}

template <class ValueType>
inline
ValueType
operator + (const ValueType & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs + rhs .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator - (const SFVec4 <ValueType> & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs .getValue () - rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator - (const SFVec4 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () - rhs;
}

template <class ValueType>
inline
ValueType
operator - (const ValueType & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs - rhs .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator * (const SFVec4 <ValueType> & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs .getValue () * rhs .getValue ();
}

template <class ValueType>
inline
ValueType
operator * (const SFVec4 <ValueType> & lhs, const ValueType & rhs)
{
	return lhs .getValue () * rhs;
}

template <class ValueType>
inline
ValueType
operator * (const ValueType & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs * rhs .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator * (const SFVec4 <ValueType> & lhs, const typename ValueType::value_type & rhs)
{
	return lhs .getValue () * rhs;
}

template <class ValueType>
inline
ValueType
operator * (const typename ValueType::value_type & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs * rhs .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator / (const SFVec4 <ValueType> & lhs, const typename ValueType::value_type & rhs)
{
	return lhs .getValue () / rhs;
}

template <class ValueType>
inline
ValueType
operator / (const typename ValueType::value_type & lhs, const SFVec4 <ValueType> & rhs)
{
	return lhs / rhs .getValue ();
}
//@}

//extern template class X3DField <Vector4d>;
//extern template class X3DField <Vector4f>;

// SFVec4d and SFVec4f
extern template class SFVec4 <Vector4d>;
extern template class SFVec4 <Vector4f>;

typedef SFVec4 <Vector4d> SFVec4d;
typedef SFVec4 <Vector4f> SFVec4f;

} // X3D
} // titania

#endif
