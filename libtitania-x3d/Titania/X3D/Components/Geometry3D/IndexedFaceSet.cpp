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
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Rendering/X3DNormalNode.h"
#include "../Texturing/X3DTextureCoordinateNode.h"
#include "../Texturing/TextureCoordinateGenerator.h"

namespace titania {
namespace X3D {

const std::string IndexedFaceSet::componentName  = "Geometry3D";
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
	                 fields (),
	            numPolygons ()
{
	addField (inputOutput,    "metadata",          metadata ());

	addField (initializeOnly, "solid",             solid ());
	addField (initializeOnly, "ccw",               ccw ());
	addField (initializeOnly, "convex",            convex ());
	addField (initializeOnly, "creaseAngle",       creaseAngle ());
	addField (initializeOnly, "colorPerVertex",    colorPerVertex ());
	addField (initializeOnly, "normalPerVertex",   normalPerVertex ());

	addField (inputOutput,    "texCoordIndex",     texCoordIndex ());
	addField (inputOutput,    "colorIndex",        colorIndex ());
	addField (inputOutput,    "normalIndex",       normalIndex ());
	addField (inputOutput,    "coordIndex",        coordIndex ());

	addField (inputOutput,    "attrib",            attrib ());
	addField (inputOutput,    "fogCoord",          fogCoord ());
	addField (inputOutput,    "texCoord",          texCoord ());
	addField (inputOutput,    "color",             color ());
	addField (inputOutput,    "normal",            normal ());
	addField (inputOutput,    "coord",             coord ());
}

X3DBaseNode*
IndexedFaceSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedFaceSet (executionContext);
}

void
IndexedFaceSet::initialize ()
{
	X3DComposedGeometryNode::initialize ();

	texCoordIndex () .addInterest (this, &IndexedFaceSet::set_texCoordIndex);
	colorIndex ()    .addInterest (this, &IndexedFaceSet::set_colorIndex);
	normalIndex ()   .addInterest (this, &IndexedFaceSet::set_normalIndex);
	coordIndex ()    .addInterest (this, &IndexedFaceSet::set_coordIndex);

	texCoord () .addInterest (this, &IndexedFaceSet::set_texCoordIndex);
	color ()    .addInterest (this, &IndexedFaceSet::set_colorIndex);
	normal ()   .addInterest (this, &IndexedFaceSet::set_normalIndex);
	coord ()    .addInterest (this, &IndexedFaceSet::set_coordIndex);

	set_coordIndex ();
}

void
IndexedFaceSet::set_coordIndex ()
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	numPolygons = 0;

	if (_coord)
	{
		if (not coordIndex () .empty ())
		{
			// Determine number of points and polygons.

			int32_t numPoints = -1;

			for (const auto & index : coordIndex ())
			{
				numPoints = std::max <int32_t> (numPoints, index);

				if (index < 0)
					++ numPolygons;
			}

			++ numPoints;

			if (coordIndex () .back () > -1)
				++ numPolygons;

			// Resize coord .point if to small
			_coord -> resize (numPoints);

			set_texCoordIndex ();
			set_colorIndex    ();
			set_normalIndex   ();
		}
	}
}

void
IndexedFaceSet::set_texCoordIndex ()
{
	auto _textureCoordinate = x3d_cast <X3DTextureCoordinateNode*> (texCoord ());

	if (_textureCoordinate)
	{
		// Fill up texCoordIndex if to small.
		for (size_t i = texCoordIndex () .size (), size = coordIndex () .size (); i < size; ++ i)
		{
			texCoordIndex () .emplace_back (coordIndex () [i]);
		}

		// Determine number of used texCoords.

		int numTexCoord = -1;

		for (const auto & index : texCoordIndex ())
		{
			numTexCoord = std::max <int32_t> (numTexCoord, index);
		}

		++ numTexCoord;

		// Resize textureCoordinate .point if to small.
		_textureCoordinate -> resize (numTexCoord);
	}
}

void
IndexedFaceSet::set_colorIndex ()
{
	auto _color = x3d_cast <X3DColorNode*> (color ());

	if (_color)
	{
		// Fill up colorIndex if to small.
		if (colorPerVertex ())
		{
			for (size_t i = colorIndex () .size (), size = coordIndex () .size (); i < size; ++ i)
			{
				colorIndex () .emplace_back (coordIndex () [i]);
			}
		}
		else
		{
			for (size_t i = colorIndex () .size (); i < numPolygons; ++ i)
			{
				colorIndex () .emplace_back (i);
			}
		}

		// Determine number of used colors.

		int numColors = -1;

		for (const auto & index : colorIndex ())
		{
			numColors = std::max <int32_t> (numColors, index);
		}

		++ numColors;

		// Resize color .color if to small.
		if (_color)
			_color -> resize (numColors);
	}
}

void
IndexedFaceSet::set_normalIndex ()
{
	auto _normal = x3d_cast <X3DNormalNode*> (normal ());

	if (_normal)
	{
		// Fill up normalIndex if to small.
		if (normalPerVertex ())
		{
			for (size_t i = normalIndex () .size (), size = coordIndex () .size (); i < size; ++ i)
			{
				normalIndex () .emplace_back (coordIndex () [i]);
			}
		}
		else
		{
			for (size_t i = normalIndex () .size (); i < numPolygons; ++ i)
			{
				normalIndex () .emplace_back (i);
			}
		}

		// Determine number of used normals.

		int numNormals = -1;

		for (const auto & index : normalIndex ())
		{
			numNormals = std::max <int32_t> (numNormals, index);
		}

		++ numNormals;

		// Resize normal .vector if to small.
		_normal -> resize (numNormals);
	}
}

void
IndexedFaceSet::build ()
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	// Tesselate

	PolygonArray polygons;
	size_t       reserve = 0;

	tesselate (polygons, reserve);

	// Build arrays

	if (not _coord or polygons .empty ())
		return;

	// Color

	auto _color = x3d_cast <X3DColorNode*> (color ());

	if (_color)
		getColors () .reserve (reserve);

	// TextureCoordinate

	auto _textureCoordinate          = x3d_cast <X3DTextureCoordinateNode*> (texCoord ());
	auto _textureCoordinateGenerator = x3d_cast <TextureCoordinateGenerator*> (texCoord ());

	if (_textureCoordinateGenerator)
		;
	else if (_textureCoordinate)
		_textureCoordinate -> init (getTexCoord (), reserve);
	else
		getTexCoord () .emplace_back ();

	// Normal

	auto _normal = x3d_cast <X3DNormalNode*> (normal ());

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
				size_t size  = getVertices () .size ();
				size_t count = size - vertices;

				addElements (vertexMode, count);

				vertices   = size;
				vertexMode = currentVertexMode;
			}

			for (const auto & i : element)
			{
				if (_color)
				{
					if (colorPerVertex ())
						_color -> addColor (getColors (), colorIndex () [i]);

					else
						_color -> addColor (getColors (), colorIndex () [face]);
				}

				if (_textureCoordinateGenerator)
					;
				else if (_textureCoordinate)
					_textureCoordinate -> addTexCoord (getTexCoord (), texCoordIndex () [i]);

				if (_normal)
				{
					if (normalPerVertex ())
						_normal -> addVector (getNormals (), normalIndex () [i]);

					else
						_normal -> addVector (getNormals (), normalIndex () [face]);
				}

				_coord -> addVertex (getVertices (), coordIndex () [i]);
			}
		}

		++ face;
	}

	addElements (vertexMode, getVertices () .size () - vertices);

	// Autogenerate normal and texCoord if not specified

	if (not _textureCoordinate)
		buildTexCoord ();

	if (not _normal)
		buildNormals (polygons);

	setTextureCoordinateGenerator (_textureCoordinateGenerator);
	setSolid (solid ());
	setCCW (ccw ());
}

void
IndexedFaceSet::buildNormals (const PolygonArray & polygons)
{
	std::vector <Vector3f> normals;

	NormalIndex normalIndex;

	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	for (const auto & polygon : polygons)
	{
		// Determine polygon normal.
		Vector3f normal;

		for (const auto & element : polygon .elements)
		{
			for (size_t i = 1, size = element .size () - 1; i < size; ++ i)
			{
				normal += _coord -> getNormal (coordIndex () [element [0]],
				                               coordIndex () [element [i]],
				                               coordIndex () [element [i + 1]]);
			}
		}

		// Add a normal index for each point.
		for (size_t i = 0, size = polygon .vertices .size (); i < size; ++ i)
			normalIndex [coordIndex () [polygon .vertices [i]]] .emplace_back (normals .size () + i);

		// Add this normal for each vertex.
		normals .resize (normals .size () + polygon .vertices .size (), normalize (normal));
		normals .emplace_back ();
	}

	refineNormals (normalIndex, normals, creaseAngle (), ccw ());

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
IndexedFaceSet::tesselate (PolygonArray & polygons, size_t & numVertices)
{
	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	if (not _coord)
		return;

	if (not coordIndex () .empty ())
	{
		// Add -1 (polygon end marker) to coordIndex if not present.
		if (coordIndex () .back () > -1)
			coordIndex () .emplace_back (-1);

		// Construct triangle array and determine the number of used points.
		size_t i = 0;

		Vertices vertices;

		for (const auto & index : coordIndex ())
		{
			if (index > -1)
				// Add vertex.
				vertices .emplace_back (i);

			else
			{
				// Negativ index.

				if (not vertices .empty ())
				{
					// Closed polygon.
					if (vertices .front () == vertices .back ())
						vertices .pop_back ();

					if (vertices .size () == 3)
					{
						numVertices += 3;

						// Add polygon with one triangle.
						polygons .emplace_back (std::move (vertices),
						                        std::move (ElementArray ({ std::move (Element ({ { vertices [0],
						                                                                           vertices [1],
						                                                                           vertices [2] } })) })));
					}
					else if (vertices .size () > 3)
					{
						numVertices += convex () ? vertices .size () : (vertices .size () - 2) * 3;

						// Tesselate polygons.
						polygons .emplace_back (std::move (vertices), std::move (tesselate (vertices)));
					}

					else
						vertices .clear ();
				}
			}

			++ i;
		}
	}
}

IndexedFaceSet::ElementArray
IndexedFaceSet::tesselate (const Vertices & vertices)
{
	if (convex ())
		return ElementArray { vertices };

	auto _coord = x3d_cast <X3DCoordinateNode*> (coord ());

	ElementArray elements;

	opengl::tesselator <size_t> tesselator;

	for (const auto & i : vertices)
		_coord -> addVertex (tesselator, coordIndex () [i], i);

	tesselator .tesselate ();

	for (const auto & polygonElement : tesselator .polygon ())
	{
		switch (polygonElement .type ())
		{
			case GL_TRIANGLE_FAN :
				{
					for (size_t i = 1, size = polygonElement .size () - 1; i < size; ++ i)
					{
						// Add triangle to polygon.
						elements .emplace_back (std::move (Element ({ { std::get <0> (polygonElement [0] .data ()),
						                                                std::get <0> (polygonElement [i] .data ()),
						                                                std::get <0> (polygonElement [i + 1] .data ()) } })));
					}

					break;
				}
			case GL_TRIANGLE_STRIP:
			{
				for (size_t i = 0, size = polygonElement .size () - 2; i < size; ++ i)
				{
					// Add triangle to polygon.
					elements .emplace_back (std::move (Element ({ { std::get <0> (polygonElement [is_odd (i) ? i + 1 : i] .data ()),
					                                                std::get <0> (polygonElement [is_odd (i) ? i : i + 1] .data ()),
					                                                std::get <0> (polygonElement [i + 2] .data ()) } })));
				}

				break;
			}
			case GL_TRIANGLES:
			{
				for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
				{
					// Add triangle to polygon.
					elements .emplace_back (std::move (Element ({ { std::get <0> (polygonElement [i] .data ()),
					                                                std::get <0> (polygonElement [i + 1] .data ()),
					                                                std::get <0> (polygonElement [i + 2] .data ()) } })));
				}

				break;
			}
			default:
				break;
		}
	}

	return elements;
}

//void
//IndexedFaceSet::buildTexCoord (const PolygonArray & polygons)
//{
//	Vector3f min;
//
//	float Ssize;
//	int   Sindex, Tindex;
//
//	getTexCoordParams (min, Ssize, Sindex, Tindex);
//
//	auto _coord = x3d_cast <X3DCoordinateNode*> (coord);
//
//	for (const auto & polygon : polygons)
//	{
//		for (const auto & element : polygon .elements)
//		{
//			for (const auto & i : element)
//			{
//				const Vector3f & point = _coord -> point [coordIndex [i]];
//
//				getTexCoord () .emplace_back ((point [Sindex] - min [Sindex]) / Ssize,
//				                              (point [Tindex] - min [Tindex]) / Ssize);
//			}
//		}
//	}
//}

} // X3D

} // titania
