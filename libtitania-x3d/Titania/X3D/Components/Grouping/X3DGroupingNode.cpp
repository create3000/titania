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

#include "X3DGroupingNode.h"

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"

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
	pointingDeviceSensors (),
	        cameraObjects (),
	           clipPlanes (),
	            localFogs (),
	               lights (),
	           childNodes ()
{
	addType (X3DConstants::X3DGroupingNode);

	X3DParentObject::addChildren (visible,
	                              pointingDeviceSensors,
	                              cameraObjects,
	                              clipPlanes,
	                              localFogs,
	                              lights,
	                              childNodes);
}

void
X3DGroupingNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	visible               .isTainted (true);
	pointingDeviceSensors .isTainted (true);
	cameraObjects         .isTainted (true);
	clipPlanes            .isTainted (true);
	localFogs             .isTainted (true);
	lights                .isTainted (true);
	childNodes            .isTainted (true);

	addChildren ()    .addInterest (this, &X3DGroupingNode::set_addChildren);
	removeChildren () .addInterest (this, &X3DGroupingNode::set_removeChildren);
	children ()       .addInterest (this, &X3DGroupingNode::set_children);

	set_children ();
}

Box3d
X3DGroupingNode::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
		return X3DBoundedObject::getBBox (children ());

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

//void
//X3DGroupingNode::set_addChildren ()
//{
//	if (addChildren () .empty ())
//		return;
//
//	MFNode childrenToAdd = addChildren ();
//
//	childrenToAdd .remove (children () .begin (), children () .end ());
//
//	children () .insert (children () .end (), childrenToAdd .begin (), childrenToAdd .end ());
//
//	addChildren () .set ({ });
//}

void
X3DGroupingNode::set_addChildren ()
{
	if (addChildren () .empty ())
		return;

	addChildren () .isTainted (true);
	addChildren () .erase (basic::remove (addChildren () .begin (), addChildren () .end (),
	                                      children () .begin (), children () .end ()),
	                       addChildren () .end ());

	if (not children () .isTainted ())
	{
		children () .removeInterest (this, &X3DGroupingNode::set_children);
		children () .addInterest (this, &X3DGroupingNode::connectChildren);
	}

	children () .insert (children () .end (), addChildren () .begin (), addChildren () .end ());
	add (addChildren ());

	addChildren () .set ({ });
	addChildren () .isTainted (false);
}

void
X3DGroupingNode::set_removeChildren ()
{
	if (removeChildren () .empty ())
		return;

	if (children () .empty ())
		return;

	if (not children () .isTainted ())
	{
		children () .removeInterest (this, &X3DGroupingNode::set_children);
		children () .addInterest (this, &X3DGroupingNode::connectChildren);
	}

	children () .erase (basic::remove (children () .begin (), children () .end (),
	                                   removeChildren () .begin (), removeChildren () .end ()),
	                    children () .end ());

	removeChildren () .set ({ });

	set_children ();
}

void
X3DGroupingNode::set_children ()
{
	clear ();
	add (children ());
}

void
X3DGroupingNode::connectChildren ()
{
	children () .removeInterest (this, &X3DGroupingNode::connectChildren);
	children () .addInterest (this, &X3DGroupingNode::set_children);
}

void
X3DGroupingNode::add (const MFNode & children)
{
	if (hidden)
		return;

	size_t i = 0;

	for (const auto & child : children)
	{
		if (child and (i >= getVisible () .size () or getVisible () [i]))
		{
			try
			{
				const auto innerNode = child -> getInnerNode ();

				for (const auto & type : basic::make_reverse_range (innerNode -> getType ()))
				{
					switch (type)
					{
						case X3DConstants::X3DPointingDeviceSensorNode:
						{
							pointingDeviceSensors .emplace_back (dynamic_cast <X3DPointingDeviceSensorNode*> (innerNode));
							goto NEXT;
						}
						case X3DConstants::ClipPlane:
						{
							clipPlanes .emplace_back (dynamic_cast <ClipPlane*> (innerNode));
							goto NEXT;
						}
						case X3DConstants::LocalFog :
						{
							localFogs .emplace_back (dynamic_cast <LocalFog*> (innerNode));
							goto NEXT;
						}
						case X3DConstants::X3DLightNodeTool:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject () .addInterest (this, &X3DGroupingNode::set_cameraObjects);

							childNodes .emplace_back (childNode);
							// Procceed with next step.
						}
						case X3DConstants::X3DLightNode:
						{
							lights .emplace_back (dynamic_cast <X3DLightNode*> (innerNode));
							goto NEXT;
						}
						case X3DConstants::X3DChildNode:
						{
							const auto childNode = dynamic_cast <X3DChildNode*> (innerNode);

							childNode -> isCameraObject () .addInterest (this, &X3DGroupingNode::set_cameraObjects);

							childNodes .emplace_back (childNode);
							goto NEXT;
						}
						case X3DConstants::BooleanFilter:
						case X3DConstants::BooleanToggle:
						case X3DConstants::NurbsOrientationInterpolator:
						case X3DConstants::NurbsPositionInterpolator:
						case X3DConstants::NurbsSurfaceInterpolator:
						case X3DConstants::TimeSensor:
						case X3DConstants::X3DFollowerNode:
						case X3DConstants::X3DInfoNode:
						case X3DConstants::X3DInterpolatorNode:
						case X3DConstants::X3DLayoutNode:
						case X3DConstants::X3DScriptNode:
						case X3DConstants::X3DSequencerNode:
						case X3DConstants::X3DTriggerNode:
							goto NEXT;
						default:
							break;
					}
				}
			}
			catch (const X3DError &)
			{ }
		}

NEXT:
		++ i;
	}

	set_cameraObjects ();
}

void
X3DGroupingNode::clear ()
{
	for (const auto & childNode : childNodes)
	{
	   if (not childNode)
	      __LOG__ << getTypeName () << " : " << getName () << std::endl;

		childNode -> isCameraObject () .removeInterest (this, &X3DGroupingNode::set_cameraObjects);
	}
	
	pointingDeviceSensors .clear ();
	cameraObjects .clear ();
	clipPlanes    .clear ();
	localFogs     .clear ();
	lights        .clear ();
	childNodes    .clear ();
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
X3DGroupingNode::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			if (not pointingDeviceSensors .empty ())
			{
				getBrowser () -> getSensors () .emplace_back ();

				for (const auto & childNode : pointingDeviceSensors)
					childNode -> push ();
			}

			for (const auto & childNode : clipPlanes)
				childNode -> push ();

			for (const auto & childNode : childNodes)
				childNode -> traverse (type);

			for (const auto & childNode : basic::make_reverse_range (clipPlanes))
				childNode -> pop ();

			if (not pointingDeviceSensors .empty ())
				getBrowser () -> getSensors () .pop_back ();

			return;
		}
		case TraverseType::CAMERA:
		{
			for (const auto & childNode : cameraObjects)
				childNode -> traverse (type);

			return;
		}
		case TraverseType::COLLISION:
		{
			for (const auto & childNode : clipPlanes)
				childNode -> push ();

			for (const auto & childNode : childNodes)
				childNode -> traverse (type);

			for (const auto & childNode : basic::make_reverse_range (clipPlanes))
				childNode -> pop ();

			return;
		}
		case TraverseType::DISPLAY:
		case TraverseType::DEPTH:
		{
			for (const auto & childNode : clipPlanes)
				childNode -> push ();

			for (const auto & childNode : localFogs)
				childNode -> push ();

			for (const auto & childNode : lights)
				childNode -> push ();

			for (const auto & childNode : childNodes)
				childNode -> traverse (type);

			for (const auto & childNode : basic::make_reverse_range (lights))
				childNode -> pop ();

			for (const auto & childNode : basic::make_reverse_range (localFogs))
				childNode -> pop ();

			for (const auto & childNode : basic::make_reverse_range (clipPlanes))
				childNode -> pop ();

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

} // X3D
} // titania
