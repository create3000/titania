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

#include "IndexedLineSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string IndexedLineSet::componentName  = "Rendering";
const std::string IndexedLineSet::typeName       = "IndexedLineSet";
const std::string IndexedLineSet::containerField = "geometry";

IndexedLineSet::Fields::Fields () :
	colorPerVertex (new SFBool (true)),
	    colorIndex (new MFInt32 ()),
	    coordIndex (new MFInt32 ()),
	        attrib (new MFNode ()),
	      fogCoord (new SFNode ()),
	         color (new SFNode ()),
	         coord (new SFNode ())
{ }

IndexedLineSet::IndexedLineSet (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields (),
	      colorNode (),
	      coordNode (),
	   numPolylines (0),
	    transparent (false)
{
	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "colorPerVertex", colorPerVertex ());
	addField (inputOutput,    "colorIndex",     colorIndex ());
	addField (inputOutput,    "coordIndex",     coordIndex ());
	addField (inputOutput,    "attrib",         attrib ());
	addField (inputOutput,    "fogCoord",       fogCoord ());
	addField (inputOutput,    "color",          color ());
	addField (inputOutput,    "coord",          coord ());

	addChildren (colorNode,
	             coordNode);
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

	coordIndex () .addInterest (this, &IndexedLineSet::set_coordIndex);
	colorIndex () .addInterest (this, &IndexedLineSet::set_colorIndex);
	color ()      .addInterest (this, &IndexedLineSet::set_color);
	coord ()      .addInterest (this, &IndexedLineSet::set_coord);

	set_color ();
	set_coord ();
	set_coordIndex ();
}

void
IndexedLineSet::set_coordIndex ()
{
	numPolylines = 0;

	if (coordNode)
	{
		if (not coordIndex () .empty ())
		{
			// Determine number of points and polygons.

			for (const auto & index : coordIndex ())
			{
				if (index < 0)
					++ numPolylines;
			}

			if (coordIndex () .back () > -1)
				++ numPolylines;

			set_colorIndex ();
		}
	}
}

void
IndexedLineSet::set_colorIndex ()
{
	if (colorNode)
	{
		// Fill up colorIndex if to small.
		if (colorPerVertex ())
		{
			for (int32_t i = colorIndex () .size (), size = coordIndex () .size (); i < size; ++ i)
			{
				colorIndex () .emplace_back (coordIndex () [i]);
			}
		}
		else
		{
			for (int32_t i = colorIndex () .size (), size = numPolylines; i < size; ++ i)
			{
				colorIndex () .emplace_back (i);
			}
		}
	}
}

void
IndexedLineSet::set_color ()
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
IndexedLineSet::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this);

	coordNode .set (x3d_cast <X3DCoordinateNode*> (coord ()));

	if (coordNode)
		coordNode -> addInterest (this);
}

Box3f
IndexedLineSet::createBBox ()
{
	if (coordNode)
		return coordNode -> getBBox ();

	return Box3f ();
}

std::vector <Vector3f>
IndexedLineSet::getPolylines () const
{
	// Polyline map

	std::vector <Vector3f> polylines;

	if (not coordNode or coordNode -> isEmpty ())
		return polylines;

	auto polylineIndices = std::move (getPolylineIndices ());

	for (const auto polyline : polylineIndices)
	{
		// Create two vertices for each line.

		for (size_t line = 0, end = polyline .size () - 1; line < end; ++ line)
		{
			for (size_t index = line, end = line + 2; index < end; ++ index)
			{
				size_t i = polyline [index];
				coordNode -> addVertex (polylines, coordIndex () [i]);
			}
		}
	}

	return polylines;
}

std::deque <std::deque <size_t>> 
IndexedLineSet::getPolylineIndices () const
{
	std::deque <std::deque <size_t>>  polylines;
	std::deque <size_t>               polyline;

	if (not coordIndex () .empty ())
	{
		size_t i = 0;

		for (const auto & index : coordIndex ())
		{
			if (index >= 0)
				// Add vertex.
				polyline .emplace_back (i);

			else
			{
				// Negativ index.

				if (not polyline .empty ())
				{
					if (polyline .size () > 1)
					{
						// Add polylines.
						polylines .emplace_back (std::move (polyline));
					}

					polyline .clear ();
				}
			}

			++ i;
		}

		if (coordIndex () .back () >= 0)
		{
			if (not polyline .empty ())
			{
				if (polyline .size () > 1)
					polylines .emplace_back (std::move (polyline));
			}
		}
	}

	return polylines;
}

void
IndexedLineSet::build ()
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	auto polylines = std::move (getPolylineIndices ());

	// Fill GeometryNode

	int face = 0;

	for (const auto polyline : polylines)
	{
		// Create two vertices for each line.

		for (size_t line = 0, end = polyline .size () - 1; line < end; ++ line)
		{
			for (size_t index = line, end = line + 2; index < end; ++ index)
			{
				auto i = polyline [index];

				if (colorNode)
				{
					if (colorPerVertex ())
						colorNode -> addColor (getColors (), colorIndex () [i]);

					else
						colorNode -> addColor (getColors (), colorIndex () [face]);
				}

				coordNode -> addVertex (getVertices (), coordIndex () [i]);
			}
		}

		++ face;
	}

	addElements (GL_LINES, getVertices () .size ());
	setSolid (false);
}

void
IndexedLineSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

void
IndexedLineSet::dispose ()
{
	colorNode .dispose ();
	coordNode .dispose ();

	X3DGeometryNode::dispose ();
}

} // X3D
} // titania
