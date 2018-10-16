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

#include "../Browser.h"

#include "PlaneViewer.h"

#include "../../Components/Geospatial/GeoViewpoint.h"
#include "../../Components/Navigation/OrthoViewpoint.h"
#include "../../Components/Navigation/Viewpoint.h"

namespace titania {
namespace X3D {

static constexpr double SCROLL_FACTOR = 0.05;

const ComponentType PlaneViewer::component      = ComponentType::TITANIA;
const std::string   PlaneViewer::typeName       = "PlaneViewer";
const std::string   PlaneViewer::containerField = "viewer";

PlaneViewer::PlaneViewer (X3DExecutionContext* const executionContext, const NodeType type) :
	   X3DBaseNode (executionContext -> getBrowser (), executionContext),
	     X3DViewer (),
	     fromPoint (),
	        button (0)
{
	addType (type == X3DConstants::PlaneViewer3D ? X3DConstants::PlaneViewer3D : X3DConstants::PlaneViewer);

	addField (outputOnly, "isActive",   isActive ());
	addField (outputOnly, "scrollTime", scrollTime ());
}

X3DBaseNode*
PlaneViewer::create (X3DExecutionContext* const executionContext) const
{
	return new PlaneViewer (executionContext, getType () .back ());
}

void
PlaneViewer::initialize ()
{
	X3DViewer::initialize ();

	getBrowser () -> signal_button_press_event   () .connect (sigc::mem_fun (this, &PlaneViewer::on_button_press_event));
	getBrowser () -> signal_button_release_event () .connect (sigc::mem_fun (this, &PlaneViewer::on_button_release_event), false);
	getBrowser () -> signal_motion_notify_event  () .connect (sigc::mem_fun (this, &PlaneViewer::on_motion_notify_event),  false);
	getBrowser () -> signal_scroll_event         () .connect (sigc::mem_fun (this, &PlaneViewer::on_scroll_event));
}

bool
PlaneViewer::on_button_press_event (GdkEventButton* event)
{
	try
	{
		if (button)
			return false;

		if (event -> button == 2)
		{
			button = event -> button;

			getBrowser () -> setCursor ("MOVE");
			getActiveViewpoint () -> transitionStop ();

			fromPoint = getPointOnCenterPlane (event -> x, event -> y);

			isActive () = true;
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
PlaneViewer::on_button_release_event (GdkEventButton* event)
{
	button = 0;
	getBrowser () -> setCursor ("ARROW");

	isActive () = false;
	return false;
}

bool
PlaneViewer::on_motion_notify_event (GdkEventMotion* event)
{
	try
	{
		if (button == 2)
		{
			const auto & viewpoint = getActiveViewpoint ();

			const auto toPoint     = getPointOnCenterPlane (event -> x, event -> y);
			const auto translation = (fromPoint - toPoint) * viewpoint -> getUserOrientation ();

			viewpoint -> positionOffset ()         += translation;
			viewpoint -> centerOfRotationOffset () += translation;

			fromPoint = toPoint;
	
			return false;
		}
	}
	catch (const X3DError &)
	{ }

	return false;
}

bool
PlaneViewer::on_scroll_event (GdkEventScroll* event)
{
	try
	{
		const auto & viewpoint      = getActiveViewpoint ();
		const auto   step           = getDistanceToCenter () * SCROLL_FACTOR;
		const auto   positionOffset = Vector3d (0, 0, abs (step)) * viewpoint -> getUserOrientation ();
		const auto   fromPoint      = getPointOnCenterPlane (event -> x, event -> y);

		viewpoint -> transitionStop ();

		if (getType () .back () == X3DConstants::PlaneViewer3D)
		{
			if (getBrowser () -> getShiftKey () and not getBrowser () -> getControlKey ())
			{
				if (event -> direction == GDK_SCROLL_UP) // Move backwards.
				{
					viewpoint -> positionOffset () -= positionOffset;
				}
				else if (event -> direction == GDK_SCROLL_DOWN) // Move forwards.
				{
					viewpoint -> positionOffset () += positionOffset;
				}
			}
		}

		if (event -> direction == GDK_SCROLL_UP) // Move backwards.
		{
			viewpoint -> fieldOfViewScale () = std::max (0.00001, viewpoint -> fieldOfViewScale () * (1 - SCROLL_FACTOR));
		}
		else if (event -> direction == GDK_SCROLL_DOWN) // Move forwards.
		{
			viewpoint -> fieldOfViewScale () = viewpoint -> fieldOfViewScale () * (1 + SCROLL_FACTOR);

			constrainFieldOfViewScale ();
		}

		const auto toPoint     = getPointOnCenterPlane (event -> x, event -> y);
		const auto translation = (fromPoint - toPoint) * viewpoint -> getUserOrientation ();

		viewpoint -> positionOffset ()         += translation;
		viewpoint -> centerOfRotationOffset () += translation;

		scrollTime () = getCurrentTime ();
	}
	catch (const X3DError &)
	{ }

	return false;
}

void
PlaneViewer::constrainFieldOfViewScale () const
{
	try
	{
		const auto & viewpointNode = getActiveViewpoint ();
		const auto   viewpoint     = dynamic_cast <Viewpoint*> (viewpointNode);

		if (viewpoint)
		{
			if (viewpoint -> fieldOfView () * viewpoint -> fieldOfViewScale () >= pi <double>)
				viewpoint -> fieldOfViewScale () = (pi <double> - 0.001) / viewpoint -> fieldOfView ();
		}
		else
		{
			const auto geoViewpoint = dynamic_cast <GeoViewpoint*> (viewpointNode);

			if (geoViewpoint)
			{
				if (geoViewpoint -> fieldOfView () * geoViewpoint -> fieldOfViewScale () >= pi <double>)
					geoViewpoint -> fieldOfViewScale () = (pi <double> - 0.001) / geoViewpoint -> fieldOfView ();
			}
		}
	}
	catch (const X3DError &)
	{ }
}

PlaneViewer::~PlaneViewer ()
{ }

} // X3D
} // titania
