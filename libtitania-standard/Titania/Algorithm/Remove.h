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

#ifndef __TITANIA_ALGORITHM_REMOVE_H__
#define __TITANIA_ALGORITHM_REMOVE_H__

#include "../Utility/Range.h"
#include "../Utility/Value.h"

#include <cstddef>
#include <set>
#include <utility>

namespace titania {
namespace basic {

/***
 *  Removes all elements in the range [@a rfirst, @a rlast) from the list [@a first, @a last).
 *  The Elements in the range can be in any order. The order in the list is preserved
 *
 *  Return value:
 *    Iterator to the new end of the range.
 */

template <class ForwardIterator>
ForwardIterator
remove (ForwardIterator first, ForwardIterator last, const std::set <typename ForwardIterator::value_type> & range)
{
	if (range .empty ())
		return last;

	size_t count = 0;

	for (; first not_eq last; ++ first)
	{
		if (range .count (*first))
		{
			++ count;
			break;
		}
	}

	for (; ;)
	{
		auto second = first + count;

		for (; second not_eq last; ++ first, ++ second)
		{
			if (range .count (*second))
			{
				++ count;
				goto LOOP;
			}

			*first = std::move (*second);
		}

		break;

LOOP:;
	}

	for (auto second = first + count; second not_eq last; ++ first, ++ second)
	{
		*first = std::move (*second);
	}

	return first;
}

template <class ForwardIterator, class RangeIterator>
ForwardIterator
remove (ForwardIterator first, ForwardIterator last, const RangeIterator & rfirst, const RangeIterator & rlast)
{
	std::set <typename ForwardIterator::value_type> range;

	for (const auto & element : std::pair (rfirst, rlast))
		range .emplace (value (element));

	return remove (first, last, range);
}

///***
// *  Removes all elements in the range [@a rfirst, @a rlast) from the list [@a first, @a last).
// *  The Elements in the range must be ordered as they can be found in the list.
// *
// *  Return value:
// *    Iterator to the new end of the range.
// ***/
//
//template <class ForwardIterator, class RangeIterator>
//ForwardIterator
//remove_ordered (ForwardIterator first, ForwardIterator last, RangeIterator rfirst, RangeIterator rlast)
//{
//	if (rfirst == rlast)
//		return last;
//
//	size_t count = 0;
//
//	for (; first not_eq last; ++ first)
//	{
//		if (*first == value (*rfirst))
//		{
//			++ count;
//			break;
//		}
//	}
//
//	while (++ rfirst not_eq rlast)
//	{
//		for (auto second = first + count; second not_eq last; ++ first, ++ second)
//		{
//			if (*second == value (*rfirst))
//				break;
//
//			*first = std::move (*second);
//		}
//
//		++ count;
//	}
//
//	for (auto second = first + count; second not_eq last; ++ first, ++ second)
//	{
//		*first = std::move (*second);
//	}
//
//	return first;
//}

} // basic
} // titania

#endif
