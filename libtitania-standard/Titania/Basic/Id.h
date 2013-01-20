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
 * For Silvio, Joy and Adi.
 *
 ******************************************************************************/

#ifndef __TITANIA_BASIC_ID_H__
#define __TITANIA_BASIC_ID_H__

#include <sstream>
#include <string>
#include <vector>

namespace titania {
namespace basic {

template <class StringT>
class basic_id
{
public:

	typedef StringT                          string_type;
	typedef typename string_type::value_type char_type;
	typedef std::vector <string_type>        value_type;
	typedef size_t                           size_type;

	basic_id () :
		value ()
	{ }

	basic_id (const basic_id & id) :
		value (id .value)
	{ }

	basic_id (const string_type & string) :
		value ({ string })
	{ }

	basic_id (const char_type* string) :
		value ({ string })
	{ }

	basic_id (std::initializer_list <string_type> list) :
		value (list)
	{ }

	// Assign:
	//@{
	basic_id &
	operator = (const basic_id & id)
	{
		value = id .value;
		return *this;
	}

	basic_id &
	operator = (const string_type & string)
	{
		value = { string };
		return *this;
	}

	basic_id &
	operator = (const char_type* string)
	{
		value = { string };
		return *this;
	}

	basic_id &
	operator = (const std::initializer_list <string_type> & list)
	{
		value = list;
		return *this;
	}
	//@}

	// Element access:

	//@{
	operator const string_type & () const
	{ return first (); }

	const string_type &
	first () const
	{ return operator [ ] (0); }

	const string_type &
	last () const
	{ return operator [ ] (count () - 1); }

	const string_type &
	operator [ ] (size_type index) const
	{ return count () ? value [min_index (index)] : empty_string; }
	//@}

	// Capacity:

	bool
	empty () const
	{ return value .empty (); }

	size_type
	count () const
	{ return value .size (); }

	size_type
	length () const
	{ return operator [ ] (0) .length (); }

	void
	clear () { value .clear (); }


private:

	constexpr size_type
	min_index (size_type index) const
	{ return std::min (index, count () - 1); }

	value_type value;

	static StringT empty_string;

};

template <class StringT>
StringT basic_id <StringT>::empty_string;

///  Returns a string containing characters from @a id followed by the characters from @a string.
template <class StringT>
inline
constexpr StringT
operator + (const basic_id <StringT> & id, const StringT & string)
{
	return StringT (id) + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a id.
template <class StringT>
inline
constexpr StringT
operator + (const StringT & string, const basic_id <StringT> & id)
{
	return string + StringT (id);
}

///  Returns a string containing characters from @a id followed by the characters from @a string.
template <class StringT>
inline
constexpr StringT
operator + (const basic_id <StringT> & id, const typename StringT::value_type* string)
{
	return StringT (id) + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a id.
template <class StringT>
inline
constexpr StringT
operator + (const typename StringT::value_type* string, const basic_id <StringT> & id)
{
	return string + StringT (id);
}

///  Extraction operator for id values.
template <class CharT, class Traits, class StringT>
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, basic_id <StringT> & id)
{
	//	std::string string;
	//
	//	inputStream >> string;
	//
	//	if (string == getName ())
	//		return inputStream;
	//
	//	inputStream .seekg (-string .size (), std::ios_base::cur);
	//
	//	inputStream .setstate (inputStream .failbit);

	return istream;
}

///  Insertion operator for id values.
template <class CharT, class Traits, class StringT>
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const basic_id <StringT> & id)
{
	return ostream << id [0];
}

// Id

typedef basic_id <std::string>  id;
typedef basic_id <std::wstring> wid;

//
extern template class basic_id <std::string>;
extern template class basic_id <std::wstring>;

//
extern template std::istream & operator >> (std::istream &, id &);
extern template std::wistream & operator >> (std::wistream &, wid &);

//
extern template std::ostream & operator << (std::ostream &, const id &);
extern template std::wostream & operator << (std::wostream &, const wid &);

} // basic
} // titania

#endif
