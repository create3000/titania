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

#include "IndexedFaceSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Rendering/Tessellator.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Rendering/Normal.h"
#include "../Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const ComponentType IndexedFaceSet::component      = ComponentType::GEOMETRY_3D;
const std::string   IndexedFaceSet::typeName       = "IndexedFaceSet";
const std::string   IndexedFaceSet::containerField = "geometry";

IndexedFaceSet::Fields::Fields () :
	       convex (new SFBool (true)),
	  creaseAngle (new SFFloat ()),
	texCoordIndex (new MFInt32 ()),
	   colorIndex (new MFInt32 ()),
	  normalIndex (new MFInt32 ()),
	   coordIndex (new MFInt32 ())
{ }

IndexedFaceSet::IndexedFaceSet (X3DExecutionContext* const executionContext) :
	            X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DComposedGeometryNode (),
	                 fields ()
{
	addType (X3DConstants::IndexedFaceSet);

	addField (inputOutput,    "metadata",          metadata ());

	addField (initializeOnly, "solid",             solid ());
	addField (initializeOnly, "ccw",               ccw ());
	addField (initializeOnly, "convex",            convex ());
	addField (initializeOnly, "creaseAngle",       creaseAngle ());
	addField (initializeOnly, "colorPerVertex",    colorPerVertex ());
	addField (initializeOnly, "normalPerVertex",   normalPerVertex ());

	addField (inputOutput,    "colorIndex",        colorIndex ());
	addField (inputOutput,    "texCoordIndex",     texCoordIndex ());
	addField (inputOutput,    "normalIndex",       normalIndex ());
	addField (inputOutput,    "coordIndex",        coordIndex ());

	addField (inputOutput,    "attrib",            attrib ());
	addField (inputOutput,    "fogCoord",          fogCoord ());
	addField (inputOutput,    "color",             color ());
	addField (inputOutput,    "texCoord",          texCoord ());
	addField (inputOutput,    "normal",            normal ());
	addField (inputOutput,    "coord",             coord ());
}

X3DBaseNode*
IndexedFaceSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedFaceSet (executionContext);
}

size_t
IndexedFaceSet::getTexCoordIndex (const size_t index) const
{
	if (index < texCoordIndex () .size ())
		return texCoordIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getColorIndex (const size_t index, const bool) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getColorIndex (const size_t index) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return index;
}

size_t
IndexedFaceSet::getNormalIndex (const size_t index, const bool) const
{
	if (index < normalIndex () .size ())
		return normalIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getNormalIndex (const size_t index) const
{
	if (index < normalIndex () .size ())
		return normalIndex () [index];

	return index;
}

void
IndexedFaceSet::build ()
{
	// Tessellate

	PolygonArray polygons;
	size_t       reserve = 0;

	tessellate (convex (), polygons, reserve);

	// Build arrays

	if (not getCoord () or polygons .empty ())
		return;

	// Vertex attribute

	std::vector <std::vector <float>> attribArrays (getAttrib () .size ());

	for (size_t a = 0, size = getAttrib () .size (); a < size; ++ a)
		attribArrays [a] .reserve (reserve);

	// Color

	if (getColor ())
		getColors () .reserve (reserve);

	// TextureCoordinate

	if (getTexCoord ())
		getTexCoord () -> init (getTexCoords (), reserve);

	// Normal

	getNormals () .reserve (reserve);

	// Vertices

	getVertices () .reserve (reserve);

	// Fill GeometryNode

	int    face              = 0;
	GLenum vertexMode        = getVertexMode (polygons [0] .elements [0] .size ());
	GLenum currentVertexMode = 0;
	size_t vertices          = 0;

	for (const auto polygon : polygons)
	{
		for (const auto element : polygon .elements)
		{
			currentVertexMode = getVertexMode (element .size ());

			if (currentVertexMode not_eq vertexMode or vertexMode == GL_POLYGON)
			{
				const size_t size  = getVertices () .size ();
				const size_t count = size - vertices;

				addElements (vertexMode, count);

				vertices   = size;
				vertexMode = currentVertexMode;
			}

			for (const auto & i : element)
			{
				const size_t index = coordIndex () [i];

				for (size_t a = 0, size = getAttrib () .size (); a < size; ++ a)
					getAttrib () [a] -> addValue (attribArrays [a], index);

				if (getColor ())
				{
					if (colorPerVertex ())
						getColor () -> addColor (getColors (), getColorIndex (i, true));

					else
						getColor () -> addColor (getColors (), getColorIndex (face));
				}

				if (getTexCoord ())
					getTexCoord () -> addTexCoord (getTexCoords (), getTexCoordIndex (i));

				if (getNormal ())
				{
					if (normalPerVertex ())
						getNormal () -> addVector (getNormals (), getNormalIndex (i, true));

					else
						getNormal () -> addVector (getNormals (), getNormalIndex (face));
				}

				getCoord () -> addVertex (getVertices (), index);
			}
		}

		++ face;
	}

	addElements (vertexMode, getVertices () .size () - vertices);

	// Autogenerate normal if not specified.

	if (not getNormal ())
		buildNormals (polygons);

	setSolid (solid ());
	setCCW (ccw ());
	setAttribs (getAttrib (), attribArrays);
	setTextureCoordinate (getTexCoord ());
}

void
IndexedFaceSet::tessellate (const bool convex, PolygonArray & polygons, size_t & numVertices)
{
	if (not getCoord ())
		return;

	if (not coordIndex () .empty ())
	{
		std::unique_ptr <Tessellator> tessellator;

		if (not convex)
			tessellator .reset (new Tessellator ());

		// Add -1 (polygon end marker) to coordIndex if not present.
		if (coordIndex () .back () > -1)
			coordIndex () .emplace_back (-1);

		// Construct triangle array and determine the number of used points.
		size_t i = 0;

		polygons .emplace_back ();

		for (const auto & index : coordIndex ())
		{
			Vertices & vertices = polygons .back () .vertices;

			if (index > -1)
			{
				// Add vertex index.
				vertices .emplace_back (i);
			}
			else
			{
				// Negativ index.

				if (not vertices .empty ())
				{
					// Closed polygon.
					if (vertices .front () == vertices .back ())
						vertices .pop_back ();

					switch (vertices .size ())
					{
						case 0:
						case 1:
						case 2:
						{
							vertices .clear ();
							break;
						}
						case 3:
						{
							numVertices += 3;

							// Add polygon with one triangle.

							polygons .back () .elements .emplace_back (std::move (vertices));
							polygons .emplace_back ();
							break;
						}
						default:
						{
							numVertices += convex ? vertices .size () : (vertices .size () - 2) * 3;

							// Tessellate polygons.

							tessellate (tessellator, polygons);

							if (polygons .back () .elements .empty ())
								vertices .clear ();
							else
								polygons .emplace_back ();

							break;
						}
					}
				}
			}

			++ i;
		}

		if (polygons .back () .elements .empty ())
			polygons .pop_back ();
	}
}


void
IndexedFaceSet::tessellate (const std::unique_ptr <Tessellator> & tessellator, PolygonArray & polygons)
{
	Vertices &     vertices = polygons .back () .vertices;
	ElementArray & elements = polygons .back () .elements;

	if (not tessellator)
	{
		elements .emplace_back (std::move (vertices));
		return;
	}

	tessellator -> begin_polygon ();
	tessellator -> begin_contour ();

	for (const auto & i : vertices)
		getCoord () -> addVertex (*tessellator, coordIndex () [i], i);

	tessellator -> end_contour ();
	tessellator -> end_polygon ();

	for (const auto & polygonElement : tessellator -> polygon ())
	{
		switch (polygonElement .type ())
		{
			case GL_TRIANGLE_FAN:
			{
				for (size_t i = 1, size = polygonElement .size () - 1; i < size; ++ i)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygonElement [0] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [i] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [i + 1] .data ()));
				}

				break;
			}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygonElement .size () - 2; i < size; ++ i)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygonElement [is_odd (i) ? i + 1 : i] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [is_odd (i) ? i : i + 1] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [i + 2] .data ()));
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygonElement [i] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [i + 1] .data ()));
					elements .back () .emplace_back (std::get <0> (polygonElement [i + 2] .data ()));
				}

				break;
			}
			default:
				break;
		}
	}
}

void
IndexedFaceSet::buildNormals (const PolygonArray & polygons)
{
	const auto normals = createNormals (polygons);

	for (const auto & polygon : polygons)
	{
		for (const auto & element : polygon .elements)
		{
			for (const auto & i : element)
			{
				getNormals () .emplace_back (normals [i]);
			}
		}
	}
}

void
IndexedFaceSet::addTexCoords ()
{
	const auto texCoordNode = getExecutionContext () -> createNode <TextureCoordinate> ();

	texCoordIndex () .clear ();
	texCoord () = texCoordNode;

	Vector3f min;
	float    Ssize;
	int      Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	std::map <int32_t, size_t> indices; // coord index, texCoord index

	for (const auto & index : coordIndex ())
	{
		if (index < 0)
			continue;

		if (indices .emplace (index, texCoordNode -> point () .size ()) .second)
		{
			const auto vertex = getCoord () -> get1Point (index);

			texCoordNode -> point () .emplace_back ((vertex [Sindex] - min [Sindex]) / Ssize,
		                                           (vertex [Tindex] - min [Tindex]) / Ssize);
		}
	}

	for (const auto & index : coordIndex ())
	{
		if (index < 0)
			texCoordIndex () .emplace_back (-1);

		else
			texCoordIndex () .emplace_back (indices [index]);
	}

	getExecutionContext () -> realize ();
}

void
IndexedFaceSet::addNormals ()
{
	PolygonArray polygons;
	size_t       reserve = 0;

	tessellate (true, polygons, reserve);

	const auto normals    = createNormals (polygons);
	const auto normalNode = getExecutionContext () -> createNode <Normal> ();

	normalIndex () .clear ();
	normal () = normalNode;

	size_t i      = 0;
	auto   normal = normals .begin ();

	for (const auto & index : coordIndex ())
	{
		if (index < 0)
			normalIndex () .emplace_back (-1);

		else
		{
			normalIndex () .emplace_back (i ++);
			normalNode -> vector () .emplace_back (*normal);
		}

		++ normal;
	}

	getExecutionContext () -> realize ();
}

std::vector <Vector3f>
IndexedFaceSet::createNormals (const PolygonArray & polygons) const
{
	std::vector <Vector3f> normals;

	NormalIndex normalIndex;
	Vector3f    normal;

	for (const auto & polygon : polygons)
	{
		const auto & vertices = polygon .vertices .empty () ? polygon .elements [0] : polygon .vertices;

		switch (vertices .size ())
		{
			case 3:
			{
				normal = getCoord () -> getNormal (coordIndex () [vertices [0]],
				                                   coordIndex () [vertices [1]],
				                                   coordIndex () [vertices [2]]);
				break;
			}
			case 4:
			{
				normal = getCoord () -> getNormal (coordIndex () [vertices [0]],
				                                   coordIndex () [vertices [1]],
				                                   coordIndex () [vertices [2]],
				                                   coordIndex () [vertices [3]]);
				break;
			}
			default:
			{
				// Determine polygon normal.
				// Or use Newell's method https://www.opengl.org/wiki/Calculating_a_Surface_Normal

				normal = Vector3f ();

				for (const auto & element : polygon .elements)
				{
					for (size_t i = 0, size = element .size (); i < size; ++ i)
					{
						normal += getCoord () -> getNormal (coordIndex () [element [i]],
						                                    coordIndex () [element [(i + 1) % size]],
						                                    coordIndex () [element [(i + 2) % size]]);
					}
				}

				normal .normalize ();
			}
		}

		// Add a normal index for each point.
		for (size_t i = 0, size = vertices .size (); i < size; ++ i)
			normalIndex [coordIndex () [vertices [i]]] .emplace_back (normals .size () + i);

		// Add this normal for each vertex and for -1.
		normals .resize (normals .size () + vertices .size () + 1, normal);
	}

	refineNormals (normalIndex, normals, creaseAngle (), ccw ());

	return normals;
}

SFNode
IndexedFaceSet::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> metadata ()       = metadata ();
	geometry -> colorIndex ()     = colorIndex ();
	geometry -> coordIndex ()     = coordIndex ();
	geometry -> colorPerVertex () = colorPerVertex ();
	geometry -> attrib ()         = attrib ();
	geometry -> fogCoord ()       = fogCoord ();
	geometry -> color ()          = color ();
	geometry -> coord ()          = coord ();

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D

} // titania
