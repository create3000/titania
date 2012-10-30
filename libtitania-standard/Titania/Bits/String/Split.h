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
 ******************************************************************************/

#ifndef __TITANIA_BITS_STRING_SPLIT_H__
#define __TITANIA_BITS_STRING_SPLIT_H__

#include <deque>
#include <string>

namespace titania {
namespace basic {

template <class StringT, template <class, class Allocator = std::allocator <StringT>> class ArrayType = std::deque>
ArrayType <StringT>
basic_split (const StringT & string, const StringT & delimiter)
{
	ArrayType <StringT> array;

	typename StringT::size_type first = 0;
	typename StringT::size_type last  = string .find (delimiter, first);

	while (last not_eq StringT::npos)
	{
		array .emplace_back (string .substr (first, last - first));

		first = last + delimiter .length ();
		last  = string .find (delimiter, first);
	}

	array .emplace_back (string .substr (first, string .length () - first));

	return array;
}

inline
std::deque <std::string>
split (const std::string & string, const std::string & delimiter)
{
	return basic_split <std::string> (string, delimiter);
}

inline
std::deque <std::wstring>
wsplit (const std::wstring & string, const std::wstring & delimiter)
{
	return basic_split <std::wstring> (string, delimiter);
}

extern template
std::deque <std::string>
basic_split (const std::string &, const std::string  &);

extern template
std::deque <std::wstring>
basic_split (const std::wstring &, const std::wstring  &);

} // basic
} // titania

#endif
