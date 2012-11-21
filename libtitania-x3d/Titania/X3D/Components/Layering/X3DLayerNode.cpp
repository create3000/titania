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

#include "X3DLayerNode.h"

#include "../../Browser/Browser.h"
#include "../EnvironmentalEffects/Background.h"

namespace titania {
namespace X3D {

X3DLayerNode::X3DLayerNode () :
	             X3DNode (),                                            
	          isPickable (true),                                        // SFBool [in,out] isPickable      TRUE
	            viewport (),                                            // SFNode [in,out] viewport        NULL      [X3DViewportNode]
	         addChildren (),                                            // MFNode[in]      addChildren               [ ]
	      removeChildren (),                                            // MFNode[in]      removeChildren            [ ]
	            children (),                                            // MFNode[in,out]  children        [ ]       [X3DChildNode]
	     defaultViewport (new Viewport (getExecutionContext ())),       
	 navigationInfoStack (new NavigationInfo (getExecutionContext (), false)), 
	     backgroundStack (new Background     (getExecutionContext (), false)), 
	            fogStack (new Fog            (getExecutionContext (), false)), 
	      viewpointStack (new Viewpoint      (getExecutionContext (), false)),
	           localFogs (),                                            
	          timeSensor (),                                            
	positionInterpolator (),                                            
	           _viewport (0)                                            
{
	addNodeType (X3DLayerNodeType);
	
	setChildren (defaultViewport,
	             *navigationInfoStack .top (),
	             *backgroundStack     .top (),
	             *fogStack            .top (),
	             *viewpointStack      .top (),
	             timeSensor,
	             positionInterpolator);

	backgroundStack     .top () -> transparency = 1;
	fogStack            .top () -> transparency = 1;
	viewpointStack      .top () -> description  = "Default Viewpoint " + std::to_string ((size_t) viewpointStack .top ());
}

void
X3DLayerNode::initialize ()
{
	X3DNode::initialize ();

	defaultViewport -> setup ();

	navigationInfoStack .top () -> setup ();
	backgroundStack     .top () -> setup ();
	fogStack            .top () -> setup ();
	viewpointStack      .top () -> setup ();

	viewport .addInterest (this, &X3DLayerNode::set_viewport);

	set_viewport ();
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
X3DLayerNode::correctLights (const Matrix4d & correctionMatrix)
{
	for (const auto & light : cachedLocalLights)
		light -> correct (correctionMatrix);

	for (const auto & light : globalLights)
		light -> correct (correctionMatrix);
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
X3DLayerNode::showAllObjects ()
{
	{
		SFNode <Viewpoint> viewpoint = getViewpoint ();

		if (viewpoint)
		{
			std::clog << "Show all using viewpoint: " << viewpoint -> description << "." << std::endl;

			Box3f bbox = getBBox ();

			Vector3f   translation, scale;
			Rotation4f rotation, scaleOrientation;

			Matrix4f matrix = viewpoint -> getMatrix ();
			matrix .get (translation, rotation, scale, scaleOrientation);

			float    radius   = abs (bbox .size ()) * 0.5f;
			Vector3f position = bbox .center () + rotation* Vector3f (0, 0, radius / std::tan (viewpoint -> fieldOfView * 0.5f));

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//matrix [3] = Vector4f (position, 1);
			//viewpoint -> setMatrix (matrix);

			timeSensor                  = new TimeSensor (getExecutionContext ());
			timeSensor -> cycleInterval = 0.2;
			timeSensor -> startTime     = 1;
			timeSensor -> setup ();

			positionInterpolator             = new PositionInterpolator (getExecutionContext ());
			positionInterpolator -> key      = { 0, 1 };
			positionInterpolator -> keyValue = { viewpoint -> position, position };
			positionInterpolator -> setup ();

			getExecutionContext () -> addRoute (timeSensor, "fraction_changed", positionInterpolator, "set_fraction");
			getExecutionContext () -> addRoute (positionInterpolator, "value_changed", viewpoint, "set_position");

			viewpoint -> centerOfRotation = bbox .center ();
			viewpoint -> set_bind         = true;
		}
		else
			throw Error <INVALID_X3D> ("No current viewpoint available!");
	}

	std::clog << getViewpoint () << std::endl;
}

void
X3DLayerNode::select ()
{
	if (not isPickable)
		return;

	_viewport -> enable ();

	getViewpoint () -> draw ();

	intersect ();

	_viewport -> disable ();
}

void
X3DLayerNode::display ()
{
	_viewport -> enable ();

	glClear (GL_DEPTH_BUFFER_BIT);

	getBackground ()     -> draw ();
	getNavigationInfo () -> enable ();
	getViewpoint ()      -> draw ();

	X3DRenderer::display ();

	getNavigationInfo () -> disable ();
	_viewport            -> disable ();
	clearLights ();
}

void
X3DLayerNode::set_viewport ()
{
	_viewport = dynamic_cast <Viewport*> (viewport .getValue ());

	if (not _viewport)
		_viewport = dynamic_cast <Viewport*> (defaultViewport .getValue ());
}

void
X3DLayerNode::dispose ()
{
	//	delete navigationInfoStack .bottom ();
	//	delete backgroundStack     .bottom ();
	//	delete fogStack            .bottom ();
	//	delete viewpointStack      .bottom ();

	X3DNode::dispose ();
}

} // X3D
} // titania
