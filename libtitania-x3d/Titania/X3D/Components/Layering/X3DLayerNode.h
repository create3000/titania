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

#ifndef __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__
#define __TITANIA_X3D_COMPONENTS_LAYERING_X3DLAYER_NODE_H__

#include "../../Rendering/X3DRenderObject.h"

namespace titania {
namespace X3D {

class X3DGroupingNode;
class X3DViewportNode;

using UserViewpointList = std::vector <X3D::X3DViewpointNode*>;

class X3DLayerNode :
	virtual public X3DNode,
	public X3DRenderObject
{
public:

	///  @name Common members

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext) final override;

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
	isLayer0 (const bool value);

	bool
	isLayer0 () const
	{ return layer0; }

	Box3d
	getBBox () const;

	///  @name Member access

	virtual
	X3DLayerNode*
	getLayer () const final override
	{ return const_cast <X3DLayerNode*> (this); }

	const X3DPtr <X3DViewportNode> &
	getViewport () const
	{ return currentViewport; }

	virtual
	NavigationInfo*
	getNavigationInfo () const final override;

	virtual
	X3DViewpointNode*
	getViewpoint () const final override;

	virtual
	X3DBackgroundNode*
	getBackground () const final override;

	///  @name X3DBindableNode stack handling

	const NavigationInfoStackPtr &
	getNavigationInfoStack () const
	{ return navigationInfoStack; }

	const ViewpointStackPtr &
	getViewpointStack () const
	{ return viewpointStack; }

	const BackgroundStackPtr &
	getBackgroundStack () const
	{ return backgroundStack; }

	const FogStackPtr &
	getFogStack () const
	{ return fogStack; }

	///  @name X3DBindableNode list handling

	const NavigationInfoListPtr &
	getNavigationInfos () const
	{ return navigationInfos; }

	const ViewpointListPtr &
	getViewpoints () const
	{ return viewpoints; }

	const BackgroundListPtr &
	getBackgrounds () const
	{ return backgrounds; }

	const FogListPtr &
	getFogs () const
	{ return fogs; }

	UserViewpointList
	getUserViewpoints () const;

	///  @name Friends handling

	X3DPtr <X3DGroupingNode> &
	getFriends ()
	{ return friendsNode; }

	const X3DPtr <X3DGroupingNode> &
	getFriends () const
	{ return friendsNode; }

	///  @name Operations

	void
	lookAt (const double factor = 1, const bool straighten = false, const time_type cycleInterval = 0.2);

	void
	bind (const std::string & viewpointName = "");

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DLayerNode () override;


protected:

	///  @name Friends

	friend class X3DLightNode;

	///  @name Construction

	X3DLayerNode (X3DViewpointNode*, X3DGroupingNode*);

	virtual
	void
	initialize () override;

	///  @name Member access

	const X3DPtr <X3DGroupingNode> &
	getGroup () const
	{ return groupNode; }

	virtual
	X3DFogObject*
	getFog () const final override;


private:

	///  @name Operations

	void
	set_viewport ();

	void
	pointer (const TraverseType type, X3DRenderObject* const renderObject);

	void
	camera (const TraverseType type, X3DRenderObject* const renderObject);

	void
	collision (const TraverseType type, X3DRenderObject* const renderObject);

	void
	display (const TraverseType type, X3DRenderObject* const renderObject);

	void
	collect (const TraverseType, X3DRenderObject* const renderObject);

	///  @name Fields

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

	///  @name Members
	
	bool layer0;

	X3DPtr <NavigationInfo>    defaultNavigationInfo;
	X3DPtr <X3DViewpointNode>  defaultViewpoint;
	X3DPtr <X3DBackgroundNode> defaultBackground;
	X3DPtr <Fog>               defaultFog;

	X3DPtr <X3DViewportNode> currentViewport;

	NavigationInfoStackPtr navigationInfoStack;
	ViewpointStackPtr      viewpointStack;
	BackgroundStackPtr     backgroundStack;
	FogStackPtr            fogStack;

	NavigationInfoListPtr navigationInfos;
	ViewpointListPtr      viewpoints;
	BackgroundListPtr     backgrounds;
	FogListPtr            fogs;

	X3DPtr <X3DGroupingNode> groupNode;
	X3DPtr <X3DGroupingNode> friendsNode;

};

} // X3D
} // titania

#endif
