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

#include "GeoLocation.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string GeoLocation::componentName  = "Geospatial";
const std::string GeoLocation::typeName       = "GeoLocation";
const std::string GeoLocation::containerField = "children";
GeoLocation::Fields::Fields () :
	geoCoords (new SFVec3d ())
{ }

GeoLocation::GeoLocation (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DGroupingNode (),
	X3DGeospatialObject (),
	             fields (),
	             matrix ()
{
	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "geoSystem",      geoSystem ());
	addField (inputOutput,    "geoCoords",      geoCoords ());
	addField (initializeOnly, "geoOrigin",      geoOrigin ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());
}

X3DBaseNode*
GeoLocation::create (X3DExecutionContext* const executionContext) const
{
	return new GeoLocation (executionContext);
}

void
GeoLocation::initialize ()
{
	X3DGroupingNode::initialize ();
	X3DGeospatialObject::initialize ();
	
	addInterest (this, &GeoLocation::eventsProcessed);

	eventsProcessed ();
}

void
GeoLocation::eventsProcessed ()
{
	Vector3d t = convert (geoCoords ());

	Vector3d x = normalize (cross (Vector3d (0, 0, 1), t));
	
	// Handle poles
	
	if (x == Vector3d ())
		x = Vector3d (1, 0, 0);

	Vector3d y = normalize (t);
	Vector3d z = normalize (cross (x, t));

	matrix = Matrix4d (x [0], x [1], x [2], 0,
	                   y [0], y [1], y [2], 0,
	                   z [0], z [1], z [2], 0,
	                   t [0], t [1], t [2], 1);
}

Box3f
GeoLocation::getBBox () const
{
	if (getDisplay ())
		return Box3d (X3DGroupingNode::getBBox ()) * matrix;

	return Box3f ();
}

void
GeoLocation::traverse (const TraverseType type)
{
	getModelViewMatrix () .push ();

	getModelViewMatrix () .multLeft (matrix);

	X3DGroupingNode::traverse (type);

	getModelViewMatrix () .pop ();
}

void
GeoLocation::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
