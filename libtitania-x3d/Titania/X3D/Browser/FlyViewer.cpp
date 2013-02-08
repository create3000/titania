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

#include "FlyViewer.h"

#include "../Browser/Browser.h"
#include "../Components/Geospatial/GeoViewpoint.h"
#include "../Components/Navigation/OrthoViewpoint.h"

#include <cmath>
#include <glibmm/main.h>

#define SPEED_FACTOR            0.02f
#define ROTATION_SPEED_FACTOR   0.2f
#define FRAME_RATE              100

namespace titania {
namespace X3D {

FlyViewer::FlyViewer (Browser* const browser, NavigationInfo* navigationInfo) :
	                      X3DViewer (browser),        
	                 navigationInfo (navigationInfo), 
	                     fromVector (),               
	                       toVector (),               
	                      direction (),               
	                         button (0),              
	                      shift_key (false),          
	button_press_event_connection   (),               
	button_release_event_connection (),               
	motion_notify_event_connection  (),               
	                         fly_id (),                
	                         pan_id ()                
{ }

void
FlyViewer::initialize ()
{
	button_press_event_connection   = getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &FlyViewer::on_button_press_event));
	button_release_event_connection = getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &FlyViewer::on_button_release_event));
	motion_notify_event_connection  = getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &FlyViewer::on_motion_notify_event));
	key_press_event_connection      = getBrowser () -> signal_key_press_event      () .connect (sigc::mem_fun (*this, &FlyViewer::on_key_press_event));
	key_release_event_connection    = getBrowser () -> signal_key_release_event    () .connect (sigc::mem_fun (*this, &FlyViewer::on_key_release_event));
}

bool
FlyViewer::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;

	if (button == 1)
	{
		fromVector = toVector = Vector3f (event -> x, 0, event -> y);

		if (getBrowser () -> getBrowserOptions () -> rubberBand)
			getBrowser () -> displayed .addInterest (this, &FlyViewer::display);
	}

	else if (button == 2)
	{
		fromVector = toVector = Vector3f (event -> x, -event -> y, 0);
	}

	return false;
}

bool
FlyViewer::on_button_release_event (GdkEventButton* event)
{
	if (button == 1)
	{
		fly_id .disconnect ();

		getBrowser () -> displayed .removeInterest (this, &FlyViewer::display);
		getBrowser () -> changed .processInterests ();
	}
	
	else if (button == 2)
	{
		pan_id .disconnect ();
	}

	button = 0;

	return false;
}

bool
FlyViewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 1)
	{
		toVector = Vector3f (event -> x, 0, event -> y);

		direction = (toVector - fromVector) * SPEED_FACTOR * navigationInfo -> speed .getValue ();

		addFly ();
	}

	else if (button == 2)
	{
		toVector = Vector3f (event -> x, -event -> y, 0);

		direction = (toVector - fromVector) * SPEED_FACTOR * navigationInfo -> speed .getValue ();

		addPan ();
	}

	return false;
}

bool
FlyViewer::on_key_press_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Shift_L or event -> keyval == GDK_KEY_Shift_R)
		shift_key = true;

	return false;
}

bool
FlyViewer::on_key_release_event (GdkEventKey* event)
{
	if (event -> keyval == GDK_KEY_Shift_L or event -> keyval == GDK_KEY_Shift_R)
		shift_key = false;

	return false;
}

#define M_PI1_2  (M_PI / 2)

bool
FlyViewer::fly ()
{
	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	float frameRate = std::max <float> (getBrowser () -> getCurrentFrameRate (), FRAME_RATE);

	Rotation4f rotation = direction .z () > 0
	                      ? ~Rotation4f (Vector3f (0, 0, 1), direction)
								 : Rotation4f (Vector3f (0, 0, -1), direction);

	float angle = rotation .angle ();

	rotation .angle (angle * math::abs (direction) * ROTATION_SPEED_FACTOR / frameRate);

	float speed_factor = shift_key ? 4 : 1;
	speed_factor *= 1 - angle / M_PI1_2;

	viewpoint -> orientationOffset *= rotation;
	viewpoint -> positionOffset    += viewpoint -> getUserOrientation () * direction * speed_factor / frameRate;

	//__LOG__ << math::abs (viewpoint -> getUserOrientation () * direction * speed_factor / frameRate) << std::endl;

	return true;
}

void
FlyViewer::addFly ()
{
	if (not fly_id .connected ())
		fly_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &FlyViewer::fly), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

bool
FlyViewer::pan ()
{
	X3DViewpointNode* viewpoint = getBrowser () -> getActiveViewpoint ();

	float frameRate = std::max <float> (getBrowser () -> getCurrentFrameRate (), FRAME_RATE);

	float speed_factor = shift_key ? 4 : 1;

	viewpoint -> positionOffset += viewpoint -> getUserOrientation () * direction * speed_factor / frameRate;

	//__LOG__ << math::abs (viewpoint -> getUserOrientation () * direction * speed_factor / frameRate) << std::endl;

	return true;
}

void
FlyViewer::addPan ()
{
	if (not pan_id .connected ())
		pan_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &FlyViewer::pan), 1000.0 / FRAME_RATE, GDK_PRIORITY_REDRAW);
}

void
FlyViewer::display ()
{
	if (button == 1)
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

		glDisable (GL_DEPTH_TEST);

		// Display Rubberband.

		Matrix4d::array_type modelview, projection;

		glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev (GL_PROJECTION_MATRIX, projection);

		GLdouble fx, fy, fz, tx, ty, tz;

		// Starting point
		gluUnProject (fromVector .x (), height - fromVector .z (), 0, modelview, projection, viewport, &fx, &fy, &fz);

		gluUnProject (toVector .x (), height - toVector .z (), 0, modelview, projection, viewport, &tx, &ty, &tz);

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

}

FlyViewer::~FlyViewer ()
{
	button_press_event_connection   .disconnect ();
	motion_notify_event_connection  .disconnect ();
	button_release_event_connection .disconnect ();
	key_press_event_connection      .disconnect ();
	key_release_event_connection    .disconnect ();
	fly_id .disconnect ();
	pan_id .disconnect ();
}

} // X3D
} // titania
