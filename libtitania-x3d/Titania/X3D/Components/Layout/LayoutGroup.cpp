/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "LayoutGroup.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layout/Layout.h"
#include "../../Tools/Layout/LayoutGroupTool.h"

namespace titania {
namespace X3D {

const ComponentType LayoutGroup::component      = ComponentType::LAYOUT;
const std::string   LayoutGroup::typeName       = "LayoutGroup";
const std::string   LayoutGroup::containerField = "children";

LayoutGroup::Fields::Fields () :
	viewport (new SFNode ()),
	  layout (new SFNode ())
{ }

LayoutGroup::LayoutGroup (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGroupingNode (),
	         fields (),
	   viewportNode (),
	     layoutNode (),
	modelViewMatrix (),
	   screenMatrix (),
	         matrix ()
{
	addType (X3DConstants::LayoutGroup);

	addField (inputOutput,    "metadata",       metadata ());
	addField (inputOutput,    "layout",         layout ());
	addField (inputOutput,    "viewport",       viewport ());
	addField (initializeOnly, "bboxSize",       bboxSize ());
	addField (initializeOnly, "bboxCenter",     bboxCenter ());
	addField (inputOnly,      "addChildren",    addChildren ());
	addField (inputOnly,      "removeChildren", removeChildren ());
	addField (inputOutput,    "children",       children ());

	X3DParentObject::addChildren (viewportNode,
	                              layoutNode);
}

X3DBaseNode*
LayoutGroup::create (X3DExecutionContext* const executionContext) const
{
	return new LayoutGroup (executionContext);
}

void
LayoutGroup::initialize ()
{
	X3DGroupingNode::initialize ();

	viewport () .addInterest (this, &LayoutGroup::set_viewport);
	layout ()   .addInterest (this, &LayoutGroup::set_layout);

	set_viewport ();
	set_layout ();
}

Box3f
LayoutGroup::getBBox () const
{
	return X3DGroupingNode::getBBox () * getMatrix ();
}

Box3f
LayoutGroup::getRectangleBBox () const
{
	if (layoutNode)
	{
		const auto & size   = layoutNode -> getRectangleSize ();
		const auto & center = layoutNode -> getRectangleCenter ();

		Vector3f t, s;
		Rotation4f r;
		
		modelViewMatrix .get (t, r, s);

		return Box3f (Vector3f (size .x (), size .y (), 0) / s, Vector3f (center .x (), center .y (), 0) / s);
	}

	return Box3f ();
}

const Matrix4f &
LayoutGroup::getMatrix () const
{
	try
	{
		if (layoutNode)
			const_cast <LayoutGroup*> (this) -> matrix = screenMatrix * inverse (modelViewMatrix);
		else
			const_cast <LayoutGroup*> (this) -> matrix = Matrix4f ();
	}
	catch (const std::domain_error &)
	{ }

	return matrix;
}

void
LayoutGroup::set_viewport ()
{
	viewportNode .set (x3d_cast <X3DViewportNode*> (viewport ()));
}

void
LayoutGroup::set_layout ()
{
	layoutNode .set (x3d_cast <X3DLayoutNode*> (layout ()));
}

void
LayoutGroup::traverse (const TraverseType type)
{
	switch (type)
	{
		case TraverseType::CAMERA:
		case TraverseType::POINTER:
		case TraverseType::DISPLAY:
		{
			if (viewportNode)
				viewportNode -> push (type);

			if (layoutNode)
			{
				getModelViewMatrix () .push ();

				modelViewMatrix = getModelViewMatrix (type);

				layoutNode -> transform (type);
				
				screenMatrix = layoutNode -> getMatrix ();

				getBrowser () -> getLayouts () .push (layoutNode);

				X3DGroupingNode::traverse (type);

				getBrowser () -> getLayouts () .pop ();

				getModelViewMatrix () .pop ();
			}
			else
				X3DGroupingNode::traverse (type);

			if (viewportNode)
				viewportNode -> pop (type);

			break;
		}
		default:
			break;
	}
}

void
LayoutGroup::addTool ()
{
	X3DGroupingNode::addTool (new LayoutGroupTool (this));
}

} // X3D
} // titania
