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

#ifndef __TITANIA_MATH_GEOMETRY_CAMERA_H__
#define __TITANIA_MATH_GEOMETRY_CAMERA_H__

#include "../Geometry/Box3.h"
#include "../Numbers/Matrix4.h"

namespace titania {
namespace math {

template <class Type>
static
matrix4 <Type>
frustum (const Type & l, const Type & r, const Type & b, const Type & t, const Type & n, const Type & f)
{
	const Type r_l = r - l;
	const Type t_b = t - b;
	const Type f_n = f - n;
	const Type n_2 = 2 * n;

	const Type A = (r + l) / r_l;
	const Type B = (t + b) / t_b;
	const Type C = -(f + n) / f_n;
	const Type D = -n_2 * f / f_n;
	const Type E = n_2 / r_l;
	const Type F = n_2 / t_b;

	return matrix4 <Type> (E, 0, 0, 0,
	                       0, F, 0, 0,
	                       A, B, C, -1,
	                       0, 0, D, 0);
}

template <class Type>
matrix4 <Type>
perspective (const Type & fieldOfView, const Type & zNear, const Type & zFar, const vector4 <int> & viewport)
{
	const int width  = viewport [2];
	const int height = viewport [3];

	const Type ratio = std::tan (fieldOfView / 2) * zNear;

	if (width > height)
	{
		const Type aspect = width * ratio / height;
		return frustum (-aspect, aspect, -ratio, ratio, zNear, zFar);
	}
	else
	{
		const Type aspect = height * ratio / width;
		return frustum (-ratio, ratio, -aspect, aspect, zNear, zFar);
	}
}

template <class Type>
static
matrix4 <Type>
ortho (const Type & l, const Type & r, const Type & b, const Type & t, const Type & n, const Type & f)
{
	const Type r_l = r - l;
	const Type t_b = t - b;
	const Type f_n = f - n;

	const Type A =  2 / r_l;
	const Type B =  2 / t_b;
	const Type C = -2 / f_n;
	const Type D = -(r + l) / r_l;
	const Type E = -(t + b) / t_b;
	const Type F = -(f + n) / f_n;

	return matrix4 <Type> (A, 0, 0, 0,
	                       0, B, 0, 0,
	                       0, 0, C, 0,
	                       D, E, F, 1);
}

template <class Type>
static
matrix4 <Type>
ortho (const box3 <Type> & bbox)
{
	const auto   extents = bbox .extents ();
	const auto & min     = extents .first;
	const auto & max     = extents .second;

	return ortho (min .x (), max .x (), min .y (), max .y (), -max .z (), -min .z ());
}

} // math
} // titania

#endif
