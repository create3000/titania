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

#include "ViewpointGroup.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   ViewpointGroup::component      = Component ("Navigation", 2);
const std::string ViewpointGroup::typeName       = "ViewpointGroup";
const std::string ViewpointGroup::containerField = "children";

ViewpointGroup::Fields::Fields () :
	displayed (new SFBool (true)),
	     size (new SFVec3f ()),
	   center (new SFVec3f ()),
	 children (new MFNode ())
{ }

ViewpointGroup::ViewpointGroup (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext),
	      X3DChildNode (),
	X3DViewpointObject (),
	            fields (),
	   proximitySensor (new ProximitySensor (executionContext)),
	  viewpointObjects ()
{
	addType (X3DConstants::ViewpointGroup);

	addField (inputOutput, "metadata",          metadata ());
	addField (inputOutput, "displayed",         displayed ());
	addField (inputOutput, "description",       description ());
	addField (inputOutput, "retainUserOffsets", retainUserOffsets ());
	addField (inputOutput, "size",              size ());
	addField (inputOutput, "center",            center ());
	addField (inputOutput, "children",          children ());

	size ()   .setUnit (UnitCategory::LENGTH);
	center () .setUnit (UnitCategory::LENGTH);

	addChildObjects (proximitySensor);

	setCameraObject (true);
}

X3DBaseNode*
ViewpointGroup::create (X3DExecutionContext* const executionContext) const
{
	return new ViewpointGroup (executionContext);
}

void
ViewpointGroup::initialize ()
{
	X3DChildNode::initialize ();
	X3DViewpointObject::initialize ();

	proximitySensor -> setup ();

	size ()   .addInterest (proximitySensor -> size ());
	center () .addInterest (proximitySensor -> center ());

	proximitySensor -> size ()   = size ();
	proximitySensor -> center () = center ();

	displayed () .addInterest (&ViewpointGroup::set_displayed, this);
	size ()      .addInterest (&ViewpointGroup::set_displayed, this);
	children ()  .addInterest (&ViewpointGroup::set_children, this);

	set_displayed ();
	set_children ();
}

void
ViewpointGroup::setExecutionContext (X3DExecutionContext* const executionContext)
{
	proximitySensor -> setExecutionContext (executionContext);

	X3DChildNode::setExecutionContext (executionContext);
}

const std::vector <X3DViewpointObject*>
ViewpointGroup::getViewpointObjects () const
{
	return viewpointObjects;
}

void
ViewpointGroup::set_displayed ()
{
	proximitySensor -> enabled () = displayed () and size () not_eq Vector3f ();

	if (proximitySensor -> enabled ())
	{
		proximitySensor -> isCameraObject () .addInterest (&ViewpointGroup::setCameraObject, static_cast <X3DChildNode*> (this));
		setCameraObject (proximitySensor -> isCameraObject ());
	}
	else
	{
		proximitySensor -> isCameraObject () .removeInterest (&ViewpointGroup::setCameraObject, static_cast <X3DChildNode*> (this));
		setCameraObject (true);
	}
}

void
ViewpointGroup::set_children ()
{
	viewpointObjects .clear ();
	viewpointObjects .reserve (children () .size ());

	for (const auto & child : children ())
	{
		X3DViewpointObject* viewpointObject = x3d_cast <X3DViewpointObject*> (child);

		if (viewpointObject)
			viewpointObjects .emplace_back (viewpointObject);
	}
}

void
ViewpointGroup::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	proximitySensor -> traverse (type, renderObject);

	if (proximitySensor -> isActive () or size () == Vector3f ())
	{
		for (const auto & viewpointObject : viewpointObjects)
			viewpointObject -> traverse (type, renderObject);
	}
}

void
ViewpointGroup::addTool ()
{
	proximitySensor -> addTool ();
}

void
ViewpointGroup::removeTool (const bool really)
{
	proximitySensor -> removeTool (really);
}

void
ViewpointGroup::dispose ()
{
	X3DViewpointObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
