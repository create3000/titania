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

#ifndef __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_LODTOOL_H__
#define __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_LODTOOL_H__

#include "../Core/X3DChildNodeTool.h"
#include "../Geospatial/X3DGeospatialObjectTool.h"
#include "../Grouping/X3DBoundedObjectTool.h"
#include "../ToolColors.h"

#include "../../Components/Geospatial/GeoLOD.h"

namespace titania {
namespace X3D {

class GeoLODTool :
	public X3DChildNodeTool <GeoLOD>,
	public X3DBoundedObjectTool <GeoLOD>,
	public X3DGeospatialObjectTool <GeoLOD>
{
public:

	///  @name Construction

	GeoLODTool (GeoLOD* const node) :
		                  X3DBaseNode (node -> getExecutionContext () -> getBrowser (), node -> getExecutionContext ()),
		         X3DBaseTool <GeoLOD> (node),
		    X3DChildNodeTool <GeoLOD> (),
		X3DBoundedObjectTool <GeoLOD> (ToolColors::DARK_CYAN),
		      X3DGeospatialObjectTool ()
	{
		addType (X3DConstants::GeoLODTool);

		setDisplayCenter (true);
	}

	///  @name Fields

	virtual
	MFString &
	rootUrl ()
	{ return getNode () -> rootUrl (); }

	virtual
	const MFString &
	rootUrl () const
	{ return getNode () -> rootUrl (); }

	virtual
	MFString &
	child1Url ()
	{ return getNode () -> child1Url (); }

	virtual
	const MFString &
	child1Url () const
	{ return getNode () -> child1Url (); }

	virtual
	MFString &
	child2Url ()
	{ return getNode () -> child2Url (); }

	virtual
	const MFString &
	child2Url () const
	{ return getNode () -> child2Url (); }

	virtual
	MFString &
	child3Url ()
	{ return getNode () -> child3Url (); }

	virtual
	const MFString &
	child3Url () const
	{ return getNode () -> child3Url (); }

	virtual
	MFString &
	child4Url ()
	{ return getNode () -> child4Url (); }

	virtual
	const MFString &
	child4Url () const
	{ return getNode () -> child4Url (); }

	virtual
	SFVec3d &
	center ()
	{ return getNode () -> center (); }

	virtual
	const SFVec3d &
	center () const
	{ return getNode () -> center (); }

	virtual
	SFFloat &
	range ()
	{ return getNode () -> range (); }

	virtual
	const SFFloat &
	range () const
	{ return getNode () -> range (); }

	virtual
	SFInt32 &
	level_changed ()
	{ return getNode () -> level_changed (); }

	virtual
	const SFInt32 &
	level_changed () const
	{ return getNode () -> level_changed (); }

	virtual
	MFNode &
	rootNode ()
	{ return getNode () -> rootNode (); }

	virtual
	const MFNode &
	rootNode () const
	{ return getNode () -> rootNode (); }

	virtual
	MFNode &
	children ()
	{ return getNode () -> children (); }

	virtual
	const MFNode &
	children () const
	{ return getNode () -> children (); }

	///  @name Member access

	virtual
	void
	setKeepCurrentLevel (const bool value) final override
	{ getNode () -> setKeepCurrentLevel (value); }

	virtual
	bool
	getKeepCurrentLevel () const final override
	{ return getNode () -> getKeepCurrentLevel (); }

	///  @name Operations

	virtual
	void
	traverse (const TraverseType type) final override
	{
		X3DChildNodeTool <GeoLOD>::traverse (type);
		X3DBoundedObjectTool <GeoLOD>::traverse (type);
		X3DGeospatialObjectTool <GeoLOD>::traverse (type);
	}

	///  @name Destruction

	virtual
	void
	dispose () final override
	{
		X3DGeospatialObjectTool <GeoLOD>::dispose ();
		X3DBoundedObjectTool <GeoLOD>::dispose ();
		X3DChildNodeTool <GeoLOD>::dispose ();
	}

private:

	using X3DChildNodeTool <GeoLOD>::addType;
	using X3DChildNodeTool <GeoLOD>::getNode;

	///  @name Construction

	virtual
	void
	initialize () final override
	{
		X3DChildNodeTool <GeoLOD>::initialize ();
		X3DBoundedObjectTool <GeoLOD>::initialize ();
		X3DGeospatialObjectTool <GeoLOD>::initialize ();
	}

	///  @name Operations

	virtual
	void
	reshape () final override
	{
		try
		{
			const SFNode & tool = getToolNode ();

			tool -> setField <SFVec3f> ("set_center", getCoord (center ()), true);
		}
		catch (const X3DError & error)
		{ }

		X3DBoundedObjectTool <GeoLOD>::reshape ();
	}

};

} // X3D
} // titania

#endif
