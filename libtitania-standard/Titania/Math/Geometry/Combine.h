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

#ifndef __TITANIA_MATH_GEOMETRY_COMBINE_H__
#define __TITANIA_MATH_GEOMETRY_COMBINE_H__

#include "../Numbers/Vector3.h"

#include <vector>
#include <cstdint>

namespace titania {
namespace math {

template <class Type>
class mesh3
{
public:

	///  @name Member types

	using indices_type = std::vector <int32_t>;
	using points_type  = std::vector <vector3 <Type>>;

	///  Default constructor. Constructs an empty mesh.
	mesh3 () = default;

	///  Copy constructor.
	mesh3 (const mesh3 & other) = default;

	///  Move constructor.
	mesh3 (mesh3 && other) = default;

	///  Copy constructor.  Constructs a mesh from @a indices and @a points.
	mesh3 (const indices_type & indices, const points_type & points) :
		m_indices (indices),
		 m_points (points)
	{ }

	///  Move constructor.  Constructs a mesh from @a indices and @a points.
	mesh3 (indices_type && indices, points_type && points) :
		m_indices (std::move (indices)),
		 m_points (std::move (points))
	{ }

	///  @name Assignment operator

	///  Assigns mesh @a other to this mesh.
	mesh3 &
	operator = (const mesh3 & other) = default;

	///  Assigns mesh @a other to this mesh with move semantics.
	mesh3 &
	operator = (mesh3 && other) = default;

	///  @name Member access

	///  Returns the indices of this mesh.
	const indices_type &
	indices () const
	{ return m_indices; }

	///  Returns the points of this mesh.
	const points_type &
	points () const
	{ return m_points; }


private:

	///  @name Members

	indices_type m_indices;
	points_type  m_points;

};

mesh3 <double>
mesh_union (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2);

mesh3 <double>
mesh_difference (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2);

mesh3 <double>
mesh_intersection (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2);

mesh3 <double>
mesh_exclusion (const mesh3 <double> & mesh1, const mesh3 <double> & mesh2);

} // math
} // titania

#endif
