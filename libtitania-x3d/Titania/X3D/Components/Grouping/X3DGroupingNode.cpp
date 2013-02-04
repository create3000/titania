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

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"

#include <Titania/Utility/Adapter.h>

namespace titania {
namespace X3D {

X3DGroupingNode::X3DGroupingNode () :
	    X3DChildNode (), 
	X3DBoundedObject (), 
	        children (), // MFNode[in,out] children        [ ]       [X3DChildNode]
	     addChildren (), // MFNode[in]     addChildren               [X3DChildNode]
	  removeChildren ()  // MFNode[in]     removeChildren            [X3DChildNode]
{
	addNodeType (X3DConstants::X3DGroupingNode);
}

void
X3DGroupingNode::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	addChildren    .addInterest (this, &X3DGroupingNode::set_addChildren);
	removeChildren .addInterest (this, &X3DGroupingNode::set_removeChildren);
	children       .addInterest (this, &X3DGroupingNode::set_children);

	set_children ();
}

Box3f
X3DGroupingNode::getBBox ()
{
	if (bboxSize == Vector3f (-1, -1, -1))
	{
		return X3DBoundedObject::getBBox (children);
	}

	return Box3f (bboxSize, bboxCenter);
}

void
X3DGroupingNode::set_addChildren ()
{
	if (addChildren .size ())
	{
		add (addChildren);

		children .insert (children .end (), addChildren .begin (), addChildren .end ());

		children .removeInterest (this, &X3DGroupingNode::set_children);
		children .addInterest    (this, &X3DGroupingNode::set_endChildren);
	}
}

void
X3DGroupingNode::set_removeChildren ()
{
	if (removeChildren .size ())
	{
		auto new_end = basic::remove (children .begin (), children .end (), removeChildren .begin (), removeChildren .end ());
		children .erase (new_end, children .end ());

		if (not children .hasInterest (this, &X3DGroupingNode::set_children))
			set_children ();
	}
}

void
X3DGroupingNode::set_endChildren ()
{
	children .removeInterest (this, &X3DGroupingNode::set_endChildren);
	children .addInterest    (this, &X3DGroupingNode::set_children);
}

void
X3DGroupingNode::set_children ()
{
	pointingDeviceSensors .clear ();
	lights     .clear ();
	localFogs  .clear ();
	childNodes .clear ();

	add (children);
}

void
X3DGroupingNode::add (const MFNode & children)
{
	for (const auto & child : children)
	{
		auto pointingDeviceSensorNode = x3d_cast <X3DPointingDeviceSensorNode*> (child .getValue ());

		if (pointingDeviceSensorNode)
			pointingDeviceSensors .emplace_back (pointingDeviceSensorNode);

		else
		{
			auto lightNode = x3d_cast <X3DLightNode*> (child .getValue ());

			if (lightNode)
				lights .emplace_back (lightNode);

			else
			{
				auto localFog = x3d_cast <LocalFog*> (child .getValue ());

				if (localFog)
					localFogs .emplace_back (localFog);

				else
				{
					auto childNode = x3d_cast <X3DChildNode*> (child .getValue ());

					if (childNode)
						childNodes .emplace_back (childNode);
				}
			}
		}
	}
}

void
X3DGroupingNode::pick ()
{
	//	if (not getBrowser () -> getEditMode ())
	//	{
	for (const auto & child : pointingDeviceSensors)
		child -> display ();

	//	}

	for (const auto & child : childNodes)
		child -> pick ();

	//	if (not getBrowser () -> getEditMode ())
	//	{
	for (const auto & child : basic::adapter (pointingDeviceSensors .crbegin (), pointingDeviceSensors .crend ()))
		child -> finish ();

	//	}
}

void
X3DGroupingNode::display ()
{
	for (const auto & child : lights)
		child -> display ();

	if (localFogs .size ())
		localFogs .front () -> display ();

	for (const auto & child : childNodes)
		child -> display ();

	if (localFogs .size ())
		localFogs .front () -> finish ();

	for (const auto & child : basic::adapter (lights .crbegin (), lights .crend ()))
		child -> finish ();
}

void
X3DGroupingNode::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
