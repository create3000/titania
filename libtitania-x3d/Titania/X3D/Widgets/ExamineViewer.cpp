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

#include "ExamineViewer.h"

#include "../Components/Geospatial/GeoViewpoint.h"
#include "../Components/Navigation/OrthoViewpoint.h"

#include "Surface.h"
#include <cmath>
#include <glibmm/main.h>

#define TIMEOUT_INTERVAL 10
#define SPIN_ANGLE       0.006
#define SCOLL_FACTOR     (1.0f / 50.0f)

namespace titania {
namespace X3D {

ExamineViewer::ExamineViewer (Surface & surface) :
	                      X3DViewer (surface .getBrowser ()), 
	                        surface (surface),                
	                       distance (),                       
	                    orientation (),                       
	                       rotation (),                       
	                     fromVector (),                       
	                      fromPoint (),                       
	                         button (0),                      
	button_press_event_connection   (),                       
	motion_notify_event_connection  (),                       
	button_release_event_connection (),                       
	scroll_event_connection         (),                       
	                        spin_id (),                       
	                    activeLayer ()                        
{ }

void
ExamineViewer::initialize ()
{
	button_press_event_connection   = surface .signal_button_press_event   () .connect (sigc::mem_fun (*this, &ExamineViewer::on_button_press_event));
	motion_notify_event_connection  = surface .signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &ExamineViewer::on_motion_notify_event));
	button_release_event_connection = surface .signal_button_release_event () .connect (sigc::mem_fun (*this, &ExamineViewer::on_button_release_event));
	scroll_event_connection         = surface .signal_scroll_event         () .connect (sigc::mem_fun (*this, &ExamineViewer::on_scroll_event));

	getBrowser () -> initialized .addInterest (this, &ExamineViewer::set_scene);

	set_scene ();
}

void
ExamineViewer::set_scene ()
{
	spin_id .disconnect ();

	getBrowser () -> getExecutionContext () -> getLayerSet () -> activeLayer .addInterest (this, &ExamineViewer::set_activeLayer);

	set_activeLayer ();
}

void
ExamineViewer::set_activeLayer ()
{
	if (activeLayer)
		activeLayer -> viewpointStack .removeInterest (this, &ExamineViewer::set_viewpoint);

	activeLayer = getBrowser () -> getExecutionContext () -> getLayerSet () -> getActiveLayer ();

	activeLayer -> viewpointStack .addInterest (this, &ExamineViewer::set_viewpoint);

	set_viewpoint ();
}

void
ExamineViewer::set_viewpoint ()
{
	// Update distance and rotation.

	Viewpoint* viewpoint = dynamic_cast <Viewpoint*> (getBrowser () -> getActiveViewpoint ());

	if (viewpoint)
	{
		//if (reset)
		//{
		//	viewpoint -> translation = Vector3f ();
		//	viewpoint -> rotation    = Rotation4f ();
		//
		//	orientation = viewpoint -> orientation;
		//	distance    = viewpoint -> position - viewpoint -> centerOfRotation - viewpoint -> center;
		//}

		orientation = getOrientation (viewpoint);
		distance    = getDistance (viewpoint);
	}
}

bool
ExamineViewer::on_button_press_event (GdkEventButton* event)
{
	spin_id .disconnect ();

	button = event -> button;

	if (button == 1)
	{
		fromVector = trackballProjectToSphere (event -> x, event -> y);

		rotation = Rotation4f ();

		return false;
	}

	if (button == 2)
	{
		fromPoint = getPoint (event -> x, event -> y);

		return false;
	}

	return false;
}

bool
ExamineViewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (button == 1)
	{
		Viewpoint* viewpoint = dynamic_cast <Viewpoint*> (getBrowser () -> getActiveViewpoint ());

		if (viewpoint)
		{
			Vector3f toVector = trackballProjectToSphere (event -> x, event -> y);

			rotation = ~Rotation4f (fromVector, toVector);

			viewpoint -> rotation    = getRotation (viewpoint);
			viewpoint -> translation = getTranslation (viewpoint);

			fromVector = toVector;
		}
	}

	else if (button == 2)
	{
		Viewpoint* viewpoint = dynamic_cast <Viewpoint*> (getBrowser () -> getActiveViewpoint ());

		if (viewpoint)
		{
			Vector3f toPoint = getPoint (event -> x, event -> y);

			Vector3f translation = toPoint - fromPoint;
			translation = viewpoint -> orientation * viewpoint -> rotation * translation;

			viewpoint -> translation += translation;
			viewpoint -> center      += translation;

			fromPoint = toPoint;
		}
	}

	return false;
}

bool
ExamineViewer::on_button_release_event (GdkEventButton* event)
{
	if (button == 1)
	{
		float angle = rotation .angle ();

		if (angle > SPIN_ANGLE)
		{
			rotation .angle (angle - SPIN_ANGLE);
			addSpinning ();
		}
	}

	button = 0;

	return false;
}

bool
ExamineViewer::on_scroll_event (GdkEventScroll* event)
{
	Viewpoint* viewpoint = dynamic_cast <Viewpoint*> (getBrowser () -> getActiveViewpoint ());

	if (viewpoint)
	{
		Vector3f step        = distance * SCOLL_FACTOR;
		Vector3f translation = viewpoint -> orientation * viewpoint -> rotation * Vector3f (0, 0, abs (step));

		if (event -> direction == 0)      // Move backwards.
		{
			viewpoint -> translation += translation;
			distance                 += step;
		}

		else if (event -> direction == 1) // Move forwards.
		{
			viewpoint -> translation -= translation;
			distance                 -= step;
		}

		distance = getDistance (viewpoint);
	}

	return false;
}

bool
ExamineViewer::spin ()
{
	Viewpoint* viewpoint = dynamic_cast <Viewpoint*> (getBrowser () -> getActiveViewpoint ());

	if (viewpoint)
	{
		viewpoint -> rotation    = getRotation (viewpoint);
		viewpoint -> translation = getTranslation (viewpoint);
	}

	return true;
}

void
ExamineViewer::addSpinning ()
{
	if (not spin_id .connected ())
		spin_id = Glib::signal_timeout () .connect (sigc::mem_fun (*this, &ExamineViewer::spin), TIMEOUT_INTERVAL, GDK_PRIORITY_REDRAW);
}

Vector3f
ExamineViewer::getDistance (const SFNode <Viewpoint> & viewpoint) const
{
	return ~viewpoint -> rotation * (viewpoint -> position + viewpoint -> translation - viewpoint -> centerOfRotation - viewpoint -> center);
}

Rotation4f
ExamineViewer::getOrientation (const SFNode <Viewpoint> & viewpoint) const
{
	return viewpoint -> orientation * viewpoint -> rotation;
}

Vector3f
ExamineViewer::getTranslation (const SFNode <Viewpoint> & viewpoint) const
{
	// The new translation is calculated here by calculating the new position and
	// then subtracting the viewpoints position to get the new translation.

	return viewpoint -> centerOfRotation + viewpoint -> center
	       + viewpoint -> rotation * distance
	       - viewpoint -> position;
}

Rotation4f
ExamineViewer::getRotation (const SFNode <Viewpoint> & viewpoint)
{
	orientation = rotation * orientation;
	return ~viewpoint -> orientation * orientation;
}

Vector3f
ExamineViewer::getPoint (double x, double y)
{
	X3DViewpointNode* viewpoint      = getBrowser () -> getActiveViewpoint ();
	NavigationInfo*   navigationInfo = getBrowser () -> getActiveNavigationInfo ();

	GLint                viewport [4]; // x, y, width, heigth
	Matrix4d             modelview;
	Matrix4d::array_type projection;

	viewpoint -> reshape (navigationInfo -> getZNear (), navigationInfo -> getZFar ());
	glGetDoublev (GL_PROJECTION_MATRIX, projection);

	glGetIntegerv (GL_VIEWPORT, viewport);
	GLdouble px, py, pz;

	// Far plane point
	gluUnProject (x, y, 1, modelview .data (), projection, viewport, &px, &py, &pz);
	Vector3f direction = normalize (Vector3f (-px, py, pz));

	return direction * abs (distance) / dot (direction, Vector3f (0, 0, -1));
}

Vector3f
ExamineViewer::trackballProjectToSphere (double x, double y) const
{
	return Vector3f (x / surface .get_width () - 0.5f,
	                 -y / surface .get_height () + 0.5f,
	                 tb_project_to_sphere (0.5, 0, 0));
}

float
ExamineViewer::tb_project_to_sphere (const float r, const float x, const float y) const
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

ExamineViewer::~ExamineViewer ()
{
	button_press_event_connection   .disconnect ();
	motion_notify_event_connection  .disconnect ();
	button_release_event_connection .disconnect ();
	scroll_event_connection         .disconnect ();
	spin_id                         .disconnect ();
}

} // X3D
} // titania
