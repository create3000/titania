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

#ifndef __TITANIA_INPUT_OUTPUT_HEX_H__
#define __TITANIA_INPUT_OUTPUT_HEX_H__

#include <istream>
#include <string>

#include "../LOG.h"

namespace titania {
namespace io {

/**
 *  Template to represent a number match agains a std::basic_istream.
 *
 *  Extern instantiations for char and wchar are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  CharT   Type of characters.
 *  @param  Traits  Character traits
 */
template <class Type, class CharT, class Traits = std::char_traits <CharT>> 
class basic_hex
{
public:

	///  @name Construction

	///  Constructs the io::basic_hex.  A std::basic_istream can then be tested against the number type.
	constexpr
	basic_hex ();

	///  @name Operations

	///  Test whether the next characters in @a istream are a number. Returns true on success and changes
	///  @a istreams position to the next character after the matched characters otherwise it returns false and doesn't
	///  changes the streams current position.
	bool
	operator () (std::basic_istream <CharT, Traits> & istream, Type & value) const;

};

template <class Type, class CharT, class Traits>
inline
constexpr
basic_hex <Type, CharT, Traits>::basic_hex ()
{ }

template <class Type, class CharT, class Traits>
bool
basic_hex <Type, CharT, Traits>::operator () (std::basic_istream <CharT, Traits> & istream, Type & value) const
{
	const auto state = istream .rdstate ();
	const auto pos   = istream .tellg ();

	if (istream >> std::hex >> value)
		return true;

	istream .clear (state);

	for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
		istream .unget ();

	return false;
}

template <class Type>
using hex = basic_hex <Type, char>;

template <class Type>
using whex = basic_hex <Type, wchar_t>;

} // io
} // titania

#endif
