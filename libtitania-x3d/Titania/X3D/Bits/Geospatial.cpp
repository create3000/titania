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

#include "Geospatial.h"

#include <Titania/Geospatial/Geodetic.h>
#include <Titania/Geospatial/ReferenceEllipsoids.h>
#include <Titania/Geospatial/UniversalTransverseMercator.h>

namespace titania {
namespace X3D {

using Geodetic                    = geospatial::geodetic <double>;
using UniversalTransverseMercator = geospatial::universal_transverse_mercator <double>;

const std::map <std::string, Geospatial::CoordinateSystemType> Geospatial::coordinateSystems = {
	std::make_pair ("GD",  CoordinateSystemType::GD),
	std::make_pair ("GDC", CoordinateSystemType::GD),
	std::make_pair ("UTM", CoordinateSystemType::UTM),
	std::make_pair ("GC",  CoordinateSystemType::GC),
	std::make_pair ("GCC", CoordinateSystemType::GC),
	std::make_pair ("GS",  CoordinateSystemType::GC)

};

const std::map <std::string, Spheroid3d> Geospatial::ellipsoids = {
	std::make_pair ("AA", geospatial::AA),
	std::make_pair ("AM", geospatial::AM),
	std::make_pair ("AN", geospatial::AN),
	std::make_pair ("BN", geospatial::BN),
	std::make_pair ("BR", geospatial::BR),
	std::make_pair ("CC", geospatial::CC),
	std::make_pair ("CD", geospatial::CD),
	std::make_pair ("EA", geospatial::EA),
	std::make_pair ("EB", geospatial::EB),
	std::make_pair ("EC", geospatial::EC),
	std::make_pair ("ED", geospatial::ED),
	std::make_pair ("EE", geospatial::EE),
	std::make_pair ("EF", geospatial::EF),
	std::make_pair ("FA", geospatial::FA),
	std::make_pair ("HE", geospatial::HE),
	std::make_pair ("HO", geospatial::HO),
	std::make_pair ("ID", geospatial::ID),
	std::make_pair ("IN", geospatial::IN),
	std::make_pair ("KA", geospatial::KA),
	std::make_pair ("RF", geospatial::RF),
	std::make_pair ("SA", geospatial::SA),
	std::make_pair ("WD", geospatial::WD),
	std::make_pair ("WE", geospatial::WE)

};

Geospatial::ReferenceFramePtr
Geospatial::getReferenceFrame (const MFString & geoSystem)
{
	switch (getCoordinateSystem (geoSystem))
	{
		case CoordinateSystemType::GD:
		{
			const bool longitude_first = getLongitudeFirst (geoSystem);

			return ReferenceFramePtr (new Geodetic (getEllipsoid (geoSystem), longitude_first));
		}
		case CoordinateSystemType::UTM:
		{
			return ReferenceFramePtr (new UniversalTransverseMercator (geospatial::WE));
		}
		case CoordinateSystemType::GC:
		{
			return ReferenceFramePtr (new Geocentric ());
		}
	}

	return ReferenceFramePtr (new Geodetic (geospatial::WE));
}

Geospatial::CoordinateSystemType
Geospatial::getCoordinateSystem (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		const auto coordinateSystem = coordinateSystems .find (string);

		if (coordinateSystem not_eq coordinateSystems .end ())
			return coordinateSystem -> second;
	}

	return CoordinateSystemType::GD;
}

Spheroid3d
Geospatial::getEllipsoid (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		const auto ellipsoid = ellipsoids .find (string);

		if (ellipsoid not_eq ellipsoids .end ())
			return ellipsoid -> second;
	}

	return geospatial::WE;
}

bool
Geospatial::getLongitudeFirst (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		if (string == "longitude_first")
			return true;
	}

	return false;
}

} // X3D
} // titania
