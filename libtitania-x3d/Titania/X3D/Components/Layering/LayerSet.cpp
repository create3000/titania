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

#include "LayerSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/Layer.h"

namespace titania {
namespace X3D {

const std::string LayerSet::componentName  = "Layering";
const std::string LayerSet::typeName       = "LayerSet";
const std::string LayerSet::containerField = "children";

LayerSet::Fields::Fields () :
	activeLayer (new SFInt32 ()),
	order (new MFInt32 ({ 0 })),
	layers (new MFNode ())
{ }

LayerSet::LayerSet (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        X3DNode (),
	         fields (),
	       children ({ new Layer (executionContext) }),
	         layer0 (children [0]),
	activeLayerNode ()
{
	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "activeLayer", activeLayer ());
	addField (inputOutput, "order",       order ());
	addField (inputOutput, "layers",      layers ());

	addChildren (layer0, activeLayerNode);
}

X3DBaseNode*
LayerSet::create (X3DExecutionContext* const executionContext) const
{
	return new LayerSet (executionContext);
}

void
LayerSet::initialize ()
{
	X3DNode::initialize ();

	layer0 -> setInternal (true);
	layer0 -> setup ();
	layer0 -> getBackgroundStack () -> bottom () -> transparency () = 0;

	activeLayer () .addInterest (this, &LayerSet::set_activeLayer);
	layers ()      .addInterest (this, &LayerSet::set_layers);

	set_layers ();
}

void
LayerSet::setLayer0 (const X3DSFNode <X3DLayerNode> & value)
{
	layer0       = value;
	children [0] = value;

	set_activeLayer ();
}

void
LayerSet::set_activeLayer ()
{
	if (activeLayer () >= 0 and activeLayer () < (int32_t) children .size ())
	{
		if (activeLayerNode not_eq children [activeLayer ()])
			activeLayerNode = children [activeLayer ()];
	}
	else
	{
		if (activeLayerNode not_eq nullptr)
			activeLayerNode = nullptr;
	}
}

void
LayerSet::set_layers ()
{
	children .resize (1);

	for (const auto & layer : layers ())
	{
		auto child = x3d_cast <X3DLayerNode*> (layer);

		if (child)
			children .emplace_back (child);
	}

	set_activeLayer ();
}

void
LayerSet::traverse (const TraverseType type)
{
	for (const auto & index : order ())
	{
		if (index >= 0 and index < (int32_t) children .size ())
		{
			children [index] -> traverse (type);
		}
	}
}

void
LayerSet::dispose ()
{
	children .clear ();

	layer0          .dispose ();
	activeLayerNode .dispose ();

	X3DNode::dispose ();
}

} // X3D
} // titania
