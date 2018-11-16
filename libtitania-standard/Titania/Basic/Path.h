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

#ifndef __TITANIA_BASIC_PATH_H__
#define __TITANIA_BASIC_PATH_H__

#include "../LOG.h"
#include "../String/split.h"
#include "../Utility/Range.h"

#include <glibmm/uriutils.h>
#include <algorithm>
#include <deque>
#include <iterator>
#include <sstream>

namespace titania {
namespace basic {

template <class StringT>
class basic_path;

template <class StringT, class Traits>
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> &, const basic_path <StringT> &);

/**
 *  Template to represent paths.
 *  This can be file system paths or even urn paths.
 */
template <class StringT>
class basic_path :
	public std::deque <StringT>
{
public:

	typedef StringT                      string_type;
	typedef typename StringT::value_type char_type;
	typedef typename StringT::size_type  size_type;
	typedef std::deque <StringT>         array_type;

	typedef typename array_type::const_iterator const_iterator;

	using array_type::size;
	using array_type::begin;
	using array_type::end;
	using array_type::front;
	using array_type::back;
	using array_type::pop_front;
	using array_type::pop_back;

	///  @name Constructors

	basic_path (const basic_path & other) :
		std::deque <StringT> (other),
		               value (other .value)
	{ }

	explicit
	basic_path (const string_type & separator) :
		std::deque <StringT> (),
		               value ({ separator, false, false })
	{ }

	basic_path (const string_type & path, const StringT & separator);

	///  @name Element access

	const string_type &
	separator () const
	{ return value .separator; }

	bool
	leading_separator () const
	{ return value .leading_separator; }

	bool
	trailing_separator () const
	{ return value .trailing_separator; }

	///  Returns the root directory of this Path.
	bool
	is_relative () const;

	///  Returns the root directory of this Path.
	basic_path
	root () const;

	///  Returns the base directory of this Path.
	basic_path
	base () const;

	///  Returns the parent directory of this Path.
	basic_path
	parent () const;

	///  Return relative path form this path to descendant.
	basic_path
	relative_path (const basic_path &) const;

	///  Remove dot segments.
	basic_path
	remove_dot_segments () const;

	///  Returns an escaped path.
	basic_path
	escape () const;

	///  Returns an unescaped path.
	basic_path
	unescape () const;

	///  Returns the string representation of this Path.
	string_type
	str () const;


private:

	struct Value
	{
		string_type separator;
		bool leading_separator;
		bool trailing_separator;
	};

	basic_path (const Value & value) :
		std::deque <string_type> (),
		                   value (value)
	{ }

	basic_path (array_type && array, const Value & value) :
		std::deque <string_type> (array),
		                   value (value)
	{ }

	basic_path (const_iterator first, const_iterator last, const Value & value) :
		std::deque <string_type> (first, last),
		                   value (value)
	{ }

	Value value;

	static const string_type dot;
	static const string_type dots;

};

template <class StringT>
const StringT basic_path <StringT>::dot = ".";

template <class StringT>
const StringT basic_path <StringT>::dots = "..";

template <class StringT>
basic_path <StringT>::basic_path (const StringT & path, const StringT & separator) :
	std::deque <string_type> (),
	                   value ({ separator, false, false })
{
	basic_split <StringT> (std::back_inserter (*this), path, separator);

	if (size ())
	{
		if (front () == string_type ())
		{
			pop_front ();
			value .leading_separator = true;
		}
	}

	if (size () > 1)
	{
		if (back () == string_type ())
		{
			pop_back ();
			value .trailing_separator = true;
		}
	}
}

template <class StringT>
bool
basic_path <StringT>::is_relative () const
{
	return this -> empty () or this -> front () == dots;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::root () const
{
	return basic_path (Value { separator (), true, false });
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::base () const
{
	if (trailing_separator ())
		return *this;

	return parent ();
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::parent () const
{
	switch (size ())
	{
		case 0:
		case 1:
		{
			if (leading_separator ())
				return root ();

			return basic_path (array_type { dots }, { separator (), false, false });
		}

		default:
			return basic_path (begin (), -- end (), { separator (), leading_separator (), true });
	}

}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::relative_path (const basic_path & descendant) const
{
	if (descendant .is_relative ())
		return descendant;

	basic_path path (separator ());

	auto base_path       = remove_dot_segments () .base ();
	auto descendant_path = descendant .remove_dot_segments ();

	size_t i = 0, j = 0, size = 0;

	for (i = 0, size = std::min (base_path .size (), descendant_path .size ()); i < size; ++ i)
	{
		if (base_path [i] not_eq descendant_path [i])
			break;
	}

	for (j = i, size = base_path .size (); j < size; ++ j)
		path .emplace_back (dots);

	for (j = i, size = descendant_path .size (); j < size; ++ j)
		path .emplace_back (descendant_path [j]);

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::remove_dot_segments () const
{
	basic_path path (array_type { }, { separator (), leading_separator (), trailing_separator () });

	if (size ())
	{
		auto first = begin ();

		while (first not_eq end ())
		{
			if (*first == dot)
				path .value .trailing_separator = true;

			else if (*first == dots)
			{
				path .value .trailing_separator = true;

				if (path .size ())
					path .pop_back ();
			}

			else
			{
				path .value .trailing_separator = false;
				path .emplace_back (*first);
			}

			++ first;
		}

		path .value .trailing_separator |= trailing_separator ();
	}

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::escape () const
{
	basic_path path (*this);

	for (auto & segment : path)
		segment = Glib::uri_escape_string (segment);

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::unescape () const
{
	basic_path path (*this);

	for (auto & segment : path)
		segment = Glib::uri_unescape_string (segment);

	return path;
}

template <class StringT>
typename basic_path <StringT>::string_type
basic_path <StringT>::str () const
{
	std::basic_ostringstream <char_type> osstream;

	osstream << *this;

	return osstream .str ();
}

///  @relates basic_path
///  @name Input/Output operations

///  Insertion operator for URI values.
template <class StringT, class Traits>
inline
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> & ostream, const basic_path <StringT> & path)
{
	if (path .leading_separator ())
		ostream << path .separator ();

	if (path .size ())
	{
		if (path .size () > 1)
		{
			for (const auto & segment : std::pair (path .begin (), -- path .end ()))
				ostream << segment << path .separator ();
		}

		ostream << path .back ();
	}

	if (path .trailing_separator ())
		ostream << path .separator ();

	return ostream;
}

typedef basic_path <std::string> path;

extern template class basic_path <std::string>;
extern template std::ostream & operator << (std::ostream &, const path &);

} // basic
} // titania

#endif
