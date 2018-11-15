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

#ifndef __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__
#define __TITANIA_X3D_COMPONENTS_GEOMETRY3D_INDEXED_FACE_SET_H__

#include "../Rendering/X3DComposedGeometryNode.h"

#include <Titania/Math/Mesh/Tessellator.h>

namespace titania {
namespace opengl {

template <class Type, class ... Args>
class tessellator;

} // opengl
} // titania

namespace titania {
namespace X3D {

class IndexedFaceSet :
	virtual public X3DComposedGeometryNode
{
public:

	///  @name Construction

	IndexedFaceSet (X3DExecutionContext* const executionContext);

	virtual
	X3DBaseNode*
	create (X3DExecutionContext* const executionContext) const final override;

	///  @name Common members

	virtual
	ComponentType
	getComponent () const final override
	{ return component; }

	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	const std::string &
	getContainerField () const final override
	{ return containerField; }

	///  @name Fields

	virtual
	SFBool &
	convex ()
	{ return *fields .convex; }

	virtual
	const SFBool &
	convex () const
	{ return *fields .convex; }

	virtual
	SFFloat &
	creaseAngle ()
	{ return *fields .creaseAngle; }

	virtual
	const SFFloat &
	creaseAngle () const
	{ return *fields .creaseAngle; }

	virtual
	MFInt32 &
	texCoordIndex ()
	{ return *fields .texCoordIndex; }

	virtual
	const MFInt32 &
	texCoordIndex () const
	{ return *fields .texCoordIndex; }

	virtual
	MFInt32 &
	colorIndex ()
	{ return *fields .colorIndex; }

	virtual
	const MFInt32 &
	colorIndex () const
	{ return *fields .colorIndex; }

	virtual
	MFInt32 &
	normalIndex ()
	{ return *fields .normalIndex; }

	virtual
	const MFInt32 &
	normalIndex () const
	{ return *fields .normalIndex; }

	virtual
	MFInt32 &
	coordIndex ()
	{ return *fields .coordIndex; }

	virtual
	const MFInt32 &
	coordIndex () const
	{ return *fields .coordIndex; }

	///  @name Member access

	size_t
	getVertexTexCoordIndex (const size_t index) const;

	size_t
	getVertexColorIndex (const size_t index) const;

	size_t
	getFaceColorIndex (const size_t index) const;

	size_t
	getVertexNormalIndex (const size_t index) const;

	size_t
	getFaceNormalIndex (const size_t index) const;

	///  @name Operations

	virtual
	bool
	isColorPerVertex () const;

	virtual
	bool
	isNormalPerVertex () const;

	virtual
	MFInt32
	getColorIndex (const bool colorPerVertex) const;

	virtual
	MFInt32
	getNormalIndex (const bool normalPerVertex) const;

	virtual
	void
	addColors ();

	virtual
	void
	addTexCoords ();

	virtual
	void
	addNormals () override;

	void
	rebuildIndices ();

	std::map <int32_t, int32_t>
	rebuildColor ();

	std::map <int32_t, int32_t>
	rebuildTexCoord ();

	std::map <int32_t, int32_t>
	rebuildNormal ();

	std::map <int32_t, int32_t>
	rebuildCoord ();

	void
	mergePoints (const double distance);

	virtual
	NodeType
	getPrimitiveType () const final override
	{ return X3DConstants::IndexedLineSet; }

	virtual
	SFNode
	toPrimitive () const override;

	///  @name Destruction

	virtual
	~IndexedFaceSet () override;


protected:

	///  @name Member types

	using Vertices = std::vector <size_t>;

	///  @name Operations

	Vector3f
	getPolygonNormal (const Vertices & vertices) const;

	double
	getPolygonArea (const Vertices & vertices) const;


private:

	///  @name Member types

	using ElementArray = std::vector <Vertices>;

	struct Polygon
	{
		Vertices     vertices;
		ElementArray elements;
		size_t       face;
	};

	using PolygonArray = std::vector <Polygon>;
	using Tessellator  = math::tessellator <double, size_t>;

	///  @name Operations

	virtual
	void
	build () final override;

	std::vector <Vector3f>
	createNormals (const PolygonArray & polygons) const;

	void
	buildNormals (const PolygonArray & polygons);

	void
	tessellate (const bool convex, PolygonArray & polygons, size_t & numVertices) const;

	void
	tessellate (const std::unique_ptr <Tessellator> & tessellator, PolygonArray & polygons) const;

	bool
	isPerVertex (const MFInt32 & index) const;

	MFInt32
	getIndex (const bool perVertex, const bool currentPerVertex, const MFInt32 & currentIndices) const;

	void
	rebuildIndices (const size_t faceIndex,
	                const size_t faceNumber,
	                const size_t count,
	                std::vector <size_t> & indices,
	                std::vector <size_t> & faceNumbers);

	void
	rebuildTexCoord (const X3DPtr <X3DTextureCoordinateNode> & texCoord, const std::map <int32_t, int32_t> & map);

	///  @name Static members

	static const ComponentType component;
	static const std::string   typeName;
	static const std::string   containerField;

	///  @name Members

	struct Fields
	{
		Fields ();

		SFBool* const convex;
		SFFloat* const creaseAngle;
		MFInt32* const texCoordIndex;
		MFInt32* const colorIndex;
		MFInt32* const normalIndex;
		MFInt32* const coordIndex;
	};

	Fields fields;

};

} // X3D
} // titania

#endif
