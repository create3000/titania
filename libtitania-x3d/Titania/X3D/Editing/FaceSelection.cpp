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

#include "FaceSelection.h"

namespace titania {
namespace X3D {

FaceSelection::FaceSelection ()
{ }

void
FaceSelection::setGeometry (const X3D::X3DPtr <X3D::IndexedFaceSet> & value)
{
	geometry = value;
	
	if (not geometry)
		return;

	faceIndex .clear ();

	size_t face   = 0;
	size_t vertex = 0;

	for (const int32_t index : geometry -> coordIndex ())
	{
		if (index < 0)
		{
			face  += vertex + 1;
			vertex = 0;
			continue;
		}

		faceIndex .emplace (index, std::make_pair (face, vertex));

		++ vertex;
	}
}

std::set <size_t>
FaceSelection::getFaces () const
{
	std::set <size_t> faces;

	if (not geometry)
		return faces;

	size_t face   = 0;
	size_t vertex = 0;

	for (const int32_t index : geometry -> coordIndex ())
	{
		if (index < 0)
		{
			if (vertex < 3)
				faces .erase (face);

			face  += vertex + 1;
			vertex = 0;
			continue;
		}

		if (vertex == 0)
			faces .emplace (face);

		++ vertex;
	}

	return faces;
}

void
FaceSelection::setCoord (const X3D::X3DPtr <X3D::X3DCoordinateNode> & value)
{
	coord = value;
	
	if (not coord)
		return;

	pointIndex .clear ();

	for (size_t i = 0, size = coord -> getSize (); i < size; ++ i)
		pointIndex .emplace (coord -> get1Point (i), i);
}

void
FaceSelection::createIndices (const X3D::Vector3f & hitPoint, const X3D::MFVec3f & hitTriangle)
{
	const std::array <float, 3> distances = {
		math::abs (hitPoint - hitTriangle [0]),
		math::abs (hitPoint - hitTriangle [1]),
		math::abs (hitPoint - hitTriangle [2])
	};

	const auto iter  = std::min_element (distances .begin (), distances .end ());
	const auto index = iter - distances .begin ();
	const auto point = hitTriangle [index] .getValue ();
	
	indices .clear ();

	for (const auto & index : pointIndex .equal_range (point))
		indices .emplace_back (index .second);
}

void
FaceSelection::setHitPoint (const Vector3d & point)
{
	indices .clear ();

	for (const auto & index : pointIndex .equal_range (point))
		indices .emplace_back (index .second);

	createFaces (point);
}

void
FaceSelection::setHitPoint (const Vector3d & hitPoint, const MFVec3f & hitTriangle)
{
	createIndices (hitPoint, hitTriangle);
	createFaces (hitPoint);
}

void
FaceSelection::createFaces (const Vector3d & hitPoint)
{
	faces .clear ();

	for (const auto & index : indices)
	{
		const auto range = faceIndex .equal_range (index);

		for (const auto & face : range)
			faces .emplace_back (face .second);
	}

	if (faces .empty ())
		return;

	// Get distances of faces to hitPoint.
	
	double minDistance = std::numeric_limits <double>::infinity ();
	size_t minIndex    = 0;
	size_t i           = 0;

	for (const auto & face : faces)
	{
		const auto vertices = getVertices (face .first);

		if (vertices .size () < 3)
		{
			++ i;
			continue;
		}

		const auto vertex   = face .second;
		const auto i1       = geometry -> coordIndex () [vertices [vertex == 0 ? vertices .size () - 1 : vertex - 1]];
		const auto i2       = geometry -> coordIndex () [vertices [vertex]];
		const auto i3       = geometry -> coordIndex () [vertices [(vertex + 1) % vertices .size ()]];
		const auto p1       = coord -> get1Point (i1);
		const auto p2       = coord -> get1Point (i2);
		const auto p3       = coord -> get1Point (i3);
		const auto distance = triangle_distance_to_point (p1, p2, p3, hitPoint);
		
		if (distance < minDistance)
		{
			minDistance = distance;
			minIndex    = i;
		}

		++ i;
	}

	face = faces [minIndex];
}

std::vector <size_t>
FaceSelection::getVertices (const size_t face) const
{
	std::vector <size_t> vertices;

	for (size_t i = face, size = geometry -> coordIndex () .size (); i < size; ++ i)
	{
		const auto index = geometry -> coordIndex () [i];

		if (index < 0)
			break;

		vertices .emplace_back (i);
	}

	return vertices;
}

FaceSelection::~FaceSelection ()
{ }

} // X3D
} // titania
