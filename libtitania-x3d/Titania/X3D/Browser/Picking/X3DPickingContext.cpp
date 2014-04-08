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

#include "X3DPickingContext.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Components/Networking/Anchor.h"
#include "../../Components/PointingDeviceSensor/X3DDragSensorNode.h"
#include "../../Components/PointingDeviceSensor/X3DTouchSensorNode.h"
#include "../../Context.h"
#include "../../Execution/World.h"
#include "../../Rendering/ViewVolume.h"

namespace titania {
namespace X3D {

X3DPickingContext::X3DPickingContext () :
	   X3DBaseNode (),
	  pickedOutput (),
	       picking (true),
	             x (0),
	             y (0),
	       pickRay (),
	          hits (),
	       hitComp (),
	enabledSensors ({ NodeSet () }),
	   overSensors (),
	 activeSensors (),
	  pickingLayer (nullptr)
{
	addChildren (picking,
	             overSensors,
	             activeSensors);
}

void
X3DPickingContext::initialize ()
{
	getBrowser () -> shutdown () .addInterest (this, &X3DPickingContext::set_shutdown);
}

void
X3DPickingContext::set_shutdown ()
{
	hits .clear ();

	overSensors   .clear ();
	activeSensors .clear ();
}

void
X3DPickingContext::pick (const double _x, const double _y)
{
	x = _x;
	y = _y;

	// Clear hits.

	hits .clear ();

	// Pick.

	//update (); // We cannot make an update here because of gravity.

	getWorld () -> traverse (TraverseType::PICKING);

	picked () .processInterests ();

	// Selection end.

	std::stable_sort (hits .begin (), hits .end (), hitComp);

	enabledSensors = { NodeSet () };
}

bool
X3DPickingContext::intersect (const Vector4i & scissor) const
{
	return x > scissor .x () and x <scissor .x () + scissor .z () and
	                                y> scissor .y () and y < scissor .y () + scissor .w ();
}

Line3d
X3DPickingContext::getPickRay (const Matrix4d & modelViewMatrix, const Matrix4d & projectionMatrix, const Vector4i & viewport) const
{
	try
	{
		return ViewVolume::unProjectLine (x, y, modelViewMatrix, projectionMatrix, viewport);
	}
	catch (const std::domain_error &)
	{
		return Line3d (Vector3d (), Vector3d ());
	}
}

void
X3DPickingContext::addHit (const Matrix4d & transformationMatrix, const IntersectionPtr & intersection, X3DShapeNode* const shape, X3DLayerNode* const layer)
{
	hits .emplace_front (new Hit (x, y, transformationMatrix, pickRay, intersection, enabledSensors .back (), shape, layer));
}

void
X3DPickingContext::motionNotifyEvent ()
{
	// Set isOver to FALSE for appropriate nodes

	std::vector <X3DBaseNode*> difference;

	if (getHits () .empty ())
		difference .assign (overSensors .begin (), overSensors .end ());

	else
	{
		// overSensors and sensors are always sorted.

		std::set_difference (overSensors .begin (), overSensors .end (),
		                     getHits () .front () -> sensors .begin (), getHits () .front () -> sensors .end (),
		                     std::back_inserter (difference));
	}

	for (const auto & node : difference)
	{
		const auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node);

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_over (getHits () .front (), false);

		else
		{
			const auto anchor = dynamic_cast <Anchor*> (node);

			if (anchor)
				anchor -> set_over (false);
		}
	}

	// Set isOver to TRUE for appropriate nodes

	if (getHits () .empty ())
		overSensors .clear ();

	else
	{
		overSensors .assign (getHits () .front () -> sensors .begin (),
		                     getHits () .front () -> sensors .end ());

		for (const auto & node : overSensors)
		{
			const auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

			if (pointingDeviceSensorNode)
				pointingDeviceSensorNode -> set_over (getHits () .front (), true);

			else
			{
				const auto anchor = dynamic_cast <Anchor*> (node .getValue ());

				if (anchor)
					anchor -> set_over (true);
			}
		}
	}

	// Forward motion event to active drag sensor nodes

	for (const auto & node : activeSensors)
	{
		const auto dragSensorNode = dynamic_cast <X3DDragSensorNode*> (node .getValue ());

		if (dragSensorNode)
		{
			dragSensorNode -> set_motion (getHits () .empty ()
			                              ? std::make_shared <Hit> (x, y, Matrix4d (), pickRay, std::make_shared <Intersection> (), NodeSet (), nullptr, nullptr)
													: getHits () .front ());
		}
	}
}

void
X3DPickingContext::buttonPressEvent ()
{
	pickingLayer = getHits () .front () -> layer;

	activeSensors .assign (getHits () .front () -> sensors .begin (),
	                       getHits () .front () -> sensors .end ());

	for (const auto & node : activeSensors)
	{
		const auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_active (getHits () .front (), true);

		else
		{
			const auto anchor = dynamic_cast <Anchor*> (node .getValue ());

			if (anchor)
				anchor -> set_active (true);
		}
	}
}

void
X3DPickingContext::buttonReleaseEvent ()
{
	pickingLayer = nullptr;

	for (const auto & node : activeSensors)
	{
		const auto pointingDeviceSensorNode = dynamic_cast <X3DPointingDeviceSensorNode*> (node .getValue ());

		if (pointingDeviceSensorNode)
			pointingDeviceSensorNode -> set_active (std::make_shared <Hit> (x, y, Matrix4d (), pickRay, std::make_shared <Intersection> (), NodeSet (), nullptr, nullptr),
			                                        false);

		else
		{
			const auto anchor = dynamic_cast <Anchor*> (node .getValue ());

			if (anchor)
				anchor -> set_active (false);
		}
	}

	activeSensors .clear ();
}

void
X3DPickingContext::leaveNotifyEvent ()
{
	// Clear hits.

	hits .clear ();

	motionNotifyEvent ();
}

void
X3DPickingContext::dispose ()
{
	pickedOutput .dispose ();
}

X3DPickingContext::~X3DPickingContext ()
{ }

} // X3D
} // titania
