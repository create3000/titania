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

#include "Geospatial.h"

#include <Titania/Geospatial/Geodetic.h>
#include <Titania/Geospatial/ReferenceEllipsoids.h>
#include <Titania/Geospatial/UniversalTransverseMercator.h>

#include <regex>

namespace titania {
namespace X3D {

using Geodetic                    = geospatial::geodetic <double>;
using UniversalTransverseMercator = geospatial::universal_transverse_mercator <double>;

const std::map <std::string, Geospatial::CoordinateSystemType> Geospatial::coordinateSystems = {
	std::pair ("GD",  CoordinateSystemType::GD),
	std::pair ("GDC", CoordinateSystemType::GD),
	std::pair ("UTM", CoordinateSystemType::UTM),
	std::pair ("GC",  CoordinateSystemType::GC),
	std::pair ("GCC", CoordinateSystemType::GC),
	std::pair ("GS",  CoordinateSystemType::GC)

};

const std::map <std::string, Spheroid3d> Geospatial::ellipsoids = {
	std::pair ("AA", geospatial::AA),
	std::pair ("AM", geospatial::AM),
	std::pair ("AN", geospatial::AN),
	std::pair ("BN", geospatial::BN),
	std::pair ("BR", geospatial::BR),
	std::pair ("CC", geospatial::CC),
	std::pair ("CD", geospatial::CD),
	std::pair ("EA", geospatial::EA),
	std::pair ("EB", geospatial::EB),
	std::pair ("EC", geospatial::EC),
	std::pair ("ED", geospatial::ED),
	std::pair ("EE", geospatial::EE),
	std::pair ("EF", geospatial::EF),
	std::pair ("FA", geospatial::FA),
	std::pair ("HE", geospatial::HE),
	std::pair ("HO", geospatial::HO),
	std::pair ("ID", geospatial::ID),
	std::pair ("IN", geospatial::IN),
	std::pair ("KA", geospatial::KA),
	std::pair ("RF", geospatial::RF),
	std::pair ("SA", geospatial::SA),
	std::pair ("WD", geospatial::WD),
	std::pair ("WE", geospatial::WE)

};

Geospatial::ReferenceFramePtr
Geospatial::getReferenceFrame (const MFString & geoSystem, const bool radians)
{
	switch (getCoordinateSystem (geoSystem))
	{
		case CoordinateSystemType::GD:
		{
			return ReferenceFramePtr (new Geodetic (getEllipsoid (geoSystem),
			                                        getLatitudeFirst (geoSystem),
			                                        radians));
		}
		case CoordinateSystemType::UTM:
		{
			return ReferenceFramePtr (new UniversalTransverseMercator (getEllipsoid (geoSystem),
			                                                           getZone (geoSystem),
			                                                           getNorthernHemisphere (geoSystem),
			                                                           getNorthingFirst (geoSystem)));
		}
		case CoordinateSystemType::GC:
		{
			return ReferenceFramePtr (new Geocentric ());
		}
	}

	return ReferenceFramePtr (new Geodetic (geospatial::WE, true, radians));
}

Geospatial::ElevationFramePtr
Geospatial::getElevationFrame (const MFString & geoSystem, const bool radians)
{
	return ElevationFramePtr (new Geodetic (getEllipsoid (geoSystem), true, radians));
}

Geospatial::CoordinateSystemType
Geospatial::getCoordinateSystem (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		try
		{
			return coordinateSystems .at (string);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return CoordinateSystemType::GD;
}

Spheroid3d
Geospatial::getEllipsoid (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		try
		{
			return ellipsoids .at (string);
		}
		catch (const std::out_of_range &)
		{ }
	}

	return geospatial::WE;
}

std::string
Geospatial::getEllipsoidString (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		if (ellipsoids .count (string))
			return string;
	}

	return "WE";
}

bool
Geospatial::isStandardOrder (const MFString & geoSystem)
{
	switch (getCoordinateSystem (geoSystem))
	{
		case CoordinateSystemType::GD:
		{
			return getLatitudeFirst (geoSystem);
		}
		case CoordinateSystemType::UTM:
		{
			return getNorthingFirst (geoSystem);
		}
		case CoordinateSystemType::GC:
		{
			return true;
		}
	}

	return getLatitudeFirst (geoSystem);
}

bool
Geospatial::getLatitudeFirst (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		if (string == "longitude_first")
			return false;
	}

	return true;
}

bool
Geospatial::getNorthingFirst (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		if (string == "easting_first")
			return false;
	}

	return true;
}

int32_t
Geospatial::getZone (const MFString & geoSystem)
{
	static const std::regex Zone (R"/(Z(\d+))/");

	for (const auto & string : geoSystem)
	{
		std::smatch zone;

		if (std::regex_match (string .raw (), zone, Zone))
			return std::atoi (zone .str (1) .c_str ());
	}

	return 1;
}

bool
Geospatial::getNorthernHemisphere (const MFString & geoSystem)
{
	for (const auto & string : geoSystem)
	{
		if (string == "S")
			return false;
	}

	return true;
}

} // X3D
} // titania
