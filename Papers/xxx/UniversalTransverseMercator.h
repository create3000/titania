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

#ifndef __TITANIA_GEOSPATIAL_UNIVERSAL_TRANSVERSE_MERCATOR_H__
#define __TITANIA_GEOSPATIAL_UNIVERSAL_TRANSVERSE_MERCATOR_H__

#include "../Math/Functional.h"
#include "Geodetic.h"

namespace titania {
namespace geospatial {

///  @name Convert universal transverse mercator (utm) coordinates to geocentric coordinates
template <class Type>
class universal_transverse_mercator :
	public basic_converter <Type>
{
public:

	constexpr
	universal_transverse_mercator (const spheroid3 <Type> & spheroid, const int zone, const bool hemisphere_north = true, const bool easting_first = false) :
		              zone (zone),
		  hemisphere_north (hemisphere_north),
		     easting_first (easting_first),
		                 a (spheroid .a ()),
		                 c (spheroid .c ()),
		                ee (1 - sqr (c / a)),
		                EE (ee / (1 - ee)),
		                e1 ((1 - std::sqrt (1 - ee)) / (1 + std::sqrt (1 - ee))),
		               E11 (3 * e1 / 2 - 7 * e1 * e1 * e1 / 32),
		               E22 (21 * e1 * e1 / 16 - 55 * e1 * e1 * e1 * e1 / 32),
		               E33 (151 * e1 * e1 * e1 / 96),
		               E44 (a * (1 - ee / 4 - 3 * ee * ee / 64 - 5 * ee * ee * ee / 256)),
		geodetic_converter (spheroid)
	{ }

	virtual
	vector3 <Type>
	convert (const vector3 <Type> & utm) const final override
	{
		static constexpr Type N0 = 1.0e7;
		static constexpr Type E0 = 5.0e5;
		static constexpr Type k0 = 0.9996;

		Type       northing = easting_first ? utm .y () : utm .x ();
		const Type easting  = easting_first ? utm .x () : utm .y ();

		bool hn = hemisphere_north;

		if (northing < 0)
		{
			hn       = false;
			northing = -northing;
		}

		if (not hn)
			northing = northing - N0;

		const Type y          = northing;
		const Type x          = easting - E0;       //remove 500,000 meter offset for longitude
		const Type longOrigin = radians (zone * 6 - 183); //origin in middle of zone

		const Type M  = y / k0;
		const Type mu = M / E44;

		const Type phi1Rad = mu + E11* std::sin (2 * mu) + E22* std::sin (4 * mu) + E33* std::sin (6 * mu);

		const Type sphi1Rad2 = sqr (std::sin (phi1Rad));
		const Type cphi1Rad  = std::cos (phi1Rad);
		const Type tphi1Rad  = std::tan (phi1Rad);

		const Type N1 = a / std::sqrt (1 - ee * sphi1Rad2);
		const Type T1 = sqr (tphi1Rad);
		const Type C1 = EE * sqr (cphi1Rad);
		const Type R1 = a * (1 - ee) / std::pow (1 - ee * sphi1Rad2, 1.5);
		const Type D  = x / (N1 * k0);

		const Type latitude = phi1Rad - (N1 * tphi1Rad / R1) * (D * D / 2 - (5 + 3 * T1 + 10 * C1 - 4 * C1 * C1 - 9 * EE) * std::pow (D, 4) / 24
		                                                        + (61 + 90 * T1 + 298 * C1 + 45 * T1 * T1 - 252 * EE - 3 * C1 * C1) * std::pow (D, 6) / 720);

		const Type longitude = longOrigin + (D - (1 + 2 * T1 + C1) * std::pow (D, 3) / 6 + (5 - 2 * C1 + 28 * T1 - 3 * C1 * C1 + 8 * EE + 24 * T1 * T1) * std::pow (D, 5) / 120) / cphi1Rad;

		return geodetic_converter (vector3 <Type> (latitude, longitude, utm .z ()));
	}

private:

	const Type zone;
	const bool hemisphere_north;
	const bool easting_first;

	const Type a;
	const Type c;

	const Type ee;
	const Type EE;
	const Type e1;

	const Type E11;
	const Type E22;
	const Type E33;
	const Type E44;

	geodetic <Type> geodetic_converter;

};

/////  @name Convert universal transverse mercator (utm) coordinates to geocentric coordinates
//template <class Type>
//class universal_transverse_mercator :
//	public basic_converter <Type>
//{
//public:
//
//	constexpr
//	universal_transverse_mercator (const spheroid3 <Type> & spheroid, const int zone, const bool hemisphere_north = true, const bool easting_first = false) :
//		              zone (zone),
//		  hemisphere_north (hemisphere_north),
//		     easting_first (easting_first),
//		                 a (spheroid .a ()),
//		                 f (spheroid .f ()),
//		geodetic_converter (spheroid)
//	{ }
//
//	virtual
//	vector3 <Type>
//	convert (const vector3 <Type> & utm) const final override
//	{
//		Type       northing = easting_first ? utm .y () : utm .x ();
//		const Type easting  = easting_first ? utm .x () : utm .y ();
//
//		bool hn = hemisphere_north;
//
//		if (northing < 0)
//		{
//			hn       = false;
//			northing = -northing;
//		}
//
//		if (not hn)
//			northing = northing - 1.0e7;
//
//		static constexpr Type N0 = 1.0e7;
//		static constexpr Type E0 = 5.0e5;
//		static constexpr Type k0 = 0.9996;
//
//		const Type N = f / (2 - f);
//
//		const Type A = a / (1 + N) * (1 + std::pow (N, 2) / 4 + std::pow (N, 4) / 64);
//
//		const Type b [3] = {
//			N / 2.0 - 2.0 / 3.0 * N * N + 37.0 / 96.0 * N * N * N,
//			N * N / 48.0 + N * N * N / 15.0,
//			17.0 / 480.0 * N * N * N
//		};
//
//		const Type d [3] = {
//			2 * N - 2.0 / 3.0 * N * N - 2 * N * N * N,
//			7.0 / 3.0 * N * N - 8.0 / 5.0 * N * N * N,
//			56.0 / 15.0 * N * N * N
//		};
//
//		const Type e = (northing /* - N0 */) / (k0 * A);
//		const Type n = (easting - E0) / (k0 * A);
//
//		Type te = 0;
//
//		for (int j = 1; j < 4; ++ j)
//			te += b [j - 1] * std::sin (2 * j * e) * std::cosh (2 * j * n);
//
//		const Type ee = e - te;
//
//		Type tn = 0;
//
//		for (int j = 1; j < 4; ++ j)
//			tn += b [j - 1] * std::cos (2 * j * e) * std::sinh (2 * j * n);
//
//		const Type nn = n - tn;
//
//		const Type x = std::asin (std::sin (ee) / std::cosh (nn));
//
//		Type tx = 0;
//
//		for (int j = 1; j < 4; ++ j)
//			tx += d [j - 1] * std::sin (2 * j * x);
//
//		const Type latitude = x + tx;
//
//		const Type longitude0 = radians (zone * 6 - 183);
//		const Type longitude  = longitude0 + std::atan (std::sinh (nn) / std::cos (ee));
//
//		__LOG__ << degrees (latitude) << std::endl;
//		__LOG__ << degrees (longitude) << std::endl;
//
//		return geodetic_converter (vector3 <Type> (latitude, longitude, utm .z ()));
//	}
//
//private:
//
//	const Type zone;
//	const bool hemisphere_north;
//	const bool easting_first;
//
//	const Type a;
//	const Type f;
//
//	geodetic <Type> geodetic_converter;
//
//};

extern template class universal_transverse_mercator <float>;
extern template class universal_transverse_mercator <double>;
extern template class universal_transverse_mercator <long double>;

} // geospatial
} // titania

#endif
