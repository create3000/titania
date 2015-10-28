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

#include "ElevationGrid.h"

#include "../../Browser/Core/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const ComponentType ElevationGrid::component      = ComponentType::GEOMETRY_3D;
const std::string   ElevationGrid::typeName       = "ElevationGrid";
const std::string   ElevationGrid::containerField = "geometry";

ElevationGrid::Fields::Fields () :
	     xDimension (new SFInt32 ()),
	     zDimension (new SFInt32 ()),
	       xSpacing (new SFFloat (1)),
	       zSpacing (new SFFloat (1)),
	          solid (new SFBool (true)),
	            ccw (new SFBool (true)),
	    creaseAngle (new SFFloat ()),
	 colorPerVertex (new SFBool (true)),
	normalPerVertex (new SFBool (true)),
	         attrib (new MFNode ()),
	       fogCoord (new SFNode ()),
	          color (new SFNode ()),
	       texCoord (new SFNode ()),
	         normal (new SFNode ()),
	         height (new MFFloat ())
{ }

ElevationGrid::ElevationGrid (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	    attribNodes (),
	      colorNode (),
	   texCoordNode (),
	     normalNode (),
	    transparent (false)
{
	addType (X3DConstants::ElevationGrid);

	addField (inputOutput,    "metadata",        metadata ());

	addField (initializeOnly, "xDimension",      xDimension ());
	addField (initializeOnly, "zDimension",      zDimension ());
	addField (initializeOnly, "xSpacing",        xSpacing ());
	addField (initializeOnly, "zSpacing",        zSpacing ());

	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (initializeOnly, "creaseAngle",     creaseAngle ());
	addField (initializeOnly, "colorPerVertex",  colorPerVertex ());
	addField (initializeOnly, "normalPerVertex", normalPerVertex ());

	addField (inputOutput,    "attrib",          attrib ());
	addField (inputOutput,    "fogCoord",        fogCoord ());
	addField (inputOutput,    "color",           color ());
	addField (inputOutput,    "texCoord",        texCoord ());
	addField (inputOutput,    "normal",          normal ());
	addField (inputOutput,    "height",          height ());

	xSpacing ()    .setUnit (UnitCategory::LENGTH);
	zSpacing ()    .setUnit (UnitCategory::LENGTH);
	creaseAngle () .setUnit (UnitCategory::ANGLE);
	height ()      .setUnit (UnitCategory::LENGTH);

	addChildren (attribNodes,
	             colorNode,
	             texCoordNode,
	             normalNode);
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

	attrib ()   .addInterest (this, &ElevationGrid::set_attrib);
	color ()    .addInterest (this, &ElevationGrid::set_color);
	texCoord () .addInterest (this, &ElevationGrid::set_texCoord);
	normal ()   .addInterest (this, &ElevationGrid::set_normal);

	set_attrib ();
	set_color ();
	set_texCoord ();
	set_normal ();
}

void
ElevationGrid::set_attrib ()
{
	for (const auto & node : attribNodes)
		node -> removeInterest (this);

	std::vector <X3DVertexAttributeNode*> value;

	for (const auto & node : attrib ())
	{
		const auto attribNode = x3d_cast <X3DVertexAttributeNode*> (node);
		
		if (attribNode)
			value .emplace_back (attribNode);
	}

	attribNodes .set (value .begin (), value .end ());

	for (const auto & node : attribNodes)
		node -> addInterest (this);
}

void
ElevationGrid::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (this);
		colorNode -> removeInterest (this, &ElevationGrid::set_transparency);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (this);
		colorNode -> addInterest (this, &ElevationGrid::set_transparency);
		
		set_transparency ();
	}
	else
		transparent = false;
}

void
ElevationGrid::set_transparency ()
{
	transparent = colorNode -> isTransparent ();
}

void
ElevationGrid::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (this);

	texCoordNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (this);
}

void
ElevationGrid::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (this);

	normalNode .set (x3d_cast <X3DNormalNode*> (normal ()));

	if (normalNode)
		normalNode -> addInterest (this);
}

Box3f
ElevationGrid::createBBox ()
{
	if (xDimension () < 2 or zDimension () < 2)
		return Box3f ();

	const size_t vertices = xDimension () * zDimension ();

	const float x = xSpacing () * (xDimension () - 1);
	const float z = zSpacing () * (zDimension () - 1);

	float miny = getHeight (0);
	float maxy = getHeight (0);

	for (size_t i = 1; i < vertices; ++ i)
	{
		miny = std::min <float> (miny, getHeight (i));
		maxy = std::max <float> (maxy, getHeight (i));
	}

	const float y = maxy - miny;

	const Vector3f size   = Vector3f (x, y, z);
	const Vector3f center = Vector3f (x / 2, miny + y / 2, z / 2);

	return Box3f (size, center);
}

float
ElevationGrid::getHeight (const size_t index) const
{
	if (index < height () .size ())
		return height () [index];

	return 0;
}

std::vector <Vector4f>
ElevationGrid::createTexCoord () const
{
	std::vector <Vector4f> texCoord;
	texCoord .reserve (xDimension () * zDimension ());

	const float xSize = xDimension () - 1;
	const float zSize = zDimension () - 1;

	for (int32_t z = 0; z < zDimension (); ++ z)
	{
		for (int32_t x = 0; x < xDimension (); ++ x)
		{
			texCoord .emplace_back (x / xSize,
			                        z / zSize,
			                        0,
			                        1);
		}
	}

	return texCoord;
}

std::vector <Vector3f>
ElevationGrid::createNormals (const std::vector <Vector3f> & points, const std::vector <size_t> & coordIndex, const float creaseAngle) const
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	for (auto index = coordIndex .cbegin (); index not_eq coordIndex .cend (); index += 3)
	{
		for (size_t i = 0; i < 3; ++ i)
			normalIndex [*(index + i)] .emplace_back (normals .size () + i);

		const Vector3f normal = math::normal (points [*(index)],
		                                      points [*(index + 1)],
		                                      points [*(index + 2)]);

		normals .resize (normals .size () + 3, normal);
	}

	refineNormals (normalIndex, normals, creaseAngle, ccw ());

	return normals;
}

// p1 - p4 
//  | \ |
// p2 - p3

std::vector <size_t>
ElevationGrid::createCoordIndex () const
{
	std::vector <size_t> coordIndex;
	coordIndex .reserve ((xDimension () - 1) * (zDimension () - 1) * 6);

	for (int32_t z = 0, size = zDimension () - 1; z < size; ++ z)
	{
		for (int32_t x = 0, size = xDimension () - 1; x < size; ++ x)
		{
			const auto i1 =       z * xDimension () + x;
			const auto i2 = (z + 1) * xDimension () + x;
			const auto i3 = (z + 1) * xDimension () + (x + 1);
			const auto i4 =       z * xDimension () + (x + 1);

			coordIndex .emplace_back (i1); // p1
			coordIndex .emplace_back (i2); // p2
			coordIndex .emplace_back (i3); // p3

			coordIndex .emplace_back (i1); // p1
			coordIndex .emplace_back (i3); // p3
			coordIndex .emplace_back (i4); // p4
		}
	}

	return coordIndex;
}

std::vector <Vector3f>
ElevationGrid::createPoints () const
{
	std::vector <Vector3f> points;
	points .reserve (xDimension () * zDimension ());

	for (int32_t z = 0; z < zDimension (); ++ z)
	{
		for (int32_t x = 0; x < xDimension (); ++ x)
		{
			points .emplace_back (xSpacing () * x,
			                      getHeight (x + z * xDimension ()),
			                      zSpacing () * z);
		}
	}

	return points;
}

void
ElevationGrid::build ()
{
	if (xDimension () < 2 or zDimension () < 2)
		return;

	const std::vector <size_t>   coordIndex = createCoordIndex ();
	const std::vector <Vector3f> points     = createPoints ();

	getVertices () .reserve (coordIndex .size ());

	// Vertex attribute

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
		attribArrays [a] .reserve (coordIndex .size ());

	// Color

	if (colorNode)
		getColors () .reserve (coordIndex .size ());

	// TexCoord

	std::vector <Vector4f> texCoords;

	if (texCoordNode)
		texCoordNode -> init (getTexCoords (), coordIndex .size ());
	else
	{
		texCoords = std::move (createTexCoord ());
		getTexCoords () .emplace_back ();
		getTexCoords () [0] .reserve (coordIndex .size ());
	}

	// Normals

	if (normalNode)
		getNormals () .reserve (coordIndex .size ());
	else
		getNormals () = createNormals (points, coordIndex, creaseAngle ());

	// Build geometry

	int32_t face = 0;

	std::vector <size_t>::const_iterator index;

	for (index = coordIndex .begin (); index not_eq coordIndex .end (); ++ face)
	{
		for (int32_t p = 0; p < 6; ++ p, ++ index)
		{
			const size_t i = *index;

			for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
				attribNodes [a] -> addValue (attribArrays [a], i);

			if (texCoordNode)
				texCoordNode -> addTexCoord (getTexCoords (), i);

			else
				getTexCoords () [0] .emplace_back (texCoords [i]);

			if (colorNode)
			{
				if (colorPerVertex ())
					colorNode -> addColor (getColors (), i);

				else
					colorNode -> addColor (getColors (), face);
			}

			if (normalNode)
			{
				if (normalPerVertex ())
					normalNode -> addVector (getNormals (), i);

				else
					normalNode -> addVector (getNormals (), face);
			}

			getVertices  () .emplace_back (points [i]);
		}
	}

	addElements (GL_TRIANGLES, getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());
	setAttribs (attribNodes, attribArrays);
	setTextureCoordinate (texCoordNode);
}

void
ElevationGrid::addNormals ()
{
	const auto coordIndex   = createCoordIndex ();
	const auto points       = createPoints ();
	const auto normals      = createNormals (points, coordIndex, M_PI);
	const auto normalNode   = getExecutionContext () -> createNode <Normal> ();
	const auto xDimension_1 = xDimension () - 1;
	const auto zDimension_1 = zDimension () - 1;
	const auto xDimension_2 = xDimension () - 2;
	const auto zDimension_2 = zDimension () - 2;

	for (int32_t z = 0; z < zDimension_1; ++ z)
	{
		for (int32_t x = 0; x < xDimension_1; ++ x)
		{
			const auto i = (x + z * xDimension_1) * 6;

			normalNode -> vector () .emplace_back (normals [i]);
		}

		const auto i = (xDimension_2 + z * xDimension_1) * 6;

		normalNode -> vector () .emplace_back (normals [i + 3]);
	}

	for (int32_t x = 0; x < xDimension_1; ++ x)
	{
		const auto i = (x + zDimension_2 * xDimension_1) * 6;

		normalNode -> vector () .emplace_back (normals [i + 1]);
	}

	const auto i = (xDimension_2 + zDimension_2 * xDimension_1) * 6;

	normalNode -> vector () .emplace_back (normals [i + 2]);

	//

	normal () = normalNode;

	getExecutionContext () -> realize ();
}

SFNode
ElevationGrid::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto texCoord = texCoordNode ? X3DPtr <TextureCoordinate> () : getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	geometry -> metadata ()        = metadata ();
	geometry -> solid ()           = solid ();
	geometry -> ccw ()             = ccw ();
	geometry -> creaseAngle ()     = creaseAngle ();
	geometry -> colorPerVertex ()  = colorPerVertex ();
	geometry -> normalPerVertex () = normalPerVertex ();
	geometry -> attrib ()          = attrib ();
	geometry -> fogCoord ()        = fogCoord ();
	geometry -> color ()           = color ();

	if (texCoordNode)
		geometry -> texCoord () = texCoordNode;
	else
		geometry -> texCoord () = texCoord;

	geometry -> normal () = normal ();
	geometry -> coord ()  = coord;

	const auto coordIndex = createCoordIndex ();
	const auto points     = createPoints ();

	coord -> point () .assign (points .begin (), points .end ());

	if (texCoord)
	{
		const auto texCoords = createTexCoord ();
		
		for (const auto & point : texCoords)
			texCoord -> point () .emplace_back (point .x (), point .y ());
	}

	for (size_t i = 0, size = coordIndex .size (); i < size; i += 3)
	{
		geometry -> coordIndex () .emplace_back (coordIndex [i]);
		geometry -> coordIndex () .emplace_back (coordIndex [i + 1]);
		geometry -> coordIndex () .emplace_back (coordIndex [i + 2]);
		geometry -> coordIndex () .emplace_back (-1);
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
