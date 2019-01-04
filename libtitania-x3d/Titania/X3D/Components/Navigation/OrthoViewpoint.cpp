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

#include "OrthoViewpoint.h"

#include "../../Execution/X3DExecutionContext.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

const std::string OrthoViewpoint::componentName  = "Navigation";
const std::string OrthoViewpoint::typeName       = "OrthoViewpoint";
const std::string OrthoViewpoint::containerField = "children";

OrthoViewpoint::Fields::Fields () :
	        position (new SFVec3f (0, 0, 10)),
	centerOfRotation (new SFVec3f ()),
	     fieldOfView (new MFFloat ({ -1, -1, 1, 1 }))
{ }

OrthoViewpoint::OrthoViewpoint (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DViewpointNode (),
	          fields ()
{
	addType (X3DConstants::OrthoViewpoint);

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
	fieldOfView ()      .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
OrthoViewpoint::create (X3DExecutionContext* const executionContext) const
{
	return new OrthoViewpoint (executionContext);
}

double
OrthoViewpoint::getMinimumX () const
{
	return (fieldOfView () .size () > 0 ? fieldOfView () [0] : -1.0) * fieldOfViewScale ();
}

double
OrthoViewpoint::getMinimumY () const
{
	return (fieldOfView () .size () > 1 ? fieldOfView () [1] : -1.0) * fieldOfViewScale ();
}

double
OrthoViewpoint::getMaximumX () const
{
	return (fieldOfView () .size () > 2 ? fieldOfView () [2] : 1.0) * fieldOfViewScale ();
}

double
OrthoViewpoint::getMaximumY () const
{
	return (fieldOfView () .size () > 3 ? fieldOfView () [3] : 1.0) * fieldOfViewScale ();
}

double
OrthoViewpoint::getSizeX () const
{
	return getMaximumX () - getMinimumX ();
}

double
OrthoViewpoint::getSizeY () const
{
	return getMaximumY () - getMinimumY ();
}

///  Returns size of one pixel in meters.
Vector3d
OrthoViewpoint::getScreenScale (const Vector3d &, const Vector4i & viewport) const
{
	const double width  = viewport [2];
	const double height = viewport [3];
	const double sizeX  = getSizeX ();
	const double sizeY  = getSizeY ();
	const double aspect = width / height;

	if (aspect > sizeX / sizeY)
	{
		const double s = getSizeY () / height;

		return Vector3d (s, s, s);
	}

	const double s = getSizeX () / width;

	return Vector3d (s, s, s);
}

///  Returns viewport size in meters.
Vector2d
OrthoViewpoint::getViewportSize (const Vector4i & viewport, const double nearValue) const
{
	const double width  = viewport [2];
	const double height = viewport [3];
	const double sizeX  = getSizeX ();
	const double sizeY  = getSizeY ();
	const double aspect = width / height;

	if (aspect > sizeX / sizeY)
		return Vector2d (sizeY * aspect, sizeY);

	return Vector2d (sizeX, sizeX / aspect);
}

std::pair <double, double>
OrthoViewpoint::getLookAtDistance (const Box3d & bbox) const
{
	const auto s  = abs (bbox .size ()) / 2;
	const auto s1 = s * 1.05;

	const auto fovs = std::max ({ s1 / std::abs (getMinimumX () / fieldOfViewScale ()),
	                              s1 / std::abs (getMinimumY () / fieldOfViewScale ()),
	                              s1 / std::abs (getMaximumX () / fieldOfViewScale ()), 
	                              s1 / std::abs (getMaximumY () / fieldOfViewScale ()) });

	return std::pair (s + 10, fovs);
}

Matrix4d
OrthoViewpoint::getProjectionMatrix (const double nearValue, const double farValue, const Vector4i & viewport, const bool) const
{
	const double width  = viewport [2];
	const double height = viewport [3];
	const double aspect = width / height;

	if (aspect > getSizeX () / getSizeY ())
	{
		const double center  = (getMinimumX () + getMaximumX ()) / 2;
		const double size1_2 = (getSizeY () * aspect) / 2;

		return camera <double>::ortho (center - size1_2, center + size1_2, getMinimumY (), getMaximumY (), nearValue, farValue);
	}

	const double center  = (getMinimumY () + getMaximumY ()) / 2;
	const double size1_2 = (getSizeX () / aspect) / 2;

	return camera <double>::ortho (getMinimumX (), getMaximumX (), center - size1_2, center + size1_2, nearValue, farValue);
}

} // X3D
} // titania
