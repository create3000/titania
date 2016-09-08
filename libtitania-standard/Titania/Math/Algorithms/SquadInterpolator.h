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

#ifndef __TITANIA_MATH_ALGORITHMS_SQUAD_INTERPOLATOR_H__
#define __TITANIA_MATH_ALGORITHMS_SQUAD_INTERPOLATOR_H__

#include "../Numbers/Rotation4.h"

#include <cmath>
#include <vector>

namespace titania {
namespace math {

template <class Type, class Scalar>
class squad_interpolator
{
public:

	///  @name Construction

	squad_interpolator () :
		s ()
	{ }

	template <class Key, class KeyValue>
	squad_interpolator (const bool closed,
                       const Key & key,
                       const KeyValue & keyValue) :
      squad_interpolator ()
	{ generate (closed, key, keyValue); }

	///  @name Operations

	template <class Key, class KeyValue>
	void
	generate (const bool, const Key &, const KeyValue &);

	template <class KeyValue>
	Type
	interpolate (const size_t, const size_t, const Scalar &, const KeyValue &) const;


private:

	///  @name Memebers

	std::vector <Type> s;

};

template <class Type, class Scalar>
template <class Key, class KeyValue>
void
squad_interpolator <Type, Scalar>::generate (const bool closed,
                                             const Key & key,
                                             const KeyValue & keyValue)
{
	s .clear ();
	s. reserve (key .size ());

	if (key .size () > 1)
	{
		if (closed)
		{
			s .emplace_back (spline <Scalar> (keyValue [key .size () - 2],
			                                  keyValue [0],
			                                  keyValue [1]));
		}
		else
		{
			s .emplace_back (keyValue .front ());
		}

		for (size_t i = 1, size = key .size () - 1; i < size; ++ i)
		{
			s .emplace_back (spline <Scalar> (keyValue [i - 1],
			                                  keyValue [i],
			                                  keyValue [i + 1]));
		}

		if (closed)
		{
			s .emplace_back (spline <Scalar> (keyValue [key .size () - 2],
			                                  keyValue [key .size () - 1],
			                                  keyValue [1]));
		}
		else
		{
			s .emplace_back (keyValue [key .size () - 1]);
		}
	}
	else if (key .size () > 0)
		s .emplace_back (keyValue .front ());
}

template <class Type, class Scalar>
template <class KeyValue>
Type
squad_interpolator <Type, Scalar>::interpolate (const size_t index0,
                                                const size_t index1,
                                                const Scalar & weight,
                                                const KeyValue & keyValue) const
{
	return squad <Scalar> (keyValue [index0], s [index0], s [index1], keyValue [index1], weight);
}

} // math
} // titania

#endif
