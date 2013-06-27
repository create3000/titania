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

#include "X3DFlyViewer.h"

#include "../Browser.h"

#include <Titania/Chrono/Now.h>
#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr float SPEED_FACTOR          = 0.007;
static constexpr float SHIFT_SPEED_FACTOR    = 4;
static constexpr float ROTATION_SPEED_FACTOR = 0.3;
//static constexpr float PAN_SPEED_FACTOR       = 2;
static constexpr float PAN_SHIFT_SPEED_FACTOR = 4;
static constexpr float ROLL_ANGLE             = M_PI / 16;
static constexpr float ROLL_TIME              = 0.2;
static constexpr float FRAME_RATE             = 100;

Vector3f X3DFlyViewer::upVector (0, 1, 0);

X3DFlyViewer::X3DFlyViewer (Browser* const browser, NavigationInfo* navigationInfo) :
	     X3DViewer (browser),
	navigationInfo (navigationInfo),
	    fromVector (),
	      toVector (),
	     direction (),
	      rotation (),
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
}

bool
X3DFlyViewer::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	disconnect ();

	getBrowser () -> notify ();

	if (button == 1)
	{
		fromVector = toVector = Vector3f (event -> x, 0, event -> y);

		if (getBrowser () -> getBrowserOptions () -> rubberBand ())
			getBrowser () -> displayed .addInterest (this, &X3DFlyViewer::display);
	}

	else if (button == 2)
	{
		fromVector = toVector = Vector3f (event -> x, -event -> y, 0);
	}

	return false;
}

bool
X3DFlyViewer::on_button_release_event (GdkEventButton* event)
{
	disconnect ();

	getBrowser () -> notify ();

	if (button == 1)
	{
		getBrowser () -> displayed .removeInterest (this, &X3DFlyViewer::display);
	}

	else if (button == 2)
	{ }

	button = 0;

	return false;
}

bool
X3DFlyViewer::on_motion_notify_event (GdkEventMotion* event)
{
	getBrowser () -> notify ();

	if (button == 1)
	{
		toVector = Vector3f (event -> x, 0, event -> y);

		direction = (toVector - fromVector) * SPEED_FACTOR * navigationInfo -> speed () .getValue ();

		addFly ();

		return true;
	}

	else if (button == 2)
	{
		toVector = Vector3f (event -> x, -event -> y, 0);

		direction = (toVector - fromVector) * SPEED_FACTOR * navigationInfo -> speed () .getValue ();

		addPan ();

		return true;
	}

	return false;
}

bool
X3DFlyViewer::on_scroll_event (GdkEventScroll* event)
{
	disconnect ();

	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	if (event -> direction == 0)      // Roll up.
	{
		rotation  = Rotation4f (viewpoint -> getUserOrientation () * Vector3f (-1, 0, 0), ROLL_ANGLE / ROLL_TIME);
		startTime = chrono::now ();
		addRoll ();
	}

	else if (event -> direction == 1) // Roll down.
	{
		rotation  = Rotation4f (viewpoint -> getUserOrientation () * Vector3f (1, 0, 0), ROLL_ANGLE / ROLL_TIME);
		startTime = chrono::now ();
		addRoll ();
	}

	return false;
}

bool
X3DFlyViewer::on_key_press_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			keys .shift_l = true;
			break;
		case GDK_KEY_Shift_R:
			keys .shift_r = true;
			break;
		default:
			break;
	}

	return false;
}

bool
X3DFlyViewer::on_key_release_event (GdkEventKey* event)
{
	switch (event -> keyval)
	{
		case GDK_KEY_Shift_L:
			keys .shift_l = false;
			break;
		case GDK_KEY_Shift_R:
			keys .shift_r = false;
			break;
		default:
			break;
	}

	return false;
}

bool
X3DFlyViewer::fly ()
{
	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	float frameRate = getBrowser () -> getCurrentFrameRate ();

	// Orientation offeset

	Rotation4f rotation = direction .z () > 0
	                      ? ~Rotation4f (Vector3f (0, 0, 1), direction)
								 : Rotation4f (Vector3f (0, 0, -1), direction);

	float angle = rotation .angle ();

	viewpoint -> orientationOffset () *= rotation * (math::abs (direction) * ROTATION_SPEED_FACTOR / frameRate);

	// Position offset

	float speed_factor = keys .shift () ? SHIFT_SPEED_FACTOR : 1;
	speed_factor *= 1 - angle / M_PI1_2;

	Rotation4f orientation = viewpoint -> getUserOrientation () * Rotation4f (viewpoint -> getUserOrientation () * upVector, upVector);
	Vector3f   translation = orientation * direction * speed_factor / frameRate;

	viewpoint -> positionOffset () += getTranslation (translation);

	return true;
}

bool
X3DFlyViewer::pan ()
{
	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	float frameRate = getBrowser () -> getCurrentFrameRate ();

	float speed_factor = keys .shift () ? PAN_SHIFT_SPEED_FACTOR : 1;

	Rotation4f orientation = viewpoint -> getUserOrientation () * Rotation4f (viewpoint -> getUserOrientation () * upVector, upVector);
	Vector3f   translation = orientation * direction * speed_factor / frameRate;

	viewpoint -> positionOffset () += getTranslation (translation);

	return true;
}

bool
X3DFlyViewer::roll ()
{
	if (chrono::now () - startTime > ROLL_TIME)
		return false;

	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	float frameRate = getBrowser () -> getCurrentFrameRate ();

	viewpoint -> orientationOffset () *= Rotation4f (rotation .axis (), rotation .angle () / frameRate);

	return true;
}

Vector3f
X3DFlyViewer::getTranslation (const Vector3f & translation) const
{
	auto navigationInfo = getBrowser () -> getActiveNavigationInfo ();

	float zFar            = navigationInfo -> getFarPlane ();
	float collisionRadius = navigationInfo -> getCollisionRadius ();

	float distance = getDistance (translation);
	float length   = math::abs (translation);

	if (zFar - distance > 0) // Are there polygons under the viewer
	{
		distance -= collisionRadius;

		if (distance > 0)
		{
			// Move

			if (length > distance)
			{
				// Collision: The ground is reached.
				//__LOG__ << std::endl;
				return normalize (translation) * distance;
			}

			else
				return translation;
		}
		else
		{
			// Collision
			//__LOG__ << std::endl;
			return Vector3f ();
		}
	}
	else
		return translation;
}

float
X3DFlyViewer::getDistance (const Vector3f & direction) const
{
	// Calculate position offset

	NavigationInfo* navigationInfo  = getBrowser () -> getActiveNavigationInfo ();
	float           collisionRadius = navigationInfo -> getCollisionRadius ();
	float           positionOffset  = (collisionRadius + navigationInfo -> getAvatarHeight () - navigationInfo -> getStepHeight ()) / 2 - collisionRadius;

	// Translate camera

	X3DViewpointNode* viewpoint         = getBrowser () -> getActiveViewpoint ();
	Matrix4f          cameraSpaceMatrix = viewpoint -> getModelViewMatrix ();

	cameraSpaceMatrix .translate (viewpoint -> getUserPosition () - Vector3f (0, positionOffset, 0));
	cameraSpaceMatrix .rotate (Rotation4f (Vector3f (0, 0, 1), -direction));

	glLoadMatrixf (inverse (cameraSpaceMatrix) .data ());

	// Traverse and get distance

	const auto & activeLayer = getBrowser () -> getActiveLayer ();
	activeLayer -> traverse (TraverseType::NAVIGATION);

	return activeLayer -> getDistance ();
}

void
X3DFlyViewer::addFly ()
{
	if (not fly_id .connected ())
		fly_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::fly), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

void
X3DFlyViewer::addPan ()
{
	if (not pan_id .connected ())
		pan_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::pan), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

void
X3DFlyViewer::addRoll ()
{
	if (not roll_id .connected ())
		roll_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &X3DFlyViewer::roll), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

void
X3DFlyViewer::disconnect ()
{
	fly_id  .disconnect ();
	pan_id  .disconnect ();
	roll_id .disconnect ();
}

void
X3DFlyViewer::display ()
{
	// Configure HUD

	GLint viewport [4];

	glGetIntegerv (GL_VIEWPORT, viewport);

	GLint width  = viewport [2];
	GLint height = viewport [3];

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (0, width, 0, height, -1, 1);
	glMatrixMode (GL_MODELVIEW);

	glLoadIdentity ();

	glDisable (GL_DEPTH_TEST);

	// Display Rubberband.

	Matrix4d::array_type modelview, projection;

	glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, projection);

	GLdouble fx, fy, fz, tx, ty, tz;

	// From point
	gluUnProject (fromVector .x (), height - fromVector .z (), 0, modelview, projection, viewport, &fx, &fy, &fz);

	// To point
	gluUnProject (toVector .x (), height - toVector .z (), 0, modelview, projection, viewport, &tx, &ty, &tz);

	// Draw a black and a white line
	glLineWidth (2);
	glColor3f (0, 0, 0);

	glBegin (GL_LINES);
	glVertex3f (fx, fy, fz);
	glVertex3f (tx, ty, tz);
	glEnd ();

	glLineWidth (1);
	glColor3f (1, 1, 1);

	glBegin (GL_LINES);
	glVertex3f (fx, fy, fz);
	glVertex3f (tx, ty, tz);
	glEnd ();
}

X3DFlyViewer::~X3DFlyViewer ()
{
	getBrowser () -> displayed .removeInterest (this, &X3DFlyViewer::display);

	disconnect ();
}

} // X3D
} // titania
