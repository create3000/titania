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

#include "GeoPositionInterpolator.h"

#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Geospatial/Interpolation.h>

namespace titania {
namespace X3D {

const std::string GeoPositionInterpolator::componentName  = "Geospatial";
const std::string GeoPositionInterpolator::typeName       = "GeoPositionInterpolator";
const std::string GeoPositionInterpolator::containerField = "children";

GeoPositionInterpolator::Fields::Fields () :
	        keyValue (new MFVec3d ()),
	   value_changed (new SFVec3d ()),
	geovalue_changed (new SFVec3d ())
{ }

GeoPositionInterpolator::GeoPositionInterpolator (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DInterpolatorNode (),
	X3DGeospatialObject (),
	             fields ()
{
	addType (X3DConstants::GeoPositionInterpolator);

	addField (inputOutput,    "metadata",         metadata ());
	addField (initializeOnly, "geoOrigin",        geoOrigin ());
	addField (initializeOnly, "geoSystem",        geoSystem ());
	addField (inputOnly,      "set_fraction",     set_fraction ());
	addField (inputOutput,    "key",              key ());
	addField (inputOutput,    "keyValue",         keyValue ());
	addField (outputOnly,     "value_changed",    value_changed ());
	addField (outputOnly,     "geovalue_changed", geovalue_changed ());

	keyValue ()         .setUnit (UnitCategory::LENGTH);
	value_changed ()    .setUnit (UnitCategory::LENGTH);
	geovalue_changed () .setUnit (UnitCategory::LENGTH);

	geovalue_changed () .isGeospatial (true);
}

X3DBaseNode*
GeoPositionInterpolator::create (X3DExecutionContext* const executionContext) const
{
	return new GeoPositionInterpolator (executionContext);
}

void
GeoPositionInterpolator::setup ()
{
	X3DGeospatialObject::initialize ();
	X3DInterpolatorNode::setup ();
}

void
GeoPositionInterpolator::initialize ()
{
	X3DInterpolatorNode::initialize ();

	keyValue () .addInterest (&GeoPositionInterpolator::set_keyValue, this);
}

void
GeoPositionInterpolator::set_keyValue ()
{
	if (keyValue () .size () < key () .size ())
		keyValue () .resize (key () .size (), keyValue () .size () ? keyValue () .back () : Vector3d ());
}

void
GeoPositionInterpolator::interpolate (size_t index0, size_t index1, const float weight)
{
	try
	{
		const auto keyValue0 = getCoord (keyValue () .get1Value (index0));
		const auto keyValue1 = getCoord (keyValue () .get1Value (index1));
		const auto coord     = geospatial::gc_lerp (keyValue0, keyValue1, double (weight));
	
		geovalue_changed () = getGeoCoord (coord);
		value_changed ()    = coord;
	}
	catch (const std::domain_error &)
	{ }
}

void
GeoPositionInterpolator::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DInterpolatorNode::dispose ();
}

} // X3D
} // titania
