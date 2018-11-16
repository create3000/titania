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

#ifndef __TITANIA_INPUT_OUTPUT_SEQUENCE_H__
#define __TITANIA_INPUT_OUTPUT_SEQUENCE_H__

#include <algorithm>
#include <istream>
#include <set>
#include <string>
#include <vector>

namespace titania {
namespace io {

/**
 *  Template to represent a character class sequence. The stream is matched against the characters provided until any
 *  other character occured or EOF. If no character was matched the sequence returns true otherwise false.
 *
 *  Specialization for CharT char is part of the library.
 *
 *  Extern instantiations for char and wchar are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  CharT   Type of characters.
 *  @param  Traits  Character traits
 */
template <class CharT, class Traits = std::char_traits <CharT>> 
class basic_sequence
{
public:

	basic_sequence (const std::basic_string <CharT> &);

	bool
	operator () (std::basic_istream <CharT, Traits> &, std::basic_string <CharT> &) const;


private:

	using int_type = typename std::basic_istream <CharT, Traits>::int_type;

	const std::set <int_type> characters;

};

template <class CharT, class Traits>
inline
basic_sequence <CharT, Traits>::basic_sequence (const std::basic_string <CharT> & string) :
	characters (string .begin (), string .end ())
{ }

template <class CharT, class Traits>
bool
basic_sequence <CharT, Traits>::operator () (std::basic_istream <CharT, Traits> & istream, std::basic_string <CharT> & string) const
{
	const auto size = string .size ();

	while (istream)
	{
		const int_type c = istream .peek ();

		if (characters .count (c))
			string .push_back (istream .get ());

		else
			break;
	}

	return string .size () not_eq size;
}

/**
 *  Template specialization for basic_sequence <char>
 */
template <>
class basic_sequence <char, std::char_traits <char>> 
{
public:

	basic_sequence (const std::basic_string <char> & value) :
		       max (*std::max_element (value .begin (), value .end ()) + 1),
		characters (max)
	{
		for (const auto & c : value)
			characters [c] = true;
	}

	bool
	operator () (std::basic_istream <char> & istream, std::basic_string <char> & string) const
	{
		const auto size = string .size ();

		while (istream)
		{
			const int_type c = istream .peek ();

			if (c > -1 and c < max and characters [c])
				string .push_back (istream .get ());

			else
				break;
		}

		return string .size () not_eq size;
	}

private:

	using int_type = typename std::basic_istream <char>::int_type;

	const int_type             max;
	mutable std::vector <bool> characters;

};

using sequence  = basic_sequence <char>;
using wsequence = basic_sequence <wchar_t>;

extern template class basic_sequence <char>;
extern template class basic_sequence <wchar_t>;

} // io
} // titania

#endif
