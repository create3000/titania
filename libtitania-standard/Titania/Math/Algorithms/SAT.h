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

#ifndef __TITANIA_MATH_ALGORITHMS_SAT_H__
#define __TITANIA_MATH_ALGORITHMS_SAT_H__

#include "../Numbers/Vector3.h"

#include <vector>

namespace titania {
namespace math {

/**
 *  Class to represent the Separating Axis Theorem.
 */
class sat
{
public:

	///  Returns true if the object defined by @a points1 and the object defined by @a points2 are separated, otherwise
	///  false.  You must provide suitable axes for this test to operate on.  This test only gives reasonable result for
	///  convex objects.  For 2d objects it is sufficient to use the normal vectors of the edges as axes.  For 3d
	///  objects, the axes are the normal vectors of the faces of each object and the cross product of each edge from one
	///  object with each edge from the other object.  It is not needed to provide normalized axes.
	template <class Type>
	static
	bool
	separated (const std::vector <vector3 <Type>> & axes,
	           const std::vector <vector3 <Type>> & points1,
	           const std::vector <vector3 <Type>> & points2)
	{
		// http://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection

		for (const auto & axis : axes)
		{
			Type min1, max1, min2, max2;

			project (points1, axis, min1, max1);
			project (points2, axis, min2, max2);

			if (overlaps (min1, max1, min2, max2))
				continue;

			return true;
		}

		return false;
	}

private:

	///  Projects @a points to @a axis and returns the minimum and maximum bounds.
	template <class Type>
	static
	void
	project (const std::vector <vector3 <Type>> & points,
	         const vector3 <Type> & axis,
	         Type & min,
	         Type & max)
	{
		min = std::numeric_limits <Type>::infinity ();
		max = -min;

		for (const auto & point : points)
		{
			// Just dot it to get the min and max along this axis.
			// NOTE: the axis must be normalized to get accurate projections to calculate the MTV, but if it is only needed to
			// know whether it overlaps, every axis can be used.

			const Type dotVal = dot (point, axis);

			if (dotVal < min)
				min = dotVal;

			if (dotVal > max)
				max = dotVal;
		}
	}

	///  Returns true if both ranges overlap, otherwise false.
	template <class Type>
	static
	bool
	overlaps (const Type min1, const Type max1, const Type min2, const Type max2)
	{
		return is_between (min2, min1, max1) or is_between (min1, min2, max2);
	}

	///  Returns true if @a value is between @a lowerBound and @a upperBound, otherwise false.
	template <class Type>
	static
	inline
	bool
	is_between (const Type value, const Type lowerBound, const Type upperBound)
	{
		return lowerBound <= value and value <= upperBound;
	}

};

} // math
} // titania

#endif
