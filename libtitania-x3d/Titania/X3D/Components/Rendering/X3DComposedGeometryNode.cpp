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

#include "X3DComposedGeometryNode.h"

#include "../../Bits/Cast.h"

namespace titania {
namespace X3D {

X3DComposedGeometryNode::Fields::Fields () :
	 colorPerVertex (new SFBool (true)),
	normalPerVertex (new SFBool (true)),
	          solid (new SFBool (true)),
	            ccw (new SFBool (true)),
	         attrib (new MFNode ()),
	       fogCoord (new SFNode ()),
	          color (new SFNode ()),
	       texCoord (new SFNode ()),
	         normal (new SFNode ()),
	          coord (new SFNode ())
{ }

X3DComposedGeometryNode::X3DComposedGeometryNode () :
	X3DGeometryNode (),
	         fields (),
	      colorNode (),
	   texCoordNode (),
	     normalNode (),
	      coordNode (),
	    transparent (false)
{
	addNodeType (X3DConstants::X3DComposedGeometryNode);

	addChildren (colorNode,
	             texCoordNode,
	             normalNode,
	             coordNode);
}

void
X3DComposedGeometryNode::initialize ()
{
	X3DGeometryNode::initialize ();

	color ()    .addInterest (this, &X3DComposedGeometryNode::set_color);
	texCoord () .addInterest (this, &X3DComposedGeometryNode::set_texCoord);
	normal ()   .addInterest (this, &X3DComposedGeometryNode::set_normal);
	coord ()    .addInterest (this, &X3DComposedGeometryNode::set_coord);
	
	set_color ();
	set_texCoord ();
	set_normal ();
	set_coord ();
}

void
X3DComposedGeometryNode::set_color ()
{
	if (colorNode)
		colorNode -> removeInterest (this);

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
		colorNode -> addInterest (this);

	// Transparent

	transparent = colorNode and colorNode -> isTransparent ();
}

void
X3DComposedGeometryNode::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (this);

	texCoordNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (this);
}

void
X3DComposedGeometryNode::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (this);

	normalNode .set (x3d_cast <X3DNormalNode*> (normal ()));

	if (normalNode)
		normalNode -> addInterest (this);
}

void
X3DComposedGeometryNode::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this);

	coordNode .set (x3d_cast <X3DCoordinateNode*> (coord ()));

	if (coordNode)
		coordNode -> addInterest (this);
}

Box3f
X3DComposedGeometryNode::createBBox ()
{
	if (getCoord ())
		return getCoord () -> getBBox ();

	return Box3f ();
}

void
X3DComposedGeometryNode::buildPolygons (const size_t vertexCount, size_t size)
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	// Set size to a multiple of vertexCount.

	size -= size % vertexCount;

	// Color

	if (colorNode)
		getColors () .reserve (size);

	// TextureCoordinate

	if (texCoordNode)
		texCoordNode -> init (getTexCoords (), size);

	// Normal

	getNormals () .reserve (size);

	// Vertices

	getVertices () .reserve (size);

	// Fill GeometryNode

	for (size_t i = 0, face = 0; i < size; ++ face)
	{
		for (size_t v = 0; v < vertexCount; ++ v, ++ i)
		{
			const size_t index = getIndex (i);

			if (colorNode)
			{
				if (colorPerVertex ())
					colorNode -> addColor (getColors (), index);

				else
					colorNode -> addColor (getColors (), face);
			}

			if (texCoordNode)
				texCoordNode -> addTexCoord (getTexCoords (), index);

			if (normalNode)
			{
				if (normalPerVertex ())
					normalNode -> addVector (getNormals (), index);

				else
					normalNode -> addVector (getNormals (), face);
			}

			coordNode -> addVertex (getVertices (), index);
		}
	}

	// Autogenerate normals if not specified.

	if (not normalNode)
		buildNormals (vertexCount, size);

	addElements (getVertexMode (vertexCount), getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());
	setTextureCoordinate (texCoordNode);
}

void
X3DComposedGeometryNode::buildNormals (const size_t vertexCount, const size_t size)
{
	buildFaceNormals (vertexCount, size);

	if (normalPerVertex ())
	{
		NormalIndex normalIndex;

		for (size_t i = 0; i < size; ++ i)
			normalIndex [getIndex (i)] .emplace_back (i);

		refineNormals (normalIndex, getNormals (), M_PI, true);
	}
}

void
X3DComposedGeometryNode::buildFaceNormals (const size_t vertexCount, const size_t size)
{
	for (size_t index = 0; index < size; index += vertexCount)
	{
		Vector3f normal;

		for (size_t i = 1, size = vertexCount - 1; i < size; ++ i)
		{
			// Determine polygon normal.
			normal += coordNode -> getNormal (getIndex (index),
			                                  getIndex (index + i),
			                                  getIndex (index + i + 1));
		}

		getNormals () .resize (getNormals () .size () + vertexCount, vertexCount == 3 ? normal : normalize (normal));
	}

	if (not ccw ())
		std::for_each (getNormals () .begin (), getNormals () .end (), std::mem_fn (&Vector3f::negate));
}

void
X3DComposedGeometryNode::dispose ()
{
	colorNode    .dispose ();
	texCoordNode .dispose ();
	normalNode   .dispose ();
	coordNode    .dispose ();

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
