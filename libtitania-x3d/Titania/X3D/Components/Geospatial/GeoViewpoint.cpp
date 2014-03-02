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

#include "GeoViewpoint.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const std::string GeoViewpoint::componentName  = "Geospatial";
const std::string GeoViewpoint::typeName       = "GeoViewpoint";
const std::string GeoViewpoint::containerField = "children";

GeoViewpoint::Fields::Fields () :
	        position (new SFVec3d (0, 0, 100000)),
	//centerOfRotation (new SFVec3d ()),
	     fieldOfView (new SFFloat (0.785398)),
	     speedFactor (new SFFloat (1))
{ }

GeoViewpoint::GeoViewpoint (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DViewpointNode (),
	X3DGeospatialObject (),
	             fields ()
{
	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "geoSystem",         geoSystem ());
	addField (inputOutput,    "description",       description ());
	addField (inputOnly,      "set_bind",          set_bind ());
	addField (inputOutput,    "position",          position ());
	addField (inputOutput,    "orientation",       orientation ());
	addField (inputOutput,    "centerOfRotation",  centerOfRotation ());
	addField (inputOutput,    "fieldOfView",       fieldOfView ());
	addField (inputOutput,    "jump",              jump ());
	addField (inputOutput,    "retainUserOffsets", retainUserOffsets ());
	addField (initializeOnly, "speedFactor",       speedFactor ());
	addField (outputOnly,     "isBound",           isBound ());
	addField (outputOnly,     "bindTime",          bindTime ());
	addField (initializeOnly, "geoOrigin",         geoOrigin ());
}

X3DBaseNode*
GeoViewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new GeoViewpoint (executionContext);
}

void
GeoViewpoint::initialize ()
{
	X3DViewpointNode::initialize ();
	X3DGeospatialObject::initialize ();
}

Vector3f
GeoViewpoint::getPosition () const
{
	return getCoord (position ());
}

Rotation4f
GeoViewpoint::getOrientation () const
{
	Rotation4f localOrientation = Rotation4d (Matrix3d (getLocationMatrix (position ())));

	return orientation () * localOrientation;
}

double
GeoViewpoint::getFieldOfView () const
{
	const double fov = fieldOfView () * fieldOfViewScale ();

	return fov > 0 and fov < M_PI ? fov : M_PI / 4;
}

Vector3d
GeoViewpoint::getScreenScale (const double distance, const Vector4i & viewport) const
{
	const int width  = viewport [2];
	const int height = viewport [3];
	double    size   = distance * std::tan (getFieldOfView () / 2) * 2;

	if (width > height)
		size /= height;

	else
		size /= width;

	return Vector3d (size, size, size);
}

Vector3f
GeoViewpoint::getUpVector () const
{
	return Vector3f (0, 1, 0);
}

Vector3f
GeoViewpoint::getLookAtPositionOffset (const Box3f & bbox) const
{
	if (getBrowser () -> getActiveLayer ())
	{
		const float distance    = (abs (bbox .size ()) / 2) / std::tan (getFieldOfView () / 2);
		const float minDistance = getBrowser () -> getActiveLayer () -> getNavigationInfo () -> getNearPlane () * 2;

		return bbox .center ()
		       + Vector3f (0, 0, std::max (distance, minDistance)) * getUserOrientation ()
		       - getPosition ();
	}

	return Vector3f ();
}

void
GeoViewpoint::reshape (const double zNear, const double zFar)
{
	static constexpr double zFar0 = 1e8;

	glMatrixMode (GL_PROJECTION);

	const Vector4i viewport = Viewport4i ();

	const size_t width  = viewport [2];
	const size_t height = viewport [3];

	const double ratio = std::tan (getFieldOfView () / 2) * zNear;

	if (width > height)
	{
		const double aspect = width * ratio / height;
		glLoadMatrixd (frustum (-aspect, aspect, -ratio, ratio, zNear, zFar0) .data ());
	}
	else
	{
		const double aspect = height * ratio / width;
		glLoadMatrixd (frustum (-ratio, ratio, -aspect, aspect, zNear, zFar0) .data ());
	}

	glMatrixMode (GL_MODELVIEW);
}

void
GeoViewpoint::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DViewpointNode::dispose ();
}

} // X3D
} // titania
