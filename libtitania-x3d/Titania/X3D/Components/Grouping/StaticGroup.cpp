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

#include "StaticGroup.h"

#include "../Grouping/Group.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const Component   StaticGroup::component      = Component ("Grouping", 3);
const std::string StaticGroup::typeName       = "StaticGroup";
const std::string StaticGroup::containerField = "children";

StaticGroup::Fields::Fields () :
	children (new MFNode ())
{ }

StaticGroup::StaticGroup (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DChildNode (),
	X3DBoundedObject (),
	          fields (),
	           group (new Group (executionContext)),
	            bbox ()
{
	addType (X3DConstants::StaticGroup);

	addField (inputOutput,    "metadata",   metadata ());
	addField (initializeOnly, "bboxSize",   bboxSize ());
	addField (initializeOnly, "bboxCenter", bboxCenter ());
	addField (initializeOnly, "children",   children ());

	addChildObjects (group);
}

X3DBaseNode*
StaticGroup::create (X3DExecutionContext* const executionContext) const
{
	return new StaticGroup (executionContext);
}

void
StaticGroup::initialize ()
{
	X3DChildNode::initialize ();
	X3DBoundedObject::initialize ();

	bboxSize ()   .addInterest (group -> bboxSize ());
	bboxCenter () .addInterest (group -> bboxCenter ());
	children ()   .addInterest (group -> children ());

	group -> bboxSize ()   = bboxSize ();
	group -> bboxCenter () = bboxCenter ();
	group -> children ()   = children ();

	group -> setPrivate (true);
	group -> setup ();

	// Connect after Group setup.
	group -> isCameraObject ()   .addInterest (&StaticGroup::setCameraObject,   static_cast <X3DChildNode*> (this));
	group -> isPickableObject () .addInterest (&StaticGroup::setPickableObject, static_cast <X3DChildNode*> (this));
	group -> children ()         .addInterest (&StaticGroup::set_children,      this);

	setCameraObject   (group -> isCameraObject ());
	setPickableObject (group -> isPickableObject ());

	set_children ();
}

void
StaticGroup::setExecutionContext (X3DExecutionContext* const executionContext)
{
	group -> setExecutionContext (executionContext);

	X3DChildNode::setExecutionContext (executionContext);
}

void
StaticGroup::set_children ()
{
	bbox = group -> getBBox ();
}

Box3d
StaticGroup::getBBox () const
{
	return bbox;
}

void
StaticGroup::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	group -> traverse (type, renderObject);
}

void
StaticGroup::addTool ()
{
	group -> addTool ();
}

void
StaticGroup::removeTool (const bool really)
{
	group -> removeTool (really);
}

void
StaticGroup::dispose ()
{
	X3DBoundedObject::dispose ();
	X3DChildNode::dispose ();
}

} // X3D
} // titania
