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

#include "X3DPointingDeviceSensorContext.h"

#include "../Navigation/X3DViewer.h"
#include "../PointingDeviceSensor/PointingDevice.h"

#include "../../Components/Layering/X3DLayerNode.h"
#include "../../Components/Shape/X3DShapeNode.h"

#include "../../Execution/World.h"
#include "../../Rendering/ViewVolume.h"

#include "../Selection.h"
#include "../X3DBrowser.h"

#include "PointingDeviceSensorContainer.h"

namespace titania {
namespace X3D {

static constexpr time_type SELECTION_TIME = 0.01; // Use ExamineViewer SPIN_RELEASE_TIME here.

X3DPointingDeviceSensorContext::X3DPointingDeviceSensorContext () :
	       X3DBaseNode (),
	   pointingDevice  (new PointingDevice (getBrowser ())),
	        selectable (false),
	            cursor ("ARROW"),
	     privateCursor ("DEFAULT"),
	           pointer (),
	            hitRay (),
	              hits (),
	    enabledSensors ({ PointingDeviceSensorContainerSet () }),
	       overSensors (),
	     activeSensors (),
	     selectedLayer (),
	       layerNumber (0),
	         hierarchy (),
	   buttonPressTime (0),
	buttonPressPointer (),
	     selectionType (SelectionType::DEFAULT),
	   selectionBuffer (),
	       depthBuffer ()
{
	addChildObjects (pointingDevice,
	                 selectable,
	                 cursor,
	                 privateCursor,
	                 selectedLayer);
}

void
X3DPointingDeviceSensorContext::initialize ()
{
	getBrowser () -> signal_map () .connect (sigc::mem_fun (this, &X3DPointingDeviceSensorContext::on_map));

	getCursor ()        .addInterest (&X3DPointingDeviceSensorContext::set_cursor, this);
	getPrivateCursor () .addInterest (&X3DPointingDeviceSensorContext::set_cursor, this);

	setCursor ("ARROW");

	pointingDevice -> setup ();
}

void
X3DPointingDeviceSensorContext::on_map ()
{
	set_cursor ();
}

void
X3DPointingDeviceSensorContext::set_cursor ()
{
	try
	{
		static const std::map <std::string, std::string> cursors = {
			std::pair ("ARROW",    "default"),
			std::pair ("GRAB",     "grab"),
			std::pair ("GRABBING", "grabbing"),
			std::pair ("MOVE",     "move"),
			std::pair ("LOOK_AT",  "cell"),
		};

		if (not getBrowser () -> get_mapped ())
			return;
	
		const auto string = cursors .at (getPrivateCursor () == "DEFAULT" ? getCursor () : getPrivateCursor ());

		getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), string));
	}
	catch (const std::out_of_range & error)
	{
		__LOG__ << error .what () << std::endl;
		__LOG__ << (getPrivateCursor () == "DEFAULT" ? getCursor () : getPrivateCursor ()) << std::endl;

		getBrowser () -> get_window () -> set_cursor (Gdk::Cursor::create (Gdk::Display::get_default (), "default"));
	}
}

bool
X3DPointingDeviceSensorContext::isPointerInRectangle (const Vector4i & rectangle) const
{
	return pointer .x () > rectangle .x () and
	       pointer .x () < rectangle .x () + rectangle .z () and
	       pointer .y () > rectangle .y () and
	       pointer .y () < rectangle .y () + rectangle .w ();
}

void
X3DPointingDeviceSensorContext::setHitRay (const Matrix4d & projectionMatrix, const Vector4i & viewport)
{
	try
	{
		hitRay = ViewVolume::unProjectRay (pointer, Matrix4d (), projectionMatrix, viewport);
	}
	catch (const std::domain_error &)
	{
		hitRay = Line3d (Vector3d (), Vector3d ());
	}
}

void
X3DPointingDeviceSensorContext::addHit (const Matrix4d & modelMatrix,
                                        const IntersectionPtr & intersection,
                                        X3DShapeNode* const shape,
                                        X3DLayerNode* const layer,
                                        const double depthOffset)
{
	// Or do this in Selection

	if (shape -> getPrivate ())
		return;

	auto hierarchy = getHierarchy ();

	hierarchy .erase (std::remove_if (hierarchy .begin (),
	                                  hierarchy .end (),
	                                  [ ] (X3DBaseNode* const node)
	                                  { return node -> getPrivate (); }),
	                  hierarchy .end ());

	//

	hits .emplace_back (new Hit (pointer,
	                             modelMatrix,
	                             hitRay,
	                             intersection,
	                             enabledSensors .back (),
	                             X3DPtr <X3DShapeNode> (shape),
	                             X3DPtr <X3DLayerNode> (layer),
	                             layerNumber,
	                             depthOffset,
	                             MFNode (hierarchy .cbegin (), hierarchy .cend ())));
}

bool
X3DPointingDeviceSensorContext::setButtonPressEvent (const double x, const double y)
{
	buttonPressTime    = SFTime::now ();
	buttonPressPointer = Vector2d (x, y);

	touch (x, y);

	if (getHits () .empty ())
		return false;

	if (getNearestHit () -> getSensors () .empty ())
		return false;

	selectedLayer = getNearestHit () -> getLayer ();

	activeSensors .assign (getNearestHit () -> getSensors () .cbegin (),
	                       getNearestHit () -> getSensors () .cend ());

	for (const auto & pointingDeviceSensorNode : activeSensors)
		pointingDeviceSensorNode -> set_active (true, getNearestHit ());

	return true;
}

bool
X3DPointingDeviceSensorContext::setButtonReleaseEvent (const double x, const double y)
{
	// Selection
	pointer = Vector2d (x, y);

	if (selectable)
	{
		// TODO: Sometimes there is no selection.
		if (distance (buttonPressPointer, pointer) < 1 or distance (buttonPressTime, SFTime::now ()) < SELECTION_TIME)
		{
			if (getBrowser () -> getSelection () -> selectNode (getBrowser ()))
				return true;
		}
	}

	const auto nearestHit = getHits () .empty ()
	                        ? std::make_shared <Hit> (pointer, hitRay, selectedLayer)
	                        : getNearestHit ();

	selectedLayer = nullptr;

	for (const auto & pointingDeviceSensorNode : activeSensors)
		pointingDeviceSensorNode -> set_active (false, nearestHit);

	activeSensors .clear ();
	return true;
}

void
X3DPointingDeviceSensorContext::setLeaveNotifyEvent (const double x, const double y)
{
	// Clear hits.

	hits .clear ();

	motion ();
}

bool
X3DPointingDeviceSensorContext::setMotionNotifyEvent (const double x, const double y)
{
	touch (x, y);

	motion ();

	return not getHits () .empty () and not getNearestHit () -> getSensors () .empty ();
}

void
X3DPointingDeviceSensorContext::motion ()
{
	const auto hitRay = selectedLayer ? this -> hitRay : Line3d (Vector3d (), Vector3d ());

	const auto nearestHit = getHits () .empty ()
	                        ? std::make_shared <Hit> (pointer, hitRay, selectedLayer)
	                        : getNearestHit ();

	// Set isOver to FALSE for appropriate nodes

	std::vector <PointingDeviceSensorContainerPtr> difference;

	if (getHits () .empty ())
	{
		difference .assign (overSensors .cbegin (), overSensors .cend ());
	}
	else
	{
		// overSensors and sensors are always sorted.

		std::set_difference (overSensors .cbegin (), overSensors .cend (),
		                     nearestHit -> getSensors () .cbegin (), nearestHit -> getSensors () .cend (),
		                     std::back_inserter (difference),
									[ ] (const PointingDeviceSensorContainerPtr & lhs, const PointingDeviceSensorContainerPtr & rhs)
		{
			return lhs -> getNode () < rhs -> getNode ();
		});
	}

	for (const auto & pointingDeviceSensorNode : difference)
		pointingDeviceSensorNode -> set_over (false, nearestHit);

	// Set isOver to TRUE for appropriate nodes

	if (getHits () .empty ())
	{
		overSensors .clear ();
	}
	else
	{
		overSensors .assign (nearestHit -> getSensors () .begin (),
		                     nearestHit -> getSensors () .end ());

		for (const auto & pointingDeviceSensorNode : overSensors)
			pointingDeviceSensorNode -> set_over (true, nearestHit);
	}

	// Forward motion event to active drag sensor nodes

	for (const auto & pointingDeviceSensorNode : activeSensors)
		pointingDeviceSensorNode -> set_motion (nearestHit);
}

void
X3DPointingDeviceSensorContext::touch (const double x, const double y)
{
	if (getBrowser () -> getViewer () -> isActive ())
		return;

	pointer = Vector2d (x, y);

	// Clear hits.

	hits .clear ();

	// Pick.

	try
	{
		ContextLock lock (getBrowser ());

		//update (); // We cannot make an update here because of gravity.

		getBrowser () -> getWorld () -> traverse (TraverseType::POINTER, nullptr);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	// Picking end.

	std::stable_sort (hits .begin (), hits .end (), HitComp { });

	enabledSensors = { PointingDeviceSensorContainerSet () };
}

void
X3DPointingDeviceSensorContext::dispose ()
{
	hits           .clear ();
	enabledSensors .clear ();
	overSensors    .clear ();
	activeSensors  .clear ();
}

X3DPointingDeviceSensorContext::~X3DPointingDeviceSensorContext ()
{ }

} // X3D
} // titania