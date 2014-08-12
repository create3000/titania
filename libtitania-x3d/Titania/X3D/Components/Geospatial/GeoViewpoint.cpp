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
#include "../Layering/X3DLayerNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const std::string GeoViewpoint::componentName  = "Geospatial";
const std::string GeoViewpoint::typeName       = "GeoViewpoint";
const std::string GeoViewpoint::containerField = "children";

GeoViewpoint::Fields::Fields () :
	        position (new SFVec3d (0, 0, 100000)),
	centerOfRotation (new SFVec3d ()),
	     fieldOfView (new SFFloat (0.785398)),
	     speedFactor (new SFFloat (1)),
	         navType (nullptr),
	       headlight (nullptr)
{ }

GeoViewpoint::GeoViewpoint (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DViewpointNode (),
	X3DGeospatialObject (),
	             fields (),
	              coord (),
	          elevation (0),
	 navigationInfoNode ()
{
	addType (X3DConstants::GeoViewpoint);

	addField (inputOutput,    "metadata",          metadata ());
	addField (inputOnly,      "set_bind",          set_bind ());
	addField (inputOutput,    "description",       description ());
	addField (initializeOnly, "geoSystem",         geoSystem ());
	addField (inputOutput,    "position",          position ());
	addField (inputOutput,    "orientation",       orientation ());
	addField (inputOutput,    "centerOfRotation",  centerOfRotation ());
	addField (inputOutput,    "fieldOfView",       fieldOfView ());
	addField (inputOutput,    "jump",              jump ());
	addField (inputOutput,    "retainUserOffsets", retainUserOffsets ());

	switch (executionContext -> getVersion ())
	{
		case VRML_V2_0:
		case X3D_V3_0:
		case X3D_V3_1:
		case X3D_V3_2:
		{
			navigationInfoNode .set (new NavigationInfo (executionContext));

			fields .navType   = new MFString ({ "EXAMINE", "ANY" });
			fields .headlight = new SFBool (true);

			addField (inputOutput, "navType",   navType ());
			addField (inputOutput, "headlight", headlight ());

			addChildren (navigationInfoNode);
			break;
		}
		default:
			break;
	}

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

	position ()       .addInterest (this, &GeoViewpoint::set_position);
	positionOffset () .addInterest (this, &GeoViewpoint::set_position);

	set_position ();

	if (navigationInfoNode)
	{
		navType ()   .addInterest (navigationInfoNode -> type ());
		headlight () .addInterest (navigationInfoNode -> headlight ());

		navigationInfoNode -> setup ();
	}
}

void
GeoViewpoint::set_position ()
{
	coord     = getCoord (position ());
	elevation = getElevation (coord + Vector3d (positionOffset () .getValue ()));
}

void
GeoViewpoint::setPosition (const Vector3f & value)
{

}

void
GeoViewpoint::setOrientation (const Rotation4f & value)
{

}

///  Returns the resulting orientation for this viewpoint.
Rotation4f
GeoViewpoint::getOrientation () const
{
	Rotation4d localOrientation = Rotation4d (Matrix3d (getLocationMatrix (position ())));

	return Rotation4d (orientation () .getValue ()) * localOrientation;
}

///  Same as in Viewpoint
double
GeoViewpoint::getFieldOfView () const
{
	const double fov = fieldOfView () * fieldOfViewScale ();

	return fov > 0 and fov < M_PI ? fov : M_PI / 4;
}

Vector3f
GeoViewpoint::getUpVector () const
{
	return X3DGeospatialObject::getUpVector (coord + Vector3d (positionOffset () .getValue ()));
}

float
GeoViewpoint::getSpeedFactor () const
{
	return (std::max (elevation, 0.0) + 10) / 10 * speedFactor ();
}

///  Same as in Viewpoint
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

///  Same as in Viewpoint
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
GeoViewpoint::bindToLayer (X3DLayerNode* const layer)
{
	if (navigationInfoNode)
		navigationInfoNode -> bindToLayer (layer);

	X3DViewpointNode::bindToLayer (layer);
}

void
GeoViewpoint::unbindFromLayer (X3DLayerNode* const layer)
{
	if (navigationInfoNode)
		navigationInfoNode -> unbindFromLayer (layer);

	X3DViewpointNode::unbindFromLayer (layer);
}

void
GeoViewpoint::removeFromLayer (X3DLayerNode* const layer)
{
	if (navigationInfoNode)
		navigationInfoNode -> removeFromLayer (layer);

	X3DViewpointNode::removeFromLayer (layer);
}

///  Reshape viewpoint that it suits for X3DBackground.
void
GeoViewpoint::background (const double zNear, const double zFar)
{
	glMatrixMode (GL_PROJECTION);

	glLoadMatrixd (perspective (getFieldOfView (), zNear, zFar, Viewport4i ()) .data ());

	glMatrixMode (GL_MODELVIEW);
}

///  Main reshape function.
void
GeoViewpoint::reshape (const double zNear, const double zFar)
{
	const double geoZNear = zNear * std::max (elevation / 100, 1.0);
	const double geoZFar  = zFar;

	glMatrixMode (GL_PROJECTION);

	glLoadMatrixd (perspective (getFieldOfView (), geoZNear, geoZFar, Viewport4i ()) .data ());

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
