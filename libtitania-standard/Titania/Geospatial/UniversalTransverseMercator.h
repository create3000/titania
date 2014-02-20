/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "BasicConverter.h"

namespace titania {
namespace geospatial {

///  @name Convert universal transverse mercator (utm) coordinates to geocentric coordinates
template <class Type>
class universal_transverse_mercator :
	public basic_converter <Type>
{
public:

	constexpr
	universal_transverse_mercator (const spheroid3 <Type> & spheroid) :
		a      (spheroid .a ()),
		a1_2   (a / 2),
		a2     (a * a),
		c2     (spheroid .c () * spheroid .c ()),
		f      (spheroid .f ()),
		eps2_f (f * (2 - f)),
		eps1_4 (eps2_f / 4)
	{ }

	virtual
	vector3 <Type>
	convert (const vector3 <Type> & geospatial) const final override
	{
		const Type latitude  = geospatial .x ();
		const Type longitude = geospatial .y ();
		const Type elevation = geospatial .z ();

		const Type slat  = std::sin (latitude);
		const Type slat2 = slat * slat;

		const Type clat = std::cos (latitude);

		const Type Rn   = a1_2 / std::sqrt (.25 - eps1_4 * slat2);
		const Type RnPh = Rn + elevation;

		return vector3 <Type> (RnPh * clat * std::cos (longitude),
		                       RnPh * clat * std::sin (longitude),
		                       (c2 / a2 * Rn + elevation) * slat);
	}

private:

	const Type a;
	const Type a1_2;
	const Type a2;
	const Type c2;
	const Type f;
	const Type eps2_f;
	const Type eps1_4;

};

extern template class universal_transverse_mercator <float>;
extern template class universal_transverse_mercator <double>;
extern template class universal_transverse_mercator <long double>;

} // geospatial
} // titania

#endif
