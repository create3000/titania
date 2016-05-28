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

#include "../../Browser/Core/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../EnvironmentalEffects/Background.h"
#include "../Layering/Viewport.h"

namespace titania {
namespace X3D {

X3DLayerNode::Fields::Fields () :
	    isPickable (new SFBool (true)),
	      viewport (new SFNode ()),
	   addChildren (new MFNode ()),
	removeChildren (new MFNode ()),
	      children (new MFNode ())
{ }

X3DLayerNode::X3DLayerNode (X3DViewpointNode* defaultViewpoint_, X3DGroupingNode* layerGroup_) :
	              X3DNode (),
	          X3DRenderer (),
	               fields (),
	               layer0 (false),
	defaultNavigationInfo (new NavigationInfo (getExecutionContext ())),
	     defaultViewpoint (defaultViewpoint_),
	    defaultBackground (new Background (getExecutionContext ())),
	           defaultFog (new Fog (getExecutionContext ())),
	      currentViewport (),
	  navigationInfoStack (new NavigationInfoStack (getExecutionContext (), defaultNavigationInfo)),
	       viewpointStack (new ViewpointStack (getExecutionContext (), defaultViewpoint)),
	      backgroundStack (new BackgroundStack (getExecutionContext (), defaultBackground)),
	             fogStack (new FogStack (getExecutionContext (), defaultFog)),
	      navigationInfos (new NavigationInfoList (getExecutionContext ())),
	           viewpoints (new ViewpointList (getExecutionContext ())),
	          backgrounds (new BackgroundList (getExecutionContext ())),
	                 fogs (new FogList (getExecutionContext ())),
	            localFogs (),
	               hitRay (),
	                group (layerGroup_),
	              friends (layerGroup_ -> create (getExecutionContext ()))
{
	addType (X3DConstants::X3DLayerNode);

	addChildren (defaultNavigationInfo,
	             defaultViewpoint,
	             defaultBackground,
	             defaultFog,
	             currentViewport,
	             navigationInfoStack,
	             viewpointStack,
	             backgroundStack,
	             fogStack,
	             navigationInfos,
	             viewpoints,
	             backgrounds,
	             fogs,
	             group,
	             friends);

	defaultNavigationInfo -> isBound () = true;
	defaultViewpoint      -> isBound () = true;
	defaultBackground     -> isBound () = true;
	defaultFog            -> isBound () = true;

	defaultBackground -> isHidden (true);
	defaultFog        -> isHidden (true);
}

void
X3DLayerNode::initialize ()
{
	X3DNode::initialize ();
	X3DRenderer::initialize ();

	defaultNavigationInfo -> setup ();
	defaultViewpoint      -> setup ();
	defaultBackground     -> setup ();
	defaultFog            -> setup ();

	navigationInfoStack -> setup ();
	viewpointStack      -> setup ();
	backgroundStack     -> setup ();
	fogStack            -> setup ();

	navigationInfos -> setup ();
	viewpoints      -> setup ();
	backgrounds     -> setup ();
	fogs            -> setup ();

	group -> isPrivate (true);
	group -> children () = children ();
	group -> setup ();

	friends -> isPrivate (true);
	friends -> setup ();

	viewport ()       .addInterest (this, &X3DLayerNode::set_viewport);
	addChildren ()    .addInterest (group -> addChildren ());
	removeChildren () .addInterest (group -> removeChildren ());
	children ()       .addInterest (group -> children ());

	set_viewport ();
}

void
X3DLayerNode::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	defaultNavigationInfo -> setExecutionContext (executionContext);
	defaultViewpoint      -> setExecutionContext (executionContext);
	defaultBackground     -> setExecutionContext (executionContext);
	defaultFog            -> setExecutionContext (executionContext);

	navigationInfoStack -> setExecutionContext (executionContext);
	viewpointStack      -> setExecutionContext (executionContext);
	backgroundStack     -> setExecutionContext (executionContext);
	fogStack            -> setExecutionContext (executionContext);

	navigationInfos -> setExecutionContext (executionContext);
	viewpoints      -> setExecutionContext (executionContext);
	backgrounds     -> setExecutionContext (executionContext);
	fogs            -> setExecutionContext (executionContext);

	group   -> setExecutionContext (executionContext);
	friends -> setExecutionContext (executionContext);

	X3DRenderer::setExecutionContext (executionContext);
	X3DNode::setExecutionContext (executionContext);

	set_viewport ();
}

void
X3DLayerNode::isLayer0 (const bool value)
{
	layer0 = value;

	defaultBackground -> isHidden (not value);
}

Box3d
X3DLayerNode::getBBox () const
{
	return group -> getBBox ();
}

NavigationInfo*
X3DLayerNode::getNavigationInfo () const
{
	return navigationInfoStack -> top ();
}

X3DViewpointNode*
X3DLayerNode::getViewpoint () const
{
	return viewpointStack -> top ();
}

X3DBackgroundNode*
X3DLayerNode::getBackground () const
{
	return backgroundStack -> top ();
}

X3DFogObject*
X3DLayerNode::getFog () const
{
	if (localFogs .empty ())
		return fogStack -> top ();

	return localFogs .top ();
}

UserViewpointList
X3DLayerNode::getUserViewpoints () const
{
	UserViewpointList userViewpoints;

	for (const auto & viewpoint : *getViewpoints ())
	{
		if (not viewpoint -> description () .empty ())
			userViewpoints .emplace_back (viewpoint);
	}

	return userViewpoints;
}

void
X3DLayerNode::lookAt ()
{
	getViewpoint () -> lookAt (getBBox ());
}

void
X3DLayerNode::set_viewport ()
{
	currentViewport .set (x3d_cast <X3DViewportNode*> (viewport ()));

	if (not currentViewport)
		currentViewport .set (getBrowser () -> getViewport ());
}

void
X3DLayerNode::bind ()
{
	traverse (TraverseType::CAMERA);

	if (not getNavigationInfos () -> empty ())
	{
		const auto navigationInfo = getNavigationInfos () -> bound ();
		getNavigationInfoStack () -> force_push (navigationInfo);
		navigationInfo -> addLayer (this);
	}

	if (not getViewpoints () -> empty ())
	{
		const auto viewpoint = getViewpoints () -> bound ();
		getViewpointStack () -> force_push (viewpoint);
		viewpoint -> addLayer (this);
	}

	if (not getBackgrounds () -> empty ())
	{
		const auto background = getBackgrounds () -> bound ();
		getBackgroundStack () -> force_push (background);
		background -> addLayer (this);
	}

	if (not getFogs () -> empty ())
	{
		const auto fog = getFogs () -> bound ();
		getFogStack () -> force_push (fog);
		fog -> addLayer (this);
	}
}

void
X3DLayerNode::traverse (const TraverseType type)
{
	getBrowser () -> getLayers () .push (this);

	currentViewport -> enable ();

	switch (type)
	{
		case TraverseType::POINTER:
		{
			pointer ();
			break;
		}
		case TraverseType::CAMERA:
		{
			camera ();
			break;
		}
		case TraverseType::COLLISION:
		{
			collision ();
			break;
		}
		case TraverseType::DISPLAY:
		{
			display (type);
			break;
		}
		case TraverseType::DEPTH:
		{
			display (type);
			break;
		}
	}

	currentViewport -> disable ();

	getBrowser () -> getLayers () .pop ();
}

void
X3DLayerNode::pointer ()
{
	if (isPickable ())
	{
		if (getBrowser () -> getSelectedLayer ())
		{
			if (getBrowser () -> getSelectedLayer () not_eq this)
				return;
		}
		else
		{
			if (not getBrowser () -> isPointerInRectangle (currentViewport -> getRectangle ()))
				return;
		}

		getModelViewMatrix () .identity ();
		getViewpoint () -> reshape ();
		getBrowser ()   -> setHitRay (getModelViewMatrix () .get (), getBrowser () -> getProjectionMatrix (), currentViewport -> getRectangle ());
		getViewpoint () -> transform ();

		currentViewport -> push ();

		collect (TraverseType::POINTER);

		currentViewport -> pop ();

		getGlobalObjects () .clear ();
	}
}

void
X3DLayerNode::camera ()
{
	getModelViewMatrix () .identity ();
	getViewpoint () -> reshape ();

	defaultNavigationInfo -> traverse (TraverseType::CAMERA);
	defaultViewpoint      -> traverse (TraverseType::CAMERA);
	defaultBackground     -> traverse (TraverseType::CAMERA);
	defaultFog            -> traverse (TraverseType::CAMERA);

	currentViewport -> push ();
	collect (TraverseType::CAMERA);
	currentViewport -> pop ();

	navigationInfos -> update ();
	viewpoints      -> update ();
	backgrounds     -> update ();
	fogs            -> update ();
}

void
X3DLayerNode::collision ()
{
	getViewpoint () -> reshape ();
	getModelViewMatrix () .identity ();

	// Render
	currentViewport -> push ();
	render (TraverseType::COLLISION);
	currentViewport -> pop ();
}

void
X3DLayerNode::display (const TraverseType type)
{
	glClear (GL_DEPTH_BUFFER_BIT);

	getModelViewMatrix () .identity ();
	getBackground () -> draw ();

	getNavigationInfo () -> enable ();
	getViewpoint ()      -> reshape ();
	getViewpoint ()      -> transform ();

	currentViewport -> push ();
	render (type);
	currentViewport -> pop ();

	getNavigationInfo () -> disable ();
}

void
X3DLayerNode::collect (const TraverseType type)
{
	group   -> traverse (type);
	friends -> traverse (type);
}

void
X3DLayerNode::dispose ()
{
	X3DNode::dispose ();
	X3DRenderer::dispose ();
}

} // X3D
} // titania
