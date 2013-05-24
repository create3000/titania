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

#include "Scene.h"

#include "../Bits/Cast.h"
#include "../Browser/X3DBrowser.h"
#include <iostream>

namespace titania {
namespace X3D {

Scene::Scene (X3DBrowser* const browser) :
	X3DBaseNode (browser, this),      
	   X3DScene (),                   
	   layerSet (new LayerSet (this)) 
{
	std::clog << "Constructing Scene:" << std::endl;

	setComponent ("Browser");
	setTypeName ("Scene");

	setChildren (layerSet);

	std::clog << "\tDone constructing Scene." << std::endl;
}

Scene*
Scene::create (X3DExecutionContext* const executionContext) const
{
	return new Scene (executionContext -> getBrowser ());
}

void
Scene::initialize ()
{
	X3DScene::initialize ();

	layerSet -> setup ();
}

// BBox

Box3f
Scene::getBBox ()
{
	return getLayerSet () -> getBBox ();
}

// Root node handling

void
Scene::addRootNode (const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	SFNode <LayerSet> rootLayerSet = x3d_cast <LayerSet*> (rootNode .getValue ());

	if (rootLayerSet)
	{
		rootLayerSet -> getLayers () [0] -> children () = layerSet -> getLayers () [0] -> children ();
		layerSet                                        = rootLayerSet;
	}

	else // remove else and add layer set to layer 0 children, this syncs rootNodes and layer 0 children
		layerSet -> getLayers () [0] -> children () .emplace_back (rootNode);

	X3DScene::addRootNode (rootNode);
}

void
Scene::removeRootNode (const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
//	MFNode & children = layerSet -> getLayers () [0] -> children ();
//
//	std::remove (children .begin (), children .end (), rootNode);
//	
//	// 
//	
//	SFNode <LayerSet> rootLayerSet = x3d_cast <LayerSet*> (rootNode .getValue ());
//	
//	if (rootLayerSet == layerSet)
//		new LayerSet (this);
//
//	X3DScene::removeRootNode (rootNode);
}

void
Scene::setRootNode (size_t index, const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
//	SFNode <LayerSet> rootLayerSet = x3d_cast <LayerSet*> (rootNode .getValue ());
//
//	MFNode & children = layerSet -> getLayers () [0] -> children ();
//
//	if (children .size () <= index)
//		children .resize (index + 1) ;
//		
//	children [index] = rootNode;
//
//	X3DScene::setRootNode (index, rootNode);
}

const SFNode <X3DBaseNode> &
Scene::getRootNode (size_t index)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	return X3DScene::getRootNode (index);
}

// Traveral

void
Scene::traverse (TraverseType type)
{
	layerSet -> traverse (type);
}

// Input/Output

void
Scene::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DScene::fromStream (istream);
}

// Dispose

void
Scene::dispose ()
{
	layerSet .dispose ();

	X3DScene::dispose ();
}

Scene::~Scene ()
{
	//__LOG__ << std::endl;
}

} // X3D
} // titania
