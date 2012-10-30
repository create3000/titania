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
 ******************************************************************************/

#include "X3DScene.h"

#include "../Bits/Error.h"
#include "../Browser/Browser.h"
#include "../Prototype/ExternProto.h"
#include "../Prototype/Proto.h"

#include <iostream>

namespace titania {
namespace X3D {

X3DScene::X3DScene () :
	X3DExecutionContext (),                   
	           layerSet (new LayerSet (this)) 
{
	appendField (initializeOnly, "layerSet", layerSet);
}

void
X3DScene::initialize ()
{
	X3DExecutionContext::initialize ();

	layerSet -> setup ();
}

void
X3DScene::addRootNode (const SFNode <X3DBasicNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DExecutionContext::addRootNode (rootNode);

	SFNode <LayerSet> rootLayersSet = rootNode;

	if (rootLayersSet)
	{
		rootLayersSet -> getLayers () [0] -> children = layerSet -> getLayers () [0] -> children;
		layerSet                                      = rootLayersSet;
	}

	else
		layerSet -> getLayers () [0] -> children .push_back (rootNode);
}

void
X3DScene::removeRootNode (const SFNode <X3DBasicNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

Box3f
X3DScene::getBBox ()
{
	return getLayerSet () -> getBBox ();
}

const SFNode <LayerSet> &
X3DScene::getLayerSet () const
{
	return layerSet;
}

const SFNode <X3DLayerNode>
X3DScene::getActiveLayer () const
{
	return layerSet -> getActiveLayer ();
}

void
X3DScene::display ()
{
	layerSet -> display ();
}

void
X3DScene::dispose ()
{
	std::clog << "\tDisposing X3DScene: " << getWorldURL () << std::endl;

	X3DExecutionContext::dispose ();

	std::clog << "\tDone disposing X3DScene." << std::endl;
}

} // X3D
} // titania
