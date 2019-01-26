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

#include "World.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"

#include "../Components/Layering/Layer.h"
#include "../Components/Layering/LayerSet.h"
#include "../Routing/Router.h"

namespace titania {
namespace X3D {

const Component   World::component      = Component ("Titania", 1);
const std::string World::typeName       = "World";
const std::string World::containerField = "world";

World::World (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	        layerSet (new LayerSet (executionContext)),
	 defaultLayerSet (layerSet),
	          layer0 (new Layer (executionContext)),
	     activeLayer (layer0)
{
	addType (X3DConstants::World);

	addChildObjects (layerSet, defaultLayerSet, layer0, activeLayer);
}

World*
World::create (X3DExecutionContext* const executionContext) const
{
	return new World (executionContext);
}

void
World::initialize ()
{
	X3DBaseNode::initialize ();

	layerSet -> setPrivate (true);
	layerSet -> setup ();
	layerSet -> setLayer0 (layer0);
	layerSet -> getActiveLayer () .addInterest (&World::set_activeLayer, this);

	getExecutionContext () -> getRootNodes () .addInterest (&World::set_rootNodes, this);

	set_rootNodes (); // This can happen twice when rootNodes is tainted

	layer0 -> isLayer0 (true);
	layer0 -> setPrivate (true);
	layer0 -> setup ();
}

void
World::set_rootNodes ()
{
	const X3DPtr <LayerSet> oldLayerSet = layerSet;

	layerSet = defaultLayerSet;

	layer0 -> children () = getExecutionContext () -> getRootNodes ();

	for (const auto & rootNode : getExecutionContext () -> getRootNodes ())
	{
		LayerSet* const rootLayerSet = x3d_cast <LayerSet*> (rootNode);

		if (rootLayerSet)
		{
			rootLayerSet -> setLayer0 (layer0);
			layerSet = rootLayerSet;
		}
	}

	if (layerSet not_eq oldLayerSet)
	{
		oldLayerSet -> getActiveLayer () .removeInterest (&World::set_activeLayer, this);
		layerSet    -> getActiveLayer () .addInterest (&World::set_activeLayer, this);

		set_activeLayer ();
	}
}

void
World::set_activeLayer ()
{
	activeLayer = layerSet -> getActiveLayer ();
}

///  Bind first viewpoint and other bindables found.
void
World::bind ()
{
	// Bind first X3DBindableNodes found in each layer.

	layerSet -> bind (getExecutionContext () -> getWorldURL () .fragment ());
}

World::~World ()
{ }

} // X3D
} // titania
