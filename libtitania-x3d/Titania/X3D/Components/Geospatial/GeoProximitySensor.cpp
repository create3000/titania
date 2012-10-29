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

#include "GeoProximitySensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

GeoProximitySensor::GeoProximitySensor (X3DExecutionContext* const executionContext) :
	              X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentalSensorNode (),                                 
	                 geoCenter (),                                 // SFVec3d    [in,out] geoCenter                 0 0 0              (-∞,∞)
	  centerOfRotation_changed (),                                 // SFVec3f    [out]    centerOfRotation_changed
	          geoCoord_changed (),                                 // SFVec3d    [out]    geoCoord_changed
	       orientation_changed (),                                 // SFRotation [out]    orientation_changed
	          position_changed (),                                 // SFVec3f    [out]    position_changed
	                 geoOrigin (),                                 // SFNode     [ ]      geoOrigin                 NULL               [GeoOrigin]
	                 geoSystem ({ "GD", "WE" })                    // MFString   [ ]      geoSystem                 ["GD","WE"]        [see <a href="#Specifyingaspatialreference">25.2.3</a>]
{
	setComponent ("Geospatial");
	setTypeName ("GeoProximitySensor");

	appendField (inputOutput,    "metadata",                 metadata);
	appendField (inputOutput,    "enabled",                  enabled);
	appendField (inputOutput,    "size",                     size);
	appendField (inputOutput,    "center",                   center);
	appendField (outputOnly,     "enterTime",                enterTime);
	appendField (outputOnly,     "exitTime",                 exitTime);
	appendField (outputOnly,     "isActive",                 isActive);
	appendField (inputOutput,    "geoCenter",                geoCenter);
	appendField (outputOnly,     "centerOfRotation_changed", centerOfRotation_changed);
	appendField (outputOnly,     "geoCoord_changed",         geoCoord_changed);
	appendField (outputOnly,     "orientation_changed",      orientation_changed);
	appendField (outputOnly,     "position_changed",         position_changed);
	appendField (initializeOnly, "geoOrigin",                geoOrigin);
	appendField (initializeOnly, "geoSystem",                geoSystem);
}

X3DBasicNode*
GeoProximitySensor::create (X3DExecutionContext* const executionContext) const
{
	return new GeoProximitySensor (executionContext);
}

void
GeoProximitySensor::update ()
{ }

} // X3D
} // titania
