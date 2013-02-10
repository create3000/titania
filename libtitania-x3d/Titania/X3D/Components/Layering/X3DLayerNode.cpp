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

#include "X3DLayerNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../EnvironmentalEffects/Background.h"

namespace titania {
namespace X3D {

X3DLayerNode::X3DLayerNode () :
	              X3DNode (),                                                   
	          X3DRenderer (),                                                   
	           isPickable (true),                                               // SFBool [in,out] isPickable      TRUE
	             viewport (),                                                   // SFNode [in,out] viewport        NULL      [X3DViewportNode]
	          addChildren (),                                                   // MFNode[in]      addChildren               [X3DChildNode]
	       removeChildren (),                                                   // MFNode[in]      removeChildren            [X3DChildNode]
	             children (),                                                   // MFNode[in,out]  children        [ ]       [X3DChildNode]
	      defaultViewport (new Viewport (getExecutionContext ())),              
	defaultNavigationInfo (new NavigationInfo (getExecutionContext (), false)), 
	    defaultBackground (new Background     (getExecutionContext (), false)), 
	           defaultFog (new Fog            (getExecutionContext (), false)), 
	     defaultViewpoint (new Viewpoint      (getExecutionContext (), false)), 
	      currentViewport (*defaultViewport),                                   
	  navigationInfoStack (*defaultNavigationInfo),                             
	      backgroundStack (*defaultBackground),                                 
	             fogStack (*defaultFog),                                        
	       viewpointStack (*defaultViewpoint),                                  
	      navigationInfos (),                                                   
	          backgrounds (),                                                   
	                 fogs (),                                                   
	           viewpoints (),                                                   
	            localFogs (),                                                   
	          localLights (),                                                   
	    cachedLocalLights (),                                                   
	         globalLights (),                                                   
	                group (new Group (getExecutionContext ()))                  
{
	addNodeType (X3DConstants::X3DLayerNode);

	setChildren (defaultViewport,
	             defaultNavigationInfo,
	             defaultBackground,
	             defaultFog,
	             defaultViewpoint,
	             group);

	defaultViewpoint -> description = "Default Viewpoint";
}

void
X3DLayerNode::initialize ()
{
	X3DNode::initialize ();

	defaultViewport       -> setup ();
	defaultNavigationInfo -> setup ();
	defaultBackground     -> setup ();
	defaultFog            -> setup ();
	defaultViewpoint      -> setup ();

	defaultBackground -> transparency = 1;
	defaultFog        -> transparency = 1;
	defaultViewpoint  -> isBound      = true;

	viewport .addInterest (this, &X3DLayerNode::set_viewport);

	addChildren    .addInterest (group -> addChildren);
	removeChildren .addInterest (group -> removeChildren);
	children       .addInterest (group -> children);

	set_viewport ();

	group -> children = children;
	group -> setup ();
}

Box3f
X3DLayerNode::getBBox ()
{
	return group -> getBBox ();
}

NavigationInfo*
X3DLayerNode::getNavigationInfo ()
{
	return navigationInfoStack .top ();
}

X3DBackgroundNode*
X3DLayerNode::getBackground ()
{
	return backgroundStack .top ();
}

X3DFogObject*
X3DLayerNode::getFog ()
{
	if (localFogs .size ())
		return localFogs .top ();

	return fogStack .top ();
}

X3DViewpointNode*
X3DLayerNode::getViewpoint ()
{
	return viewpointStack .top ();
}

void
X3DLayerNode::pushLocalLight (X3DLightNode* light)
{
	LightContainer* lightContainer = new LightContainer (light);

	localLights .push_back (lightContainer);
	cachedLocalLights .push_back (lightContainer);
}

void
X3DLayerNode::clearLights ()
{
	for (const auto & light : cachedLocalLights)
		delete light;

	cachedLocalLights .clear ();

	for (const auto & light : globalLights)
		delete light;

	globalLights .clear ();
}

void
X3DLayerNode::lookAt ()
{
	getViewpoint () -> lookAt (getBBox ());
}

void
X3DLayerNode::set_viewport ()
{
	currentViewport = x3d_cast <X3DViewportNode*> (viewport .getValue ());

	if (not currentViewport)
		currentViewport = defaultViewport .getValue ();
}

void
X3DLayerNode::pick ()
{
	if (not isPickable)
		return;

	glPushMatrix ();
	glLoadIdentity ();

	currentViewport -> enable ();
	getViewpoint () -> reshape ();
	getViewpoint () -> transform ();

	group -> pick ();

	currentViewport -> disable ();

	glPopMatrix ();
}

void
X3DLayerNode::traverse ()
{
	glPushMatrix ();
	glLoadIdentity ();

	//defaultViewpoint -> traverse ();
	group -> traverse ();

	glPopMatrix ();
}

void
X3DLayerNode::display ()
{
	currentViewport -> enable ();

	glClear (GL_DEPTH_BUFFER_BIT);

	getBackground ()     -> draw ();
	getNavigationInfo () -> enable ();
	getViewpoint ()      -> reshape ();
	defaultViewpoint     -> display ();

	render ();

	getNavigationInfo () -> disable ();
	currentViewport      -> disable ();
	clearLights ();
}

void
X3DLayerNode::collect ()
{
	group -> display ();
}

void
X3DLayerNode::dispose ()
{
	__LOG__ << (void*) this << std::endl;

	defaultViewport       .dispose ();
	defaultNavigationInfo .dispose ();
	defaultBackground     .dispose ();
	defaultFog            .dispose ();
	defaultViewpoint      .dispose ();

	navigationInfoStack .dispose ();
	backgroundStack     .dispose ();
	fogStack            .dispose ();
	viewpointStack      .dispose ();

	navigationInfos .dispose ();
	backgrounds     .dispose ();
	viewpoints      .dispose ();
	fogs            .dispose ();

	group .dispose ();

	// Dont't dispose stack nodes, they were automatically disposed.

	X3DNode::dispose ();
}

} // X3D
} // titania
