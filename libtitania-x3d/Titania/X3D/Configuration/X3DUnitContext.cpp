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

#include "X3DUnitContext.h"

namespace titania {
namespace X3D {

const UnitIndex X3DUnitContext::unitCategories = {
	std::make_pair ("angle",  UnitCategory::ANGLE),
	std::make_pair ("force",  UnitCategory::FORCE),
	std::make_pair ("length", UnitCategory::LENGTH),
	std::make_pair ("mass",   UnitCategory::MASS)

};

const UnitArray X3DUnitContext::standardUnits = {
	Unit ("none",   "none",     1),
	Unit ("angle",  "radian",   1),
	Unit ("force",  "newton",   1),
	Unit ("length", "metre",    1),
	Unit ("mass",   "kilogram", 1)

};

X3DUnitContext::X3DUnitContext () :
	X3DBaseNode (),
	      units (standardUnits),
	unitsOutput ()
{
	addChildren (unitsOutput);
}

void
X3DUnitContext::initialize ()
{ }

void
X3DUnitContext::updateUnit (const std::string & category, const std::string & name, const double conversionFactor)
throw (Error <INVALID_NAME>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	try
	{
		const auto & unitCategory = unitCategories .at (category);

		units [size_t (unitCategory)] = Unit (category, name, conversionFactor);

		unitsOutput = getCurrentTime ();
	}
	catch (const std::out_of_range &)
	{
		throw Error <INVALID_NAME> ("Unkown standard unit category '" + category + "'.");
	}
}

const Unit &
X3DUnitContext::getUnit (const UnitCategory category) const
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return units [size_t (category)];
}

double
X3DUnitContext::fromUnit (const UnitCategory category, const double value) const
throw (Error <DISPOSED>)
{
	switch (category)
	{
	   case UnitCategory::NONE:
			return value;
	   
		// Base units
	   case UnitCategory::ANGLE:
	   case UnitCategory::FORCE:
	   case UnitCategory::LENGTH:
	   case UnitCategory::MASS:
			return value / getUnit (category) .getConversionFactor ();
	
		// Derived units
		case UnitCategory::ACCELERATION:
		case UnitCategory::ANGULAR_RATE:
		case UnitCategory::AREA:
		case UnitCategory::SPEED:
		case UnitCategory::VOLUME:
			return value;
	}

	return value;
}

double
X3DUnitContext::toUnit (const UnitCategory category, const double value) const
throw (Error <DISPOSED>)
{
	switch (category)
	{
	   case UnitCategory::NONE:
			return value;
	   
		// Base units
	   case UnitCategory::ANGLE:
	   case UnitCategory::FORCE:
	   case UnitCategory::LENGTH:
	   case UnitCategory::MASS:
			return value * getUnit (category) .getConversionFactor ();
	
		// Derived units
		case UnitCategory::ACCELERATION:
		case UnitCategory::ANGULAR_RATE:
		case UnitCategory::AREA:
		case UnitCategory::SPEED:
		case UnitCategory::VOLUME:
			return value;
	}

	return value;
}

double
X3DUnitContext::fromRadians (const double value) const
throw (Error <DISPOSED>)
{
	return value / getUnit (UnitCategory::ANGLE) .getConversionFactor ();
}

double
X3DUnitContext::toRadians (const double value) const
throw (Error <DISPOSED>)
{
	return value * getUnit (UnitCategory::ANGLE) .getConversionFactor ();
}

double
X3DUnitContext::fromNewton (const double value) const
throw (Error <DISPOSED>)
{
	return value / getUnit (UnitCategory::FORCE) .getConversionFactor ();
}

double
X3DUnitContext::toNewton (const double value) const
throw (Error <DISPOSED>)
{
	return value * getUnit (UnitCategory::FORCE) .getConversionFactor ();
}

double
X3DUnitContext::fromMetre (const double value) const
throw (Error <DISPOSED>)
{
	return value / getUnit (UnitCategory::LENGTH) .getConversionFactor ();
}

double
X3DUnitContext::toMetre (const double value) const
throw (Error <DISPOSED>)
{
	return value * getUnit (UnitCategory::LENGTH) .getConversionFactor ();
}

double
X3DUnitContext::fromKilogram (const double value) const
throw (Error <DISPOSED>)
{
	return value / getUnit (UnitCategory::MASS) .getConversionFactor ();
}

double
X3DUnitContext::toKilogram (const double value) const
throw (Error <DISPOSED>)
{
	return value * getUnit (UnitCategory::MASS) .getConversionFactor ();
}

void
X3DUnitContext::dispose ()
{
}

X3DUnitContext::~X3DUnitContext ()
{ }

} // X3D
} // titania
