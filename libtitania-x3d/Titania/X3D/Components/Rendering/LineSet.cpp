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

#include "LineSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../Rendering/IndexedLineSet.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/X3DVertexAttributeNode.h"


namespace titania {
namespace X3D {

const std::string LineSet::componentName  = "Rendering";
const std::string LineSet::typeName       = "LineSet";
const std::string LineSet::containerField = "geometry";

LineSet::Fields::Fields () :
	vertexCount (new MFInt32 ()),
	     attrib (new MFNode ()),
	   fogCoord (new SFNode ()),
	      color (new SFNode ()),
	      coord (new SFNode ())
{ }

LineSet::LineSet (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields (),
	        attribNodes (),
	          colorNode (),
	          coordNode (),
	        transparent (false)
{
	addType (X3DConstants::LineSet);

	addField (inputOutput, "metadata",    metadata ());
	addField (inputOutput, "vertexCount", vertexCount ());
	addField (inputOutput, "attrib",      attrib ());
	addField (inputOutput, "fogCoord",    fogCoord ());
	addField (inputOutput, "color",       color ());
	addField (inputOutput, "coord",       coord ());

	addChildObjects (attribNodes,
	                 colorNode,
	                 coordNode);

	setGeometryType (GeometryType::GEOMETRY_LINES);
}

X3DBaseNode*
LineSet::create (X3DExecutionContext* const executionContext) const
{
	return new LineSet (executionContext);
}

void
LineSet::initialize ()
{
	X3DLineGeometryNode::initialize ();

	attrib () .addInterest (&LineSet::set_attrib, this);
	color ()  .addInterest (&LineSet::set_color, this);
	coord ()  .addInterest (&LineSet::set_coord, this);

	set_attrib ();
	set_color ();
	set_coord ();
}

const X3DPtr <ComposedShader> &
LineSet::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getWireframeShader ();
}

void
LineSet::set_attrib ()
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
LineSet::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (this);
		colorNode -> removeInterest (&LineSet::set_transparency, this);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (this);
		colorNode -> addInterest (&LineSet::set_transparency, this);
		
		set_transparency ();
	}
	else
		transparent = false;
}

void
LineSet::set_transparency ()
{
	transparent = colorNode -> isTransparent ();
}

void
LineSet::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this);

	coordNode .set (x3d_cast <X3DCoordinateNode*> (coord ()));

	if (coordNode)
		coordNode -> addInterest (this);
}

void
LineSet::build ()
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	// Fill GeometryNode

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	const size_t size        = coordNode -> getSize ();
	size_t       index       = 0;

	for (int32_t count : vertexCount ())
	{
		if (index + count > size)
			break;

		if (count > 1)
		{
			const size_t numVertices = 2 * count - 2;

			for (size_t i = 0; i < numVertices; ++ i, index += i & 1)
			{
				for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
					attribNodes [a] -> addValue (attribArrays [a], index);

				if (colorNode)
					colorNode -> addColor (getColors (), index);

				coordNode -> addVertex (getVertices (), index);
			}

			++ index;
		}
		else
		{
			index += count;
		}
	}

	addElements (GL_LINES, getVertices () .size ());

	setSolid (true);
	setAttribs (attribNodes, attribArrays);
}

SFNode
LineSet::toPrimitive () const
{
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> metadata () = metadata ();
	geometry -> attrib ()   = attrib ();
	geometry -> fogCoord () = fogCoord ();
	geometry -> color ()    = color ();
	geometry -> coord ()    = coord ();

	size_t       index = 0;
	const size_t size  = coordNode -> getSize ();

	for (const auto & count : vertexCount ())
	{
		if (index + count > size)
			break;

		if (count > 1)
		{
			for (size_t i = 0; i < (size_t) count; ++ i, ++ index)
				geometry -> coordIndex () .emplace_back (index);

			geometry -> coordIndex () .emplace_back (-1);
		}
		else
			index += count;
	}

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

LineSet::~LineSet ()
{ }

} // X3D
} // titania
