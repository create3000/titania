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

#include "TriangleSet2D.h"

#include "../../Execution/X3DExecutionContext.h"

namespace titania {
namespace X3D {

const std::string TriangleSet2D::componentName  = "Geometry2D";
const std::string TriangleSet2D::typeName       = "TriangleSet2D";
const std::string TriangleSet2D::containerField = "geometry";

TriangleSet2D::Fields::Fields () :
	vertices (new MFVec2f ()),
	   solid (new SFBool (true))
{ }

TriangleSet2D::TriangleSet2D (X3DExecutionContext* const executionContext) :
	    X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometryNode (),
	         fields ()
{
	addField (inputOutput,    "metadata", metadata ());
	addField (inputOutput,    "vertices", vertices ());
	addField (initializeOnly, "solid",    solid ());
}

X3DBaseNode*
TriangleSet2D::create (X3DExecutionContext* const executionContext) const
{
	return new TriangleSet2D (executionContext);
}

void
TriangleSet2D::build ()
{
	size_t elements = solid () ? 1 : 2;
	size_t reserve  = elements * vertices () .size ();

	getTexCoord () .reserve (reserve);
	getNormals  () .reserve (reserve);
	getVertices () .reserve (reserve);

	for (const auto & vertex : vertices ())
	{
		getNormals  () .emplace_back (0, 0, 1);
		getVertices () .emplace_back (vertex .getX (), vertex .getY (), 0);
	}

	size_t resize = vertices () .size () - (vertices () .size () % 3);

	getNormals  () .resize (resize);
	getVertices () .resize (resize);

	buildTexCoord ();

	addElements (GL_TRIANGLES, getVertices () .size ());
	setSolid (true);

	if (not solid ())
		addMirrorVertices (GL_TRIANGLES, true);
}

void
TriangleSet2D::buildTexCoord ()
{
	getTexCoord () .reserve (getVertices () .size ());

	Vector3f min;
	float    Ssize;
	int      Sindex, Tindex;

	getTexCoordParams (min, Ssize, Sindex, Tindex);

	for (const auto & point : getVertices ())
	{
		getTexCoord () .emplace_back ((point [0] - min [0]) / Ssize,
		                              (point [1] - min [1]) / Ssize,
		                              0,
		                              1);
	}
}

} // X3D
} // titania
