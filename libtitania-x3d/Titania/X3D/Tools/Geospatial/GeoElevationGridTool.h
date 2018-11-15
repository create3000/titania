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

#ifndef __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_ELEVATION_GRID_TOOL_H__
#define __TITANIA_X3D_TOOLS_GEOSPATIAL_GEO_ELEVATION_GRID_TOOL_H__

#include "../../Components/Geospatial/GeoElevationGrid.h"
#include "../Geospatial/X3DGeospatialObjectTool.h"
#include "../Rendering/X3DGeometryNodeTool.h"

namespace titania {
namespace X3D {

class GeoElevationGridTool :
	virtual public GeoElevationGrid,
	public X3DGeometryNodeTool,
	public X3DGeospatialObjectTool
{
public:

	///  @name Construction

	GeoElevationGridTool (X3DBaseNode* const node);

	///  @name Fields

	virtual
	SFVec3d &
	geoGridOrigin () final override
	{ return getNode <GeoElevationGrid> () -> geoGridOrigin (); }

	virtual
	const SFVec3d &
	geoGridOrigin () const final override
	{ return getNode <GeoElevationGrid> () -> geoGridOrigin (); }

	virtual
	SFInt32 &
	xDimension () final override
	{ return getNode <GeoElevationGrid> () -> xDimension (); }

	virtual
	const SFInt32 &
	xDimension () const final override
	{ return getNode <GeoElevationGrid> () -> xDimension (); }

	virtual
	SFInt32 &
	zDimension () final override
	{ return getNode <GeoElevationGrid> () -> zDimension (); }

	virtual
	const SFInt32 &
	zDimension () const final override
	{ return getNode <GeoElevationGrid> () -> zDimension (); }

	virtual
	SFDouble &
	xSpacing () final override
	{ return getNode <GeoElevationGrid> () -> xSpacing (); }

	virtual
	const SFDouble &
	xSpacing () const final override
	{ return getNode <GeoElevationGrid> () -> xSpacing (); }

	virtual
	SFDouble &
	zSpacing () final override
	{ return getNode <GeoElevationGrid> () -> zSpacing (); }

	virtual
	const SFDouble &
	zSpacing () const final override
	{ return getNode <GeoElevationGrid> () -> zSpacing (); }

	virtual
	SFFloat &
	yScale () final override
	{ return getNode <GeoElevationGrid> () -> yScale (); }

	virtual
	const SFFloat &
	yScale () const final override
	{ return getNode <GeoElevationGrid> () -> yScale (); }

	virtual
	SFBool &
	solid () final override
	{ return getNode <GeoElevationGrid> () -> solid (); }

	virtual
	const SFBool &
	solid () const final override
	{ return getNode <GeoElevationGrid> () -> solid (); }

	virtual
	SFBool &
	ccw () final override
	{ return getNode <GeoElevationGrid> () -> ccw (); }

	virtual
	const SFBool &
	ccw () const final override
	{ return getNode <GeoElevationGrid> () -> ccw (); }

	virtual
	SFDouble &
	creaseAngle () final override
	{ return getNode <GeoElevationGrid> () -> creaseAngle (); }

	virtual
	const SFDouble &
	creaseAngle () const final override
	{ return getNode <GeoElevationGrid> () -> creaseAngle (); }

	virtual
	SFBool &
	colorPerVertex () final override
	{ return getNode <GeoElevationGrid> () -> colorPerVertex (); }

	virtual
	const SFBool &
	colorPerVertex () const final override
	{ return getNode <GeoElevationGrid> () -> colorPerVertex (); }

	virtual
	SFBool &
	normalPerVertex () final override
	{ return getNode <GeoElevationGrid> () -> normalPerVertex (); }

	virtual
	const SFBool &
	normalPerVertex () const final override
	{ return getNode <GeoElevationGrid> () -> normalPerVertex (); }

	virtual
	SFNode &
	color () final override
	{ return getNode <GeoElevationGrid> () -> color (); }

	virtual
	const SFNode &
	color () const final override
	{ return getNode <GeoElevationGrid> () -> color (); }

	virtual
	SFNode &
	texCoord () final override
	{ return getNode <GeoElevationGrid> () -> texCoord (); }

	virtual
	const SFNode &
	texCoord () const final override
	{ return getNode <GeoElevationGrid> () -> texCoord (); }

	virtual
	SFNode &
	normal () final override
	{ return getNode <GeoElevationGrid> () -> normal (); }

	virtual
	const SFNode &
	normal () const final override
	{ return getNode <GeoElevationGrid> () -> normal (); }

	virtual
	MFDouble &
	height () final override
	{ return getNode <GeoElevationGrid> () -> height (); }

	virtual
	const MFDouble &
	height () const final override
	{ return getNode <GeoElevationGrid> () -> height (); }

	///  @name Member access

	virtual
	bool
	isTransparent () const final override
	{ return getNode <GeoElevationGrid> () -> isTransparent (); }

	///  @name Height map handling

	virtual
	const SFEnum <LoadState> &
	checkLoadState () const final override
	{ return getNode <GeoElevationGrid> () -> checkLoadState (); }

	virtual
	void
	loadHeightMap (const MFString & url, const double minHeight, const double maxHeight) final override
	{ getNode <GeoElevationGrid> () -> loadHeightMap (url, minHeight, maxHeight); }

	///  @name Operations

	virtual
	void
	addNormals () final override
	{ getNode <GeoElevationGrid> () -> addNormals (); }

	virtual
	SFNode
	toPrimitive () const final override
	{ return getNode <GeoElevationGrid> () -> toPrimitive (); }

	///  @name Destruction

	virtual
	void
	dispose () final override;

	virtual
	~GeoElevationGridTool () final override;


protected:

	///  @name Construction

	virtual
	void
	initialize () final override;


private:

	///  @name Members

	struct Fields
	{
		Fields ();

	};

	Fields fields;

};

} // X3D
} // titania

#endif
