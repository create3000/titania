/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DFlyViewer.h"

#include "../../Rendering/ViewVolume.h"
#include "../X3DBrowserSurface.h"

#include <Titania/Chrono/Now.h>
#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr float     SPEED_FACTOR           = 0.007;
static constexpr float     SHIFT_SPEED_FACTOR     = 4;
static constexpr float     ROTATION_SPEED_FACTOR  = 0.48;
static constexpr float     PAN_SHIFT_SPEED_FACTOR = 4;
static constexpr float     ROLL_ANGLE             = M_PI / 32;
static constexpr time_type ROLL_TIME              = 0.2;
static constexpr double    FRAME_RATE             = 60;

Vector3f X3DFlyViewer::upVector (0, 1, 0);

X3DFlyViewer::X3DFlyViewer (X3DBrowserSurface* const browser, NavigationInfo* const navigationInfo) :
	          X3DViewer (browser),
	     navigationInfo (navigationInfo),
	        orientation (),
	         fromVector (),
	           toVector (),
	          direction (),
	     sourceRotation (),
	destinationRotation (),
	          startTime (),
	             button (0),
	               keys (),
	             fly_id (),
	             pan_id (),
	            roll_id ()
{ }

void
X3DFlyViewer::initialize ()
{
	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_button_press_event));
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_button_release_event));
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_motion_notify_event), false);
	getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_scroll_event));
	getBrowser () -> signal_key_press_event      () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_key_press_event));
	getBrowser () -> signal_key_release_event    () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_key_release_event));

	//getActiveViewpoint () -> straighten (true);
}

bool
X3DFlyViewer::on_button_press_event (GdkEventButton* event)
{
	disconnect ();

	button = event -> button;

	getBrowser () -> addEvent ();

	if (button == 1)
	{
		getActiveViewpoint () -> transitionStop ();

		if (keys .control ())
		{
			orientation = getActiveViewpoint () -> getUserOrientation ();
			
			fromVector = trackballProjectToSphere (event -> x, event -> y);
		}
		else
		{
			fromVector = toVector = Vector3f (event -> x, 0, event -> y);

			if (getBrowser () -> getBrowserOptions () -> rubberBand ())
				getBrowser () -> displayed () .addInterest (this, &X3DFlyViewer::display);
		}
	}

	else if (button == 2)
	{
		getActiveViewpoint () -> transitionStop ();

		fromVector = toVector = Vector3f (event -> x, -event -> y, 0);
	}

	return false;
}

bool
X3DFlyViewer::on_button_release_event (GdkEventButton* event)
{
	disconnect ();

	return false;
}

bool
X3DFlyViewer::on_motion_notify_event (GdkEventMotion* event)
{
	getBrowser () -> addEvent ();

	if (button == 1)
	{
		if (keys .control ())
		{
			Vector3f toVector = trackballProjectToSphere (event -> x, event -> y);
			
			orientation = Rotation4f (toVector, fromVector) * orientation;
			orientation = orientation * X3DViewpointNode::straightenHorizon (orientation);

			getActiveViewpoint () -> orientationOffset () = ~getActiveViewpoint () -> orientation () * orientation;

			fromVector = toVector;
		}
		else
		{
			toVector  = Vector3f (event -> x, 0, event -> y);
			direction = (toVector - fromVector) * (SPEED_FACTOR * navigationInfo -> speed ());

			addFly ();
		}
	}

	else if (button == 2)
	{
		toVector  = Vector3f (event -> x, -event -> y, 0);
		direction = (toVector - fromVector) * (SPEED_FACTOR * navigationInfo -> speed ());

		addPan ();
	}

	return false;
}

bool
X3DFlyViewer::on_scroll_event (GdkEventScroll* event)
{
	auto viewpoint = getActiveViewpoint ();
	
	viewpoint  -> transitionStop ();

	if (event -> direction == GDK_SCROLL_UP)
	{
		sourceRotation      = viewpoint -> orientationOffset ();
		destinationRotation = sourceRotation * Rotation4f (viewpoint -> getUserOrientation () * Vector3f (-1, 0, 0), ROLL_ANGLE);
		addRoll ();
	}

	else if (event -> direction == GDK_SCROLL_DOWN)
	{
		sourceRotation      = viewpoint -> orientationOffset ();
		destinationRotation = sourceRotation * Rotation4f (viewpoint -> getUserOrientation () * Vector3f (1, 0, 0), ROLL_ANGLE);
		addRoll ();
	}

	return false;
}

bool
X3DFlyViewer::on_key_press_event (GdkEventKey* event)
{
	keys .press (event);
	
	switch (event -> keyval)
	{
		case GDK_KEY_Control_L:
		case GDK_KEY_Control_R:
		{
			disconnect ();
			break;
		}
		default:
			break;
	}

	return false;
}

bool
X3DFlyViewer::on_key_release_event (GdkEventKey* event)
{
	keys .release (event);

	switch (event -> keyval)
	{
		case GDK_KEY_Control_L:
		case GDK_KEY_Control_R:
		{
			disconnect ();
			break;
		}
		default:
			break;
	}

	return false;
}

bool
X3DFlyViewer::fly ()
{
	time_type now = chrono::now ();
	float     dt  = now - startTime;

	auto viewpoint = getActiveViewpoint ();

	// Orientation offset

	Rotation4f rotation = direction .z () > 0
	                      ? Rotation4f (direction, Vector3f (0, 0, 1))
								 : Rotation4f (Vector3f (0, 0, -1), direction);

	viewpoint -> orientationOffset () *= math::slerp <float> (Rotation4f (), rotation, math::abs (direction) / navigationInfo -> getAvatarHeight () * ROTATION_SPEED_FACTOR * dt);

	// Position offset

	float speed_factor = keys .shift () ? SHIFT_SPEED_FACTOR : 1.0;
	speed_factor *= 1 - rotation .angle () / M_PI1_2;

	Vector3f translation = getTranslationOffset (direction * (speed_factor * dt));

	viewpoint -> positionOffset () += getTranslation (translation);

	startTime = now;
	return true;
}

bool
X3DFlyViewer::pan ()
{
	time_type now = chrono::now ();
	float     dt  = now - startTime;

	auto viewpoint = getActiveViewpoint ();

	float speed_factor = keys .shift () ? PAN_SHIFT_SPEED_FACTOR : 1.0;

	Rotation4f orientation = viewpoint -> getUserOrientation () * Rotation4f (viewpoint -> getUserOrientation () * upVector, upVector);
	Vector3f   translation = orientation * direction * (speed_factor * dt);

	viewpoint -> positionOffset () += getTranslation (translation);

	startTime = now;
	return true;
}

bool
X3DFlyViewer::roll ()
{
	time_type elapsedTime = chrono::now () - startTime;

	if (elapsedTime > ROLL_TIME)
		return false;

	auto viewpoint = getActiveViewpoint ();

	viewpoint -> orientationOffset () = math::slerp <float> (sourceRotation, destinationRotation, elapsedTime / ROLL_TIME);

	return true;
}

Vector3f
X3DFlyViewer::getTranslation (const Vector3f & translation) const
{
	// Get position offset

	float collisionRadius = navigationInfo -> getCollisionRadius ();
	float positionOffset  = (collisionRadius + navigationInfo -> getAvatarHeight () - navigationInfo -> getStepHeight ()) / 2 - collisionRadius;

	// Get width and height of camera

	float width  = collisionRadius * 2;
	float height = collisionRadius + navigationInfo -> getAvatarHeight () - navigationInfo -> getStepHeight ();

	return getBrowser () -> getActiveLayer () -> getTranslation (Vector3f (0, -positionOffset, 0), width, height, translation);
}

void
X3DFlyViewer::addFly ()
{
	if (not fly_id .connected ())
	{
		startTime = chrono::now ();
		fly_id    = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::fly), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
	}
}

void
X3DFlyViewer::addPan ()
{
	if (not pan_id .connected ())
	{
		startTime = chrono::now ();
		pan_id    = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::pan), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
	}
}

void
X3DFlyViewer::addRoll ()
{
	if (not roll_id .connected ())
	{
		disconnect ();
		roll_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::roll), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
	}

	startTime = chrono::now ();
}

void
X3DFlyViewer::disconnect ()
{
	getBrowser () -> addEvent ();

	if (button == 1)
	{
		getBrowser () -> displayed () .removeInterest (this, &X3DFlyViewer::display);
	}

	else if (button == 2)
	{ }

	button = 0;

	fly_id  .disconnect ();
	pan_id  .disconnect ();
	roll_id .disconnect ();
}

void
X3DFlyViewer::display ()
{
	try
	{
		// Configure HUD

		Vector4i viewport = Viewport4i ();

		int width  = viewport [2];
		int height = viewport [3];

		glDisable (GL_DEPTH_TEST);

		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (0, width, 0, height, -1, 1);
		glMatrixMode (GL_MODELVIEW);

		glLoadIdentity ();

		// Display Rubberband.

		Matrix4d modelview; // Use identity
		Matrix4d projection = ProjectionMatrix4d ();

		// From point
		Vector3d from = ViewVolume::unProjectPoint (fromVector .x (), height - fromVector .z (), 0, modelview, projection, viewport);

		// To point
		Vector3d to = ViewVolume::unProjectPoint (toVector .x (), height - toVector .z (), 0, modelview, projection, viewport);

		// Draw a black and a white line
		glLineWidth (2);
		glColor3f (0, 0, 0);

		glBegin (GL_LINES);
		glVertex3dv (from .data ());
		glVertex3dv (to   .data ());
		glEnd ();

		glLineWidth (1);
		glColor3f (1, 1, 1);

		glBegin (GL_LINES);
		glVertex3dv (from .data ());
		glVertex3dv (to   .data ());
		glEnd ();

		glEnable (GL_DEPTH_TEST);
	}
	catch (const std::domain_error &)
	{
		// unproject is not posible
	}
}

} // X3D
} // titania
