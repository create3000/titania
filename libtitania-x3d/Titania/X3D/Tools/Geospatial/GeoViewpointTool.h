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

#ifndef __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_VIEWPOINT_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_VIEWPOINT_TOOL_H__

#include "../Geospatial/X3DGeospatialObjectTool.h"
#include "../Navigation/X3DViewpointNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/Geospatial/GeoViewpoint.h"

namespace titania {
namespace X3D {

class GeoViewpointTool :
	virtual public GeoViewpoint,
	public X3DViewpointNodeTool,
	public X3DGeospatialObjectTool
{
public:

	///  @name Construction

	GeoViewpointTool (X3DBaseNode* const node) :
		            X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		           GeoViewpoint (node -> getExecutionContext ()),
		            X3DBaseTool (node),
		   X3DViewpointNodeTool (),
		X3DGeospatialObjectTool ()
	{
		addType (X3DConstants::GeoViewpointTool);
	}

	///  @name Fields

	virtual
	SFVec3d &
	position () final override
	{ return getNode <GeoViewpoint> () -> position (); }

	virtual
	const SFVec3d &
	position () const final override
	{ return getNode <GeoViewpoint> () -> position (); }

	virtual
	SFVec3d &
	centerOfRotation () final override
	{ return getNode <GeoViewpoint> () -> centerOfRotation (); }

	virtual
	const SFVec3d &
	centerOfRotation () const final override
	{ return getNode <GeoViewpoint> () -> centerOfRotation (); }

	virtual
	SFFloat &
	fieldOfView () final override
	{ return getNode <GeoViewpoint> () -> fieldOfView (); }

	virtual
	const SFFloat &
	fieldOfView () const final override
	{ return getNode <GeoViewpoint> () -> fieldOfView (); }

	virtual
	SFFloat &
	speedFactor () final override
	{ return getNode <GeoViewpoint> () -> speedFactor (); }

	virtual
	const SFFloat &
	speedFactor () const final override
	{ return getNode <GeoViewpoint> () -> speedFactor (); }

	///  @name Member access

	virtual
	void
	setExecutionContext (X3DExecutionContext* const executionContext)
	throw (Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) final override
	{ X3DViewpointNodeTool::setExecutionContext (executionContext); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type, X3DRenderObject* const renderObject) final override
	{
		X3DViewpointNodeTool::traverse (type, renderObject);
		X3DGeospatialObjectTool::traverse (type, renderObject);
	}

	///  @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DGeospatialObjectTool::dispose ();
		X3DViewpointNodeTool::dispose ();
	}

private:

	///  @name Destruction

	virtual
	void
	initialize () final override
	{
		X3DViewpointNodeTool::initialize ();
		X3DGeospatialObjectTool::initialize ();
	}

	virtual
	void
	reshape () final override
	{
		try
		{
	      getBrowser () -> endUpdateForFrame ();
			
			const SFNode & tool = getToolNode ();

			tool -> setField <SFVec3f>    ("translation", getNode <GeoViewpoint> () -> getPosition (),    true);
			tool -> setField <SFRotation> ("rotation",    getNode <GeoViewpoint> () -> getOrientation (), true);

			getBrowser () -> processEvents ();
	      getBrowser () -> beginUpdateForFrame ();
		}
		catch (const X3DError & error)
		{
			getBrowser () -> beginUpdateForFrame ();
		}
	}

};

} // X3D
} // titania

#endif
