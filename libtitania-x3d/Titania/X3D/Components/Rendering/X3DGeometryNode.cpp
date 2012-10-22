/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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
 ******************************************************************************/

#include "X3DGeometryNode.h"

namespace titania {
namespace X3D {

X3DGeometryNode::X3DGeometryNode () :
	X3DNode (), 
	//	     creaseAngle (),             // SFFloat [ ]      creaseAngle      0           [0,∞)
	//	           solid (true)          // SFBool  [ ]      solid            TRUE
	//	             ccw (true),         // SFBool  [ ]      ccw              TRUE
	//	  colorPerVertex (true),         // SFBool  [ ]      colorPerVertex   TRUE
	//	 normalPerVertex (true),         // SFBool  [ ]      normalPerVertex  TRUE
	//	           color (),             // SFNode [in,out] color            NULL        [X3DColorObject]
	//	           coord (),             // SFNode [in,out] coord            NULL        [X3DCoordinateNode]
	//	        fogCoord (),             // SFNode [in,out] fogCoord         [ ]         [FogCoordinate]
	//	          normal (),             // SFNode [in,out] normal           NULL        [X3DNormalNode]
	//	        texCoord (),             // SFNode [in,out] texCoord         NULL        [X3DTextureCoordinateNode]
	//	         polygon (GL_TRIANGLES),
	static_draw (false),
	texCoordBufferId (0),
	colorBufferId (0),
	normalBufferId (0),
	pointBufferId (0)
{
	addNodeType (X3DGeometryNodeType);
}

void
X3DGeometryNode::initialize ()
{
	X3DNode::initialize ();
		
	if (not GLEW_ARB_vertex_buffer_object)
		throw std::runtime_error ("X3DGeometryNode::initialize: The glew vertex buffer objects are not supported.");

	glGenBuffers (1, &texCoordBufferId);
	glGenBuffers (1, &colorBufferId);
	glGenBuffers (1, &normalBufferId);
	glGenBuffers (1, &pointBufferId);
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
	if (not glPoints  .size ())
		update ();

	return bbox;
}

Box3f
X3DGeometryNode::createBBox ()
{
	if (glIndices >= 3)
	{
		Vector3f min = Vector3f (glPoints [0], glPoints [1], glPoints [2]);
		Vector3f max = min;

		int points = glIndices * 3;

		for (int i = 3; i < points; i += 3)
		{
			Vector3f point = Vector3f (glPoints [i], glPoints [i + 1], glPoints [i + 2]);
			min = math::min (min, point);
			max = math::max (max, point);
		}

		Vector3f size   = max - min;
		Vector3f center = min + size * 0.5f;

		//size = size .max(Vector3f(1e-5, 1e-5, 1e-5));

		return Box3f (size, center);
	}

	return Box3f ();
}

bool
X3DGeometryNode::intersect (const Line3f & hitRay, Hit*) const
{
	if (bbox .intersect (hitRay))
	{
		return true;
	}

	return false;
}

void
X3DGeometryNode::refineNormals (std::vector <Vector3f> & normals, const VertexMap & vertexMap, const float creaseAngle, const bool ccw)
{
	if (not ccw)                     // if refined normals are less then this then place this at the end
	{
		for (auto & normal : normals) // FIXME std::generate ???
			normal = -normal;
	}

	if (creaseAngle == 0)            // FIXME why generate normals if creaseAngle == 0
		return;

	float cosCreaseAngle = std::cos (creaseAngle);

	std::vector <Vector3f> _normals (normals .size ());

	for (const auto & index : vertexMap)
	{
		for (const auto & vertex : index .second)
		{
			Vector3f         n;
			const Vector3f & m = normals [vertex];

			for (const auto & triangle : index .second)
			{
				if (dot (normals [triangle], m) > cosCreaseAngle)
					n += normals [triangle];
			}

			_normals [vertex] = normalize (n);
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
	bbox = createBBox ();
}

void
X3DGeometryNode::clear ()
{
	solid = true;
	ccw   = GL_CCW;
	glTexCoord .clear ();
	textureCoordinateGenerator = nullptr;
	glNumColors                = 3;
	glColors  .clear ();
	glNormals .clear ();
	glPoints  .clear ();
	glIndices = 0;
	glMode    = GL_TRIANGLES;
}

void
X3DGeometryNode::build ()
{ }

void
X3DGeometryNode::transfer ()
{
	GLenum usage = static_draw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW;

	glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * glTexCoord .size (), &glTexCoord [0], usage);

	glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * glColors .size (), &glColors [0], usage);

	glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * glNormals .size (), &glNormals [0], usage);

	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);
	glBufferData (GL_ARRAY_BUFFER, sizeof (GLfloat) * glPoints .size (), &glPoints [0], usage);

	glBindBuffer (GL_ARRAY_BUFFER, 0);

	static_draw = false;
}

void
X3DGeometryNode::display ()
{
	if (not glPoints  .size ())
		update ();

	draw ();
}

void
X3DGeometryNode::draw ()
{
	if (not glIndices or not glPoints .size ())
		return;

	if (solid)
	{
		glEnable (GL_CULL_FACE);
		glCullFace (GL_BACK);
	}
	else
		glDisable (GL_CULL_FACE);

	glFrontFace (ccw);

	if (glIsEnabled (GL_TEXTURE_2D))
	{
		if (textureCoordinateGenerator)
			textureCoordinateGenerator -> enable ();
		else if (glTexCoord .size ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, texCoordBufferId);
			glEnableClientState (GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer (2, GL_FLOAT, 0, 0);
		}
	}

	if (glColors .size ())
	{
		if (glIsEnabled (GL_LIGHTING))
			glEnable (GL_COLOR_MATERIAL);

		glBindBuffer (GL_ARRAY_BUFFER, colorBufferId);
		glEnableClientState (GL_COLOR_ARRAY);
		glColorPointer (glNumColors, GL_FLOAT, 0, 0);
	}

	if (glIsEnabled (GL_LIGHTING))
	{
		if (glNormals .size ())
		{
			glBindBuffer (GL_ARRAY_BUFFER, normalBufferId);
			glEnableClientState (GL_NORMAL_ARRAY);
			glNormalPointer (GL_FLOAT, 0, 0);
		}
	}

	glBindBuffer (GL_ARRAY_BUFFER, pointBufferId);
	glEnableClientState (GL_VERTEX_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, 0);

	glDrawArrays (glMode, 0, glIndices);

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
