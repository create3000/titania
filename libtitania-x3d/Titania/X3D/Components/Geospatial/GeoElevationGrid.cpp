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

#include "GeoElevationGrid.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string GeoElevationGrid::componentName  = "Geospatial";
const std::string GeoElevationGrid::typeName       = "GeoElevationGrid";
const std::string GeoElevationGrid::containerField = "geometry";

GeoElevationGrid::Fields::Fields () :
	  geoGridOrigin (new SFVec3d ()),
	         yScale (new SFFloat (1)),
	     xDimension (new SFInt32 ()),
	       xSpacing (new SFDouble (1)),
	     zDimension (new SFInt32 ()),
	       zSpacing (new SFDouble (1)),
	          solid (new SFBool (true)),
	            ccw (new SFBool (true)),
	    creaseAngle (new SFDouble ()),
	 colorPerVertex (new SFBool (true)),
	normalPerVertex (new SFBool (true)),
	          color (new SFNode ()),
	       texCoord (new SFNode ()),
	         normal (new SFNode ()),
	         height (new MFDouble ())
{ }

GeoElevationGrid::GeoElevationGrid (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DGeometryNode (),
	X3DGeospatialObject (),
	             fields (),
	          colorNode (),
	       texCoordNode (),
	         normalNode (),
	        transparent (false)
{
	addField (inputOutput,    "metadata",        metadata ());
	addField (initializeOnly, "geoSystem",       geoSystem ());
	addField (initializeOnly, "geoGridOrigin",   geoGridOrigin ());

	addField (inputOutput,    "yScale",          yScale ());
	addField (initializeOnly, "xDimension",      xDimension ());
	addField (initializeOnly, "xSpacing",        xSpacing ());
	addField (initializeOnly, "zDimension",      zDimension ());
	addField (initializeOnly, "zSpacing",        zSpacing ());

	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (initializeOnly, "creaseAngle",     creaseAngle ());
	addField (initializeOnly, "colorPerVertex",  colorPerVertex ());
	addField (initializeOnly, "normalPerVertex", normalPerVertex ());

	addField (inputOutput,    "color",           color ());
	addField (inputOutput,    "texCoord",        texCoord ());
	addField (inputOutput,    "normal",          normal ());
	addField (inputOutput,    "height",          height ());
	addField (initializeOnly, "geoOrigin",       geoOrigin ());

	addChildren (colorNode,
	             texCoordNode,
	             normalNode);
}

X3DBaseNode*
GeoElevationGrid::create (X3DExecutionContext* const executionContext) const
{
	return new GeoElevationGrid (executionContext);
}

void
GeoElevationGrid::initialize ()
{
	X3DGeometryNode::initialize ();
	X3DGeospatialObject::initialize ();

	color ()    .addInterest (this, &GeoElevationGrid::set_color);
	texCoord () .addInterest (this, &GeoElevationGrid::set_texCoord);
	normal ()   .addInterest (this, &GeoElevationGrid::set_normal);

	set_color ();
	set_texCoord ();
	set_normal ();
}

void
GeoElevationGrid::set_color ()
{
	if (colorNode)
		colorNode -> removeInterest (this);

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
		colorNode -> addInterest (this);

	// Transparent

	transparent = colorNode and colorNode -> isTransparent ();
}

void
GeoElevationGrid::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (this);

	texCoordNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (this);
}

void
GeoElevationGrid::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (this);

	normalNode .set (x3d_cast <X3DNormalNode*> (normal ()));

	if (normalNode)
		normalNode -> addInterest (this);
}

double
GeoElevationGrid::getHeight (const size_t index) const
{
	if (index < height () .size ())
		return height () [index] * yScale ();

	return 0;
}

std::vector <Vector4f>
GeoElevationGrid::createTexCoord () const
{
	std::vector <Vector4f> texCoord;
	texCoord .reserve (xDimension () * zDimension ());

	const double xSize = xDimension () - 1;
	const double zSize = zDimension () - 1;

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
GeoElevationGrid::createNormals (const std::vector <Vector3f> & points, const std::vector <size_t> & coordIndex) const
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	for (auto index = coordIndex .cbegin (); index not_eq coordIndex .cend (); index += 6)
	{
		for (size_t i = 0; i < 6; ++ i)
			normalIndex [*(index + i)] .emplace_back (normals .size () + i);

		const Vector3f normal = math::normal (points [*(index)],
		                                      points [*(index + 1)],
		                                      points [*(index + 4)],
		                                      points [*(index + 2)]);

		normals .resize (normals .size () + 6, normal);
	}

	refineNormals (normalIndex, normals, creaseAngle (), ccw ());

	return normals;
}

// p2 - p3
//  | \ |
// p1 - p4 

std::vector <size_t>
GeoElevationGrid::createCoordIndex () const
{
	std::vector <size_t> coordIndex;
	coordIndex .reserve ((xDimension () - 1) * (zDimension () - 1) * 6);

	for (int32_t z = 0, size = zDimension () - 1; z < size; ++ z)
	{
		for (int32_t x = 0, size = xDimension () - 1; x < size; ++ x)
		{
			// Triangle one
			coordIndex .emplace_back (z * xDimension () + x);             // p1
			coordIndex .emplace_back (z * xDimension () + (x + 1));       // p4
			coordIndex .emplace_back ((z + 1) * xDimension () + x);       // p2

			// Triangle two
			coordIndex .emplace_back (z * xDimension () + (x + 1));       // p4
			coordIndex .emplace_back ((z + 1) * xDimension () + (x + 1)); // p3
			coordIndex .emplace_back ((z + 1) * xDimension () + x);       // p2
		}
	}

	return coordIndex;
}

std::vector <Vector3f>
GeoElevationGrid::createPoints () const
{
	std::vector <Vector3f> points;
	points .reserve (xDimension () * zDimension ());

	// When the geoSystem is "GD", xSpacing refers to the number of units of longitude in angle base units between
	// adjacent height values and zSpacing refers to the number of units of latitude in angle base units between
	// vertical height values.

	// When the geoSystem is "UTM", xSpacing refers to the number of eastings (length base units) between adjacent
	// height values and zSpacing refers to the number of northings (length base units) between vertical height values.

	if (getReversedOrder ())
	{
		for (int32_t z = 0; z < zDimension (); ++ z)
		{
			for (int32_t x = 0; x < xDimension (); ++ x)
			{
				points .emplace_back (getCoord (Vector3d (xSpacing () * x, // longitude, easting
				                                          zSpacing () * z, // latitude, northing
				                                          getHeight (x + z * xDimension ())) + geoGridOrigin ()));
			}
		}
	}
	else
	{
		for (int32_t z = 0; z < zDimension (); ++ z)
		{
			for (int32_t x = 0; x < xDimension (); ++ x)
			{
				points .emplace_back (getCoord (Vector3d (zSpacing () * z, // latitude, northing
				                                          xSpacing () * x, // longitude, easting
				                                          getHeight (x + z * xDimension ())) + geoGridOrigin ()));
			}
		}
	}

	return points;
}

void
GeoElevationGrid::build ()
{
	if (xDimension () < 2 or zDimension () < 2)
		return;

	const std::vector <size_t>   coordIndex = createCoordIndex ();
	const std::vector <Vector3f> points     = createPoints ();

	getVertices () .reserve (coordIndex .size ());

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
		getNormals () = std::move (createNormals (points, coordIndex));

	// Build geometry

	int32_t face = 0;

	std::vector <size_t>::const_iterator index;

	for (index = coordIndex .begin (); index not_eq coordIndex .end (); ++ face)
	{
		for (int32_t p = 0; p < 6; ++ p, ++ index)
		{
			const size_t i = *index;

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
	setTextureCoordinate (texCoordNode);
}

void
GeoElevationGrid::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
