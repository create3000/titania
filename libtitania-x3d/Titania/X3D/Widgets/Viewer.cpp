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
 ******************************************************************************/

#include "Viewer.h"

#include "Surface.h"
#include <cmath>
#include <glibmm/main.h>

#define TIMEOUT_INTERVAL 10
#define SPIN_ANGLE 0.006

namespace titania {
namespace X3D {

Viewer::Viewer (Surface & surface) :
	surface (surface), 
	 button (0)        
{ }

const SFNode <X3DBrowser> &
Viewer::getBrowser ()
{
	return surface .getBrowser ();
}

void
Viewer::initialize ()
{
	surface .signal_button_press_event   () .connect (sigc::mem_fun (*this, &Viewer::on_button_press_event));
	surface .signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &Viewer::on_motion_notify_event));
	surface .signal_button_release_event () .connect (sigc::mem_fun (*this, &Viewer::on_button_release_event));
	surface .signal_scroll_event         () .connect (sigc::mem_fun (*this, &Viewer::on_scroll_event));

	getBrowser () -> initialized .addInterest (this, &Viewer::set_scene);

	set_scene ();
}

void
Viewer::set_scene ()
{
	timeout_remove ();
}

void
Viewer::set_viewpoint ()
{
	SFNode <Viewpoint> viewpoint = getBrowser () -> getActiveViewpoint ();

	if (viewpoint)
	{
		// Save distance and orientation.
		distance    = viewpoint -> position - viewpoint -> centerOfRotation;
		orientation = viewpoint -> orientation;

		//		// Assign current viewpoint to default viewpoint.
		//		defaultViewpoint -> setTransformationMatrix (viewpoint -> getTransformationMatrix ());
		//
		//		defaultViewpoint -> position         = viewpoint -> position;
		//		defaultViewpoint -> orientation      = viewpoint -> orientation;
		//		defaultViewpoint -> centerOfRotation = viewpoint -> centerOfRotation;
		//		defaultViewpoint -> fieldOfView      = viewpoint -> fieldOfView;
		//
		//		// Bind viewpoint.
		//		defaultViewpoint -> set_bind = true;
	}
}

bool
Viewer::on_button_press_event (GdkEventButton* event)
{
	timeout_remove ();

	button = event -> button;

	if (button == 1)
	{
		fromVector .x (event -> x / surface .get_width () - 0.5f);
		fromVector .y (-event -> y / surface .get_height () + 0.5f);
		fromVector .z (tb_project_to_sphere (0.5f, fromVector .x (), fromVector .y ()));

		deltaRotation = Rotation4f ();

		set_viewpoint ();

		return false;
	}

	if (button == 2)
	{
		lastTranslation = Vector3f (event -> x, -event -> y, 0);

		set_viewpoint ();

		return false;
	}

	return false;
}

bool
Viewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 1)
	{
		SFNode <Viewpoint> viewpoint = getBrowser () -> getActiveViewpoint ();

		if (viewpoint)
		{
			Vector3f toVector;
			toVector .x (event -> x / surface .get_width () - 0.5f);
			toVector .y (-event -> y / surface .get_height () + 0.5f);
			toVector .z (tb_project_to_sphere (0.5, toVector .x (), toVector .y ()));

			deltaRotation = ~Rotation4f (fromVector, toVector);

			viewpoint -> orientation = deltaRotation * viewpoint -> orientation;
			viewpoint -> position    = viewpoint -> centerOfRotation + ~orientation * viewpoint -> orientation * distance;

			fromVector = toVector;
		}
	}

	else if (button == 2)
	{
		SFNode <Viewpoint> viewpoint = getBrowser () -> getActiveViewpoint ();

		if (viewpoint)
		{
			float step = abs (distance) / 300;

			Vector3f translation (event -> x, -event -> y, 0);

			Vector3f vector = translation - lastTranslation;
			vector = normalize (vector) * -step;

			vector = viewpoint -> orientation * vector;

			viewpoint -> position = viewpoint -> position + vector;

			lastTranslation = translation;
		}
	}

	return false;
}

bool
Viewer::on_button_release_event (GdkEventButton* event)
{
	if (button == 1)
	{
		float angle = deltaRotation .angle ();

		if (angle > SPIN_ANGLE)
		{
			deltaRotation .angle (angle - SPIN_ANGLE);
			timeout_add ();
		}
	}

	button = 0;

	return false;
}

bool
Viewer::on_scroll_event (GdkEventScroll* event)
{
	SFNode <Viewpoint> viewpoint = getBrowser () -> getActiveViewpoint ();

	if (viewpoint)
	{
		float step = abs (distance) / 50;

		Vector3f vector = viewpoint -> orientation * Vector3f (0, 0, step);

		if (event -> direction == 0)
		{
			viewpoint -> position = viewpoint -> position + vector;
		}

		else if (event -> direction == 1)
		{
			viewpoint -> position = viewpoint -> position - vector;
		}

		distance    = viewpoint -> position - viewpoint -> centerOfRotation;
		orientation = viewpoint -> orientation;
	}

	return false;
}

bool
Viewer::timeout ()
{
	SFNode <Viewpoint> viewpoint = getBrowser () -> getActiveViewpoint ();

	if (viewpoint)
	{
		viewpoint -> orientation = deltaRotation * viewpoint -> orientation;
		viewpoint -> position    = viewpoint -> centerOfRotation + ~orientation * viewpoint -> orientation * distance;
	}

	return true;
}

void
Viewer::timeout_add ()
{
	if (not timeout_id .connected ())
		timeout_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &Viewer::timeout), TIMEOUT_INTERVAL, GDK_PRIORITY_REDRAW);
}

void
Viewer::timeout_remove ()
{
	timeout_id .disconnect ();
}

float
Viewer::tb_project_to_sphere (const float r, const float x, const float y) const
{
	float z = 0;

	float d = std::sqrt (x * x + y * y);

	if (d < r * std::sqrt (0.5)) // Inside sphere
	{
		z = std::sqrt (r * r - d * d);
	}
	else                         // On hyperbola
	{
		float t = r / std::sqrt (2);
		z = t * t / d;
	}

	return z;
}

Viewer::~Viewer ()
{
	timeout_remove ();
}

} // X3D
} // titania
