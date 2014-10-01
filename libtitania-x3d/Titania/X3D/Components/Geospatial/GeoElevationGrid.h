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

#include "../Geospatial/X3DGeospatialObject.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DGeometryNode.h"
#include "../Rendering/X3DNormalNode.h"
#include "../Texturing/TextureCoordinateGenerator.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

namespace titania {
namespace X3D {

class GeoElevationGrid :
	public X3DGeometryNode, public X3DGeospatialObject
{
public:

	///  @name Construction

	GeoElevationGrid (X3DExecutionContext* const);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const
	throw (Error <DISPOSED>) final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const
	throw (Error <DISPOSED>) final override
	{ return containerField; }

	///  @name Fields

	SFVec3d &
	geoGridOrigin ()
	{ return *fields .geoGridOrigin; }

	const SFVec3d &
	geoGridOrigin () const
	{ return *fields .geoGridOrigin; }

	SFFloat &
	yScale ()
	{ return *fields .yScale; }

	const SFFloat &
	yScale () const
	{ return *fields .yScale; }

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

	SFDouble &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	const SFDouble &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	SFBool &
	colorPerVertex ()
	{ return *fields .colorPerVertex; }

	const SFBool &
	colorPerVertex () const
	{ return *fields .colorPerVertex; }

	SFBool &
	normalPerVertex ()
	{ return *fields .normalPerVertex; }

	const SFBool &
	normalPerVertex () const
	{ return *fields .normalPerVertex; }

	SFNode &
	color ()
	{ return *fields .color; }

	const SFNode &
	color () const
	{ return *fields .color; }

	SFNode &
	texCoord ()
	{ return *fields .texCoord; }

	const SFNode &
	texCoord () const
	{ return *fields .texCoord; }

	SFNode &
	normal ()
	{ return *fields .normal; }

	const SFNode &
	normal () const
	{ return *fields .normal; }

	MFDouble &
	height ()
	{ return *fields .height; }

	const MFDouble &
	height () const
	{ return *fields .height; }

	///  @name Tests

	virtual
	bool
	isTransparent () const final override
	{ return transparent; }

	virtual
	bool
	isLineGeometry () const final override
	{ return false; }

	///  @name Destruction

	virtual
	void
	dispose () final override;


private:

	///  @name Construction

	virtual
	void
	initialize () final override;

	///  @name Event handlers

	void
	set_color ();

	void
	set_texCoord ();

	void
	set_normal ();

	///  @name Operations

	double
	getHeight (const size_t) const;

	std::vector <Vector4f>
	createTexCoord () const;

	std::vector <Vector3f>
	createNormals (const std::vector <Vector3f> &, const std::vector <size_t> &) const;

	std::vector <size_t>
	createCoordIndex () const;

	std::vector <Vector3f>
	createPoints () const;

	virtual
	void
	build () final override;

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFVec3d* const geoGridOrigin;
		SFFloat* const yScale;
		SFInt32* const xDimension;
		SFDouble* const xSpacing;
		SFInt32* const zDimension;
		SFDouble* const zSpacing;
		SFBool* const solid;
		SFBool* const ccw;
		SFDouble* const creaseAngle;
		SFBool* const colorPerVertex;
		SFBool* const normalPerVertex;
		SFNode* const color;
		SFNode* const texCoord;
		SFNode* const normal;
		MFDouble* const height;
	};

	Fields fields;

	X3DPtr <X3DColorNode>             colorNode;
	X3DPtr <X3DTextureCoordinateNode> texCoordNode;
	X3DPtr <X3DNormalNode>            normalNode;
	bool                              transparent;

};

} // X3D
} // titania

#endif
