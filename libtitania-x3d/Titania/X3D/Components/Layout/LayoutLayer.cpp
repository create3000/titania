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

#include "LayoutLayer.h"

#include "../../Execution/X3DExecutionContext.h"

#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../Navigation/OrthoViewpoint.h"
#include "../Layout/LayoutGroup.h"

namespace titania {
namespace X3D {

const Component   LayoutLayer::component      = Component ("Layout", 1);
const std::string LayoutLayer::typeName       = "LayoutLayer";
const std::string LayoutLayer::containerField = "layers";

LayoutLayer::Fields::Fields () :
	        layout (new SFNode ()),
	   addChildren (new MFNode ()),
	removeChildren (new MFNode ()),
	      children (new MFNode ())
{ }

LayoutLayer::LayoutLayer (X3DExecutionContext* const executionContext) :
	 X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLayerNode (new OrthoViewpoint (executionContext), new LayoutGroup (executionContext)),
	      fields ()
{
	addType (X3DConstants::LayoutLayer);

	addField (inputOutput, "metadata",       metadata ());
	addField (inputOutput, "isPickable",     isPickable ());
	addField (inputOutput, "layout",         layout ());
	addField (inputOutput, "viewport",       viewport ());
	addField (inputOnly,   "addChildren",    addChildren ());
	addField (inputOnly,   "removeChildren", removeChildren ());
	addField (inputOutput, "children",       children ());
}

X3DBaseNode*
LayoutLayer::create (X3DExecutionContext* const executionContext) const
{
	return new LayoutLayer (executionContext);
}

void
LayoutLayer::initialize ()
{
	X3DLayerNode::initialize ();

	const auto groupNode   = dynamic_cast <LayoutGroup*> (getGroup ()   .getValue ());
	const auto friendsNode = dynamic_cast <LayoutGroup*> (getFriends () .getValue ());

	layout () .addInterest (groupNode   -> layout ());
	layout () .addInterest (friendsNode -> layout ());

	addChildren ()    .addInterest (groupNode -> addChildren ());
	removeChildren () .addInterest (groupNode -> removeChildren ());
	children ()       .addInterest (groupNode -> children ());

	groupNode   -> layout () = layout ();
	friendsNode -> layout () = layout ();

	groupNode -> setPrivate (true);
	groupNode -> children () = children ();
	groupNode -> setup ();
}

void
LayoutLayer::addTool ()
{
	const auto group = dynamic_cast <LayoutGroup*> (getGroup () .getValue ());

	group -> addTool ();
}

void
LayoutLayer::removeTool (const bool really)
{
	const auto group = dynamic_cast <LayoutGroup*> (getGroup () .getValue ());

	group -> removeTool (really);
}

LayoutLayer::~LayoutLayer ()
{ }

} // X3D
} // titania
