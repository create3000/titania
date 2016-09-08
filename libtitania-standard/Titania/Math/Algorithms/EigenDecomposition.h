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

#ifndef __TITANIA_MATH_ALGORITHMS_EIGEN_DECOMPOSITION_H__
#define __TITANIA_MATH_ALGORITHMS_EIGEN_DECOMPOSITION_H__

#include <cmath>

namespace titania {
namespace math {

// This algorithm is taken from SGI's OpenInventor Matrix4 implementation.
// The original name of this function is 'jacobi'.

template <class MatrixType, size_t JacobiRank = MatrixType::order ()>
void
eigen_decomposition (MatrixType matrix,
                     typename MatrixType::value_type evalues [JacobiRank],
                     typename MatrixType::value_type evectors [JacobiRank] [JacobiRank])
{
	typedef typename MatrixType::value_type value_type;

	value_type sm;             // smallest entry
	value_type theta;          // angle for Jacobi rotation
	value_type c, s, t;        // cosine, sine, tangent of theta
	value_type tau;            // sine / (1 + cos)
	value_type h, g;           // two scrap values
	value_type thresh;         // threshold below which no rotation done
	value_type b [JacobiRank]; // more scratch
	value_type z [JacobiRank]; // more scratch
	value_type a [JacobiRank] [JacobiRank];

	// initializations
	for (size_t i = 0; i < JacobiRank; i ++)
	{
		b [i] = evalues [i] = matrix [i] [i];
		z [i] = 0;

		for (size_t j = 0; j < JacobiRank; j ++)
		{
			evectors [i] [j] = (i == j) ? 1 : 0;
			a [i] [j]        = matrix [i] [j];
		}
	}

	// Why 50? I don't know--it's the way the folks who wrote the
	// algorithm did it: 50 is hight enough to iterate over the
	// values. The algorithm normally returns after 3 or 4 iterations.
	for (size_t i = 0; i < 50; i ++)
	{
		sm = 0;

		for (size_t p = 0; p < JacobiRank - 1; p ++)
			for (size_t q = p + 1; q < JacobiRank; q ++)
				sm += std::abs (a [p] [q]);

		if (sm == 0)
			return;

		thresh = i < 3 ?
		         sm / (JacobiRank * JacobiRank) / 5:
		         0;

		for (size_t p = 0; p < JacobiRank - 1; p ++)
		{
			for (size_t q = p + 1; q < JacobiRank; q ++)
			{
				g = 100 * std::abs (a [p] [q]);

				if (i > 3 and (std::abs (evalues [p]) + g == std::abs (evalues [p])) and (std::abs (evalues [q]) + g == std::abs (evalues [q])))
					a [p] [q] = 0;

				else if (std::abs (a [p] [q]) > thresh)
				{
					h = evalues [q] - evalues [p];

					if (std::abs (h) + g == std::abs (h))
						t = a [p] [q] / h;
					else
					{
						theta = h / a [p] [q] / 2;
						t     = 1 / (std::abs (theta) + std::sqrt (1 + theta * theta));

						if (theta < 0)
							t = -t;
					}

					// End of computing tangent of rotation angle

					c            = 1 / std::sqrt (1 + t * t);
					s            = t * c;
					tau          = s / (1 + c);
					h            = t * a [p] [q];
					z [p]       -= h;
					z [q]       += h;
					evalues [p] -= h;
					evalues [q] += h;
					a [p] [q]    = 0;

					for (size_t j = 0; j < p; j ++)
					{
						g         = a [j] [p];
						h         = a [j] [q];
						a [j] [p] = g - s * (h + g * tau);
						a [j] [q] = h + s * (g - h * tau);
					}

					for (size_t j = p + 1; j < q; j ++)
					{
						g         = a [p] [j];
						h         = a [j] [q];
						a [p] [j] = g - s * (h + g * tau);
						a [j] [q] = h + s * (g - h * tau);
					}

					for (size_t j = q + 1; j < JacobiRank; j ++)
					{
						g         = a [p] [j];
						h         = a [q] [j];
						a [p] [j] = g - s * (h + g * tau);
						a [q] [j] = h + s * (g - h * tau);
					}

					for (size_t j = 0; j < JacobiRank; j ++)
					{
						g                = evectors [j] [p];
						h                = evectors [j] [q];
						evectors [j] [p] = g - s * (h + g * tau);
						evectors [j] [q] = h + s * (g - h * tau);
					}
				}
			}
		}

		for (size_t p = 0; p < JacobiRank; p ++)
		{
			evalues [p] = b [p] += z [p];
			z [p]       = 0;
		}
	}
}

} // math
} // titania

#endif
