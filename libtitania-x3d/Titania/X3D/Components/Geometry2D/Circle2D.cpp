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

#include "Circle2D.h"

#include "../../Browser/Geometry2D/Circle2DOptions.h"
#include "../../Browser/X3DBrowser.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Rendering/IndexedLineSet.h"
#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const ComponentType Circle2D::component      = ComponentType::GEOMETRY_2D;
const std::string   Circle2D::typeName       = "Circle2D";
const std::string   Circle2D::containerField = "geometry";

Circle2D::Fields::Fields () :
	radius (new SFFloat (1))
{ }

Circle2D::Circle2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addType (X3DConstants::Circle2D);

	addField (inputOutput,    "metadata", metadata ());
	addField (initializeOnly, "radius",   radius ());
}

X3DBaseNode*
Circle2D::create (X3DExecutionContext* const executionContext) const
{
	return new Circle2D (executionContext);
}

void
Circle2D::initialize ()
{
	X3DGeometryNode::initialize ();

	getBrowser () -> getCircle2DOptions () .addInterest (this, &Circle2D::update);
}

void
Circle2D::setExecutionContext (X3DExecutionContext* const executionContext)
throw (Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	getBrowser () -> getCircle2DOptions () .removeInterest (this, &Circle2D::update);

	X3DGeometryNode::setExecutionContext (executionContext);

	if (isInitialized ())
		getBrowser () -> getCircle2DOptions () .addInterest (this, &Circle2D::update);
}

Box3f
Circle2D::createBBox ()
{
	const float diameter = std::abs (radius ()) * 2;

	return Box3f (Vector3f (diameter, diameter, 0), Vector3f ());
}

void
Circle2D::build ()
{
	const Circle2DOptions* const options = getBrowser () -> getCircle2DOptions ();

	if (std::abs (radius ()) == 1.0f)
		getVertices () = options -> getVertices ();

	else
	{
		getVertices () .reserve (options -> getVertices () .size ());

		for (const auto & vertex : options -> getVertices ())
			getVertices () .emplace_back (vertex * std::abs (radius ()));
	}

	addElements (options -> getVertexMode (), getVertices () .size ());
	setSolid (false);
}

void
Circle2D::draw ()
{
	glDisable (GL_LIGHTING);
	X3DGeometryNode::draw ();
}

SFNode
Circle2D::toPrimitive () const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	if (getElements () .empty ())
		throw Error <DISPOSED> ("Circle2D::toPrimitive");

	const auto coord    = getExecutionContext () -> createNode <Coordinate> ();
	const auto geometry = getExecutionContext () -> createNode <IndexedLineSet> ();

	geometry -> metadata () = metadata ();
	geometry -> coord ()    = coord;

	for (const auto & vertex : getVertices ())
		coord -> point () .emplace_back (vertex);

	for (int32_t i = 0, size = getVertices () .size (); i < size; ++ i)
		geometry -> coordIndex () .emplace_back (i);

	geometry -> coordIndex () .emplace_back (0);
	geometry -> coordIndex () .emplace_back (-1);

	getExecutionContext () -> realize ();
	return SFNode (geometry);
}

} // X3D
} // titania
