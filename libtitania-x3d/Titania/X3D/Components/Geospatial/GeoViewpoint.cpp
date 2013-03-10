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

#include "GeoViewpoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

GeoViewpoint::GeoViewpoint (X3DExecutionContext* const executionContext, bool displayed) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DViewpointNode (displayed),                                           
	 set_orientation (),                                                    // SFRotation [in]     set_orientation
	    set_position (),                                                    // SFVec3d    [in]     set_position
	     fieldOfView (0.785398),                                            // SFFloat    [in,out] fieldOfView      π/4                      (0,π)
	       headlight (true),                                                // SFBool     [in,out] headlight        TRUE
	         navType ({ "EXAMINE", "ANY" }),                                // MFString   [in,out] navType          ["EXAMINE","ANY"]
	       geoOrigin (),                                                    // SFNode     [ ]      geoOrigin        NULL                     [GeoOrigin]
	       geoSystem ({ "GD", "WE" }),                                      // MFString   [ ]      geoSystem        ["GD","WE"]              [see <a href="#Specifyingaspatialreference">25.2.3</a>]
	        position (0, 0, 100000),                                        // SFVec3d    [ ]      position         0 0 100000               (-∞,∞)
	     speedFactor (1)                                                    // SFFloat    [ ]      speedFactor      1.0                      [0,∞)
{
	setComponent ("Geospatial");
	setTypeName ("GeoViewpoint");

	addField (inputOutput,    "metadata",          metadata);
	addField (inputOnly,      "set_bind",          set_bind);
	addField (inputOutput,    "jump",              jump);
	addField (inputOutput,    "retainUserOffsets", retainUserOffsets);
	addField (inputOutput,    "centerOfRotation",  centerOfRotation);
	addField (inputOutput,    "description",       description);
	addField (outputOnly,     "bindTime",          bindTime);
	addField (outputOnly,     "isBound",           isBound);
	addField (inputOnly,      "set_orientation",   set_orientation);
	addField (inputOnly,      "set_position",      set_position);
	addField (inputOutput,    "fieldOfView",       fieldOfView);
	addField (inputOutput,    "headlight",         headlight);
	addField (inputOutput,    "navType",           navType);
	addField (initializeOnly, "geoOrigin",         geoOrigin);
	addField (initializeOnly, "geoSystem",         geoSystem);
	addField (initializeOnly, "position",          position);
	addField (initializeOnly, "orientation",       orientation);
	addField (initializeOnly, "speedFactor",       speedFactor);
}

X3DBaseNode*
GeoViewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new GeoViewpoint (executionContext, true);
}

Vector3f
GeoViewpoint::getPosition () const
{
	return position .getValue ();
}

Vector3f
GeoViewpoint::lookAtPositionOffset (Box3f bbox)
{
	return bbox .center ()
	       + getUserOrientation () * (Vector3f (0, 0, (math::abs (bbox .size ()) * 0.5f) / std::tan (fieldOfView * 0.5f)))
	       - getPosition ();
}

void
GeoViewpoint::reshape (const float zNear, const float zFar)
{ }

void
GeoViewpoint::traverse (TraverseType)
{ }

} // X3D
} // titania
