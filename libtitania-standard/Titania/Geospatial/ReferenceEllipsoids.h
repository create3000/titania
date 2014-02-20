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

#ifndef __TITANIA_GEOSPATIAL_REFERENCE_ELLIPSOIDS_H__
#define __TITANIA_GEOSPATIAL_REFERENCE_ELLIPSOIDS_H__

#include "../Math/Geometry/Spheroid3.h"

namespace titania {
namespace geospatial {

using namespace titania::math;

constexpr spheroid3 <double> AA (6377563.396, 299.3249646,   inv_flattening_type ()); // Airy 1830
constexpr spheroid3 <double> AM (6377340.189, 299.3249646,   inv_flattening_type ()); // Modified Airy
constexpr spheroid3 <double> AN (6378160,     298.25,        inv_flattening_type ()); // Australian National
constexpr spheroid3 <double> BN (6377483.865, 299.1528128,   inv_flattening_type ()); // Bessel 1841 (Namibia)
constexpr spheroid3 <double> BR (6377397.155, 299.1528128,   inv_flattening_type ()); // Bessel 1841 (Ethiopia Indonesia...)
constexpr spheroid3 <double> CC (6378206.4,   294.9786982,   inv_flattening_type ()); // Clarke 1866
constexpr spheroid3 <double> CD (6378249.145, 293.465,       inv_flattening_type ()); // Clarke 1880
constexpr spheroid3 <double> EA (6377276.345, 300.8017,      inv_flattening_type ()); // Everest (India 1830)
constexpr spheroid3 <double> EB (6377298.556, 300.8017,      inv_flattening_type ()); // Everest (Sabah & Sarawak)
constexpr spheroid3 <double> EC (6377301.243, 300.8017,      inv_flattening_type ()); // Everest (India 1956)
constexpr spheroid3 <double> ED (6377295.664, 300.8017,      inv_flattening_type ()); // Everest (W. Malaysia 1969)
constexpr spheroid3 <double> EE (6377304.063, 300.8017,      inv_flattening_type ()); // Everest (W. Malaysia & Singapore 1948)
constexpr spheroid3 <double> EF (6377309.613, 300.8017,      inv_flattening_type ()); // Everest (Pakistan)
constexpr spheroid3 <double> FA (6378155,     298.3,         inv_flattening_type ()); // Modified Fischer 1960
constexpr spheroid3 <double> HE (6378200,     298.3,         inv_flattening_type ()); // Helmert 1906
constexpr spheroid3 <double> HO (6378270,     297,           inv_flattening_type ()); // Hough 1960
constexpr spheroid3 <double> ID (6378160,     298.247,       inv_flattening_type ()); // Indonesian 1974
constexpr spheroid3 <double> IN (6378388,     297,           inv_flattening_type ()); // International 1924
constexpr spheroid3 <double> KA (6378245,     298.3,         inv_flattening_type ()); // Krassovsky 1940
constexpr spheroid3 <double> RF (6378137,     298.257222101, inv_flattening_type ()); // Geodetic Reference System 1980 (GRS 80)
constexpr spheroid3 <double> SA (6378160,     298.25,        inv_flattening_type ()); // South American 1969
constexpr spheroid3 <double> WD (6378135,     298.26,        inv_flattening_type ()); // WGS 72
constexpr spheroid3 <double> WE (6378137,     298.257223563, inv_flattening_type ()); // WGS 84

} // geospatial
} // titania

#endif
