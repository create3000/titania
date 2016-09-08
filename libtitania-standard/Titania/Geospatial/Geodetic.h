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

#ifndef __TITANIA_GEOSPATIAL_GEODETIC_H__
#define __TITANIA_GEOSPATIAL_GEODETIC_H__

#include "../Math/Functional.h"
#include "BasicConverter.h"

namespace titania {
namespace geospatial {

/**
 *  Template to convert geodetic coordinates to geocentric coordinates and vice versa.
 *
 *  @param  Type  Type of vector values.
 */

template <class Type>
class geodetic :
	public basic_converter <Type>
{
public:

	constexpr
	geodetic (const spheroid3 <Type> & spheroid, const bool latitude_first = true, const bool radians = true) :
		longitude_first (not latitude_first),
		        degrees (not radians),
		              a (spheroid .semi_major_axis ()),
		              c (spheroid .semi_minor_axis ()),
		           c2a2 (sqr (c / a)),
		           ecc2 (1 - c2a2)
	{ }

	virtual
	vector3 <Type>
	convert (const vector3 <Type> & geospatial) const final override;

	virtual
	vector3 <Type>
	convert_radians (const vector3 <Type> & geospatial) const;

	virtual
	vector3 <Type>
	apply (const vector3 <Type> & geocentric) const final override;

	virtual
	vector3 <Type>
	apply_radians (const vector3 <Type> & geocentric) const;

	virtual
	vector3 <Type>
	normal (const vector3 <Type> & geocentric) const;


private:

	const bool longitude_first;
	const bool degrees;

	const Type a;
	const Type c;
	const Type c2a2;
	const Type ecc2;

};

// http://en.wikipedia.org/wiki/Geodetic_datum#From_geodetic_to_ECEF

template <class Type>
vector3 <Type>
geodetic <Type>::convert (const vector3 <Type> & geodetic) const
{
	Type       latitude  = geodetic .x ();
	Type       longitude = geodetic .y ();
	const Type elevation = geodetic .z ();

	if (longitude_first)
		std::swap (latitude, longitude);

	if (degrees)
	{
		latitude  = math::radians (latitude);
		longitude = math::radians (longitude);
	}

	return convert_radians (vector3 <Type> (latitude, longitude, elevation));
}

template <class Type>
vector3 <Type>
geodetic <Type>::convert_radians (const vector3 <Type> & geodetic) const
{
	const Type latitude  = geodetic .x ();
	const Type longitude = geodetic .y ();
	const Type elevation = geodetic .z ();

	const Type slat  = std::sin (latitude);
	const Type slat2 = sqr (slat);

	const Type clat = std::cos (latitude);

	const Type N   = a / std::sqrt (1 - ecc2 * slat2);
	const Type Nhl = (N + elevation) * clat;

	return vector3 <Type> (Nhl * std::cos (longitude),
	                       Nhl * std::sin (longitude),
	                       (N * c2a2 + elevation) * slat);
}

// http://www.ifp.uni-stuttgart.de/publications/software/gcl/index.en.html

template <class Type>
vector3 <Type>
geodetic <Type>::apply (const vector3 <Type> & geocentric) const
{
	const vector3 <Type> geodetic = apply_radians (geocentric);

	Type       latitude  = geodetic .x ();
	Type       longitude = geodetic .y ();
	const Type elevation = geodetic .z ();

	if (degrees)
	{
		latitude  = math::degrees (latitude);
		longitude = math::degrees (longitude);
	}

	if (longitude_first)
		return vector3 <Type> (longitude, latitude, elevation);

	return vector3 <Type> (latitude, longitude, elevation);
}

template <class Type>
vector3 <Type>
geodetic <Type>::apply_radians (const vector3 <Type> & geocentric) const
{
	static constexpr Type   EPS_H = 1e-3;
	static constexpr Type   EPS_P = 1e-10;
	static constexpr size_t IMAX  = 30;

	const Type x = geocentric .x ();
	const Type y = geocentric .y ();
	const Type z = geocentric .z ();

	const Type P = std::sqrt (x * x + y * y);

	if (P == 0)
		return vector3 <Type> (PI <Type>, 0, z - c);

	Type latitude  = 0;
	Type longitude = std::atan2 (y, x);
	Type elevation = 0;

	Type N = a;

	for (size_t i = 0; i < IMAX; ++ i)
	{
		const Type h0 = elevation;
		const Type b0 = latitude;

		latitude = std::atan (z / P / (1 - ecc2 * N / (N + elevation)));

		const Type sin_p = std::sin (latitude);

		N         = a / std::sqrt (1 - ecc2 * sin_p * sin_p);
		elevation = P / std::cos (latitude) - N;

		if (std::abs (elevation - h0) < EPS_H && std::abs (latitude - b0) < EPS_P)
			break;
	}

	return vector3 <Type> (latitude, longitude, elevation);
}

template <class Type>
vector3 <Type>
geodetic <Type>::normal (const vector3 <Type> & geocentric) const
{
	const vector3 <Type> geodetic = apply_radians (geocentric);

	Type latitude  = geodetic .x ();
	Type longitude = geodetic .y ();

	const Type clat = std::cos (latitude);

	const Type nx = std::cos (longitude) * clat;
	const Type ny = std::sin (longitude) * clat;
	const Type nz = std::sin (latitude);

	return vector3 <Type> (nx, ny, nz);
}

extern template class geodetic <float>;
extern template class geodetic <double>;
extern template class geodetic <long double>;

} // geospatial
} // titania

#endif
