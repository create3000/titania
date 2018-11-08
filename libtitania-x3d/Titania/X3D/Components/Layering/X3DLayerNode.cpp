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

#include "X3DLayerNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../EnvironmentalEffects/Background.h"
#include "../EnvironmentalEffects/Fog.h"
#include "../EnvironmentalEffects/LocalFog.h"
#include "../Grouping/X3DGroupingNode.h"
#include "../Layering/Viewport.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"

#include <Titania/Math/Geometry/Camera.h>

namespace titania {
namespace X3D {

X3DLayerNode::Fields::Fields () :
	    isPickable (new SFBool (true)),
	      viewport (new SFNode ()),
	   addChildren (new MFNode ()),
	removeChildren (new MFNode ()),
	      children (new MFNode ())
{ }

X3DLayerNode::X3DLayerNode (X3DViewpointNode* p_defaultViewpoint, X3DGroupingNode* p_layerGroup) :
	              X3DNode (),
	      X3DRenderObject (),
	               fields (),
	               layer0 (false),
	defaultNavigationInfo (new NavigationInfo (getExecutionContext ())),
	     defaultViewpoint (p_defaultViewpoint),
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
	            groupNode (p_layerGroup),
	          friendsNode (p_layerGroup -> create (getExecutionContext ()))
{
	addType (X3DConstants::X3DLayerNode);

	addChildObjects (defaultNavigationInfo,
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
	                 groupNode,
	                 friendsNode);

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
	X3DRenderObject::initialize ();

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

	groupNode -> setPrivate (true);
	groupNode -> children () = children ();
	groupNode -> setup ();

	friendsNode -> setPrivate (true);
	friendsNode -> setup ();

	viewport ()       .addInterest (&X3DLayerNode::set_viewport, this);
	addChildren ()    .addInterest (groupNode -> addChildren ());
	removeChildren () .addInterest (groupNode -> removeChildren ());
	children ()       .addInterest (groupNode -> children ());

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

	groupNode   -> setExecutionContext (executionContext);
	friendsNode -> setExecutionContext (executionContext);

	X3DRenderObject::setExecutionContext (executionContext);
	X3DNode::setExecutionContext (executionContext);

	if (isInitialized ())
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
	return groupNode -> getBBox ();
}

NavigationInfo*
X3DLayerNode::getNavigationInfo () const
{
	return navigationInfoStack -> getTop ();
}

X3DViewpointNode*
X3DLayerNode::getViewpoint () const
{
	return viewpointStack -> getTop ();
}

X3DBackgroundNode*
X3DLayerNode::getBackground () const
{
	return backgroundStack -> getTop ();
}

X3DFogObject*
X3DLayerNode::getFog () const
{
	return fogStack -> getTop ();
}

UserViewpointList
X3DLayerNode::getUserViewpoints () const
{
	UserViewpointList userViewpoints;

	for (const auto & viewpoint : getViewpoints () -> getList ())
	{
		if (not viewpoint -> description () .empty ())
			userViewpoints .emplace_back (viewpoint);
	}

	return userViewpoints;
}

void
X3DLayerNode::lookAt (const double factor, const bool straighten, const time_type cycleInterval)
{
	getViewpoint () -> lookAt (getBBox (), factor, straighten, cycleInterval);
}

void
X3DLayerNode::set_viewport ()
{
	currentViewport .set (x3d_cast <X3DViewportNode*> (viewport ()));

	if (not currentViewport)
		currentViewport .set (getBrowser () -> getDefaultViewport ());
}

void
X3DLayerNode::bind (const std::string & viewpointName)
{
	traverse (TraverseType::CAMERA, this);

	if (not getNavigationInfos () -> getList () .empty ())
	{
		const auto navigationInfo = getNavigationInfos () -> getBound ();
		getNavigationInfoStack () -> pushOnTop (navigationInfo, true);
		navigationInfo -> addLayer (this);
	}

	if (not getViewpoints () -> getList () .empty ())
	{
		const auto viewpoint = getViewpoints () -> getBound (viewpointName);
		getViewpointStack () -> pushOnTop (viewpoint, true);
		viewpoint -> addLayer (this);
		viewpoint -> resetUserOffsets ();
	}

	if (not getBackgrounds () -> getList () .empty ())
	{
		const auto background = getBackgrounds () -> getBound ();
		getBackgroundStack () -> pushOnTop (background, true);
		background -> addLayer (this);
	}

	if (not getFogs () -> getList () .empty ())
	{
		const auto fog = getFogs () -> getBound ();
		getFogStack () -> pushOnTop (fog, true);
		fog -> addLayer (this);
	}
}

void
X3DLayerNode::traverse (const TraverseType type, X3DRenderObject* const p_renderObject)
{
	const auto renderObject = p_renderObject ? p_renderObject : this;

	getCameraSpaceMatrix        () .push (getViewpoint () -> getCameraSpaceMatrix ());
	getInverseCameraSpaceMatrix () .push (getViewpoint () -> getInverseCameraSpaceMatrix ());
	getProjectionMatrix         () .push (getViewpoint () -> getProjectionMatrix (renderObject));

	switch (type)
	{
		case TraverseType::POINTER:
		{
			pointer (type, renderObject);
			break;
		}
		case TraverseType::CAMERA:
		{
			camera (type, renderObject);
			break;
		}
		case TraverseType::COLLISION:
		{
			collision (type, renderObject);
			break;
		}
		case TraverseType::DEPTH:
		{
			display (type, renderObject);
			break;
		}
		case TraverseType::DISPLAY:
		{
			display (type, renderObject);
			break;
		}
	}

	getProjectionMatrix         () .pop ();
	getInverseCameraSpaceMatrix () .pop ();
	getCameraSpaceMatrix        () .pop ();
}

void
X3DLayerNode::pointer (const TraverseType type, X3DRenderObject* const renderObject)
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
			if (not getBrowser () -> isPointerInRectangle (currentViewport -> getRectangle (getBrowser ())))
				return;
		}

		getBrowser () -> setHitRay (getProjectionMatrix () .get (), currentViewport -> getRectangle (getBrowser ()));

		getModelViewMatrix () .push (getInverseCameraSpaceMatrix () .get ());

		currentViewport -> push (this);
		collect (type, renderObject);
		currentViewport -> pop (this);

		getModelViewMatrix () .pop ();
	}
}

void
X3DLayerNode::camera (const TraverseType type, X3DRenderObject* const renderObject)
{
	getModelViewMatrix () .push (Matrix4d ());

	defaultNavigationInfo -> traverse (TraverseType::CAMERA, renderObject);
	defaultViewpoint      -> traverse (TraverseType::CAMERA, renderObject);
	defaultBackground     -> traverse (TraverseType::CAMERA, renderObject);
	defaultFog            -> traverse (TraverseType::CAMERA, renderObject);

	currentViewport -> push (this);
	collect (type, renderObject);
	currentViewport -> pop (this);

	navigationInfos -> update ();
	viewpoints      -> update ();
	backgrounds     -> update ();
	fogs            -> update ();

	getModelViewMatrix () .pop ();
}

void
X3DLayerNode::collision (const TraverseType type, X3DRenderObject* const renderObject)
{
	using namespace std::placeholders;

	const auto collisionRadius  = getNavigationInfo () -> getCollisionRadius ();
	const auto avatarHeight     = getNavigationInfo () -> getAvatarHeight ();
	const auto size             = std::max (collisionRadius * 2, avatarHeight * 2);
	const auto projectionMatrix = math::camera <double>::ortho (-size, size, -size, size, -size, size);

	getProjectionMatrix () .push (projectionMatrix);
	getModelViewMatrix  () .push (getViewpoint () -> getInverseCameraSpaceMatrix ()); // !!! Must be from viewpoint.

	// Render
	currentViewport -> push (this);
	renderObject -> render (type, std::bind (&X3DLayerNode::collect, this, _1, _2));
	currentViewport -> pop (this);

	getModelViewMatrix  () .pop ();
	getProjectionMatrix () .pop ();
}

void
X3DLayerNode::display (const TraverseType type, X3DRenderObject* const renderObject)
{
	using namespace std::placeholders;

	getNavigationInfo () -> enable (type, renderObject);
	getModelViewMatrix () .push (getInverseCameraSpaceMatrix () .get ());

	currentViewport -> push (this);
	renderObject -> render (type, std::bind (&X3DLayerNode::collect, this, _1, _2));
	currentViewport -> pop (this);

	getModelViewMatrix () .pop ();
	getNavigationInfo () -> disable ();
}

void
X3DLayerNode::collect (const TraverseType type, X3DRenderObject* const renderObject)
{
	groupNode   -> traverse (type, renderObject);
	friendsNode -> traverse (type, renderObject);
}

void
X3DLayerNode::dispose ()
{
	X3DNode::dispose ();
	X3DRenderObject::dispose ();
}

X3DLayerNode::~X3DLayerNode ()
{ }

} // X3D
} // titania
