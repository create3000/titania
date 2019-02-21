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

#include "Switch.h"

#include "../../Bits/Cast.h"
#include "../../Browser/PointingDeviceSensor/HierarchyGuard.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

const Component   Switch::component      = Component ("Grouping", 2);
const std::string Switch::typeName       = "Switch";
const std::string Switch::containerField = "children";

Switch::Fields::Fields () :
	whichChoice (new SFInt32 (-1))
{ }

Switch::Switch (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	         fields (),
	  privateChoice (-1),
	      childNode ()
{
	addType (X3DConstants::Switch);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "whichChoice",    whichChoice ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());

	addField (VRML_V2_0, "choice", "children");

	addChildObjects (childNode);
}

X3DBaseNode*
Switch::create (X3DExecutionContext* const executionContext) const
{
	return new Switch (executionContext);
}

void
Switch::initialize ()
{
	X3DGroupingNode::initialize ();

	whichChoice () .addInterest (&Switch::setPrivateChoice, this, -1);

	set_whichChoice ();
}

Box3d
Switch::getBBox () const
{
	if (bboxSize () == Vector3f (-1, -1, -1))
	{
		const auto boundedObject = x3d_cast <X3DBoundedObject*> (childNode);

		if (boundedObject)
			return boundedObject -> getBBox ();
		
		return Box3d ();
	}

	return Box3d (bboxSize () .getValue (), bboxCenter () .getValue ());
}

void
Switch::setPrivateChoice (const int32_t value)
{
	privateChoice = value;

	set_whichChoice ();
}

int32_t
Switch::getWhichChoice () const
{
	if (privateChoice < 0)
		return whichChoice ();

	return privateChoice;
}

void
Switch::set_whichChoice ()
{
	const auto currentChoice = getWhichChoice ();

	if (currentChoice >= 0 and currentChoice < (int32_t) children () .size ())
		childNode .set (x3d_cast <X3DChildNode*> (children () [currentChoice]));
	
	else
		childNode .set (nullptr);

	set_cameraObjects ();
	set_pickableObjects ();

	const_cast <SFTime &> (getExecutionContext () -> bbox_changed ()) = getCurrentTime ();
}

void
Switch::set_cameraObjects ()
{
	setCameraObject (childNode and childNode -> isCameraObject ());
}

void
Switch::set_pickableObjects ()
{
	setPickableObject (childNode and childNode -> isPickableObject ());
}

void
Switch::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		{
			HierarchyGuard guard (renderObject -> getBrowser (), this);
		
			if (childNode)
				childNode -> traverse (type, renderObject);

			break;
		}
		default:
		{
			if (childNode)
				childNode -> traverse (type, renderObject);

			break;
		}
	}
}

} // X3D
} // titania
