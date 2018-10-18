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

#include "GeoTouchSensor.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType GeoTouchSensor::component      = ComponentType::GEOSPATIAL;
const std::string   GeoTouchSensor::typeName       = "GeoTouchSensor";
const std::string   GeoTouchSensor::containerField = "children";

GeoTouchSensor::Fields::Fields () :
	hitTexCoord_changed (new SFVec2f ()),
	  hitNormal_changed (new SFVec3f ()),
	   hitPoint_changed (new SFVec3f ()),
	hitGeoCoord_changed (new SFVec3d ())
{ }

GeoTouchSensor::GeoTouchSensor (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	 X3DTouchSensorNode (),
	X3DGeospatialObject (),
	             fields ()
{
	addType (X3DConstants::GeoTouchSensor);

	addField (inputOutput,    "metadata",            metadata ());
	addField (initializeOnly, "geoOrigin",           geoOrigin ());
	addField (initializeOnly, "geoSystem",           geoSystem ());
	addField (inputOutput,    "description",         description ());
	addField (inputOutput,    "enabled",             enabled ());
	addField (outputOnly,     "hitTexCoord_changed", hitTexCoord_changed ());
	addField (outputOnly,     "hitNormal_changed",   hitNormal_changed ());
	addField (outputOnly,     "hitPoint_changed",    hitPoint_changed ());
	addField (outputOnly,     "hitGeoCoord_changed", hitGeoCoord_changed ());
	addField (outputOnly,     "isOver",              isOver ());
	addField (outputOnly,     "isActive",            isActive ());
	addField (outputOnly,     "touchTime",           touchTime ());

	hitPoint_changed ()    .setUnit (UnitCategory::LENGTH);
	hitGeoCoord_changed () .setUnit (UnitCategory::LENGTH);

	hitGeoCoord_changed () .isGeospatial (true);
}

X3DBaseNode*
GeoTouchSensor::create (X3DExecutionContext* const executionContext) const
{
	return new GeoTouchSensor (executionContext);
}

void
GeoTouchSensor::initialize ()
{
	X3DTouchSensorNode::initialize ();
	X3DGeospatialObject::initialize ();
}

void
GeoTouchSensor::set_over (const bool over,
                          const HitPtr & hit,
                          const Matrix4d & modelViewMatrix,
                          const Matrix4d & projectionMatrix,
                          const Vector4i & viewport)
{
	try
	{
		X3DTouchSensorNode::set_over (over, hit, modelViewMatrix, projectionMatrix, viewport);

		if (isOver ())
		{
			const auto & intersection = hit -> getIntersection ();
			const auto   hitPoint     = intersection -> getPoint () * inverse (modelViewMatrix);

			hitTexCoord_changed () = Vector2f (intersection -> getTexCoord () .x (), intersection -> getTexCoord () .y ());
			hitNormal_changed ()   = normalize (modelViewMatrix .mult_matrix_dir (intersection -> getNormal ()));
			hitPoint_changed ()    = hitPoint;
			hitGeoCoord_changed () = getGeoCoord (hitPoint);
		}
	}
	catch (const std::exception &)
	{ }
}

void
GeoTouchSensor::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DTouchSensorNode::dispose ();
}

} // X3D
} // titania
