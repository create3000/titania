/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*- */
/*******************************************************************************
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

#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Coordinate.h"
#include "IndexedLineSet.h"

namespace titania {
namespace X3D {

IndexedLineSet::IndexedLineSet (X3DExecutionContext* const executionContext) :
	   X3DBasicNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),                                 
	 set_colorIndex (),                                 // MFInt32 [in]     set_colorIndex
	 set_coordIndex (),                                 // MFInt32 [in]     set_coordIndex
	         attrib (),                                 // MFNode  [in,out] attrib          [ ]         [X3DVertexAttributeNode]
	          color (),                                 // SFNode  [in,out] color           NULL        [X3DColorNode]
	          coord (),                                 // SFNode  [in,out] coord           NULL        [X3DCoordinateNode]
	       fogCoord (),                                 // SFNode  [in,out] fogCoord        [ ]         [FogCoordinate]
	     colorIndex (),                                 // MFInt32 [ ]      colorIndex      [ ]         [0,∞) or -1
	 colorPerVertex (true),                             // SFBool  [ ]      colorPerVertex  TRUE
	     coordIndex (),                                 // MFInt32 [ ]      coordIndex      [ ]         [0,∞) or -1
	      numPoints (0),                                
	       numFaces (0),                                
	      numColors (0)
{
	setComponent ("Rendering");
	setTypeName ("IndexedLineSet");

	appendField (inputOutput,    "metadata",       metadata);
	appendField (inputOnly,      "set_coordIndex", set_coordIndex);
	appendField (inputOnly,      "set_colorIndex", set_colorIndex);
	appendField (inputOutput,    "attrib",         attrib);
	appendField (inputOutput,    "coord",          coord);
	appendField (inputOutput,    "color",          color);
	appendField (inputOutput,    "fogCoord",       fogCoord);
	appendField (initializeOnly, "coordIndex",     coordIndex);
	appendField (initializeOnly, "colorIndex",     colorIndex);
	appendField (initializeOnly, "colorPerVertex", colorPerVertex);
}

X3DBasicNode*
IndexedLineSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedLineSet (executionContext);
}

void
IndexedLineSet::initialize ()
{
	X3DGeometryNode::initialize ();

	set_coordIndex .addInterest (this, &IndexedLineSet::_set_coordIndex);
	set_colorIndex .addInterest (this, &IndexedLineSet::_set_colorIndex);

	_set_coordIndex (coordIndex);
}

void
IndexedLineSet::_set_coordIndex (const MFInt32::value_type & value)
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

		_set_colorIndex (colorIndex);
	}
}

void
IndexedLineSet::_set_colorIndex (const MFInt32::value_type & value)
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

Box3f
IndexedLineSet::createBBox ()
{
	SFNode <Coordinate> _coord = coord;

	if (not _coord or not _coord -> point .size ())
		return Box3f ();

	Vector3f min = _coord -> point .front ();
	Vector3f max = _coord -> point .front ();

	for (const auto & point : _coord -> point)
	{
		min = math::min <float> (min, point);
		max = math::max <float> (max, point);
	}

	Vector3f size   = max - min;
	Vector3f center = min + size * 0.5f;

	return Box3f (size, center);
}

void
IndexedLineSet::build ()
{
	X3DGeometryNode::build ();

	SFNode <Coordinate> _coord = coord;

	if (not _coord or not _coord -> point .size ())
		return;

	if (_coord -> point .size () < (size_t) numPoints)
		_coord -> point .resize (numPoints);

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

	float r = 0, g = 0, b = 0, a = 0, x, y, z;

	GLsizei glIndices = 0;

	for (size_t i = 0, face = 0; i < coordIndex .size (); ++ i, ++ face)
	{
		int numIndices = 0;

		for (
		   size_t _i = i;
		   coordIndex [_i] >= 0 and _i < coordIndex .size ();
		   ++ _i, ++ numIndices
		   )
			;

		if (numIndices > 1)
		{
			SFColor     faceColor;
			SFColorRGBA faceColorRGBA;

			if (not colorPerVertex)
			{
				if (_color and colorIndex [face] >= 0)
					faceColor = _color -> color [colorIndex [face]];
				else if (_colorRGBA and colorIndex [face] >= 0)
					faceColorRGBA = _colorRGBA -> color [colorIndex [face]];
			}

			for (int _i = 0; _i < numIndices - 1; ++ _i, ++ i)
			{
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

				_coord -> point .at (coordIndex [i]) .getValue (x, y, z);
				getGLPoints () -> push_back (x);
				getGLPoints () -> push_back (y);
				getGLPoints () -> push_back (z);

				++ glIndices;

				if (_color)
				{
					if (colorPerVertex and colorIndex [i + 1] >= 0)
						_color -> color [colorIndex [i + 1]] .getValue (r, g, b);
					else
						faceColor .getValue (r, g, b);

					getGLColors () -> push_back (r);
					getGLColors () -> push_back (g);
					getGLColors () -> push_back (b);
				}
				else if (_colorRGBA)
				{
					if (colorPerVertex and colorIndex [i + 1] >= 0)
						_colorRGBA -> color [colorIndex [i + 1]] .getValue (r, g, b, a);
					else
						faceColorRGBA .getValue (r, g, b, a);

					getGLColors () -> push_back (r);
					getGLColors () -> push_back (g);
					getGLColors () -> push_back (b);
					getGLColors () -> push_back (1 - a);
				}

				_coord -> point .at (coordIndex [i + 1]) .getValue (x, y, z);
				getGLPoints () -> push_back (x);
				getGLPoints () -> push_back (y);
				getGLPoints () -> push_back (z);

				++ glIndices;
			}

			++ i;
		}
		else
		{
			i += numIndices;
		}
	}

	setGLMode (GL_LINES);
	setGLIndices (glIndices);
}

void
IndexedLineSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

} // X3D
} // titania
