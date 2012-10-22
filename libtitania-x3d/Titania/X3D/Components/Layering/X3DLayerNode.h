/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#ifndef __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__

#include "../../Rendering/LightContainerArray.h"
#include "../../Rendering/X3DRenderer.h"
#include "../../Types/Geometry.h"
#include "../Core/X3DBindableNode/BindableNodeStack.h"
#include "../Core/X3DNode.h"
#include "../EnvironmentalEffects/Background.h"
#include "../EnvironmentalEffects/Fog.h"
#include "../Interpolation/PositionInterpolator.h"
#include "../Layering/Viewport.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/Viewpoint.h"
#include "../Time/TimeSensor.h"

#include <stack>

namespace titania {
namespace X3D {

class Viewport;

class X3DLayerNode :
	virtual public X3DNode, public X3DRenderer
{
public:

	SFBool                   isPickable;
	SFNode <X3DViewportNode> viewport;
	MFNode <X3DBasicNode>    addChildren;
	MFNode <X3DBasicNode>    removeChildren;
	MFNode <X3DBasicNode>    children;

	X3DRenderer*
	getRenderer () { return this; }

	const X3DRenderer*
	getRenderer () const { return this; }

	// Bindable node stack handling

	//@{
	NavigationInfo*
	getActiveNavigationInfo ();

	X3DBackgroundNode*
	getActiveBackground ();

	X3DFogObject*
	getActiveFog ();

	X3DViewpointNode*
	getActiveViewpoint ();
	//@}

	void
	pushLocalLight (X3DLightNode*);

	void
	popLocalLight () { localLights .pop_back (); }

	const LightContainerArray &
	getLocalLights () { return localLights; }

	void
	addGlobalLight (X3DLightNode* light) { globalLights .push_back (new LightContainer (light)); }

	const LightContainerArray &
	getGlobalLights () { return globalLights; }

	void
	correctLights (const Matrix4d &);

	virtual
	Box3f
	getBBox () = 0;

	void
	showAllObjects ();

	virtual
	void
	select ();

	virtual
	void
	display ();

	virtual
	void
	dispose ();

	SFNode <Viewport> defaultViewport;

	BindableNodeStack <NavigationInfo>    navigationInfoStack;
	BindableNodeStack <X3DBackgroundNode> backgroundStack;
	BindableNodeStack <Fog>               fogStack;
	BindableNodeStack <X3DViewpointNode>  viewpointStack;

	std::stack <LocalFog*> localFogStack;

	SFNode <TimeSensor>           timeSensor;
	SFNode <PositionInterpolator> positionInterpolator;


protected:

	X3DLayerNode ();

	virtual
	void
	initialize ();


private:

	void
	clearLights ();

	void
	set_viewport ();

	Viewport* _viewport;

	LightContainerArray localLights;
	LightContainerArray cachedLocalLights;
	LightContainerArray globalLights;

};

} // X3D
} // titania

#endif
