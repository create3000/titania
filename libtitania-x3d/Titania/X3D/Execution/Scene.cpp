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
 ******************************************************************************/

#include "Scene.h"

#include "../Browser/Browser.h"
#include <iostream>

namespace titania {
namespace X3D {

Scene::Scene (X3DBrowser* const browser) :
	   X3DBaseNode (browser, this),      
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

	addField (initializeOnly, "layerSet", layerSet);

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
Scene::addRootNode (const SFNode <X3DBaseNode> & rootNode)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	X3DScene::addRootNode (rootNode);

	SFNode <LayerSet> rootLayerSet = rootNode;

	if (rootLayerSet)
	{
		rootLayerSet -> getLayers () [0] -> children = layerSet -> getLayers () [0] -> children;
		layerSet                                     = rootLayerSet;
	}

	else
		layerSet -> getLayers () [0] -> children .push_back (rootNode);
}

void
Scene::removeRootNode (const SFNode <X3DBaseNode> & rootNode)
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

// Traveral

void
Scene::pick ()
{
	layerSet -> pick ();
}

void
Scene::display ()
{
	layerSet -> display ();
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

	if (getNavigationInfos () .size ())
		getNavigationInfos () [0] -> set_bind = true;

	if (getBackgrounds () .size ())
		getBackgrounds () [0] -> set_bind = true;

	if (getFogs () .size ())
		getFogs () [0] -> set_bind = true;

	// Bind viewpoint from URL,

	if (getWorldURL () .fragment () .length ())
		getBrowser () -> changeViewpoint (getWorldURL () .fragment ());

	// or bind first viewpoint in viewpoint stack.

	else if (getViewpoints () .size ())
		getViewpoints () [0] -> set_bind = true;
}

// Dispose

void
Scene::clear ()
{
	__LOG__ << getWorldURL () << std::endl;
	
	layerSet .set (new LayerSet (this));
	layerSet -> setup ();

	X3DScene::clear ();
}

void
Scene::dispose ()
{
	__LOG__ << getWorldURL () << std::endl;

	navigationInfos .dispose ();
	backgrounds     .dispose ();
	viewpoints      .dispose ();
	fogs            .dispose ();

	layerSet .dispose ();

	X3DScene::dispose ();

	__LOG__ << std::endl;
}

Scene::~Scene ()
{
	__LOG__ << std::endl;
}

} // X3D
} // titania
