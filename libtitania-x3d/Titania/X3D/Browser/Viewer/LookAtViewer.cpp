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

#include "../X3DBrowserSurface.h"

#include "LookAtViewer.h"

namespace titania {
namespace X3D {

LookAtViewer::LookAtViewer (X3DBrowserSurface* const browser) :
	X3DViewer (browser),
	  picking (browser -> getPicking ()),
	   isOver (false)
{ }

void
LookAtViewer::initialize ()
{
	X3DViewer::initialize ();

	getBrowser () -> signal_button_press_event ()   .connect (sigc::mem_fun (*this, &LookAtViewer::on_button_press_event),   false);
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (*this, &LookAtViewer::on_button_release_event), false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (*this, &LookAtViewer::on_motion_notify_event),  false);

	getBrowser () -> getActiveViewpointChanged () .addInterest (this, &LookAtViewer::set_viewpoint);

	getBrowser () -> setPicking (false);
}

void
LookAtViewer::set_viewpoint ()
{
	// Update orientationOffset.

	auto viewpoint = getActiveViewpoint ();

	orientation = viewpoint -> getUserOrientation ();
}

bool
LookAtViewer::on_button_press_event (GdkEventButton* event)
{
	button = event -> button;
	motion = false;

	getBrowser () -> grab_focus ();

	if (event -> button == 1)
	{
		getActiveViewpoint () -> transitionStop ();

		set_viewpoint ();

		fromVector = trackballProjectToSphere (event -> x, event -> y);
		rotation   = Rotation4f ();
	}

	return true;
}

bool
LookAtViewer::on_button_release_event (GdkEventButton* event)
{
	if (event -> button == 1)
	{
		if (not motion and pick (event -> x, event -> y))
		{
			auto hit  = getBrowser () -> getHits () .front ();
			auto bbox = hit -> shape -> getBBox () * Matrix4f (hit -> modelViewMatrix) * getActiveViewpoint () -> getTransformationMatrix ();

			getActiveViewpoint () -> lookAt (bbox, 1.0 / 3.0);
		}
	}

	button = 0;

	return true;
}

bool
LookAtViewer::on_motion_notify_event (GdkEventMotion* event)
{
	if (pick (event -> x, event -> y))
	{
		if (not isOver)
		{
			getBrowser () -> setCursor (Gdk::TARGET);
			isOver = true;
		}
	}
	else
	{
		if (isOver)
		{
			getBrowser () -> setCursor (Gdk::ARROW);
			isOver = false;
		}
	}

	if (button == 1)
	{
		motion = true;

		const auto & viewpoint = getActiveViewpoint ();

		Vector3f toVector = trackballProjectToSphere (event -> x, event -> y);

		rotation = ~Rotation4f (fromVector, toVector);

		viewpoint -> orientationOffset () = getOrientationOffset ();

		fromVector = toVector;
	}

	return true;
}

Rotation4f
LookAtViewer::getOrientationOffset ()
{
	auto viewpoint = getActiveViewpoint ();

	orientation = rotation * orientation;

	return ~viewpoint -> orientation () * orientation;
}

bool
LookAtViewer::pick (const double x, const double y)
{
	getBrowser () -> pick (x, getBrowser () -> get_height () - y);

	return not getBrowser () -> getHits () .empty ();
}

LookAtViewer::~LookAtViewer ()
{
	getBrowser () -> setPicking (picking);
}

} // X3D
} // titania
