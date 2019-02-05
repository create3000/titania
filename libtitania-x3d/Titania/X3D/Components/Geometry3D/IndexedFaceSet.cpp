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

#include "IndexedFaceSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../EnvironmentalEffects/FogCoordinate.h"
#include "../Geospatial/GeoCoordinate.h"
#include "../NURBS/CoordinateDouble.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Normal.h"
#include "../Shaders/X3DVertexAttributeNode.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Texturing/MultiTextureCoordinate.h"
#include "../Texturing3D/TextureCoordinate3D.h"
#include "../Texturing3D/TextureCoordinate4D.h"

#include <Titania/Math/Mesh/Tessellator.h>

namespace titania {
namespace X3D {

const Component   IndexedFaceSet::component      = Component ("Geometry3D", 2);
const std::string IndexedFaceSet::typeName       = "IndexedFaceSet";
const std::string IndexedFaceSet::containerField = "geometry";

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

	creaseAngle () .setUnit (UnitCategory::ANGLE);
}

X3DBaseNode*
IndexedFaceSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedFaceSet (executionContext);
}

size_t
IndexedFaceSet::getVertexTexCoordIndex (const size_t index) const
{
	if (index < texCoordIndex () .size ())
		return texCoordIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getVertexColorIndex (const size_t index) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getFaceColorIndex (const size_t index) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return index;
}

size_t
IndexedFaceSet::getVertexNormalIndex (const size_t index) const
{
	if (index < normalIndex () .size ())
		return normalIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedFaceSet::getFaceNormalIndex (const size_t index) const
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

	GLenum vertexMode  = getVertexMode (polygons [0] .elements [0] .size ());
	size_t numVertices = 0;

	for (const auto & polygon : polygons)
	{
		const auto face = polygon .face;

		for (const auto & element : polygon .elements)
		{
			const auto currentVertexMode  = getVertexMode (element .size ());
			const auto currentNumVertices = getVertices () .size ();

			if (currentVertexMode not_eq vertexMode or (vertexMode == GL_POLYGON and currentNumVertices))
			{
				const auto count = currentNumVertices - numVertices;

				addElements (vertexMode, count);

				numVertices = currentNumVertices;
				vertexMode  = currentVertexMode;
			}

			for (const auto & i : element)
			{
				const size_t index = coordIndex () [i];

				for (size_t a = 0, size = getAttrib () .size (); a < size; ++ a)
					getAttrib () [a] -> addValue (attribArrays [a], index);

				if (getFogCoord ())
					getFogCoord () -> addDepth (getFogDepths (), index);

				if (getColor ())
				{
					if (colorPerVertex ())
						getColor () -> addColor (getColors (), getVertexColorIndex (i));

					else
						getColor () -> addColor (getColors (), getFaceColorIndex (face));
				}

				if (getTexCoord ())
					getTexCoord () -> addTexCoord (getTexCoords (), getVertexTexCoordIndex (i));

				if (getNormal ())
				{
					if (normalPerVertex ())
						getNormal () -> addVector (getNormals (), getVertexNormalIndex (i));

					else
						getNormal () -> addVector (getNormals (), getFaceNormalIndex (face));
				}

				getCoord () -> addVertex (getVertices (), index);
			}
		}
	}

	addElements (vertexMode, getVertices () .size () - numVertices);

	// Autogenerate normal if not specified.

	if (not getNormal ())
		buildNormals (polygons);

	setSolid (solid ());
	setCCW (ccw ());
	setAttribs (getAttrib (), attribArrays);
}

void
IndexedFaceSet::tessellate (const bool convex, PolygonArray & polygons, size_t & numVertices) const
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
			const_cast <IndexedFaceSet*> (this) -> coordIndex () .emplace_back (-1);

		// Construct triangle array and determine the number of used points.
		size_t face = 0;
		size_t i    = 0;

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

				polygons .back () .face = face ++;

				if (not vertices .empty ())
				{
					// Closed polygon.
					//if (coordIndex () [vertices .front ()] == coordIndex () [vertices .back ()])
					//	vertices .pop_back ();

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

							polygons .back () .elements .emplace_back (vertices);
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
IndexedFaceSet::tessellate (const std::unique_ptr <Tessellator> & tessellator, PolygonArray & polygons) const
{
	Vertices &     vertices = polygons .back () .vertices; // Index to coord index
	ElementArray & elements = polygons .back () .elements; // Index to vertices

	if (not tessellator)
	{
		elements .emplace_back (vertices);
		return;
	}

	tessellator -> clear ();
	tessellator -> begin_polygon ();
	tessellator -> begin_contour ();

	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		const auto index = vertices [i];
		getCoord () -> addVertex (*tessellator, coordIndex () [index], index);
	}

	tessellator -> end_contour ();
	tessellator -> end_polygon ();

	for (const auto & polygon : tessellator -> polygons ())
	{
		switch (polygon .type ())
		{
			case GL_TRIANGLE_FAN:
			{
				for (size_t i = 1, size = polygon .size () - 1; i < size; ++ i)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygon [0] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [i] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [i + 1] .data ()));
				}

				break;
			}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygon .size () - 2; i < size; ++ i)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygon [is_odd (i) ? i + 1 : i] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [is_odd (i) ? i : i + 1] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [i + 2] .data ()));
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygon .size (); i < size; i += 3)
				{
					// Add triangle to polygon.
					elements .emplace_back ();
					elements .back () .emplace_back (std::get <0> (polygon [i + 0] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [i + 1] .data ()));
					elements .back () .emplace_back (std::get <0> (polygon [i + 2] .data ()));
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

bool
IndexedFaceSet::isColorPerVertex () const
{
	return isPerVertex (colorIndex ());
}

bool
IndexedFaceSet::isNormalPerVertex () const
{
	return isPerVertex (normalIndex ());
}

bool
IndexedFaceSet::isPerVertex (const MFInt32 & indices) const
{
	int32_t first = -1;

	for (const int32_t index : indices)
	{
		if (index < 0)
		{
			first = -1;
			continue;
		}

		if (first == -1)
		{
			first = index;
		}
		else
		{
			if (index not_eq first)
				return true;
		}
	}

	return false;
}

void
IndexedFaceSet::addColorIndex ()
{
	if (colorIndex () .size ())
		return;

	if (colorPerVertex ())
	{
		colorIndex () = coordIndex ();
	}
	else
	{
		size_t face = 0;

		for (const auto & index : coordIndex ())
		{
			if (index < 0)
				colorIndex () .emplace_back (face ++);
		}
	}
}

void
IndexedFaceSet::addTexCoordIndex ()
{
	texCoordIndex () = coordIndex ();
}

void
IndexedFaceSet::addNormalIndex ()
{
	if (normalIndex () .size ())
		return;

	if (normalPerVertex ())
	{
		normalIndex () = coordIndex ();
	}
	else
	{
		size_t face = 0;

		for (const auto & index : coordIndex ())
		{
			if (index < 0)
				normalIndex () .emplace_back (face ++);
		}
	}
}

MFInt32
IndexedFaceSet::getColorIndex (const bool colorPerVertex) const
{
	return getIndex (colorPerVertex, this -> colorPerVertex (), colorIndex ());
}

MFInt32
IndexedFaceSet::getNormalIndex (const bool normalPerVertex) const
{
	return getIndex (normalPerVertex, this -> normalPerVertex (), normalIndex ());
}

MFInt32
IndexedFaceSet::getIndex (const bool perVertex, const bool currentPerVertex, const MFInt32 & currentIndices) const
{
	if (perVertex == currentPerVertex)
		return currentIndices;

	MFInt32 indices;

	if (perVertex)
	{
		// PerFace to PerVertex

		size_t face = 0;

		for (const auto & index : coordIndex ())
		{
			if (index < 0)
			{
				++ face;
				indices .emplace_back (-1);
				continue;
			}

			if (face < currentIndices .size ())
				indices .emplace_back (currentIndices [face]);
			else
				indices .emplace_back (face);
		}
	}
	else
	{
		// PerVertex to PerFace

		size_t face  = 0;
		bool   first = true;

		for (size_t i = 0, size = coordIndex () .size (); i < size; ++ i)
		{
			const int32_t index = coordIndex () [i];

			if (index < 0)
			{
				++ face;
				first = true;
				continue;
			}

			if (first)
			{
				first = false;

				if (i < currentIndices .size ())
				{
					indices .emplace_back (currentIndices [i]);
				}
				else
					indices .emplace_back (index);
			}
		}
	}

	return indices;
}

void
IndexedFaceSet::addFogCoords ()
{
	const auto fogCoordNode = getExecutionContext () -> createNode <FogCoordinate> ();

	fogCoord () = fogCoordNode;

	if (getCoord ())
		fogCoordNode -> resize (getCoord () -> getSize ());
}

void
IndexedFaceSet::addColors ()
{
	const auto colorNode = getExecutionContext () -> createNode <ColorRGBA> ();

	colorIndex () .clear ();
	color () = colorNode;

	if (coordIndex () .empty ())
		return;

	colorNode -> color () .emplace_back (1, 1, 1, 1);

	for (const auto & index : basic::make_const_range (coordIndex ()))
		colorIndex () .emplace_back (index < 0 ? -1 : 0);
}

void
IndexedFaceSet::addTexCoords ()
{
	const auto texCoordNode = getExecutionContext () -> createNode <TextureCoordinate> ();

	texCoordIndex () .clear ();
	texCoord () = texCoordNode;

	Vector3d min;
	double   Ssize;
	int32_t  Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	std::map <int32_t, size_t> indices; // coord index, texCoord index

	for (const auto & index : basic::make_const_range (coordIndex ()))
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

	for (const auto & index : basic::make_const_range (coordIndex ()))
	{
		if (index < 0)
			texCoordIndex () .emplace_back (-1);

		else
			texCoordIndex () .emplace_back (indices [index]);
	}
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

	for (size_t i = 0, size = coordIndex () .size (); i < size; ++ i)
	{
		const auto index = coordIndex () [i];

		if (index < 0)
		{
			normalIndex () .emplace_back (-1);
		}
		else
		{
			normalIndex () .emplace_back (normalNode -> vector () .size ());
			normalNode -> vector () .emplace_back (normals [i]);
		}
	}
}

std::vector <Vector3f>
IndexedFaceSet::createNormals (const PolygonArray & polygons) const
{
	std::vector <Vector3f> normals;

	NormalIndex normalIndex;
	Vector3f    normal;

	for (const auto & polygon : polygons)
	{
		const auto & vertices = polygon .vertices;

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
				normal = getPolygonNormal (vertices);
				break;
			}
		}

		// Add a normal index for each point.
		for (size_t i = 0, size = vertices .size (); i < size; ++ i)
			normalIndex [coordIndex () [vertices [i]]] .emplace_back (normals .size () + i);

		// Add this normal for each vertex
		normals .resize (normals .size () + vertices .size () + 1, normal);
	}

	refineNormals (normalIndex, normals, creaseAngle (), ccw ());

	return normals;
}

Vector3f
IndexedFaceSet::getPolygonNormal (const Vertices & vertices) const
{
	// Determine polygon normal.
	// We use Newell's method https://www.opengl.org/wiki/Calculating_a_Surface_Normal here:

	if (vertices .size () < 3)
	   return Vector3f ();

	float x = 0, y = 0, z = 0;

	auto next = getCoord () -> get1Point (coordIndex () [vertices [0]]);

	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		auto current = next;

		next = getCoord () -> get1Point (coordIndex () .at (vertices [(i + 1) % size]));

		x += (current .y () - next .y ()) * (current .z () + next .z ());
		y += (current .z () - next .z ()) * (current .x () + next .x ());
		z += (current .x () - next .x ()) * (current .y () + next .y ());
	}

	return normalize (Vector3f (x, y, z));
}

double
IndexedFaceSet::getPolygonArea (const Vertices & vertices) const
{
	double area = 0;

	// Get distances of face to hitPoint.
	
	if (vertices .size () < 3)
		return 0;

	if (convex () or vertices .size () == 3)
	{
		for (size_t v = 0, size = vertices .size () - 2; v < size; ++ v)
		{
		   const auto i0       = vertices [0];
		   const auto i1       = vertices [v + 1];
		   const auto i2       = vertices [v + 2];
			const auto index0   = coordIndex () [i0];
			const auto index1   = coordIndex () [i1];
			const auto index2   = coordIndex () [i2];
			const auto point0   = getCoord () -> get1Point (index0);
			const auto point1   = getCoord () -> get1Point (index1);
			const auto point2   = getCoord () -> get1Point (index2);

			area += Triangle3f (point0, point1, point2) .area ();
		}
	}
	else
	{
		math::tessellator <double, size_t> tessellator;
	
		tessellator .begin_polygon ();
		tessellator .begin_contour ();
	
		for (const auto & vertex : vertices)
			tessellator .add_vertex (getCoord () -> get1Point (coordIndex () [vertex]), vertex);
	
		tessellator .end_contour ();
		tessellator .end_polygon ();
	
		const auto triangles = tessellator .triangles ();

		for (size_t v = 0, size = triangles .size (); v < size; )
		{
			const auto & point0 = triangles [v ++] .point ();
			const auto & point1 = triangles [v ++] .point ();
			const auto & point2 = triangles [v ++] .point ();

			area += Triangle3f (point0, point1, point2) .area ();
		}
	}

	return area;
}

///  Removes degenerated edges and faces from coordIndex and rewites color, texCoord, normal index.
void
IndexedFaceSet::rebuildIndices ()
{
	std::vector <size_t> indices;
	std::vector <size_t> faceNumbers;

	size_t faceIndex  = 0;
	size_t faceNumber = 0;
	size_t count      = 0;

	for (const int32_t index : basic::make_const_range (coordIndex ()))
	{
		if (index < 0)
		{
			rebuildIndices (faceIndex, faceNumber, count, indices, faceNumbers);

			faceIndex  += count + 1;
			faceNumber += 1;
			count       = 0;
			continue;
		}

		++ count;
	}

	MFInt32 color;
	MFInt32 texCoord;
	MFInt32 normal;
	MFInt32 coord;

	size_t face = 0;

	for (const auto & vertex : indices)
	{
		const int32_t index      = coordIndex () .get1Value (vertex);
		const size_t  faceNumber = faceNumbers [face];

		if (index < 0)
		{
			if (not colorIndex () .empty ())
			{
				if (colorPerVertex ())
					color .emplace_back (-1);
				else
					color .emplace_back (getFaceColorIndex (faceNumber));
			}

			if (not texCoordIndex () .empty ())
				texCoord .emplace_back (-1);

			if (not normalIndex () .empty ())
			{
				if (normalPerVertex ())
					normal .emplace_back (-1);
				else
					normal .emplace_back (getFaceNormalIndex (faceNumber));
			}

			coord .emplace_back (-1);

		   ++ face;

			continue;
		}

		if (not colorIndex () .empty ())
		{
			if (colorPerVertex ())
				color .emplace_back (getVertexColorIndex (vertex));
		}

		if (not texCoordIndex () .empty ())
			texCoord .emplace_back (getVertexTexCoordIndex (vertex));

		if (not normalIndex () .empty ())
		{
			if (normalPerVertex ())
				normal .emplace_back (getVertexNormalIndex (vertex));
		}

		coord .emplace_back (index);
	}

	colorIndex ()    = color;
	texCoordIndex () = texCoord;
	normalIndex ()   = normal;
	coordIndex ()    = coord;
}

void
IndexedFaceSet::rebuildIndices (const size_t faceIndex,
                                const size_t faceNumber,
                                const size_t count,
                                std::vector <size_t> & indices,
                                std::vector <size_t> & faceNumbers)
{
	const auto size = indices .size ();

	for (size_t i = faceIndex, last = faceIndex + count; i < last; ++ i)
	{
		const auto i0 = i;
		const auto i1 = i + 1 == last ? faceIndex : i + 1;

	   if (coordIndex () .get1Value (i0) not_eq coordIndex () .get1Value (i1))
	      indices .emplace_back (i0);
	}

	if (indices .size () - size < 3)
	{
		indices .resize (size);
	}
	else
	{
		indices .emplace_back (faceIndex + count);
		faceNumbers .emplace_back (faceNumber);
	}
}

///  Removes unused points from color color.
std::map <int32_t, int32_t>
IndexedFaceSet::rebuildColor ()
{
	std::map <int32_t, int32_t> map;

	const X3DPtr <X3DColorNode> colorNode (color ());

	if (not colorNode)
	   return map;

	// Build indices map

	for (const auto & index : basic::make_const_range (colorIndex () .empty () ? coordIndex () : colorIndex ()))
	{
		if (index < 0)
			continue;

		map .emplace (index, 0);
	}

	size_t i = 0;

	for (auto & pair : map)
	   pair .second = i ++;

	// Rebuild index
	   
	for (MFInt32::reference index : colorIndex ())
	{
		if (index < 0)
			continue;

		index = map [index];
	}

	// Rebuild node
	   
	std::vector <Color4f> colors;

	for (const auto & pair : map)
	   colors .emplace_back (colorNode -> get1Color (pair .first));

	colorNode -> assignColors (colors);

	return map;
}

///  Removes unused points from texCoord point.
std::map <int32_t, int32_t>
IndexedFaceSet::rebuildTexCoord ()
{
	std::map <int32_t, int32_t> map;

	const X3DPtr <X3DTextureCoordinateNode> texCoordNode (texCoord ());

	if (not texCoordNode)
	   return map;

	// Build indices map

	for (const auto & index : basic::make_const_range (texCoordIndex () .empty () ? coordIndex () : texCoordIndex ()))
	{
		if (index < 0)
			continue;

		map .emplace (index, 0);
	}

	size_t i = 0;

	for (auto & pair : map)
	   pair .second = i ++;

	// Rebuild index
	   
	for (MFInt32::reference index : texCoordIndex ())
	{
		if (index < 0)
			continue;

		index = map [index];
	}

	// Rebuild node
	   
	switch (texCoordNode -> getType () .back ())
	{
	   case X3DConstants::MultiTextureCoordinate:
		{
			const X3DPtr <MultiTextureCoordinate> multiTexCoordNode (texCoordNode);

			for (const auto & texCoordNode : multiTexCoordNode -> getTexCoord ())
			   rebuildTexCoord (texCoordNode, map);

			break;
		}
	   default:
		{
	      rebuildTexCoord (texCoordNode, map);
	      break;
		}
	}

	return map;
}

void
IndexedFaceSet::rebuildTexCoord (const X3DPtr <X3DTextureCoordinateNode> & texCoord, const std::map <int32_t, int32_t> & map)
{
	switch (texCoord -> getType () .back ())
	{
		case X3DConstants::TextureCoordinate:
		{
			const X3DPtr <TextureCoordinate> node (texCoord);

			std::vector <Vector2f> points;

			for (const auto & pair : map)
			{
				const auto t = node -> get1Point (pair .first);
				points .emplace_back (t .x (), t .y ());
			}

			node -> point () .assign (points .cbegin (), points .cend ());
			break;
		}
		case X3DConstants::TextureCoordinate3D:
		{
			const X3DPtr <TextureCoordinate3D> node (texCoord);

			std::vector <Vector3f> points;

			for (const auto & pair : map)
			{
				const auto t = node -> get1Point (pair .first);
				points .emplace_back (t .x (), t .y (), t .z ());
			}

			node -> point () .assign (points .begin (), points .end ());
			break;
		}
		case X3DConstants::TextureCoordinate4D:
		{
			const X3DPtr <TextureCoordinate4D> node (texCoord);

			std::vector <Vector4f> points;

			for (const auto & pair : map)
			   points .emplace_back (node -> get1Point (pair .first));

			node -> point () .assign (points .cbegin (), points .cend ());
			break;
		}
		default:
		   break;
	}
}

///  Removes unused points from normal vector.
std::map <int32_t, int32_t>
IndexedFaceSet::rebuildNormal ()
{
	std::map <int32_t, int32_t> map;

	const X3DPtr <X3DNormalNode> normalNode (normal ());

	if (not normalNode)
	   return map;

	// Build indices map

	for (const auto & index : basic::make_const_range (normalIndex () .empty () ? coordIndex () : normalIndex ()))
	{
		if (index < 0)
			continue;

		map .emplace (index, 0);
	}

	size_t i = 0;

	for (auto & pair : map)
	   pair .second = i ++;

	// Rebuild index
	   
	for (MFInt32::reference index : normalIndex ())
	{
		if (index < 0)
			continue;

		index = map [index];
	}

	// Rebuild node
	   
	std::vector <Vector3f> normals;

	for (const auto & pair : map)
	   normals .emplace_back (normalNode -> get1Vector (pair .first));

	normalNode -> assignVectors (normals);

	return map;
}

///  Removes unused points from coord point.
std::map <int32_t, int32_t>
IndexedFaceSet::rebuildCoord ()
{
	std::map <int32_t, int32_t> map;

	const X3DPtr <X3DCoordinateNode> coordNode (coord ());

	if (not coordNode)
		return map;

	// Build indices map

	for (const auto & index : basic::make_const_range (coordIndex ()))
	{
		if (index < 0)
			continue;

		map .emplace (index, 0);
	}

	size_t i = 0;

	for (auto & pair : map)
	   pair .second = i ++;

	// Rebuild coordIndex.
	   
	for (MFInt32::reference index : coordIndex ())
	{
		if (index < 0)
			continue;

		index = map [index];
	}

	// Rebuild node

	if (getFogCoord () and getFogCoord () -> getSize ())
	{
		std::vector <float> depths;

		for (const auto & pair : map)
		   depths .emplace_back (getFogCoord () -> get1Depth (pair .first));

		getFogCoord () -> assignDepths (depths);
	}

	std::vector <Vector3d> points;

	for (const auto & pair : map)
	   points .emplace_back (coordNode -> get1Point (pair .first));

	coordNode -> assignPoints (points);

	return map;
}

/// Merges all points in coord where the distance is less than @a distance.
/// Rewites color, texCoord, normal index, coord index.
void
IndexedFaceSet::mergePoints (const double distance)
{
	const auto Compare = [&distance] (const Vector3d & lhs, const Vector3d rhs)
	{
		return abs (lhs - rhs) < distance ? false : lhs < rhs;
	};

	const X3DPtr <X3DCoordinateNode> coordNode (coord ());

	if (not coordNode)
		return;

	std::map <Vector3d, std::pair <int32_t, int32_t>, std::function <bool (const Vector3d &, const Vector3d &)>> map (Compare);

	// Create points map.

	for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
	   map .emplace (coordNode -> get1Point (i), std::pair (i, map .size ()));
	
	// Rewrite coordIndex.

	for (MFInt32::reference index : coordIndex ())
	{
		try
		{
			if (index < 0)
				continue;

			index = map .at (coordNode -> get1Point (index)) .second;
		}
		catch (const std::out_of_range &)
		{
			__LOG__ << std::endl;
		}
	}

	// Rewrite coord point.

	if (getFogCoord () and getFogCoord () -> getSize ())
	{
		std::vector <float> depths (map .size ());

		for (const auto & [point, indices] : map)
			depths [indices .second] = getFogCoord () -> get1Depth (indices .first);

		getFogCoord () -> assignDepths (depths);
	}

	for (const auto & [point, indices] : map)
		coordNode -> set1Point (indices .second, point);

	coordNode -> resize (map .size ());

	// Remove degenerated faces.

	rebuildIndices ();
}

SFNode
IndexedFaceSet::toPrimitive () const
{
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> colorPerVertex () = colorPerVertex ();
	geometry -> attrib ()         = attrib ();
	geometry -> fogCoord ()       = fogCoord ();
	geometry -> color ()          = color ();
	geometry -> coord ()          = coord ();

	if (not coordIndex () .empty ())
	{
	   // The coord index must end with -1!

		std::set <std::pair <int32_t, int32_t>> lineIndex;

		auto &  lineColorIndex = geometry -> colorIndex ();
		auto &  lineCoordIndex = geometry -> coordIndex ();
		bool    line           = false;
		int32_t last           = -1;
		size_t  first          = 0;
		size_t  face           = 0;
	   
		for (size_t i = 1, size = coordIndex () .size (); i < size; ++ i)
		{
		   const auto    p        = i - 1;
			const int32_t previous = coordIndex () [p];
			int32_t       index    = coordIndex () [i];
			auto          c        = i;

			if (index == -1)
			{
				index = coordIndex () [first];
				c     = first;
			}

			const bool exists = not lineIndex .emplace (std::minmax (previous, index)) .second;

			if ((previous == -1 or exists) and line)
			{
				if (not colorIndex () .empty ())
				{
					if (colorPerVertex ())
						lineColorIndex .emplace_back (-1);
					else
						lineColorIndex .emplace_back (getFaceColorIndex (face));
				}

				lineCoordIndex .emplace_back (-1);

				line = false;
			}

			if (previous == -1)
			{
				first = i;
				face += 1;
				last  = -1;
				continue;
			}

			if (exists)
				continue;

			if (last not_eq previous)
			{
				if (not colorIndex () .empty ())
				{
					if (colorPerVertex ())
						lineColorIndex .emplace_back (getVertexColorIndex (p));
				}

				lineCoordIndex .emplace_back (previous);
			}

			if (not colorIndex () .empty ())
			{
				if (colorPerVertex ())
					lineColorIndex .emplace_back (getVertexColorIndex (c));
			}

			lineCoordIndex .emplace_back (index);

			last = index;
			line = true;
		}
	}

	return geometry;
}

IndexedFaceSet::~IndexedFaceSet ()
{ }

} // X3D
} // titania
