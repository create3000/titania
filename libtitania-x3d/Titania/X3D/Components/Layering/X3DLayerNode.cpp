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
#include "../../Execution/BindableNodeList.h"
#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../EnvironmentalEffects/Background.h"
#include "../Layering/Viewport.h"

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

X3DLayerNode::X3DLayerNode (X3DViewpointNode* defaultViewpoint_, X3DGroupingNode* layerGroup_) :
	              X3DNode (),
	          X3DRenderer (),
	               fields (),
	               layer0 (false),
	defaultNavigationInfo (new NavigationInfo (getExecutionContext ())),
	    defaultBackground (new Background (getExecutionContext ())),
	           defaultFog (new Fog (getExecutionContext ())),
	     defaultViewpoint (defaultViewpoint_),
	      currentViewport (),
	  navigationInfoStack (new NavigationInfoStack (getExecutionContext (), defaultNavigationInfo)),
	      backgroundStack (new BackgroundStack (getExecutionContext (), defaultBackground)),
	             fogStack (new FogStack (getExecutionContext (), defaultFog)),
	       viewpointStack (new ViewpointStack (getExecutionContext (), defaultViewpoint)),
	      navigationInfos (new NavigationInfoList (getExecutionContext ())),
	          backgrounds (new BackgroundList (getExecutionContext ())),
	                 fogs (new FogList (getExecutionContext ())),
	           viewpoints (new ViewpointList (getExecutionContext ())),
	            localFogs (),
	                group (layerGroup_),
	              friends ()
{
	addType (X3DConstants::X3DLayerNode);

	addChildren (defaultNavigationInfo,
	             defaultBackground,
	             defaultFog,
	             defaultViewpoint,
	             currentViewport,
	             navigationInfoStack,
	             backgroundStack,
	             fogStack,
	             viewpointStack,
	             navigationInfos,
	             backgrounds,
	             fogs,
	             viewpoints,
	             group,
	             friends);

	defaultBackground -> isHidden (true);
	defaultFog        -> isHidden (true);
}

void
X3DLayerNode::initialize ()
{
	X3DNode::initialize ();
	X3DRenderer::initialize ();

	defaultNavigationInfo -> setup ();
	defaultBackground     -> setup ();
	defaultFog            -> setup ();
	defaultViewpoint      -> setup ();

	navigationInfoStack -> setup ();
	backgroundStack     -> setup ();
	viewpointStack      -> setup ();
	fogStack            -> setup ();

	navigationInfos -> setup ();
	backgrounds     -> setup ();
	viewpoints      -> setup ();
	fogs            -> setup ();

	group -> isPrivate (true);
	group -> children () = children ();
	group -> setup ();

	viewport ()       .addInterest (this, &X3DLayerNode::set_viewport);
	addChildren ()    .addInterest (group -> addChildren ());
	removeChildren () .addInterest (group -> removeChildren ());
	children ()       .addInterest (group -> children ());

	set_viewport ();
}

void
X3DLayerNode::isLayer0 (const bool value)
{
	layer0 = value;

	defaultBackground -> isHidden (not value);
}

Box3f
X3DLayerNode::getBBox () const
{
	return group -> getBBox ();
}

Vector3f
X3DLayerNode::getTranslation (const Vector3f & positionOffset, const float width, const float height, const Vector3f & translation)
{
	float       distance = getDistance (positionOffset, width, height, translation);
	const float length   = math::abs (translation);

	const float zFar            = getNavigationInfo () -> getFarPlane (getViewpoint ());
	const float collisionRadius = getNavigationInfo () -> getCollisionRadius ();

	if (zFar - distance > 0) // Are there polygons under the viewer
	{
		distance -= collisionRadius;

		if (distance > 0)
		{
			// Move

			if (length > distance)
			{
				// Collision: The wall is reached.
				return normalize (translation) * distance;
			}

			else
				return translation;
		}
		else
		{
			// Collision
			return Vector3f ();
		}
	}
	else
		return translation;
}

float
X3DLayerNode::getDistance (const Vector3f & positionOffset, const float width, const float height, const Vector3f & direction)
{
	try
	{
		const double width1_2  = width / 2;
		const double height1_2 = height / 2;

		const double zNear = getNavigationInfo () -> getNearPlane ();
		const double zFar  = getNavigationInfo () -> getFarPlane (getViewpoint ());

		// Reshape camera

		glMatrixMode (GL_PROJECTION);
		glLoadMatrixd (ortho (-width1_2, width1_2, -height1_2, height1_2, zNear, zFar) .data ());
		glMatrixMode (GL_MODELVIEW);

		// Translate camera

		Rotation4f localOrientation = ~getViewpoint () -> orientation () * getViewpoint () -> getOrientation ();
		Matrix4f   modelViewMatrix  = getViewpoint () -> getParentMatrix ();

		modelViewMatrix .translate (getViewpoint () -> getUserPosition () + positionOffset);
		modelViewMatrix .rotate (Rotation4f (Vector3f (0, 0, 1), -direction) * localOrientation);
		modelViewMatrix .inverse ();

		getModelViewMatrix () .set (modelViewMatrix);

		// Traverse and get distance

		traverse (TraverseType::NAVIGATION);
	}
	catch (const std::domain_error &)
	{ }

	return X3DRenderer::getDistance ();
}

NavigationInfo*
X3DLayerNode::getNavigationInfo () const
{
	return navigationInfoStack -> top ();
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

X3DViewpointNode*
X3DLayerNode::getViewpoint () const
{
	return viewpointStack -> top ();
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

	// Bind first viewpoint in viewpoint stack.

	if (not getViewpoints () -> empty ())
	{
		const auto viewpoint = getViewpoints () -> bound ();
		getViewpointStack () -> force_push (viewpoint);
		viewpoint -> addLayer (this);
	}
}

void
X3DLayerNode::traverse (const TraverseType type)
{
	getBrowser () -> getLayers () .push (this);

	currentViewport -> enable ();

	switch (type)
	{
		case TraverseType::PICKING:
		{
			pick ();
			break;
		}
		case TraverseType::CAMERA:
		{
			camera ();
			break;
		}
		case TraverseType::NAVIGATION:
		{
			navigation ();
			break;
		}
		case TraverseType::COLLISION:
		{
			collision ();
			gravite ();
			break;
		}
		case TraverseType::DISPLAY:
		{
			collect ();
			break;
		}
	}

	currentViewport -> disable ();

	getBrowser () -> getLayers () .pop ();
}

void
X3DLayerNode::pick ()
{
	if (isPickable ())
	{
		if (getBrowser () -> getPickingLayer ())
		{
			if (getBrowser () -> getPickingLayer () not_eq this)
				return;
		}
		else
		{
			if (not getBrowser () -> isPointerInRectangle (currentViewport -> getRectangle ()))
				return;
		}

		getModelViewMatrix () .identity ();
		getViewpoint () -> reshape ();
		getBrowser ()   -> setPickRay (getModelViewMatrix () .get (), ProjectionMatrix4d (), currentViewport -> getRectangle ());
		getViewpoint () -> transform ();

		currentViewport -> push (TraverseType::PICKING);

		collect (TraverseType::PICKING);
		
		currentViewport -> pop (TraverseType::PICKING);

		getGlobalObjects () .clear ();
	}
}

void
X3DLayerNode::camera ()
{
	getModelViewMatrix () .identity ();
	getViewpoint () -> reshape ();

	defaultNavigationInfo -> traverse (TraverseType::CAMERA);
	defaultBackground     -> traverse (TraverseType::CAMERA);
	defaultFog            -> traverse (TraverseType::CAMERA);
	defaultViewpoint      -> traverse (TraverseType::CAMERA);

	currentViewport -> push (TraverseType::CAMERA);

	collect (TraverseType::CAMERA);

	currentViewport -> pop (TraverseType::CAMERA);

	navigationInfos -> update ();
	backgrounds     -> update ();
	fogs            -> update ();
	viewpoints      -> update ();
}

void
X3DLayerNode::navigation ()
{
	// Render
	currentViewport -> enable ();
	render (TraverseType::NAVIGATION);
	currentViewport -> disable ();
}

void
X3DLayerNode::collision ()
{
	try
	{
		// Get NavigationInfo values

		const auto navigationInfo = getNavigationInfo ();

		const double zNear           = navigationInfo -> getNearPlane ();
		const double zFar            = navigationInfo -> getFarPlane (getViewpoint ());
		const double collisionRadius = zNear / std::sqrt (2.0f); // Use zNear instead of navigationInfo -> getCollisionRatius ();

		// Reshape viewpoint

		glMatrixMode (GL_PROJECTION);
		glLoadMatrixd (ortho (-collisionRadius, collisionRadius, -collisionRadius, collisionRadius, zNear, zFar) .data ());
		glMatrixMode (GL_MODELVIEW);

		// Transform viewpoint

		const auto viewpoint = getViewpoint ();

		Matrix4f   modelViewMatrix = viewpoint -> getParentMatrix ();
		Rotation4f down (Vector3f (0, 0, 1) * viewpoint -> getUserOrientation (), viewpoint -> getUpVector ());

		modelViewMatrix .translate (viewpoint -> getUserPosition ());
		modelViewMatrix .rotate (viewpoint -> getUserOrientation () * down);
		modelViewMatrix .inverse ();

		getModelViewMatrix () .set (modelViewMatrix);

		// Render
		currentViewport -> push (TraverseType::COLLISION);
		render (TraverseType::COLLISION);
		currentViewport -> pop (TraverseType::COLLISION);
	}
	catch (const std::domain_error &)
	{ }
}

void
X3DLayerNode::collect ()
{
	glClear (GL_DEPTH_BUFFER_BIT);

	getModelViewMatrix () .identity ();
	getBackground () -> draw ();

	getNavigationInfo () -> enable ();
	getViewpoint ()      -> reshape ();
	getViewpoint ()      -> transform ();

	currentViewport -> push (TraverseType::DISPLAY);
	render (TraverseType::DISPLAY);
	currentViewport -> pop (TraverseType::DISPLAY);

	getNavigationInfo () -> disable ();
}

void
X3DLayerNode::collect (const TraverseType type)
{
	group -> traverse (type);

	for (const auto & friendNode : friends)
		friendNode -> traverse (type);
}

void
X3DLayerNode::dispose ()
{
	X3DNode::dispose ();
	X3DRenderer::dispose ();
}

} // X3D
} // titania
