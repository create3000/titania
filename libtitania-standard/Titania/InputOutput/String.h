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

#ifndef __TITANIA_INPUT_OUTPUT_STRING_H__
#define __TITANIA_INPUT_OUTPUT_STRING_H__

#include <istream>
#include <string>

#include "../LOG.h"

namespace titania {
namespace io {

static constexpr uint32_t CASE_INSENSITIVE = 0b0001;

/**
 *  Template to represent a string match agains a std::basic_istream.
 *
 *  Extern instantiations for char and wchar are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  CharT   Type of characters.
 *  @param  Traits  Character traits
 */
template <class CharT, class Traits = std::char_traits <CharT>>
class basic_string
{
public:

	///  @name Construction

	///  Constructs the io::basic_string from @a string.  A std::basic_istream can then be tested against the string.
	basic_string (const std::basic_string <CharT> & string, const uint32_t flags = 0);

	///  @name Member access

	///  Returns the match string.
	const std::basic_string <CharT> &
	operator () () const
	{ return value; }

	///  @name Operations

	///  Test whether the next characters in @a istream are the match string. Returns true on success and changes
	///  @a istreams position to the next character after the match string otherwise it returns false and doesn't
	///  changes the streams current position.
	bool
	operator () (std::basic_istream <CharT, Traits> & istream) const;

	///  Test whether the next characters in @a istream are the match string. Returns true on success otherwise false.
	///  Does not change the streams current position.
	bool
	lookahead (std::basic_istream <CharT, Traits> & istream) const;

	///  Rewinds @a istream as many characters as the match string.  Returns true on success otherwise false.
	bool
	rewind (std::basic_istream <CharT, Traits> & istream) const;


private:

	using int_type = typename std::basic_istream <CharT, Traits>::int_type;

	const std::basic_string <CharT> value;
	const size_t                    size;
	const uint32_t                  flags;

};

template <class CharT, class Traits>
inline
basic_string <CharT, Traits>::basic_string (const std::basic_string <CharT> & value, const uint32_t flags) :
	value (value),
	 size (value .size ()),
	flags (flags)
{ }

template <class CharT, class Traits>
bool
basic_string <CharT, Traits>::operator () (std::basic_istream <CharT, Traits> & istream) const
{
	const auto state = istream .rdstate ();

	for (size_t count = 0; count < size; ++ count)
	{
		auto peek      = istream .peek ();
		auto character = (int_type) value [count];

		if (flags & CASE_INSENSITIVE)
		{
			peek      = std::tolower (peek);
			character = std::tolower (character);
		}

		if (peek == character)
			istream .get ();

		else
		{
			if (count)
			{
				istream .clear (state);

				for (size_t i = 0; i < count; ++ i)
					istream .unget ();
			}
	
			return false;
		}
	}

	return true;
}

template <class CharT, class Traits>
bool
basic_string <CharT, Traits>::lookahead (std::basic_istream <CharT, Traits> & istream) const
{
	const auto state = istream .rdstate ();

	if (operator () (istream))
	{
		istream .clear (state);

		for (size_t i = 0; i < size; ++ i)
			istream .unget ();

		return true;
	}

	return false;
}

template <class CharT, class Traits>
inline
bool
basic_string <CharT, Traits>::rewind (std::basic_istream <CharT, Traits> & istream) const
{
	for (size_t i = 0; i < size; ++ i)
		istream .unget ();

	return static_cast <bool> (istream);
}

using string  = basic_string <char>;
using wstring = basic_string <wchar_t>;

extern template class basic_string <char>;
extern template class basic_string <wchar_t>;

} // io
} // titania

#endif
