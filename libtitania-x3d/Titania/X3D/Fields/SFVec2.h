/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#ifndef __TITANIA_X3D_FIELDS_SFVEC2_H__
#define __TITANIA_X3D_FIELDS_SFVEC2_H__

#include "../Basic/X3DField.h"
#include "../Types/Numbers.h"

namespace titania {
namespace X3D {

extern template class X3DField <Vector2d>;
extern template class X3DField <Vector2f>;

template <class ValueType>
class SFVec2 :
	public X3DField <ValueType>
{
public:

	typedef typename ValueType::size_type size_type;

	typedef typename ValueType::value_type scalar_type;

	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	/*
	 * These constructors are all trival and therefore made inline.
	 */

	SFVec2 () :
		X3DField <ValueType> ()
	{ }

	SFVec2 (const SFVec2 & field) :
		X3DField <ValueType> (field)
	{ }

	explicit
	SFVec2 (const ValueType & value) :
		X3DField <ValueType> (value)
	{ }

	SFVec2 (const scalar_type & x, const scalar_type & y) :
		X3DField <ValueType> (ValueType (x, y))
	{ }

	/*
	 * These function are all trival and therefore made inline.
	 */

	virtual
	SFVec2*
	copy () const;

	void
	setX (const scalar_type &);

	scalar_type
	getX () const;

	void
	setY (const scalar_type &);

	scalar_type
	getY () const;

	void
	set1Value (const size_type &, const scalar_type &);

	scalar_type
	get1Value (const size_type &) const;

	void
	setValue (const scalar_type &, const scalar_type &);

	void
	getValue (scalar_type &, scalar_type &) const;

	SFVec2*
	negate () const;

	SFVec2*
	add (const SFVec2 &) const;

	SFVec2*
	subtract (const SFVec2 &) const;

	SFVec2*
	multiply (const scalar_type &) const;

	SFVec2*
	divide (const scalar_type &) const;

	scalar_type
	dot (const SFVec2 &) const;

	SFVec2*
	normalize () const;

	scalar_type
	length () const;


private:

	using X3DField <ValueType>::notifyParents;
	using X3DField <ValueType>::get;

};

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::copy () const
{
	return new SFVec2 (*this);
}

template <class ValueType>
void
SFVec2 <ValueType>::setX (const scalar_type & x)
{
	get () .x (x);
	notifyParents ();
}

template <class ValueType>
inline
typename SFVec2 <ValueType>::scalar_type
SFVec2 <ValueType>::getX () const
{
	return getValue () .x ();
}

template <class ValueType>
void
SFVec2 <ValueType>::setY (const scalar_type & y)
{
	get () .y (y);
	notifyParents ();
}

template <class ValueType>
inline
typename SFVec2 <ValueType>::scalar_type
SFVec2 <ValueType>::getY () const
{
	return getValue () .y ();
}

template <class ValueType>
void
SFVec2 <ValueType>::set1Value (const size_type  & index, const scalar_type & value)
{
	get () [index] = value;
	notifyParents ();
}

template <class ValueType>
inline
typename SFVec2 <ValueType>::scalar_type
SFVec2 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class ValueType>
void
SFVec2 <ValueType>::setValue (const scalar_type & x, const scalar_type & y)
{
	setValue (ValueType (x, y));
}

template <class ValueType>
inline
void
SFVec2 <ValueType>::getValue (scalar_type & x, scalar_type & y) const
{
	x = getValue () .x ();
	y = getValue () .y ();
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::negate () const
{
	return new SFVec2 (-getValue ());
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::add (const SFVec2 & value) const
{
	return new SFVec2 (getValue () + value .getValue ());
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::subtract (const SFVec2 & value) const
{
	return new SFVec2 (getValue () - value .getValue ());
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::multiply (const scalar_type & value) const
{
	return new SFVec2 (getValue () * value);
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::divide (const scalar_type & value) const
{
	return new SFVec2 (getValue () / value);
}

template <class ValueType>
inline
typename SFVec2 <ValueType>::scalar_type
SFVec2 <ValueType>::dot (const SFVec2 & value) const
{
	return math::dot (getValue (), value .getValue ());
}

template <class ValueType>
inline
SFVec2 <ValueType>*
SFVec2 <ValueType>::normalize () const
{
	return new SFVec2 (math::normalize (getValue ()));
}

template <class ValueType>
inline
typename SFVec2 <ValueType>::scalar_type
SFVec2 <ValueType>::length () const
{
	return abs (getValue ());
}

// Aritmetic operators.

//@{
template <class ValueType>
inline
ValueType
operator - (const SFVec2 <ValueType> & vector)
{
	return -vector .getValue ();
}
//@}

//@{
template <class ValueType>
inline
ValueType
operator + (const SFVec2 <ValueType> & a, const SFVec2 <ValueType> & b)
{
	return a .getValue () + b .getValue ();
}

template <class ValueType>
inline
ValueType
operator + (const SFVec2 <ValueType> & a, const ValueType & b)
{
	return a .getValue () + b;
}

template <class ValueType>
inline
ValueType
operator + (const ValueType & a, const SFVec2 <ValueType> & b)
{
	return a + b .getValue ();
}
//@}

//extern template class X3DField <Vector2d>;
//extern template class X3DField <Vector2f>;

// SFVec2d and SFVec2f
extern template class SFVec2 <Vector2d>;
extern template class SFVec2 <Vector2f>;

typedef SFVec2 <Vector2d> SFVec2d;
typedef SFVec2 <Vector2f> SFVec2f;

} // X3D
} // titania

#endif
