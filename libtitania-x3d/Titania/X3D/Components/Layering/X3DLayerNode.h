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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__

#include "../../Rendering/X3DRenderer.h"
#include "../../Types/Pointer.h"
#include "../EnvironmentalEffects/LocalFog.h"
#include "../Grouping/X3DGroupingNode.h"
#include "../Layering/X3DViewportNode.h"

#include "../EnvironmentalEffects/Fog.h"
#include "../EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Navigation/NavigationInfo.h"
#include "../Navigation/X3DViewpointNode.h"

#include <stack>

namespace titania {
namespace X3D {

typedef std::vector <X3D::X3DViewpointNode*> UserViewpointList;
typedef std::stack <LocalFog*>               LocalFogStack;

class X3DLayerNode :
	virtual public X3DNode, public X3DRenderer
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override;

	///  @name Fields

	SFBool &
	isPickable ()
	{ return *fields .isPickable; }

	const SFBool &
	isPickable () const
	{ return *fields .isPickable; }

	SFNode &
	viewport ()
	{ return *fields .viewport; }

	const SFNode &
	viewport () const
	{ return *fields .viewport; }

	MFNode &
	addChildren ()
	{ return *fields .addChildren; }

	const MFNode &
	addChildren () const
	{ return *fields .addChildren; }

	MFNode &
	removeChildren ()
	{ return *fields .removeChildren; }

	const MFNode &
	removeChildren () const
	{ return *fields .removeChildren; }

	MFNode &
	children ()
	{ return *fields .children; }

	const MFNode &
	children () const
	{ return *fields .children; }

	///  @name Member access

	void
	isLayer0 (const bool);

	bool
	isLayer0 () const
	{ return layer0; }

	Box3f
	getBBox () const;

	///  @name Bindable node stack handling

	X3DViewportNode*
	getViewport () const
	{ return currentViewport; }

	NavigationInfo*
	getNavigationInfo () const;

	X3DBackgroundNode*
	getBackground () const;

	X3DViewpointNode*
	getViewpoint () const;

	///  @name X3DBindableNode stack handling

	const NavigationInfoStackPtr &
	getNavigationInfoStack () const
	{ return navigationInfoStack; }

	const BackgroundStackPtr &
	getBackgroundStack () const
	{ return backgroundStack; }

	const FogStackPtr &
	getFogStack () const
	{ return fogStack; }

	const ViewpointStackPtr &
	getViewpointStack () const
	{ return viewpointStack; }

	///  @name X3DBindableNode list handling

	const NavigationInfoListPtr &
	getNavigationInfos () const
	{ return navigationInfos; }

	const BackgroundListPtr &
	getBackgrounds () const
	{ return backgrounds; }

	const FogListPtr &
	getFogs () const
	{ return fogs; }

	const ViewpointListPtr &
	getViewpoints () const
	{ return viewpoints; }

	UserViewpointList
	getUserViewpoints () const;

	///  @name Fog handling

	LocalFogStack &
	getLocalFogs ()
	{ return localFogs; }

	///  @name Friends handling

	MFNode &
	getFriends ()
	{ return friends; }

	const MFNode &
	getFriends () const
	{ return friends; }

	///  @name Operations

	Vector3f
	getTranslation (const Vector3f &, const float, const float, const Vector3f &);

	void
	lookAt ();

	void
	bind ();

	virtual
	void
	traverse (const TraverseType) override;

	///  @name Destruction

	virtual
	void
	dispose () override;


protected:

	using X3DNode::addChildren;
	using X3DNode::removeChildren;

	///  @name Construction

	X3DLayerNode (X3DViewpointNode*, X3DGroupingNode*);

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DPtr <X3DGroupingNode>
	getGroup () const
	{ return group; }

	virtual
	X3DFogObject*
	getFog () const final override;


private:

	///  @name Member access

	float
	getDistance (const Vector3f &, const float, const float, const Vector3f &);

	///  @name Operations

	void
	set_viewport ();

	void
	pick ();

	void
	camera ();

	void
	navigation ();

	void
	collision ();

	void
	collect ();

	virtual
	void
	collect (const TraverseType) override;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const isPickable;
		SFNode* const viewport;
		MFNode* const addChildren;
		MFNode* const removeChildren;
		MFNode* const children;
	};

	Fields fields;
	
	bool layer0;

	NavigationInfoPtr    defaultNavigationInfo;
	X3DBackgroundNodePtr defaultBackground;
	FogPtr               defaultFog;
	X3DViewpointNodePtr  defaultViewpoint;

	X3DViewportNodePtr currentViewport;

	NavigationInfoStackPtr navigationInfoStack;
	BackgroundStackPtr     backgroundStack;
	FogStackPtr            fogStack;
	ViewpointStackPtr      viewpointStack;

	NavigationInfoListPtr navigationInfos;
	BackgroundListPtr     backgrounds;
	FogListPtr            fogs;
	ViewpointListPtr      viewpoints;

	LocalFogStack  localFogs;

	X3DPtr <X3DGroupingNode> group;
	MFNode                   friends;

};

} // X3D
} // titania

#endif
