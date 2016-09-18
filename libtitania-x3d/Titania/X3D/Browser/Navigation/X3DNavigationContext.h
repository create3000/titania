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

#ifndef __TITANIA_X3D_BROWSER_NAVIGATION_X3DNAVIGATION_CONTEXT_H__
#define __TITANIA_X3D_BROWSER_NAVIGATION_X3DNAVIGATION_CONTEXT_H__

#include "../../Fields.h"
#include "../../Types/Pointer.h"

namespace titania {
namespace X3D {

class DirectionalLight;

class X3DNavigationContext :
	virtual public X3DBaseNode
{
public:

	const X3DPtr <DirectionalLight> &
	getHeadLight () const
	{ return headLight; }

	const X3DLayerNodePtr &
	getActiveLayer () const
	{ return activeLayer; }

	const SFTime &
	getActiveNavigationInfoEvent () const
	{ return activeNavigationInfoOutput; }

	const SFTime &
	getActiveViewpointEvent () const
	{ return activeViewpointOutput; }

	X3D::X3DConstants::NodeType
	getCurrentViewer () const;

	virtual
	void
	setViewerType (const X3DConstants::NodeType value)
	{ viewerType = value; }

	virtual
	const SFEnum <X3DConstants::NodeType> &
	getViewerType () const
	{ return viewerType; }

	void
	setPrivateViewer (const X3DConstants::NodeType value)
	{ privateViewer = value; }

	const SFEnum <X3DConstants::NodeType> &
	getPrivateViewer () const
	{ return privateViewer; }

	const MFEnum <X3DConstants::NodeType> &
	getAvailableViewers () const
	{ return availableViewers; }

	void
	addCollision (const X3DBaseNode* collision)
	{ activeCollisions .emplace (collision); }

	void
	removeCollision (const X3DBaseNode* collision)
	{ activeCollisions .erase (collision); }

	std::set <const X3DBaseNode*>
	getActiveCollisions () const
	{ return activeCollisions; }

	void
	setStraightenHorizon (const bool value)
	{ straightenHorizon = value; }

	const SFBool &
	getStraightenHorizon () const
	{ return straightenHorizon; }

	///  @name Destruction

	virtual
	void
	dispose () override
	{ }

	virtual
	~X3DNavigationContext ();


protected:

	///  @name Constructor

	X3DNavigationContext ();

	virtual
	void
	initialize () override;

	///  @name Members access

	NavigationInfo*
	getActiveNavigationInfo () const
	{ return activeNavigationInfo; }


private:

	///  @name Member access

	virtual
	const WorldPtr &
	getWorld () const = 0;

	///  @name Event handlers

	void
	set_initialized ();

	void
	set_activeLayer ();

	void
	set_navigationInfo ();

	void
	remove_navigationInfo ();

	void
	set_viewpoint ();

	void
	set_navigationInfo_type ();

	///  @name Members

	X3DPtr <DirectionalLight>       headLight;
	X3DLayerNodePtr                 activeLayer;
	NavigationInfo*                 activeNavigationInfo;
	SFTime                          activeNavigationInfoOutput;
	SFEnum <X3DConstants::NodeType> viewerType;
	SFEnum <X3DConstants::NodeType> privateViewer;
	MFEnum <X3DConstants::NodeType> availableViewers;
	SFTime                          activeViewpointOutput;
	std::set <const X3DBaseNode*>   activeCollisions;
	SFBool                          straightenHorizon;

};

} // X3D
} // titania

#endif
