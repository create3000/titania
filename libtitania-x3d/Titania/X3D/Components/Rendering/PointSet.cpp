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

#include "PointSet.h"

#include "../../Bits/Cast.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../EnvironmentalEffects/FogCoordinate.h"
#include "../Rendering/X3DColorNode.h"
#include "../Rendering/X3DCoordinateNode.h"
#include "../Shaders/ComposedShader.h"
#include "../Shaders/X3DVertexAttributeNode.h"


namespace titania {
namespace X3D {

const Component   PointSet::component      = Component ("Rendering", 1);
const std::string PointSet::typeName       = "PointSet";
const std::string PointSet::containerField = "geometry";

PointSet::Fields::Fields () :
	  attrib (new MFNode ()),
	fogCoord (new SFNode ()),
	   color (new SFNode ()),
	   coord (new SFNode ())
{ }

PointSet::PointSet (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DLineGeometryNode (),
	             fields (),
	        attribNodes (),
	       fogCoordNode (),
	          colorNode (),
	          coordNode ()
{
	addType (X3DConstants::PointSet);

	addField (inputOutput, "metadata", metadata ());
	addField (inputOutput, "attrib",   attrib ());
	addField (inputOutput, "fogCoord", fogCoord ());
	addField (inputOutput, "color",    color ());
	addField (inputOutput, "coord",    coord ());

	addChildObjects (attribNodes,
	                 fogCoordNode,
	                 colorNode,
	                 coordNode);

	setGeometryType (GeometryType::GEOMETRY_POINTS);
}

X3DBaseNode*
PointSet::create (X3DExecutionContext* const executionContext) const
{
	return new PointSet (executionContext);
}

void
PointSet::initialize ()
{
	X3DLineGeometryNode::initialize ();

	attrib ()   .addInterest (&PointSet::set_attrib,   this);
	fogCoord () .addInterest (&PointSet::set_fogCoord, this);
	color ()    .addInterest (&PointSet::set_color,    this);
	coord ()    .addInterest (&PointSet::set_coord,    this);

	set_attrib ();
	set_fogCoord ();
	set_color ();
	set_coord ();
}

const X3DPtr <ComposedShader> &
PointSet::getShaderNode (X3DBrowser* const browser)
{
	return browser -> getPointShader ();
}

void
PointSet::set_attrib ()
{
	for (const auto & node : attribNodes)
		node -> removeInterest (&PointSet::requestRebuild, this);

	std::vector <X3DVertexAttributeNode*> value;

	for (const auto & node : attrib ())
	{
		const auto attribNode = x3d_cast <X3DVertexAttributeNode*> (node);
		
		if (attribNode)
			value .emplace_back (attribNode);
	}

	attribNodes .set (value .cbegin (), value .cend ());

	for (const auto & node : attribNodes)
		node -> addInterest (&PointSet::requestRebuild, this);
}

void
PointSet::set_fogCoord ()
{
	if (fogCoordNode)
		fogCoordNode -> removeInterest (&PointSet::requestRebuild, this);

	fogCoordNode = x3d_cast <FogCoordinate*> (fogCoord ());

	if (fogCoordNode)
		fogCoordNode -> addInterest (&PointSet::requestRebuild, this);
}

void
PointSet::set_color ()
{
	if (colorNode)
	{
		colorNode -> removeInterest (&PointSet::requestRebuild,  this);
		colorNode -> removeInterest (&PointSet::set_transparent, this);
	}

	colorNode .set (x3d_cast <X3DColorNode*> (color ()));

	if (colorNode)
	{
		colorNode -> addInterest (&PointSet::requestRebuild,  this);
		colorNode -> addInterest (&PointSet::set_transparent, this);
	}
		
	set_transparent ();
}

void
PointSet::set_transparent ()
{
	setTransparent (colorNode and colorNode -> isTransparent ());
}

void
PointSet::set_coord ()
{
	if (coordNode)
		coordNode -> removeInterest (this);

	coordNode .set (x3d_cast <X3DCoordinateNode*> (coord ()));

	if (coordNode)
		coordNode -> addInterest (this);
}

void
PointSet::build ()
{
	if (not coordNode)
		return;

	std::vector <std::vector <float>> attribArrays (attribNodes .size ());

	for (size_t a = 0, size = attribNodes .size (); a < size; ++ a)
	{
		attribArrays [a] .reserve (coordNode -> getSize ());

		for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
			attribNodes [a] -> addValue (attribArrays [a], i);
	}

	if (fogCoordNode)
	{
		getFogDepths () .reserve (coordNode -> getSize ());

		for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
			fogCoordNode -> addDepth (getFogDepths (), i);
	}

	if (colorNode)
	{
		getColors () .reserve (coordNode -> getSize ());

		for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
			colorNode -> addColor (getColors (), i);
	}

	getVertices () .reserve (coordNode -> getSize ());

	for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
		coordNode -> addVertex (getVertices (), i);

	addElements (GL_POINTS, getVertices () .size ());

	setAttribs (attribNodes, attribArrays);
}

PointSet::~PointSet ()
{ }

} // X3D
} // titania
