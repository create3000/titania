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
#include <sstream>
#include <vector>

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
class basic_path
{
private:

	///  @name Member types

	using array_type = std::vector <StringT>;


public:

	///  @name Member types

	///  String typedef.
	using string_type = StringT;

	///  Char typedef.
	using char_type = typename StringT::value_type;

	///  Size typedef.  Used for size and indices.
	using size_type = typename array_type::size_type;

	///  std::ptrdiff_t
	using difference_type = typename array_type::difference_type;

	///  value_type &
	using reference = typename array_type::reference;

	///  const value_type &
	using const_reference = typename array_type::const_reference;

	///  value_type*
	using pointer = typename array_type::pointer;

	///  const value_type*
	using const_pointer = typename array_type::const_pointer;

	///  Random access iterator
	using iterator = typename array_type::iterator;

	///  Constant random access iterator 
	using const_iterator = typename array_type::const_iterator;

	///  std::reverse_iterator <iterator>
	using reverse_iterator = typename array_type::reverse_iterator;

	///  std::reverse_iterator <iterator>
	using const_reverse_iterator = typename array_type::const_reverse_iterator;

	///  @name Construction

	///  Copy constructor.
	basic_path (const basic_path & other) :
		             m_array (other .m_array),
		         m_separator (other .m_separator),
		 m_leading_separator (other .m_leading_separator),
		m_trailing_separator (other .m_trailing_separator)
	{ }

	///  Move constructor.
	basic_path (basic_path && other) :
		             m_array (std::move (other .m_array)),
		         m_separator (other .m_separator),
		 m_leading_separator (std::exchange (other .m_leading_separator, false)),
		m_trailing_separator (std::exchange (other .m_trailing_separator, false))
	{ }

	///  Path constructor. Construct path from @a path and @a separator.
	basic_path (const string_type & path, const string_type & separator);

	///  @name Assignment operator

	///  Assign @a other to this path.
	basic_path &
	operator = (const basic_path & other);

	///  Assign @a other to this path.
	basic_path &
	operator = (basic_path && other);

	///  @name Member access

	///  Return the separator value of this path.
	const string_type &
	separator () const
	{ return m_separator; }

	///  Return whether this path has a leading separator.
	bool
	leading_separator () const
	{ return m_leading_separator; }

	///  Return whether this path has a trailing separator.
	bool
	trailing_separator () const
	{ return m_trailing_separator; }

	///  Access specified element with bounds checking.
	const_reference
	at (const size_type index) const
	{ return m_array .at (index); }

	///  Access components by @a index.
	const_reference
	operator [ ] (const size_type index) const
	{ return m_array [index]; }

	///  Returns a reference to the first element in the container. 
	const_reference
	front () const
	{ return m_array .front (); }

	///  Returns reference to the last element in the container. 
	const_reference
	back () const
	{ return m_array .back (); }

	///  Returns pointer to the underlying array serving as element storage.
	const_pointer
	data () const
	{ return m_array .data (); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	const_iterator
	begin () const
	{ return m_array .begin (); }

	///  Returns an iterator to the beginning.
	const_iterator
	cbegin () const
	{ return m_array .cbegin (); }

	///  Returns an iterator to the end.
	const_iterator
	end () const
	{ return m_array .end (); }

	///  Returns an iterator to the end.
	const_iterator
	cend () const
	{ return m_array .cend (); }

	///  returns a reverse iterator to the beginning.
	const_reverse_iterator
	rbegin () const
	{ return m_array .rbegin (); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	crbegin () const
	{ return m_array .crbegin (); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	rend () const
	{ return m_array .rend (); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	crend () const
	{ return m_array .crend (); }

	///  @name Capacity

	///  Checks whether the container is empty. Always returns false.
	[[nodiscard]]
	bool
	empty () const
	{ return m_array .empty (); }

	///  Returns the number of elements in the container.
	size_type
	size () const
	{ return m_array .size (); }

	///  Returns the maximum possible number of elements. Because each vector is a fixed-size container,
	///  the value is also the value returned by size.
	size_type
	max_size () const
	{ return m_array .max_size (); }

	///  @name Operations

	///  Swaps the contents.
	void
	swap (basic_path & other);

	///  @name Operations

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

	///  Returns the full basename of this Path with extension.
	string_type
	basename () const;

	///  Returns the basename of this Path without extension.
	string_type
	stem () const;

	///  Adds @a extension to basename.
	void
	extension (const string_type & extension);

	///  Returns the extension of this Path's basename.
	string_type
	extension () const;

	///  Return relative path form this path to @a descendant.
	basic_path
	relative_path (const basic_path & descendant) const;

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

	///  @name Construction

	basic_path (const array_type & array, const string_type & separator, const bool leading_separator, const bool trailing_separator) :
		             m_array (array),
		         m_separator (separator),
		 m_leading_separator (leading_separator),
		m_trailing_separator (trailing_separator)
	{ }

	basic_path (const_iterator first, const_iterator last, const string_type & separator, const bool leading_separator, const bool trailing_separator) :
		             m_array (first, last),
		         m_separator (separator),
		 m_leading_separator (leading_separator),
		m_trailing_separator (trailing_separator)
	{ }

	///  @name Static members

	static const string_type dot;
	static const string_type dots;

	///  @name Members

	array_type  m_array;
	string_type m_separator;
	bool        m_leading_separator;
	bool        m_trailing_separator;

};

template <class StringT>
basic_path <StringT>::basic_path (const string_type & path, const string_type & separator) :
		             m_array (),
		         m_separator (separator),
		 m_leading_separator (false),
		m_trailing_separator (false)
{
	basic_split <StringT> (std::back_inserter (m_array), path, separator);

	if (m_array .size ())
	{
		if (m_array .front () .empty ())
		{
			m_array .erase (m_array .begin ());
			m_leading_separator = true;
		}
	}

	if (m_array .size ())
	{
		if (m_array .back () .empty ())
		{
			m_array .pop_back ();
			m_trailing_separator = true;
		}
	}
}

template <class StringT>
basic_path <StringT> &
basic_path <StringT>::operator = (const basic_path & other)
{
	m_array              = other .m_array;
	m_separator          = other .m_separator;
	m_leading_separator  = other .m_leading_separator;
	m_trailing_separator = other .m_trailing_separator;

	return *this;
}

template <class StringT>
basic_path <StringT> &
basic_path <StringT>::operator = (basic_path && other)
{
	m_array              = other .m_array;
	m_separator          = other .m_separator;
	m_leading_separator  = std::exchange (other .m_leading_separator, false);
	m_trailing_separator = std::exchange (other .m_trailing_separator, false);

	return *this;
}

template <class StringT>
void
basic_path <StringT>::swap (basic_path & other)
{
	std::swap (m_array,              other .m_array);
	std::swap (m_separator,          other .m_separator);
	std::swap (m_leading_separator,  other .m_leading_separator);
	std::swap (m_trailing_separator, other .m_trailing_separator);
}

template <class StringT>
bool
basic_path <StringT>::is_relative () const
{
	return m_array .empty () or m_array .front () == dots;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::root () const
{
	return basic_path ({ }, separator (), true, true);
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

			return basic_path ({ dots }, separator (), false, false);
		}

		default:
			return basic_path (begin (), -- end (), separator (), leading_separator (), true);
	}

}

template <class StringT>
typename basic_path <StringT>::string_type
basic_path <StringT>::basename () const
{
	if (empty ())
		return string_type ();

	return back ();
}

template <class StringT>
typename basic_path <StringT>::string_type
basic_path <StringT>::stem () const
{
	const auto basename = this -> basename ();

	if (not trailing_separator () and basename .size ())
	{
		const auto extension = this -> extension ();

		if (not extension .empty ())
		{
			return basename .substr (0, basename .size () - extension .size ());
		}
	}

	return basename;
}

template <class StringT>
void
basic_path <StringT>::extension (const string_type & extension)
{
	*this = basic_path (str () + extension, separator ());
}

template <class StringT>
typename basic_path <StringT>::string_type
basic_path <StringT>::extension () const
{
	const auto basename = this -> basename ();
	const auto dot      = basename .rfind (dots);

	if (dot not_eq string_type::npos and dot not_eq 0)
		return basename .substr (dot);

	return string_type ();
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::relative_path (const basic_path & descendant) const
{
	if (descendant .is_relative ())
		return descendant;

	basic_path path ({ }, separator (), false, descendant .trailing_separator ());

	const auto base_path       = remove_dot_segments () .base ();
	const auto descendant_path = descendant .remove_dot_segments ();

	size_t i = 0, j = 0, size = 0;

	for (i = 0, size = std::min (base_path .size (), descendant_path .size ()); i < size; ++ i)
	{
		if (base_path [i] not_eq descendant_path [i])
			break;
	}

	for (j = i, size = base_path .size (); j < size; ++ j)
		path .m_array .emplace_back (dots);

	for (j = i, size = descendant_path .size (); j < size; ++ j)
		path .m_array .emplace_back (descendant_path [j]);

	if (path .m_array .empty ())
		path .m_array .emplace_back (dot);

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::remove_dot_segments () const
{
	basic_path path ({ }, separator (), leading_separator (), trailing_separator ());

	if (size ())
	{
		auto first = begin ();

		while (first not_eq end ())
		{
			if (first -> empty ())
			{
				;
			}
			else if (*first == dot)
			{
				path .m_trailing_separator = true;
			}
			else if (*first == dots)
			{
				path .m_trailing_separator = true;

				if (path .size ())
					path .m_array .pop_back ();
			}
			else
			{
				path .m_trailing_separator = false;
				path .m_array .emplace_back (*first);
			}

			++ first;
		}

		path .m_trailing_separator |= trailing_separator ();
	}

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::escape () const
{
	basic_path path (*this);

	for (auto & segment : path .m_array)
		segment = Glib::uri_escape_string (segment);

	return path;
}

template <class StringT>
basic_path <StringT>
basic_path <StringT>::unescape () const
{
	basic_path path (*this);

	for (auto & segment : path .m_array)
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

	if (path .leading_separator () and path .empty ())
		return ostream;

	if (path .trailing_separator ())
		ostream << path .separator ();

	return ostream;
}

using path  = basic_path <std::string>;
//using wpath = basic_path <std::wstring>;

extern template class basic_path <std::string>;
//extern template class basic_path <std::wstring>;

} // basic
} // titania

namespace std {

/// Specializes the std::swap algorithm for basic_path.
template <class StringT>
inline
void
swap (titania::basic::basic_path <StringT> & lhs, titania::basic::basic_path <StringT> & rhs)
{
	lhs .swap (rhs);
}

} // std

#endif
