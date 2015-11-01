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

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/ViewVolume.h"
#include "../Browser.h"
#include "../BrowserOptions.h"

#include <Titania/Chrono/Now.h>
#include <Titania/Math/Geometry/Camera.h>

#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr time_type SPEED_FACTOR           = 0.007;
static constexpr time_type SHIFT_SPEED_FACTOR     = 4 * SPEED_FACTOR;
static constexpr time_type ROTATION_SPEED_FACTOR  = 1.4;
static constexpr int       ROTATION_LIMIT         = 40;
static constexpr time_type PAN_SPEED_FACTOR       = SPEED_FACTOR;
static constexpr time_type PAN_SHIFT_SPEED_FACTOR = 1.4 * PAN_SPEED_FACTOR;
static constexpr double    ROLL_ANGLE             = M_PI / 32;
static constexpr time_type ROLL_TIME              = 0.2;
static constexpr time_type FRAME_RATE             = 60;

static constexpr Vector3f yAxis (0, 1, 0);

X3DFlyViewer::X3DFlyViewer () :
	          X3DViewer (),
	         fromVector (),
	           toVector (),
	          direction (),
	     sourceRotation (),
	destinationRotation (),
	          startTime (),
	             button (0),
	             fly_id (),
	             pan_id (),
	            roll_id ()
{ }

void
X3DFlyViewer::initialize ()
{
	X3DViewer::initialize ();

	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_button_press_event));
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_button_release_event), false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_motion_notify_event),  false);
	getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (*this, &X3DFlyViewer::on_scroll_event));

	getBrowser () -> hasControlKey () .addInterest (this, &X3DFlyViewer::disconnect);

	//getActiveViewpoint () -> straighten (true); // Do this only with Walk Viewer, wenn ®πberhaupt.
}

bool
X3DFlyViewer::on_button_press_event (GdkEventButton* event)
{
	try
	{
		if (button)
			return false;

		switch (event -> button)
		{
			case 1:
			{
				button = event -> button;

				disconnect ();

				getBrowser () -> addEvent ();
				getBrowser () -> setCursor (Gdk::FLEUR);
				getActiveViewpoint () -> transitionStop ();
				addCollision ();

				if (getBrowser () -> hasControlKey ())
				{
					// Look around.

					fromVector = trackballProjectToSphere (event -> x, event -> y);
				}
				else
				{
					// Move.

					fromVector = toVector = Vector3f (event -> x, 0, event -> y);

					if (getBrowser () -> getBrowserOptions () -> RubberBand ())
						getBrowser () -> displayed () .addInterest (this, &X3DFlyViewer::display);
				}

				return false;
			}

			case 2:
			{
				button = event -> button;

				disconnect ();

				getBrowser () -> addEvent ();
				getBrowser () -> setCursor (Gdk::FLEUR);
				getActiveViewpoint () -> transitionStop ();
				addCollision ();

				fromVector = toVector = Vector3f (event -> x, -event -> y, 0);

				return false;
			}
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DFlyViewer::on_button_release_event (GdkEventButton* event)
{
	if (event -> button not_eq button)
		return false;

	button = 0;

	disconnect ();

	getBrowser () -> addEvent ();
	getBrowser () -> setCursor (Gdk::TOP_LEFT_ARROW);
	removeCollision ();

	return false;
}

bool
X3DFlyViewer::on_motion_notify_event (GdkEventMotion* event)
{
	try
	{
		getBrowser () -> addEvent ();

		switch (button)
		{
			case 1:
			{
				if (getBrowser () -> hasControlKey ())
				{
					// Look around

					const auto & viewpoint = getActiveViewpoint ();

					auto       orientation = getActiveViewpoint () -> getUserOrientation ();
					const auto toVector    = trackballProjectToSphere (event -> x, event -> y);

					orientation  = Rotation4f (toVector, fromVector) * orientation;
					orientation *= viewpoint -> straightenHorizon (orientation);

					getActiveViewpoint () -> orientationOffset () = ~getActiveViewpoint () -> getOrientation () * orientation;

					fromVector = toVector;
				}
				else
				{
					// Fly

					toVector  = Vector3f (event -> x, 0, event -> y);
					direction = toVector - fromVector;

					addFly ();
				}

				return false;
			}

			case 2:
			{
				// Pan

				toVector  = Vector3f (event -> x, -event -> y, 0);
				direction = toVector - fromVector;

				addPan ();

				return false;
			}
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DFlyViewer::on_scroll_event (GdkEventScroll* event)
{
	try
	{
		const auto & viewpoint = getActiveViewpoint ();

		viewpoint  -> transitionStop ();

		if (event -> direction == GDK_SCROLL_UP)
		{
			sourceRotation      = viewpoint -> orientationOffset ();
			destinationRotation = sourceRotation * Rotation4f (Vector3f (-1, 0, 0) * viewpoint -> getUserOrientation (), ROLL_ANGLE);
			addRoll ();
		}

		else if (event -> direction == GDK_SCROLL_DOWN)
		{
			sourceRotation      = viewpoint -> orientationOffset ();
			destinationRotation = sourceRotation * Rotation4f (Vector3f (1, 0, 0) * viewpoint -> getUserOrientation (), ROLL_ANGLE);
			addRoll ();
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
X3DFlyViewer::fly ()
{
	try
	{
		const time_type now = chrono::now ();
		const time_type dt  = now - startTime;

		const auto & viewpoint = getActiveViewpoint ();
		const auto   upVector  = viewpoint -> getUpVector ();

		// Rubberband values

		const Rotation4f up (yAxis, upVector);

		const Rotation4f rubberBandRotation = direction .z () > 0
		                                      ? Rotation4f (direction * up, Vector3f (0, 0, 1) * up)
														  : Rotation4f (Vector3f (0, 0, -1) * up, direction * up);

		const double rubberBandLength = abs (direction);

		// Position offset

		float speedFactor = 1 - rubberBandRotation .angle () / M_PI1_2;

		speedFactor *= getNavigationInfo () -> speed ();
		speedFactor *= viewpoint -> getSpeedFactor ();
		speedFactor *= getBrowser () -> hasShiftKey () ? SHIFT_SPEED_FACTOR : SPEED_FACTOR;
		speedFactor *= dt;

		const auto translation = getTranslationOffset (speedFactor * direction);

		viewpoint -> positionOffset () += getActiveLayer () -> constrainTranslation (translation);

		// Rotation

		float weight = ROTATION_SPEED_FACTOR * dt;

		weight *= std::pow (rubberBandLength / (rubberBandLength + ROTATION_LIMIT), 2);

		viewpoint -> orientationOffset () *= slerp (Rotation4f (), rubberBandRotation, weight);

		// GeoRotation

		const auto geoRotation = Rotation4f (upVector, viewpoint -> getUpVector ());

		viewpoint -> orientationOffset () *= geoRotation;

		startTime = now;
		return true;
	}
	catch (const X3DError &)
	{
		return false;
	}
}

bool
X3DFlyViewer::pan ()
{
	try
	{
		const time_type now = chrono::now ();
		const time_type dt  = now - startTime;

		const auto & viewpoint = getActiveViewpoint ();
		const auto   upVector  = viewpoint -> getUpVector ();

		float speedFactor = 1.0;

		speedFactor *= getNavigationInfo () -> speed ();
		speedFactor *= viewpoint -> getSpeedFactor ();
		speedFactor *= getBrowser () -> hasShiftKey () ? PAN_SHIFT_SPEED_FACTOR : PAN_SPEED_FACTOR;
		speedFactor *= dt;

		const auto orientation = viewpoint -> getUserOrientation () * Rotation4f (yAxis * viewpoint -> getUserOrientation (), upVector);
		const auto translation = speedFactor * direction * orientation;

		viewpoint -> positionOffset () += getActiveLayer () -> constrainTranslation (translation);

		startTime = now;
		return true;
	}
	catch (const X3DError &)
	{
		return false;
	}
}

bool
X3DFlyViewer::roll ()
{
	try
	{
		const time_type elapsedTime = chrono::now () - startTime;

		if (elapsedTime > ROLL_TIME)
			return false;

		const auto & viewpoint = getActiveViewpoint ();

		viewpoint -> orientationOffset () = slerp <float> (sourceRotation, destinationRotation, elapsedTime / ROLL_TIME);

		return true;
	}
	catch (const X3DError &)
	{
		return false;
	}
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
	getBrowser () -> displayed () .removeInterest (this, &X3DFlyViewer::display);

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

		const auto & viewport = getBrowser () -> getRectangle ();
		const int    width    = viewport [2];
		const int    height   = viewport [3];

		const Matrix4d projection = ortho <double> (0, width, 0, height, -1, 1);

		glMatrixMode (GL_PROJECTION);
		glLoadMatrixd (projection .data ());
		glMatrixMode (GL_MODELVIEW);

		// Display Rubberband.

		glDisable (GL_DEPTH_TEST);

		glLoadIdentity ();

		const Vector3d fromPoint (fromVector .x (), height - fromVector .z (), 0);
		const Vector3d toPoint   (toVector   .x (), height - toVector   .z (), 0);

		// Draw a black and a white line.
		glLineWidth (2);
		glColor3f (0, 0, 0);

		glBegin (GL_LINES);
		glVertex3dv (fromPoint .data ());
		glVertex3dv (toPoint   .data ());
		glEnd ();

		glLineWidth (1);
		glColor3f (1, 1, 1);

		glBegin (GL_LINES);
		glVertex3dv (fromPoint .data ());
		glVertex3dv (toPoint   .data ());
		glEnd ();

		glEnable (GL_DEPTH_TEST);
	}
	catch (const std::domain_error &)
	{
		// unProjectPoint is not posible
	}
}

void
X3DFlyViewer::dispose ()
{
	X3DViewer::dispose ();
}

X3DFlyViewer::~X3DFlyViewer ()
{ }

} // X3D
} // titania
