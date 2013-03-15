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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#include "IndexedLineSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/Color.h"
#include "../Rendering/ColorRGBA.h"
#include "../Rendering/Coordinate.h"

namespace titania {
namespace X3D {

IndexedLineSet::IndexedLineSet (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext), 
	X3DGeometryNode (),                                                    
	 colorPerVertex (true),                                                // SFBool  [ ]      colorPerVertex  TRUE
	     colorIndex (),                                                    // MFInt32 [ ]      colorIndex      [ ]         [0,∞) or -1
	     coordIndex (),                                                    // MFInt32 [ ]      coordIndex      [ ]         [0,∞) or -1
	         attrib (),                                                    // MFNode  [in,out] attrib          [ ]         [X3DVertexAttributeNode]
	       fogCoord (),                                                    // SFNode  [in,out] fogCoord        [ ]         [FogCoordinate]
	          color (),                                                    // SFNode  [in,out] color           NULL        [X3DColorNode]
	          coord (),                                                    // SFNode  [in,out] coord           NULL        [X3DCoordinateNode]
	      polylines ()                                                     
{
	setComponent ("Rendering");
	setTypeName ("IndexedLineSet");

	addField (inputOutput,    "metadata",       metadata);
	addField (initializeOnly, "colorPerVertex", colorPerVertex);
	addField (inputOutput,    "colorIndex",     colorIndex);
	addField (inputOutput,    "coordIndex",     coordIndex);
	addField (inputOutput,    "attrib",         attrib);
	addField (inputOutput,    "fogCoord",       fogCoord);
	addField (inputOutput,    "color",          color);
	addField (inputOutput,    "coord",          coord);
}

X3DBaseNode*
IndexedLineSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedLineSet (executionContext);
}

void
IndexedLineSet::initialize ()
{
	X3DGeometryNode::initialize ();

	coordIndex .addInterest (this, &IndexedLineSet::set_coordIndex);
	colorIndex .addInterest (this, &IndexedLineSet::set_colorIndex);

	set_coordIndex ();
}

void
IndexedLineSet::set_coordIndex ()
{
	auto _coord = x3d_cast <Coordinate*> (coord .getValue ());

	if (not _coord)
		return;

	polylines .clear ();

	// Fill up coordIndex if there are no indices.
	if (coordIndex .empty ())
	{
		for (size_t i = 0; i < _coord -> point .size (); ++ i)
			coordIndex .push_back (i);
	}

	if (coordIndex .size ())
	{
		// Add -1 (polylines end marker) to coordIndex if not present.
		if (coordIndex .back () >= 0)
			coordIndex .push_back (-1);

		// Construct polylines array and determine the number of used points.
		size_t  i         = 0;
		int32_t numPoints = -1;

		std::deque <size_t> polyline;

		for (const auto & index : coordIndex)
		{
			numPoints = std::max <int32_t> (numPoints, index);

			if (index >= 0)
				// Add vertex.
				polyline .emplace_back (i);

			else
			{
				// Negativ index.

				if (polyline .size ())
				{
					if (polyline .size () > 1)
					{
						// Add polylines.
						polylines .emplace_back (polyline);
					}

					polyline .clear ();
				}
			}

			++ i;
		}

		++ numPoints;

		if (polylines .size ())
		{
			// Resize coord .point if to small
			_coord -> resize (numPoints);

			set_colorIndex ();
		}
	}
}

void
IndexedLineSet::set_colorIndex ()
{
	auto _color     = x3d_cast <Color*> (color .getValue ());
	auto _colorRGBA = x3d_cast <ColorRGBA*> (color .getValue ());

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
			for (size_t i = colorIndex .size (); i < polylines .size (); ++ i)
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
			_color -> resize (numColors);
		}
		else if (_colorRGBA)
		{
			_colorRGBA -> resize (numColors);
		}
	}
}

void
IndexedLineSet::build ()
{
	auto _coord = x3d_cast <Coordinate*> (coord .getValue ());

	if (not _coord or not _coord -> point .size ())
		return;

	// Color

	auto _color     = x3d_cast <Color*> (color .getValue ());
	auto _colorRGBA = x3d_cast <ColorRGBA*> (color .getValue ());

	// Fill GeometryNode

	int face = 0;

	for (const auto polyline : polylines)
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

		// Create two vertices for each line.

		for (size_t line = 0, end = polyline .size () - 1; line < end; ++ line)
		{
			for (size_t index = line, end = line + 2; index < end; ++ index)
			{
				auto i = polyline [index];

				if (_color)
				{
					if (colorPerVertex and colorIndex [i] >= 0)
						getColors () .emplace_back (_color -> color [colorIndex [i]]);

					else
						getColors () .emplace_back (faceColor);
				}
				else if (_colorRGBA)
				{
					if (colorPerVertex and colorIndex [i] >= 0)
						getColorsRGBA () .emplace_back (_colorRGBA -> color [colorIndex [i]]);

					else
						getColorsRGBA () .emplace_back (faceColorRGBA);
				}

				getVertices () .emplace_back (_coord -> point [coordIndex [i]]);
			}
		}

		++ face;
	}

	addElement (getVertices () .size ());
	setVertexMode (GL_LINES);
	setSolid (false);
}

void
IndexedLineSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

} // X3D
} // titania
