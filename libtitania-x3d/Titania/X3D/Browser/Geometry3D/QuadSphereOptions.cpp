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

#include "QuadSphereOptions.h"

#include "../../Components/Geometry3D/Sphere.h"
#include "../../Execution/X3DExecutionContext.h"
#include <complex>

namespace titania {
namespace X3D {

const std::string QuadSphereOptions::componentName  = "Browser";
const std::string QuadSphereOptions::typeName       = "QuadSphereOptions";
const std::string QuadSphereOptions::containerField = "sphereOptions";

QuadSphereOptions::Fields::Fields () :
	uDimension (new SFInt32 (40)),
	vDimension (new SFInt32 (20))
{ }

QuadSphereOptions::QuadSphereOptions (X3DExecutionContext* const executionContext) :
	        X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSphereOptionNode (),
	             fields ()
{
	addField (inputOutput, "uDimension", uDimension ());
	addField (inputOutput, "vDimension", vDimension ());

	//addField (inputOutput, "texIndices", texIndices ());
	//addField (inputOutput, "texCoord",   texCoord ());
	//addField (inputOutput, "indices",    indices ());
	//addField (inputOutput, "points",     points ());
}

QuadSphereOptions*
QuadSphereOptions::create (X3DExecutionContext* const executionContext) const
{
	return new QuadSphereOptions (executionContext);
}

std::vector <int32_t>
QuadSphereOptions::createTexIndices ()
{
	std::vector <int32_t> texIndices;

	for (int32_t p = 0, v = 0; v < vDimension () - 1; ++ v, ++ p)
	{
		for (int32_t u = 0; u < uDimension () - 1; ++ u, ++ p)
		{
			texIndices .emplace_back (p);
			texIndices .emplace_back (p + uDimension ());
			texIndices .emplace_back (p + uDimension () + 1);
			texIndices .emplace_back (p + 1);
		}
	}

	return texIndices;
}

std::vector <Vector4f>
QuadSphereOptions::createTexCoord ()
{
	std::vector <Vector4f> texCoord;

	for (int32_t v = 0; v < vDimension (); ++ v)
	{
		float y = v / float (vDimension () - 1);

		for (int u = 0; u < uDimension () - 1; ++ u)
		{
			float x = u / float (uDimension () - 1);
			texCoord .emplace_back (x, 1 - y, 0, 1);
		}

		texCoord .emplace_back (1, 1 - y, 0, 1);
	}

	return texCoord;
}

std::vector <int32_t>
QuadSphereOptions::createCoordIndices ()
{
	std::vector <int32_t> indices;

	for (int32_t p = 0, v = 0; v < vDimension () - 1; ++ v, ++ p)
	{
		for (int32_t u = 0; u < uDimension () - 2; ++ u, ++ p)
		{
			indices .emplace_back (p);
			indices .emplace_back (p + uDimension () - 1);
			indices .emplace_back (p + uDimension ());
			indices .emplace_back (p + 1);
		}

		indices .emplace_back (p);
		indices .emplace_back (p + uDimension () - 1);
		indices .emplace_back (p + 1);
		indices .emplace_back (p - uDimension () + 2);
	}

	return indices;
}

std::vector <Vector3f>
QuadSphereOptions::createPoints ()
{
	std::vector <Vector3f> points;

	// north pole
	for (int32_t u = 0; u < uDimension () - 1; ++ u)
		points .emplace_back (0, 1, 0);

	// sphere segments
	for (int32_t v = 1; v < vDimension () - 1; ++ v)
	{
		std::complex <float> zPlane = std::polar <float> (1, -M_PI * (v / float (vDimension () - 1)));

		for (int32_t u = 0; u < uDimension () - 1; ++ u)
		{
			std::complex <float> yPlane = std::polar <float> (zPlane .imag (), (2 * M_PI) * (u / float (uDimension () - 1)));

			points .emplace_back (yPlane .imag (), zPlane .real (), yPlane .real ());
		}
	}

	// south pole
	for (int32_t u = 0; u < uDimension () - 1; ++ u)
		points .emplace_back (0, -1, 0);

	return points;
}

void
QuadSphereOptions::build ()
{
	const std::vector <int32_t>  texIndices = createTexIndices ();
	const std::vector <Vector4f> texCoord   = createTexCoord ();
	const std::vector <int32_t>  indices    = createCoordIndices ();
	const std::vector <Vector3f> points     = createPoints ();

	auto index    = indices .begin ();
	auto texIndex = texIndices .begin ();

	while (index not_eq indices .end ())
	{
		for (size_t i = 0; i < 4; ++ i, ++ index, ++ texIndex)
		{
			const auto & point = points [*index];

			getTexCoords () .emplace_back (texCoord [*texIndex]);
			getNormals   () .emplace_back (point);
			getVertices  () .emplace_back (point);
		}
	}
}

} // X3D
} // titania
