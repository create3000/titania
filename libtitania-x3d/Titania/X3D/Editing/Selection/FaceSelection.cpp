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

#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/X3DCoordinateNode.h"
#include "../../Rendering/Tessellator.h"

namespace titania {
namespace X3D {

const ComponentType FaceSelection::component      = ComponentType::TITANIA;
const std::string   FaceSelection::typeName       = "FaceSelection";
const std::string   FaceSelection::containerField = "selection";

FaceSelection::Fields::Fields () :
	geometry (new SFNode ())
{ }

FaceSelection::FaceSelection (X3DExecutionContext* const executionContext) :
	     X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DFaceSelection (),
	          fields (),
	    geometryNode (),
	       coordNode (),
	coincidentPoints (),
	      pointIndex (),
	       faceIndex (),
	            face (),
	           faces (),
	        triangle ()
{
	addType (X3DConstants::FaceSelection);

	addField (inputOutput, "geometry", geometry ());

	addChildren (geometryNode,
                coordNode);
}

FaceSelection*
FaceSelection::create (X3DExecutionContext* const executionContext) const
{
	return new FaceSelection (executionContext);
}

void
FaceSelection::initialize ()
{
	X3DFaceSelection::initialize ();

	geometry () .addInterest (this, &FaceSelection::set_geometry);

	set_geometry (geometry ());
}

void
FaceSelection::set_geometry (const SFNode & value)
{
	if (geometryNode)
	{
		geometryNode -> coordIndex () .removeInterest (this, &FaceSelection::set_coordIndex);
		geometryNode -> getCoord ()   .removeInterest (this, &FaceSelection::set_coord);
	}

	geometryNode = value;

	if (geometryNode)
	{
		geometryNode -> coordIndex () .addInterest (this, &FaceSelection::set_coordIndex);
		geometryNode -> getCoord ()   .addInterest (this, &FaceSelection::set_coord);

		set_coordIndex (geometryNode -> coordIndex ());
		set_coord (geometryNode -> getCoord ());
	}
	else
	{
		set_coordIndex (MFInt32 ());
		set_coord (nullptr);
	}
}

void
FaceSelection::set_coordIndex (const MFInt32 & coordIndex)
{
	// Update face index

	faceIndex .clear ();

	size_t face   = 0;
	size_t vertex = 0;

	for (const int32_t index : coordIndex)
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

///  Updates the coordinate node of this selection.
void
FaceSelection::set_coord (const X3DPtr <X3DCoordinateNode> & value)
{
	if (coordNode)
		coordNode -> removeInterest (this, &FaceSelection::set_point);

	coordNode = value;
	
	if (coordNode)
		coordNode -> addInterest (this, &FaceSelection::set_point);

	set_point ();
}

///  Updates the coordinate node of this selection.
void
FaceSelection::set_point ()
{
	pointIndex .clear ();

	if (coordNode)
	{
		for (size_t i = 0, size = coordNode -> getSize (); i < size; ++ i)
			pointIndex .emplace (coordNode -> get1Point (i), i);
	}
}

///  Returns a set of all faces in geometry.
std::set <size_t>
FaceSelection::getFaces () const
{
	std::set <size_t> faces;

	if (not geometryNode)
		return faces;

	size_t face   = 0;
	size_t vertex = 0;

	for (const int32_t index : geometryNode -> coordIndex ())
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

///  Finds the all points that are equal to point, the result is an array of point indices.
void
FaceSelection::setCoincidentPoints (const Vector3d & point)
{
	coincidentPoints .clear ();

	for (const auto & index : pointIndex .equal_range (point))
		coincidentPoints .emplace_back (index .second);
}

///  Finds the all points that are equal to the nearest point to hitPoint in triangle.
void
FaceSelection::findCoincidentPoints (const Vector3d & hitPoint)
{
	coincidentPoints .clear ();

	if (pointIndex .empty ())
		return;

	auto iter = std::min_element (pointIndex .begin (),
                                 pointIndex .end (),
                                 [&hitPoint] (const PointIndex::value_type & lhs, const PointIndex::value_type & rhs)
                                 {
                                   return math::abs (hitPoint - lhs .first) < math::abs (hitPoint - rhs .first);
                                 });

	for (const auto & index : pointIndex .equal_range (iter -> first))
		coincidentPoints .emplace_back (index .second);
}

///  Finds the nearest face for hitPoint and all adjacent faces.
void
FaceSelection::setAdjacentFaces (const Vector3d & hitPoint)
{
	faces .clear ();

	for (const auto & index : coincidentPoints)
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

		if (geometryNode -> convex () or vertices .size () == 3)
		{
			for (size_t v = 0, size = vertices .size (); v < size; ++ v)
			{
				const auto i1       = geometryNode -> coordIndex () [vertices [v == 0 ? vertices .size () - 1 : v - 1]];
				const auto i2       = geometryNode -> coordIndex () [vertices [v]];
				const auto i3       = geometryNode -> coordIndex () [vertices [(v + 1) % vertices .size ()]];
				const auto p1       = coordNode -> get1Point (i1);
				const auto p2       = coordNode -> get1Point (i2);
				const auto p3       = coordNode -> get1Point (i3);
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
				tessellator .add_vertex (coordNode -> get1Point (geometryNode -> coordIndex () [vertex]), geometryNode -> coordIndex () [vertex]);
		
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

	for (size_t i = face, size = geometryNode -> coordIndex () .size (); i < size; ++ i)
	{
		const auto index = geometryNode -> coordIndex () [i];

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
	const auto point0 = coordNode -> get1Point (triangle [0]);
	const auto point1 = coordNode -> get1Point (triangle [1]);
	const auto point2 = coordNode -> get1Point (triangle [2]);

	const auto line0 = Line3d (point0, point1, math::point_type ());
	const auto line1 = Line3d (point1, point2, math::point_type ());
	const auto line2 = Line3d (point2, point0, math::point_type ());

	const std::vector <double> distances = {
		not geometryNode -> convex () or isEdge (vertices, triangle [0], triangle [1]) ? abs (hitPoint - line0 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity (),
		not geometryNode -> convex () or isEdge (vertices, triangle [1], triangle [2]) ? abs (hitPoint - line1 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity (),
		not geometryNode -> convex () or isEdge (vertices, triangle [2], triangle [0]) ? abs (hitPoint - line2 .closest_point (hitPoint)) : std::numeric_limits <double>::infinity ()
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
		if (geometryNode -> coordIndex () [vertices [i]] == index1)
		{
			if (geometryNode -> coordIndex () [vertices [(i + 1) % size]] == index2)
				return true;
		}
	}

	return false;
}

FaceSelection::~FaceSelection ()
{ }

} // X3D
} // titania
