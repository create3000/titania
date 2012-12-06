
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

#ifndef __TITANIA_X3D_EXECUTION_SCENE_H__
#define __TITANIA_X3D_EXECUTION_SCENE_H__

#include "../Components/EnvironmentalEffects/Fog.h"
#include "../Components/EnvironmentalEffects/X3DBackgroundNode.h"
#include "../Components/Layering/LayerSet.h"
#include "../Components/Navigation/NavigationInfo.h"
#include "../Components/Navigation/X3DViewpointNode.h"
#include "../Execution/BindableNodeList.h"
#include "../Execution/X3DScene.h"
#include "../Types/Geometry.h"

namespace titania {
namespace X3D {

typedef BindableNodeList <NavigationInfo>    NavigationInfoList;
typedef BindableNodeList <X3DBackgroundNode> BackgroundList;
typedef BindableNodeList <Fog>               FogList;
typedef BindableNodeList <X3DViewpointNode>  ViewpointList;

class Scene :
	public X3DScene
{
public:

	///  @name Construction

	Scene (X3DBrowser* const);

	Scene*
	create (const basic::uri &, std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	Scene*
	create (X3DExecutionContext* const) const;

	///  @name BBox

	Box3f
	getBBox ();

	///  @name NavigationInfo list handling

	void
	addNavigationInfo (NavigationInfo* const);

	void
	removeNavigationInfo (NavigationInfo* const);

	const NavigationInfoList &
	getNavigationInfos () const;

	///  @name Background list handling

	void
	addBackground (X3DBackgroundNode* const);

	void
	removeBackground (X3DBackgroundNode* const);

	const BackgroundList &
	getBackgrounds () const;

	///  @name Fog list handling

	void
	addFog (Fog* const);

	void
	removeFog (Fog* const);

	const FogList &
	getFogs () const;

	///  @name Viewpoint list handling

	void
	addViewpoint (X3DViewpointNode* const);

	void
	removeViewpoint (X3DViewpointNode* const);

	const ViewpointList &
	getViewpoints () const;

	///  @name Root node handling

	virtual
	void
	addRootNode (const SFNode <X3DBaseNode> &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	virtual
	void
	removeRootNode (const SFNode <X3DBaseNode> &)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>);

	///  @name Layer handling

	const SFNode <LayerSet> &
	getLayerSet () const;

	const SFNode <X3DLayerNode>
	getActiveLayer () const;

	///  @name Display

	virtual
	void
	display ();

	///  @name Dispose

	// Object:
	virtual
	void
	dispose ();


private:

	virtual
	void
	initialize ();

	virtual
	void
	clear ();

	NavigationInfoList navigationInfos;
	BackgroundList     backgrounds;
	FogList            fogs;
	ViewpointList      viewpoints;

	SFNode <LayerSet> layerSet;

};

} // X3D
} // titania

#endif
