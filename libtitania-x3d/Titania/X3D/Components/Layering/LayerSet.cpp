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

#include "LayerSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Layering/Layer.h"

namespace titania {
namespace X3D {

const Component   LayerSet::component      = Component ("Layering", 1);
const std::string LayerSet::typeName       = "LayerSet";
const std::string LayerSet::containerField = "children";

LayerSet::Fields::Fields () :
	privateActiveLayer (-1),
	       activeLayer (new SFInt32 ()),
	             order (new MFInt32 ({ 0 })),
	            layers (new MFNode ())
{ }

LayerSet::LayerSet (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        X3DNode (),
	         fields (),
	     layerNodes ({ new Layer (executionContext) }),
	     layerNode0 (layerNodes [0]),
	activeLayerNode ()
{
	addType (X3DConstants::LayerSet);

	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "activeLayer", activeLayer ());
	addField (inputOutput, "order",       order ());
	addField (inputOutput, "layers",      layers ());

	addChildObjects (fields .privateActiveLayer, layerNodes, layerNode0, activeLayerNode);
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

	layerNode0 -> setPrivate (true);
	layerNode0 -> setup ();

	privateActiveLayer () .addInterest (&LayerSet::set_activeLayer, this);
	activeLayer ()        .addInterest (&LayerSet::set_activeLayer, this);
	order ()              .addInterest (&LayerSet::set_layers, this);
	layers ()             .addInterest (&LayerSet::set_layers, this);

	set_layers ();
}

int32_t
LayerSet::getActiveLayerIndex () const
{
	if (privateActiveLayer () < 0)
		return activeLayer ();

	return privateActiveLayer ();
}

void
LayerSet::setLayer0 (const X3DPtr <X3DLayerNode> & value)
{
	layerNode0 = value;

	set_layers ();
}

void
LayerSet::set_activeLayer ()
{
	int32_t index = getActiveLayerIndex ();

	if (index == 0)
	{
		if (activeLayerNode not_eq layerNode0)
			activeLayerNode = layerNode0;
	}
	else
	{
		-- index;

		if (index >= 0 and index < (int32_t) layers () .size ())
		{
			if (activeLayerNode not_eq layers () [index])
				activeLayerNode = x3d_cast <X3DLayerNode*> (layers () [index]);
		}
		else
		{
			if (activeLayerNode not_eq nullptr)
				activeLayerNode = nullptr;
		}
	}
}

void
LayerSet::set_layers ()
{
	layerNodes .clear ();
	layerNodes .reserve (layers () .size () + 1);

	for (int32_t index : order ())
	{
		if (index == 0)
			layerNodes .emplace_back (layerNode0);

		else
		{
			-- index;

			if (index >= 0 and index < (int32_t) layers () .size ())
			{
				const auto layerNode = x3d_cast <X3DLayerNode*> (layers () [index]);

				if (layerNode)
					layerNodes .emplace_back (layerNode);
			}
		}
	}

	set_activeLayer ();
}

void
LayerSet::bind (const std::string & viewpointName)
{
	layerNode0 -> bind (viewpointName);

	for (const auto & layer : layers ())
	{
		const auto layerNode = x3d_cast <X3DLayerNode*> (layer);

		if (layerNode)
			layerNode -> bind (viewpointName);
	}
}

void
LayerSet::traverse (const TraverseType type, X3DRenderObject* const renderObject)
{
	if (type == TraverseType::POINTER)
	{
		size_t layerNumber = 0;

		for (const auto & layerNode : layerNodes)
		{
			getBrowser () -> setLayerNumber (layerNumber ++);
			layerNode -> traverse (type, renderObject);
		}
	}
	else
	{
		for (const auto & layerNode : layerNodes)
		{
			layerNode -> traverse (type, renderObject);
		}
	}
}

void
LayerSet::dispose ()
{
	X3DNode::dispose ();
}

LayerSet::~LayerSet ()
{ }

} // X3D
} // titania
