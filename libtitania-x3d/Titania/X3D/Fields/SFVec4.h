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

	typedef typename ValueType::value_type scalar_type;
	typedef typename ValueType::size_type size_type;

	using X3DField <ValueType>::setValue;
	using X3DField <ValueType>::getValue;
	using X3DField <ValueType>::operator =;

	/*
	 * These constructors are all trival and therefore made inline.
	 */

	SFVec4 ();

	SFVec4 (const SFVec4 &);

	explicit
	SFVec4 (const ValueType &);

	SFVec4 (const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &);

	/*
	 * These function are all trival and therefore made inline.
	 */

	virtual
	SFVec4*
	clone () const;

	void
	setX (const scalar_type &);

	scalar_type
	getX () const;

	void
	setY (const scalar_type &);

	scalar_type
	getY () const;

	void
	setZ (const scalar_type &);

	scalar_type
	getZ () const;

	void
	setW (const scalar_type &);

	scalar_type
	getW () const;

	void
	set1Value (const size_type &, const scalar_type &);

	scalar_type
	get1Value (const size_type &) const;

	void
	setValue (const scalar_type &, const scalar_type &, const scalar_type &, const scalar_type &);

	void
	getValue (scalar_type &, scalar_type &, scalar_type &, scalar_type &) const;

	SFVec4*
	negate () const;

	SFVec4*
	add (const SFVec4 &) const;

	SFVec4*
	subtract (const SFVec4 &) const;

	SFVec4*
	multiply (const scalar_type &) const;

	SFVec4*
	divide (const scalar_type &) const;

	scalar_type
	dot (const SFVec4 &) const;

	SFVec4*
	normalize () const;

	scalar_type
	length () const;

	///  Output operator.
	virtual
	void
	toStream (std::ostream &) const;


private:

	using X3DField <ValueType>::notifyParents;
	using X3DField <ValueType>::get;

};

template <class ValueType>
SFVec4 <ValueType>::SFVec4 () :
	X3DField <ValueType> ()
{ }

template <class ValueType>
SFVec4 <ValueType>::SFVec4 (const SFVec4 & field) :
	X3DField <ValueType> (field)
{ }

template <class ValueType>
SFVec4 <ValueType>::SFVec4 (const ValueType & value) :
	X3DField <ValueType> (value)
{ }

template <class ValueType>
SFVec4 <ValueType>::SFVec4 (const scalar_type & x, const scalar_type & y, const scalar_type & z, const scalar_type & w) :
	X3DField <ValueType> (ValueType (x, y, z, w))
{ }

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::clone () const
{
	return new SFVec4 (*this);
}

template <class ValueType>
void
SFVec4 <ValueType>::setX (const scalar_type & x)
{
	get () .x (x);
	notifyParents ();
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::getX () const
{
	return getValue () .x ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setY (const scalar_type & y)
{
	get () .y (y);
	notifyParents ();
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::getY () const
{
	return getValue () .y ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setZ (const scalar_type & z)
{
	get () .z (z);
	notifyParents ();
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::getZ () const
{
	return getValue () .z ();
}

template <class ValueType>
void
SFVec4 <ValueType>::setW (const scalar_type & w)
{
	get () .w (w);
	notifyParents ();
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::getW () const
{
	return getValue () .w ();
}

template <class ValueType>
void
SFVec4 <ValueType>::set1Value (const size_type & index, const scalar_type & value)
{
	get () [index] = value;
	notifyParents ();
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::get1Value (const size_type & index) const
{
	return getValue () [index];
}

template <class ValueType>
void
SFVec4 <ValueType>::setValue (const scalar_type & x, const scalar_type & y, const scalar_type & z, const scalar_type & w)
{
	setValue (ValueType (x, y, z, w));
}

template <class ValueType>
void
SFVec4 <ValueType>::getValue (scalar_type & x, scalar_type & y, scalar_type & z, scalar_type & w) const
{
	x = getValue () .x ();
	y = getValue () .y ();
	z = getValue () .z ();
	w = getValue () .w ();
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::negate () const
{
	return new SFVec4 (-getValue ());
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::add (const SFVec4 & value) const
{
	return new SFVec4 (getValue () + value .getValue ());
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::subtract (const SFVec4 & value) const
{
	return new SFVec4 (getValue () - value .getValue ());
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::multiply (const scalar_type & value) const
{
	return new SFVec4 (getValue () * value);
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::divide (const scalar_type & value) const
{
	return new SFVec4 (getValue () / value);
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::dot (const SFVec4 & value) const
{
	return math::dot (getValue (), value .getValue ());
}

template <class ValueType>
SFVec4 <ValueType>*
SFVec4 <ValueType>::normalize () const
{
	return new SFVec4 (math::normalize (getValue ()));
}

template <class ValueType>
typename SFVec4 <ValueType>::scalar_type
SFVec4 <ValueType>::length () const
{
	return abs (getValue ());
}

template <class ValueType>
void
SFVec4 <ValueType>::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type> << getValue ();
}

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
