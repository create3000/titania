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

#include "../../Browser/ContextLock.h"
#include "../../Browser/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/ShapeContainer.h"
#include "../Layering/X3DLayerNode.h"
#include "../Shaders/X3DShaderNode.h"

namespace titania {
namespace X3D {

X3DGeometryNode::X3DGeometryNode () :
	          X3DNode (),
	     cameraObject (),
	             bbox (),
	      attribNodes (),
	           colors (),
#ifndef SHADER_PIPELINE
	     texCoordNode (),
#endif
	        texCoords (),
	          normals (),
	         vertices (),
	            solid (true),
	        frontFace (GL_CCW),
	         elements (),
	  attribBufferIds (),
	    colorBufferId (0),
	texCoordBufferIds (),
	   normalBufferId (0),
	   vertexBufferId (0)

{
	addType (X3DConstants::X3DGeometryNode);

	addChildren (cameraObject);

	#ifndef SHADER_PIPELINE
	addChildren (texCoordNode);
	#endif
}

void
X3DGeometryNode::setup ()
{
	X3DNode::setup ();

	#ifndef SHADER_PIPELINE
	texCoordNode .set (getBrowser () -> getDefaultTexCoord ());
	#endif

	if (glXGetCurrentContext ())
	{
		glGenBuffers (1, &colorBufferId);
		glGenBuffers (1, &normalBufferId);
		glGenBuffers (1, &vertexBufferId);

		addInterest (this, &X3DGeometryNode::update);

		update ();
	}
}

void
X3DGeometryNode::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	#ifndef SHADER_PIPELINE
	if (isInitialized ())
	{
		if (texCoordNode == getBrowser () -> getDefaultTexCoord ())
			texCoordNode .set (executionContext -> getBrowser () -> getDefaultTexCoord ());
	}
	#endif

	X3DNode::setExecutionContext (executionContext);
}

void
X3DGeometryNode::setCameraObject (const bool value)
{
	if (value not_eq cameraObject)
	   cameraObject = value;
}

Box3d
X3DGeometryNode::createBBox ()
{
	return Box3d (vertices .begin (), vertices .end (), math::iterator_type ());
}

void
X3DGeometryNode::setAttribs (const X3DPtrArray <X3DVertexAttributeNode> & nodes, const std::vector <std::vector <float>> & attribs)
{
	if (attribs .empty ())
		return;

	const size_t numAttribs = nodes .size ();

	attribNodes .assign (nodes .begin (), nodes .end ());
	attribBufferIds .resize (numAttribs);

	glGenBuffers (numAttribs, attribBufferIds .data ());

	for (size_t i = 0; i < numAttribs; ++ i)
	{
		glBindBuffer (GL_ARRAY_BUFFER, attribBufferIds [i]);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * attribs [i] .size (), attribs [i] .data (), GL_STATIC_COPY);
	}

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

#ifndef SHADER_PIPELINE
void
X3DGeometryNode::setTextureCoordinate (X3DTextureCoordinateNode* const value)
{
	if (value)
		texCoordNode .set (value);

	else
		texCoordNode .set (getBrowser () -> getDefaultTexCoord ());
}
#endif

bool
X3DGeometryNode::intersects (Line3d line, std::vector <IntersectionPtr> & intersections) const
{
	try
	{
		if (isLineGeometry ())
			return false;

		const Matrix4d matrix          = getMatrix ();                           // Get the current matrix from screen nodes.
		const Matrix4d modelViewMatrix = matrix * getModelViewMatrix () .get (); // This matrix is for clipping only.

		line *= ~matrix;

		if (not bbox .intersects (line))
			return false;

		bool   intersected = false;
		size_t first       = 0;

		for (const auto & element : elements)
		{
			switch (element .vertexMode)
			{
				case GL_TRIANGLES :
				{
					for (size_t i = first, size = first + element .count; i < size; i += 3)
					{
						intersected |= intersects (line, i, i + 1, i + 2, modelViewMatrix, intersections);
					}

					break;
				}
				case GL_QUADS:
				{
					for (size_t i = first, size = first + element .count; i < size; i += 4)
					{
						intersected |= intersects (line, i, i + 1, i + 2, modelViewMatrix, intersections);
						intersected |= intersects (line, i, i + 2, i + 3, modelViewMatrix, intersections);
					}

					break;
				}
				case GL_QUAD_STRIP:
				{
					for (size_t i = first, size = first + element .count - 2; i < size; i += 2)
					{
						intersected |= intersects (line, i,     i + 1, i + 2, modelViewMatrix, intersections);
						intersected |= intersects (line, i + 1, i + 3, i + 2, modelViewMatrix, intersections);
					}

					break;
				}
				case GL_POLYGON:
				{
					for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
					{
						intersected |= intersects (line, first, i, i + 1, modelViewMatrix, intersections);
					}

					break;
				}
				default:
					break;
			}

			first += element .count;
		}

		return intersected;
	}
	catch (const std::domain_error &)
	{
		return false;
	}
}

bool
X3DGeometryNode::intersects (const Line3d & line,
	                          const size_t i1,
	                          const size_t i2,
	                          const size_t i3,
	                          const Matrix4d & modelViewMatrix,
	                          std::vector <IntersectionPtr> & intersections) const
{
	double u, v, t;

	if (not line .intersects (vertices [i1], vertices [i2], vertices [i3], u, v, t))
		return false;

	t = 1 - u - v;

	Vector4f     texCoord (0, 0, 0, 1);
	const size_t texCoordSize = texCoords .empty () ? 0 : texCoords [0] .size (); // LineGeometry doesn't have texCoords

	if (i1 < texCoordSize)
		texCoord = float (t) * texCoords [0] [i1] + float (u) * texCoords [0] [i2] + float (v) * texCoords [0] [i3];

	const Vector3f normal = normalize (float (t) * normals [i1] + float (u) * normals [i2] + float (v) * normals [i3]);
	const Vector3d point  = t * vertices [i1] + u * vertices [i2] + v * vertices [i3];

	if (isClipped (point, modelViewMatrix))
		return false;

	intersections .emplace_back (new Intersection { texCoord, normal, point * getMatrix (), std::array <Vector3d, 3> { vertices [i1], vertices [i2], vertices [i3] } });
	return true;
}

bool
X3DGeometryNode::isClipped (const Vector3d & point, const Matrix4d & modelViewMatrix) const
{
	return isClipped (point, modelViewMatrix, getCurrentLayer () -> getLocalObjects ());
}

bool
X3DGeometryNode::isClipped (const Vector3d & point,
	                         const Matrix4d & modelViewMatrix,
	                         const CollectableObjectArray & localObjects) const
{
	return std::any_of (localObjects .begin (),
	                    localObjects .end (),
	                    [&point, &modelViewMatrix] (const std::shared_ptr <X3DCollectableObject> & node)
	                    { return node -> isClipped (point, modelViewMatrix); });
}
	                    	                   
std::vector <Vector3d>
X3DGeometryNode::intersects (const std::shared_ptr <FrameBuffer> & frameBuffer,
                       	     const std::shared_ptr <FrameBuffer> & depthBuffer,
                       	     std::vector <IntersectionPtr> & intersections)
{
	try
	{
		std::vector <Vector3d> hitPoints;

		const auto & depth               = depthBuffer -> getDepth ();
		const auto & viewport            = getCurrentLayer () -> getViewVolumeStack () .back () .getScissor ();
		const auto   modelViewProjection = getModelViewMatrix () .get () * getBrowser () -> getProjectionMatrix ();
		const auto   invProjection       = inverse (getBrowser () -> getProjectionMatrix ());

		for (const Vector3d & vertex : vertices)
		{
			const auto screen = ViewVolume::projectPoint (vertex, modelViewProjection, viewport);
			const auto x      = std::floor (screen .x ());
			const auto y      = std::floor (screen .y ());

			if (x < 0 or x >= frameBuffer -> getWidth ())
				continue;

			if (y < 0 or y >= frameBuffer -> getHeight ())
				continue;

			const auto world = vertex * getModelViewMatrix () .get () ;

			if (world .z () > 0)
			   continue;

			switch (getBrowser () -> getRenderingProperties () -> getShading ())
			{
				case ShadingType::GOURAUD:
				case ShadingType::PHONG:
				{
					const auto z      = depth [x + y * depthBuffer -> getWidth ()];
					const auto zWorld = ViewVolume::unProjectPoint (x, y, z, invProjection, viewport);
	
					if (world .z () - zWorld .z () < -0.05)
						continue;
				}
				default:
					break;
			}
	
			const auto index = x * 4 + y * frameBuffer -> getWidth () * 4;
	
			if (frameBuffer -> getPixels () [index])
				hitPoints .emplace_back (vertex);
		}
	
		std::sort (hitPoints .begin (), hitPoints .end ());

		hitPoints .erase (std::unique (hitPoints .begin (), hitPoints .end ()), hitPoints .end ());

		return hitPoints;
	}
	catch (const std::exception & error)
	{
		return std::vector <Vector3d> ();
	}
}

bool
X3DGeometryNode::intersects (CollisionSphere3d sphere, const CollectableObjectArray & localObjects) const
{
	if (not sphere .intersects (getBBox ()))
		return false;
	
	sphere .mult_left (getMatrix ());

	size_t first = 0;

	for (const auto & element : elements)
	{
		switch (element .vertexMode)
		{
			case GL_TRIANGLES :
			{
				for (size_t i = first, size = first + element .count; i < size; i += 3)
				{
					if (isClipped (vertices [i], sphere .matrix (), localObjects))
						continue;

					if (sphere .intersects (vertices [i], vertices [i + 1], vertices [i + 2]))
						return true;
				}

				break;
			}
			case GL_QUADS:
			{
				for (size_t i = first, size = first + element .count; i < size; i += 4)
				{
					if (isClipped (vertices [i], sphere .matrix (), localObjects))
						continue;

					if (sphere .intersects (vertices [i], vertices [i + 1], vertices [i + 2]))
						return true;

					if (sphere .intersects (vertices [i], vertices [i + 2], vertices [i + 3]))
						return true;
				}

				break;
			}
			case GL_QUAD_STRIP:
			{
				for (size_t i = first, size = first + element .count - 2; i < size; i += 4)
				{
					if (isClipped (vertices [i], sphere .matrix (), localObjects))
						continue;

					if (sphere .intersects (vertices [i], vertices [i + 1], vertices [i + 2]))
						return true;

					if (sphere .intersects (vertices [i + 1], vertices [i + 3], vertices [i + 2]))
						return true;
				}

				break;
			}
			case GL_POLYGON:
			{
				for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
				{
					if (isClipped (vertices [first], sphere .matrix (), localObjects))
						continue;

					if (sphere .intersects (vertices [first], vertices [i], vertices [i + 1]))
						return true;
				}

				break;
			}
			default:
				break;
		}

		first += element .count;
	}

	return false;
}

bool
X3DGeometryNode::cut (const Line2d & cutLine)
{
	return false;
}

void
X3DGeometryNode::triangulate (std::vector <Color4f> & colors_,
	                          TexCoordArray & texCoords_,
	                          std::vector <Vector3f> & normals_,
	                          std::vector <Vector3d> & vertices_) const
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
					triangulate (i, i + 1, i + 2, colors_, texCoords_, normals_, vertices_);
				}

				break;
			}
			case GL_QUADS:
			{
				for (size_t i = first, size = first + element .count; i < size; i += 4)
				{
					triangulate (i, i + 1, i + 2, colors_, texCoords_, normals_, vertices_);
					triangulate (i, i + 2, i + 3, colors_, texCoords_, normals_, vertices_);
				}

				break;
			}
			case GL_QUAD_STRIP:
			{
				for (size_t i = first, size = first + element .count - 2; i < size; i += 4)
				{
					triangulate (i + 0, i + 1, i + 2, colors_, texCoords_, normals_, vertices_);
					triangulate (i + 1, i + 3, i + 2, colors_, texCoords_, normals_, vertices_);
				}

				break;
			}
			case GL_POLYGON:
			{
				for (int32_t i = first + 1, size = first + element .count - 1; i < size; ++ i)
				{
					triangulate (first, i, i + 1, colors_, texCoords_, normals_, vertices_);
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
X3DGeometryNode::triangulate (const size_t i1,
	                           const size_t i2,
	                           const size_t i3,
	                           std::vector <Color4f> & colors_,
	                           TexCoordArray & texCoords_,
	                           std::vector <Vector3f> & normals_,
	                           std::vector <Vector3d> & vertices_) const
{
	if (not colors .empty ())
	{
		colors_ .emplace_back (colors [i1]);
		colors_ .emplace_back (colors [i2]);
		colors_ .emplace_back (colors [i3]);
	}

	for (size_t t = 0, size = texCoords .size (); t < size; ++ t)
	{
		texCoords_ [t] .emplace_back (texCoords [t] [i1]);
		texCoords_ [t] .emplace_back (texCoords [t] [i2]);
		texCoords_ [t] .emplace_back (texCoords [t] [i3]);
	}

	normals_ .emplace_back (normals [i1]);
	normals_ .emplace_back (normals [i2]);
	normals_ .emplace_back (normals [i3]);

	vertices_ .emplace_back (vertices [i1]);
	vertices_ .emplace_back (vertices [i2]);
	vertices_ .emplace_back (vertices [i3]);
}

void
X3DGeometryNode::buildTexCoords ()
{
	Vector3d min;
	double   Ssize;
	int      Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	getTexCoords () .emplace_back ();
	getTexCoords () [0] .reserve (getVertices () .size ());

	for (const auto & vertex : getVertices ())
	{
		getTexCoords () [0] .emplace_back ((vertex [Sindex] - min [Sindex]) / Ssize,
		                                   (vertex [Tindex] - min [Tindex]) / Ssize,
		                                   0,
		                                   1);
	}
}

///  Determine the min extent of the bbox, the largest size of the bbox and the two largest indices.
void
X3DGeometryNode::getTexCoordParams (Vector3d & min, double & Ssize, int & Sindex, int & Tindex)
{
	// Thanks to H3D.

	const Box3d bbox = getBBox ();
	const auto  size = bbox .size ();

	min = bbox .center () - size / 2.0;

	const auto Xsize = size .x ();
	const auto Ysize = size .y ();
	const auto Zsize = size .z ();

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
X3DGeometryNode::refineNormals (const NormalIndex & normalIndex,
                                std::vector <Vector3f> & normals,
                                const float creaseAngle,
                                const bool ccw) const
{
	if (not ccw)
		std::for_each (normals .begin (), normals .end (), std::mem_fn (&Vector3f::negate));

	if (creaseAngle == 0.0f)
		return;

	const float cosCreaseAngle = std::cos (creaseAngle);

	std::vector <Vector3f> normals_ (normals .size ());

	std::for_each (normalIndex .begin (), normalIndex .end (), [&normals, &normals_, &cosCreaseAngle] (const NormalIndex::value_type & point)
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

			normals_ [index] = n;
		}
	});

	std::for_each (normals_ .begin (), normals_ .end (), std::mem_fn (&Vector3f::normalize));

	normals .swap (normals_);
}

/*
 * Adds all vertices, normals and texCoordss mirrors onto the XY-plane to the arrays.
 * If the shape is not convext, the this not convex one point must be the first point in the arrays.
 */

void
X3DGeometryNode::addMirrorVertices (const GLenum vertexMode, const bool convex)
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

	const_cast <SFTime &> (getExecutionContext () -> bbox_changed ()) = getCurrentTime ();

	if (not isLineGeometry ())
	{
		// Autogenerate texCoords if not specified.

		if (texCoords .empty ())
			buildTexCoords ();
	}

	transfer ();
}

void
X3DGeometryNode::clear ()
{
	if (not attribBufferIds .empty ())
	{
		glDeleteBuffers (attribBufferIds .size (), attribBufferIds .data ());

		attribNodes .clear ();
		attribBufferIds .clear ();
	}

	if (not texCoordBufferIds .empty ())
		glDeleteBuffers (texCoordBufferIds .size (), texCoordBufferIds .data ());

	colors    .clear ();
	texCoords .clear ();
	normals   .clear ();
	vertices  .clear ();
	elements  .clear ();
}

void
X3DGeometryNode::transfer ()
{
	if (not colors .empty ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * colors .size (), colors .data (), GL_STATIC_COPY);
	}

	if (not texCoords .empty ())
	{
		texCoordBufferIds .resize (texCoords .size ());

		glGenBuffers (texCoords .size (), texCoordBufferIds .data ());

		for (size_t i = 0, size = texCoords .size (); i < size; ++ i)
		{
			glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferIds [i]);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * texCoords [i] .size (), texCoords [i] .data (), GL_STATIC_COPY);
		}
	}

	if (not normals .empty ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * normals .size (), normals .data (), GL_STATIC_COPY);
	}

	if (not vertices .empty ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3d) * vertices .size (), vertices .data (), GL_STATIC_COPY);
	}

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::collision (const CollisionContainer* const context)
{
	glDisable (GL_CULL_FACE);

	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_DOUBLE, 0, 0);

	size_t first = 0;

	for (const auto & element : elements)
	{
		glDrawArrays (element .vertexMode, first, element .count);
		first += element .count;
	}
	
	glDisableClientState (GL_VERTEX_ARRAY);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::draw (ShapeContainer* const context)
{
	const auto shaderNode = getBrowser () -> getShader ();

	context -> setColorMaterial (not colors .empty ());

	// Enable vertex attribute nodes

	if (not attribNodes .empty ())
	{
		GLint program = 0;

		glGetIntegerv (GL_CURRENT_PROGRAM, &program);

		if (program)
		{
			for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
				attribNodes [i] -> enable (program, attribBufferIds [i]);
		}
	}

	if (shaderNode)
	{
		// Enable shader

		shaderNode -> setGlobalUniforms (context);
		shaderNode -> setLocalUniforms (context);

		if (not colors .empty ())
			shaderNode -> enableColorAttrib (colorBufferId);

		shaderNode -> enableTexCoordAttrib (texCoordBufferIds);
		shaderNode -> enableNormalAttrib   (normalBufferId);
		shaderNode -> enableVertexAttrib   (vertexBufferId);
	}

	#ifndef SHADER_PIPELINE
	{
		// Enable colors, texture coords, normals and vertices.

		if (not colors .empty ())
		{
			if (glIsEnabled (GL_LIGHTING))
				glEnable (GL_COLOR_MATERIAL);

			glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
			glEnableClientState (GL_COLOR_ARRAY);
			glColorPointer (4, GL_FLOAT, 0, 0);
		}

		if (getBrowser () -> getTexture ())
			texCoordNode -> enable (texCoordBufferIds);

		if (glIsEnabled (GL_LIGHTING) or shaderNode)
		{
			if (not normals .empty ())
			{
				glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
				glEnableClientState (GL_NORMAL_ARRAY);
				glNormalPointer (GL_FLOAT, 0, 0);
			}
		}

		glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
		glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (3, GL_DOUBLE, 0, 0);
	}
	#endif

	// Draw depending on ccw, transparency and solid.

	const auto positiveScale = determinant3 (ModelViewMatrix4d ()) > 0;

	if (context -> isTransparent () && not solid)
	{
		glEnable (GL_CULL_FACE);
		glFrontFace (positiveScale ? frontFace : (frontFace == GL_CCW ? GL_CW : GL_CCW));

		glCullFace (GL_FRONT);

		// Draw

		size_t first = 0;

		for (const auto & element : elements)
		{
			glDrawArrays (element .vertexMode, first, element .count);
			first += element .count;
		}

		glCullFace (GL_BACK);

		// Draw

		first = 0;

		for (const auto & element : elements)
		{
			glDrawArrays (element .vertexMode, first, element .count);
			first += element .count;
		}
	}
	else
	{
		// Solid & ccw.

		if (solid)
			glEnable (GL_CULL_FACE);

		else
			glDisable (GL_CULL_FACE);

		glFrontFace (positiveScale ? frontFace : (frontFace == GL_CCW ? GL_CW : GL_CCW));

		size_t first = 0;

		for (const auto & element : elements)
		{
			glDrawArrays (element .vertexMode, first, element .count);
			first += element .count;
		}
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

	if (shaderNode)
	{
		// Disable shader

		shaderNode -> disableColorAttrib ();
		shaderNode -> disableTexCoordAttrib ();
		shaderNode -> disableNormalAttrib ();
		shaderNode -> disableVertexAttrib ();
	}

	#ifndef SHADER_PIPELINE
	{
		// Texture
	
		if (getBrowser () -> getTexture ())
			texCoordNode -> disable ();

		// Other arrays

		glDisableClientState (GL_COLOR_ARRAY);
		glDisableClientState (GL_NORMAL_ARRAY);
		glDisableClientState (GL_VERTEX_ARRAY);
	}
	#endif

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::dispose ()
{
	try
	{
		ContextLock lock (getBrowser ());

		if (not attribBufferIds .empty ())
			glDeleteBuffers (attribBufferIds .size (), attribBufferIds .data ());

		if (colorBufferId)
			glDeleteBuffers (1, &colorBufferId);

		if (not texCoordBufferIds .empty ())
			glDeleteBuffers (texCoordBufferIds .size (), texCoordBufferIds .data ());

		if (normalBufferId)
			glDeleteBuffers (1, &normalBufferId);

		if (vertexBufferId)
			glDeleteBuffers (1, &vertexBufferId);
	}
	catch (const Error <INVALID_OPERATION_TIMING> &)
	{ }

	X3DNode::dispose ();
}

} // X3D
} // titania
