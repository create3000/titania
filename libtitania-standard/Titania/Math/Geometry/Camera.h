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

#ifndef __TITANIA_MATH_GEOMETRY_CAMERA_H__
#define __TITANIA_MATH_GEOMETRY_CAMERA_H__

#include "../Geometry/Box3.h"
#include "../Numbers/Matrix4.h"

namespace titania {
namespace math {

template <class Type>
class camera
{
public:

	///  Creates an perspective projection matrix.
	///  @param left      Specify the coordinates for the left vertical clipping plane.
	///  @param right     Specify the coordinates for the left vertical clipping plane.
	///  @param bottom    Specify the coordinates for the bottom horizontal clipping plane.
	///  @param top       Specify the coordinates for the bottom horizontal clipping plane.
	///  @param nearVal   Specify the distances to the nearer depth clipping plane.
	///                   This value must be positive.
	///  @param farVal    Specify the distances to the nearer depth clipping plane.
	///                   This value must be positive.
	static
	matrix4 <Type>
	frustum (const Type & left, const Type & right, const Type & bottom, const Type & top, const Type & nearVal, const Type & farVal)
	{
		const auto r_l = right - left;
		const auto t_b = top - bottom;
		const auto f_n = farVal - nearVal;
		const auto n_2 = 2 * nearVal;

		const auto A = (right + left) / r_l;
		const auto B = (top + bottom) / t_b;
		const auto C = -(farVal + nearVal) / f_n;
		const auto D = -n_2 * farVal / f_n;
		const auto E = n_2 / r_l;
		const auto F = n_2 / t_b;

		return matrix4 <Type> (E, 0, 0, 0,
		                       0, F, 0, 0,
		                       A, B, C, -1,
		                       0, 0, D, 0);
	}

	///  Creates an perspective projection matrix.
	///  @param fieldOfView   Specify the field of view angle.
	///  @param nearVal       Specify the distances to the nearer depth clipping plane.
	///                       This value must be positive.
	///  @param width         Specify the width of the current viewport.
	///  @param height        Specify the height of the current viewport.
	static
	matrix4 <Type>
	perspective (const Type & fieldOfView, const Type & nearVal, const Type & farVal, const Type & width, const Type & height)
	{
		const auto ratio = std::tan (fieldOfView / 2) * nearVal;

		if (width > height)
		{
			const auto aspect = width * ratio / height;
			return frustum (-aspect, aspect, -ratio, ratio, nearVal, farVal);
		}
		else
		{
			const auto aspect = height * ratio / width;
			return frustum (-ratio, ratio, -aspect, aspect, nearVal, farVal);
		}
	}

	///  Creates an perspective projection matrix.
	///  @param fieldOfView   Specify the field of view angle for both width and height.
	///  @param nearVal       Specify the distances to the nearer depth clipping plane.
	///                       This value must be positive.
	///  @param width         Specify the width of the current viewport.
	///  @param height        Specify the height of the current viewport.
	static
	matrix4 <Type>
	perspective2 (const Type & fieldOfView, const Type & nearVal, const Type & farVal, const Type & width, const Type & height)
	{
		const auto ratio = std::tan (fieldOfView / 2) * nearVal;

		return frustum (-ratio, ratio, -ratio, ratio, nearVal, farVal);
	}

	///  Creates an othographic projection matrix.
	///  @param left      Specify the coordinates for the left vertical clipping plane.
	///  @param right     Specify the coordinates for the left vertical clipping plane.
	///  @param bottom    Specify the coordinates for the bottom horizontal clipping plane.
	///  @param top       Specify the coordinates for the bottom horizontal clipping plane.
	///  @param nearVal   Specify the distances to the nearer depth clipping plane.
	///                   This value is negative if the plane is to be behind the viewer.
	///  @param farVal    Specify the distances to the nearer depth clipping plane.
	///                   This value is negative if the plane is to be behind the viewer.
	static
	matrix4 <Type>
	ortho (const Type & left, const Type & right, const Type & bottom, const Type & top, const Type & nearVal, const Type & farVal)
	{
		const auto r_l = right - left;
		const auto t_b = top - bottom;
		const auto f_n = farVal - nearVal;

		const auto A =  2 / r_l;
		const auto B =  2 / t_b;
		const auto C = -2 / f_n;
		const auto D = -(right + left) / r_l;
		const auto E = -(top + bottom) / t_b;
		const auto F = -(farVal + nearVal) / f_n;

		return matrix4 <Type> (A, 0, 0, 0,
		                       0, B, 0, 0,
		                       0, 0, C, 0,
		                       D, E, F, 1);
	}

	///  Creates an othographic projection matrix that has the size of @a box.
	static
	matrix4 <Type>
	ortho (const box3 <Type> & box)
	{
		const auto   extents = box .extents ();
		const auto & min     = extents .first;
		const auto & max     = extents .second;

		return ortho (min .x (), max .x (), min .y (), max .y (), -max .z (), -min .z ());
	}
};

} // math
} // titania

#endif
