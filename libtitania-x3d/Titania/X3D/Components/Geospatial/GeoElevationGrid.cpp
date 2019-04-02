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

#include "GeoElevationGrid.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Geospatial/GeoCoordinate.h"
#include "../../Components/Rendering/Normal.h"
#include "../../Components/Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const Component   GeoElevationGrid::component      = Component ("Geospatial", 1);
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
	         height (new MFDouble ({ 0, 0 }))
{ }

GeoElevationGrid::GeoElevationGrid (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	    X3DGeometryNode (),
	X3DGeospatialObject (),
	             fields (),
	          colorNode (),
	       texCoordNode (),
	         normalNode (),
	             future (),
	          loadState (NOT_STARTED_STATE)
{
	addType (X3DConstants::GeoElevationGrid);

	addField (inputOutput,    "metadata",        metadata ());
	addField (initializeOnly, "geoOrigin",       geoOrigin ());
	addField (initializeOnly, "geoSystem",       geoSystem ());
	addField (initializeOnly, "geoGridOrigin",   geoGridOrigin ());

	addField (initializeOnly, "xDimension",      xDimension ());
	addField (initializeOnly, "zDimension",      zDimension ());
	addField (initializeOnly, "xSpacing",        xSpacing ());
	addField (initializeOnly, "zSpacing",        zSpacing ());
	addField (inputOutput,    "yScale",          yScale ());

	addField (initializeOnly, "solid",           solid ());
	addField (initializeOnly, "ccw",             ccw ());
	addField (initializeOnly, "creaseAngle",     creaseAngle ());
	addField (initializeOnly, "colorPerVertex",  colorPerVertex ());
	addField (initializeOnly, "normalPerVertex", normalPerVertex ());

	addField (inputOutput,    "color",           color ());
	addField (inputOutput,    "texCoord",        texCoord ());
	addField (inputOutput,    "normal",          normal ());
	addField (inputOutput,    "height",          height ());

	geoGridOrigin () .setUnit (UnitCategory::LENGTH);
	//xSpacing ()      .setUnit (???); // degree, or utm, set dynamically if geosystem changes
	//zSpacing ()      .setUnit (???); // degree, or utm, set dynamically if geosystem changes
	creaseAngle ()   .setUnit (UnitCategory::ANGLE);
	height ()        .setUnit (UnitCategory::LENGTH);

	geoGridOrigin () .isGeospatial (true);

	addChildObjects (colorNode,
	                 texCoordNode,
	                 normalNode,
	                 future,
	                 loadState);
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

	color ()    .addInterest (&GeoElevationGrid::set_color, this);
	texCoord () .addInterest (&GeoElevationGrid::set_texCoord, this);
	normal ()   .addInterest (&GeoElevationGrid::set_normal, this);

	set_color ();
	set_texCoord ();
	set_normal ();
}

void
GeoElevationGrid::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (&GeoElevationGrid::requestRebuild, this);
		colorNode -> isTransparent () .removeInterest (&GeoElevationGrid::set_transparent, this);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (&GeoElevationGrid::requestRebuild, this);
		colorNode -> isTransparent () .addInterest (&GeoElevationGrid::set_transparent, this);

		set_transparent ();
	}
	else
	{
		setTransparent (false);
	}
}

void
GeoElevationGrid::set_transparent ()
{
	setTransparent (colorNode -> isTransparent ());
}

void
GeoElevationGrid::set_texCoord ()
{
	if (texCoordNode)
		texCoordNode -> removeInterest (&GeoElevationGrid::requestRebuild, this);

	texCoordNode .set (x3d_cast <X3DTextureCoordinateNode*> (texCoord ()));

	if (texCoordNode)
		texCoordNode -> addInterest (&GeoElevationGrid::requestRebuild, this);

	setTextureCoordinate (texCoordNode);
}

void
GeoElevationGrid::set_normal ()
{
	if (normalNode)
		normalNode -> removeInterest (&GeoElevationGrid::requestRebuild, this);

	normalNode .set (x3d_cast <X3DNormalNode*> (normal ()));

	if (normalNode)
		normalNode -> addInterest (&GeoElevationGrid::requestRebuild, this);
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
GeoElevationGrid::createNormals (const std::vector <Vector3d> & points, const std::vector <size_t> & coordIndex, const double creaseAngle) const
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	for (auto index = coordIndex .cbegin (); index not_eq coordIndex .cend (); index += 4)
	{
		for (size_t i = 0; i < 4; ++ i)
			normalIndex [*(index + i)] .emplace_back (normals .size () + i);

		const Vector3d normal = math::normal (points [*(index)],
		                                      points [*(index + 1)],
		                                      points [*(index + 2)],
		                                      points [*(index + 3)]);

		normals .resize (normals .size () + 4, normal);
	}

	refineNormals (normalIndex, normals, creaseAngle, ccw ());

	return normals;
}

// p4 - p3
//  |   |
// p1 - p2

std::vector <size_t>
GeoElevationGrid::createCoordIndex () const
{
	std::vector <size_t> coordIndex;
	coordIndex .reserve ((xDimension () - 1) * (zDimension () - 1) * 4);

	for (int32_t z = 0, size = zDimension () - 1; z < size; ++ z)
	{
		for (int32_t x = 0, size = xDimension () - 1; x < size; ++ x)
		{
			coordIndex .emplace_back (z * xDimension () + x);             // p1
			coordIndex .emplace_back (z * xDimension () + (x + 1));       // p2
			coordIndex .emplace_back ((z + 1) * xDimension () + (x + 1)); // p3
			coordIndex .emplace_back ((z + 1) * xDimension () + x);       // p4
		}
	}

	return coordIndex;
}

std::vector <Vector3d>
GeoElevationGrid::createPoints () const
{
	using namespace std::placeholders;

	std::vector <Vector3d> points = createGeoPoints ();

	std::transform (points .cbegin (), points .cend (), points .begin (), std::bind (&GeoElevationGrid::getCoord, this, _1));

	return points;
}

std::vector <Vector3d>
GeoElevationGrid::createGeoPoints () const
{
	std::vector <Vector3d> points;
	points .reserve (xDimension () * zDimension ());

	// When the geoSystem is "GD", xSpacing refers to the number of units of longitude in angle base units between
	// adjacent height values and zSpacing refers to the number of units of latitude in angle base units between
	// vertical height values.

	// When the geoSystem is "UTM", xSpacing refers to the number of eastings (length base units) between adjacent
	// height values and zSpacing refers to the number of northings (length base units) between vertical height values.

	if (isStandardOrder ())
	{
		for (int32_t z = 0; z < zDimension (); ++ z)
		{
			for (int32_t x = 0; x < xDimension (); ++ x)
			{
				points .emplace_back (Vector3d (zSpacing () * z, // latitude, northing
				                                xSpacing () * x, // longitude, easting
				                                getHeight (x + z * xDimension ())) + geoGridOrigin ());
			}
		}
	}
	else
	{
		for (int32_t z = 0; z < zDimension (); ++ z)
		{
			for (int32_t x = 0; x < xDimension (); ++ x)
			{
				points .emplace_back (Vector3d (xSpacing () * x, // longitude, easting
				                                zSpacing () * z, // latitude, northing
				                                getHeight (x + z * xDimension ())) + geoGridOrigin ());
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
	const std::vector <Vector3d> points     = createPoints ();

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
		getNormals () = createNormals (points, coordIndex, creaseAngle ());

	// Build geometry

	int32_t face = 0;

	std::vector <size_t>::const_iterator index;

	for (index = coordIndex .cbegin (); index not_eq coordIndex .cend (); ++ face)
	{
		for (int32_t p = 0; p < 4; ++ p, ++ index)
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

	addElements (GL_QUADS, getVertices () .size ());
	setSolid (solid ());
	setCCW (ccw ());
}

void
GeoElevationGrid::loadHeightMap (const MFString & url, const double minHeight, const double maxHeight)
{
	using namespace std::placeholders;

	setLoadState (IN_PROGRESS_STATE);

	future .setValue (new TextureFuture (getExecutionContext (), url, std::bind (&GeoElevationGrid::setHeightMapTexture, this, _1, _2, minHeight, maxHeight)));
}

void
GeoElevationGrid::setHeightMapTexture (const basic::uri & url, const TexturePtr & texture, const double minHeight, const double maxHeight)
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
GeoElevationGrid::setHeightMapImage (Magick::Image & image, const double minHeight, const double maxHeight)
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
		height () [i] = project <double> (data [i], *minMax .first, *minMax .second, minHeight, maxHeight);
}

void
GeoElevationGrid::addNormals ()
{
	const auto coordIndex   = createCoordIndex ();
	const auto points       = createPoints ();
	const auto normals      = createNormals (points, coordIndex, pi <double>);
	const auto normalNode   = getExecutionContext () -> createNode <Normal> ();
	const auto xDimension_1 = xDimension () - 1;
	const auto zDimension_1 = zDimension () - 1;
	const auto xDimension_2 = xDimension () - 2;
	const auto zDimension_2 = zDimension () - 2;

	for (int32_t z = 0; z < zDimension_1; ++ z)
	{
		for (int32_t x = 0; x < xDimension_1; ++ x)
		{
			const auto i = (x + z * xDimension_1) * 4;

			normalNode -> vector () .emplace_back (normals [i]);
		}

		const auto i = (xDimension_2 + z * xDimension_1) * 4;

		normalNode -> vector () .emplace_back (normals [i + 3]);
	}

	for (int32_t x = 0; x < xDimension_1; ++ x)
	{
		const auto i = (x + zDimension_2 * xDimension_1) * 4;

		normalNode -> vector () .emplace_back (normals [i + 1]);
	}

	const auto i = (xDimension_2 + zDimension_2 * xDimension_1) * 4;

	normalNode -> vector () .emplace_back (normals [i + 2]);

	//

	normal () = normalNode;

	getExecutionContext () -> realize ();
}

SFNode
GeoElevationGrid::toPrimitive () const
{
	const auto texCoord = texCoordNode ? X3DPtr <TextureCoordinate> () : getExecutionContext () -> createNode <TextureCoordinate> ();
	const auto coord    = getExecutionContext () -> createNode <GeoCoordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedFaceSet> ();

	coord -> geoSystem () = geoSystem ();
	coord -> geoOrigin () = geoOrigin ();

	geometry -> solid ()           = solid ();
	geometry -> ccw ()             = ccw ();
	geometry -> creaseAngle ()     = creaseAngle ();
	geometry -> colorPerVertex ()  = colorPerVertex ();
	geometry -> normalPerVertex () = normalPerVertex ();;
	geometry -> color ()           = color ();

	if (texCoordNode)
		geometry -> texCoord () = texCoordNode;
	else
		geometry -> texCoord () = texCoord;

	geometry -> normal () = normal ();
	geometry -> coord ()  = coord;

	const auto coordIndex = createCoordIndex ();
	const auto points     = createGeoPoints ();

	coord -> point () .assign (points .cbegin (), points .cend ());

	if (texCoord)
	{
		const auto texCoords = createTexCoord ();
		
		for (const auto & point : texCoords)
			texCoord -> point () .emplace_back (point .x (), point .y ());
	}

	for (size_t i = 0, size = coordIndex .size (); i < size; i += 4)
	{
		geometry -> coordIndex () .emplace_back (coordIndex [i]);
		geometry -> coordIndex () .emplace_back (coordIndex [i + 1]);
		geometry -> coordIndex () .emplace_back (coordIndex [i + 2]);
		geometry -> coordIndex () .emplace_back (coordIndex [i + 3]);
		geometry -> coordIndex () .emplace_back (-1);
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

void
GeoElevationGrid::dispose ()
{
	X3DGeospatialObject::dispose ();
	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
