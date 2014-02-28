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

#ifndef __TITANIA_GEOSPATIAL_INTERPOLATION_H__
#define __TITANIA_GEOSPATIAL_INTERPOLATION_H__

#include "../Math/Functional.h"
#include "../Math/Numbers/Vector3.h"

namespace titania {
namespace geospatial {

using namespace titania::math;

template <class Type, class T>

///  Spherical linear interpolate between two vectors @a source vector
///  and @a destination vector by an amout of @a t. The vectors must not
///  be normalized.
inline
Type
vector_slerp (Type source, Type destination, const T & t)
{
	const T source_length      = abs (source);
	const T destination_length = abs (destination);

	source .normalize ();
	destination .normalize ();

	return slerp (source, destination, t) * lerp (source_length, destination_length, t);
}

///  Spherical linear interpolate between two geodetic vectors @a source
///  and @a destination by an amout of @a t.
template <class Type>
vector3 <Type>
gd_lerp (const vector3 <Type> & source, const vector3 <Type> & destination, const Type & t, const bool longitude_first = false)
{
	const Type range = longitude_first
	                   ? std::abs (destination .x () - source .x ())
							 : std::abs (destination .y () - source .y ());

	if (range <= Type (M_PI))
		return lerp (source, destination, t);

	const Type step = (Type (M_PI2) - range) * t;

	if (longitude_first)
	{
		Type longitude = source .x () < destination .x () ? source .x () - step : source .x () + step;

		if (longitude < 0)
			longitude += Type (M_PI2);

		else if (longitude > Type (M_PI2))
			longitude -= Type (M_PI2);

		return vector3 <Type> (longitude,
		                       lerp (source .y (), destination .y (), t),
		                       lerp (source .z (), destination .z (), t));
	}
	else
	{
		Type longitude = source .y () < destination .y () ? source .y () - step : source .y () + step;

		if (longitude < 0)
			longitude += Type (M_PI2);

		else if (longitude > Type (M_PI2))
			longitude -= Type (M_PI2);

		return vector3 <Type> (lerp (source .x (), destination .x (), t),
		                       longitude,
		                       lerp (source .z (), destination .z (), t));
	}
}

///  Spherical linear interpolate between two utm vectors @a source
///  and @a destination by an amout of @a t.
template <class Type>
inline
constexpr vector3 <Type>
utm_lerp (const vector3 <Type> & source, const vector3 <Type> & destination, const Type & t)
{
	return lerp (source, destination, t);
}

///  Spherical linear interpolate between two geocentrc vectors @a source
///  and @a destination by an amout of @a t.
template <class Type>
inline
vector3 <Type>
gc_lerp (const vector3 <Type> & source, const vector3 <Type> & destination, const Type & t)
{
	return vector_slerp (source, destination, t);
}

} // geospatial
} // titania

#endif
