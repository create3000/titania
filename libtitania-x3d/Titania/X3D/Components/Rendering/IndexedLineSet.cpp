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

#include "IndexedLineSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/Rendering/L-System.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"

#include "../Core/MetadataSet.h"
#include "../Rendering/PointSet.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/X3DVertexAttributeNode.h"

namespace titania {
namespace X3D {

const ComponentType IndexedLineSet::component      = ComponentType::RENDERING;
const std::string   IndexedLineSet::typeName       = "IndexedLineSet";
const std::string   IndexedLineSet::containerField = "geometry";

IndexedLineSet::Fields::Fields () :
	colorPerVertex (new SFBool (true)),
	    colorIndex (new MFInt32 ()),
	    coordIndex (new MFInt32 ()),
	        attrib (new MFNode ()),
	      fogCoord (new SFNode ()),
	         color (new SFNode ()),
	         coord (new SFNode ()),
	       options (new SFNode ())
{ }

IndexedLineSet::IndexedLineSet (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields (),
	        attribNodes (),
	          colorNode (),
	          coordNode (),
	        optionsNode (),
	        transparent (false)
{
	addType (X3DConstants::IndexedLineSet);

	addField (inputOutput,    "metadata",       metadata ());
	addField (initializeOnly, "colorPerVertex", colorPerVertex ());
	addField (inputOutput,    "colorIndex",     colorIndex ());
	addField (inputOutput,    "coordIndex",     coordIndex ());
	addField (inputOutput,    "attrib",         attrib ());
	addField (inputOutput,    "fogCoord",       fogCoord ());
	addField (inputOutput,    "color",          color ());
	addField (inputOutput,    "coord",          coord ());
	addField (inputOutput,    "options",        options ());

	addChildObjects (attribNodes,
	                 colorNode,
	                 coordNode,
	                 optionsNode);

	options () .isHidden (true);

	setGeometryType (GeometryType::GEOMETRY_LINES);
}

X3DBaseNode*
IndexedLineSet::create (X3DExecutionContext* const executionContext) const
{
	return new IndexedLineSet (executionContext);
}

void
IndexedLineSet::initialize ()
{
	X3DLineGeometryNode::initialize ();

	attrib ()   .addInterest (&IndexedLineSet::set_attrib,  this);
	color ()    .addInterest (&IndexedLineSet::set_color,   this);
	coord ()    .addInterest (&IndexedLineSet::set_coord,   this);
	options ()  .addInterest (&IndexedLineSet::set_options, this);
	shutdown () .addInterest (&IndexedLineSet::set_shutdown, this);

	set_attrib ();
	set_color ();
	set_coord ();

	try
	{
		const auto typeName    = getMetaData <std::string> ("/IndexedLineSet/options/@typeName");
		const auto metaOptions = getMetadataSet ("/IndexedLineSet/options");
	
		if (typeName == "L-System" or typeName == "LSystemOptions")
			optionsNode .set (MakePtr <LSystem> (getExecutionContext ()));

		optionsNode -> fromMetaData (metaOptions);
		optionsNode -> setup ();
		optionsNode -> addNode (this);

		options () .set (optionsNode);
	}
	catch (const X3D::X3DError & error)
	{
		set_options ();
	}
}

const X3DPtr <ComposedShader> &
IndexedLineSet::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

void
IndexedLineSet::set_attrib ()
{
	for (const auto & node : attribNodes)
		node -> removeInterest (this);

	std::vector <X3DVertexAttributeNode*> value;

	for (const auto & node : attrib ())
	{
		const auto attribNode = x3d_cast <X3DVertexAttributeNode*> (node);
		
		if (attribNode)
			value .emplace_back (attribNode);
	}

	attribNodes .set (value .cbegin (), value .cend ());

	for (const auto & node : attribNodes)
		node -> addInterest (this);
}

void
IndexedLineSet::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (this);
		colorNode -> removeInterest (&IndexedLineSet::set_transparency, this);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (this);
		colorNode -> addInterest (&IndexedLineSet::set_transparency, this);
		
		set_transparency ();
	}
	else
		transparent = false;
}

void
IndexedLineSet::set_transparency ()
{
	transparent = colorNode -> isTransparent ();
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

void
IndexedLineSet::set_options ()
{
	removeMetaData ("/IndexedLineSet/options");

	if (optionsNode)
		optionsNode -> removeNode (this);

	optionsNode .set (x3d_cast <LSystem*> (options ()));

	if (optionsNode)
		optionsNode -> addNode (this);
}

size_t
IndexedLineSet::getColorIndex (const size_t index, const bool) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return coordIndex () [index];
}

size_t
IndexedLineSet::getColorIndex (const size_t index) const
{
	if (index < colorIndex () .size ())
		return colorIndex () [index];

	return index;
}

std::vector <Vector3d>
IndexedLineSet::getPolylines () const
{
	// Polyline map

	std::vector <Vector3d> polylines;

	if (not coordNode or coordNode -> isEmpty ())
		return polylines;

	const auto polylineIndices = getPolylineIndices ();

	for (const auto & polyline : polylineIndices)
	{
		// Create two vertices for each line.

		if (polyline .empty ())
			continue;

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

std::vector <std::vector <size_t>> 
IndexedLineSet::getPolylineIndices () const
{
	std::vector <std::vector <size_t>> polylines;
	std::vector <size_t>               polyline;

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
				// Add polylines.
				polylines .emplace_back (std::move (polyline));
			}

			++ i;
		}

		if (coordIndex () .back () >= 0)
		{
			polylines .emplace_back (std::move (polyline));
		}
	}

	return polylines;
}

void
IndexedLineSet::build ()
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	const auto polylines = getPolylineIndices ();

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	// Fill GeometryNode

	size_t face = 0;

	for (const auto & polyline : polylines)
	{
		// Create two vertices for each line.

		if (polyline .size () > 1)
		{
			for (size_t line = 0, end = polyline .size () - 1; line < end; ++ line)
			{
				for (size_t index = line, end = line + 2; index < end; ++ index)
				{
					const auto i = polyline [index];
	
					for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
						attribNodes [a] -> addValue (attribArrays [a], coordIndex () [i]);
	
					if (colorNode)
					{
						if (colorPerVertex ())
							colorNode -> addColor (getColors (), getColorIndex (i, true));
	
						else
							colorNode -> addColor (getColors (), getColorIndex (face));
					}
	
					coordNode -> addVertex (getVertices (), coordIndex () [i]);
				}
			}
		}

		++ face;
	}

	addElements (GL_LINES, getVertices () .size ());
	setSolid (true);
	setAttribs (attribNodes, attribArrays);
}

SFNode
IndexedLineSet::toPrimitive () const
{
	const auto geometry = getExecutionContext () -> createNode <PointSet> ();

	geometry -> metadata () = metadata ();
	geometry -> attrib ()   = attrib ();
	geometry -> fogCoord () = fogCoord ();

	if (colorPerVertex ())
		geometry -> color () = color ();

	geometry -> coord () = coord ();

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

void
IndexedLineSet::set_shutdown ()
{
	if (optionsNode)
		optionsNode -> removeNode (this);
}

IndexedLineSet::~IndexedLineSet ()
{ }

} // X3D
} // titania
