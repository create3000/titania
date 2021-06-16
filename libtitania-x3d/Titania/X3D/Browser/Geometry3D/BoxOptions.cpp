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

#include "BoxOptions.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

namespace titania {
namespace X3D {

const Component   BoxOptions::component      = Component ("Titania", 1);
const std::string BoxOptions::typeName       = "BoxOptions";
const std::string BoxOptions::containerField = "options";

BoxOptions::BoxOptions (X3DExecutionContext* const executionContext) :
	           X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DGeometricOptionNode ()
{
	addType (X3DConstants::BoxOptions);
}

BoxOptions*
BoxOptions::create (X3DExecutionContext* const executionContext) const
{
	return new BoxOptions (executionContext);
}

void
BoxOptions::build ()
{
	// Front Face

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (-1, -1, 1);

	// p2
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (1, -1, 1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (1, 1, 1);

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (-1, -1, 1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (1, 1, 1);

	// p4
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, 0, 1);
	getVertices  () .emplace_back (-1, 1, 1);

	// Back Face

	// p1
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, -1);
	getVertices  () .emplace_back (-1, -1, -1);

	// p2
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back  (0, 0, -1);
	getVertices  () .emplace_back (-1, 1, -1);

	// p3
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, 0, -1);
	getVertices  () .emplace_back (1, 1, -1);

	// p1
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, -1);
	getVertices  () .emplace_back (-1, -1, -1);

	// p3
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, 0, -1);
	getVertices  () .emplace_back (1, 1, -1);

	// p4
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, 0, -1);
	getVertices  () .emplace_back (1, -1, -1);

	// Top Face

	// p1
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back (-1, 1, -1);

	// p2
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back (-1, 1, 1);

	// p3
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back (1, 1, 1);

	// p1
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back (-1, 1, -1);

	// p3
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back (1, 1, 1);

	// p4
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (0, 1, 0);
	getVertices  () .emplace_back  (1, 1, -1);

	// Bottom Face

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (-1, -1, -1);

	// p2
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (1, -1, -1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (1, -1, 1);

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (-1, -1, -1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (1, -1, 1);

	// p4
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (0, -1, 0);
	getVertices  () .emplace_back (-1, -1, 1);

	// Right face

	// p1
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, -1, -1);

	// p2
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, 1, -1);

	// p3
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, 1, 1);

	// p1
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, -1, -1);

	// p3
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, 1, 1);

	// p4
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (1, 0, 0);
	getVertices  () .emplace_back (1, -1, 1);

	// Left Face

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, -1, -1);

	// p2
	getTexCoords () .emplace_back (1, 0, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, -1, 1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, 1, 1);

	// p1
	getTexCoords () .emplace_back (0, 0, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, -1, -1);

	// p3
	getTexCoords () .emplace_back (1, 1, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, 1, 1);

	// p4
	getTexCoords () .emplace_back (0, 1, 0, 1);
	getNormals   () .emplace_back (-1, 0, 0);
	getVertices  () .emplace_back (-1, 1, -1);
}

SFNode
BoxOptions::toPrimitive (X3DExecutionContext* const executionContext) const
{
	const auto texCoord = executionContext -> createNode <TextureCoordinate> ();
	const auto coord    = executionContext -> createNode <Coordinate> ();
	const auto geometry = executionContext -> createNode <IndexedFaceSet> ();

	geometry -> texCoord () = texCoord;
	geometry -> coord ()    = coord;

	// Texture Coordinates
	texCoord -> point  () .emplace_back (0, 0);
	texCoord -> point  () .emplace_back (1, 0);
	texCoord -> point  () .emplace_back (1, 1);
	texCoord -> point  () .emplace_back (0, 1);

	// Front Face
	coord -> point  () .emplace_back (-1, -1, 1);
	coord -> point  () .emplace_back (1, -1, 1);
	coord -> point  () .emplace_back (1, 1, 1);
	coord -> point  () .emplace_back (-1, 1, 1);

	// Back Face
	coord -> point  () .emplace_back (-1, -1, -1);
	coord -> point  () .emplace_back (1, -1, -1);
	coord -> point  () .emplace_back (1, 1, -1);
	coord -> point  () .emplace_back (-1, 1, -1);

	geometry -> texCoordIndex () = {
		0, 1, 2, 3, -1, // front
		0, 1, 2, 3, -1, // back
		0, 1, 2, 3, -1, // top
		0, 1, 2, 3, -1, // bottom
		0, 1, 2, 3, -1, // right
		0, 1, 2, 3, -1  // left
	};

	geometry -> coordIndex () = {
		0, 1, 2, 3, -1, // front
		5, 4, 7, 6, -1, // back
		3, 2, 6, 7, -1, // top
		4, 5, 1, 0, -1, // bottom
		4, 0, 3, 7, -1, // left
		1, 5, 6, 2, -1  // right
	};

	return geometry;
}

} // X3D
} // titania
