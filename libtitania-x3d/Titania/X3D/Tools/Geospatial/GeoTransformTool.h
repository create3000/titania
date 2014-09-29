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

#ifndef __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_TRANSFORM_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_TRANSFORM_TOOL_H__

#include "../Geospatial/X3DGeospatialObjectTool.h"
#include "../Grouping/X3DTransformMatrix4DNodeTool.h"
#include "../ToolColors.h"

#include "../../Components/Geospatial/GeoTransform.h"

namespace titania {
namespace X3D {

class GeoTransformTool :
	public X3DTransformMatrix4DNodeTool <GeoTransform>,
	public X3DGeospatialObjectTool <GeoTransform>
{
public:

	///  @name Construction

	GeoTransformTool (GeoTransform* const node) :
		                                X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		                 X3DBaseTool <GeoTransform> (node),
		X3DTransformMatrix4DNodeTool <GeoTransform> (ToolColors::DARK_GREEN),
		     X3DGeospatialObjectTool <GeoTransform> ()
	{
		addType (X3DConstants::GeoTransformTool);

		setLinetype (1);
		setDisplayCenter (true);
	}

	///  @name Fields

	virtual
	SFVec3f &
	translation () final override
	{ return getNode () -> translation (); }

	virtual
	const SFVec3f &
	translation () const final override
	{ return getNode () -> translation (); }

	virtual
	SFRotation &
	rotation () final override
	{ return getNode () -> rotation (); }

	virtual
	const SFRotation &
	rotation () const final override
	{ return getNode () -> rotation (); }

	virtual
	SFVec3f &
	scale () final override
	{ return getNode () -> scale (); }

	virtual
	const SFVec3f &
	scale () const final override
	{ return getNode () -> scale (); }

	virtual
	SFRotation &
	scaleOrientation () final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	const SFRotation &
	scaleOrientation () const final override
	{ return getNode () -> scaleOrientation (); }

	virtual
	SFVec3d &
	geoCenter () final override
	{ return getNode () -> geoCenter (); }

	virtual
	const SFVec3d &
	geoCenter () const final override
	{ return getNode () -> geoCenter (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type) final override
	{
		X3DTransformMatrix4DNodeTool <GeoTransform>::traverse (type);
		X3DGeospatialObjectTool <GeoTransform>::traverse (type);
	}

	///  @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DGeospatialObjectTool <GeoTransform>::dispose ();
		X3DTransformMatrix4DNodeTool <GeoTransform>::dispose ();
	}

private:

	using X3DTransformMatrix4DNodeTool <GeoTransform>::addType;
	using X3DTransformMatrix4DNodeTool <GeoTransform>::getNode;

	///  @name Destruction

	virtual
	void
	initialize () final override
	{
		X3DTransformMatrix4DNodeTool <GeoTransform>::initialize ();
		X3DGeospatialObjectTool <GeoTransform>::initialize ();
	}

};

} // X3D
} // titania

#endif
