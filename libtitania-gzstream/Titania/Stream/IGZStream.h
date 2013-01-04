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

#ifndef __TITANIA_STREAM_IGZSTREAM_H__
#define __TITANIA_STREAM_IGZSTREAM_H__

#include "GZStreamBuf.h"
#include <istream>

namespace titania {
namespace basic {

/**
 *  Template to represent basic gzstream handling.
 *
 *  Extern instantiations for char and wchar_t are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  CharT   Type of ... values.
 *  @param  Traits  Traits ...
 */

template <class CharT,
          class Traits = std::char_traits <CharT>>
class basic_igzstream :
	public std::basic_istream <CharT, Traits>
{
public:

	using std::basic_istream <CharT, Traits>::rdbuf;

	/// @name Constructors

	basic_igzstream (basic_igzstream &&);

	basic_igzstream (const std::basic_istream <CharT, Traits> &);

	/// @name Destructor

	virtual
	~basic_igzstream ();


private:

	void
	close ();

};

template <class CharT, class Traits>
basic_igzstream <CharT, Traits>::basic_igzstream (basic_igzstream <CharT, Traits>&& gzstream) :
	std::basic_istream <CharT, Traits> (gzstream .rdbuf (NULL))
{ }

template <class CharT, class Traits>
basic_igzstream <CharT, Traits>::basic_igzstream (const std::basic_istream <CharT, Traits> & istream) :
	std::basic_istream <CharT, Traits> (new basic_gzstreambuf <CharT, Traits> (istream .rdbuf ()))
{ }

template <class CharT, class Traits>
void
basic_igzstream <CharT, Traits>::close ()
{
	if (rdbuf ())
		delete rdbuf ();
}

template <class CharT, class Traits>
basic_igzstream <CharT, Traits>::~basic_igzstream ()
{
	close ();
}

typedef basic_igzstream <char> igzstream;

extern template class basic_igzstream <char>;

template <class CharT,
          class Traits = std::char_traits <CharT>>
inline
basic_igzstream <CharT, Traits>
operator >> (std::basic_istream <CharT, Traits> & istream,
             basic_igzstream <CharT, Traits> (* pf) (std::basic_istream <CharT, Traits> &))
{
	return pf (istream);
}

template <class CharT,
          class Traits = std::char_traits <CharT>>
inline
basic_igzstream <CharT, Traits>
gunzip (std::basic_istream <CharT, Traits> & istream)
{
	return igzstream (istream);
}

} // basic
} // titania

#endif
