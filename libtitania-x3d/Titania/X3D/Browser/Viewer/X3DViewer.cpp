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

#include "../X3DBrowserSurface.h"

#include "X3DViewer.h"

#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

X3DViewer::X3DViewer (X3DBrowserSurface* const browser) :
	X3DWidget (browser)
{ }

X3DViewpointNode*
X3DViewer::getActiveViewpoint () const
{
	return getBrowser () -> getActiveLayer () -> getViewpoint ();
}

/// Returns the picking point on the center plane.
Vector3f
X3DViewer::getPointOnCenterPlane (const double x, const double y)
{
	if (getBrowser () -> makeCurrent ())
	{
		try
		{
			const auto viewpoint = getActiveViewpoint ();

			viewpoint -> reshape (0.125, 100000);

			const Matrix4d modelview; // Use identity
			const Matrix4d projection = ProjectionMatrix4d ();

			const auto viewport = getBrowser () -> getActiveLayer () -> getViewport () -> getViewport (getBrowser () -> get_width (),
			                                                                                           getBrowser () -> get_height ());
	
			// Far plane point
			const Vector3d far = ViewVolume::unProjectPoint (x, getBrowser () -> get_height () - y, 0.9, modelview, projection, viewport);

			if (dynamic_cast <OrthoViewpoint*> (viewpoint))
				return Vector3f (far .x (), far .y (), -abs (getDistanceToCenter ()));

			const Vector3f direction = normalize (far);

			return direction * abs (getDistanceToCenter ()) / dot (direction, Vector3f (0, 0, -1));
		}
		catch (const std::domain_error & error)
		{ }
	}

	return Vector3f ();
}

Vector3f
X3DViewer::getDistanceToCenter () const
{
	const auto viewpoint = getActiveViewpoint ();

	return (viewpoint -> getUserPosition () - viewpoint -> getUserCenterOfRotation ()) * ~viewpoint -> orientationOffset ();
}

Vector3f
X3DViewer::trackballProjectToSphere (double x, double y) const
{
	x = x / getBrowser () -> get_width () - 0.5;
	y = -y / getBrowser () -> get_height () + 0.5;

	return Vector3f (x, y, tb_project_to_sphere (0.5, x, y));
}

float
X3DViewer::tb_project_to_sphere (const float r, const float x, const float y)
{
	const float d = std::sqrt (x * x + y * y);

	if (d < r * std::sqrt (0.5)) // Inside sphere
	{
		return std::sqrt (r * r - d * d);
	}

	// On hyperbola

	const float t = r / std::sqrt (2);
	return t * t / d;
}

} // X3D
} // titania
