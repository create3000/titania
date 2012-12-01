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

#include "Scene.h"

#include "../Browser/Browser.h"
#include <iostream>

namespace titania {
namespace X3D {

Scene::Scene (X3DBrowser* const browser) :
	   X3DBasicNode (browser, this),      
	       X3DScene (),                   
	navigationInfos (),                   
	    backgrounds (),                   
	           fogs (),                   
	     viewpoints (),                   
	       layerSet (new LayerSet (this)) 
{
	std::clog << "Constructing Scene:" << std::endl;

	setComponent ("Browser");
	setTypeName ("Scene");

	appendField (initializeOnly, "layerSet", layerSet);

	std::clog << "\tDone constructing Scene." << std::endl;
}

Scene*
Scene::create (const basic::uri & worldURL, std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	Scene* scene = create (this);

	scene -> setup ();
	scene -> fromStream (worldURL, istream);

	return scene;
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

// NavigationInfo list handling

void
Scene::addNavigationInfo (NavigationInfo* const navigationInfo)
{
	navigationInfos .push_back (navigationInfo);
}

void
Scene::removeNavigationInfo (NavigationInfo* const navigationInfo)
{
	navigationInfos .erase (navigationInfo);
}

const NavigationInfoList &
Scene::getNavigationInfos () const
{
	return navigationInfos;
}

// Background list handling

void
Scene::addBackground (X3DBackgroundNode* const background)
{
	backgrounds .push_back (background);
}

void
Scene::removeBackground (X3DBackgroundNode* const background)
{
	backgrounds .erase (background);
}

const BackgroundList &
Scene::getBackgrounds () const
{
	return backgrounds;
}

// Fog list handling

void
Scene::addFog (Fog* const fog)
{
	fogs .push_back (fog);
}

void
Scene::removeFog (Fog* const fog)
{
	fogs .erase (fog);
}

const FogList &
Scene::getFogs () const
{
	return fogs;
}

// Viewpoint list handling

void
Scene::addViewpoint (X3DViewpointNode* const viewpoint)
{
	viewpoints .push_back (viewpoint);
}

void
Scene::removeViewpoint (X3DViewpointNode* const viewpoint)
{
	viewpoints .erase (viewpoint);
}

const ViewpointList &
Scene::getViewpoints () const
{
	return viewpoints;
}

// Root node handling

void
Scene::addRootNode (const SFNode <X3DBasicNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DScene::addRootNode (rootNode);

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
Scene::removeRootNode (const SFNode <X3DBasicNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

// Layer handling

const SFNode <LayerSet> &
Scene::getLayerSet () const
{
	return layerSet;
}

const SFNode <X3DLayerNode>
Scene::getActiveLayer () const
{
	return layerSet -> getActiveLayer ();
}

// Display

void
Scene::display ()
{
	layerSet -> display ();
}

// Dispose

void
Scene::clear ()
{
	layerSet .set (new LayerSet (this));
	layerSet -> setup ();

	X3DScene::clear ();
}

void
Scene::dispose ()
{
	//std::clog << "\tDisposing Scene: " << getWorldURL () << std::endl;

	navigationInfos .dispose ();
	backgrounds     .dispose ();
	viewpoints      .dispose ();
	fogs            .dispose ();

	X3DScene::dispose ();

	//std::clog << "\tDone disposing Scene." << std::endl;
}

} // X3D
} // titania
