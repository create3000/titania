/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstraﬂe 31a, Leipzig, Germany 2011.
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

#include "World.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include "../Components/Layering/Layer.h"
#include "../Execution/BindableNodeList.h"
#include "../Execution/BindableNodeStack.h"

namespace titania {
namespace X3D {

World::World (const SFNode <Scene> & _scene) :
	    X3DBaseNode (_scene -> getBrowser (), _scene),
	          scene (_scene),
	       layerSet (new LayerSet (_scene)),
	defaultLayerSet (layerSet),
	         layer0 (new Layer (_scene)),
	    activeLayer (layer0)
{
	setComponent ("Browser");
	setTypeName ("World");

	addChildren (scene, layerSet, defaultLayerSet, layer0, activeLayer);
}

World*
World::create (X3DExecutionContext* const executionContext) const
{
	return new World (scene);
}

void
World::initialize ()
{
	X3DBaseNode::initialize ();

	layerSet -> setup ();
	layerSet -> setLayer0 (layer0);
	layerSet -> getActiveLayer () .addInterest (this, &World::set_activeLayer);

	scene -> getRootNodes () .addInterest (this, &World::set_rootNodes);

	set_rootNodes (); // This can happen twice when rootNodes is tainted

	layer0 -> setup ();
	layer0 -> getBackgroundStack () -> bottom () -> transparency () = 0;
}

void
World::set_rootNodes ()
{
	SFNode <LayerSet> oldLayerSet = layerSet;
	layerSet = defaultLayerSet;

	layer0 -> children () = scene -> getRootNodes ();

	for (const auto & rootNode : scene -> getRootNodes ())
	{
		auto rootLayerSet = x3d_cast <LayerSet*> (rootNode);

		if (rootLayerSet)
		{
			rootLayerSet -> setLayer0 (layer0);
			layerSet = rootLayerSet;
		}
	}

	if (layerSet not_eq oldLayerSet)
	{
		oldLayerSet -> getActiveLayer () .removeInterest (this, &World::set_activeLayer);
		layerSet    -> getActiveLayer () .addInterest    (this, &World::set_activeLayer);

		set_activeLayer ();
	}
}

void
World::set_activeLayer ()
{
	activeLayer = layerSet -> getActiveLayer ();
}

void
World::bind ()
{
	layerSet -> traverse (TraverseType::CAMERA);
	layerSet -> traverse (TraverseType::COLLECT);

	for (auto & layer : layerSet -> getLayers ())
	{
		if (layer -> getNavigationInfos () -> size ())
			layer -> getNavigationInfos () -> at (0) -> set_bind () = true;

		if (layer -> getBackgrounds () -> size ())
			layer -> getBackgrounds () -> at (0) -> set_bind () = true;

		if (layer -> getFogs () -> size ())
			layer -> getFogs () -> at (0) -> set_bind () = true;

		// Bind first viewpoint in viewpoint stack.

		if (layer -> getViewpoints () -> size ())
			layer -> getViewpoints () -> at (0) -> set_bind () = true;
	}

	// Bind viewpoint from URL.

	if (scene -> getWorldURL () .fragment () .length ())
		scene -> changeViewpoint (scene -> getWorldURL () .fragment ());
}

// Dispose

void
World::dispose ()
{
	scene           .dispose ();
	layerSet        .dispose ();
	defaultLayerSet .dispose ();
	layer0          .dispose ();
	activeLayer     .dispose ();

	X3DBaseNode::dispose ();
}

World::~World ()
{ }

} // X3D
} // titania
