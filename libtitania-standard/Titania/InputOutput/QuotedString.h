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

#ifndef __TITANIA_INPUT_OUTPUT_QUOTED_STRING_H__
#define __TITANIA_INPUT_OUTPUT_QUOTED_STRING_H__

#include <istream>
#include <string>
#include <Titania/LOG.h>

namespace titania {
namespace io {

template <class CharT, class Traits = std::char_traits <CharT>>
class basic_quoted_string
{
public:

	constexpr
	basic_quoted_string (const CharT &);

	char
	operator () () const
	{ return delimiter; }

	bool
	operator () (std::basic_istream <CharT, Traits> &, std::basic_string <CharT> &) const;


private:

	using int_type = typename std::basic_istream <CharT, Traits>::int_type;

	const int_type delimiter;

};

template <class CharT, class Traits>
inline
constexpr
basic_quoted_string <CharT, Traits>::basic_quoted_string (const CharT & delimiter) :
	delimiter (delimiter)
{ }

template <class CharT, class Traits>
bool
basic_quoted_string <CharT, Traits>::operator () (std::basic_istream <CharT, Traits> & istream, std::basic_string <CharT> & string) const
{
	if (istream .peek () == delimiter)
	{
		istream .get ();

		std::basic_string <CharT> characters;

		while (istream)
		{
			int_type c = istream .get ();

			if (c == delimiter)
			{
				string = std::move (characters);
				return true;
			}
			else if (c == '\\')
			{
				if (istream)
				{
					c = istream .get ();

					if (not (c == delimiter or c == (int_type) '\\'))
						characters .push_back ('\\');
				}
				else
					return false;
			}

			characters .push_back (c);
		}
	}

	return false;
}

using quoted_string  = basic_quoted_string <char>;
using wquoted_string = basic_quoted_string <wchar_t>;

extern template class basic_quoted_string <char>;
extern template class basic_quoted_string <wchar_t>;

} // io
} // titania

#endif
