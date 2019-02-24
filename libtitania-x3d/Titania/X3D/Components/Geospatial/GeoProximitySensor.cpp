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

#include "GeoProximitySensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   GeoProximitySensor::component      = Component ("Geospatial", 2);
const std::string GeoProximitySensor::typeName       = "GeoProximitySensor";
const std::string GeoProximitySensor::containerField = "children";

GeoProximitySensor::Fields::Fields () :
	        geoCoord_changed (new SFVec3d ()),
	        position_changed (new SFVec3f ()),
	     orientation_changed (new SFRotation ()),
	centerOfRotation_changed (new SFVec3f ())
{ }

GeoProximitySensor::GeoProximitySensor (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DEnvironmentalSensorNode (),
	       X3DGeospatialObject (),
	                    fields (),
	           proximitySensor (new ProximitySensor (executionContext))
{
	addType (X3DConstants::GeoProximitySensor);

	addField (inputOutput,    "metadata",                 metadata ());
	addField (initializeOnly, "geoOrigin",                geoOrigin ());
	addField (initializeOnly, "geoSystem",                geoSystem ());
	addField (inputOutput,    "enabled",                  enabled ());
	addField (inputOutput,    "size",                     size ());
	addField (inputOutput,    "center",                   center ());
	addField (outputOnly,     "isActive",                 isActive ());
	addField (outputOnly,     "enterTime",                enterTime ());
	addField (outputOnly,     "exitTime",                 exitTime ());
	addField (outputOnly,     "geoCoord_changed",         geoCoord_changed ());
	addField (outputOnly,     "position_changed",         position_changed ());
	addField (outputOnly,     "orientation_changed",      orientation_changed ());
	addField (outputOnly,     "centerOfRotation_changed", centerOfRotation_changed ());

	addChildObjects (proximitySensor);

	geoCoord_changed ()         .setUnit (UnitCategory::LENGTH);
	position_changed ()         .setUnit (UnitCategory::LENGTH);
	centerOfRotation_changed () .setUnit (UnitCategory::LENGTH);

	geoCoord_changed () .isGeospatial (true);

	setCameraObject   (proximitySensor -> isCameraObject ());
	setPickableObject (proximitySensor -> isPickableObject ());
}

X3DBaseNode*
GeoProximitySensor::create (X3DExecutionContext* const executionContext) const
{
	return new GeoProximitySensor (executionContext);
}

void
GeoProximitySensor::initialize ()
{
	X3DEnvironmentalSensorNode::initialize ();
	X3DGeospatialObject::initialize ();

	enabled () .addInterest (proximitySensor -> enabled ());
	size ()    .addInterest (proximitySensor -> size ());
	center ()  .addInterest (proximitySensor -> center ());

	proximitySensor -> isCameraObject ()   .addInterest (&GeoProximitySensor::setCameraObject,   static_cast <X3DChildNode*> (this));
	proximitySensor -> isPickableObject () .addInterest (&GeoProximitySensor::setPickableObject, static_cast <X3DChildNode*> (this));

	proximitySensor -> isActive ()                 .addInterest (isActive ());
	proximitySensor -> enterTime ()                .addInterest (enterTime ());
	proximitySensor -> exitTime ()                 .addInterest (exitTime ());
	proximitySensor -> position_changed ()         .addInterest (position_changed ());
	proximitySensor -> orientation_changed ()      .addInterest (orientation_changed ());
	proximitySensor -> centerOfRotation_changed () .addInterest (centerOfRotation_changed ());

	proximitySensor -> position_changed () .addInterest (&GeoProximitySensor::set_position, this);

	proximitySensor -> enabled () = enabled ();
	proximitySensor -> size ()    = size ();
	proximitySensor -> center ()  = center ();

	proximitySensor -> setup ();
}

void
GeoProximitySensor::setExecutionContext (X3DExecutionContext* const executionContext)
{
	proximitySensor -> setExecutionContext (executionContext);

	X3DEnvironmentalSensorNode::setExecutionContext (executionContext);
}

void
GeoProximitySensor::set_position (const Vector3f & position)
{
	geoCoord_changed () = getGeoCoord (proximitySensor -> getPosition ());
}

void
GeoProximitySensor::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	proximitySensor -> traverse (type, renderObject);
}

void
GeoProximitySensor::addTool ()
{
	proximitySensor -> addTool ();
}

void
GeoProximitySensor::removeTool (const bool really)
{
	proximitySensor -> removeTool (really);
}

void
GeoProximitySensor::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DEnvironmentalSensorNode::dispose ();
}

} // X3D
} // titania
