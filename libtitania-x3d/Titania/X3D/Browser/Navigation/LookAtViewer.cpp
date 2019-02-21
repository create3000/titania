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

#include "LookAtViewer.h"

#include "../../Browser/PointingDeviceSensor/PointingDevice.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Navigation/X3DViewpointNode.h"

namespace titania {
namespace X3D {

const Component   LookAtViewer::component      = Component ("Titania", 1);
const std::string LookAtViewer::typeName       = "LookAtViewer";
const std::string LookAtViewer::containerField = "viewer";

LookAtViewer::LookAtViewer (X3DExecutionContext* const executionContext) :
	X3DBaseNode (executionContext -> getBrowser (), executionContext),
	  X3DViewer (),
	  touchable (executionContext -> getBrowser () -> getPointingDevice () -> getEnabled ()),
	     isOver (false),
	orientation (),
	   rotation (),
	 fromVector (),
	     button (0),
	     motion (false)
{
	addType (X3DConstants::LookAtViewer);

	addField (outputOnly, "isActive",   isActive ());
	addField (outputOnly, "scrollTime", scrollTime ());
}

X3DBaseNode*
LookAtViewer::create (X3DExecutionContext* const executionContext) const
{
	return new LookAtViewer (executionContext);
}

void
LookAtViewer::initialize ()
{
	X3DViewer::initialize ();

	getBrowser () -> signal_button_press_event ()   .connect (sigc::mem_fun (this, &LookAtViewer::on_button_press_event),   false);
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (this, &LookAtViewer::on_button_release_event), false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (this, &LookAtViewer::on_motion_notify_event),  false);

	getBrowser () -> getPointingDevice () -> setEnabled (false);
}

bool
LookAtViewer::on_button_press_event (GdkEventButton* event)
{
	try
	{
		if (button)
			return false;

		if (event -> button == 1)
		{
			button = event -> button;

			motion = false;

			getBrowser () -> grab_focus ();
			getActiveViewpoint () -> transitionStop ();

			orientation = getActiveViewpoint () -> getUserOrientation ();
			fromVector  = trackballProjectToSphere (event -> x, event -> y);

			isActive () = true;
		}
	}
	catch (const X3DError &)
	{ }

	return true;
}

bool
LookAtViewer::on_button_release_event (GdkEventButton* event)
{
	try
	{
		if (not button)
			return false;

		button = 0;

		if (not motion)
			lookAt (event -> x, event -> y, false, true);

		isActive () = false;
	}
	catch (const X3DError &)
	{ }

	return true;
}

bool
LookAtViewer::on_motion_notify_event (GdkEventMotion* event)
{
	try
	{
		if (touch (event -> x, event -> y))
		{
			if (not isOver)
			{
				getBrowser () -> setCursor ("LOOK_AT");
				isOver = true;
			}
		}
		else
		{
			if (isOver)
			{
				getBrowser () -> setCursor ("ARROW");
				isOver = false;
			}
		}

		if (button == 1)
		{
			motion = true;

			const auto & viewpoint = getActiveViewpoint ();
			const auto   toVector  = trackballProjectToSphere (event -> x, event -> y);

			rotation = Rotation4d (toVector, fromVector);

			viewpoint -> orientationOffset () = getOrientationOffset ();

			fromVector = toVector;
		}
	}
	catch (const X3DError &)
	{ }

	return true;
}

Rotation4d
LookAtViewer::getOrientationOffset ()
{
	try
	{
		const auto & viewpoint = getActiveViewpoint ();

		orientation = rotation * orientation;

		return ~viewpoint -> getOrientation () * orientation;
	}
	catch (const X3DError &)
	{
	   return Rotation4d ();
	}
}

void
LookAtViewer::dispose ()
{
	getBrowser () -> getPointingDevice () -> setEnabled (touchable);

	X3DViewer::dispose ();
}

LookAtViewer::~LookAtViewer ()
{ }

} // X3D
} // titania
