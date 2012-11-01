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

#include "../../Browser/Browser.h"
#include "X3DGroupingNode.h"

namespace titania {
namespace X3D {

X3DGroupingNode::X3DGroupingNode () :
	    X3DChildNode (), 
	X3DBoundedObject (), 
	     addChildren (), // MFNode[in]     addChildren               [X3DChildNode]
	  removeChildren (), // MFNode[in]     removeChildren            [X3DChildNode]
	        children ()  // MFNode[in,out] children        [ ]       [X3DChildNode]
{
	addNodeType (X3DGroupingNodeType);

	setChildren (pointingDeviceSensors,
	             lights,
	             localFogs,
	             childNodes);
}

void
X3DGroupingNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	children .addInterest (this, &X3DGroupingNode::set_children);
	set_children ();
}

Box3f
X3DGroupingNode::getBBox ()
{
	if (bboxSize == Vector3f (-1, -1, -1))
	{
		// Find bounding box objects

		MFNode <X3DBoundedObject> boundedObjects;

		for (const auto & child : children)
		{
			SFNode <X3DBoundedObject> boundedObject = child;

			if (boundedObject)
				boundedObjects .push_back (boundedObject);
		}

		// Find first non zero bounding box

		Vector3f min;
		Vector3f max;

		for (const auto & boundedObject : boundedObjects)
		{
			const Box3f & bbox = boundedObject -> getBBox ();

			if (bbox .size () not_eq Vector3f ())
			{
				bbox .minmax (min, max);
				break;
			}
		}

		// Add bounding boxes

		for (const auto & boundedObject : boundedObjects)
		{
			const Box3f & bbox = boundedObject -> getBBox ();

			if (bbox .size () not_eq Vector3f ())
			{
				min = math::min (min, bbox .min ());
				max = math::max (max, bbox .max ());
			}
		}

		Vector3f size   = max - min;
		Vector3f center = min + size * 0.5f;

		return Box3f (size, center);
	}

	return Box3f (bboxSize, bboxCenter);
}

void
X3DGroupingNode::set_children ()
{
	pointingDeviceSensors .clear ();
	lights .clear ();
	localFogs .clear ();
	childNodes .clear ();

	for (const auto & child : children)
	{
		SFNode <X3DPointingDeviceSensorNode> pointingDeviceSensorNode = child;

		if (pointingDeviceSensorNode)
			pointingDeviceSensors .push_back (pointingDeviceSensorNode);

		else
		{
			SFNode <X3DLightNode> lightNode = child;

			if (lightNode)
				lights .push_back (lightNode);

			else
			{
				SFNode <LocalFog> localFogNode = child;

				if (localFogNode)
					localFogs .push_back (localFogNode);

				else
					childNodes .push_back (child);
			}
		}
	}
}

void
X3DGroupingNode::intersect ()
{
	if (pointingDeviceSensors .size ())
		pointingDeviceSensorsDisplay ();

	childrenSelect ();

	if (pointingDeviceSensors .size ())
		pointingDeviceSensorsPostDisplay ();
}

void
X3DGroupingNode::draw ()
{
	if (lights .size ())
		lightsDisplay ();

	if (localFogs .size ())
		localFogsDisplay ();

	childrenDisplay ();

	if (localFogs .size ())
		localFogsPostDisplay ();

	if (lights .size ())
		lightsPostDisplay ();
}

void
X3DGroupingNode::pointingDeviceSensorsDisplay ()
{
	//	if (not getBrowser () -> getEditMode ())
	//	{
	for (const auto & pointingDeviceSensor : pointingDeviceSensors)
		pointingDeviceSensor -> display ();

	//	}
}

void
X3DGroupingNode::lightsDisplay ()
{
	for (const auto & light : lights)
		light -> display ();
}

void
X3DGroupingNode::localFogsDisplay ()
{
	localFogs .back () -> display ();
}

void
X3DGroupingNode::childrenSelect ()
{
	for (const auto & child : childNodes)
		child -> select ();
}

void
X3DGroupingNode::childrenDisplay ()
{
	for (const auto & child : childNodes)
		child -> display ();
}

void
X3DGroupingNode::localFogsPostDisplay ()
{
	localFogs .back () -> postDisplay ();
}

void
X3DGroupingNode::lightsPostDisplay ()
{
	for (auto light = lights .crbegin ();
	     light not_eq lights .crend ();
	     ++ light)
		(*light) -> finish ();
}

void
X3DGroupingNode::pointingDeviceSensorsPostDisplay ()
{
	//	if (not getBrowser () -> getEditMode ())
	//	{
	for (auto pointingDeviceSensor = pointingDeviceSensors .crbegin ();
	     pointingDeviceSensor not_eq pointingDeviceSensors .crend ();
	     ++ pointingDeviceSensor)
		(*pointingDeviceSensor) -> postDisplay ();

	//	}
}

void
X3DGroupingNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
