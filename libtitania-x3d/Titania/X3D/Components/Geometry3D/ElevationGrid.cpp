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

#include "ElevationGrid.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../EnvironmentalEffects/FogCoordinate.h"
#include "../Geometry3D/IndexedFaceSet.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/Normal.h"
#include "../Rendering/X3DColorNode.h"
#include "../Shaders/X3DVertexAttributeNode.h"
#include "../Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const Component   ElevationGrid::component      = Component ("Geometry3D", 3);
const std::string ElevationGrid::typeName       = "ElevationGrid";
const std::string ElevationGrid::containerField = "geometry";

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
	   fogCoordNode (),
	      colorNode (),
	   texCoordNode (),
	     normalNode (),
	         future (),
	      loadState (NOT_STARTED_STATE)
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

	addChildObjects (attribNodes,
	                 fogCoordNode,
	                 colorNode,
	                 texCoordNode,
	                 normalNode,
	                 future,
	                 loadState);
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

	attrib ()   .addInterest (&ElevationGrid::set_attrib,   this);
	fogCoord () .addInterest (&ElevationGrid::set_fogCoord, this);
	color ()    .addInterest (&ElevationGrid::set_color,    this);
	texCoord () .addInterest (&ElevationGrid::set_texCoord, this);
	normal ()   .addInterest (&ElevationGrid::set_normal,   this);

	set_attrib ();
	set_fogCoord ();
	set_color ();
	set_texCoord ();
	set_normal ();
}

void
ElevationGrid::set_attrib ()
{
	for (const auto & node : attribNodes)
		node -> removeInterest (&ElevationGrid::requestRebuild, this);

	std::vector <X3DVertexAttributeNode*> value;

	for (const auto & node : attrib ())
	{
		const auto attribNode = x3d_cast <X3DVertexAttributeNode*> (node);
		
		if (attribNode)
			value .emplace_back (attribNode);
	}

	attribNodes .set (value .cbegin (), value .cend ());

	for (const auto & node : attribNodes)
		node -> addInterest (&ElevationGrid::requestRebuild, this);
}

void
ElevationGrid::set_fogCoord ()
{
	if (fogCoordNode)
		fogCoordNode -> removeInterest (&ElevationGrid::requestRebuild, this);

	fogCoordNode = x3d_cast <FogCoordinate*> (fogCoord ());

	if (fogCoordNode)
		fogCoordNode -> addInterest (&ElevationGrid::requestRebuild, this);
}

void
ElevationGrid::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (&ElevationGrid::requestRebuild, this);
		colorNode -> isTransparent () .removeInterest (&ElevationGrid::set_transparent, this);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (&ElevationGrid::requestRebuild, this);
		colorNode -> isTransparent () .addInterest (&ElevationGrid::set_transparent, this);

		set_transparent ();
	}
	else
	{
		setTransparent (false);
	}
}

void
ElevationGrid::set_transparent ()
{
	setTransparent (colorNode -> isTransparent ());
}

void
ElevationGrid::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (&ElevationGrid::requestRebuild, this);

	texCoordNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (&ElevationGrid::requestRebuild, this);

	setTextureCoordinate (texCoordNode);
}

void
ElevationGrid::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (&ElevationGrid::requestRebuild, this);

	normalNode .set (x3d_cast <X3DNormalNode*> (normal ()));

	if (normalNode)
		normalNode -> addInterest (&ElevationGrid::requestRebuild, this);
}

Box3d
ElevationGrid::createBBox () const
{
	if (xDimension () < 2 or zDimension () < 2)
		return Box3d ();

	const size_t vertices = xDimension () * zDimension ();

	const double x = xSpacing () * (xDimension () - 1);
	const double z = zSpacing () * (zDimension () - 1);

	double miny = getHeight (0);
	double maxy = getHeight (0);

	for (size_t i = 1; i < vertices; ++ i)
	{
		miny = std::min <double> (miny, getHeight (i));
		maxy = std::max <double> (maxy, getHeight (i));
	}

	const double y = maxy - miny;

	const Vector3d size   = Vector3d (x, y, z);
	const Vector3d center = Vector3d (x / 2, miny + y / 2, z / 2);

	return Box3d (size, center);
}

double
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
ElevationGrid::createNormals (const std::vector <Vector3d> & points, const std::vector <size_t> & coordIndex, const double creaseAngle) const
{
	std::vector <Vector3f> normals;

	NormalIndex normalIndex;

	for (auto index = coordIndex .cbegin (); index not_eq coordIndex .cend (); index += 3)
	{
		for (size_t i = 0; i < 3; ++ i)
			normalIndex [*(index + i)] .emplace_back (normals .size () + i);

		const Vector3f normal = Triangle3f (points [*(index)], points [*(index + 1)], points [*(index + 2)]) .normal ();

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

std::vector <Vector3d>
ElevationGrid::createPoints () const
{
	std::vector <Vector3d> points;

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
	const std::vector <Vector3d> points     = createPoints ();

	// Vertex attribute

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	// TexCoord

	std::vector <Vector4f> texCoords;

	if (texCoordNode)
	{
		texCoordNode -> init (getMultiTexCoords ());
	}
	else
	{
		texCoords = createTexCoord ();

		getMultiTexCoords () .emplace_back ();
	}

	// Normals

	if (not normalNode)
		getNormals () = createNormals (points, coordIndex, creaseAngle () .getValue ());

	// Build geometry

	int32_t face = 0;

	std::vector <size_t>::const_iterator index;

	for (index = coordIndex .cbegin (); index not_eq coordIndex .cend (); ++ face)
	{
		for (int32_t p = 0; p < 6; ++ p, ++ index)
		{
			const size_t i = *index;

			for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
				attribNodes [a] -> addValue (attribArrays [a], i);

			if (fogCoordNode)
				fogCoordNode -> addDepth (getFogDepths (), i);

			if (colorNode)
			{
				if (colorPerVertex ())
					colorNode -> addColor (getColors (), i);

				else
					colorNode -> addColor (getColors (), face);
			}

			if (texCoordNode)
				texCoordNode -> addTexCoord (getMultiTexCoords (), i);

			else
				getTexCoords () .emplace_back (texCoords [i]);

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
}

std::pair <float, float>
ElevationGrid::getMinMaxHeight () const
{
	if (height () .empty ())
		return std::pair (0, 0);

	const auto pair = std::minmax_element (height () .cbegin (), height () .cend ());

	return std::pair (*pair .first, *pair .second);
}

void
ElevationGrid::loadHeightMap (const MFString & url, const float minHeight, const float maxHeight)
{
	using namespace std::placeholders;

	setLoadState (IN_PROGRESS_STATE);

	future .setValue (new TextureFuture (getExecutionContext (), url, std::bind (&ElevationGrid::setHeightMapTexture, this, _1, _2, minHeight, maxHeight)));
}

void
ElevationGrid::setHeightMapTexture (const basic::uri & url, const TexturePtr & texture, const float minHeight, const float maxHeight)
{
	if (not texture)
	{
		setLoadState (FAILED_STATE);
		return;
	}

	setLoadState (COMPLETE_STATE);

	setHeightMapImage (texture -> getImages () -> front (), minHeight, maxHeight);
}

void
ElevationGrid::setHeightMapImage (Magick::Image & image, const float minHeight, const float maxHeight)
{
	if (xDimension () < 1 or zDimension () < 1)
		return;

	// Scale image.

	Magick::Geometry geometry (xDimension (), zDimension ());

	geometry .aspect (true);

	image .filterType (Magick::LanczosFilter);
	image .zoom (geometry);
	image .flip ();

	// Get image data.

	Magick::Blob blob;

	image .magick ("GRAY");
	image .interlaceType (Magick::NoInterlace);
	image .endian (Magick::LSBEndian);
	image .depth (8);

	image .write (&blob);

	// Set height field.

	const auto   data   = static_cast <const uint8_t*> (blob .data ());
	const size_t size   = xDimension () * zDimension ();
	const auto   minMax = std::minmax_element (data, data + size);

	height () .resize (size);

	for (size_t i = 0; i < size; ++ i)
		height () [i] = project <float> (data [i], *minMax .first, *minMax .second, minHeight, maxHeight);
}

void
ElevationGrid::addNormals ()
{
	const auto coordIndex   = createCoordIndex ();
	const auto points       = createPoints ();
	const auto normals      = createNormals (points, coordIndex, pi <float>);
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
{
	const auto texCoord = texCoordNode ? X3DPtr <TextureCoordinate> () : getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

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

	coord -> point () .assign (points .cbegin (), points .cend ());

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

	return geometry;
}

ElevationGrid::~ElevationGrid ()
{ }

} // X3D
} // titania
