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

#include "X3DGeometryNode.h"

#include "../../Browser/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../Shaders/X3DShaderNode.h"
#include "../Shaders/X3DVertexAttributeNode.h"
#include "../Texturing/TextureCoordinateGenerator.h"

#include <Titania/Math/Algorithms/Barycentric.h>

namespace titania {
namespace X3D {

const Matrix4d X3DGeometryNode::matrix;

X3DGeometryNode::X3DGeometryNode () :
	          X3DNode (),
	     cameraObject (),
	             bbox (),
	      attribNodes (),
	           colors (),
	     texCoordNode (),
	        texCoords (),
	          normals (),
	      faceNormals (),
	         vertices (),
	     geometryType (GeometryType::GEOMETRY_3D),
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

	addChildObjects (cameraObject, texCoordNode);
}

void
X3DGeometryNode::setup ()
{
	texCoordNode .set (getBrowser () -> getDefaultTexCoord ());

	X3DNode::setup ();

	glGenBuffers (1, &colorBufferId);
	glGenBuffers (1, &normalBufferId);
	glGenBuffers (1, &vertexBufferId);

	getBrowser () -> getRenderingProperties () -> getShading () .addInterest (&X3DGeometryNode::set_shading, this);
	getBrowser () -> getFixedPipelineRequired () .addInterest (&X3DGeometryNode::set_fixedPipeline, this);

	addInterest (&X3DGeometryNode::update, this);

	update ();
}

void
X3DGeometryNode::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	if (isInitialized ())
	{
		getBrowser () -> getRenderingProperties () -> getShading () .removeInterest (&X3DGeometryNode::set_shading, this);
		getBrowser () -> getFixedPipelineRequired () .removeInterest (&X3DGeometryNode::set_fixedPipeline, this);

		if (texCoordNode == getBrowser () -> getDefaultTexCoord ())
			texCoordNode .set (executionContext -> getBrowser () -> getDefaultTexCoord ());
	}

	X3DNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getBrowser () -> getRenderingProperties () -> getShading () .addInterest (&X3DGeometryNode::set_shading, this);
		getBrowser () -> getFixedPipelineRequired () .addInterest (&X3DGeometryNode::set_fixedPipeline, this);
	}
}

void
X3DGeometryNode::setCameraObject (const bool value)
{
	if (value not_eq cameraObject)
	   cameraObject = value;
}

Box3d
X3DGeometryNode::createBBox () const
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

void
X3DGeometryNode::setTextureCoordinate (X3DTextureCoordinateNode* const value)
{
	if (value)
		texCoordNode .set (value);

	else
		texCoordNode .set (getBrowser () -> getDefaultTexCoord ());
}

GLenum
X3DGeometryNode::getVertexMode (const size_t n)
{
	switch (n)
	{
		case 3:
			return GL_TRIANGLES;
		case 4:
			return GL_QUADS;
		default:
			return GL_POLYGON;
	}
}

void
X3DGeometryNode::addElements (const GLenum vertexMode, const size_t count)
{
	const auto first = elements .empty () ? 0 : elements .back () .last ();

	elements .emplace_back (vertexMode, first, first + count);
}

bool
X3DGeometryNode::isClipped (const Vector3d & point, const ClipPlaneContainerArray & clipPlanes) const
{
	return std::any_of (clipPlanes .begin (),
	                    clipPlanes .end (),
	                    [&point] (const std::shared_ptr <ClipPlaneContainer> & clipPlane)
	                    { return clipPlane -> isClipped (point); });
}

bool
X3DGeometryNode::intersects (Line3d line,
                             const ClipPlaneContainerArray & clipPlanes,
                             Matrix4d modelViewMatrix,
                             std::vector <IntersectionPtr> & intersections) const
{
	try
	{
		if (not getBBox () .intersects (line))
			return false;

		const auto & matrix = getMatrix ();  // Get the current matrix from screen nodes.

		modelViewMatrix .mult_left (matrix); // This matrix is for clipping only.

		line *= inverse (matrix);

		bool intersected = false;

		for (const auto & element : elements)
		{
			switch (element .vertexMode ())
			{
				case GL_TRIANGLES:
				{
					for (size_t i = element .first (), size = element .last (); i < size; i += 3)
					{
						intersected |= intersects (line, i, i + 1, i + 2, clipPlanes, modelViewMatrix, intersections);
					}

					continue;
				}
				case GL_QUADS:
				{
					for (size_t i = element .first (), size = element .last (); i < size; i += 4)
					{
						intersected |= intersects (line, i, i + 1, i + 2, clipPlanes, modelViewMatrix, intersections);
						intersected |= intersects (line, i, i + 2, i + 3, clipPlanes, modelViewMatrix, intersections);
					}

					continue;
				}
				case GL_POLYGON:
				{
					const auto first = element .first ();

					for (int32_t i = first + 1, size = element .last () - 1; i < size; ++ i)
					{
						intersected |= intersects (line, first, i, i + 1, clipPlanes, modelViewMatrix, intersections);
					}

					continue;
				}
				default:
					continue;
			}
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
                             const ClipPlaneContainerArray & clipPlanes,
	                          const Matrix4d & modelViewMatrix,
	                          std::vector <IntersectionPtr> & intersections) const
{
	const auto intersection = line .intersects (vertices [i1], vertices [i2], vertices [i3]);

	if (not intersection .second)
		return false;

	const auto & uvt          = intersection .first;
	const size_t texCoordSize = texCoords .empty () ? 0 : texCoords [0] .size (); // LineGeometry doesn't have texCoords
	auto         texCoord     = Vector4f (0, 0, 0, 1);

	if (i1 < texCoordSize)
		texCoord = from_barycentric <float> (uvt, texCoords [0] [i1], texCoords [0] [i2], texCoords [0] [i3]);

	const auto normal = normalize (from_barycentric <float> (uvt, normals [i1], normals [i2], normals [i3]));
	const auto point  = from_barycentric (uvt, vertices [i1], vertices [i2], vertices [i3]);

	if (isClipped (point * modelViewMatrix, clipPlanes))
		return false;

	intersections .emplace_back (new Intersection { texCoord, normal, point * getMatrix (), std::array <Vector3d, 3> { vertices [i1], vertices [i2], vertices [i3] } });
	return true;
}

bool
X3DGeometryNode::intersects (Box3d box,
                             const ClipPlaneContainerArray & clipPlanes, 
                             Matrix4d modelViewMatrix) const
{
	try
	{
		if (not box .intersects (getBBox ()))
			return false;
		
		const auto & matrix = getMatrix ();  // Get the current matrix from screen nodes.

		modelViewMatrix .mult_left (matrix); // This matrix is for clipping only.

		box *= inverse (matrix);
	
		for (const auto & element : elements)
		{
			switch (element .vertexMode ())
			{
				case GL_TRIANGLES :
				{
					for (size_t i = element .first (), size = element .last (); i < size; i += 3)
					{
						if (box .intersects (vertices [i], vertices [i + 1], vertices [i + 2]))
						{
							if (not clipPlanes .empty ())
							{
								if (isClipped (vertices [i + 0] * modelViewMatrix, clipPlanes))
									continue;
	
								if (isClipped (vertices [i + 1] * modelViewMatrix, clipPlanes))
									continue;

								if (isClipped (vertices [i + 2] * modelViewMatrix, clipPlanes))
									continue;
							}

							return true;
						}
					}

					continue;
				}
				case GL_QUADS:
				{
					for (size_t i = element .first (), size = element .last (); i < size; i += 4)
					{
						// Triangle 1.

						if (box .intersects (vertices [i], vertices [i + 1], vertices [i + 2]))
						{
							if (not clipPlanes .empty ())
							{
								if (not (isClipped (vertices [i + 0] * modelViewMatrix, clipPlanes) or
									      isClipped (vertices [i + 1] * modelViewMatrix, clipPlanes) or
									      isClipped (vertices [i + 2] * modelViewMatrix, clipPlanes)))
								{
									return true;
								}
							}
							else
								return true;
						}

						// Triangle 2.

						if (box .intersects (vertices [i], vertices [i + 2], vertices [i + 3]))
						{
							if (not clipPlanes .empty ())
							{
								if (not (isClipped (vertices [i + 0] * modelViewMatrix, clipPlanes) or
									      isClipped (vertices [i + 1] * modelViewMatrix, clipPlanes) or
									      isClipped (vertices [i + 3] * modelViewMatrix, clipPlanes)))
								{
									return true;
								}
							}
							else
								return true;
						}
					}
	
					continue;
				}
				case GL_POLYGON:
				{
					const auto first = element .first ();

					for (int32_t i = first + 1, size = element .last () - 1; i < size; ++ i)
					{
						if (box .intersects (vertices [first], vertices [i], vertices [i + 1]))
						{
							if (not clipPlanes .empty ())
							{
								if (isClipped (vertices [first] * modelViewMatrix, clipPlanes))
									continue;
		
								if (isClipped (vertices [i] * modelViewMatrix, clipPlanes))
									continue;
		
								if (isClipped (vertices [i + 1] * modelViewMatrix, clipPlanes))
									continue;
							}

							return true;
						}
					}

					continue;
				}
				default:
					continue;
			}
		}

		return false;
	}
	catch (const std::domain_error &)
	{
		return false;
	}
}

std::vector <Vector3d>
X3DGeometryNode::intersects (X3DRenderObject* const renderObject,
                             const std::shared_ptr <FrameBuffer> & frameBuffer,
                       	     const std::shared_ptr <FrameBuffer> & depthBuffer,
                       	     std::vector <IntersectionPtr> & intersections)
{
	try
	{
		std::vector <Vector3d> hitPoints;

		const auto   shading             = renderObject -> getBrowser () -> getRenderingProperties () -> getShading ();
		const auto & projectionMatrix    = renderObject -> getProjectionMatrix () .get ();
		const auto & modelViewMatrix     = renderObject -> getModelViewMatrix () .get ();
		const auto & viewport            = renderObject -> getViewVolumes () .back () .getScissor ();
		const auto   modelViewProjection = modelViewMatrix * projectionMatrix;
		const auto   width               = frameBuffer -> getWidth ();
		const auto   height              = frameBuffer -> getHeight ();
		const auto & depth               = depthBuffer -> getDepth ();

		for (const Vector3d & vertex : vertices)
		{
			const auto screen = ViewVolume::projectPoint (vertex, modelViewProjection, viewport);
			const auto x      = std::floor (screen .x ());
			const auto y      = std::floor (screen .y ());

			if (x < 0 or x >= width)
				continue;

			if (y < 0 or y >= height)
				continue;

			const auto world = vertex * modelViewMatrix;

			if (world .z () > 0)
			   continue;

			switch (shading)
			{
				case ShadingType::GOURAUD:
				case ShadingType::PHONG:
				{
					const auto z     = depth [x + y * depthBuffer -> getWidth ()];
					const auto delta = screen .z () - z;

					if (delta > 1e-3)
						continue;
				}
				default:
					break;
			}
	
			const auto index = x * 4 + y * width * 4;
	
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

void
X3DGeometryNode::triangulate (std::vector <Color4f> & colors_,
	                           TexCoordArray & texCoords_,
	                           std::vector <Vector3f> & normals_,
	                           std::vector <Vector3d> & vertices_) const
{
	texCoords_ .resize (texCoords .size ());

	for (const auto & element : elements)
	{
		switch (element .vertexMode ())
		{
			case GL_TRIANGLES :
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 3)
				{
					triangulate (i, i + 1, i + 2, colors_, texCoords_, normals_, vertices_);
				}

				continue;
			}
			case GL_QUADS:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 4)
				{
					triangulate (i, i + 1, i + 2, colors_, texCoords_, normals_, vertices_);
					triangulate (i, i + 2, i + 3, colors_, texCoords_, normals_, vertices_);
				}

				continue;
			}
			case GL_POLYGON:
			{
				const auto first = element .first ();

				for (int32_t i = first + 1, size = element .last () - 1; i < size; ++ i)
				{
					triangulate (first, i, i + 1, colors_, texCoords_, normals_, vertices_);
				}

				continue;
			}
			default:
				continue;
		}
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
	int32_t  Sindex, Tindex;

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
X3DGeometryNode::getTexCoordParams (Vector3d & min, double & Ssize, int32_t & Sindex, int32_t & Tindex)
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
	if (getSolid ())
		return;

	if (getBrowser () -> getFixedPipelineRequired ())
	{
		auto & texCoords = this -> texCoords [0];
	
		addElements (vertexMode, getVertices () .size ());
		setSolid (true);

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
	                    	                   
bool
X3DGeometryNode::cut (X3DRenderObject* const renderObject, const Line2d & cutLine)
{
	return false;
}

void
X3DGeometryNode::set_shading (const ShadingType & shading)
{
}

void
X3DGeometryNode::set_fixedPipeline ()
{
	// If there is a default shader then shader pipeline is enabled and we must rebuild Geometry2D nodes,
	// to build double face geometry or not.

	if (geometryType == GeometryType::GEOMETRY_2D)
		update ();
}

void
X3DGeometryNode::update ()
{
	clear ();
	build ();

	bbox = createBBox ();

	const_cast <SFTime &> (getExecutionContext () -> bbox_changed ()) = getCurrentTime ();

	if (geometryType == GeometryType::GEOMETRY_2D or geometryType == GeometryType::GEOMETRY_3D)
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
	bbox = Box3d ();

	if (not attribBufferIds .empty ())
	{
		glDeleteBuffers (attribBufferIds .size (), attribBufferIds .data ());

		attribNodes .clear ();
		attribBufferIds .clear ();
	}

	if (not texCoordBufferIds .empty ())
		glDeleteBuffers (texCoordBufferIds .size (), texCoordBufferIds .data ());

	colors      .clear ();
	texCoords   .clear ();
	normals     .clear ();
	faceNormals .clear ();
	vertices    .clear ();
	elements    .clear ();
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
X3DGeometryNode::depth (const X3DShapeContainer* const context)
{
	glBindBuffer (GL_ARRAY_BUFFER, vertexBufferId);
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_DOUBLE, 0, 0);

	for (const auto & element : elements)
	{
		glDrawArrays (element .vertexMode (), element .first (), element .count ());
	}
	
	glDisableClientState (GL_VERTEX_ARRAY);
	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::draw (ShapeContainer* const context)
{
	try
	{
		const auto browser    = context -> getBrowser ();
		const auto shaderNode = browser -> getShader ();

		context -> setGeometryType  (geometryType);
		context -> setColorMaterial (not colors .empty ());
	
		if (browser -> getFixedPipelineRequired ())
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
	
			if (browser -> getTexture ())
			{
				texCoordNode -> enable (context, texCoordBufferIds);
			}
	
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

		if (shaderNode)
		{
			// Enable shader

			//shaderNode -> enable ();
			shaderNode -> setLocalUniforms (context);

			// Enable vertex attribute nodes

			for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
				attribNodes [i] -> enable (shaderNode, attribBufferIds [i]);

			if (not colors .empty ())
				shaderNode -> enableColorAttrib (colorBufferId, GL_FLOAT, 0, nullptr);

			if (not texCoords .empty ())
				shaderNode -> enableTexCoordAttrib (texCoordBufferIds, GL_FLOAT, { }, { });

			if (not normals .empty ())
				shaderNode -> enableNormalAttrib (normalBufferId, GL_FLOAT, 0, nullptr);

			shaderNode -> enableVertexAttrib (vertexBufferId, GL_DOUBLE, 0, nullptr);
		}

		// Draw depending on ccw, transparency and solid.
	
		const auto positiveScale = context -> getModelViewMatrix () .submatrix () .determinant () > 0;
	
		glFrontFace (positiveScale ? frontFace : (frontFace == GL_CCW ? GL_CW : GL_CCW));

		if (context -> isTransparent () && not solid)
		{
			glEnable (GL_CULL_FACE);
	
			// Draw

			glCullFace (GL_FRONT);
	
			for (const auto & element : elements)
			{
				glDrawArrays (element .vertexMode (), element .first (), element .count ());
			}

			// Draw

			glCullFace (GL_BACK);
	
			for (const auto & element : elements)
			{
				glDrawArrays (element .vertexMode (), element .first (), element .count ());
			}
		}
		else
		{
			// Solid & ccw.
	
			if (solid)
				glEnable (GL_CULL_FACE);
	
			else
				glDisable (GL_CULL_FACE);
	
			for (const auto & element : elements)
			{
				glDrawArrays (element .vertexMode (), element .first (), element .count ());
			}
		}
	
		// VertexAttribs
	
		if (browser -> getFixedPipelineRequired ())
		{
			// Texture
		
			if (browser -> getTexture ())
				texCoordNode -> disable (context);
	
			// Other arrays
	
			glDisableClientState (GL_COLOR_ARRAY);
			glDisableClientState (GL_NORMAL_ARRAY);
			glDisableClientState (GL_VERTEX_ARRAY);
		}
	
		if (shaderNode)
		{
			for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
				attribNodes [i] -> disable (shaderNode);
		
			// Disable shader
		
			shaderNode -> disableColorAttrib ();
			shaderNode -> disableTexCoordAttrib ();
			shaderNode -> disableNormalAttrib ();
			shaderNode -> disableVertexAttrib ();
			shaderNode -> disable ();
		}
	
		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}
	catch (const std::exception &)
	{ }
}

void
X3DGeometryNode::drawParticles (ShapeContainer* const context, const std::vector <SoftParticle> & particles, const size_t numParticles)
{
	// Soft system draw.

	try
	{
		const auto browser    = context -> getBrowser ();
		const auto shaderNode = browser -> getShader ();

		if (not shaderNode)
			return;

		context -> setGeometryType  (geometryType);
		context -> setColorMaterial (not colors .empty ());

		// Setup shader

		//shaderNode -> enable ();
		shaderNode -> setLocalUniforms (context);

		// Enable vertex attribute nodes

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> enable (shaderNode, attribBufferIds [i]);

		if (not colors .empty ())
			shaderNode -> enableColorAttrib (colorBufferId, GL_FLOAT, 0, nullptr);

		if (not texCoords .empty ())
			shaderNode -> enableTexCoordAttrib (texCoordBufferIds, GL_FLOAT, { }, { });

		if (not normals .empty ())
			shaderNode -> enableNormalAttrib (normalBufferId, GL_FLOAT, 0, nullptr);

		shaderNode -> enableVertexAttrib (vertexBufferId, GL_DOUBLE, 0, nullptr);

		// Draw particles.
	
		auto       modelViewMatrix = context -> getModelViewMatrix ();
		const auto origin          = modelViewMatrix .origin ();
	
		// Draw depending on ccw, transparency and solid.
	
		const auto positiveScale = context -> getModelViewMatrix () .submatrix () .determinant () > 0;
	
		glFrontFace (positiveScale ? frontFace : (frontFace == GL_CCW ? GL_CW : GL_CCW));

		if (context -> isTransparent () && not solid)
		{
			for (size_t p = 0; p < numParticles; ++ p)
			{
				modelViewMatrix .origin (origin);
				modelViewMatrix .translate (particles [p] .position);

				shaderNode -> setMatrices (inverse (modelViewMatrix .submatrix ()), modelViewMatrix);

				glEnable (GL_CULL_FACE);
				glCullFace (GL_FRONT);
		
				for (const auto & element : elements)
				{
					glDrawArrays (element .vertexMode (), element .first (), element .count ());
				}

				glCullFace (GL_BACK);
		
				for (const auto & element : elements)
				{
					glDrawArrays (element .vertexMode (), element .first (), element .count ());
				}
			}	
		}
		else
		{
			if (solid)
				glEnable (GL_CULL_FACE);
			else
				glDisable (GL_CULL_FACE);

			for (size_t p = 0; p < numParticles; ++ p)
			{
				modelViewMatrix .origin (origin);
				modelViewMatrix .translate (particles [p] .position);

				shaderNode -> setMatrices (inverse (modelViewMatrix .submatrix ()), modelViewMatrix);

				for (const auto & element : elements)
				{
					glDrawArrays (element .vertexMode (), element .first (), element .count ());
				}
			}
		}

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> disable (shaderNode);

		// Disable shader

		shaderNode -> disableColorAttrib ();
		shaderNode -> disableTexCoordAttrib ();
		shaderNode -> disableNormalAttrib ();
		shaderNode -> disableVertexAttrib ();
		shaderNode -> disable ();

		glBindBuffer (GL_ARRAY_BUFFER, 0);
	}
	catch (const std::exception & error)
	{
		__LOG__ << error .what () << std::endl;
	}
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

X3DGeometryNode::~X3DGeometryNode ()
{ }

} // X3D
} // titania
