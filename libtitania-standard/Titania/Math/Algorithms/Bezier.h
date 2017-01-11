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

#ifndef __TITANIA_MATH_ALGORITHMS_BEZIER_H__
#define __TITANIA_MATH_ALGORITHMS_BEZIER_H__

namespace titania {
namespace math {

/**
 *  Class to represent bezier curve algorithms.
 */
class bezier
{
public:

	/**
	 * De Casteljau (bezier) algorithm contributed by Jed Soane,
	 * evaluates a quadratic or conic (second degree) curve.
	 */
	template <class Vector, class Container>
	static
	void
	quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, Container & points);

	/**
	 * De Casteljau (bezier) algorithm contributed by Jed Soane,
	 * evaluates a cubic (third degree) curve.
	 */
	template <class Vector, class Container>
	static
	void
	cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, Container & points);

};

template <class Vector, class Container>
void
bezier::quadratic_curve (const Vector & A, const Vector & B, const Vector & C, const size_t bezier_steps, Container & points)
{
	for (size_t i = 1; i < bezier_steps; ++ i)
	{
		const auto t = static_cast <typename Vector::value_type> (i) / bezier_steps;

		const auto U = (1 - t) * A + t * B;
		const auto V = (1 - t) * B + t * C;

		const auto P = (1 - t) * U + t * V;

		if (points .empty () or (P not_eq points .back () and P not_eq points .front ()))
		{
			points .emplace_back (P);
		}
	}
}

template <class Vector, class Container>
void
bezier::cubic_curve (const Vector & A, const Vector & B, const Vector & C, const Vector & D, const size_t bezier_steps, Container & points)
{
	for (size_t i = 0; i < bezier_steps; ++ i)
	{
		const auto t = static_cast <typename Vector::value_type> (i) / bezier_steps;

		const auto U = (1 - t) * A + t * B;
		const auto V = (1 - t) * B + t * C;
		const auto W = (1 - t) * C + t * D;

		const auto M = (1 - t) * U + t * V;
		const auto N = (1 - t) * V + t * W;

		const auto P = (1 - t) * M + t * N;

		if (points .empty () or (P not_eq points .back () and P not_eq points .front ()))
		{
			points .emplace_back (P);
		}
	}
}

} // math
} // titania

#endif
