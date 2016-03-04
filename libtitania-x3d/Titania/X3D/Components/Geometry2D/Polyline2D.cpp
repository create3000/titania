/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include "Polyline2D.h"

#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType Polyline2D::component      = ComponentType::GEOMETRY_2D;
const std::string   Polyline2D::typeName       = "Polyline2D";
const std::string   Polyline2D::containerField = "geometry";

/*
 * Changed accessType of lineSegments to inputOutput, so Polyline2D can be animated.
 */

Polyline2D::Fields::Fields () :
	lineSegments (new MFVec2f ())
{ }

Polyline2D::Polyline2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Polyline2D);

	addField (inputOutput, "metadata",     metadata ());
	addField (inputOutput, "lineSegments", lineSegments ());

	lineSegments () .setUnit (UnitCategory::LENGTH);
}

X3DBaseNode*
Polyline2D::create (X3DExecutionContext* const executionContext) const
{
	return new Polyline2D (executionContext);
}

void
Polyline2D::initialize ()
{
	X3DGeometryNode::initialize ();

	lineSegments () .addInterest (this, &Polyline2D::update);
}

void
Polyline2D::build ()
{
	for (const auto & vertex : lineSegments ())
		getVertices () .emplace_back (vertex .getX (), vertex .getY (), 0);

	addElements (GL_LINE_STRIP, getVertices () .size ());
	setSolid (false);
}

void
Polyline2D::draw (const ShapeContainer* const context)
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw (context);
}

SFNode
Polyline2D::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Polyline2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> metadata () = metadata ();
	geometry -> coord ()    = coord;

	coord -> point () .assign (getVertices () .begin (), getVertices () .end ());

	for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
		geometry -> coordIndex () .emplace_back (i);

	geometry -> coordIndex () .emplace_back (-1);

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
