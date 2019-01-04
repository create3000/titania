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

#include "Viewpoint.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const std::string Viewpoint::componentName  = "Navigation";
const std::string Viewpoint::typeName       = "Viewpoint";
const std::string Viewpoint::containerField = "children";

Viewpoint::Fields::Fields () :
	        position (new SFVec3f (0, 0, 10)),
	centerOfRotation (new SFVec3f ()),
	     fieldOfView (new SFFloat (0.7854))
{ }

Viewpoint::Viewpoint (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DViewpointNode (),
	          fields ()
{
	addType (X3DConstants::Viewpoint);

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOnly,   "set_bind",          set_bind ());
	addField (inputOutput, "description",       description ());
	addField (inputOutput, "position",          position ());
	addField (inputOutput, "orientation",       orientation ());
	addField (inputOutput, "centerOfRotation",  centerOfRotation ());
	addField (inputOutput, "fieldOfView",       fieldOfView ());
	addField (inputOutput, "jump",              jump ());
	addField (inputOutput, "retainUserOffsets", retainUserOffsets ());
	addField (outputOnly,  "isBound",           isBound ());
	addField (outputOnly,  "bindTime",          bindTime ());

	position ()         .setUnit (UnitCategory::LENGTH);
	centerOfRotation () .setUnit (UnitCategory::LENGTH);
	fieldOfView ()      .setUnit (UnitCategory::ANGLE);
}

X3DBaseNode*
Viewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new Viewpoint (executionContext);
}

double
Viewpoint::getFieldOfView () const
{
	const double fov = fieldOfView () * fieldOfViewScale ();

	return fov > 0 and fov < pi <double> ? fov : pi <double> / 4;
}

///  Returns size of one pixel in meters.
Vector3d
Viewpoint::getScreenScale (const Vector3d & point, const Vector4i & viewport) const
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
Viewpoint::getViewportSize (const Vector4i & viewport, const double nearValue) const
{
	const double width  = viewport [2];
	const double height = viewport [3];
	const double size   = nearValue * std::tan (getFieldOfView () / 2) * 2;
	const double aspect = width / height;

	if (aspect > 1)
		return Vector2d (size * aspect, size);

	return Vector2d (size, size / aspect);
}

std::pair <double, double>
Viewpoint::getLookAtDistance (const Box3d & bbox) const
{
	return std::pair ((abs (bbox .size ()) / 2) / std::tan (getFieldOfView () / 2), fieldOfViewScale ());
}

Matrix4d
Viewpoint::getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool) const
{
	return camera <double>::perspective (getFieldOfView (), nearValue, farValue, viewport [2], viewport [3]);
}

} // X3D
} // titania
