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

#include "ExamineViewer.h"

#include "../X3DBrowserSurface.h"

#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr double MOTION_TIME       = 0.05;
static constexpr double SPIN_RELEASE_TIME = 0.01;
static constexpr float  SPIN_ANGLE        = 0.006;
static constexpr float  SPIN_FACTOR       = 0.6;
static constexpr float  SCROLL_FACTOR      = 1.0f / 50.0f;
static constexpr float  FRAME_RATE        = 60;

ExamineViewer::ExamineViewer (X3DBrowserSurface* const browser, NavigationInfo* const navigationInfo) :
	     X3DViewer (browser),
	navigationInfo (navigationInfo),
	      distance (),
	   orientation (),
	      rotation (),
	    fromVector (),
	     fromPoint (),
	     pressTime (0),
	    motionTime (0),
	        button (0),
	       spin_id ()
{ }

void
ExamineViewer::initialize ()
{
	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &ExamineViewer::on_button_press_event));
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &ExamineViewer::on_button_release_event));
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &ExamineViewer::on_motion_notify_event), false);
	getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (*this, &ExamineViewer::on_scroll_event));

	getNavigationInfo () -> transitionStart () .addInterest (this, &ExamineViewer::set_transitionStart);
	getBrowser () -> getActiveViewpointChanged () .addInterest (this, &ExamineViewer::set_viewpoint);
}

void
ExamineViewer::set_transitionStart ()
{
	spin_id .disconnect ();
}

void
ExamineViewer::set_viewpoint ()
{
	// Update distance and orientationOffset.

	spin_id .disconnect ();

	auto viewpoint = getActiveViewpoint ();

	orientation = viewpoint -> getUserOrientation ();
	distance    = getDistanceToCenter ();
}

bool
ExamineViewer::on_button_press_event (GdkEventButton* event)
{
	button    = event -> button;
	pressTime = chrono::now ();

	if (button == 1)
	{
		getActiveViewpoint () -> transitionStop ();

		set_viewpoint ();

		fromVector = trackballProjectToSphere (event -> x, event -> y);
		rotation   = Rotation4f ();

		motionTime = 0;
	}

	else if (button == 2)
	{
		getActiveViewpoint () -> transitionStop ();

		set_viewpoint ();

		fromPoint = getPointOnCenterPlane (event -> x, event -> y);
	}

	return false;
}

bool
ExamineViewer::on_button_release_event (GdkEventButton* event)
{
	if (button == 1)
	{
		if (std::abs (rotation .angle ()) > SPIN_ANGLE and chrono::now () - motionTime < SPIN_RELEASE_TIME)
		{
			rotation = slerp (Rotation4f (), rotation, SPIN_FACTOR);
			addSpinning ();
		}
	}

	button = 0;

	return false;
}

bool
ExamineViewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 1)
	{
		const auto & viewpoint = getActiveViewpoint ();

		Vector3f toVector = trackballProjectToSphere (event -> x, event -> y);

		rotation = ~Rotation4f (fromVector, toVector);
		
		if (std::abs (rotation .angle ()) < SPIN_ANGLE and chrono::now () - pressTime < MOTION_TIME)
			return false;

		viewpoint -> orientationOffset () = getOrientationOffset ();
		viewpoint -> positionOffset ()    = getPositionOffset ();

		fromVector = toVector;
		
		motionTime = chrono::now ();
	}

	else if (button == 2)
	{
		const auto & viewpoint = getActiveViewpoint ();

		Vector3f toPoint     = getPointOnCenterPlane (event -> x, event -> y);
		Vector3f translation = viewpoint -> getUserOrientation () * (fromPoint - toPoint);

		viewpoint -> positionOffset ()         += translation;
		viewpoint -> centerOfRotationOffset () += translation;

		fromPoint = toPoint;
	}

	return false;
}

bool
ExamineViewer::on_scroll_event (GdkEventScroll* event)
{
	auto viewpoint = getActiveViewpoint ();

	viewpoint -> transitionStop ();

	Vector3f step           = getDistanceToCenter () * SCROLL_FACTOR;
	Vector3f positionOffset = viewpoint -> getUserOrientation () * Vector3f (0, 0, abs (step));

	if (event -> direction == 0)      // Move backwards.
	{
		viewpoint -> positionOffset () += positionOffset;
	}

	else if (event -> direction == 1) // Move forwards.
	{
		viewpoint -> positionOffset () -= positionOffset;
	}

	distance = getDistanceToCenter ();

	return false;
}

bool
ExamineViewer::spin ()
{
	auto viewpoint = getActiveViewpoint ();

	viewpoint -> orientationOffset () = getOrientationOffset ();
	viewpoint -> positionOffset ()    = getPositionOffset ();

	return true;
}

void
ExamineViewer::addSpinning ()
{
	if (not spin_id .connected ())
		spin_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &ExamineViewer::spin), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

Vector3f
ExamineViewer::getPositionOffset () const
{
	auto viewpoint = getActiveViewpoint ();

	// The new positionOffset is calculated here by calculating the new position and
	// then subtracting the viewpoints position to get the new positionOffset.

	return viewpoint -> getUserCenterOfRotation ()
	       + viewpoint -> orientationOffset () * distance
	       - (viewpoint -> getUserPosition () - viewpoint -> positionOffset ());
}

Rotation4f
ExamineViewer::getOrientationOffset ()
{
	auto viewpoint = getActiveViewpoint ();

	orientation = rotation * orientation;
	return ~viewpoint -> orientation () * orientation;
}

} // X3D
} // titania
