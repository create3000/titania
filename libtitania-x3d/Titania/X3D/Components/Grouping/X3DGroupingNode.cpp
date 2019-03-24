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

#include "X3DGroupingNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Picking/PickingHierarchyGuard.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Rendering/X3DRenderObject.h"

#include "../EnvironmentalEffects/LocalFog.h"
#include "../EnvironmentalSensor/TransformSensor.h"
#include "../Lighting/X3DLightNode.h"
#include "../Picking/X3DPickSensorNode.h"
#include "../PointingDeviceSensor/X3DPointingDeviceSensorNode.h"
#include "../Rendering/ClipPlane.h"

#include <Titania/Utility/Range.h>

namespace titania {
namespace X3D {

X3DGroupingNode::Fields::Fields () :
	   addChildren (new MFNode ()),
	removeChildren (new MFNode ()),
	      children (new MFNode ())
{ }

X3DGroupingNode::X3DGroupingNode () :
	             X3DChildNode (),
	         X3DBoundedObject (),
	                   fields (),
	                   hidden (false),
	                  visible (),
	             allowedTypes (),
	pointingDeviceSensorNodes (),
	            cameraObjects (),
	           clipPlaneNodes (),
	            localFogNodes (),
	               lightNodes (),
	     transformSensorNodes (),
	          pickSensorNodes (),
	      pickableSensorNodes (),
	          pickableObjects (),
	               childNodes ()
{
	addType (X3DConstants::X3DGroupingNode);

	addChildObjects (visible,
	                 pointingDeviceSensorNodes,
	                 cameraObjects,
	                 clipPlaneNodes,
	                 localFogNodes,
	                 lightNodes,
	                 transformSensorNodes,
	                 pickSensorNodes,
	                 pickableSensorNodes,
	                 pickableObjects,
	                 childNodes);
}

void
X3DGroupingNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	visible                   .setTainted (true);
	pointingDeviceSensorNodes .setTainted (true);
	cameraObjects             .setTainted (true);
	clipPlaneNodes            .setTainted (true);
	localFogNodes             .setTainted (true);
	lightNodes                .setTainted (true);
	transformSensorNodes      .setTainted (true);
	pickSensorNodes           .setTainted (true);
	pickableSensorNodes       .setTainted (true);
	pickableObjects           .setTainted (true);
	childNodes                .setTainted (true);

	transformSensors_changed () .addInterest (&X3DGroupingNode::set_pickableObjects, this);

	addChildren ()    .addInterest (&X3DGroupingNode::set_addChildren,    this);
	removeChildren () .addInterest (&X3DGroupingNode::set_removeChildren, this);
	children ()       .addInterest (&X3DGroupingNode::set_children,       this);

	set_children ();
}

Box3d
X3DGroupingNode::getBBox () const
{
	if (isHidden ())
		return Box3d ();

	if (bboxSize () == Vector3f (-1, -1, -1))
		return X3DBoundedObject::getBBox (childNodes);

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
X3DGroupingNode::isHidden (const bool value)
{
	if (value not_eq hidden)
	{
		hidden = value;

		set_children ();
	}
}

void
X3DGroupingNode::setAllowedTypes (const NodeTypeSet & value)
{
	allowedTypes = value;
}

void
X3DGroupingNode::set_addChildren ()
{
	if (addChildren () .empty ())
		return;

	std::set <size_t> set;

	for (const auto & value : children ())
		set .emplace (value ? value -> getId () : 0);

	addChildren () .setTainted (true);
	addChildren () .erase (std::remove_if (addChildren () .begin (), addChildren () .end (),
	                                       [&set] (const SFNode & value) { return set .count (value ? value -> getId () : 0); }),
	                       addChildren () .end ());

	if (not children () .getTainted ())
	{
		children () .removeInterest (&X3DGroupingNode::set_children, this);
		children () .addInterest (&X3DGroupingNode::connectChildren, this);
	}

	const auto first = children () .size ();

	children () .insert (children () .end (), addChildren () .cbegin (), addChildren () .cend ());
	add (first, addChildren ());

	addChildren () .set ({ });
	addChildren () .setTainted (false);
}

void
X3DGroupingNode::set_removeChildren ()
{
	if (removeChildren () .empty ())
		return;

	if (children () .empty ())
		return;

	if (not children () .getTainted ())
	{
		children () .removeInterest (&X3DGroupingNode::set_children, this);
		children () .addInterest (&X3DGroupingNode::connectChildren, this);
	}

	children () .set_difference (removeChildren ());

	remove (removeChildren ());

	removeChildren () .set ({ });
}

void
X3DGroupingNode::set_children ()
{
	clear ();
	add (0, children ());
}

void
X3DGroupingNode::connectChildren ()
{
	children () .removeInterest (&X3DGroupingNode::connectChildren, this);
	children () .addInterest (&X3DGroupingNode::set_children, this);
}

void
X3DGroupingNode::add (const size_t first, const MFNode & children)
{
	if (hidden)
		return;

	size_t i = first;

	for (const auto & node : children)
	{
		if (node and (i >= getVisible () .size () or getVisible () [i]))
		{
			try
			{
				const auto innerNode = node -> getInnerNode ();

				if (allowedTypes .size ())
				{
					if (not innerNode -> isType (allowedTypes))
						continue;
				}

				for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
				{
					switch (type)
					{
						case X3DConstants::X3DPointingDeviceSensorNode:
						{
							pointingDeviceSensorNodes .emplace_back (dynamic_cast <X3DPointingDeviceSensorNode*> (innerNode));
							break;
						}
						case X3DConstants::ClipPlane:
						{
							clipPlaneNodes .emplace_back (dynamic_cast <ClipPlane*> (innerNode));
							break;
						}
						case X3DConstants::LocalFog :
						{
							localFogNodes .emplace_back (dynamic_cast <LocalFog*> (innerNode));
							break;
						}
						case X3DConstants::X3DLightNodeTool:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject ()   .addInterest (&X3DGroupingNode::set_cameraObjects,   this);
							childNode -> isPickableObject () .addInterest (&X3DGroupingNode::set_pickableObjects, this);

							childNodes .emplace_back (childNode);

							// Procceed with next step.
						}
						case X3DConstants::X3DLightNode:
						{
							lightNodes .emplace_back (dynamic_cast <X3DLightNode*> (innerNode));
							break;
						}
						case X3DConstants::TransformSensor:
						{
							const auto transformSensorNode = dynamic_cast <TransformSensor*> (innerNode);

							transformSensorNode -> isPickableObject () .addInterest (&X3DGroupingNode::set_pickableObjects, this);

							transformSensorNodes .emplace_back (transformSensorNode);
							break;
						}
						case X3DConstants::X3DPickSensorNode:
						{
							const auto pickSensorNode = dynamic_cast <X3DPickSensorNode*> (innerNode);

							pickSensorNode -> isPickableObject () .addInterest (&X3DGroupingNode::set_pickableObjects, this);

							pickSensorNodes .emplace_back (pickSensorNode);
							break;
						}
						case X3DConstants::TransformSensorTool:
						case X3DConstants::X3DChildNode:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject ()   .addInterest (&X3DGroupingNode::set_cameraObjects,   this);
							childNode -> isPickableObject () .addInterest (&X3DGroupingNode::set_pickableObjects, this);

							childNodes .emplace_back (childNode);
							break;
						}
						case X3DConstants::BooleanFilter:
						case X3DConstants::BooleanToggle:
						case X3DConstants::CollisionSensor:
						case X3DConstants::NurbsOrientationInterpolator:
						case X3DConstants::NurbsPositionInterpolator:
						case X3DConstants::NurbsSurfaceInterpolator:
						case X3DConstants::RigidBodyCollection:
						case X3DConstants::TimeSensor:
						case X3DConstants::X3DFollowerNode:
						case X3DConstants::X3DInfoNode:
						case X3DConstants::X3DInterpolatorNode:
						case X3DConstants::X3DLayoutNode:
						case X3DConstants::X3DScriptNode:
						case X3DConstants::X3DSequencerNode:
						case X3DConstants::X3DTriggerNode:
							break;
						default:
							continue;
					}

					break;
				}
			}
			catch (const X3DError &)
			{ }
		}

		++ i;
	}

	set_cameraObjects ();
	set_pickableObjects ();
}

void
X3DGroupingNode::remove (const MFNode & children)
{
	for (const auto & node : children)
	{
		if (node)
		{
			try
			{
				const auto innerNode = node -> getInnerNode ();

				for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
				{
					switch (type)
					{
						case X3DConstants::X3DPointingDeviceSensorNode:
						{
							pointingDeviceSensorNodes .erase (std::remove (pointingDeviceSensorNodes .begin (),
							                                               pointingDeviceSensorNodes .end (),
							                                               dynamic_cast <X3DPointingDeviceSensorNode*> (innerNode)),
							                                  pointingDeviceSensorNodes .end ());
							break;
						}
						case X3DConstants::ClipPlane:
						{
							clipPlaneNodes .erase (std::remove (clipPlaneNodes .begin (),
							                                    clipPlaneNodes .end (),
							                                    dynamic_cast <ClipPlane*> (innerNode)),
							                       clipPlaneNodes .end ());
							break;
						}
						case X3DConstants::LocalFog :
						{
							localFogNodes .erase (std::remove (localFogNodes .begin (),
							                                   localFogNodes .end (),
							                                   dynamic_cast <LocalFog*> (innerNode)),
							                      localFogNodes .end ());
							break;
						}
						case X3DConstants::X3DLightNodeTool:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject ()   .removeInterest (&X3DGroupingNode::set_cameraObjects,   this);
							childNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

							childNodes .erase (std::remove (childNodes .begin (),
							                                childNodes .end (),
							                                childNode),
							                   childNodes .end ());

							// Procceed with next step.
						}
						case X3DConstants::X3DLightNode:
						{
							lightNodes .erase (std::remove (lightNodes .begin (),
							                                lightNodes .end (),
							                                dynamic_cast <X3DLightNode*> (innerNode)),
							                   lightNodes .end ());
							break;
						}
						case X3DConstants::TransformSensor:
						{
							const auto transformSensorNode = dynamic_cast <TransformSensor*> (innerNode);

							transformSensorNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

							transformSensorNodes .erase (std::remove (transformSensorNodes .begin (),
							                                          transformSensorNodes .end (),
							                                          transformSensorNode),
							                             transformSensorNodes .end ());
							break;
						}
						case X3DConstants::X3DPickSensorNode:
						{
							const auto pickSensorNode = dynamic_cast <X3DPickSensorNode*> (innerNode);

							pickSensorNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

							pickSensorNodes .erase (std::remove (pickSensorNodes .begin (),
							                                     pickSensorNodes .end (),
							                                     pickSensorNode),
							                        pickSensorNodes .end ());
							break;
						}
						case X3DConstants::TransformSensorTool:
						case X3DConstants::X3DChildNode:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject ()   .removeInterest (&X3DGroupingNode::set_cameraObjects,   this);
							childNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

							childNodes .erase (std::remove (childNodes .begin (),
							                                childNodes .end (),
							                                childNode),
							                   childNodes .end ());
							break;
						}
						case X3DConstants::BooleanFilter:
						case X3DConstants::BooleanToggle:
						case X3DConstants::CollisionSensor:
						case X3DConstants::NurbsOrientationInterpolator:
						case X3DConstants::NurbsPositionInterpolator:
						case X3DConstants::NurbsSurfaceInterpolator:
						case X3DConstants::RigidBodyCollection:
						case X3DConstants::TimeSensor:
						case X3DConstants::X3DFollowerNode:
						case X3DConstants::X3DInfoNode:
						case X3DConstants::X3DInterpolatorNode:
						case X3DConstants::X3DLayoutNode:
						case X3DConstants::X3DScriptNode:
						case X3DConstants::X3DSequencerNode:
						case X3DConstants::X3DTriggerNode:
							break;
						default:
							continue;
					}

					break;
				}
			}
			catch (const X3DError &)
			{ }
		}
	}

	set_cameraObjects ();
	set_pickableObjects ();
}

void
X3DGroupingNode::clear ()
{
	for (const auto & transformSensorNode : transformSensorNodes)
		transformSensorNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

	for (const auto & pickSensorNode : pickSensorNodes)
		pickSensorNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);

	for (const auto & childNode : childNodes)
	{
		childNode -> isCameraObject ()   .removeInterest (&X3DGroupingNode::set_cameraObjects,   this);
		childNode -> isPickableObject () .removeInterest (&X3DGroupingNode::set_pickableObjects, this);
	}

	pointingDeviceSensorNodes .clear ();
	cameraObjects             .clear ();
	clipPlaneNodes            .clear ();
	localFogNodes             .clear ();
	lightNodes                .clear ();
	transformSensorNodes      .clear ();
	pickSensorNodes           .clear ();
	childNodes                .clear ();
}

void
X3DGroupingNode::set_cameraObjects ()
{
	cameraObjects .clear ();

	for (const auto & childNode : childNodes)
	{
		if (childNode -> isCameraObject ())
			cameraObjects .emplace_back (childNode);
	}

	setCameraObject (not cameraObjects .empty ());
}

void
X3DGroupingNode::set_pickableObjects ()
{
	pickableSensorNodes .clear ();
	pickableObjects     .clear ();

	for (const auto & transformSensorNode : transformSensorNodes)
	{
		if (transformSensorNode -> isPickableObject ())
			pickableSensorNodes .emplace_back (transformSensorNode);
	}

	for (const auto & pickSensorNode : pickSensorNodes)
	{
		if (pickSensorNode -> isPickableObject ())
			pickableSensorNodes .emplace_back (pickSensorNode);
	}

	for (const auto & childNode : childNodes)
	{
		if (childNode -> isPickableObject ())
			pickableObjects .emplace_back (childNode);
	}

	setPickableObject (not (getTransformSensors () .empty () and pickableSensorNodes .empty () and pickableObjects .empty ()));
}

void
X3DGroupingNode::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			HierarchyGuard guard (renderObject -> getBrowser (), this);

			if (not pointingDeviceSensorNodes .empty ())
			{
				renderObject -> getBrowser () -> getSensors () .emplace_back ();

				for (const auto & pointingDeviceSensorNode : pointingDeviceSensorNodes)
					pointingDeviceSensorNode -> push (renderObject);
			}

			for (const auto & clipPlaneNode : clipPlaneNodes)
				clipPlaneNode -> push (renderObject);

			for (const auto & childNode : childNodes)
				childNode -> traverse (type, renderObject);

			for (const auto & clipPlaneNode : basic::make_reverse_range (clipPlaneNodes))
				clipPlaneNode -> pop (renderObject);

			if (not pointingDeviceSensorNodes .empty ())
				renderObject -> getBrowser () -> getSensors () .pop_back ();

			return;
		}
		case TraverseType::CAMERA:
		{
			for (const auto & cameraObject : cameraObjects)
				cameraObject -> traverse (type, renderObject);

			return;
		}
		case TraverseType::PICKING:
		{
			PickingHierarchyGuard guard (renderObject -> getBrowser (), this);

			if (not getTransformSensors () .empty ())
			{
				const auto bbox = getSubBBox () * renderObject -> getModelViewMatrix () .get ();

				for (const auto & transformSensorNode : getTransformSensors ())
					transformSensorNode -> collect (bbox);
			}

			for (const auto & pickableSensorNode : pickableSensorNodes)
				pickableSensorNode -> traverse (type, renderObject);

			if (renderObject -> getBrowser () -> getPickable () .top ())
			{
				for (const auto & childNode : childNodes)
					childNode -> traverse (type, renderObject);
			}
			else
			{
				for (const auto & pickableObject : pickableObjects)
					pickableObject -> traverse (type, renderObject);
			}

			return;
		}
		case TraverseType::COLLISION:
		case TraverseType::DEPTH:
		{
			for (const auto & clipPlaneNode : clipPlaneNodes)
				clipPlaneNode -> push (renderObject);

			for (const auto & childNode : childNodes)
				childNode -> traverse (type, renderObject);

			for (const auto & clipPlaneNode : basic::make_reverse_range (clipPlaneNodes))
				clipPlaneNode -> pop (renderObject);

			return;
		}
		case TraverseType::DISPLAY:
		{
			for (const auto & clipPlaneNode : clipPlaneNodes)
				clipPlaneNode -> push (renderObject);

			for (const auto & localFogNode : localFogNodes)
				localFogNode -> push (renderObject);

			for (const auto & lightNode : lightNodes)
				lightNode -> push (renderObject, this);

			for (const auto & childNode : childNodes)
				childNode -> traverse (type, renderObject);

			for (const auto & lightNode : basic::make_reverse_range (lightNodes))
				lightNode -> pop (renderObject);

			for (const auto & localFogNode : basic::make_reverse_range (localFogNodes))
				localFogNode -> pop (renderObject);

			for (const auto & clipPlaneNode : basic::make_reverse_range (clipPlaneNodes))
				clipPlaneNode -> pop (renderObject);

			return;
		}
	}
}

void
X3DGroupingNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

X3DGroupingNode::~X3DGroupingNode ()
{ }

} // X3D
} // titania
