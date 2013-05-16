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
#include "../../Rendering/Tesselator.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Coordinate.h"
#include "../Rendering/Normal.h"
#include "../Texturing/TextureCoordinate.h"
#include "../Texturing/TextureCoordinateGenerator.h"
#include <iostream>

namespace titania {
namespace X3D {

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
	setComponent ("Geometry3D");
	setTypeName ("IndexedFaceSet");

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

	coordIndex ()    .addInterest (this, &IndexedFaceSet::set_coordIndex);
	texCoordIndex () .addInterest (this, &IndexedFaceSet::set_texCoordIndex);
	colorIndex ()    .addInterest (this, &IndexedFaceSet::set_colorIndex);
	normalIndex ()   .addInterest (this, &IndexedFaceSet::set_normalIndex);

	set_coordIndex ();
}

void
IndexedFaceSet::set_coordIndex ()
{
	auto _coord = x3d_cast <Coordinate*> (coord () .getValue ());
	
	numPolygons = 0;

	if (_coord)
	{
		int32_t numPoints = -1;

		if (coordIndex () .size ())
		{
			// Determine number of points and polygons.

			for (const auto & index : coordIndex ())
			{
				numPoints = std::max <int32_t> (numPoints, index);

				if (index < 0)
					++ numPolygons;
			}

			++ numPoints;

			if (coordIndex () .back () >= 0)
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
	auto _textureCoordinate = x3d_cast <TextureCoordinate*> (texCoord () .getValue ());
	
	if (_textureCoordinate)
	{
		// Fill up texCoordIndex if to small.
		for (size_t i = texCoordIndex () .size (); i < coordIndex () .size (); ++ i)
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
	auto _color     = x3d_cast <Color*> (color () .getValue ());
	auto _colorRGBA = x3d_cast <ColorRGBA*> (color () .getValue ());

	if (_color or _colorRGBA)
	{
		// Fill up colorIndex if to small.
		if (colorPerVertex ())
		{
			for (size_t i = colorIndex () .size (); i < coordIndex () .size (); ++ i)
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
		{
			_color -> resize (numColors);
		}
		else if (_colorRGBA)
		{
			_colorRGBA -> resize (numColors);
		}
	}
}

void
IndexedFaceSet::set_normalIndex ()
{
	auto _normal = x3d_cast <Normal*> (normal () .getValue ());

	if (_normal)
	{
		// Fill up normalIndex if to small.
		if (normalPerVertex ())
		{
			for (size_t i = normalIndex () .size (); i < coordIndex () .size (); ++ i)
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
	auto _coord = x3d_cast <Coordinate*> (coord () .getValue ());

	// Tesselate

	PolygonArray polygons;
	size_t       numTriangles = 0;

	tesselate (polygons, numTriangles);

	// Build arrays

	if (not _coord or not polygons .size ())
		return;

	size_t reserve = numTriangles * 3;

	// Color

	auto _color     = x3d_cast <Color*> (color () .getValue ());
	auto _colorRGBA = x3d_cast <ColorRGBA*> (color () .getValue ());

	if (_color)
		getColors () .reserve (reserve);

	else if (_colorRGBA)
		getColorsRGBA () .reserve (reserve);

	// TextureCoordinate

	auto _textureCoordinate          = x3d_cast <TextureCoordinate*> (texCoord () .getValue ());
	auto _textureCoordinateGenerator = x3d_cast <TextureCoordinateGenerator*> (texCoord () .getValue ());

	if (_textureCoordinate or not _textureCoordinateGenerator)
		getTexCoord () .reserve (reserve);

	// Normal

	auto _normal = x3d_cast <Normal*> (normal () .getValue ());

	getNormals () .reserve (reserve);

	// Vertices

	getVertices () .reserve (reserve);

	// Fill GeometryNode

	int face = 0;

	for (const auto polygon : polygons)
	{
		for (const auto triangle : polygon .triangles)
		{
			Vector3f    faceNormal;
			SFColor     faceColor;
			SFColorRGBA faceColorRGBA;

			if (not colorPerVertex ())
			{
				if (_color and colorIndex () [face] >= 0)
					faceColor = _color -> color () [colorIndex () [face]];

				else if (_colorRGBA and colorIndex () [face] >= 0)
					faceColorRGBA = _colorRGBA -> color () [colorIndex () [face]];
			}

			if (_normal)
			{
				if (not normalPerVertex () and normalIndex () [face] >= 0)
					faceNormal = _normal -> vector () [normalIndex () [face]];
			}

			for (const auto & i : triangle)
			{
				if (_color)
				{
					if (colorPerVertex () and colorIndex () [i] >= 0)
						getColors () .emplace_back (_color -> color () [colorIndex () [i]]);

					else
						getColors () .emplace_back (faceColor);
				}
				else if (_colorRGBA)
				{
					if (colorPerVertex () and colorIndex () [i] >= 0)
						getColorsRGBA () .emplace_back (_colorRGBA -> color () [colorIndex () [i]]);

					else
						getColorsRGBA () .emplace_back (faceColorRGBA);
				}

				if (_textureCoordinate)
				{
					if (texCoordIndex () [i] >= 0)
					{
						const auto & t = _textureCoordinate -> point () [texCoordIndex () [i]];
						getTexCoord () .emplace_back (t .getX (), t .getY (), 0);
					}
					else
						getTexCoord () .emplace_back (0, 0, 0);
				}

				if (_normal)
				{
					if (normalPerVertex () and normalIndex () [i] >= 0)
						getNormals () .emplace_back (_normal -> vector () [normalIndex () [i]]);

					else
						getNormals () .emplace_back (faceNormal);
				}

				getVertices () .emplace_back (_coord -> point () [coordIndex () [i]]);
			}
		}

		++ face;
	}

	// Autogenerate normal and texCoord if not specified

	if (not _textureCoordinate and not _textureCoordinateGenerator)
		buildTexCoord ();

	if (not _normal)
		buildNormals (polygons);

	addElement (getVertices () .size ());
	setTextureCoordinateGenerator (_textureCoordinateGenerator);
	setVertexMode (GL_TRIANGLES);
	setSolid (solid ());
}

void
IndexedFaceSet::tesselate (PolygonArray & polygons, size_t & numTriangles)
{
	auto _coord = x3d_cast <Coordinate*> (coord () .getValue ());

	if (not _coord)
		return;

	// Fill up coordIndex if there are no indices.
	if (coordIndex () .empty ())
	{
		for (size_t i = 0; i < _coord -> point () .size (); ++ i)
			coordIndex () .emplace_back (i);
	}

	if (coordIndex () .size ())
	{
		// Add -1 (polygon end marker) to coordIndex if not present.
		if (coordIndex () .back () >= 0)
			coordIndex () .emplace_back (-1);

		// Construct triangle array and determine the number of used points.
		size_t i = 0;

		Vertices vertices;

		for (const auto & index : coordIndex ())
		{
			if (index >= 0)
				// Add vertex.
				vertices .emplace_back (i);

			else
			{
				// Negativ index.

				if (vertices .size ())
				{
					// Closed polygon.
					if (vertices .front () == vertices .back ())
						vertices .pop_back ();

					if (vertices .size () == 3)
					{
						// Add polygon with one triangle.
						polygons .emplace_back (Polygon ({ vertices,
						                                   std::move (TriangleArray ({ std::move (Triangle ({ { vertices [0],
						                                                                                        vertices [1],
						                                                                                        vertices [2] } })) })) }));

						++ numTriangles;
					}
					else if (vertices .size () > 3)
					{
						// Tesselate polygons.
						polygons .emplace_back (Polygon ({ vertices, std::move (tesselate (vertices)) }));

						numTriangles += polygons .back () .triangles .size ();
					}

					vertices .clear ();
				}
			}

			++ i;
		}
	}
}

IndexedFaceSet::TriangleArray
IndexedFaceSet::tesselate (const Vertices & vertices)
{
	auto _coord = x3d_cast <Coordinate*> (coord () .getValue ());

	TriangleArray triangles;

	if (convex ())
	{
		for (size_t i = 1, size = vertices .size () - 1; i < size; ++ i)
		{
			// Add triangle to polygon.
			triangles .emplace_back (std::move (Triangle ({ { vertices [0],
			                                                  vertices [i],
			                                                  vertices [i + 1] } })));
		}
	}
	else
	{
		opengl::tesselator <size_t> tesselator;

		for (const auto & i : vertices)
			tesselator .add_vertex (_coord -> point () [coordIndex () [i]], i);

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
							triangles .emplace_back (std::move (Triangle ({ { std::get <0> (polygonElement [0] .data ()),
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
						triangles .emplace_back (std::move (Triangle ({ { std::get <0> (polygonElement [i % 2 ? i + 1 : i] .data ()),
						                                                  std::get <0> (polygonElement [i % 2 ? i : i + 1] .data ()),
						                                                  std::get <0> (polygonElement [i + 2] .data ()) } })));
					}

					break;
				}
				case GL_TRIANGLES:
				{
					for (size_t i = 0, size = polygonElement .size (); i < size; i += 3)
					{
						// Add triangle to polygon.
						triangles .emplace_back (std::move (Triangle ({ { std::get <0> (polygonElement [i] .data ()),
						                                                  std::get <0> (polygonElement [i + 1] .data ()),
						                                                  std::get <0> (polygonElement [i + 2] .data ()) } })));
					}

					break;
				}
				default:
					break;
			}
		}

		//		for (size_t i = 0; i < polygon .size (); ++ i)
		//			delete vertices [i];
	}

	return triangles;
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
//	auto _coord = x3d_cast <Coordinate*> (coord .getValue ());
//
//	for (const auto & polygon : polygons)
//	{
//		for (const auto & triangle : polygon .triangles)
//		{
//			for (const auto & i : triangle)
//			{
//				const Vector3f & point = _coord -> point [coordIndex [i]];
//
//				getTexCoord () .emplace_back ((point [Sindex] - min [Sindex]) / Ssize,
//				                              (point [Tindex] - min [Tindex]) / Ssize);
//			}
//		}
//	}
//}

void
IndexedFaceSet::buildNormals (const PolygonArray & polygons)
{
	std::vector <Vector3f> normals;

	normals .reserve (coordIndex () .size ());

	NormalIndex normalIndex;

	auto _coord = x3d_cast <Coordinate*> (coord () .getValue ());

	for (const auto & polygon : polygons)
	{
		// Determine polygon normal.
		Vector3f normal;

		for (const auto & triangle : polygon .triangles)
		{
			normal += math::normal <float> (_coord -> point () [coordIndex () [triangle [0]]],
			                                _coord -> point () [coordIndex () [triangle [1]]],
			                                _coord -> point () [coordIndex () [triangle [2]]]);
		}

		// Add a normal index for each point.
		for (size_t i = 0, size = polygon .vertices .size (); i < size; ++ i)
			normalIndex [coordIndex () [polygon .vertices [i]]] .emplace_back (normals .size () + i);

		// Add this normal for each vertex.
		normals .resize (normals .size () + polygon .vertices .size (), normalize (normal));
		normals .emplace_back ();
	}

	refineNormals (normalIndex, normals, creaseAngle ());

	for (const auto & polygon : polygons)
	{
		for (const auto & triangle : polygon .triangles)
		{
			for (const auto & i : triangle)
			{
				getNormals () .emplace_back (normals [i]);
			}
		}
	}

}

void
IndexedFaceSet::dispose ()
{
	X3DComposedGeometryNode::dispose ();
}

} // X3D

} // titania
