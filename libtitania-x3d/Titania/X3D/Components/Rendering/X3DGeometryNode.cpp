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

namespace titania {
namespace X3D {

X3DGeometryNode::X3DGeometryNode () :
	                   X3DNode (),               
	                     solid (true),           // SFBool  [ ]      solid            TRUE
	                       ccw (true),           // SFBool  [ ]      ccw              TRUE
	               creaseAngle (),               // SFFloat [ ]      creaseAngle      0           [0,∞)
	                      bbox (),               
	                  texCoord (),               
	textureCoordinateGenerator (NULL),           
	                    colors (),               
	                colorsRGBA (),               
	                   normals (),               
	                  vertices (),               
	                vertexMode (),               
	               bufferUsage (GL_STATIC_DRAW), 
	          texCoordBufferId (0),              
	             colorBufferId (0),              
	            normalBufferId (0),              
	             pointBufferId (0)               
{
	addNodeType (X3DConstants::X3DGeometryNode);
}

void
X3DGeometryNode::setup ()
{
	X3DNode::setup ();

	if (GLEW_ARB_vertex_buffer_object)
		update ();

	//else
	//	std::clog << "Warning: X3DGeometryNode::setup: The glew vertex buffer objects are not supported." << std::endl;
}

void
X3DGeometryNode::initialize ()
{
	X3DNode::initialize ();

	if (GLEW_ARB_vertex_buffer_object)
	{
		glGenBuffers (1, &texCoordBufferId);
		glGenBuffers (1, &colorBufferId);
		glGenBuffers (1, &normalBufferId);
		glGenBuffers (1, &pointBufferId);
	}
}

void
X3DGeometryNode::eventsProcessed ()
{
	X3DNode::eventsProcessed ();

	update ();
}

const Box3f
X3DGeometryNode::getBBox ()
{
	if (bbox == Box3f ())
		return bbox = createBBox ();

	return bbox;
}

Box3f
X3DGeometryNode::createBBox ()
{
	if (vertices .size ())
	{
		Vector3f min = vertices [0];
		Vector3f max = min;

		for (const auto & vertex : vertices)
		{
			min = math::min (min, vertex);
			max = math::max (max, vertex);
		}

		return Box3f (min, max, true);
	}

	return Box3f ();
}

bool
X3DGeometryNode::intersect (const Line3f & hitRay, Vector3f & hitPoint) const
{
	if (bbox .intersect (hitRay, hitPoint))
	{
		return true;
	}

	return false;
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
X3DGeometryNode::refineNormals (const NormalIndex & normalIndex, std::vector <Vector3f> & normals)
{
__LOG__ << ccw << std::endl;

	if (not ccw)
	{
		for (auto & normal : normals)
			normal .negate ();
	}
	
	if (creaseAngle == 0.0f)
		return;

	float cosCreaseAngle = std::cos (creaseAngle);

	std::vector <Vector3f> _normals (normals .size ());

	for (const auto & point : normalIndex)
	{
		for (const auto & index : point .second)
		{
			Vector3f         n;
			const Vector3f & m = normals [index];

			for (const auto & index : point .second)
			{
				if (dot (normals [index], m) > cosCreaseAngle)
					n += normals [index];
			}

			_normals [index] = n .normalize ();
		}
	}

	normals .swap (_normals);
}

void
X3DGeometryNode::update ()
{
	clear ();
	build ();
	transfer ();
}

void
X3DGeometryNode::clear ()
{
	bbox = Box3f ();

	texCoord .clear ();
	textureCoordinateGenerator = NULL;
	colors     .clear ();
	colorsRGBA .clear ();
	normals    .clear ();
	vertices   .clear ();
}

void
X3DGeometryNode::build ()
{ }

void
X3DGeometryNode::transfer ()
{
	glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector2f) * texCoord .size (), texCoord .data (), bufferUsage);

	if (colors .size ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Color3f) * colors .size (), colors .data (), bufferUsage);
	}
	else if (colorsRGBA .size ())
	{
		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glBufferData (GL_ARRAY_BUFFER, sizeof (Color4f) * colorsRGBA .size (), colorsRGBA .data (), bufferUsage);
	}

	glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * normals .size (), normals .data (), bufferUsage);

	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (Vector3f) * vertices .size (), vertices .data (), bufferUsage);

	glBindBuffer (GL_ARRAY_BUFFER, 0);

	bufferUsage = GL_DYNAMIC_DRAW;
}

void
X3DGeometryNode::display ()
{
	if (not vertices .size ())
		return;

	if (solid)
		glEnable (GL_CULL_FACE);

	else
		glDisable (GL_CULL_FACE);

	glFrontFace (ccw ? GL_CCW : GL_CW);

	if (glIsEnabled (GL_TEXTURE_2D))
	{
		if (textureCoordinateGenerator)
			textureCoordinateGenerator -> enable ();

		else if (texCoord .size ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferId);
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer (2, GL_FLOAT, 0, 0);
		}
	}

	if (colors .size () or colorsRGBA .size ())
	{
		if (glIsEnabled (GL_LIGHTING))
			glEnable (GL_COLOR_MATERIAL);

		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (colors .size () ? 3 : 4, GL_FLOAT, 0, 0);
	}

	if (glIsEnabled (GL_LIGHTING))
	{
		if (normals .size ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
			glEnableClientState (GL_NORMAL_ARRAY);
			glNormalPointer (GL_FLOAT, 0, 0);
		}
	}

	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, 0);

	glDrawArrays (vertexMode, 0, vertices .size ());

	if (textureCoordinateGenerator)
		textureCoordinateGenerator -> disable ();

	glDisableClientState (GL_TEXTURE_COORD_ARRAY);
	glDisableClientState (GL_COLOR_ARRAY);
	glDisableClientState (GL_NORMAL_ARRAY);
	glDisableClientState (GL_VERTEX_ARRAY);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
}

void
X3DGeometryNode::dispose ()
{
	if (texCoordBufferId)
		glDeleteBuffers (1, &texCoordBufferId);

	if (colorBufferId)
		glDeleteBuffers (1, &colorBufferId);

	if (normalBufferId)
		glDeleteBuffers (1, &normalBufferId);

	if (pointBufferId)
		glDeleteBuffers (1, &pointBufferId);

	X3DNode::dispose ();
}

} // X3D
} // titania
