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

#include "../Browser.h"

#include "PlaneViewer.h"

#include "../../Components/Geospatial/GeoViewpoint.h"
#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Navigation/Viewpoint.h"

namespace titania {
namespace X3D {

static constexpr float SCROLL_FACTOR = 0.05;

PlaneViewer::PlaneViewer (Browser* const browser, NavigationInfo* const navigationInfo) :
	     X3DViewer (browser),
	navigationInfo (navigationInfo),
	     fromPoint (),
	        button (0)
{ }

void
PlaneViewer::initialize ()
{
	X3DViewer::initialize ();

	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &PlaneViewer::on_button_press_event));
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &PlaneViewer::on_button_release_event));
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &PlaneViewer::on_motion_notify_event), false);
	getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (*this, &PlaneViewer::on_scroll_event));
}

bool
PlaneViewer::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	if (button == 2)
	{
		getActiveViewpoint () -> transitionStop ();

		fromPoint = getPointOnCenterPlane (event -> x, event -> y);
	}

	return false;
}

bool
PlaneViewer::on_button_release_event (GdkEventButton* event)
{
	button = 0;
	return false;
}

bool
PlaneViewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 2)
	{
		const auto & viewpoint = getActiveViewpoint ();

		const Vector3f toPoint     = getPointOnCenterPlane (event -> x, event -> y);
		const Vector3f translation = (fromPoint - toPoint) * viewpoint -> getUserOrientation ();

		viewpoint -> positionOffset ()         += translation;
		viewpoint -> centerOfRotationOffset () += translation;

		fromPoint = toPoint;
	}

	return false;
}

bool
PlaneViewer::on_scroll_event (GdkEventScroll* event)
{
	const auto viewpoint = getActiveViewpoint ();

	viewpoint -> transitionStop ();

	Vector3f fromPoint = getPointOnCenterPlane (event -> x, event -> y);

	if (event -> direction == 0)      // Move backwards.
	{
		viewpoint -> fieldOfViewScale () = std::max (0.00001f, viewpoint -> fieldOfViewScale () * (1 - SCROLL_FACTOR));
	}

	else if (event -> direction == 1) // Move forwards.
	{
		viewpoint -> fieldOfViewScale () = viewpoint -> fieldOfViewScale () * (1 + SCROLL_FACTOR);

		constrainFieldOfViewScale ();
	}

	const Vector3f toPoint     = getPointOnCenterPlane (event -> x, event -> y);
	const Vector3f translation = (fromPoint - toPoint) * viewpoint -> getUserOrientation ();

	viewpoint -> positionOffset ()         += translation;
	viewpoint -> centerOfRotationOffset () += translation;

	return false;
}

void
PlaneViewer::constrainFieldOfViewScale () const
{
	const auto viewpointNode = getActiveViewpoint ();

	const auto viewpoint = dynamic_cast <Viewpoint*> (viewpointNode);

	if (viewpoint)
	{
		if (viewpoint -> fieldOfView () * viewpoint -> fieldOfViewScale () >= M_PI)
			viewpoint -> fieldOfViewScale () = (M_PI - 0.001) / viewpoint -> fieldOfView ();
	}
	else
	{
		const auto geoViewpoint = dynamic_cast <GeoViewpoint*> (viewpointNode);

		if (geoViewpoint)
		{
			if (geoViewpoint -> fieldOfView () * geoViewpoint -> fieldOfViewScale () >= M_PI)
				geoViewpoint -> fieldOfViewScale () = (M_PI - 0.001) / geoViewpoint -> fieldOfView ();
		}
	}
}

PlaneViewer::~PlaneViewer ()
{ }

} // X3D
} // titania
