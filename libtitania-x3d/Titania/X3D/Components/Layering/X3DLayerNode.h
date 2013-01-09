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

#ifndef __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__

#include "../../Execution/BindableNodeStack.h"
#include "../../Execution/BindableNodeList.h"
#include "../../Rendering/LightContainerArray.h"
#include "../../Rendering/X3DRenderer.h"
#include "../Core/X3DNode.h"
#include "../EnvironmentalEffects/LocalFog.h"
#include "../Grouping/X3DGroupingNode.h"
#include "../Layering/Viewport.h"

#include <stack>

namespace titania {
namespace X3D {

typedef BindableNodeList <NavigationInfo>    NavigationInfoList;
typedef BindableNodeList <X3DBackgroundNode> BackgroundList;
typedef BindableNodeList <Fog>               FogList;
typedef BindableNodeList <X3DViewpointNode>  ViewpointList;

typedef std::stack <LocalFog*> LocalFogStack;

class X3DLayerNode :
	virtual public X3DNode, public X3DRenderer
{
public:

	///  @name Fields

	SFBool                   isPickable;
	SFNode <X3DViewportNode> viewport;
	MFNode <X3DChildNode>    addChildren;
	MFNode <X3DChildNode>    removeChildren;
	MFNode <X3DChildNode>    children;

	///  @name Bindable node stack handling

	NavigationInfo*
	getNavigationInfo ();

	X3DBackgroundNode*
	getBackground ();

	X3DFogObject*
	getFog ();

	X3DViewpointNode*
	getViewpoint ();

	///  @name X3DBindableNode stack handling

	BindableNodeStack <NavigationInfo> &
	getNavigationInfoStack () { return navigationInfoStack; }

	BindableNodeStack <X3DBackgroundNode> &
	getBackgroundStack () { return backgroundStack; }

	BindableNodeStack <Fog> &
	getFogStack () { return fogStack; }

	BindableNodeStack <X3DViewpointNode> &
	getViewpointStack () { return viewpointStack; }

	///  @name X3DBindableNode list handling

	NavigationInfoList &
	getNavigationInfos () { return navigationInfos; }

	BackgroundList &
	getBackgrounds () { return backgrounds; }

	FogList &
	getFogs () { return fogs; }

	ViewpointList &
	getViewpoints () { return viewpoints; }

	///  @name Fog handling

	LocalFogStack &
	getLocalFogs () { return localFogs; }

	///  @name Light handling

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

	///  @name ...

	virtual
	Box3f
	getBBox () = 0;

	void
	lookAt ();

	virtual
	void
	pick ();

	virtual
	void
	intersect () = 0;

	virtual
	void
	display ();

	virtual
	void
	dispose ();


protected:

	X3DLayerNode ();

	virtual
	void
	initialize ();


private:

	void
	enableHeadlight ();

	void
	disableHeadlight ();

	void
	clearLights ();

	void
	set_viewport ();

	SFNode <Viewport>          defaultViewport;
	SFNode <NavigationInfo>    defaultNavigationInfo;
	SFNode <X3DBackgroundNode> defaultBackground;
	SFNode <Fog>               defaultFog;
	SFNode <X3DViewpointNode>  defaultViewpoint;

	X3DViewportNode* currentViewport;

	BindableNodeStack <NavigationInfo>    navigationInfoStack;
	BindableNodeStack <X3DBackgroundNode> backgroundStack;
	BindableNodeStack <Fog>               fogStack;
	BindableNodeStack <X3DViewpointNode>  viewpointStack;

	NavigationInfoList navigationInfos;
	BackgroundList     backgrounds;
	FogList            fogs;
	ViewpointList      viewpoints;

	LocalFogStack       localFogs;
	LightContainerArray localLights;
	LightContainerArray cachedLocalLights;
	LightContainerArray globalLights;

};

} // X3D
} // titania

#endif
