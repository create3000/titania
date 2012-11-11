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
	              numPoints (0),                                                   
	               numFaces (0),                                                   
	            numTexCoord (0),                                                   
	              numColors (0),                                                   
	             numNormals (0)                                                    
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

	numPoints = -1;
	numFaces  = -1;

	for (const auto & index : coordIndex)
	{
		numPoints = std::max <size_t> (numPoints, index);

		if (index < 0)
			++ numFaces;
	}

	++ numPoints;
	++ numFaces;

	if (numPoints)
	{
		if (coordIndex .back () >= 0)
			++ numFaces;

		SFNode <Coordinate> _coord = coord;

		if (not _coord or not _coord -> point .size ())
			return;

		if (_coord -> point .size () < (size_t) numPoints)
			_coord -> point .resize (numPoints);

		bbox = createBBox ();

		_set_texCoordIndex (texCoordIndex);
		_set_colorIndex    (colorIndex);
		_set_normalIndex   (normalIndex);
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
		for (size_t i = texCoordIndex .size (); i < coordIndex .size (); ++ i)
		{
			texCoordIndex .push_back (coordIndex .at (i));
		}

		numTexCoord = -1;

		for (const auto & index : texCoordIndex)
		{
			numTexCoord = std::max <size_t> (numTexCoord, index);
		}

		++ numTexCoord;
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
		if (colorPerVertex)
		{
			for (size_t i = colorIndex .size (); i < coordIndex .size (); ++ i)
			{
				colorIndex .push_back (coordIndex .at (i));
			}
		}
		else
		{
			for (size_t i = colorIndex .size (); i < (size_t) numFaces; ++ i)
			{
				colorIndex .push_back (i);
			}
		}

		numColors = -1;

		for (const auto & index : colorIndex)
		{
			numColors = std::max <size_t> (numColors, index);
		}

		++ numColors;
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
		if (normalPerVertex)
		{
			for (size_t i = normalIndex .size (); i < coordIndex .size (); ++ i)
			{
				normalIndex .push_back (coordIndex .at (i));
			}
		}
		else
		{
			for (size_t i = normalIndex .size (); i < (size_t) numFaces; ++ i)
			{
				normalIndex .push_back (i);
			}
		}

		numNormals = -1;

		for (const auto & index : normalIndex)
		{
			numNormals = std::max <size_t> (numNormals, index);
		}

		++ numNormals;
	}
}

Box3f
IndexedFaceSet::createBBox ()
{
	SFNode <Coordinate> _coord = coord;

	if (not _coord or not _coord -> point .size ())
		return Box3f ();

	Vector3f min = _coord -> point .at (0);
	Vector3f max = _coord -> point .at (0);

	for (const auto & point : _coord -> point)
	{
		min = math::min <float> (min, point);
		max = math::max <float> (max, point);
	}

	Vector3f size   = max - min;
	Vector3f center = min + size * 0.5f;

	return Box3f (size, center);
}

std::vector <Vector2f>
IndexedFaceSet::getTexCoord ()
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

	for (const auto & index : coordIndex)
	{
		if (index >= 0)
		{
			const Vector3f & point = _coord -> point .at (index);

			texCoord .push_back (Vector2f ((point [Sindex] - min [Sindex]) / Ssize,
			                               (point [Tindex] - min [Tindex]) / Ssize));
		}
		else
			texCoord .push_back (Vector2f ());
	}

	return texCoord;
}

std::vector <Vector3f>
IndexedFaceSet::getNormals ()
{
	std::vector <Vector3f> normals;
	normals .reserve (coordIndex .size ());

	NormalIndex normalIndex;

	SFNode <Coordinate> _coord = coord;

	for (MFInt32::const_iterator index = coordIndex .begin (); index not_eq coordIndex .end (); ++ index)
	{
		int numIndices = 0;

		for (MFInt32::const_iterator iter = index;
		     *iter >= 0 and iter not_eq coordIndex .end ();
		     ++ iter, ++ numIndices)
			;

		Vector3f normal;

		if (numIndices == 3)
		{
			const Vector3f & p1 = _coord -> point .at (*index);
			const Vector3f & p2 = _coord -> point .at (*(index + 1));
			const Vector3f & p3 = _coord -> point .at (*(index + 2));

			normal = cross (p3 - p2, p1 - p2);

			for (int i = 0; i < 3; ++ i)
				normalIndex [*(index + i)] .push_back (normals .size () + i);
		}
		else if (numIndices > 3)
		{
			if (convex)
			{
				int end = *index == *(index + (numIndices - 1)) ? 2 : 1;

				for (int _i = 1; _i < numIndices - end; ++ _i)
				{
					const Vector3f & p1 = _coord -> point .at (*index);
					const Vector3f & p2 = _coord -> point .at (*(index + _i));
					const Vector3f & p3 = _coord -> point .at (*(index + _i + 1));

					normal += normalize (cross (p3 - p2, p1 - p2));
				}

				//const Vector3f& p1 = _coord -> point .at((index + (numIndices - end - 1)) -> getValue()) .getValue();
				//const Vector3f& p2 = _coord -> point .at(index -> getValue()) .getValue();
				//const Vector3f& p3 = _coord -> point .at((index + 1) -> getValue()) .getValue();

				//normal += p3 .subtract(p2) .cross(p1 .subtract(p2)) .normalize();

				for (int i = 0; i < numIndices - end + 1; ++ i)
					normalIndex [*(index + i)] .push_back (normals .size () + i);
			}
			else if (tess)
			{
				int end = *index == *(index + (numIndices - 1)) ? 1 : 0;

				Polygon polygon;

				std::vector <Vertex*> vertices;
				vertices .reserve ((numIndices - end) * 3);

				for (int _i = 0; _i < numIndices - end; ++ _i)
					vertices .push_back (new Vertex (&_coord -> point, index + _i, 0));

				gluTessBeginPolygon (tess, &polygon);
				gluTessBeginContour (tess);

				for (int _i = 0; _i < numIndices - end; ++ _i)
					gluTessVertex (tess, &vertices [_i] -> location [0], vertices [_i]);

				//gluTessEndContour(tess);
				gluEndPolygon (tess);

				Polygon::const_iterator polygonElement;

				for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
				{
					switch (polygonElement -> type)
					{
						case GL_TRIANGLE_FAN:

							for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
							{
								const Vector3f & p1 = _coord -> point .at (*polygonElement -> vertices [0] -> index);
								const Vector3f & p2 = _coord -> point .at (*polygonElement -> vertices [_i] -> index);
								const Vector3f & p3 = _coord -> point .at (*polygonElement -> vertices [_i + 1] -> index);

								normal += normalize (cross (p3 - p2, p1 - p2));
							}

							break;
						case GL_TRIANGLE_STRIP:

							for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
							{
								const Vector3f & p1 = _coord -> point .at (*polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> index);
								const Vector3f & p2 = _coord -> point .at (*polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> index);
								const Vector3f & p3 = _coord -> point .at (*polygonElement -> vertices [_i + 2] -> index);

								normal += normalize (cross (p3 - p2, p1 - p2));
							}

							break;
						case GL_TRIANGLES:

							for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
							{
								const Vector3f & p1 = _coord -> point .at (*polygonElement -> vertices [_i] -> index);
								const Vector3f & p2 = _coord -> point .at (*polygonElement -> vertices [_i + 1] -> index);
								const Vector3f & p3 = _coord -> point .at (*polygonElement -> vertices [_i + 2] -> index);

								normal += normalize (cross (p3 - p2, p1 - p2));
							}

							break;
						default:
							break;
					}
				}

				for (int _i = 0; _i < numIndices - end; ++ _i)
					delete vertices [_i];

				for (int i = 0; i < numIndices - end; ++ i)
					normalIndex [*(index + i)] .push_back (normals .size () + i);
			}
		}

		normals .resize (normals .size () + numIndices + 1, normalize (normal));

		index += numIndices;

		if (index == coordIndex .end ())
			break;
	}

	refineNormals (normalIndex, normals);

	return normals;
}

void
IndexedFaceSet::build ()
{
	X3DComposedGeometryNode::build ();

	SFNode <Coordinate> _coord = coord;

	if (not _coord or not _coord -> point .size ())
		return;

	SFNode <Color>     _color     = color;
	SFNode <ColorRGBA> _colorRGBA = color;

	if (_color or _colorRGBA)
	{
		if (_color)
		{
			if (_color -> color .size () < (size_t) numColors)
				_color -> color .resize (numColors);

			setGLNumColors (3);
		}
		else if (_colorRGBA)
		{
			if (_colorRGBA -> color .size () < (size_t) numColors)
				_colorRGBA -> color .resize (numColors);

			setGLNumColors (4);
		}
	}

	std::vector <Vector2f> _texCoord;

	SFNode <TextureCoordinate>          _textureCoordinate          = texCoord;
	SFNode <TextureCoordinateGenerator> _textureCoordinateGenerator = texCoord;

	if (_textureCoordinate)
	{
		if (_textureCoordinate -> point .size () < (size_t) numTexCoord)
			_textureCoordinate -> point .resize (numTexCoord);

	}
	else if (_textureCoordinateGenerator)
	{ }
	else
		_texCoord = getTexCoord ();

	std::vector <Vector3f> _normals;

	SFNode <Normal> _normal = normal;

	if (_normal)
	{
		if (_normal -> vector .size () < (size_t) numNormals)
			_normal -> vector .resize (numNormals);
	}
	else
	{
		_normals = getNormals ();
	}

	GLsizei glIndices = 0;

	MFInt32::const_iterator index;
	int                     face, i;

	for (index = coordIndex .begin (), face = 0, i = 0; index not_eq coordIndex .end (); ++ index, ++ face, ++ i)
	{
		int numIndices = 0;

		for (MFInt32::const_iterator iter = index;
		     *iter >= 0 and iter not_eq coordIndex .end ();
		     ++ iter, ++ numIndices)
			;

		if (numIndices > 2)
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

			if (numIndices == 3)
			{
				for (int _i = 0; _i < numIndices; ++ _i, ++ index, ++ i)
				{
					setPoint (index,
					          i,
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

					++ glIndices;
				}
			}
			else if (numIndices > 3)
			{
				if (convex)
				{
					int end = *index == *(index + (numIndices - 1)) ? 2 : 1;

					for (int _i = 1; _i < numIndices - end; ++ _i)
					{
						setPoint (index,
						          i,
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

						++ glIndices;

						setPoint (index + _i,
						          i + _i,
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

						++ glIndices;

						setPoint (index + _i + 1,
						          i + _i + 1,
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

						++ glIndices;
					}
				}
				else if (tess)
				{
					int end = *index == *(index + (numIndices - 1)) ? 1 : 0;

					Polygon polygon;

					std::vector <Vertex*> vertices;
					vertices .reserve ((numIndices - end) * 3);

					for (int _i = 0; _i < numIndices - end; ++ _i)
						vertices .push_back (new Vertex (&_coord -> point, index + _i, i + _i));

					gluTessBeginPolygon (tess, &polygon);
					gluTessBeginContour (tess);

					for (int _i = 0; _i < numIndices - end; ++ _i)
						gluTessVertex (tess, &vertices [_i] -> location [0], vertices [_i]);

					//gluTessEndContour(tess);
					gluEndPolygon (tess);

					Polygon::const_iterator polygonElement;

					for (polygonElement = polygon .begin (); polygonElement not_eq polygon .end (); ++ polygonElement)
					{
						switch (polygonElement -> type)
						{
							case GL_TRIANGLE_FAN:

								for (size_t _i = 1; _i < polygonElement -> vertices .size () - 1; ++ _i)
								{
									setPoint (polygonElement -> vertices [0] -> index,
									          polygonElement -> vertices [0] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i] -> index,
									          polygonElement -> vertices [_i] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i + 1] -> index,
									          polygonElement -> vertices [_i + 1] -> i,
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

									++ glIndices;
								}

								break;
							case GL_TRIANGLE_STRIP:

								for (size_t _i = 0; _i < polygonElement -> vertices .size () - 2; ++ _i)
								{
									setPoint (polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> index,
									          polygonElement -> vertices [_i % 2 ? _i + 1 : _i] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> index,
									          polygonElement -> vertices [_i % 2 ? _i : _i + 1] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i + 2] -> index,
									          polygonElement -> vertices [_i + 2] -> i,
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

									++ glIndices;
								}

								break;
							case GL_TRIANGLES:

								for (size_t _i = 0; _i < polygonElement -> vertices .size (); _i += 3)
								{
									setPoint (polygonElement -> vertices [_i] -> index,
									          polygonElement -> vertices [_i] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i + 1] -> index,
									          polygonElement -> vertices [_i + 1] -> i,
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

									++ glIndices;

									setPoint (polygonElement -> vertices [_i + 2] -> index,
									          polygonElement -> vertices [_i + 2] -> i,
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

									++ glIndices;
								}

								break;
							default:
								break;
						}
					}

					for (int _i = 0; _i < numIndices - end; ++ _i)
						delete vertices [_i];
				}

				index += numIndices;
				i     += numIndices;
			}
		}
		else
		{
			index += numIndices;
			i     += numIndices;
		}

		if (index == coordIndex .end ())
			break;
	}

	setTextureCoordinateGenerator (*_textureCoordinateGenerator);
	setGLMode (GL_TRIANGLES);
	setGLIndices (glIndices);
}

void
IndexedFaceSet::setPoint (
   const MFInt32::const_iterator & index,
   const int i,
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
   const SFNode <Coordinate> & _coord
   )
{
	float tx = 0, ty = 0, nx = 0, ny = 0, nz = 0, r = 0, g = 0, b = 0, a = 0, x, y, z;

	if (_textureCoordinate)
	{
		if (texCoordIndex [i] >= 0)
		{
			_textureCoordinate -> point .at (texCoordIndex [i]) .getValue (tx, ty);
		}

		getGLTexCoord () -> push_back (tx);
		getGLTexCoord () -> push_back (ty);
	}
	else if (_textureCoordinateGenerator)
	{ }
	else
	{
		tx = _texCoord [i] .x ();
		ty = _texCoord [i] .y ();
		getGLTexCoord () -> push_back (tx);
		getGLTexCoord () -> push_back (ty);
	}

	if (_normal)
	{
		if (normalPerVertex and normalIndex [i] >= 0)
			_normal -> vector [normalIndex [i]] .getValue (nx, ny, nz);
		else
		{
			nx = faceNormal .x ();
			ny = faceNormal .y ();
			nz = faceNormal .z ();
		}
	}
	else
	{
		nx = _normals [i] .x ();
		ny = _normals [i] .y ();
		nz = _normals [i] .z ();
	}

	if (_color)
	{
		if (colorPerVertex and colorIndex [i] >= 0)
			_color -> color [colorIndex [i]] .getValue (r, g, b);
		else
			faceColor .getValue (r, g, b);

		getGLColors () -> push_back (r);
		getGLColors () -> push_back (g);
		getGLColors () -> push_back (b);
	}
	else if (_colorRGBA)
	{
		if (colorPerVertex and colorIndex [i] >= 0)
			_colorRGBA -> color [colorIndex [i]] .getValue (r, g, b, a);
		else
			faceColorRGBA .getValue (r, g, b, a);

		getGLColors () -> push_back (r);
		getGLColors () -> push_back (g);
		getGLColors () -> push_back (b);
		getGLColors () -> push_back (1 - a);
	}

	getGLNormals () -> push_back (nx);
	getGLNormals () -> push_back (ny);
	getGLNormals () -> push_back (nz);

	_coord -> point .at (*index) .getValue (x, y, z);

	getGLPoints () -> push_back (x);
	getGLPoints () -> push_back (y);
	getGLPoints () -> push_back (z);
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
