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

#include "../Rendering/Tessellator.h"

namespace titania {
namespace X3D {

FaceSelection::FaceSelection ()
{ }

///  Updates the geometry node of this selection.
void
FaceSelection::setGeometry (const X3DPtr <IndexedFaceSet> & value)
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

///  Returns a set of all faces in geometry.
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

///  Updates the coordinate node of this selection.
void
FaceSelection::setCoord (const X3DPtr <X3DCoordinateNode> & value)
{
	coord = value;
	
	if (not coord)
		return;

	pointIndex .clear ();

	for (size_t i = 0, size = coord -> getSize (); i < size; ++ i)
		pointIndex .emplace (coord -> get1Point (i), i);
}

///  Finds the all points that are equal to the nearest point to hitPoint in triangle.
void
FaceSelection::setCoincidentPoints (const Vector3d & hitPoint, const MFVec3f & hitTriangle)
{
	const std::array <double, 3> distances = {
		math::abs (hitPoint - Vector3d (hitTriangle [0] .getValue ())),
		math::abs (hitPoint - Vector3d (hitTriangle [1] .getValue ())),
		math::abs (hitPoint - Vector3d (hitTriangle [2] .getValue ()))
	};

	const auto iter  = std::min_element (distances .begin (), distances .end ());
	const auto index = iter - distances .begin ();
	const auto point = hitTriangle [index] .getValue ();

	setCoincidentPoints (point);
}

///  Finds the all points that are equal to point, the result is an array of point indices.
void
FaceSelection::setCoincidentPoints (const Vector3d & point)
{
	indices .clear ();

	for (const auto & index : pointIndex .equal_range (point))
		indices .emplace_back (index .second);
}

///  Finds the nearest face for hitPoint and all adjacent faces.
void
FaceSelection::setAdjacentFaces (const Vector3d & hitPoint)
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

		if (geometry -> convex () or vertices .size () == 3)
		{
			for (size_t v = 0, size = vertices .size (); v < size; ++ v)
			{
				const auto i1       = geometry -> coordIndex () [vertices [v == 0 ? vertices .size () - 1 : v - 1]];
				const auto i2       = geometry -> coordIndex () [vertices [v]];
				const auto i3       = geometry -> coordIndex () [vertices [(v + 1) % vertices .size ()]];
				const auto p1       = coord -> get1Point (i1);
				const auto p2       = coord -> get1Point (i2);
				const auto p3       = coord -> get1Point (i3);
				const auto distance = triangle_distance_to_point (p1, p2, p3, hitPoint);
	
				if (distance < minDistance)
				{
					minDistance = distance;
					minIndex    = i;
					triangle    = { i1, i2, i3 };
				}
			}
		}
		else
		{
			opengl::tessellator <size_t> tessellator;
		
			tessellator .begin_polygon ();
			tessellator .begin_contour ();
		
			for (const auto & vertex : vertices)
				tessellator .add_vertex (coord -> get1Point (geometry -> coordIndex () [vertex]), geometry -> coordIndex () [vertex]);
		
			tessellator .end_contour ();
			tessellator .end_polygon ();
		
			const auto triangles = tessellator .triangles ();
	
			for (size_t v = 0, size = triangles .size (); v < size; v += 3)
			{
				const auto p1       = triangles [v + 0] .point ();
				const auto p2       = triangles [v + 1] .point ();
				const auto p3       = triangles [v + 2] .point ();
				const auto distance = triangle_distance_to_point (p1, p2, p3, hitPoint);

				if (distance < minDistance)
				{
					minDistance = distance;
					minIndex    = i;
					triangle    = {
						std::get <0> (triangles [v + 0] .data ()),
						std::get <0> (triangles [v + 1] .data ()),
						std::get <0> (triangles [v + 2] .data ())
					};
				}
			}
		}

		++ i;
	}

	face = faces [minIndex];
}

///  Returns all indices to the coordIndex for this face.
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

///  Return the nearest edge for hitPoint.
FaceSelection::Edge
FaceSelection::getEdge (const std::vector <size_t> & vertices,
                        const Vector3d & hitPoint) const
{
	const auto point0 = coord -> get1Point (triangle [0]);
	const auto point1 = coord -> get1Point (triangle [1]);
	const auto point2 = coord -> get1Point (triangle [2]);

	const auto line0 = Line3d (point0, point1, math::point_type ());
	const auto line1 = Line3d (point1, point2, math::point_type ());
	const auto line2 = Line3d (point2, point0, math::point_type ());

	const std::vector <double> distances = {
		not geometry -> convex () or isEdge (vertices, triangle [0], triangle [1]) ? abs (hitPoint - line0 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity (),
		not geometry -> convex () or isEdge (vertices, triangle [1], triangle [2]) ? abs (hitPoint - line1 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity (),
		not geometry -> convex () or isEdge (vertices, triangle [2], triangle [0]) ? abs (hitPoint - line2 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity ()
	};

	const auto iter = std::min_element (distances .begin (), distances .end ());
	const auto min  = iter - distances .begin ();

	if (min == 0)
		return Edge { triangle [0], triangle [1], point0, point1, line0 };

	if (min == 1)
		return Edge { triangle [1], triangle [2], point1, point2, line1 };

	return Edge { triangle [2], triangle [0], point2, point0, line2 };
}

///  Returns true if index1 and index2 form a edge in vertices, where the vertices form a face.
bool
FaceSelection::isEdge (const std::vector <size_t> & vertices, const int32_t index1, const int32_t index2) const
{
	for (size_t i = 0, size = vertices .size (); i < size; ++ i)
	{
		if (geometry -> coordIndex () [vertices [i]] == index1)
		{
			if (geometry -> coordIndex () [vertices [(i + 1) % size]] == index2)
				return true;
		}
	}

	return false;
}

FaceSelection::~FaceSelection ()
{ }

} // X3D
} // titania
