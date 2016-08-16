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
 * Titania is free so
std::pair <std::vector <uint32_t>, std::vector <vector3 <double>>
mesh_union (const std::vector <uint32_t> & indices1,
            const std::vector <vector3 <double>> & points1,
            const std::vector <uint32_t> & indices2,
            const std::vector <vector3 <double>> & points2);
ftware: you can redistribute it and/or modify it under the
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

#include "Combine.h"

#include "../../External/Cork/cork.h"

#include <functional>

#include <Titania/LOG.h>

namespace titania {
namespace math {

using BooleanOperation = std::function <void (CorkTriMesh, CorkTriMesh, CorkTriMesh *)>;

mesh <double>
mesh_boolean (const mesh <double> & mesh1, const mesh <double> & mesh2, const BooleanOperation & booleanOperation)
{
	auto indices = std::vector <uint32_t> ();
	auto points  = std::vector <vector3 <double>> ();

	CorkTriMesh corkMesh1;
	CorkTriMesh corkMesh2;
	CorkTriMesh result;

	corkMesh1 .n_triangles = mesh1 .first .size () / 3;
	corkMesh1 .n_vertices  = mesh1 .second .size ();
	corkMesh1 .triangles   = const_cast <uint32_t*> (mesh1 .first .data ());
	corkMesh1 .vertices    = const_cast <double*> (mesh1 .second [0] .data ());

	corkMesh2 .n_triangles = mesh2 .first .size () / 3;
	corkMesh2 .n_vertices  = mesh2 .second .size ();
	corkMesh2 .triangles   = const_cast <uint32_t*> (mesh2 .first .data ());
	corkMesh2 .vertices    = const_cast <double*> (mesh2 .second [0] .data ());

	booleanOperation (corkMesh1, corkMesh2, &result);

	indices .assign (result .triangles, result .triangles + result .n_triangles * 3);

	for (size_t i = 0, size = result .n_vertices * 3; i < size; i += 3)
	{
		points .emplace_back (result .vertices [i + 0],
		                      result .vertices [i + 1],
		                      result .vertices [i + 2]);
	}

	freeCorkTriMesh (&result);

	return std::make_pair (std::move (indices), std::move (points));
}

bool
mesh_is_solid (const mesh <double> & mesh)
{
	CorkTriMesh corkMesh;

	corkMesh .n_triangles = mesh .first .size () / 3;
	corkMesh .n_vertices  = mesh .second .size ();
	corkMesh .triangles   = const_cast <uint32_t*> (mesh .first .data ());
	corkMesh .vertices    = const_cast <double*> (mesh .second [0] .data ());

	return isSolid (corkMesh);
}

mesh <double>
mesh_union (const mesh <double> & mesh1, const mesh <double> & mesh2)
{
	return mesh_boolean (mesh1, mesh2, computeUnion);
}

mesh <double>
mesh_difference (const mesh <double> & mesh1, const mesh <double> & mesh2)
{
	return mesh_boolean (mesh1, mesh2, computeDifference);
}

mesh <double>
mesh_intersection (const mesh <double> & mesh1, const mesh <double> & mesh2)
{
	return mesh_boolean (mesh1, mesh2, computeIntersection);
}

mesh <double>
mesh_symmetric_difference (const mesh <double> & mesh1, const mesh <double> & mesh2)
{
	return mesh_boolean (mesh1, mesh2, computeSymmetricDifference);
}

} // math
} // titania
