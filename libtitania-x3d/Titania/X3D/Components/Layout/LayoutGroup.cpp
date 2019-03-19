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

#include "LayoutGroup.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Layout/Layout.h"

namespace titania {
namespace X3D {

const Component   LayoutGroup::component      = Component ("Layout", 1);
const std::string LayoutGroup::typeName       = "LayoutGroup";
const std::string LayoutGroup::containerField = "children";

LayoutGroup::Fields::Fields () :
	viewport (new SFNode ()),
	  layout (new SFNode ())
{ }

LayoutGroup::LayoutGroup (X3DExecutionContext* const executionContext) :
	               X3DBaseNode (executionContext -> getBrowser (), executionContext),
	           X3DGroupingNode (),
	X3DTransformMatrix3DObject (),
	                    fields (),
	              viewportNode (),
	                layoutNode (),
	           modelViewMatrix ()
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

	addChildObjects (viewportNode,
	                 layoutNode);

	setBBoxEvent (false);
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
	X3DTransformMatrix3DObject::initialize ();

	viewport () .addInterest (&LayoutGroup::set_viewport, this);
	layout ()   .addInterest (&LayoutGroup::set_layout, this);

	set_viewport ();
	set_layout ();
}

Box3d
LayoutGroup::getBBox () const
{
	return X3DGroupingNode::getBBox () * getMatrix ();
}

Box3d
LayoutGroup::getRectangleBBox () const
{
	if (layoutNode)
	{
		const auto & size   = layoutNode -> getRectangleSize ();
		const auto & center = layoutNode -> getRectangleCenter ();

		Vector3d t, s;
		Rotation4d r;
		
		modelViewMatrix .get (t, r, s);

		return Box3d (Vector3d (size .x (), size .y (), 0) / s, Vector3d (center .x (), center .y (), 0) / s);
	}

	return Box3d ();
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
LayoutGroup::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	switch (type)
	{
		case TraverseType::POINTER:
		case TraverseType::CAMERA:
		case TraverseType::PICKING:
		case TraverseType::DEPTH:
		case TraverseType::DISPLAY:
		{
			if (viewportNode)
				viewportNode -> push (renderObject);

			if (layoutNode)
			{
				modelViewMatrix         = renderObject -> getModelViewMatrix () .get ();
				const auto screenMatrix = layoutNode -> transform (type, renderObject);

				setMatrix (screenMatrix * inverse (modelViewMatrix));

				renderObject -> getModelViewMatrix () .push (screenMatrix);
				renderObject -> getLayouts         () .emplace_back (layoutNode);

				X3DGroupingNode::traverse (type, renderObject);

				renderObject -> getLayouts         () .pop_back ();
				renderObject -> getModelViewMatrix () .pop ();
			}
			else
			{
				X3DGroupingNode::traverse (type, renderObject);
			}

			if (viewportNode)
				viewportNode -> pop (renderObject);

			break;
		}
		default:
			break;
	}
}

void
LayoutGroup::dispose ()
{
	X3DTransformMatrix3DObject::dispose ();
	X3DGroupingNode::dispose ();
}

} // X3D
} // titania
