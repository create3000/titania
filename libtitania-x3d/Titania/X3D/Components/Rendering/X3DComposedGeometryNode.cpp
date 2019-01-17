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

#include "X3DComposedGeometryNode.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Normal.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Rendering/X3DNormalNode.h"
#include "../Shaders/X3DVertexAttributeNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"

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
	    attribNodes (),
	      colorNode (),
	   texCoordNode (),
	     normalNode (),
	      coordNode (),
	    transparent (false)
{
	addType (X3DConstants::X3DComposedGeometryNode);

	addChildObjects (attribNodes,
	                 colorNode,
	                 texCoordNode,
	                 normalNode,
	                 coordNode);
}

void
X3DComposedGeometryNode::initialize ()
{
	X3DGeometryNode::initialize ();

	attrib ()   .addInterest (&X3DComposedGeometryNode::set_attrib, this);
	color ()    .addInterest (&X3DComposedGeometryNode::set_color, this);
	texCoord () .addInterest (&X3DComposedGeometryNode::set_texCoord, this);
	normal ()   .addInterest (&X3DComposedGeometryNode::set_normal, this);
	coord ()    .addInterest (&X3DComposedGeometryNode::set_coord, this);

	set_attrib ();
	set_color ();
	set_texCoord ();
	set_normal ();
	set_coord ();
}

void
X3DComposedGeometryNode::set_attrib ()
{
	for (const auto & node : attribNodes)
		node -> removeInterest (&X3DComposedGeometryNode::requestRebuild, this);

	std::vector <X3DVertexAttributeNode*> value;

	for (const auto & node : attrib ())
	{
		const auto attribNode = x3d_cast <X3DVertexAttributeNode*> (node);
		
		if (attribNode)
			value .emplace_back (attribNode);
	}

	attribNodes .set (value .cbegin (), value .cend ());

	for (const auto & node : attribNodes)
		node -> addInterest (&X3DComposedGeometryNode::requestRebuild, this);
}

void
X3DComposedGeometryNode::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (&X3DComposedGeometryNode::requestRebuild,   this);
		colorNode -> removeInterest (&X3DComposedGeometryNode::set_transparency, this);
	}

	colorNode = x3d_cast <X3DColorNode*> (color ());

	if (colorNode)
	{
		colorNode -> addInterest (&X3DComposedGeometryNode::requestRebuild,   this);
		colorNode -> addInterest (&X3DComposedGeometryNode::set_transparency, this);
		
		set_transparency ();
	}
	else
		transparent = false;
}

void
X3DComposedGeometryNode::set_transparency ()
{
	transparent = colorNode -> isTransparent ();
}

void
X3DComposedGeometryNode::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (&X3DComposedGeometryNode::requestRebuild, this);

	texCoordNode = x3d_cast <X3DTextureCoordinateNode*> (texCoord ());

	if (texCoordNode)
		texCoordNode -> addInterest (&X3DComposedGeometryNode::requestRebuild, this);

	setTextureCoordinate (texCoordNode);
}

void
X3DComposedGeometryNode::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (&X3DComposedGeometryNode::requestRebuild, this);

	normalNode = x3d_cast <X3DNormalNode*> (normal ());

	if (normalNode)
		normalNode -> addInterest (&X3DComposedGeometryNode::requestRebuild, this);
}

void
X3DComposedGeometryNode::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (&X3DComposedGeometryNode::requestRebuild, this);

	coordNode = x3d_cast <X3DCoordinateNode*> (coord ());

	if (coordNode)
		coordNode -> addInterest (&X3DComposedGeometryNode::requestRebuild, this);
}

void
X3DComposedGeometryNode::build (const size_t vertexCount, size_t size)
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	// Set size to a multiple of vertexCount.

	size -= size % vertexCount;

	// Vertex attribute

	std::vector <std::vector <float>> attribArrays (getAttrib () .size ());

	for (size_t a = 0, size = getAttrib () .size (); a < size; ++ a)
		attribArrays [a] .reserve (size);

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

			for (size_t a = 0, size = getAttrib () .size (); a < size; ++ a)
				getAttrib () [a] -> addValue (attribArrays [a], index);

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
	setAttribs (getAttrib (), attribArrays);
}

void
X3DComposedGeometryNode::buildNormals (const size_t vertexCount, const size_t size)
{
	getNormals () = createNormals (vertexCount, size);
}

void
X3DComposedGeometryNode::addNormals (const size_t vertexCount, size_t size)
{
	// Set size to a multiple of vertexCount.

	size -= size % vertexCount;

	const auto normals    = createNormals (vertexCount, size);
	const auto normalNode = getExecutionContext () -> createNode <Normal> ();

	normalNode -> vector () .assign (normals .cbegin (), normals .cend ());

	normal () = normalNode;

	getExecutionContext () -> realize ();
}

std::vector <Vector3f>
X3DComposedGeometryNode::createNormals (const size_t vertexCount, const size_t size) const
{
	std::vector <Vector3f> normals = createFaceNormals (vertexCount, size);

	if (normalPerVertex ())
	{
		NormalIndex normalIndex;

		for (size_t i = 0; i < size; ++ i)
			normalIndex [getIndex (i)] .emplace_back (i);

		refineNormals (normalIndex, normals, pi <double>, true);
	}

	return normals;
}

std::vector <Vector3f>
X3DComposedGeometryNode::createFaceNormals (const size_t vertexCount, const size_t size) const
{
	std::vector <Vector3f> normals;

	normals .reserve (size);

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

		normals .resize (normals .size () + vertexCount, vertexCount == 3 ? normal : normalize (normal));
	}

	if (not ccw ())
		std::for_each (normals .begin (), normals .end (), std::mem_fn (&Vector3f::negate));

	return normals;
}

SFNode
X3DComposedGeometryNode::toPrimitive (const size_t vertexCount, size_t size) const
{
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> colorPerVertex ()  = colorPerVertex ();
	geometry -> normalPerVertex () = normalPerVertex ();
	geometry -> solid ()           = solid ();
	geometry -> ccw ()             = ccw ();

	geometry -> attrib ()   = attrib ();
	geometry -> fogCoord () = fogCoord ();
	geometry -> color ()    = color ();
	geometry -> texCoord () = texCoord ();
	geometry -> normal ()   = normal ();
	geometry -> coord ()    = coord ();

	// Set size to a multiple of vertexCount.

	size -= size % vertexCount;

	// Fill GeometryNode

	for (size_t i = 0; i < size; )
	{
		for (size_t v = 0; v < vertexCount; ++ v, ++ i)
			geometry -> coordIndex () .emplace_back (getIndex (i));

		geometry -> coordIndex () .emplace_back (-1);
	}

	return geometry;
}

X3DComposedGeometryNode::~X3DComposedGeometryNode ()
{ }

} // X3D
} // titania
