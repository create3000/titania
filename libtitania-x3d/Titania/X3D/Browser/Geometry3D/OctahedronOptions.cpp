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

#include "OctahedronOptions.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

#include <Titania/Math/Primitives/Polyhedron3.h>

namespace titania {
namespace X3D {

const ComponentType OctahedronOptions::component      = ComponentType::TITANIA;
const std::string   OctahedronOptions::typeName       = "OctahedronOptions";
const std::string   OctahedronOptions::containerField = "sphereOptions";

OctahedronOptions::Fields::Fields () :
	order (new SFInt32 (2))
{ }

OctahedronOptions::OctahedronOptions (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSphereOptionsNode (),
	              fields ()
{
	addType (X3DConstants::OctahedronOptions);

	addField (inputOutput, "order", order ());
}

OctahedronOptions*
OctahedronOptions::create (X3DExecutionContext* const executionContext) const
{
	return new OctahedronOptions (executionContext);
}

void
OctahedronOptions::build () 
{
	octahedron3 <double> sphere (order ());

	for (const auto & index : sphere .tex_coord_index ())
	{
		const auto & point = sphere .tex_points () [index];

		getTexCoords () .emplace_back (point .x (), point .y (), 0, 1);
	}

	for (const auto & index : sphere .coord_index ())
	{
		const auto & point = sphere .points () [index];

		getNormals ()  .emplace_back (point);
		getVertices () .emplace_back (point);
	}	
}

SFNode
OctahedronOptions::toPrimitive (X3DExecutionContext* const executionContext) const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	octahedron3 <double> sphere (order ());

	const auto texCoord = executionContext -> createNode <TextureCoordinate> ();
	const auto coord    = executionContext -> createNode <Coordinate> ();
	const auto geometry = executionContext -> createNode <IndexedFaceSet> ();

	geometry -> creaseAngle () = pi <float>;
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	// Coordinates

	for (const auto & point : sphere .tex_points ())
		texCoord -> point () .emplace_back (point);

	for (const auto & point : sphere .points ())
		coord -> point () .emplace_back (point);
	
	// Indices

	for (size_t i = 0, size = sphere .tex_coord_index () .size (); i < size; i += 3)
	{
		geometry -> texCoordIndex () .emplace_back (sphere .tex_coord_index () [i + 0]);
		geometry -> texCoordIndex () .emplace_back (sphere .tex_coord_index () [i + 1]);
		geometry -> texCoordIndex () .emplace_back (sphere .tex_coord_index () [i + 2]);
		geometry -> texCoordIndex () .emplace_back (-1);
	}

	for (size_t i = 0, size = sphere .coord_index () .size (); i < size; i += 3)
	{
		geometry -> coordIndex () .emplace_back (sphere .coord_index () [i + 0]);
		geometry -> coordIndex () .emplace_back (sphere .coord_index () [i + 1]);
		geometry -> coordIndex () .emplace_back (sphere .coord_index () [i + 2]);
		geometry -> coordIndex () .emplace_back (-1);
	}

	return geometry;
}

} // X3D
} // titania
