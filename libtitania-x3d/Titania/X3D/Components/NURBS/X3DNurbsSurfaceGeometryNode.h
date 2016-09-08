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

#ifndef __TITANIA_X3D_COMPONENTS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_H__
#define __TITANIA_X3D_COMPONENTS_NURBS_X3DNURBS_SURFACE_GEOMETRY_NODE_H__

#include "../NURBS/X3DParametricGeometryNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

namespace titania {
namespace X3D {

class X3DNurbsSurfaceGeometryNode :
	public X3DParametricGeometryNode
{
public:

	///  @name Fields

	SFInt32 &
	uTessellation ()
	{ return *fields .uTessellation; }

	const SFInt32 &
	uTessellation () const
	{ return *fields .uTessellation; }

	SFInt32 &
	vTessellation ()
	{ return *fields .vTessellation; }

	const SFInt32 &
	vTessellation () const
	{ return *fields .vTessellation; }

	SFBool &
	uClosed ()
	{ return *fields .uClosed; }

	const SFBool &
	uClosed () const
	{ return *fields .uClosed; }

	SFBool &
	vClosed ()
	{ return *fields .vClosed; }

	const SFBool &
	vClosed () const
	{ return *fields .vClosed; }

	SFBool &
	solid ()
	{ return *fields .solid; }

	const SFBool &
	solid () const
	{ return *fields .solid; }

	SFInt32 &
	uOrder ()
	{ return *fields .uOrder; }

	const SFInt32 &
	uOrder () const
	{ return *fields .uOrder; }

	SFInt32 &
	vOrder ()
	{ return *fields .vOrder; }

	const SFInt32 &
	vOrder () const
	{ return *fields .vOrder; }

	SFInt32 &
	uDimension ()
	{ return *fields .uDimension; }

	const SFInt32 &
	uDimension () const
	{ return *fields .uDimension; }

	SFInt32 &
	vDimension ()
	{ return *fields .vDimension; }

	const SFInt32 &
	vDimension () const
	{ return *fields .vDimension; }

	MFDouble &
	uKnot ()
	{ return *fields .uKnot; }

	const MFDouble &
	uKnot () const
	{ return *fields .uKnot; }

	MFDouble &
	vKnot ()
	{ return *fields .vKnot; }

	const MFDouble &
	vKnot () const
	{ return *fields .vKnot; }

	MFDouble &
	weight ()
	{ return *fields .weight; }

	const MFDouble &
	weight () const
	{ return *fields .weight; }

	SFNode &
	texCoord ()
	{ return *fields .texCoord; }

	const SFNode &
	texCoord () const
	{ return *fields .texCoord; }

	SFNode &
	controlPoint ()
	{ return *fields .controlPoint; }

	const SFNode &
	controlPoint () const
	{ return *fields .controlPoint; }


protected:

	///  @name Construction

	X3DNurbsSurfaceGeometryNode ();

	virtual
	void
	initialize () override;

	///  @name Operations

	virtual
	void
	trimSurface (GLUnurbs*) const
	{ }


private:

	///  @name Event handlers

	void
	set_texCoord ();

	void
	set_controlPoint ();

	///  @name Operations

	size_t
	getUTessellation () const;

	size_t
	getVTessellation () const;

	std::vector <float>
	getKnots (const MFDouble &, const int32_t, const int32_t) const;

	virtual
	void
	build () final override;

	///  @name Tessellator
	
	static
	void
	tessBeginData (GLenum, X3DNurbsSurfaceGeometryNode*);

	static
	void
	tessTexCoordData (GLfloat*, X3DNurbsSurfaceGeometryNode*);

	static
	void
	tessNormalData (GLfloat*, X3DNurbsSurfaceGeometryNode*);

	static
	void
	tessVertexData (GLfloat*, X3DNurbsSurfaceGeometryNode*);

	static
	void
	tessEndData (X3DNurbsSurfaceGeometryNode*);

	static
	void
	tessError (GLenum);

	///  @name Members
	
	struct Fields
	{
		Fields ();

		SFInt32* const uTessellation;
		SFInt32* const vTessellation;
		SFBool* const uClosed;
		SFBool* const vClosed;
		SFBool* const solid;
		SFInt32* const uOrder;
		SFInt32* const vOrder;
		SFInt32* const uDimension;
		SFInt32* const vDimension;
		MFDouble* const uKnot;
		MFDouble* const vKnot;
		MFDouble* const weight;
		SFNode* const texCoord;
		SFNode* const controlPoint;
	};

	Fields fields;
	
	X3DPtr <X3DTextureCoordinateNode> texCoordNode;
	X3DPtr <X3DCoordinateNode>        controlPointNode;

	GLenum type;
	std::vector <Vector4f> texCoords;
	std::vector <Vector3f> normals;
	std::vector <Vector3d> vertices;
	
	GLenum vertexMode;
	size_t numVertices;

};

} // X3D
} // titania

#endif
