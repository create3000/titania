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

#include "GeoViewpoint.h"

#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Layering/X3DViewportNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const ComponentType GeoViewpoint::component      = ComponentType::GEOSPATIAL;
const std::string   GeoViewpoint::typeName       = "GeoViewpoint";
const std::string   GeoViewpoint::containerField = "children";

GeoViewpoint::Fields::Fields () :
	        position (new SFVec3d (0, 0, 100000)),
	centerOfRotation (new SFVec3d ()),
	     fieldOfView (new SFFloat (0.7854)),
	     speedFactor (new SFFloat (1)),
	         navType (new MFString ({ "EXAMINE", "ANY" })),
	       headlight (new SFBool (true))
{ }

GeoViewpoint::GeoViewpoint (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	   X3DViewpointNode (),
	X3DGeospatialObject (),
	             fields (),
	              coord (),
	          elevation (0),
	 navigationInfoNode (new NavigationInfo (executionContext))
{
	addType (X3DConstants::GeoViewpoint);

	switch (getExecutionContext () -> getSpecificationVersion ())
	{
		case VRML_V2_0:
		case X3D_V3_0:
		case X3D_V3_1:
		case X3D_V3_2:
			break;
		default:
			navType ()   .isHidden (true);
			headlight () .isHidden (true);
			break;
	}

	addField (inputOutput,    "metadata",          metadata ());
	addField (initializeOnly, "geoOrigin",         geoOrigin ());
	addField (initializeOnly, "geoSystem",         geoSystem ());
	addField (inputOnly,      "set_bind",          set_bind ());
	addField (inputOutput,    "description",       description ());
	addField (inputOutput,    "position",          position ());
	addField (inputOutput,    "orientation",       orientation ());
	addField (inputOutput,    "centerOfRotation",  centerOfRotation ());
	addField (inputOutput,    "fieldOfView",       fieldOfView ());
	addField (inputOutput,    "jump",              jump ());
	addField (inputOutput,    "retainUserOffsets", retainUserOffsets ());
	addField (inputOutput,    "navType",           navType ());           // Obsolete since 3.3.
	addField (inputOutput,    "headlight",         headlight ());         // Obsolete since 3.3.
	addField (initializeOnly, "speedFactor",       speedFactor ());
	addField (outputOnly,     "isBound",           isBound ());
	addField (outputOnly,     "bindTime",          bindTime ());

	position ()         .setUnit (UnitCategory::LENGTH);
	centerOfRotation () .setUnit (UnitCategory::LENGTH);
	fieldOfView ()      .setUnit (UnitCategory::ANGLE);

	position () .isGeospatial (true);

	addChildObjects (navigationInfoNode);
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

	position ()       .addInterest (&GeoViewpoint::set_position, this);
	positionOffset () .addInterest (&GeoViewpoint::set_position, this);
	navType ()        .addInterest (navigationInfoNode -> type ());
	headlight ()      .addInterest (navigationInfoNode -> headlight ());

	navigationInfoNode -> setup ();

	set_position ();
}

void
GeoViewpoint::setExecutionContext (X3DExecutionContext* const executionContext)
{
	navigationInfoNode -> setExecutionContext (executionContext);

	switch (executionContext -> getSpecificationVersion ())
	{
		case VRML_V2_0:
		case X3D_V3_0:
		case X3D_V3_1:
		case X3D_V3_2:
			navType ()   .isHidden (false);
			headlight () .isHidden (false);
			break;
		default:
			navType ()   .isHidden (true);
			headlight () .isHidden (true);
			break;
	}

	X3DViewpointNode::setExecutionContext (executionContext);
}

void
GeoViewpoint::setPosition (const Vector3d & value)
{
	position () = getGeoCoord (value);
}

Vector3d
GeoViewpoint::getPosition () const
{
	return getCoord (position ());
}

void
GeoViewpoint::set_position ()
{
	coord     = getCoord (position ());
	elevation = getElevation (coord + Vector3d (positionOffset () .getValue ()));
}

void
GeoViewpoint::setOrientation (const Rotation4d & value)
{
	const Rotation4d localOrientation = Rotation4d (getLocationMatrix (position ()) .submatrix ());

	orientation () = Rotation4d (value) * ~localOrientation;
}

///  Returns the resulting orientation for this viewpoint.
Rotation4d
GeoViewpoint::getOrientation () const
{
	const Rotation4d localOrientation = Rotation4d (getLocationMatrix (position ()) .submatrix ());

	return Rotation4d (orientation () .getValue ()) * localOrientation;
}

void
GeoViewpoint::setCenterOfRotation (const Vector3d & value)
{
	centerOfRotation () = getGeoCoord (value);
}

Vector3d
GeoViewpoint::getCenterOfRotation () const
{
	return getCoord (centerOfRotation ());
}

//  Same as in Viewpoint
double
GeoViewpoint::getFieldOfView () const
{
	const double fov = fieldOfView () * fieldOfViewScale ();

	return fov > 0 and fov < pi <double> ? fov : pi <double> / 4;
}

Vector3d
GeoViewpoint::getUpVector () const
{
	return X3DGeospatialObject::getUpVector (coord + Vector3d (positionOffset () .getValue ()));
}

double
GeoViewpoint::getMaxFarValue () const
{
	return getBrowser () -> getRenderingProperties () -> LogarithmicDepthBuffer () ? 1e10 : 1e9;
}

double
GeoViewpoint::getSpeedFactor () const
{
	return (std::max (elevation, 0.0) + 10) / 10 * speedFactor ();
}

///  Returns size of one pixel in meters.
Vector3d
GeoViewpoint::getScreenScale (const Vector3d & point, const Vector4i & viewport) const
{
	const int32_t width  = viewport [2];
	const int32_t height = viewport [3];
	double        size   = abs (point .z ()) * std::tan (getFieldOfView () / 2) * 2;

	if (width > height)
		size /= height;

	else
		size /= width;

	return Vector3d (size, size, size);
}

///  Returns viewport size in meters.
Vector2d
GeoViewpoint::getViewportSize (const Vector4i & viewport, const double nearValue) const
{
	const double width  = viewport [2];
	const double height = viewport [3];
	const double size   = nearValue * std::tan (getFieldOfView () / 2) * 2;
	const double aspect = width / height;

	if (aspect > 1)
		return Vector2d (size * aspect, size);

	return Vector2d (size, size / aspect);
}

///  Same as in Viewpoint
std::pair <double, double>
GeoViewpoint::getLookAtDistance (const Box3d & bbox) const
{
	return std::pair ((abs (bbox .size ()) / 2) / std::tan (getFieldOfView () / 2), fieldOfViewScale ());
}

void
GeoViewpoint::bindToLayer (X3DLayerNode* const layer)
{
	if (not navType () .isHidden ())
		navigationInfoNode -> bindToLayer (layer);

	X3DViewpointNode::bindToLayer (layer);
}

void
GeoViewpoint::unbindFromLayer (X3DLayerNode* const layer)
{
	if (not navType () .isHidden ())
		navigationInfoNode -> unbindFromLayer (layer);

	X3DViewpointNode::unbindFromLayer (layer);
}

void
GeoViewpoint::removeFromLayer (X3DLayerNode* const layer)
{
	if (not navType () .isHidden ())
		navigationInfoNode -> removeFromLayer (layer);

	X3DViewpointNode::removeFromLayer (layer);
}

Matrix4d
GeoViewpoint::getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool limit) const
{
	if (limit or getBrowser () -> getRenderingProperties () -> LogarithmicDepthBuffer ())
		return camera <double>::perspective (getFieldOfView (), nearValue, farValue, viewport [2], viewport [3]);

	const double geoZNear = std::max (math::lerp (std::min (nearValue, 1e4), 1e4, elevation / 1e7), 1.0);
	const double geoZFar  = std::max (math::lerp (1e6, std::max (farValue, 1e6),  elevation / 1e7), 1e6);

	return camera <double>::perspective (getFieldOfView (), geoZNear, geoZFar, viewport [2], viewport [3]);
}

void
GeoViewpoint::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	X3DViewpointNode::traverse (type, renderObject);

	if (not navType () .isHidden ())
		navigationInfoNode -> traverse (type, renderObject);
}

void
GeoViewpoint::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DViewpointNode::dispose ();
}

} // X3D
} // titania
