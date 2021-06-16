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

#include "Combine.h"

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Polygon_mesh_processing/stitch_borders.h>
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

#include <Titania/LOG.h>

namespace titania {
namespace math {

using ExactKernel   = CGAL::Exact_predicates_exact_constructions_kernel;
using Polyhedron    = CGAL::Polyhedron_3 <ExactKernel>;
using NefPolyhedron = CGAL::Nef_polyhedron_3 <ExactKernel>;
using HalfedgeDS    = Polyhedron::HalfedgeDS;

// A modifier creating a triangle with the incremental builder.
template <class HDS>
class PolyhedronBuilder :
	public CGAL::Modifier_base <HDS>
{
public:

	PolyhedronBuilder (const mesh3 <double> & p_mesh) :
		m_mesh (p_mesh)
	{ }

	void
	operator () (HDS & hds)
	{
		using Vertex = typename HDS::Vertex;
		using Point  = typename Vertex::Point;

		// Postcondition: hds is a valid polyhedral surface.
		CGAL::Polyhedron_incremental_builder_3 <HDS> builder (hds, true);

		builder .begin_surface (m_mesh .points () .size (), m_mesh .indices () .size () / 3);

		for (const auto & p : m_mesh .points ())
			builder .add_vertex (Point (p .x (), p .y (), p .z ()));

		for (size_t i = 0, size = m_mesh .indices () .size (); i < size; )
		{
			builder .begin_facet ();
			builder .add_vertex_to_facet (m_mesh .indices () [i ++]);
			builder .add_vertex_to_facet (m_mesh .indices () [i ++]);
			builder .add_vertex_to_facet (m_mesh .indices () [i ++]);
			builder .end_facet ();
		}

		builder .end_surface ();

		if (builder .error ())
			builder .rollback ();
	}

private:

	const mesh3 <double> & m_mesh;

};

static
Polyhedron
mesh_to_polyhedron (const mesh3 <double> & mesh)
{
	Polyhedron polyhedron;

	PolyhedronBuilder <HalfedgeDS> builder (mesh);

	polyhedron .delegate (builder);

	if (polyhedron .empty ())
		throw std::domain_error ("Couldn't create polyhedron.");

	return polyhedron;
}

static
inline
NefPolyhedron
mesh_to_nef_polyhedron (const mesh3 <double> & mesh)
{
	auto polyhedron = mesh_to_polyhedron (mesh);

	return NefPolyhedron (polyhedron);
}

static
mesh3 <double>
polyhedron_to_mesh (const Polyhedron & polyhedron)
{
	auto indices = mesh3 <double>::indices_type ();
	auto points  = mesh3 <double>::points_type ();

	for (auto vertex = polyhedron .vertices_begin (), last = polyhedron .vertices_end (); vertex not_eq last; ++ vertex)
	{
		const auto & p = vertex -> point ();

		points .emplace_back (CGAL::to_double (p .x ()), CGAL::to_double (p .y ()), CGAL::to_double (p .z ()));
	}

	for (auto facet = polyhedron .facets_begin (), last = polyhedron .facets_end (); facet not_eq last; ++ facet)
	{
		auto halfEdge = facet -> halfedge ();

		// Assume polyhedron is triangulated.
		for (size_t i = 0; i < 3; ++ i)
		{
			indices .emplace_back (std::distance (polyhedron .vertices_begin (), halfEdge -> vertex ()));

			halfEdge = halfEdge -> next ();
		}
	}

	return mesh3 <double> (std::move (indices), std::move (points));
}

static
mesh3 <double>
nef_polyhedron_to_mesh (const NefPolyhedron & nefPolyhedron)
{
	auto polyhedron = Polyhedron ();

	nefPolyhedron .convert_to_polyhedron (polyhedron);

	return polyhedron_to_mesh (polyhedron);
}

// Public functions

mesh3 <double>
mesh_union (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2)
{
	const auto nefPolyhedron1 = mesh_to_nef_polyhedron (mesh1);
	const auto nefPolyhedron2 = mesh_to_nef_polyhedron (mesh2);

	return nef_polyhedron_to_mesh (nefPolyhedron1 + nefPolyhedron2);
}

mesh3 <double>
mesh_difference (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2)
{
	const auto nefPolyhedron1 = mesh_to_nef_polyhedron (mesh1);
	const auto nefPolyhedron2 = mesh_to_nef_polyhedron (mesh2);

	return nef_polyhedron_to_mesh (nefPolyhedron1 - nefPolyhedron2);
}

mesh3 <double>
mesh_intersection (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2)
{
	const auto nefPolyhedron1 = mesh_to_nef_polyhedron (mesh1);
	const auto nefPolyhedron2 = mesh_to_nef_polyhedron (mesh2);

	return nef_polyhedron_to_mesh (nefPolyhedron1 * nefPolyhedron2);
}

mesh3 <double>
mesh_exclusion (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2)
{
	const auto nefPolyhedron1 = mesh_to_nef_polyhedron (mesh1);
	const auto nefPolyhedron2 = mesh_to_nef_polyhedron (mesh2);

	return nef_polyhedron_to_mesh (nefPolyhedron1 ^ nefPolyhedron2);
}

} // math
} // titania
