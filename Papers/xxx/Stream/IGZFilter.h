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

#ifndef __TITANIA_STREAM_IGZFILTER_H__
#define __TITANIA_STREAM_IGZFILTER_H__

#include "GZFilterBuf.h"
#include <istream>
#include <sstream>

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
class basic_igzfilter :
	public std::basic_istream <CharT, Traits>
{
public:

	using std::basic_istream <CharT, Traits>::init;
	using std::basic_istream <CharT, Traits>::setstate;
	using std::basic_istream <CharT, Traits>::rdbuf;

	/// @name Constructors

	basic_igzfilter (basic_igzfilter &&);

	basic_igzfilter (std::basic_istream <CharT, Traits>&&);

	bool
	is_compressed ()
	{ return buf -> is_open (); }

	/// @name Destructor

	virtual
	~basic_igzfilter ();


private:

	basic_igzfilter (const basic_igzfilter &) = delete;

	basic_igzfilter &
	operator = (const basic_igzfilter &) = delete;

	void
	close ();

	basic_gzfilterbuf <CharT, Traits>* buf;

};

template <class CharT, class Traits>
basic_igzfilter <CharT, Traits>::basic_igzfilter (basic_igzfilter <CharT, Traits>&& gzstream) :
	std::basic_istream <CharT, Traits> (),
	buf (gzstream .buf)
{
	init (buf);
	gzstream .rdbuf (nullptr);
	gzstream .setstate (std::ios::badbit);
}

template <class CharT, class Traits>
basic_igzfilter <CharT, Traits>::basic_igzfilter (std::basic_istream <CharT, Traits>&& istream) :
	std::basic_istream <CharT, Traits> (),
	buf ()
{
	std::basic_ostringstream <CharT, Traits> osstring;
	osstring << istream .rdbuf ();

	buf = new basic_gzfilterbuf <CharT, Traits> (new std::basic_stringbuf <CharT, Traits> (osstring .str ()));

	init (buf);
	istream .setstate (std::ios::badbit);
}

template <class CharT, class Traits>
void
basic_igzfilter <CharT, Traits>::close ()
{
	setstate (std::ios::badbit);

	if (rdbuf ())
		delete rdbuf ();
}

template <class CharT, class Traits>
inline
basic_igzfilter <CharT, Traits>::~basic_igzfilter ()
{
	close ();
}

typedef basic_igzfilter <char> igzfilter;

extern template class basic_igzfilter <char>;

template <class CharT,
          class Traits = std::char_traits <CharT>>
inline
basic_igzfilter <CharT, Traits>
operator >> (std::basic_istream <CharT, Traits> & istream,
             basic_igzfilter <CharT, Traits> (* function) (std::basic_istream <CharT, Traits> &))
{
	return function (istream);
}

template <class CharT,
          class Traits = std::char_traits <CharT>>
inline
basic_igzfilter <CharT, Traits>
gunzip (std::basic_istream <CharT, Traits>&& istream)
{
	return igzfilter (std::move (istream));
}

} // basic
} // titania

#endif
