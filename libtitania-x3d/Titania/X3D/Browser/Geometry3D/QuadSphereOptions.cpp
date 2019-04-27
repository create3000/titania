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

#include "QuadSphereOptions.h"

#include "../../Browser/X3DBrowser.h"
#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace X3D {

const Component   QuadSphereOptions::component      = Component ("Titania", 1);
const std::string QuadSphereOptions::typeName       = "QuadSphereOptions";
const std::string QuadSphereOptions::containerField = "options";

QuadSphereOptions::Fields::Fields () :
	xDimension (new SFInt32 (32)),
	yDimension (new SFInt32 (15))
{ }

QuadSphereOptions::QuadSphereOptions (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSphereOptionsNode (),
	              fields ()
{
	addType (X3DConstants::QuadSphereOptions);

	addField (inputOutput, "xDimension", xDimension ());
	addField (inputOutput, "yDimension", yDimension ());
}

QuadSphereOptions*
QuadSphereOptions::create (X3DExecutionContext* const executionContext) const
{
	return new QuadSphereOptions (executionContext);
}

std::vector <int32_t>
QuadSphereOptions::createTexCoordIndex () const
{
	std::vector <int32_t> texCoordIndices;

	int32_t p = 0;

	for (int32_t u = 0; u < xDimension (); ++ u, ++ p)
	{
		texCoordIndices .emplace_back (p);
		texCoordIndices .emplace_back (p + xDimension ());
		texCoordIndices .emplace_back (p + xDimension () + 1);
		texCoordIndices .emplace_back (p);
	}

	for (int32_t v = 1; v < yDimension () - 2; ++ v, ++ p)
	{
		for (int32_t u = 0; u < xDimension (); ++ u, ++ p)
		{
			texCoordIndices .emplace_back (p);
			texCoordIndices .emplace_back (p + xDimension () + 1);
			texCoordIndices .emplace_back (p + xDimension () + 2);
			texCoordIndices .emplace_back (p + 1);
		}
	}

	for (int32_t u = 0; u < xDimension (); ++ u, ++ p)
	{
		texCoordIndices .emplace_back (p);
		texCoordIndices .emplace_back (p + xDimension () + 1);
		texCoordIndices .emplace_back (p + xDimension () + 1);
		texCoordIndices .emplace_back (p + 1);
	}

	return texCoordIndices;
}

std::vector <Vector4f>
QuadSphereOptions::createTexCoord () const
{
	std::vector <Vector4f> texCoord;

	const auto polOffset = 1 / (2 * double (xDimension ()));

	for (int32_t u = 0; u < xDimension (); ++ u)
	{
		const double x = u / double (xDimension ()) + polOffset;
		texCoord .emplace_back (x, 1, 0, 1);
	}

	for (int32_t v = 1; v < yDimension () - 1; ++ v)
	{
		const double y = v / double (yDimension () - 1);

		for (int32_t u = 0; u < xDimension (); ++ u)
		{
			const double x = u / double (xDimension ());
			texCoord .emplace_back (x, 1 - y, 0, 1);
		}

		texCoord .emplace_back (1, 1 - y, 0, 1);
	}

	for (int32_t u = 0; u < xDimension (); ++ u)
	{
		const double x = u / double (xDimension ()) + polOffset;
		texCoord .emplace_back (x, 0, 0, 1);
	}

	return texCoord;
}

std::vector <int32_t>
QuadSphereOptions::createCoordIndex () const
{
	std::vector <int32_t> coordIndices;

	for (int32_t p = 0, v = 0; v < yDimension () - 1; ++ v, ++ p)
	{
		for (int32_t u = 0; u < xDimension () - 1; ++ u, ++ p)
		{
			coordIndices .emplace_back (p);
			coordIndices .emplace_back (p + xDimension ());
			coordIndices .emplace_back (p + xDimension () + 1);
			coordIndices .emplace_back (p + 1);
		}

		coordIndices .emplace_back (p);
		coordIndices .emplace_back (p + xDimension ());
		coordIndices .emplace_back (p + 1);
		coordIndices .emplace_back (p - xDimension () + 1);
	}

	return coordIndices;
}

std::vector <Vector3d>
QuadSphereOptions::createPoints () const
{
	std::vector <Vector3d> points;

	// north pole
	for (int32_t u = 0; u < xDimension (); ++ u)
		points .emplace_back (0, 1, 0);

	// sphere segments
	for (int32_t v = 1; v < yDimension () - 1; ++ v)
	{
		const auto zPlane = std::polar <double> (1, -pi <double> * (v / double (yDimension () - 1)));

		for (int32_t u = 0; u < xDimension (); ++ u)
		{
			const auto yPlane = std::polar <double> (zPlane .imag (), 2 * pi <double> * (u / double (xDimension ())));

			points .emplace_back (yPlane .imag (), zPlane .real (), yPlane .real ());
		}
	}

	// south pole
	for (int32_t u = 0; u < xDimension (); ++ u)
		points .emplace_back (0, -1, 0);

	return points;
}

void
QuadSphereOptions::build ()
{
	const std::vector <int32_t>  texCoordIndices = createTexCoordIndex ();
	const std::vector <Vector4f> texCoords       = createTexCoord ();
	const std::vector <int32_t>  coordIndices    = createCoordIndex ();
	const std::vector <Vector3d> points          = createPoints ();

	auto texCoordIndex = texCoordIndices .begin ();
	auto coordIndex    = coordIndices .begin ();

	for ( ; coordIndex not_eq coordIndices .end (); ++ coordIndex, ++ texCoordIndex)
	{
		const auto & point = points [*coordIndex];

		getTexCoords () .emplace_back (texCoords [*texCoordIndex]);
		getNormals   () .emplace_back (point);
		getVertices  () .emplace_back (point);
	}
}

SFNode
QuadSphereOptions::toPrimitive (X3DExecutionContext* const executionContext) const
{
	const auto xDimension_1    = xDimension ();
	const auto xDimension_2    = xDimension () - 1;
	const auto texCoordIndices = createTexCoordIndex ();
	auto       texCoords       = createTexCoord ();
	const auto coordIndices    = createCoordIndex ();
	auto       points          = createPoints ();

	points .erase (points .begin (), points .begin () + xDimension_2);
	points .erase (points .end () - xDimension_2, points .end ());

	const auto texCoord = executionContext -> createNode <TextureCoordinate> ();
	const auto coord    = executionContext -> createNode <Coordinate> ();
	const auto geometry = executionContext -> createNode <IndexedFaceSet> ();

	geometry -> creaseAngle () = pi <float>;
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	for (size_t i = 0, size = xDimension_1 * 4; i < size; i += 4)
	{
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 1]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 2]);
		geometry -> texCoordIndex () .emplace_back (-1);
	}

	for (size_t i = xDimension_1 * 4, size = texCoordIndices .size () - xDimension_1 * 4; i < size; i += 4)
	{
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 1]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 2]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 3]);
		geometry -> texCoordIndex () .emplace_back (-1);
	}

	for (size_t i = texCoordIndices .size () - xDimension_1 * 4, size = texCoordIndices .size (); i < size; i += 4)
	{
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 1]);
		geometry -> texCoordIndex () .emplace_back (texCoordIndices [i + 3]);
		geometry -> texCoordIndex () .emplace_back (-1);
	}

	for (size_t i = 0, size = xDimension_1 * 4; i < size; i += 4)
	{
		geometry -> coordIndex () .emplace_back (0);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 1] - xDimension_2);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 2] - xDimension_2);
		geometry -> coordIndex () .emplace_back (-1);
	}

	for (size_t i = xDimension_1 * 4, size = coordIndices .size () - xDimension_1 * 4; i < size; i += 4)
	{
		geometry -> coordIndex () .emplace_back (coordIndices [i]     - xDimension_2);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 1] - xDimension_2);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 2] - xDimension_2);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 3] - xDimension_2);
		geometry -> coordIndex () .emplace_back (-1);
	}

	for (size_t i = coordIndices .size () - xDimension_1 * 4, size = coordIndices .size (); i < size; i += 4)
	{
		geometry -> coordIndex () .emplace_back (coordIndices [i] - xDimension_2);
		geometry -> coordIndex () .emplace_back (points .size () - 1);
		geometry -> coordIndex () .emplace_back (coordIndices [i + 3] - xDimension_2);
		geometry -> coordIndex () .emplace_back (-1);
	}

	for (const auto & point : texCoords)
		texCoord -> point () .emplace_back (point .x (), point .y ());

	coord -> point () .assign (points .begin (), points .end ());

	return geometry;
}


} // X3D
} // titania
