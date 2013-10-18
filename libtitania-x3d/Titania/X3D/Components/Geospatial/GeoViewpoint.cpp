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

#include "GeoViewpoint.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string GeoViewpoint::componentName  = "Geospatial";
const std::string GeoViewpoint::typeName       = "GeoViewpoint";
const std::string GeoViewpoint::containerField = "children";

GeoViewpoint::Fields::Fields () :
	set_orientation (new SFRotation ()),
	set_position (new SFVec3d ()),
	fieldOfView (new SFFloat (0.785398)),
	headlight (new SFBool (true)),
	navType (new MFString ({ "EXAMINE", "ANY" })),
	geoOrigin (new SFNode ()),
	geoSystem (new MFString ({ "GD", "WE" })),
	position (new SFVec3d ()),
	speedFactor (new SFFloat (1))
{ }

GeoViewpoint::GeoViewpoint (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DViewpointNode (),
	          fields ()
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOutput,    "description",       description ());
	addField (inputOnly,      "set_bind",          set_bind ());
	addField (inputOnly,      "set_position",      set_position ());
	addField (inputOnly,      "set_orientation",   set_orientation ());
	addField (inputOutput,    "centerOfRotation",  centerOfRotation ());
	addField (inputOutput,    "fieldOfView",       fieldOfView ());
	addField (inputOutput,    "jump",              jump ());
	addField (inputOutput,    "retainUserOffsets", retainUserOffsets ());
	addField (inputOutput,    "headlight",         headlight ());
	addField (inputOutput,    "navType",           navType ());
	addField (initializeOnly, "geoSystem",         geoSystem ());
	addField (initializeOnly, "geoOrigin",         geoOrigin ());
	addField (initializeOnly, "position",          position ());
	addField (initializeOnly, "orientation",       orientation ());
	addField (initializeOnly, "speedFactor",       speedFactor ());
	addField (outputOnly,     "isBound",           isBound ());
	addField (outputOnly,     "bindTime",          bindTime ());
}

X3DBaseNode*
GeoViewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new GeoViewpoint (executionContext);
}

Vector3f
GeoViewpoint::getPosition () const
{
	return position () .getValue ();
}

Vector3f
GeoViewpoint::getLookAtPositionOffset (Box3f bbox) const
{
	return bbox .center ()
	       + getUserOrientation () * (Vector3f (0, 0, (math::abs (bbox .size ()) / 2) / std::tan (fieldOfView () / 2)))
	       - getPosition ();
}

Vector3f
GeoViewpoint::getScreenScale (float, const Vector4i & viewport) const
{
	return Vector3f (1, 1, 1);
}

void
GeoViewpoint::reshape (const float zNear, const float zFar)
{ }

void
GeoViewpoint::traverse (const TraverseType)
{ }

} // X3D
} // titania
