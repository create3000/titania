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

#ifndef __TESTS_OBSOLETE_ROTATION_H__
#define __TESTS_OBSOLETE_ROTATION_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include "Quaternion.h"

namespace titania {
namespace Obsolete {

template <typename Type>
class Rotation4
{
public:

	typedef Type   value_type;
	typedef size_t size_type;

	static const Rotation4 Null;

	Quaternion <Type> quaternion;

	Rotation4 ();

	Rotation4 (const Type &, const Type &, const Type &, const Type &);

	template <typename T>
	Rotation4 (const Vector3 <T> &, const Type &);

	template <typename F, typename T>
	Rotation4 (const Vector3 <F> &, const Vector3 <T> &);

	template <typename T>
	Rotation4 (const Quaternion <T> &);

	template <typename T>
	Rotation4 (const Rotation4 <T> &);

	void
	setValue (const Type &, const Type &, const Type &, const Type &);

	template <typename T>
	void
	getValue (T &, T &, T &, T &) const;

	void
	set1Value (const size_type, const float);

	float
	get1Value (const size_type) const;

	void
	setX (const Type &);

	Type
	getX () const { return getAxis () .x (); }

	void
	setY (const Type &);

	Type
	getY () const { return getAxis () .y (); }

	void
	setZ (const Type &);

	Type
	getZ () const { return getAxis () .z (); }

	template <typename T>
	void
	setAxis (const Vector3 <T> &);

	Vector3 <Type>
	getAxis () const;

	void
	setAngle (const Type &);

	Type
	getAngle () const { return quaternion .getAngle (); }

	template <typename T>
	Rotation4
	slerp (const Rotation4 <T> &, const Type &) const;

	template <typename T>
	bool
	operator == (const Rotation4 <T> &) const;

	template <typename T>
	bool
	operator not_eq (const Rotation4 <T> &) const;

	template <typename T>
	Rotation4 &
	operator = (const Rotation4 <T> &);

	Rotation4
	inverse () const;

	Rotation4
	operator ~ () const;

	template <typename T>
	Rotation4 &
	operator *= (const Rotation4 <T> &);

	template <typename T>
	Rotation4
	multiply (const Rotation4 <T> &) const;

	template <typename T>
	Rotation4
	operator * (const Rotation4 <T> &) const;

	template <typename T>
	Vector3 <Type>
	multVec (const Vector3 <T> &) const;

	template <typename T>
	Vector3 <Type>
	operator * (const Vector3 <T> &) const;

};

template <typename Type>
const Rotation4 <Type> Rotation4 <Type>::Null;
template <typename Type>
Rotation4 <Type>::Rotation4 ()
{ }

template <typename Type>
Rotation4 <Type>::Rotation4 (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	quaternion .setAxisAngle (x, y, z, angle);
}

template <typename Type>
template <typename T>
Rotation4 <Type>::Rotation4 (const Vector3 <T> & vec, const Type & angle)
{
	quaternion .setAxisAngle (vec .x (), vec .y (), vec .z (), angle);
}

template <typename Type>
template <typename F, typename T>
Rotation4 <Type>::Rotation4 (const Vector3 <F> & fromVec, const Vector3 <T> & toVec)
{
	Type mod = abs (fromVec) * abs (toVec);

	if (mod)
	{
		Vector3 <Type> vector  = cross (fromVec, toVec);
		Type           dotprod = dot (fromVec, toVec);
		Type           angle   = std::acos (dotprod / mod);
		quaternion .setAxisAngle (vector .x (), vector .y (), vector .z (), angle);
	}
}

template <typename Type>
template <typename T>
Rotation4 <Type>::Rotation4 (const Quaternion <T> & quaternion) :
	quaternion (quaternion)
{ }

template <typename Type>
template <typename T>
Rotation4 <Type>::Rotation4 (const Rotation4 <T> & rotation) :
	quaternion (rotation .quaternion)
{ }

template <typename Type>
void
Rotation4 <Type>::setValue (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	quaternion .setAxisAngle (x, y, z, angle);
}

template <typename Type>
template <typename T>
void
Rotation4 <Type>::getValue (T & x, T & y, T & z, T & angle) const
{
	quaternion .getAxisAngle (x, y, z, angle);
}

template <typename Type>
void
Rotation4 <Type>::set1Value (const size_type index, const float value)
{
	Type x, y, z, angle;

	quaternion .getAxisAngle (x, y, z, angle);

	switch (index)
	{
		case 0: setX (value); return;
		case 1: setY (value); return;
		case 2: setZ (value); return;
		case 3: setAngle (value); return;
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
float
Rotation4 <Type>::get1Value (const size_type index) const
{
	switch (index)
	{
		case 0: return getX ();
		case 1: return getY ();
		case 2: return getZ ();
		case 3: return getAngle ();
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
void
Rotation4 <Type>::setX (const Type & value)
{
	Type x, y, z, angle;

	quaternion .getAxisAngle (x, y, z, angle);
	quaternion .setAxisAngle (value, y, z, angle);
}

template <typename Type>
void
Rotation4 <Type>::setY (const Type & value)
{
	Type x, y, z, angle;

	quaternion .getAxisAngle (x, y, z, angle);
	quaternion .setAxisAngle (x, value, z, angle);
}

template <typename Type>
void
Rotation4 <Type>::setZ (const Type & value)
{
	Type x, y, z, angle;

	quaternion .getAxisAngle (x, y, z, angle);
	quaternion .setAxisAngle (x, y, value, angle);
}

template <typename Type>
template <typename T>
void
Rotation4 <Type>::setAxis (const Vector3 <T> & axis)
{
	Type angle = quaternion .getAngle ();

	quaternion .setAxisAngle (axis .x (), axis .y (), axis .z (), angle);
}

template <typename Type>
Vector3 <Type>
Rotation4 <Type>::getAxis () const
{
	Type x, y, z;

	quaternion .getAxis (x, y, z);
	return Vector3 <Type> (x, y, z);
}

template <typename Type>
void
Rotation4 <Type>::setAngle (const Type & angle)
{
	Type x, y, z;

	quaternion .getAxis (x, y, z);
	quaternion .setAxisAngle (x, y, z, angle);
}

template <typename Type>
template <typename T>
Rotation4 <Type>
Rotation4 <Type>::slerp (const Rotation4 <T> & dest, const Type & t) const
{
	return Rotation4 <Type> (quaternion .slerp (dest .quaternion, t));
}

template <typename Type>
template <typename T>
bool
Rotation4 <Type>::operator == (const Rotation4 <T> & value) const
{
	return quaternion == value .quaternion;
}

template <typename Type>
template <typename T>
bool
Rotation4 <Type>::operator not_eq (const Rotation4 <T> & value) const
{
	return quaternion not_eq value .quaternion;
}

template <typename Type>
template <typename T>
Rotation4 <Type> &
Rotation4 <Type>::operator = (const Rotation4 <T> & value)
{
	quaternion = value .quaternion;
	return *this;
}

template <typename Type>
Rotation4 <Type>
Rotation4 <Type>::inverse () const
{
	return Rotation4 <Type> (~quaternion);
}

template <typename Type>
Rotation4 <Type>
Rotation4 <Type>::operator ~ () const
{
	return Rotation4 <Type> (~quaternion);
}

template <typename Type>
template <typename T>
Rotation4 <Type> &
Rotation4 <Type>::operator *= (const Rotation4 <T> & rotation)
{
	quaternion = rotation .quaternion * quaternion;
	return *this;
}

template <typename Type>
template <typename T>
Rotation4 <Type>
Rotation4 <Type>::multiply (const Rotation4 <T> & rotation) const
{
	return Rotation4 <Type> (rotation .quaternion * quaternion);
}

template <typename Type>
template <typename T>
Rotation4 <Type>
Rotation4 <Type>::operator * (const Rotation4 <T> & rotation) const
{
	return Rotation4 <Type> (rotation .quaternion * quaternion);
}

template <typename Type>
template <typename T>
Vector3 <Type>
Rotation4 <Type>::multVec (const Vector3 <T> & vector) const
{
	return quaternion * vector;
}

template <typename Type>
template <typename T>
Vector3 <Type>
Rotation4 <Type>::operator * (const Vector3 <T> & vector) const
{
	return quaternion * vector;
}

///  Extraction operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, Rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	return istream
	       >> x
	       >> y
	       >> z
	       >> angle;

	rotation .setValue (x, y, z, angle);

	return istream;
}

///  Insertion operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const Rotation4 <Type> & rotation)
{
	Type x, y, z, angle;

	rotation .getValue (x, y, z, angle);

	return ostream
	       << x << ' '
	       << y << ' '
	       << z << ' '
	       << angle;
}

} // X3D
} // titania

#endif
