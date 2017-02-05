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

#include "X3DExamineViewer.h"

#include "../../Components/Navigation/NavigationInfo.h"
#include "../Browser.h"

#include <Titania/Chrono/Now.h>
#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr time_type MOTION_TIME       = 0.05;
static constexpr time_type SPIN_RELEASE_TIME = 0.01;
static constexpr double    SPIN_ANGLE        = 0.006;
static constexpr double    SPIN_FACTOR       = 0.6;
static constexpr double    SCROLL_FACTOR     = 1.0 / 50.0;
static constexpr time_type FRAME_RATE        = 60;

const ComponentType X3DExamineViewer::component      = ComponentType::TITANIA;
const std::string   X3DExamineViewer::typeName       = "X3DExamineViewer";
const std::string   X3DExamineViewer::containerField = "viewer";

X3DExamineViewer::X3DExamineViewer () :
	        X3DViewer (),
	orientationOffset (),
	         rotation (),
	       fromVector (),
	        fromPoint (),
	        pressTime (0),
	       motionTime (0),
	           button (0),
	        numClicks (0),
	          spin_id ()
{
	addType (X3DConstants::X3DExamineViewer);
}

void
X3DExamineViewer::initialize ()
{
	try
	{
		X3DViewer::initialize ();

		getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &X3DExamineViewer::on_button_press_event));
		getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &X3DExamineViewer::on_button_release_event), false);
		getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &X3DExamineViewer::on_motion_notify_event),  false);
		getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (*this, &X3DExamineViewer::on_scroll_event));

		getNavigationInfo () -> transitionStart () .addInterest (&X3DExamineViewer::disconnect, this); // XXX: getNavigationInfo kann sich auch ?ndern, bei allen!!!
		getBrowser () -> getActiveViewpointEvent () .addInterest (&X3DExamineViewer::disconnect, this);
	}
	catch (const X3DError &)
	{ }
}

void
X3DExamineViewer::disconnect ()
{
	spin_id .disconnect ();
}

bool
X3DExamineViewer::on_button_press_event (GdkEventButton* event)
{
	switch (event -> type)
	{
		case GDK_BUTTON_PRESS:
		{
			if (button)
				return false;

			numClicks = 1;
			button    = event -> button;

			return on_1button_press_event (event);
		}
		case GDK_2BUTTON_PRESS:
		{
			numClicks = 2;
			button    = event -> button;

			return on_2button_press_event (event);
		}
		case GDK_3BUTTON_PRESS:
		{
			numClicks = 3;
			button    = event -> button;

			return on_3button_press_event (event);
		}
		default:
			return false;
	}

	return false;
}

bool
X3DExamineViewer::on_1button_press_event (GdkEventButton* event)
{
	try
	{
		//do this everywhere
		//if (not isInViewport (event))
		// return;
	
		pressTime = chrono::now ();
	
		switch (event -> button)
		{
			case 1:
			{
				disconnect ();
				getActiveViewpoint () -> transitionStop ();

				if (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
					return X3DExamineViewer::on_1button1_press_event (event);
	
				return on_1button1_press_event (event);
			}
			case 2:
				return on_1button2_press_event (event);
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DExamineViewer::on_1button1_press_event (GdkEventButton* event)
{
	try
	{
		getBrowser () -> setCursor ("move");

		fromVector = trackballProjectToSphere (event -> x, event -> y);
		rotation   = Rotation4d ();

		motionTime = 0;

		isActive () = true;
		return false;
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DExamineViewer::on_1button2_press_event (GdkEventButton* event)
{
	try
	{	
		disconnect ();

		getBrowser () -> setCursor ("move");
		getActiveViewpoint () -> transitionStop ();

		fromPoint = getPointOnCenterPlane (event -> x, event -> y);

		isActive () = true;
		return false;
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DExamineViewer::on_2button_press_event (GdkEventButton* event)
{
	return false;
}

bool
X3DExamineViewer::on_3button_press_event (GdkEventButton* event)
{
	return false;
}

bool
X3DExamineViewer::on_button_release_event (GdkEventButton* event)
{
	if (event -> button not_eq button)
	{
		button = 0;
		disconnect ();
		return false;
	}

	button = 0;
	
	switch (numClicks)
	{
		case 1:
			return on_1button_release_event (event);
		case 2:
			return on_2button_release_event (event);
		case 3:
			return on_3button_release_event (event);
		default:
			return false;
	}

	return false;
}

bool
X3DExamineViewer::on_1button_release_event (GdkEventButton* event)
{
	switch (event -> button)
	{
		case 1:
		{
			if (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
				return X3DExamineViewer::on_1button1_release_event (event);

			return on_1button1_release_event (event);
		}
		case 2:
			return on_1button2_release_event (event);
	}

	return false;
}

bool
X3DExamineViewer::on_1button1_release_event (GdkEventButton* event)
{
	getBrowser () -> setCursor ("default");

	if (std::abs (rotation .angle ()) > SPIN_ANGLE and chrono::now () - motionTime < SPIN_RELEASE_TIME)
	{
		rotation = slerp (Rotation4d (), rotation, SPIN_FACTOR);

		addSpinning ();
	}

	isActive () = false;
	return false;
}

bool
X3DExamineViewer::on_1button2_release_event (GdkEventButton* event)
{
	getBrowser () -> setCursor ("default");

	isActive () = false;
	return false;
}

bool
X3DExamineViewer::on_2button_release_event (GdkEventButton* event)
{
	//if (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
	//	lookAt (event -> x, event -> y, false, true);

	return false;
}

bool
X3DExamineViewer::on_3button_release_event (GdkEventButton* event)
{
	return false;
}

bool
X3DExamineViewer::on_motion_notify_event (GdkEventMotion* event)
{
	switch (button)
	{
		case 1:
		{
			if (getBrowser () -> getControlKey () and getBrowser () -> getShiftKey ())
				return X3DExamineViewer::on_motion1_notify_event (event);

			return on_motion1_notify_event (event);
		}
		case 2:
			return on_motion2_notify_event (event);
	}

	return false;
}

bool
X3DExamineViewer::on_motion1_notify_event (GdkEventMotion* event)
{
	try
	{
		const auto & viewpoint = getActiveViewpoint ();
		const auto   toVector  = trackballProjectToSphere (event -> x, event -> y);

		rotation = Rotation4d (toVector, fromVector);

		if (std::abs (rotation .angle ()) < SPIN_ANGLE and chrono::now () - pressTime < MOTION_TIME)
			return false;

		viewpoint -> orientationOffset () = getOrientationOffset ();
		viewpoint -> positionOffset ()    = getPositionOffset ();

		fromVector = toVector;

		motionTime = chrono::now ();

		return false;
	}
	catch (const X3DError &)
	{ }

	return false;
}
bool
X3DExamineViewer::on_motion2_notify_event (GdkEventMotion* event)
{
	try
	{
		const auto & viewpoint   = getActiveViewpoint ();
		const auto   toPoint     = getPointOnCenterPlane (event -> x, event -> y);
		const auto   translation = (fromPoint - toPoint) * viewpoint -> getUserOrientation ();
	
		viewpoint -> positionOffset ()         += translation;
		viewpoint -> centerOfRotationOffset () += translation;
	
		fromPoint = toPoint;
		return false;
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DExamineViewer::on_scroll_event (GdkEventScroll* event)
{
	try
	{
		const auto & viewpoint = getActiveViewpoint ();

		viewpoint -> transitionStop ();

		const auto step           = getDistanceToCenter () * SCROLL_FACTOR;
		const auto positionOffset = Vector3d (0, 0, abs (step)) * viewpoint -> getUserOrientation ();

		if (event -> direction == GDK_SCROLL_DOWN)    // Move backwards.
		{
			viewpoint -> positionOffset () += positionOffset;
		}

		else if (event -> direction == GDK_SCROLL_UP) // Move forwards.
		{
			viewpoint -> positionOffset () -= positionOffset;
		}

		scrollTime () = getCurrentTime ();
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DExamineViewer::spin ()
{
	try
	{
		const auto & viewpoint = getActiveViewpoint ();

		viewpoint -> orientationOffset () = getOrientationOffset ();
		viewpoint -> positionOffset ()    = getPositionOffset ();

		return true;
	}
	catch (const X3DError &)
	{
		return false;
	}
}

void
X3DExamineViewer::addSpinning ()
{
	if (not spin_id .connected ())
		spin_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DExamineViewer::spin), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

Vector3d
X3DExamineViewer::getPositionOffset () const
{
	const auto & viewpoint = getActiveViewpoint ();
	const auto   distance  = getDistanceToCenter ();

	return distance * (~orientationOffset * viewpoint -> orientationOffset ()) - distance + viewpoint -> positionOffset ();
}

Rotation4d
X3DExamineViewer::getOrientationOffset ()
{
	const auto & viewpoint = getActiveViewpoint ();

	// Assign last value to global orientationOffset
	orientationOffset = viewpoint -> orientationOffset ();

	{
		auto orientationOffset = ~viewpoint -> getOrientation () * rotation * viewpoint -> getUserOrientation ();

		if (getBrowser () -> getStraightenHorizon ())
			orientationOffset *= viewpoint -> straightenHorizon (viewpoint -> getOrientation () * orientationOffset);

		return orientationOffset;
	}
}

} // X3D
} // titania
