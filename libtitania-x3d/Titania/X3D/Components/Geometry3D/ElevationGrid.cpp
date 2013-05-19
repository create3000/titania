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

#include "ElevationGrid.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Normal.h"
#include "../Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

ElevationGrid::Fields::Fields () :
	xDimension (new SFInt32 ()),
	xSpacing (new SFFloat (1)),
	zDimension (new SFInt32 ()),
	zSpacing (new SFFloat (1)),
	solid (new SFBool (true)),
	ccw (new SFBool (true)),
	creaseAngle (new SFFloat ()),
	colorPerVertex (new SFBool (true)),
	normalPerVertex (new SFBool (true)),
	attrib (new MFNode ()),
	fogCoord (new SFNode <X3DBaseNode> ()),
	texCoord (new SFNode <X3DBaseNode> ()),
	color (new SFNode <X3DBaseNode> ()),
	normal (new SFNode <X3DBaseNode> ()),
	height (new MFFloat ())
{ }

ElevationGrid::ElevationGrid (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	         fields ()                                                     
{
	setComponent ("Geometry3D");
	setTypeName ("ElevationGrid");

	addField (inputOutput,    "metadata",        metadata ());
	
	addField (initializeOnly, "xDimension",      xDimension ());
	addField (initializeOnly, "xSpacing",        xSpacing ());
	addField (initializeOnly, "zDimension",      zDimension ());
	addField (initializeOnly, "zSpacing",        zSpacing ());
	
	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (initializeOnly, "creaseAngle",     creaseAngle ());
	addField (initializeOnly, "colorPerVertex",  colorPerVertex ());
	addField (initializeOnly, "normalPerVertex", normalPerVertex ());
	
	addField (inputOutput,    "attrib",          attrib ());
	addField (inputOutput,    "fogCoord",        fogCoord ());
	addField (inputOutput,    "texCoord",        texCoord ());
	addField (inputOutput,    "color",           color ());
	addField (inputOutput,    "normal",          normal ());
	addField (inputOutput,    "height",          height ());
}

X3DBaseNode*
ElevationGrid::create (X3DExecutionContext* const executionContext) const
{
	return new ElevationGrid (executionContext);
}

void
ElevationGrid::initialize ()
{
	X3DGeometryNode::initialize ();
}

Box3f
ElevationGrid::createBBox ()
{
	if (xDimension () < 2 or zDimension () < 2)
		return Box3f ();

	size_t vertices = xDimension () * zDimension ();

	if (height () .size () < vertices)
		height () .resize (vertices);

	float x = xSpacing () * (xDimension () - 1);
	float z = zSpacing () * (zDimension () - 1);

	float miny = height () [0];
	float maxy = height () [0];

	for (size_t i = 1; i < vertices; ++ i)
	{
		miny = std::min <float> (miny, height () [i]);
		maxy = std::max <float> (maxy, height () [i]);
	}

	float y = maxy - miny;

	Vector3f size   = Vector3f (x, y, z);
	Vector3f center = Vector3f (x / 2, miny + y / 2, z / 2);

	return Box3f (size, center);
}

std::vector <Vector3f>
ElevationGrid::createTexCoord ()
{
	std::vector <Vector3f> texCoord;
	texCoord .reserve (xDimension () * zDimension ());

	for (float j = 0; j < zDimension (); ++ j)
	{
		for (float i = 0; i < xDimension (); ++ i)
		{
			texCoord .emplace_back (i / (xDimension () - 1),
			                        j / (zDimension () - 1),
			                        0);
		}
	}

	return texCoord;
}

std::vector <Vector3f>
ElevationGrid::createNormals (const std::vector <Vector3f> & points, const std::vector <size_t> & coordIndex)
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	for (auto index = coordIndex .cbegin (); index not_eq coordIndex .cend (); index += 6)
	{
		// p1, p2, p4, p3
		const Vector3f & p1 = points [*(index)];
		const Vector3f & p3 = points [*(index + 3)];

		const Vector3f & p2 = points [*(index + 1)];
		const Vector3f & p4 = points [*(index + 2)];

		Vector3f normal = cross (p3 - p1, p4 - p2);

		for (size_t i = 0; i < 6; ++ i)
			normalIndex [*(index + i)] .push_back (normals .size () + i);

		normals .resize (normals .size () + 6, normalize (normal));
	}

	refineNormals (normalIndex, normals, creaseAngle (), ccw ());

	return normals;
}

std::vector <size_t>
ElevationGrid::createCoordIndex ()
{
	std::vector <size_t> _coordIndex;
	_coordIndex .reserve ((xDimension () - 1) * (zDimension () - 1) * 6);

	for (int32_t j = 0; j < zDimension () - 1; ++ j)
	{
		for (int32_t i = 0; i < xDimension () - 1; ++ i)
		{
			for (int32_t p = 0; p < 6; ++ p)
			{
				// P1 - P4
				// |  / |
				// P2 - P3

				size_t index;

				switch (p)
				{
					// triangle one
					case 0: index =       j * xDimension () + i; break;         // P1
					case 1: index = (j + 1) * xDimension () + i; break;         // P2
					case 2: index =       j * xDimension () + (i + 1); break;   // P4
					// triangle two
					case 3: index = (j + 1) * xDimension () + (i + 1); break;   // P3
					case 4: index =       j * xDimension () + (i + 1); break;   // P4
					case 5: index = (j + 1) * xDimension () + i; break;         // P2
				}

				_coordIndex .push_back (index);
			}
		}
	}

	return _coordIndex;
}

std::vector <Vector3f>
ElevationGrid::createPoints ()
{
	std::vector <Vector3f> points;
	points .reserve (xDimension () * zDimension ());

	for (int32_t j = 0; j < zDimension (); ++ j)
	{
		for (int32_t i = 0; i < xDimension (); ++ i)
		{
			points .push_back (Vector3f (xSpacing () * i,
			                             height () [i + j * xDimension ()],
			                             zSpacing () * j));
		}
	}

	return points;
}

void
ElevationGrid::build ()
{
	if (xDimension () < 2 or zDimension () < 2)
		return;

	size_t vertices = xDimension () * zDimension ();
	size_t faces    = (xDimension () - 1) * (zDimension () - 1);

	if (height () .size () < vertices)
		height () .resize (vertices);

	std::vector <size_t>   coordIndex = createCoordIndex ();
	std::vector <Vector3f> points     = createPoints ();

	getVertices () .reserve (coordIndex .size ());

	std::vector <Vector3f> _texCoord;

	auto _textureCoordinate          = x3d_cast <TextureCoordinate*> (texCoord () .getValue ());
	auto _textureCoordinateGenerator = x3d_cast <TextureCoordinateGenerator*> (texCoord () .getValue ());

	if (_textureCoordinate)
	{
		if (_textureCoordinate -> point () .size () < vertices)
			_textureCoordinate -> point () .resize (vertices);

		getTexCoord () .reserve (coordIndex .size ());
	}
	else if (_textureCoordinateGenerator)
	{ }
	else
	{
		_texCoord = createTexCoord ();
		getTexCoord () .reserve (coordIndex .size ());
	}

	auto _color = x3d_cast <Color*> (color () .getValue ());

	if (_color)
	{
		if (colorPerVertex ())
		{
			if (_color -> color () .size () < vertices)
				_color -> color () .resize (vertices);
		}
		else
		{
			if (_color -> color () .size () < faces)
				_color -> color () .resize (faces);
		}

		getColors () .reserve (coordIndex .size ());
	}

	auto _colorRGBA = x3d_cast <ColorRGBA*> (color () .getValue ());

	if (_colorRGBA)
	{
		if (colorPerVertex ())
		{
			if (_colorRGBA -> color () .size () < vertices)
				_colorRGBA -> color () .resize (vertices);
		}
		else
		{
			if (_colorRGBA -> color () .size () < faces)
				_colorRGBA -> color () .resize (faces);
		}

		getColorsRGBA () .reserve (coordIndex .size ());
	}

	std::vector <Vector3f> normals;

	auto _normal = x3d_cast <Normal*> (normal () .getValue ());

	if (_normal)
	{
		if (normalPerVertex ())
		{
			if (_normal -> vector () .size () < vertices)
				_normal -> vector () .resize (vertices);
		}
		else
		{
			if (_normal -> vector () .size () < faces)
				_normal -> vector () .resize (faces);
		}
	}
	else
		normals = createNormals (points, coordIndex);

	getNormals () .reserve (coordIndex .size ());

	size_t  i    = 0;
	int32_t face = 0;

	std::vector <size_t>::const_iterator index;

	for (index = coordIndex .begin (); index not_eq coordIndex .end (); ++ face)
	{
		Vector3f faceNormal;

		if (_normal)
		{
			if (not normalPerVertex ())
				getNormals () .emplace_back (_normal -> vector () [face]);
		}

		for (int32_t p = 0; p < 6; ++ p, ++ index, ++ i)
		{
			if (_textureCoordinate)
			{
				const auto & t = _textureCoordinate -> point () [*index];
				getTexCoord () .emplace_back (t .getX (), t .getY (), 0);
			}
			else if (_textureCoordinateGenerator)
			{ }
			else
			{
				getTexCoord () .emplace_back (_texCoord [*index]);
			}

			if (_color)
			{
				if (colorPerVertex ())
					getColors () .emplace_back (_color -> color () [*index]);

				else
					getColors () .emplace_back (_color -> color () [face]);

			}
			else if (_colorRGBA)
			{
				float r = 0, g = 0, b = 0, a = 0;

				if (colorPerVertex ())
					_colorRGBA -> color () [*index] .getValue (r, g, b, a);

				else
					_colorRGBA -> color () [face] .getValue (r, g, b, a);

				getColorsRGBA () .emplace_back (r, g, b, 1 - a);
			}

			if (_normal)
			{
				if (normalPerVertex ())
					getNormals () .emplace_back (_normal -> vector () [*index]);
			}
			else
			{
				getNormals () .emplace_back (normals [i]);
			}

			getVertices  () .emplace_back (points [*index]);
		}
	}

	setTextureCoordinateGenerator (_textureCoordinateGenerator);
	addElements (GL_TRIANGLES, getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());
}

} // X3D
} // titania
