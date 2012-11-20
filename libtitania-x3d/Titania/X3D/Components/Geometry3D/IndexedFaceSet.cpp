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

#include "IndexedFaceSet.h"

#include "../../Execution/X3DExecutionContext.h"
#include <iostream>

namespace titania {
namespace X3D {

IndexedFaceSet::IndexedFaceSet (X3DExecutionContext* const executionContext) :
	           X3DBasicNode (executionContext -> getBrowser (), executionContext), 
	X3DComposedGeometryNode (),                                                    
	         set_colorIndex (),                                                    // MFInt32 [in] set_colorIndex
	         set_coordIndex (),                                                    // MFInt32 [in] set_coordIndex
	        set_normalIndex (),                                                    // MFInt32 [in] set_normalIndex
	      set_texCoordIndex (),                                                    // MFInt32 [in] set_texCoordIndex
	             colorIndex (),                                                    // MFInt32 [ ]  colorIndex         [ ]          [0,∞) or -1
	                 convex (true),                                                // SFBool  [ ]  convex             TRUE
	             coordIndex (),                                                    // MFInt32 [ ]  coordIndex         [ ]          [0,∞) or -1
	            normalIndex (),                                                    // MFInt32 [ ]  normalIndex        [ ]          [0,∞) or -1
	          texCoordIndex (),                                                    // MFInt32 [ ]  texCoordIndex      [ ]          [-1,∞)
	                   tess (0),                                                   
	               polygons ()                                                     
{
	setComponent ("Geometry3D");
	setTypeName ("IndexedFaceSet");

	appendField (inputOutput,    "metadata",          metadata);
	appendField (inputOnly,      "set_coordIndex",    set_coordIndex);
	appendField (inputOnly,      "set_texCoordIndex", set_texCoordIndex);
	appendField (inputOnly,      "set_colorIndex",    set_colorIndex);
	appendField (inputOnly,      "set_normalIndex",   set_normalIndex);
	appendField (initializeOnly, "solid",             solid);
	appendField (initializeOnly, "ccw",               ccw);
	appendField (initializeOnly, "convex",            convex);
	appendField (initializeOnly, "creaseAngle",       creaseAngle);
	appendField (initializeOnly, "colorPerVertex",    colorPerVertex);
	appendField (initializeOnly, "normalPerVertex",   normalPerVertex);
	appendField (initializeOnly, "coordIndex",        coordIndex);
	appendField (initializeOnly, "texCoordIndex",     texCoordIndex);
	appendField (initializeOnly, "colorIndex",        colorIndex);
	appendField (initializeOnly, "normalIndex",       normalIndex);
	appendField (inputOutput,    "attrib",            attrib);
	appendField (inputOutput,    "coord",             coord);
	appendField (inputOutput,    "texCoord",          texCoord);
	appendField (inputOutput,    "color",             color);
	appendField (inputOutput,    "normal",            normal);
	appendField (inputOutput,    "fogCoord",          fogCoord);
}

X3DBasicNode*
IndexedFaceSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedFaceSet (executionContext);
}

void
IndexedFaceSet::initialize ()
{
	X3DComposedGeometryNode::initialize ();

	tess = gluNewTess ();

	if (tess)
	{
		gluTessProperty (tess, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
		gluTessCallback (tess, GLU_TESS_BEGIN_DATA, (_GLUfuncptr) & IndexedFaceSet::tessBeginData);
		gluTessCallback (tess, GLU_TESS_VERTEX_DATA, (_GLUfuncptr) & IndexedFaceSet::tessVertexData);

		//gluTessCallback(tess, GLU_TESS_COMBINE_DATA, (_GLUfuncptr)&IndexedFaceSet::tessCombineData);
		gluTessCallback (tess, GLU_TESS_END_DATA, (_GLUfuncptr) & IndexedFaceSet::tessEndData);
		gluTessCallback (tess, GLU_TESS_ERROR, (_GLUfuncptr) & IndexedFaceSet::tessError);
	}

	set_coordIndex    .addInterest (this, &IndexedFaceSet::_set_coordIndex);
	set_texCoordIndex .addInterest (this, &IndexedFaceSet::_set_texCoordIndex);
	set_colorIndex    .addInterest (this, &IndexedFaceSet::_set_colorIndex);
	set_normalIndex   .addInterest (this, &IndexedFaceSet::_set_normalIndex);

	_set_coordIndex (coordIndex);
}

void
IndexedFaceSet::_set_coordIndex (const MFInt32::value_type & value)
{
	if (coordIndex not_eq value)
		coordIndex = value;

	polygons .clear ();

	if (coordIndex .size ())
	{
		// Construct polygon area and determine the number of used points.
		size_t i          = 0;
		int32_t numPoints = -1;

		// Add a polygon if it look like we have one.
		if (coordIndex [0] >= 0)
			polygons .emplace_back ();

		for (const auto & index : coordIndex)
		{
			numPoints = std::max <int32_t> (numPoints, index);

			if (index >= 0)
				// Add vertex.
				polygons .back () .emplace_back (i);

			else
			{
				// Negativ index.

				if (polygons .back () .size ())
				{
					// Closed polygon.
					if (polygons .back () .front () == polygons .back () .back ())
						polygons .back () .pop_back ();

					if (polygons .back () .size () < 3)
						// Discard last polygon if number of vertices less then 3.
						polygons .back () .clear ();
					else
						// Add new polygon.
						polygons .emplace_back ();
				}
			}
			
			++ i;
		}

		++ numPoints;

		// Discard last polygon if number of vertices less then 3.
		if (polygons .back () .size () < 3)
			polygons .pop_back ();

		if (polygons .size ())
		{
			SFNode <Coordinate> _coord = coord;

			if (_coord)
			{
				// Resize coord .point if to small
				if (_coord -> point .size () < (size_t) numPoints)
					_coord -> point .resize (numPoints);

				// Calculate bbox for texture coordinate calculation.
				bbox = createBBox ();

				_set_texCoordIndex (texCoordIndex);
				_set_colorIndex    (colorIndex);
				_set_normalIndex   (normalIndex);
			}
		}
	}
}

void
IndexedFaceSet::_set_texCoordIndex (const MFInt32::value_type & value)
{
	if (texCoordIndex not_eq value)
		texCoordIndex = value;

	SFNode <TextureCoordinate> _textureCoordinate = texCoord;

	if (_textureCoordinate)
	{
		// Fill up texCoordIndex if to small.
		for (size_t i = texCoordIndex .size (); i < coordIndex .size (); ++ i)
		{
			texCoordIndex .push_back (coordIndex [i]);
		}

		// Determine number of used texCoords.

		int numTexCoord = -1;

		for (const auto & index : texCoordIndex)
		{
			numTexCoord = std::max <int32_t> (numTexCoord, index);
		}

		++ numTexCoord;

		// Resize textureCoordinate .point if to small.
		if (_textureCoordinate -> point .size () < (size_t) numTexCoord)
			_textureCoordinate -> point .resize (numTexCoord);
	}
}

void
IndexedFaceSet::_set_colorIndex (const MFInt32::value_type & value)
{
	if (colorIndex not_eq value)
		colorIndex = value;

	SFNode <Color>     _color     = color;
	SFNode <ColorRGBA> _colorRGBA = color;

	if (_color or _colorRGBA)
	{
		// Fill up colorIndex if to small.
		if (colorPerVertex)
		{
			for (size_t i = colorIndex .size (); i < coordIndex .size (); ++ i)
			{
				colorIndex .push_back (coordIndex [i]);
			}
		}
		else
		{
			for (size_t i = colorIndex .size (); i < polygons .size (); ++ i)
			{
				colorIndex .push_back (i);
			}
		}

		// Determine number of used colors.

		int numColors = -1;

		for (const auto & index : colorIndex)
		{
			numColors = std::max <int32_t> (numColors, index);
		}

		++ numColors;

		// Resize color .color if to small.
		if (_color)
		{
			if (_color -> color .size () < (size_t) numColors)
				_color -> color .resize (numColors);
		}
		else if (_colorRGBA)
		{
			if (_colorRGBA -> color .size () < (size_t) numColors)
				_colorRGBA -> color .resize (numColors);
		}
	}
}

void
IndexedFaceSet::_set_normalIndex (const MFInt32::value_type & value)
{
	if (normalIndex not_eq value)
		normalIndex = value;

	SFNode <Normal> _normal = normal;

	if (_normal)
	{
		// Fill up normalIndex if to small.
		if (normalPerVertex)
		{
			for (size_t i = normalIndex .size (); i < coordIndex .size (); ++ i)
			{
				normalIndex .push_back (coordIndex [i]);
			}
		}
		else
		{
			for (size_t i = normalIndex .size (); i < polygons .size (); ++ i)
			{
				normalIndex .push_back (i);
			}
		}

		// Determine number of used normals.

		int numNormals = -1;

		for (const auto & index : normalIndex)
		{
			numNormals = std::max <int32_t> (numNormals, index);
		}

		++ numNormals;

		// Resize normal .vector if to small.
		if (_normal -> vector .size () < (size_t) numNormals)
			_normal -> vector .resize (numNormals);
	}
}

Box3f
IndexedFaceSet::createBBox ()
{
	SFNode <Coordinate> _coord = coord;

	if (polygons .size ())
	{
		Vector3f min = _coord -> point [coordIndex [polygons .front () [0]]];
		Vector3f max = min;

		for (const auto & polygon : polygons)
		{
			for (const auto & i : polygon)
			{
				const auto & vertex = _coord -> point [coordIndex [i]];

				min = math::min <float> (min, vertex);
				max = math::max <float> (max, vertex);
			}
		}

		Vector3f size   = max - min;
		Vector3f center = min + size * 0.5f;

		return Box3f (size, center);
	}

	return Box3f ();
}

std::vector <Vector2f>
IndexedFaceSet::createTexCoord ()
{
	std::vector <Vector2f> texCoord;
	texCoord .reserve (coordIndex .size ());

	Vector3f min = bbox .center () - bbox .size () * 0.5f;

	float Xsize = bbox .size () .x ();
	float Ysize = bbox .size () .y ();
	float Zsize = bbox .size () .z ();

	float Ssize;
	int   Sindex, Tindex;

	if ((Xsize >= Ysize)and (Xsize >= Zsize))
	{
		// X size largest
		Ssize = Xsize; Sindex = 0;

		if (Ysize >= Zsize)
			Tindex = 1;
		else
			Tindex = 2;
	}
	else if ((Ysize >= Xsize)and (Ysize >= Zsize))
	{
		// Y size largest
		Ssize = Ysize; Sindex = 1;

		if (Xsize >= Zsize)
			Tindex = 0;
		else
			Tindex = 2;
	}
	else
	{
		// Z is the largest
		Ssize = Zsize; Sindex = 2;

		if (Xsize >= Ysize)
			Tindex = 0;
		else
			Tindex = 1;
	}

	SFNode <Coordinate> _coord = coord;

	for (const auto & polygon : polygons)
	{
		for (const auto & i : polygon)
		{
			const Vector3f & point = _coord -> point [coordIndex [i]];

			texCoord .emplace_back ((point [Sindex] - min [Sindex]) / Ssize,
			                        (point [Tindex] - min [Tindex]) / Ssize);
		}
		
		// Add one extra texCoord for -1.
		texCoord .emplace_back ();
	}

	return texCoord;
}

inline
Vector3f
vertexNormal (const Vector3f & v1, const Vector3f & v2, const Vector3f & v3)
{
	return normalize (cross (v3 - v2, v1 - v2));
}

std::vector <Vector3f>
IndexedFaceSet::createNormals ()
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	SFNode <Coordinate> _coord = coord;

	for (const auto & polygon : polygons)
	{
		Vector3f normal; // faceNormal

		if (polygon .size () > 3)
		{
			if (convex)
			{
				// Calculate face normal for this polygon.

				for (size_t i = 1; i < polygon .size () - 1; ++ i)
				{
					normal += vertexNormal (_coord -> point [coordIndex [polygon [0]]],
					                        _coord -> point [coordIndex [polygon [i]]],
					                        _coord -> point [coordIndex [polygon [i + 1]]]);
				}

				// Add normal index to the normalIndex for each vertex.
				for (size_t i = 0; i < polygon .size (); ++ i)
					normalIndex [coordIndex [polygon [i]]] .emplace_back (normals .size () + i);
			}
			else if (tess)
			{
				Polygon tesselatedPolygon;

				std::vector <Vertex*> vertices;
				vertices .reserve (polygon .size () * 3);

				for (const auto & i : polygon)
					vertices .emplace_back (new Vertex (_coord -> point [coordIndex [i]], i));

				gluTessBeginPolygon (tess, &tesselatedPolygon);
				gluTessBeginContour (tess);

				for (size_t i = 0; i < polygon .size (); ++ i)
					gluTessVertex (tess, vertices [i] -> vertex .data (), vertices [i]);

				//gluTessEndContour(tess);
				gluEndPolygon (tess);

				for (const auto & polygonElement : tesselatedPolygon)
				{
					switch (polygonElement .type)
					{
						case GL_TRIANGLE_FAN:
						{
							for (size_t i = 1; i < polygonElement .vertices .size () - 1; ++ i)
							{
								normal += vertexNormal (_coord -> point [coordIndex [polygonElement .vertices [0] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i + 1] -> i]]);
							}

							break;
						}
						case GL_TRIANGLE_STRIP:
						{
							for (size_t i = 0; i < polygonElement .vertices .size () - 2; ++ i)
							{
								normal += vertexNormal (_coord -> point [coordIndex [polygonElement .vertices [i % 2 ? i + 1 : i] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i % 2 ? i : i + 1] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i + 2] -> i]]);
							}

							break;
						}
						case GL_TRIANGLES:
						{
							for (size_t i = 0; i < polygonElement .vertices .size (); i += 3)
							{
								normal += vertexNormal (_coord -> point [coordIndex [polygonElement .vertices [i] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i + 1] -> i]],
								                        _coord -> point [coordIndex [polygonElement .vertices [i + 2] -> i]]);
							}

							break;
						}
						default:
							break;
					}
				}

				for (size_t i = 0; i < polygon .size (); ++ i)
					delete vertices [i];

				for (size_t i = 0; i < polygon .size (); ++ i)
					normalIndex [coordIndex [polygon [i]]] .emplace_back (normals .size () + i);
			}
		}
		else
		{
			normal = vertexNormal (_coord -> point [coordIndex [polygon [0]]],
			                       _coord -> point [coordIndex [polygon [1]]],
			                       _coord -> point [coordIndex [polygon [2]]]);

			for (int i = 0; i < 3; ++ i)
				normalIndex [coordIndex [polygon [i]]] .emplace_back (normals .size () + i);
		}

		// Add this normal for each vertex and one extra normal for -1.
		normals .resize (normals .size () + polygon .size () + 1, normalize (normal));
	}

	refineNormals (normalIndex, normals);

	return normals;
}

void
IndexedFaceSet::build ()
{
	X3DComposedGeometryNode::build ();

	SFNode <Coordinate> _coord = coord;

	if (not _coord or not polygons .size ())
		return;

	// TextureCoordinate

	std::vector <Vector2f> _texCoord;

	SFNode <TextureCoordinate>          _textureCoordinate          = texCoord;
	SFNode <TextureCoordinateGenerator> _textureCoordinateGenerator = texCoord;

	if (not _textureCoordinate and not _textureCoordinateGenerator)
		_texCoord = createTexCoord ();

	// Color

	SFNode <Color>     _color     = color;
	SFNode <ColorRGBA> _colorRGBA = color;

	// Normal

	std::vector <Vector3f> _normals;

	SFNode <Normal> _normal = normal;

	if (not _normal)
		_normals = createNormals ();

	// Fill GeometryNode

	int face = 0;

	for (const auto polygon : polygons)
	{
		Vector3f    faceNormal;
		SFColor     faceColor;
		SFColorRGBA faceColorRGBA;

		if (_normal)
		{
			if (not normalPerVertex and normalIndex [face] >= 0)
				faceNormal = _normal -> vector [normalIndex [face]];
		}

		if (not colorPerVertex)
		{
			if (_color and colorIndex [face] >= 0)
				faceColor = _color -> color [colorIndex [face]];

			else if (_colorRGBA and colorIndex [face] >= 0)
				faceColorRGBA = _colorRGBA -> color [colorIndex [face]];
		}

		if (polygon .size () > 3)
		{
			if (convex)
			{
				for (size_t i = 1; i < polygon .size () - 1; ++ i)
				{
					setPoint (polygon [0],
					          _textureCoordinate,
					          _textureCoordinateGenerator,
					          _texCoord,
					          _normal,
					          faceNormal,
					          _normals,
					          _color,
					          _colorRGBA,
					          faceColor,
					          faceColorRGBA,
					          _coord);

					setPoint (polygon [i],
					          _textureCoordinate,
					          _textureCoordinateGenerator,
					          _texCoord,
					          _normal,
					          faceNormal,
					          _normals,
					          _color,
					          _colorRGBA,
					          faceColor,
					          faceColorRGBA,
					          _coord);

					setPoint (polygon [i + 1],
					          _textureCoordinate,
					          _textureCoordinateGenerator,
					          _texCoord,
					          _normal,
					          faceNormal,
					          _normals,
					          _color,
					          _colorRGBA,
					          faceColor,
					          faceColorRGBA,
					          _coord);

				}
			}
			else if (tess)
			{
				Polygon tesselatedPolygon;

				std::vector <Vertex*> vertices;
				vertices .reserve (polygon .size () * 3);

				for (const auto & i : polygon)
					vertices .emplace_back (new Vertex (_coord -> point [coordIndex [i]], i));

				gluTessBeginPolygon (tess, &tesselatedPolygon);
				gluTessBeginContour (tess);

				for (size_t i = 0; i < polygon .size (); ++ i)
					gluTessVertex (tess, vertices [i] -> vertex .data (), vertices [i]);

				//gluTessEndContour(tess);
				gluEndPolygon (tess);

				Polygon::const_iterator polygonElement;

				for (const auto & polygonElement : tesselatedPolygon)
				{
					switch (polygonElement .type)
					{
						case GL_TRIANGLE_FAN :
							{
								for (size_t i = 1; i < polygonElement .vertices .size () - 1; ++ i)
								{
									setPoint (polygonElement .vertices [0] -> i,
									          _textureCoordinate,
									          _textureCoordinateGenerator,
									          _texCoord,
									          _normal,
									          faceNormal,
									          _normals,
									          _color,
									          _colorRGBA,
									          faceColor,
									          faceColorRGBA,
									          _coord);

									setPoint (polygonElement .vertices [i] -> i,
									          _textureCoordinate,
									          _textureCoordinateGenerator,
									          _texCoord,
									          _normal,
									          faceNormal,
									          _normals,
									          _color,
									          _colorRGBA,
									          faceColor,
									          faceColorRGBA,
									          _coord);

									setPoint (polygonElement .vertices [i + 1] -> i,
									          _textureCoordinate,
									          _textureCoordinateGenerator,
									          _texCoord,
									          _normal,
									          faceNormal,
									          _normals,
									          _color,
									          _colorRGBA,
									          faceColor,
									          faceColorRGBA,
									          _coord);

								}

								break;
							}
						case GL_TRIANGLE_STRIP:
						{
							for (size_t i = 0; i < polygonElement .vertices .size () - 2; ++ i)
							{
								setPoint (polygonElement .vertices [i % 2 ? i + 1 : i] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

								setPoint (polygonElement .vertices [i % 2 ? i : i + 1] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

								setPoint (polygonElement .vertices [i + 2] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

							}

							break;
						}
						case GL_TRIANGLES:
						{
							for (size_t i = 0; i < polygonElement .vertices .size (); i += 3)
							{
								setPoint (polygonElement .vertices [i] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

								setPoint (polygonElement .vertices [i + 1] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

								setPoint (polygonElement .vertices [i + 2] -> i,
								          _textureCoordinate,
								          _textureCoordinateGenerator,
								          _texCoord,
								          _normal,
								          faceNormal,
								          _normals,
								          _color,
								          _colorRGBA,
								          faceColor,
								          faceColorRGBA,
								          _coord);

							}

							break;
						}
						default:
							break;
					}
				}

				for (size_t i = 0; i < polygon .size (); ++ i)
					delete vertices [i];
			}
		}
		else
		{
			for (const auto & i : polygon)
			{
				setPoint (i,
				          _textureCoordinate,
				          _textureCoordinateGenerator,
				          _texCoord,
				          _normal,
				          faceNormal,
				          _normals,
				          _color,
				          _colorRGBA,
				          faceColor,
				          faceColorRGBA,
				          _coord);
			}
		}
	}

	setTextureCoordinateGenerator (*_textureCoordinateGenerator);
	setVertexMode (GL_TRIANGLES);
}

void
IndexedFaceSet::setPoint (const int32_t i,
                          const SFNode <TextureCoordinate> & _textureCoordinate,
                          const SFNode <TextureCoordinateGenerator> & _textureCoordinateGenerator,
                          const std::vector <Vector2f> & _texCoord,
                          const SFNode <Normal> & _normal,
                          const Vector3f & faceNormal,
                          const std::vector <Vector3f> & _normals,
                          const SFNode <Color> & _color,
                          const SFNode <ColorRGBA> & _colorRGBA,
                          const SFColor & faceColor,
                          const SFColorRGBA faceColorRGBA,
                          const SFNode <Coordinate> & _coord)
{
	if (_color)
	{
		if (colorPerVertex and colorIndex [i] >= 0)
			getColors () .emplace_back (_color -> color [colorIndex [i]]);

		else
			getColors () .emplace_back (faceColor);
	}
	else if (_colorRGBA)
	{
		float r = 0, g = 0, b = 0, a = 0;

		if (colorPerVertex and colorIndex [i] >= 0)
			_colorRGBA -> color [colorIndex [i]] .getValue (r, g, b, a);

		else
			faceColorRGBA .getValue (r, g, b, a);

		getColorsRGBA () .emplace_back (r, g, b, 1 - a);
	}

	if (_textureCoordinate)
	{
		if (texCoordIndex [i] >= 0)
			getTexCoord () .emplace_back (_textureCoordinate -> point [texCoordIndex [i]]);

		else
			getTexCoord () .emplace_back (0, 0);
	}
	else if (_textureCoordinateGenerator)
	{ }
	else
	{
		getTexCoord () .emplace_back (_texCoord [i]);
	}

	if (_normal)
	{
		if (normalPerVertex and normalIndex [i] >= 0)
			getNormals () .emplace_back (_normal -> vector [normalIndex [i]]);

		else
			getNormals () .emplace_back (faceNormal);
	}
	else
	{
		getNormals () .emplace_back (_normals [i]);
	}

	getVertices () .emplace_back (_coord -> point [coordIndex [i]]);
}

void
IndexedFaceSet::tessBeginData (GLenum type, void* polygon_data)
{
	Polygon* polygon = (Polygon*) polygon_data;

	polygon -> push_back (PolygonElement (type));
}

void
IndexedFaceSet::tessVertexData (void* vertex_data, void* polygon_data)
{
	Polygon* polygon = (Polygon*) polygon_data;
	Vertex*  vertex  = (Vertex*) vertex_data;

	polygon -> back () .vertices .push_back (vertex);
}

void
IndexedFaceSet::tessCombineData (GLdouble coords [3], void* vertex_data [4],
                                 GLfloat weight [4], void** outData,
                                 void* polygon_data)
{
	// not used yet
}

void
IndexedFaceSet::tessEndData (void* polygon_data)
{ }

void
IndexedFaceSet::tessError (GLenum err_no)
{
	std::clog << "Warning: in function " << __func__ << " '" << (char*) gluErrorString (err_no) << "'." << std::endl;
}

void
IndexedFaceSet::dispose ()
{
	if (tess)
		gluDeleteTess (tess);

	X3DComposedGeometryNode::dispose ();
}

} // X3D
} // titania
