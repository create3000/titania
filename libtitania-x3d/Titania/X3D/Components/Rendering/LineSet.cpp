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

#include "LineSet.h"

#include "../../Bits/Cast.h"
#include "../../Execution/X3DExecutionContext.h"

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
	X3DGeometryNode (),
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

	addChildren (attribNodes,
	             colorNode,
	             coordNode);
}

X3DBaseNode*
LineSet::create (X3DExecutionContext* const executionContext) const
{
	return new LineSet (executionContext);
}

void
LineSet::initialize ()
{
	X3DGeometryNode::initialize ();

	attrib () .addInterest (this, &LineSet::set_attrib);
	color ()  .addInterest (this, &LineSet::set_color);
	coord ()  .addInterest (this, &LineSet::set_coord);

	set_attrib ();
	set_color ();
	set_coord ();
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

	attribNodes .set (value .begin (), value .end ());

	for (const auto & node : attribNodes)
		node -> addInterest (this);
}

void
LineSet::set_color ()
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
LineSet::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this);

	coordNode .set (x3d_cast <X3DCoordinateNode*> (coord ()));

	if (coordNode)
		coordNode -> addInterest (this);
}

Box3f
LineSet::createBBox ()
{
	if (coordNode)
		return coordNode -> getBBox ();

	return Box3f ();
}

void
LineSet::build ()
{
	if (not coordNode or coordNode -> isEmpty ())
		return;

	// Fill GeometryNode

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	size_t       index = 0;
	const size_t size  = coordNode -> getSize ();

	for (const auto count : vertexCount ())
	{
		// Create two vertices for each line.

		if (index + count > size)
			break;

		if (count > 1)
		{
			for (size_t i = 0; i < (size_t) count; ++ i, ++ index)
			{
				for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
					attribNodes [a] -> addValue (attribArrays [a], index);

				if (colorNode)
					colorNode -> addColor (getColors (), index);

				coordNode -> addVertex (getVertices (), index);
			}

			addElements (GL_LINE_STRIP, count);
		}
		else
		{
			if (count > 0)
				++ index;
		}
	}

	setSolid (false);
	setAttribs (attribNodes, attribArrays);
}

void
LineSet::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

} // X3D
} // titania
