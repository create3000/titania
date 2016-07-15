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

#ifndef __TITANIA_MATH_GEOMETRY_MINIMUM_BOUNDING_BOX_H__
#define __TITANIA_MATH_GEOMETRY_MINIMUM_BOUNDING_BOX_H__

#include "../Geometry/Box3.h"
#include "../Numbers/Vector3.h"

#include <Titania/External/ApproxMVBB/ComputeApproxMVBB.hpp>

#include <vector>

namespace titania {
namespace math {

///  @relates box3
///  @name Misc

///  Returns the minimum bounding rectangle from @a hull, where hull must be convex.
///  References: http://www-cgrl.cs.mcgill.ca/~godfried/research/calipers.html
///  https://en.wikipedia.org/wiki/Rotating_calipers
template <class Type>
box3 <Type>
minimum_bounding_box (const std::vector <vector3 <Type>> & points)
{
	Eigen::Matrix <Type, 3, Eigen::Dynamic> matrix (3, points .size ());

	for (size_t i = 0, size = points .size (); i < size; ++ i)
	{
		const auto & p = points [i];

		matrix (0, i) = p .x ();
		matrix (1, i) = p .y ();
		matrix (2, i) = p .z ();
	}

	const auto a_obb      = ApproxMVBB::approximateMVBB (matrix, 0.001, 500, 5, 0, 5);
	const auto a_rotation = a_obb .m_q_KI;
	const auto a_size     = (a_obb .m_maxPoint - a_obb .m_minPoint) / 2;
	const auto a_center   = (a_obb .m_maxPoint + a_obb .m_minPoint) / 2;

	const auto rotation = rotation4 <Type> (quaternion <Type> (a_rotation .x (), a_rotation .y (), a_rotation .z (), a_rotation .w ()));
	const auto size     = vector3 <Type> (a_size (0, 0), a_size (1, 0), a_size (2, 0));
	const auto center   = vector3 <Type> (a_center (0, 0), a_center (1, 0), a_center (2, 0));

	matrix4 <Type> obb;

	obb .set (center, rotation, size);

	return box3 <Type> (obb);
}

} // math
} // titania

#endif
