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

#include "IcoSphereOptions.h"

#include "../../Execution/X3DExecutionContext.h"
#include "../../Components/Geometry3D/IndexedFaceSet.h"
#include "../../Components/Rendering/Coordinate.h"
#include "../../Components/Texturing/TextureCoordinate.h"

#include <complex>

namespace titania {
namespace math {

template <class Type>
class icosahedron
{
public:

	///  @name Construction

	icosahedron (const size_t order);

	///  @name Member access

	size_t
	order () const
	{ return m_order; }
	
	const std::vector <int32_t> &
	coord_index () const
	{ return m_coord_index; }

	const std::vector <vector3 <Type>> &
	points () const
	{ return m_points; }
	
	const std::vector <int32_t> &
	tex_coord_index () const
	{ return m_tex_coord_index; }

	const std::vector <vector2 <Type>> &
	tex_points () const
	{ return m_tex_points; }

	///  @name Destruction

	~icosahedron () = default;


private:

	///  @name Operations

	void
	create_primitive ();
	
	int32_t
	add_point (const vector3 <Type> & point);

	void
	add_triangle (const int32_t i1, const int32_t i2, const int32_t i3, std::vector <int32_t> & m_coord_index);

	void
	create_triangles ();

	int32_t
	create_middle_point (const int32_t p1, const int32_t p2);

	void
	create_tex_coord ();

	int32_t
	resolve_overlap (const int32_t i0, const int32_t i1);

	///  @name Static members

	static constexpr Type pole_threshold    = 0.001;
	static constexpr Type overlap_threshold = 0.5;

	///  @name Members

	size_t                                           m_order;
	std::vector <int32_t>                            m_coord_index;
	std::vector <vector3 <Type>>                     m_points;
	std::map <std::pair <int32_t, int32_t>, int32_t> m_middle_point_index;
	std::vector <int32_t>                            m_tex_coord_index;
	std::vector <vector2 <Type>>                     m_tex_points;

};

template <class Type>
icosahedron <Type>::icosahedron (const size_t order) :
	             m_order (order),
	       m_coord_index (),
	            m_points (),
	m_middle_point_index ()
{
	create_primitive ();
	create_triangles ();
	create_tex_coord ();
}

template <class Type>
void
icosahedron <Type>::create_primitive ()
{
	// Create 12 vertices of a icosahedron

	static constexpr auto p = phi <Type>;

	add_point (vector3 <Type> (-1,  p,  0));
	add_point (vector3 <Type> ( 1,  p,  0));
	add_point (vector3 <Type> (-1, -p,  0));
	add_point (vector3 <Type> ( 1, -p,  0));

	add_point (vector3 <Type> ( 0, -1,  p));
	add_point (vector3 <Type> ( 0,  1,  p));
	add_point (vector3 <Type> ( 0, -1, -p));
	add_point (vector3 <Type> ( 0,  1, -p));

	add_point (vector3 <Type> ( p,  0, -1));
	add_point (vector3 <Type> ( p,  0,  1));
	add_point (vector3 <Type> (-p,  0, -1));
	add_point (vector3 <Type> (-p,  0,  1));

	// Rotate point thus a vertice is a pole

	if (m_order == 0)
	{
		const auto rotation = rotation4 <Type> (0, 0, 1, std::atan (1 / p)) * rotation4 <Type> (0, 1, 0, -pi <Type> / 10);

		for (auto & point : m_points)
			point = normalize (rotation .mult_vec_rot (point));
	}

	// 5 faces around point 0
	add_triangle ( 0, 11,  5, m_coord_index);
	add_triangle ( 0,  5,  1, m_coord_index);
	add_triangle ( 0,  1,  7, m_coord_index);
	add_triangle ( 0,  7, 10, m_coord_index);
	add_triangle ( 0, 10, 11, m_coord_index);

	// 5 adjacent faces
	add_triangle ( 1,  5,  9, m_coord_index);
	add_triangle ( 5, 11,  4, m_coord_index);
	add_triangle (11, 10,  2, m_coord_index);
	add_triangle (10,  7,  6, m_coord_index);
	add_triangle ( 7,  1,  8, m_coord_index);

	// 5 faces around point
	add_triangle ( 3,  9,  4, m_coord_index);
	add_triangle ( 3,  4,  2, m_coord_index);
	add_triangle ( 3,  2,  6, m_coord_index);
	add_triangle ( 3,  6,  8, m_coord_index);
	add_triangle ( 3,  8,  9, m_coord_index);

	// 5 adjacent faces
	add_triangle ( 4,  9,  5, m_coord_index);
	add_triangle ( 2,  4, 11, m_coord_index);
	add_triangle ( 6,  2, 10, m_coord_index);
	add_triangle ( 8,  6,  7, m_coord_index);
	add_triangle ( 9,  8,  1, m_coord_index);
}

template <class Type>
int32_t
icosahedron <Type>::add_point (const vector3 <Type> & point)
{
	const auto index = m_points .size ();

	m_points .emplace_back (normalize (point));

	return index;
}

template <class Type>
void
icosahedron <Type>::add_triangle (const int32_t i1, const int32_t i2, const int32_t i3, std::vector <int32_t> & m_coord_index)
{
	m_coord_index .emplace_back (i1);
	m_coord_index .emplace_back (i2);
	m_coord_index .emplace_back (i3);
}

template <class Type>
void
icosahedron <Type>::create_triangles ()
{
	auto m_coord_index2 = std::vector <int32_t> ();

	// Refine triangles
	for (size_t o = 0; o < m_order; ++ o)
	{
		m_coord_index2 .clear ();

		for (size_t i = 0, size = m_coord_index .size (); i < size; i += 3)
		{
			// Replace triangle by 4 triangles
			const auto a = create_middle_point (m_coord_index [i + 0], m_coord_index [i + 1]);
			const auto b = create_middle_point (m_coord_index [i + 1], m_coord_index [i + 2]);
			const auto c = create_middle_point (m_coord_index [i + 2], m_coord_index [i + 0]);

			add_triangle (m_coord_index [i],     a, c, m_coord_index2);
			add_triangle (m_coord_index [i + 1], b, a, m_coord_index2);
			add_triangle (m_coord_index [i + 2], c, b, m_coord_index2);
			add_triangle (a, b, c, m_coord_index2);
		}

		std::swap (m_coord_index, m_coord_index2);
	}
}

template <class Type>
int32_t
icosahedron <Type>::create_middle_point (const int32_t p1, const int32_t p2)
{
	// First check if we have it already
	const auto key  = std::minmax (p1, p2);
	const auto iter = m_middle_point_index .find (key);

	if (iter not_eq m_middle_point_index .end ())
		return iter -> second;

	// Not in cache, calculate it
	const auto & point1 = m_points [p1];
	const auto & point2 = m_points [p2];
	
	// Add middle point, makes sure point is on unit sphere
	const auto index = add_point ((point1 + point2) / Type (2));

	// Store it, return index
	m_middle_point_index .emplace (key, index);

	return index;
}

template <class Type>
void
icosahedron <Type>::create_tex_coord ()
{
	//
	// Create texture coordinates
	// Apply spherical mapping
	//

	// Copy coordIndex
	m_tex_coord_index = m_coord_index;
	
	for (const auto & point : m_points)
	{
		m_tex_points .emplace_back (std::atan2 (point .x (), point .z ()) / (2 * pi <Type>) + Type (0.5),
		                            std::asin (point .y ()) / pi <Type> + Type (0.5));
	}

	// Refine poles
	static constexpr auto north_pole_threshold = 1 - pole_threshold;
	static constexpr auto south_pole_threshold = pole_threshold;

	for (size_t i = 0, size = m_tex_coord_index .size (); i < size; i += 3)
	{
		int32_t i0 = -1;
		int32_t i1 = -1;
		int32_t i2 = -1;
		
		// Find north pole
		
		if (m_tex_points [m_tex_coord_index [i]] .y () > north_pole_threshold)
		{
			i0 = i;
			i1 = i + 1;
			i2 = i + 2;
		}
			
		else if (m_tex_points [m_tex_coord_index [i + 1]] .y () > north_pole_threshold)
		{
			i0 = i + 1;
			i1 = i;
			i2 = i + 2;
		}
		
		else if (m_tex_points [m_tex_coord_index [i + 2]] .y () > north_pole_threshold)
		{
			i0 = i + 2;
			i1 = i;
			i2 = i + 1;
		}

		// North pole found
		if (i0 > -1)
		{
			const auto index0 = m_tex_coord_index [i0]; // North pole
			const auto index1 = m_tex_coord_index [i1];
			const auto index2 = resolve_overlap (i1, i2);
		
			m_tex_coord_index [i0] = m_tex_points .size ();

			m_tex_points .emplace_back ((m_tex_points [index1] .x () + m_tex_points [index2] .x ()) / 2, m_tex_points [index0] .y ());
			continue;
		}
		
		// Find south pole

		if (m_tex_points [m_tex_coord_index [i]] .y () < south_pole_threshold)
		{
			i0 = i;
			i1 = i + 1;
			i2 = i + 2;
		}
			
		else if (m_tex_points [m_tex_coord_index [i + 1]] .y () < south_pole_threshold)
		{
			i0 = i + 1;
			i1 = i;
			i2 = i + 2;
		}
		
		else if (m_tex_points [m_tex_coord_index [i + 2]] .y () < south_pole_threshold)
		{
			i0 = i + 2;
			i1 = i;
			i2 = i + 1;
		}

		// South pole found
		if (i0 > -1)
		{
			const auto index0 = m_tex_coord_index [i0]; // South pole
			const auto index1 = m_tex_coord_index [i1];
			const auto index2 = resolve_overlap (i1, i2);

			m_tex_coord_index [i0] = m_tex_points .size ();

			m_tex_points .emplace_back ((m_tex_points [index1] .x () + m_tex_points [index2] .x ()) / 2, m_tex_points [index0] .y ());
			continue;
		}

		resolve_overlap (i, i + 1);
		resolve_overlap (i, i + 2);
	}
}

template <class Type>
int32_t
icosahedron <Type>::resolve_overlap (const int32_t i0, const int32_t i1)
{
	const auto index1   = m_tex_coord_index [i1];
	const auto distance = m_tex_points [m_tex_coord_index [i0]] .x () - m_tex_points [index1] .x ();
	
	if (distance > overlap_threshold)
	{
		m_tex_coord_index [i1] = m_tex_points .size ();

		m_tex_points .emplace_back (m_tex_points [index1] .x () + 1, m_tex_points [index1] .y ());
	}
	else if (distance < -overlap_threshold)
	{
		m_tex_coord_index [i1] = m_tex_points .size ();

		m_tex_points .emplace_back (m_tex_points [index1] .x () - 1, m_tex_points [index1] .y ());	
	}

	return m_tex_coord_index [i1];
}

} // math
} // titania

namespace titania {
namespace X3D {

const ComponentType IcoSphereOptions::component      = ComponentType::TITANIA;
const std::string   IcoSphereOptions::typeName       = "IcoSphereOptions";
const std::string   IcoSphereOptions::containerField = "sphereOptions";

IcoSphereOptions::Fields::Fields () :
	order (new SFInt32 (2))
{ }

IcoSphereOptions::IcoSphereOptions (X3DExecutionContext* const executionContext) :
	         X3DBaseNode (executionContext -> getBrowser (), executionContext),
	X3DSphereOptionsNode (),
	              fields ()
{
	addType (X3DConstants::IcoSphereOptions);

	addField (inputOutput, "order", order ());
}

IcoSphereOptions*
IcoSphereOptions::create (X3DExecutionContext* const executionContext) const
{
	return new IcoSphereOptions (executionContext);
}

void
IcoSphereOptions::build () 
{
	icosahedron <double> icoSphere (order ());

	for (const auto & index : icoSphere .tex_coord_index ())
	{
		const auto & point = icoSphere .tex_points () [index];

		getTexCoords () .emplace_back (point .x (), point .y (), 0, 1);
	}

	for (const auto & index : icoSphere .coord_index ())
	{
		const auto & point = icoSphere .points () [index];

		getNormals ()  .emplace_back (point);
		getVertices () .emplace_back (point);
	}	
}

SFNode
IcoSphereOptions::toPrimitive (X3DExecutionContext* const executionContext) const
throw (Error <NOT_SUPPORTED>,
       Error <DISPOSED>)
{
	icosahedron <double> icoSphere (order ());

	const auto texCoord = executionContext -> createNode <TextureCoordinate> ();
	const auto coord    = executionContext -> createNode <Coordinate> ();
	const auto geometry = executionContext -> createNode <IndexedFaceSet> ();

	geometry -> creaseAngle () = pi <float>;
	geometry -> texCoord ()    = texCoord;
	geometry -> coord ()       = coord;

	// Coordinates

	for (const auto & point : icoSphere .tex_points ())
		texCoord -> point () .emplace_back (point);

	for (const auto & point : icoSphere .points ())
		coord -> point () .emplace_back (point);
	
	// Indices

	for (size_t i = 0, size = icoSphere .tex_coord_index () .size (); i < size; i += 3)
	{
		geometry -> texCoordIndex () .emplace_back (icoSphere .tex_coord_index () [i + 0]);
		geometry -> texCoordIndex () .emplace_back (icoSphere .tex_coord_index () [i + 1]);
		geometry -> texCoordIndex () .emplace_back (icoSphere .tex_coord_index () [i + 2]);
		geometry -> texCoordIndex () .emplace_back (-1);
	}

	for (size_t i = 0, size = icoSphere .coord_index () .size (); i < size; i += 3)
	{
		geometry -> coordIndex () .emplace_back (icoSphere .coord_index () [i + 0]);
		geometry -> coordIndex () .emplace_back (icoSphere .coord_index () [i + 1]);
		geometry -> coordIndex () .emplace_back (icoSphere .coord_index () [i + 2]);
		geometry -> coordIndex () .emplace_back (-1);
	}

	return geometry;
}

} // X3D
} // titania
