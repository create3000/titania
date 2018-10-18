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

#include "../Browser.h"

#include "X3DViewer.h"

#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Navigation/NavigationInfo.h"
#include "../../Components/Navigation/X3DViewpointNode.h"
#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Components/Layering/X3DViewportNode.h"
#include "../../Components/Shape/X3DShapeNode.h"
#include "../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

X3DViewer::Fields::Fields () :
	  isActive (new SFBool ()),
	scrollTime (new SFTime ())
{ }

X3DViewer::X3DViewer () :
	    X3DBaseNode (),
	sigc::trackable (),
	         fields ()
{ }

NavigationInfo*
X3DViewer::getNavigationInfo () const
{
	return getBrowser () -> getActiveLayer () -> getNavigationInfo ();
}

const X3DPtr <X3DLayerNode> &
X3DViewer::getActiveLayer () const
{
	return getBrowser () -> getActiveLayer ();
}

X3DViewpointNode*
X3DViewer::getActiveViewpoint () const
{
	return getBrowser () -> getActiveLayer () -> getViewpoint ();
}

/// Returns the picking point on the center plane.
Vector3d
X3DViewer::getPointOnCenterPlane (const double x, const double y)
{
	try
	{
		const auto &   viewpoint  = getActiveViewpoint ();
		const auto &   viewport   = getBrowser () -> getActiveLayer () -> getViewport () -> getRectangle (getBrowser ());
		const auto     projection = viewpoint -> getProjectionMatrix (getNavigationInfo () -> getNearValue (), getNavigationInfo () -> getFarValue (viewpoint), viewport);
		const Matrix4d modelview; // Use identity

		// Far plane point
		const Vector3d far = ViewVolume::unProjectPoint (x, getBrowser () -> get_height () - y, 0.9, modelview, projection, viewport);

		if (dynamic_cast <OrthoViewpoint*> (viewpoint))
			return Vector3d (far .x (), far .y (), -abs (getDistanceToCenter ()));

		const Vector3d direction = normalize (far);

		return direction * abs (getDistanceToCenter ()) / dot (direction, Vector3d (0, 0, -1));
	}
	catch (const std::exception & error)
	{
		return Vector3d ();
	}
}

Vector3d
X3DViewer::getDistanceToCenter () const
{
	const auto & viewpoint = getActiveViewpoint ();

	return viewpoint -> getUserPosition () - viewpoint -> getUserCenterOfRotation ();
}

Vector3d
X3DViewer::trackballProjectToSphere (double x, double y) const
{
	x = x / getBrowser () -> get_width () - 0.5;
	y = -y / getBrowser () -> get_height () + 0.5;

	return Vector3d (x, y, tb_project_to_sphere (0.5, x, y));
}

double
X3DViewer::tb_project_to_sphere (const double r, const double x, const double y)
{
	const double d = std::sqrt (x * x + y * y);

	if (d < r * std::sqrt (0.5)) // Inside sphere
	{
		return std::sqrt (r * r - d * d);
	}

	// On hyperbola

	const double t = r / std::sqrt (2);
	return t * t / d;
}

bool
X3DViewer::lookAt (const double x, const double y, const bool straightenHorizon, const bool seek) const
{
	if (touch (x, y))
	{
		const auto & hit = getBrowser () -> getNearestHit ();

		if (seek)
		{
			// Seek: look at selected point and fly a little closer.

			getActiveViewpoint () -> lookAt (hit -> getIntersection () -> getPoint () * getActiveViewpoint () -> getCameraSpaceMatrix (), 2 - phi <double>, straightenHorizon);
		}
		else
		{
			// Look at as specification say.

			const auto modelViewMatrix = hit -> getModelViewMatrix () * getActiveViewpoint () -> getCameraSpaceMatrix ();
			const auto bbox            = hit -> getShape () -> getBBox () * modelViewMatrix;

			getActiveViewpoint () -> lookAt (bbox, 2 - phi <double>, straightenHorizon);
		}

		return true;
	}

	return false;
}

bool
X3DViewer::touch (const double x, const double y) const
{
	getBrowser () -> touch (x, getBrowser () -> get_height () - y);

	return not getBrowser () -> getHits () .empty ();
}

void
X3DViewer::dispose ()
{
	notify_callbacks ();

	X3DBaseNode::dispose ();
}

X3DViewer::~X3DViewer ()
{ }

} // X3D
} // titania
