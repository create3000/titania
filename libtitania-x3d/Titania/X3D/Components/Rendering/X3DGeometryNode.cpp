/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "X3DGeometryNode.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Layering/X3DLayerNode.h"

#include <cassert>

namespace titania {
namespace X3D {

X3DGeometryNode::X3DGeometryNode () :
	      X3DNode (),
	         bbox (),
	  attribNodes (),
	attribBuffers (),
	       colors (),
	 texCoordNode (),
	    texCoords (),
	      normals (),
	     vertices (),
	        solid (true),
	    frontFace (GL_CCW),
	     elements ()
{
	addNodeType (X3DConstants::X3DGeometryNode);
	
	addChildren (texCoordNode);
}

void
X3DGeometryNode::setup ()
{
	X3DNode::setup ();

	addInterest (this, &X3DGeometryNode::update);

	update ();
}

Box3f
X3DGeometryNode::createBBox ()
{
	return Box3f (vertices .begin (), vertices .end (), math::iterator_type ());
}

void
X3DGeometryNode::setAttribs (const X3DPtrArray <X3DVertexAttributeNode> & nodes, const std::vector <std::vector <float>> & attribs)
{
	if (attribs .empty ())
		return;

	const size_t numAttribs = nodes .size ();

	attribNodes .assign (nodes .begin (), nodes .end ());
	attribBuffers .resize (numAttribs);

	glGenBuffers (numAttribs, attribBuffers .data ());

	for (size_t i = 0; i < numAttribs; ++ i)
	{
		glBindBuffer (GL_ARRAY_BUFFER, attribBuffers [i]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * attribs [i] .size (), attribs [i] .data (), GL_STATIC_COPY);
	}

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::setTextureCoordinate (X3DTextureCoordinateNode* const value)
{
	if (value)
		texCoordNode .set (value);

	else
		texCoordNode .set (getBrowser () -> getTexCoord ());
}

bool
X3DGeometryNode::intersect (Line3f line, std::vector <IntersectionPtr> & intersections) const
{
	bool intersected = false;

	if (getBBox () .intersect (line))
	{
		const Matrix4f matrix          = getMatrix ();                           // Get the current matrix from screen nodes.
		const Matrix4f modelViewMatrix = matrix * getModelViewMatrix () .get (); // This matrix is for clipping only.

		line *= ~matrix;

		size_t first = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
					{
						for (size_t i = first, size = first + element .count; i < size; i += 3)
						{
							intersected |= intersect (line, i, i + 1, i + 2, modelViewMatrix, intersections);
						}

						break;
					}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						intersected |= intersect (line, i, i + 1, i + 2, modelViewMatrix, intersections);
						intersected |= intersect (line, i, i + 2, i + 3, modelViewMatrix, intersections);
					}

					break;
				}
				case GL_QUAD_STRIP:
				{
					for (size_t i = first, size = first + element .count - 2; i < size; i += 2)
					{
						intersected |= intersect (line, i,     i + 1, i + 2, modelViewMatrix, intersections);
						intersected |= intersect (line, i + 1, i + 3, i + 2, modelViewMatrix, intersections);
					}

					break;
				}
				case GL_POLYGON:
				{
					for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
					{
						intersected |= intersect (line, first, i, i + 1, modelViewMatrix, intersections);
					}

					break;
				}
				default:
					break;
			}

			first += element .count;
		}
	}

	return intersected;
}

bool
X3DGeometryNode::intersect (const Line3f & line, size_t i1, size_t i2, size_t i3, const Matrix4f & modelViewMatrix, std::vector <IntersectionPtr> & intersections) const
{
	float u, v, t;

	if (line .intersect (vertices [i1], vertices [i2], vertices [i3], u, v, t))
	{
		const float t = 1 - u - v;

		Vector4f     texCoord (0, 0, 0, 1);
		const size_t texCoordSize = texCoords .empty () ? 0 : texCoords [0] .size (); // LineGeometry doesn't have texCoords

		if (i1 < texCoordSize)
			texCoord = t * texCoords [0] [i1] + u * texCoords [0] [i2] + v * texCoords [0] [i3];

		Vector3f normal = normalize (t * normals  [i1] + u * normals  [i2] + v * normals  [i3]);
		Vector3f point  = t * vertices [i1] + u * vertices [i2] + v * vertices [i3];

		if (isClipped (point, modelViewMatrix))
			return false;

		intersections .emplace_back (new Intersection { texCoord, normal, point });
		return true;
	}

	return false;
}

bool
X3DGeometryNode::isClipped (const Vector3f & point, const Matrix4f & modelViewMatrix) const
{
	return isClipped (point, modelViewMatrix, getCurrentLayer () -> getLocalObjects ());
}

bool
X3DGeometryNode::isClipped (const Vector3f & point, const Matrix4f & modelViewMatrix, const CollectableObjectArray & localObjects) const
{
	for (const auto & node : localObjects)
	{
		if (node -> isClipped (point, modelViewMatrix))
			return true;
	}

	return false;
}

bool
X3DGeometryNode::intersect (const Sphere3f & sphere, Matrix4f modelViewMatrix, const CollectableObjectArray & localObjects) const
{
	if ((getBBox () * modelViewMatrix) .intersect (sphere))
	{
		modelViewMatrix .mult_left (getMatrix ()); // Multiply by current matrix from screen nodes.

		size_t first = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
					{
						for (size_t i = first, size = first + element .count; i < size; i += 3)
						{
							if (isClipped (vertices [i], modelViewMatrix, localObjects))
								continue;

							if (sphere .intersect (vertices [i] * modelViewMatrix, vertices [i + 1] * modelViewMatrix, vertices [i + 2] * modelViewMatrix))
								return true;
						}

						break;
					}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						if (isClipped (vertices [i], modelViewMatrix, localObjects))
							continue;

						if (sphere .intersect (vertices [i] * modelViewMatrix, vertices [i + 1] * modelViewMatrix, vertices [i + 2] * modelViewMatrix))
							return true;

						if (sphere .intersect (vertices [i] * modelViewMatrix, vertices [i + 2] * modelViewMatrix, vertices [i + 3] * modelViewMatrix))
							return true;
					}

					break;
				}
				case GL_QUAD_STRIP:
				{
					for (size_t i = first, size = first + element .count - 2; i < size; i += 4)
					{
						if (isClipped (vertices [i], modelViewMatrix, localObjects))
							continue;

						if (sphere .intersect (vertices [i] * modelViewMatrix, vertices [i + 1] * modelViewMatrix, vertices [i + 2] * modelViewMatrix))
							return true;

						if (sphere .intersect (vertices [i + 1] * modelViewMatrix, vertices [i + 3] * modelViewMatrix, vertices [i + 2] * modelViewMatrix))
							return true;
					}

					break;
				}
				case GL_POLYGON:
				{
					for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
					{
						if (isClipped (vertices [first], modelViewMatrix, localObjects))
							continue;

						if (sphere .intersect (vertices [first] * modelViewMatrix, vertices [i] * modelViewMatrix, vertices [i + 1] * modelViewMatrix))
							return true;
					}

					break;
				}
				default:
					break;
			}

			first += element .count;
		}
	}

	return false;
}

void
X3DGeometryNode::triangulate (std::vector <Color4f> & _colors, TexCoordArray & _texCoords, std::vector <Vector3f> & _normals, std::vector <Vector3f> & _vertices) const
{
	size_t first = 0;

	for (const auto & element : elements)
	{
		switch (element .vertexMode)
		{
			case GL_TRIANGLES :
				{
					for (size_t i = first, size = first + element .count; i < size; i += 3)
					{
						triangulate (i, i + 1, i + 2, _colors, _texCoords, _normals, _vertices);
					}

					break;
				}
			case GL_QUADS:
			{
				for (size_t i = first, size = first + element .count; i < size; i += 4)
				{
					triangulate (i, i + 1, i + 2, _colors, _texCoords, _normals, _vertices);
					triangulate (i, i + 2, i + 3, _colors, _texCoords, _normals, _vertices);
				}

				break;
			}
			case GL_QUAD_STRIP:
			{
				for (size_t i = first, size = first + element .count - 2; i < size; i += 4)
				{
					triangulate (i + 0, i + 1, i + 2, _colors, _texCoords, _normals, _vertices);
					triangulate (i + 1, i + 3, i + 2, _colors, _texCoords, _normals, _vertices);
				}

				break;
			}
			case GL_POLYGON:
			{
				for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
				{
					triangulate (first, i, i + 1, _colors, _texCoords, _normals, _vertices);
				}

				break;
			}
			default:
				break;
		}

		first += element .count;
	}
}

void
X3DGeometryNode::triangulate (size_t i1, size_t i2, size_t i3, std::vector <Color4f> & _colors, TexCoordArray & _texCoords, std::vector <Vector3f> & _normals, std::vector <Vector3f> & _vertices) const
{
	if (not colors .empty ())
	{
		_colors .emplace_back (colors [i1]);
		_colors .emplace_back (colors [i2]);
		_colors .emplace_back (colors [i3]);
	}

	for (size_t t = 0, size = texCoords .size (); t < size; ++ t)
	{
		_texCoords [t] .emplace_back (texCoords [t] [i1]);
		_texCoords [t] .emplace_back (texCoords [t] [i2]);
		_texCoords [t] .emplace_back (texCoords [t] [i3]);
	}

	_normals .emplace_back (normals [i1]);
	_normals .emplace_back (normals [i2]);
	_normals .emplace_back (normals [i3]);

	_vertices .emplace_back (vertices [i1]);
	_vertices .emplace_back (vertices [i2]);
	_vertices .emplace_back (vertices [i3]);
}

void
X3DGeometryNode::buildTexCoords ()
{
	getTexCoords () .emplace_back ();
	getTexCoords () .reserve (getVertices () .size ());

	Vector3f min;
	float    Ssize;
	int      Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	getTexCoords () [0] .reserve (getVertices () .size ());

	for (const auto & vertex : getVertices ())
	{
		getTexCoords () [0] .emplace_back ((vertex [Sindex] - min [Sindex]) / Ssize,
		                                   (vertex [Tindex] - min [Tindex]) / Ssize,
		                                   0,
		                                   1);
	}
}

void
X3DGeometryNode::getTexCoordParams (Vector3f & min, float & Ssize, int & Sindex, int & Tindex)
{
	const Box3f bbox = getBBox ();
	const auto  size = bbox .size ();

	min = bbox .center () - size / 2.0f;

	const float Xsize = size .x ();
	const float Ysize = size .y ();
	const float Zsize = size .z ();

	if ((Xsize >= Ysize) and (Xsize >= Zsize))
	{
		// X size largest
		Ssize = Xsize; Sindex = 0;

		if (Ysize >= Zsize)
			Tindex = 1;
		else
			Tindex = 2;
	}
	else if ((Ysize >= Xsize) and (Ysize >= Zsize))
	{
		// Y size largest
		Ssize = Ysize; Sindex = 1;

		if (Xsize >= Zsize)
			Tindex = 0;
		else
			Tindex = 2;
	}
	else
	{
		// Z is the largest
		Ssize = Zsize; Sindex = 2;

		if (Xsize >= Ysize)
			Tindex = 0;
		else
			Tindex = 1;
	}
}

/*
 *  normalIndex: a map of vertices with an array of the normals associated to this vertex
 *
 *    [vertexIndex] -> [normalIndex1, normalIndex2, ... ]
 *
 *  normals: an array of a face normal for each vertex
 *
 *  Assume we have two polygons where two points (p2, p3) share more than one vertex.
 *
 *  p1                        p3
 *     v1 ------------- v3 v5
 *      | n1         n3  /|
 *      |              /  |
 *      |            / n5 |
 *      |          /      |
 *      |        /        |
 *      |      /          |
 *      | n2 /            |
 *      |  /  n4          |
 *      |/            n6  |
 *     v2 v4 ------------- v6
 *  p2                        p4
 *
 *  For these two polygons the normalIndex and the normal array would look like this:
 *
 *  normalIndex:
 *    [p1] -> [n1]
 *    [p2] -> [n2, n4]
 *    [p3] -> [n3, n5]
 *    [p4] -> [n6]
 *
 *  normals:
 *    [n1, n2, n3, n4, n5, n6]
 */

void
X3DGeometryNode::refineNormals (const NormalIndex & normalIndex, std::vector <Vector3f> & normals, float creaseAngle, bool ccw) const
{
	if (not ccw)
		std::for_each (normals .begin (), normals .end (), std::mem_fn (&Vector3f::negate));

	if (creaseAngle == 0.0f)
		return;

	const float cosCreaseAngle = std::cos (creaseAngle);

	std::vector <Vector3f> _normals (normals .size ());

	for (const auto & point : normalIndex)
	{
		for (const auto & index : point .second)
		{
			Vector3f         n;
			const Vector3f & m = normals [index];

			for (const auto & index : point .second)
			{
				if (dot (normals [index], m) >= cosCreaseAngle)
					n += normals [index];
			}

			_normals [index] = normalize (n);
		}
	}

	normals .swap (_normals);
}

/*
 * Adds all vertices, normals and texCoordss mirrors onto the XY-plane to the arrays.
 * If the shape is not convext, the this not convex one point must be the first point in the arrays.
 */

void
X3DGeometryNode::addMirrorVertices (GLenum vertexMode, const bool convex)
{
	auto & texCoords = this -> texCoords [0];

	addElements (vertexMode, getVertices () .size ());

	switch (vertexMode)
	{
		case GL_QUAD_STRIP:
		{
			for (int32_t i = texCoords .size () - 2; i >= 0; i -= 2)
			{
				const auto & texCoord1 = texCoords [i];
				const auto & texCoord0 = texCoords [i + 1];
				texCoords .emplace_back (1 - texCoord1 .x (), texCoord1 .y (), texCoord1 .z (), 1);
				texCoords .emplace_back (1 - texCoord0 .x (), texCoord0 .y (), texCoord0 .z (), 1);
			}

			for (int32_t i = getVertices () .size () - 2; i >= 0; i -= 2)
			{
				getNormals  () .emplace_back (0, 0, -1);
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (getVertices () [i]);
				getVertices () .emplace_back (getVertices () [i + 1]);
			}

			break;
		}

		default:
		{
			if (not convex)
			{
				texCoords .emplace_back (1 - texCoords .front () .x (), texCoords .front () .y (), texCoords .front () .z (), texCoords .front () .w ());
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (getVertices () .front ());
			}

			const int32_t offset = convex ? 0 : 1;

			for (int32_t i = getVertices () .size () - 1 - offset; i >= offset; -- i)
			{
				texCoords .emplace_back (1 - texCoords [i] .x (), texCoords [i] .y (), texCoords [i] .z (), texCoords [i] .w ());
				getNormals  () .emplace_back (0, 0, -1);
				getVertices () .emplace_back (getVertices () [i]);
			}
		}
	}
}

void
X3DGeometryNode::update ()
{
	clear ();
	build ();

	bbox = createBBox ();

	if (not isLineGeometry ())
	{
		// Autogenerate texCoords if not specified.

		if (texCoords .empty ())
			buildTexCoords ();
	}
}

void
X3DGeometryNode::clear ()
{
	if (not attribBuffers .empty ())
	{
		glDeleteBuffers (attribBuffers .size (), attribBuffers .data ()); // See dispose.

		attribNodes .clear ();
		attribBuffers .clear ();
	}

	colors    .clear ();
	texCoordNode .set (nullptr);
	texCoords .clear ();
	normals   .clear ();
	vertices  .clear ();
	elements  .clear ();
}

void
X3DGeometryNode::draw ()
{
	draw (solid, getBrowser () -> getTexture (), glIsEnabled (GL_LIGHTING));
}

void
X3DGeometryNode::draw (const bool solid, const bool texture, const bool lighting)
{
	if (solid)
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	glFrontFace (ModelViewMatrix4f () .determinant3 () > 0 ? frontFace : (frontFace == GL_CCW ? GL_CW : GL_CCW));

	if (not attribNodes .empty ())
	{
		GLint program = 0;

		glGetIntegerv (GL_CURRENT_PROGRAM, &program);

		if (program)
		{
			for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
				attribNodes [i] -> enable (program, attribBuffers [i]);

			glBindBuffer (GL_ARRAY_BUFFER, 0);
		}
	}

	if (not colors .empty ())
	{
		if (lighting)
			glEnable (GL_COLOR_MATERIAL);

		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (4, GL_FLOAT, 0, colors .data ());
	}

	if (texture)
	{
		if (texCoordNode)
			texCoordNode -> enable (texCoords);
	}

	if (lighting /* or shader */)
	{
		if (not normals .empty ())
		{
			glEnableClientState (GL_NORMAL_ARRAY);
			glNormalPointer (GL_FLOAT, 0, normals .data ());
		}
	}

	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices .data ());

	size_t first = 0;

	for (const auto & element : elements)
	{
		glDrawArrays (element .vertexMode, first, element .count);
		first += element .count;
	}

	// VertexAttribs

	if (not attribNodes .empty ())
	{
		GLint program = 0;

		glGetIntegerv (GL_CURRENT_PROGRAM, &program);

		if (program)
		{
			for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
				attribNodes [i] -> disable (program);
		}
	}

	// Texture

	if (texture)
	{
		if (texCoordNode)
			texCoordNode -> disable ();
	}

	// Other arrays

	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);
}

void
X3DGeometryNode::dispose ()
{
	if (not attribBuffers .empty ())
		glDeleteBuffers (attribBuffers .size (), attribBuffers .data ()); // See clear.

	X3DNode::dispose ();
}

} // X3D
} // titania
