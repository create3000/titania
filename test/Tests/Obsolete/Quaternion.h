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

#ifndef __TESTS_OBSOLETE_QUATERNION_H__
#define __TESTS_OBSOLETE_QUATERNION_H__

#include <cmath>
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>

#include <Titania/Basic/Vector3.h>

namespace titania {
namespace Obsolete {

using namespace titania::basic;

template <typename Type>
class Quaternion
{
public:

	typedef Type   value_type;
	typedef size_t size_type;

	Quaternion ();

	Quaternion (const Type &, const Type &, const Type &, const Type &);

	template <typename T>
	Quaternion (const Quaternion <T> &);

	void
	setAxisAngle (const Type &, const Type &, const Type &, const Type &);

	template <typename T>
	void
	getAxisAngle (T &, T &, T &, T &) const;

	template <typename T>
	void
	getAxis (T &, T &, T &) const;

	Type
	getAngle () const;

	template <typename T>
	Quaternion
	slerp (const Quaternion <T> &, const Type &) const;

	Type &
	operator [ ] (const size_type);

	const Type &
	operator [ ] (const size_type) const;

	template <typename T>
	bool
	operator == (const Quaternion <T> &) const;

	template <typename T>
	bool
	operator not_eq (const Quaternion <T> &) const;

	template <typename T>
	Quaternion &
	operator = (const Quaternion <T> &);

	Quaternion
	operator ~ () const;

	template <typename T>
	Quaternion
	operator * (const Quaternion <T> &) const;

	template <typename T>
	Vector3 <Type>
	operator * (const Vector3 <T> &) const;

	template <typename T>
	Type
	dot (const Quaternion <T> &) const;

	Quaternion
	normalize () const;

	Quaternion
	operator - () const;

	Quaternion
	operator + (const Quaternion &) const;

	Quaternion
	operator* (const Type &) const;

	Quaternion
	operator / (const Type &) const;

	Type x;
	Type y;
	Type z;
	Type w;

};

template <typename Type>
Quaternion <Type>::Quaternion () :
	x (0), y (0), z (0), w (1)
{ }

template <typename Type>
Quaternion <Type>::Quaternion (const Type & x, const Type & y, const Type & z, const Type & w) :
	x (x), y (y), z (z), w (w)
{ }

template <typename Type>
template <typename T>
Quaternion <Type>::Quaternion (const Quaternion <T> & quaternion) :
	x (quaternion .x), y (quaternion .y), z (quaternion .z), w (quaternion .w)
{ }

template <typename Type>
void
Quaternion <Type>::setAxisAngle (const Type & x, const Type & y, const Type & z, const Type & angle)
{
	Type scale = std::sqrt (x * x + y * y + z * z);

	if (scale == 0)
	{
		this -> x = 0;
		this -> y = 0;
		this -> z = 0;
		this -> w = 1;
		return;
	}

	// calculate
	Type halfTheta = angle / 2;
	scale     = std::sin (halfTheta) / scale;
	this -> x = x * scale;
	this -> y = y * scale;
	this -> z = z * scale;
	this -> w = std::cos (halfTheta);
}

template <typename Type>
template <typename T>
void
Quaternion <Type>::getAxisAngle (T & x, T & y, T & z, T & angle) const
{
	Quaternion <Type> n = normalize ();

	if (n .w == 1)
	{
		x     = 0;
		y     = 0;
		z     = 1;
		angle = 0;
	}
	else
	{
		Type scale = std::sqrt (n .x * n .x + n .y * n .y + n .z * n .z);
		x     = n .x / scale;
		y     = n .y / scale;
		z     = n .z / scale;
		angle = 2 * std::acos (n .w);
	}
}

template <typename Type>
template <typename T>
void
Quaternion <Type>::getAxis (T & x, T & y, T & z) const
{
	Quaternion <Type> n = normalize ();

	if (n .w == 1)
	{
		x = 0;
		y = 0;
		z = 1;
	}
	else
	{
		Type scale = std::sqrt (n .x * n .x + n .y * n .y + n .z * n .z);
		x = n .x / scale;
		y = n .y / scale;
		z = n .z / scale;
	}
}

template <typename Type>
Type
Quaternion <Type>::getAngle () const
{
	Quaternion <Type> n = normalize ();

	if (n .w == 1)
	{
		return 0;
	}
	else
	{
		return 2 * std::acos (n .w);
	}
}

template <typename Type>
template <typename T>
Quaternion <Type>
Quaternion <Type>::slerp (const Quaternion <T> & dest, const Type & t) const
{
	Quaternion <T> _dest;

	Type dotprod = dot (dest);

	if (dotprod < 0)
	{
		// Reverse signs so we travel the short way round
		dotprod = -dotprod;
		_dest   = -dest;
	}
	else
		_dest = dest;

	Type scale0, scale1;

	/* calculate coefficients */
	if ((1 - dotprod) > 1e-5)
	{
		/* standard case (SLERP) */
		Type omega = std::acos (dotprod);
		Type sinom = std::sin (omega);
		scale0 = std::sin ((1 - t) * omega) / sinom;
		scale1 = std::sin (t * omega) / sinom;
	}
	else
	{
		/* q1 & q2 are very close, so do linear interpolation */
		scale0 = 1 - t;
		scale1 = t;
	}

	return *this * scale0 + _dest * scale1;
}

template <typename Type>
Type &
Quaternion <Type>::operator [ ] (const size_type index)
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
const
Type &
Quaternion <Type>::operator [ ] (const size_type index) const
{
	switch (index)
	{
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}

	throw std::out_of_range ("index out of range");
}

template <typename Type>
template <typename T>
bool
Quaternion <Type>::operator == (const Quaternion <T> & value) const
{
	return
	   w == value .w and
	   x == value .x and
	   y == value .y and
	   z == value .z;
}

template <typename Type>
template <typename T>
bool
Quaternion <Type>::operator not_eq (const Quaternion <T> & value) const
{
	return not (*this == value);
}

template <typename Type>
template <typename T>
Quaternion <Type> &
Quaternion <Type>::operator = (const Quaternion <T> & value)
{
	x = value .x;
	y = value .y;
	z = value .z;
	w = value .w;
	return *this;
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::operator ~ () const
{
	return Quaternion <Type> (-x, -y, -z, w);
}

template <typename Type>
template <typename T>
Quaternion <Type>
Quaternion <Type>::operator * (const Quaternion <T> & quat) const
{
	return Quaternion <Type> (w * quat .x
	                          + x * quat .w
	                          + y * quat .z - z * quat .y,

	                          w * quat .y
	                          + y * quat .w
	                          + z * quat .x - x * quat .z,

	                          w * quat .z
	                          + z * quat .w
	                          + x * quat .y - y * quat .x,

	                          w * quat .w
	                          - x * quat .x
	                          - y * quat .y
	                          - z * quat .z);
}

template <typename Type>
template <typename T>
Vector3 <Type>
Quaternion <Type>::operator * (const Vector3 <T> & vector) const
{
	Quaternion <Type> p (vector .x (), vector .y (), vector .z (), 1);
	Quaternion <Type> qq = *this * (p * ~*this);
	return Vector3 <Type> (qq .x, qq .y, qq .z);
}

template <typename Type>
template <typename T>
Type
Quaternion <Type>::dot (const Quaternion <T> & quat) const
{
	return x * quat .x + y * quat .y + z * quat .z + w * quat .w;
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::normalize () const
{
	Type length = std::sqrt (x * x + y * y + z * z + w * w);

	return length ? *this / length : *this;
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::operator - () const
{
	return Quaternion <Type> (-x, -y, -z, -w);
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::operator + (const Quaternion <Type> & quat) const
{
	return Quaternion <Type> (x + quat .x,
	                          y + quat .y,
	                          z + quat .z,
	                          w + quat .w);
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::operator * (const Type & value) const
{
	return Quaternion <Type> (x * value,
	                          y * value,
	                          z * value,
	                          w * value);
}

template <typename Type>
Quaternion <Type>
Quaternion <Type>::operator / (const Type & value) const
{
	return Quaternion <Type> (x / value,
	                          y / value,
	                          z / value,
	                          w / value);
}

///  Extraction operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, Quaternion <Type> & quaternion)
{
	return istream;
}

///  Insertion operator for vector values.
template <typename CharT, class Traits, typename Type>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const Quaternion <Type> & quaternion)
{
	return ostream
	       << quaternion .x << ' '
	       << quaternion .y << ' '
	       << quaternion .z << ' '
	       << quaternion .w;
}

} // X3D
} // titania

#endif
