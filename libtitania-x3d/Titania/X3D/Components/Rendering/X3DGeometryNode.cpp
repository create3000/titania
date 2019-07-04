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

#include "../../Browser/Core/BrowserOptions.h"
#include "../../Browser/Core/RenderingProperties.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/FrameBuffer.h"
#include "../../Rendering/ShapeContainer.h"
#include "../../Rendering/X3DRenderObject.h"
#include "../RigidBodyPhysics/CollidableShape.h"
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
	           transparent (),
	         rebuildOutput (),
	                  bbox (),
	           attribNodes (),
	             fogDepths (),
	                colors (),
	 textureCoordinateNode (),
	        multiTexCoords (),
	               normals (),
	           faceNormals (),
	              vertices (),
	          geometryType (3),
	                 solid (true),
	             frontFace (GL_CCW),
	           flatShading (0),
	              elements (),
	             pickShape (),
	       attribBufferIds (),
	      fogDepthBufferId (0),
	         colorBufferId (0),
	multiTexCoordBufferIds (),
	        normalBufferId (0),
	        vertexBufferId (0)

{
	addType (X3DConstants::X3DGeometryNode);

	addChildObjects (cameraObject,
	                 transparent,
	                 rebuildOutput,
	                 textureCoordinateNode,
	                 pickShape);

	cameraObject  .setAccessType (outputOnly);
	transparent   .setAccessType (outputOnly);
	rebuildOutput .setAccessType (outputOnly);
	pickShape     .setAccessType (outputOnly);
}

void
X3DGeometryNode::setup ()
{
	X3DNode::setup ();

	rebuild ();
}

void
X3DGeometryNode::initialize ()
{
	X3DNode::initialize ();

	getExecutionContext () -> isLive () .addInterest (&X3DGeometryNode::set_live, this);
	isLive () .addInterest (&X3DGeometryNode::set_live, this);

	addInterest (&X3DGeometryNode::requestRebuild, this);
	rebuildOutput .addInterest (&X3DGeometryNode::rebuild, this);

	textureCoordinateNode .set (getBrowser () -> getDefaultTexCoord ());

	glGenBuffers (1, &fogDepthBufferId);
	glGenBuffers (1, &colorBufferId);
	glGenBuffers (1, &normalBufferId);
	glGenBuffers (1, &vertexBufferId);

	set_live ();
}

void
X3DGeometryNode::setExecutionContext (X3DExecutionContext* const executionContext)
{
	if (isInitialized ())
	{
		getBrowser () -> getBrowserOptions () -> getShading () .removeInterest (&X3DGeometryNode::set_shading, this);
		getExecutionContext () -> isLive () .removeInterest (&X3DGeometryNode::set_live, this);

		if (textureCoordinateNode == getBrowser () -> getDefaultTexCoord ())
			textureCoordinateNode .set (executionContext -> getBrowser () -> getDefaultTexCoord ());
	}

	X3DNode::setExecutionContext (executionContext);

	if (isInitialized ())
	{
		getExecutionContext () -> isLive () .addInterest (&X3DGeometryNode::set_live, this);

		set_live ();
	}
}

void
X3DGeometryNode::setCameraObject (const bool value)
{
	if (value not_eq cameraObject)
	   cameraObject = value;
}

void
X3DGeometryNode::setTransparent (const bool value)
{
	if (value not_eq transparent)
	   transparent = value;
}

Box3d
X3DGeometryNode::createBBox () const
{
	return Box3d (vertices .cbegin (), vertices .cend (), math::iterator_type ());
}

void
X3DGeometryNode::setAttribs (const X3DPtrArray <X3DVertexAttributeNode> & nodes, const std::vector <std::vector <float>> & attribs)
{
	if (attribs .empty ())
		return;

	const size_t numAttribs = nodes .size ();

	attribNodes .assign (nodes .cbegin (), nodes .cend ());
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
		textureCoordinateNode .set (value);

	else
		textureCoordinateNode .set (getBrowser () -> getDefaultTexCoord ());
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
	return std::any_of (clipPlanes .cbegin (),
	                    clipPlanes .cend (),
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
					const auto first = element .first ();

					for (size_t i = first, size = element .last (); i < size; i += 3)
					{
						intersected |= intersects (line, i, i + 1, i + 2, i, i + 3, clipPlanes, modelViewMatrix, intersections);
					}

					continue;
				}
				case GL_QUADS:
				{
					const auto first = element .first ();

					for (size_t i = first, size = element .last (); i < size; i += 4)
					{
						intersected |= intersects (line, i, i + 1, i + 2, i, i + 4, clipPlanes, modelViewMatrix, intersections);
						intersected |= intersects (line, i, i + 2, i + 3, i, i + 4, clipPlanes, modelViewMatrix, intersections);
					}

					continue;
				}
				case GL_POLYGON:
				{
					const auto first = element .first ();

					for (int32_t i = first + 1, size = element .last () - 1; i < size; ++ i)
					{
						intersected |= intersects (line, first, i, i + 1, first, size, clipPlanes, modelViewMatrix, intersections);
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
                             const size_t first,
                             const size_t last,
                             const ClipPlaneContainerArray & clipPlanes,
                             const Matrix4d & modelViewMatrix,
                             std::vector <IntersectionPtr> & intersections) const
{
	const auto & [intersection, intersected] = line .intersects (vertices [i1], vertices [i2], vertices [i3]);

	if (not intersected)
		return false;

	const auto & uvt          = intersection;
	const size_t texCoordSize = multiTexCoords .empty () ? 0 : multiTexCoords [0] .size (); // LineGeometry doesn't have multiTexCoords
	auto         texCoord     = Vector4f (0, 0, 0, 1);

	if (i1 < texCoordSize)
		texCoord = from_barycentric <float> (uvt, multiTexCoords [0] [i1], multiTexCoords [0] [i2], multiTexCoords [0] [i3]);

	const auto normal     = normalize (from_barycentric <float> (uvt, normals [i1], normals [i2], normals [i3]));
	const auto faceNormal = Triangle3d (vertices [i1], vertices [i2], vertices [i3]) .normal ();
	const auto point      = from_barycentric (uvt, vertices [i1], vertices [i2], vertices [i3]);

	if (isClipped (point * modelViewMatrix, clipPlanes))
		return false;

	std::vector <Vector3d> face (vertices .begin () + first, vertices .begin () + last);

	intersections .emplace_back (new Intersection (
		texCoord,
		normal,
		faceNormal,
		point * getMatrix (), // multiply by screen scale matrix
		std::array <Vector3d, 3> { vertices [i1], vertices [i2], vertices [i3] },
		std::move (face)
	));

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
				case ShadingType::FLAT:
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

		// Remove point behind a polygon.

		switch (shading)
		{
			case ShadingType::FLAT:
			case ShadingType::GOURAUD:
			case ShadingType::PHONG:
			{
				const auto invModelViewMatrix = inverse (modelViewMatrix);
				const auto origin             = invModelViewMatrix .origin ();
				auto       intersections      = std::vector <IntersectionPtr> ();

				const auto iter = std::remove_if (hitPoints .begin (), hitPoints .end (), [&] (const Vector3d & point)
				{
					const auto line = Line3d (origin, point, points_type ());
					const auto z    = (point * modelViewMatrix) .z ();

					intersections .clear ();

					if (intersects (line, { }, modelViewMatrix, intersections))
					{
						const auto iter = std::find_if (intersections .cbegin (), intersections .cend (),
						[&] (const IntersectionPtr & intersection)
						{
							return (intersection -> getPoint () * modelViewMatrix) .z () - z > 1e-5;
						});

						return iter not_eq intersections .end ();
					}

					return true;
				});

				hitPoints .erase (iter, hitPoints .end ());
			}
			default:
				break;
		}

		return hitPoints;
	}
	catch (const std::exception & error)
	{
		return std::vector <Vector3d> ();
	}
}

void
X3DGeometryNode::triangulate (std::vector <Color4f>* const colors_,
	                           MultiTexCoordArray* const multiTexCoords_,
	                           std::vector <Vector3f>* const faceNormals_,
	                           std::vector <Vector3f>* const normals_,
	                           std::vector <Vector3d>* const vertices_) const
{
	if (multiTexCoords_)
		multiTexCoords_ -> resize (multiTexCoords .size ());

	for (const auto & element : elements)
	{
		switch (element .vertexMode ())
		{
			case GL_TRIANGLES :
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 3)
				{
					triangulate (i, i + 1, i + 2, colors_, multiTexCoords_, faceNormals_, normals_, vertices_);
				}

				continue;
			}
			case GL_QUADS:
			{
				for (size_t i = element .first (), size = element .last (); i < size; i += 4)
				{
					triangulate (i, i + 1, i + 2, colors_, multiTexCoords_, faceNormals_, normals_, vertices_);
					triangulate (i, i + 2, i + 3, colors_, multiTexCoords_, faceNormals_, normals_, vertices_);
				}

				continue;
			}
			case GL_POLYGON:
			{
				const auto first = element .first ();

				for (int32_t i = first + 1, size = element .last () - 1; i < size; ++ i)
				{
					triangulate (first, i, i + 1, colors_, multiTexCoords_, faceNormals_, normals_, vertices_);
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
	                           std::vector <Color4f>* const colors_,
	                           MultiTexCoordArray* const multiTexCoords_,
	                           std::vector <Vector3f>* const faceNormals_,
	                           std::vector <Vector3f>* const normals_,
	                           std::vector <Vector3d>* const vertices_) const
{
	if (colors_)
	{
		if (not colors .empty ())
		{
			colors_ -> emplace_back (colors [i1]);
			colors_ -> emplace_back (colors [i2]);
			colors_ -> emplace_back (colors [i3]);
		}
	}

	if (multiTexCoords_)
	{
		for (size_t t = 0, size = multiTexCoords .size (); t < size; ++ t)
		{
			auto & texCoords_ = (*multiTexCoords_) [t];
			auto & texCoords  = multiTexCoords [t];

			texCoords_ .emplace_back (texCoords [i1]);
			texCoords_ .emplace_back (texCoords [i2]);
			texCoords_ .emplace_back (texCoords [i3]);
		}
	}

	if (faceNormals_)
		faceNormals_ -> emplace_back (Triangle3f (vertices [i1], vertices [i2], vertices [i3]) .normal ());

	if (normals_)
	{
		normals_ -> emplace_back (normals [i1]);
		normals_ -> emplace_back (normals [i2]);
		normals_ -> emplace_back (normals [i3]);
	}

	if (vertices_)
	{
		vertices_ -> emplace_back (vertices [i1]);
		vertices_ -> emplace_back (vertices [i2]);
		vertices_ -> emplace_back (vertices [i3]);
	}
}

TexCoordArray
X3DGeometryNode::buildTexCoords ()
{
	TexCoordArray texCoords;

	Vector3d min;
	double   Ssize;
	int32_t  Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	for (const auto & vertex : getVertices ())
	{
		texCoords .emplace_back ((vertex [Sindex] - min [Sindex]) / Ssize,
		                         (vertex [Tindex] - min [Tindex]) / Ssize,
		                         0,
		                         1);
	}

	return texCoords;
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

	std::for_each (normalIndex .cbegin (), normalIndex .cend (), [&normals, &normals_, &cosCreaseAngle] (const NormalIndex::value_type & point)
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

bool
X3DGeometryNode::cut (X3DRenderObject* const renderObject, const Line2d & cutLine)
{
	return false;
}

void
X3DGeometryNode::set_live ()
{
	if (isLive () and getExecutionContext () -> isLive ())
		getBrowser () -> getBrowserOptions () -> getShading () .addInterest (&X3DGeometryNode::set_shading, this);
	else
		getBrowser () -> getBrowserOptions () -> getShading () .removeInterest (&X3DGeometryNode::set_shading, this);
}

void
X3DGeometryNode::set_shading (const ShadingType & shading)
{
	if (geometryType < 2)
		return;

	const bool flatShading = shading == ShadingType::FLAT;

	if (flatShading and this -> flatShading == 1)
		return;

   this -> flatShading = flatShading ? 1 : 2;

	if (flatShading)
	{
		if (faceNormals .empty ())
		{
			for (const auto & element : elements)
			{
				switch (element .vertexMode ())
				{
					case GL_TRIANGLES :
					{
						for (size_t i = element .first (), size = element .last (); i < size; i += 3)
						{
							const auto n = Triangle3d (vertices [i], vertices [i + 1], vertices [i + 2]) .normal ();

							for (size_t k = 0; k < 3; ++ k)
								faceNormals .emplace_back (n);
						}

						continue;
					}
					case GL_QUADS:
					{
						for (size_t i = element .first (), size = element .last (); i < size; i += 4)
						{
							const auto n = normal (vertices [i], vertices [i + 1], vertices [i + 2], vertices [i + 3]);

							for (size_t k = 0; k < 4; ++ k)
								faceNormals .emplace_back (n);
						}

						continue;
					}
					case GL_POLYGON:
					{
						const auto first = element .first ();

						// Determine polygon normal.
						// We use Newell's method https://www.opengl.org/wiki/Calculating_a_Surface_Normal here:

						double x = 0, y = 0, z = 0;

						auto next = vertices [first];

						for (size_t i = 0, size = element .last () - first; i < size; ++ i)
						{
							auto current = next;

							next = vertices [first + (i + 1) % size];

							x += (current .y () - next .y ()) * (current .z () + next .z ());
							y += (current .z () - next .z ()) * (current .x () + next .x ());
							z += (current .x () - next .x ()) * (current .y () + next .y ());
						}

						const auto n = normalize (Vector3d (x, y, z));

						// Add normal for each vertex.

						for (size_t k = 0, size = element .last () - first; k < size; ++ k)
							faceNormals .emplace_back (n);

						continue;
					}
					default:
						continue;
				}
			}

			if (frontFace == GL_CW)
				std::for_each (faceNormals .begin (), faceNormals .end (), std::mem_fn (&Vector3f::negate));
		}
	}

	if (flatShading)
	{
		if (not faceNormals .empty ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * faceNormals .size (), faceNormals .data (), GL_STATIC_COPY);
		}
	}
	else
	{
		if (not normals .empty ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * normals .size (), normals .data (), GL_STATIC_COPY);
		}
	}
}

void
X3DGeometryNode::requestRebuild ()
{
	rebuildOutput = getCurrentTime ();
}

void
X3DGeometryNode::rebuild ()
{
	clear ();
	build ();

	bbox = createBBox ();

	const_cast <SFTime &> (getExecutionContext () -> bbox_changed ()) = getCurrentTime ();

	if (geometryType > 1)
	{
		if (multiTexCoords .empty ())
			multiTexCoords .emplace_back (buildTexCoords ());

		multiTexCoords .resize (getBrowser () -> getMaxTextures (), multiTexCoords .back ());
	}

	// Upload normals or flat normals.

	set_shading (getBrowser () -> getBrowserOptions () -> getShading ());

	// Upload arrays.

	transfer ();
}

void
X3DGeometryNode::clear ()
{
	bbox = Box3d ();

	if (not attribBufferIds .empty ())
	{
		glDeleteBuffers (attribBufferIds .size (), attribBufferIds .data ());

		attribNodes     .clear ();
		attribBufferIds .clear ();
	}

	flatShading = 0;

	fogDepths      .clear ();
	colors         .clear ();
	multiTexCoords .clear ();
	normals        .clear ();
	faceNormals    .clear ();
	vertices       .clear ();
	elements       .clear ();
}

void
X3DGeometryNode::transfer ()
{
	if (not fogDepths .empty ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, fogDepthBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (float) * fogDepths .size (), fogDepths .data (), GL_STATIC_COPY);
	}

	if (not colors .empty ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * colors .size (), colors .data (), GL_STATIC_COPY);
	}

	if (not multiTexCoords .empty ())
	{
		for (size_t i = multiTexCoordBufferIds .size (), size = multiTexCoords .size (); i < size; ++ i)
		{
			multiTexCoordBufferIds .emplace_back ();

			glGenBuffers (1, &multiTexCoordBufferIds .back ());
		}

		for (size_t i = 0, size = multiTexCoords .size (); i < size; ++ i)
		{
			glBindBuffer (GL_ARRAY_BUFFER, multiTexCoordBufferIds [i]);
			glBufferData (GL_ARRAY_BUFFER, sizeof (Vector4f) * multiTexCoords [i] .size (), multiTexCoords [i] .data (), GL_STATIC_COPY);
		}
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
		const auto shaderNode = context -> getShader ();

		if (not shaderNode -> isValid ())
			return;

		context -> setGeometryType (geometryType);
		context -> setFogCoord (not fogDepths .empty ());
		context -> setColorMaterial (not colors .empty ());
		context -> setTextureCoordinate (textureCoordinateNode);

		// Enable shader

		shaderNode -> enable ();
		shaderNode -> setLocalUniforms (context);

		// Enable vertex attribute nodes

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> enable (shaderNode, attribBufferIds [i]);

		if (not fogDepths .empty ())
			shaderNode -> enableFogDepthAttrib (fogDepthBufferId, GL_FLOAT, 0, nullptr);

		if (not colors .empty ())
			shaderNode -> enableColorAttrib (colorBufferId, GL_FLOAT, 0, nullptr);

		if (not multiTexCoords .empty ())
			shaderNode -> enableTexCoordAttrib (multiTexCoordBufferIds, GL_FLOAT, { }, { });

		if (not normals .empty ())
			shaderNode -> enableNormalAttrib (normalBufferId, GL_FLOAT, 0, nullptr);

		shaderNode -> enableVertexAttrib (vertexBufferId, GL_DOUBLE, 0, nullptr);

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

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> disable (shaderNode);

		// Disable shader

		shaderNode -> disableFogDepthAttrib ();
		shaderNode -> disableColorAttrib ();
		shaderNode -> disableTexCoordAttrib ();
		shaderNode -> disableNormalAttrib ();
		shaderNode -> disableVertexAttrib ();
		shaderNode -> disable ();

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
		const auto shaderNode = context -> getShader ();

		if (not shaderNode -> isValid ())
			return;

		context -> setGeometryType (geometryType);
		context -> setFogCoord (not fogDepths .empty ());
		context -> setColorMaterial (not colors .empty ());
		context -> setTextureCoordinate (textureCoordinateNode);

		// Setup shader

		shaderNode -> enable ();
		shaderNode -> setLocalUniforms (context);

		// Enable vertex attribute nodes

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> enable (shaderNode, attribBufferIds [i]);

		if (not fogDepths .empty ())
			shaderNode -> enableFogDepthAttrib (fogDepthBufferId, GL_FLOAT, 0, nullptr);

		if (not colors .empty ())
			shaderNode -> enableColorAttrib (colorBufferId, GL_FLOAT, 0, nullptr);

		if (not multiTexCoords .empty ())
			shaderNode -> enableTexCoordAttrib (multiTexCoordBufferIds, GL_FLOAT, { }, { });

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
				const auto & particle = particles [p];

				modelViewMatrix .origin (origin);
				modelViewMatrix .translate (particle .position);

				shaderNode -> setParticle (p, particle, modelViewMatrix);

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
				const auto & particle = particles [p];

				modelViewMatrix .origin (origin);
				modelViewMatrix .translate (particle .position);

				shaderNode -> setParticle (p, particle, modelViewMatrix);

				for (const auto & element : elements)
				{
					glDrawArrays (element .vertexMode (), element .first (), element .count ());
				}
			}
		}

		for (size_t i = 0, size = attribNodes .size (); i < size; ++ i)
			attribNodes [i] -> disable (shaderNode);

		// Disable shader

		shaderNode -> disableFogDepthAttrib ();
		shaderNode -> disableColorAttrib ();
		shaderNode -> disableTexCoordAttrib ();
		shaderNode -> disableNormalAttrib ();
		shaderNode -> disableVertexAttrib ();
		shaderNode -> disable ();

		glBindBuffer (GL_ARRAY_BUFFER, 0);
		shaderNode -> disable ();
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

		if (fogDepthBufferId)
			glDeleteBuffers (1, &fogDepthBufferId);

		if (colorBufferId)
			glDeleteBuffers (1, &colorBufferId);

		if (not multiTexCoordBufferIds .empty ())
			glDeleteBuffers (multiTexCoordBufferIds .size (), multiTexCoordBufferIds .data ());

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
