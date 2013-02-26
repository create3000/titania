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

#include "ViewpointGroup.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

ViewpointGroup::ViewpointGroup (X3DExecutionContext* const executionContext) :
	       X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	      X3DChildNode (),                                                    
	X3DViewpointObject (),                                                    
	         displayed (true),                                                // SFBool   [in,out] displayed          TRUE
	              size (),                                                    // SFVec3f  [in,out] size               0 0 0        (-∞,∞)
	            center (),                                                    // SFVec3f  [in,out] center             0 0 0        (-∞,∞)
	          children (),                                                    // MFNode   [in,out] children           NULL         [X3DViewpointNode|ViewpointGroup]
	          isActive (),                                                    
	  visibilitySensor (new VisibilitySensor (executionContext)),             
	  viewpointObjects ()                                                     
{
	setComponent ("Navigation");
	setTypeName ("ViewpointGroup");

	addField (inputOutput, "metadata",          metadata);
	addField (inputOutput, "displayed",         displayed);
	addField (inputOutput, "description",       description);
	addField (inputOutput, "retainUserOffsets", retainUserOffsets);
	addField (inputOutput, "size",              size);
	addField (inputOutput, "center",            center);
	addField (inputOutput, "children",          children);

	setChildren (isActive,
	             visibilitySensor);
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

	visibilitySensor -> setup ();
	visibilitySensor -> isActive .addInterest (isActive);

	size   .addInterest (visibilitySensor -> size);
	center .addInterest (visibilitySensor -> center);

	displayed .addInterest (this, &ViewpointGroup::set_displayed);
	size      .addInterest (this, &ViewpointGroup::set_size);
	children  .addInterest (this, &ViewpointGroup::set_children);
	isActive  .addInterest (this, &ViewpointGroup::set_isActive);

	set_size ();
	set_children ();
}

const std::deque <X3DViewpointObject*>
ViewpointGroup::getViewpointObjects () const
{
	return viewpointObjects;
}

void
ViewpointGroup::set_displayed ()
{
	notifyParents ();
}

void
ViewpointGroup::set_size ()
{
	if (size == Vector3f ())
	{
		if (not visibilitySensor -> isActive)
			isActive = true;

		visibilitySensor -> enabled = false;
	}
	else
		visibilitySensor -> enabled = true;
}

void
ViewpointGroup::set_children ()
{
	viewpointObjects .clear ();

	for (const auto & child : children)
	{
		X3DViewpointObject* viewpointObject = dynamic_cast <X3DViewpointObject*> (child .getValue ());

		if (viewpointObject)
			viewpointObjects .emplace_back (viewpointObject);
	}

	notifyParents ();
}

void
ViewpointGroup::set_isActive ()
{
	notifyParents ();
}

void
ViewpointGroup::traverse (TraverseType type)
{
	visibilitySensor -> traverse (type);

	for (const auto & viewpointObject : viewpointObjects)
		viewpointObject -> traverse (type);
}

void
ViewpointGroup::dispose ()
{
	visibilitySensor .dispose ();

	X3DChildNode::dispose ();
	X3DViewpointObject::dispose ();
}

} // X3D
} // titania
