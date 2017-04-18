/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra?e 31a, Leipzig, Germany 2011.
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

#include "X3DFlyViewer.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Rendering/ViewVolume.h"
#include "../Browser.h"
#include "../BrowserOptions.h"
#include "../../Rendering/OpenGL.h"

#include <Titania/Chrono/Now.h>
#include <Titania/Math/Geometry/Camera.h>

#include <cmath>
#include <glibmm/main.h>

namespace titania {
namespace X3D {

static constexpr time_type SPEED_FACTOR           = 0.007;
static constexpr time_type SHIFT_SPEED_FACTOR     = 4 * SPEED_FACTOR;
static constexpr time_type ROTATION_SPEED_FACTOR  = 1.4;
static constexpr int32_t   ROTATION_LIMIT         = 40;
static constexpr time_type PAN_SPEED_FACTOR       = SPEED_FACTOR;
static constexpr time_type PAN_SHIFT_SPEED_FACTOR = 1.4 * PAN_SPEED_FACTOR;
static constexpr double    ROLL_ANGLE             = pi <double> / 32;
static constexpr time_type ROLL_TIME              = 0.2;
static constexpr time_type FRAME_RATE             = 60;

static constexpr Vector3d yAxis (0, 1, 0);

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

	getBrowser () -> getControlKey () .addInterest (&X3DFlyViewer::disconnect, this);

	//getActiveViewpoint () -> straighten (true); // Do this only with Walk Viewer, wenn ��berhaupt.
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
				getBrowser () -> setCursor ("move");
				getActiveViewpoint () -> transitionStop ();
				addCollision ();

				if (getBrowser () -> getControlKey ())
				{
					// Look around.

					fromVector = trackballProjectToSphere (event -> x, event -> y);
				}
				else
				{
					// Move.

					fromVector = toVector = Vector3d (event -> x, 0, event -> y);

					if (getBrowser () -> getBrowserOptions () -> RubberBand ())
						getBrowser () -> displayed () .addInterest (&X3DFlyViewer::display, this, MoveType::MOVE);
				}

				isActive () = true;
				return false;
			}

			case 2:
			{
				// Pan

				button = event -> button;

				disconnect ();

				getBrowser () -> addEvent ();
				getBrowser () -> setCursor ("move");
				getActiveViewpoint () -> transitionStop ();
				addCollision ();

				fromVector = toVector = Vector3d (event -> x, -event -> y, 0);

				if (getBrowser () -> getBrowserOptions () -> RubberBand ())
					getBrowser () -> displayed () .addInterest (&X3DFlyViewer::display, this, MoveType::PAN);

				isActive () = true;
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
	getBrowser () -> setCursor ("default");
	removeCollision ();

	isActive () = false;
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
				if (getBrowser () -> getControlKey ())
				{
					// Look around

					const auto & viewpoint = getActiveViewpoint ();

					auto       orientation = getActiveViewpoint () -> getUserOrientation ();
					const auto toVector    = trackballProjectToSphere (event -> x, event -> y);

					orientation  = Rotation4d (toVector, fromVector) * orientation;
					orientation *= viewpoint -> straightenHorizon (orientation);

					getActiveViewpoint () -> orientationOffset () = ~getActiveViewpoint () -> getOrientation () * orientation;

					fromVector = toVector;
				}
				else
				{
					// Fly

					toVector  = Vector3d (event -> x, 0, event -> y);
					direction = toVector - fromVector;

					addFly ();
				}

				return false;
			}

			case 2:
			{
				// Pan

				toVector  = Vector3d (event -> x, -event -> y, 0);
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
			destinationRotation = sourceRotation * Rotation4d (Vector3d (-1, 0, 0) * viewpoint -> getUserOrientation (), ROLL_ANGLE);
			addRoll ();
		}

		else if (event -> direction == GDK_SCROLL_DOWN)
		{
			sourceRotation      = viewpoint -> orientationOffset ();
			destinationRotation = sourceRotation * Rotation4d (Vector3d (1, 0, 0) * viewpoint -> getUserOrientation (), ROLL_ANGLE);
			addRoll ();
		}

		scrollTime () = getCurrentTime ();
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

		const Rotation4d up (yAxis, upVector);

		const Rotation4d rubberBandRotation = direction .z () > 0
		                                      ? Rotation4d (direction * up, Vector3d (0, 0, 1) * up)
														  : Rotation4d (Vector3d (0, 0, -1) * up, direction * up);

		const double rubberBandLength = abs (direction);

		// Position offset

		double speedFactor = 1 - rubberBandRotation .angle () / pi_2 <double>;

		speedFactor *= getNavigationInfo () -> speed ();
		speedFactor *= viewpoint -> getSpeedFactor ();
		speedFactor *= getBrowser () -> getShiftKey () ? SHIFT_SPEED_FACTOR : SPEED_FACTOR;
		speedFactor *= dt;

		const auto translation = getTranslationOffset (speedFactor * direction);

		viewpoint -> positionOffset () += getActiveLayer () -> constrainTranslation (translation, true);

		// Rotation

		double weight = ROTATION_SPEED_FACTOR * dt;

		weight *= std::pow (rubberBandLength / (rubberBandLength + ROTATION_LIMIT), 2);

		viewpoint -> orientationOffset () *= slerp (Rotation4d (), rubberBandRotation, weight);

		// GeoRotation

		const auto geoRotation = Rotation4d (upVector, viewpoint -> getUpVector ());

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
		const auto   direction = constrainPanDirection (this -> direction);

		double speedFactor = 1.0;

		speedFactor *= getNavigationInfo () -> speed ();
		speedFactor *= viewpoint -> getSpeedFactor ();
		speedFactor *= getBrowser () -> getShiftKey () ? PAN_SHIFT_SPEED_FACTOR : PAN_SPEED_FACTOR;
		speedFactor *= dt;

		const auto orientation = viewpoint -> getUserOrientation () * Rotation4d (yAxis * viewpoint -> getUserOrientation (), upVector);
		const auto translation = speedFactor * direction * orientation;

		viewpoint -> positionOffset () += getActiveLayer () -> constrainTranslation (translation, true);

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

		viewpoint -> orientationOffset () = slerp <double> (sourceRotation, destinationRotation, elapsedTime / ROLL_TIME);

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
	getBrowser () -> displayed () .removeInterest (&X3DFlyViewer::display, this);

	fly_id  .disconnect ();
	pan_id  .disconnect ();
	roll_id .disconnect ();
}

void
X3DFlyViewer::display (const MoveType type)
{
	PolygonModeLock polygonMode (GL_FILL);

	// Configure HUD

	const auto & viewport = getBrowser () -> getViewport ();
	const auto & width    = viewport [2];
	const auto & height   = viewport [3];

	const auto projection = camera <double>::ortho (0, width, 0, height, -1, 1);

	std::vector <Vector2d> points;

	switch (type)
	{
		case MoveType::MOVE:
		{
			points .emplace_back (fromVector .x (), height - fromVector .z ());
			points .emplace_back (toVector   .x (), height - toVector   .z ());
			break;
		}
		case MoveType::PAN:
		{
			points .emplace_back (fromVector .x (), height + fromVector .y ());
			points .emplace_back (toVector   .x (), height + toVector   .y ());
			break;
		}
	}
	
	glMatrixMode (GL_PROJECTION);
	glLoadMatrixd (projection .data ());
	glMatrixMode (GL_MODELVIEW);

	// Draw a black and a white line.

	glDisable (GL_DEPTH_TEST);
	glLoadIdentity ();

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (2, GL_DOUBLE, 0, points .data ());

	glLineWidth (2);
	glColor3f (0, 0, 0);
	glDrawArrays (GL_LINES, 0, points .size ());

	glLineWidth (1);
	glColor3f (1, 1, 1);
	glDrawArrays (GL_LINES, 0, points .size ());

	glDisableClientState (GL_VERTEX_ARRAY);
	glEnable (GL_DEPTH_TEST);
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
