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

#ifndef __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_ELEVATION_GRID_H__
#define __TITANIA_X3D_COMPONENTS_GEOSPATIAL_GEO_ELEVATION_GRID_H__

#include "../Rendering/X3DGeometryNode.h"

namespace titania {
namespace X3D {

class GeoElevationGrid :
	public X3DGeometryNode
{
public:

	GeoElevationGrid (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const;

	///  @name Fields

	MFDouble &
	set_height ()
	{ return *fields .set_height; }

	const MFDouble &
	set_height () const
	{ return *fields .set_height; }

	SFNode <X3DBaseNode> &
	color ()
	{ return *fields .color; }

	const SFNode <X3DBaseNode> &
	color () const
	{ return *fields .color; }

	SFNode <X3DBaseNode> &
	normal ()
	{ return *fields .normal; }

	const SFNode <X3DBaseNode> &
	normal () const
	{ return *fields .normal; }

	SFNode <X3DBaseNode> &
	texCoord ()
	{ return *fields .texCoord; }

	const SFNode <X3DBaseNode> &
	texCoord () const
	{ return *fields .texCoord; }

	SFFloat &
	yScale ()
	{ return *fields .yScale; }

	const SFFloat &
	yScale () const
	{ return *fields .yScale; }

	SFBool &
	colorPerVertex ()
	{ return *fields .colorPerVertex; }

	const SFBool &
	colorPerVertex () const
	{ return *fields .colorPerVertex; }

	SFDouble &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	const SFDouble &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	SFVec3d &
	geoGridOrigin ()
	{ return *fields .geoGridOrigin; }

	const SFVec3d &
	geoGridOrigin () const
	{ return *fields .geoGridOrigin; }

	SFNode <X3DBaseNode> &
	geoOrigin ()
	{ return *fields .geoOrigin; }

	const SFNode <X3DBaseNode> &
	geoOrigin () const
	{ return *fields .geoOrigin; }

	MFString &
	geoSystem ()
	{ return *fields .geoSystem; }

	const MFString &
	geoSystem () const
	{ return *fields .geoSystem; }

	MFDouble &
	height ()
	{ return *fields .height; }

	const MFDouble &
	height () const
	{ return *fields .height; }

	SFBool &
	normalPerVertex ()
	{ return *fields .normalPerVertex; }

	const SFBool &
	normalPerVertex () const
	{ return *fields .normalPerVertex; }

	SFBool &
	solid ()
	{ return *fields .solid; }

	const SFBool &
	solid () const
	{ return *fields .solid; }

	SFBool &
	ccw ()
	{ return *fields .ccw; }

	const SFBool &
	ccw () const
	{ return *fields .ccw; }

	SFInt32 &
	xDimension ()
	{ return *fields .xDimension; }

	const SFInt32 &
	xDimension () const
	{ return *fields .xDimension; }

	SFDouble &
	xSpacing ()
	{ return *fields .xSpacing; }

	const SFDouble &
	xSpacing () const
	{ return *fields .xSpacing; }

	SFInt32 &
	zDimension ()
	{ return *fields .zDimension; }

	const SFInt32 &
	zDimension () const
	{ return *fields .zDimension; }

	SFDouble &
	zSpacing ()
	{ return *fields .zSpacing; }

	const SFDouble &
	zSpacing () const
	{ return *fields .zSpacing; }


private:

	virtual
	void
	initialize ();
	
	void
	set_ccw ();

	struct Fields
	{
		Fields ();

		MFDouble* const set_height;
		SFNode <X3DBaseNode>* const color;
		SFNode <X3DBaseNode>* const normal;
		SFNode <X3DBaseNode>* const texCoord;
		SFFloat* const yScale;
		SFBool* const colorPerVertex;
		SFDouble* const creaseAngle;
		SFVec3d* const geoGridOrigin;
		SFNode <X3DBaseNode>* const geoOrigin;
		MFString* const geoSystem;
		MFDouble* const height;
		SFBool* const normalPerVertex;
		SFBool* const solid;
		SFBool* const ccw;
		SFInt32* const xDimension;
		SFDouble* const xSpacing;
		SFInt32* const zDimension;
		SFDouble* const zSpacing;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
