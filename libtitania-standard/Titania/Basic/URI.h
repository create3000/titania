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

#ifndef __TITANIA_BASIC_URI_H__
#define __TITANIA_BASIC_URI_H__

#include "../Basic/Path.h"
#include "../String/to_string.h"

#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../LOG.h"

namespace titania {
namespace basic {

template <class StringT>
class basic_uri;

template <class StringT, class Traits>
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> &, const basic_uri <StringT> &);

/**
 *  Template to URI adresses.
 *  http://de.wikipedia.org/wiki/Uniform_Resource_Identifier:
 *
 *
 *  URI = scheme ":" hierarchy part [ "?" query ] [ "#" fragment ]
 *
 *
 *   foo://example.com:8042/over/there?name=ferret#nose
 *   \_/   \______________/\_________/ \_________/ \__/
 *    |           |            |            |        |
 *  scheme   authority       path        query   fragment
 *    |   _____________________|__
 *   / \ /                        \
 *   urn:example:animal:ferret:nose
 *
 *
 *  * * * * * * * * * * * * * * * *
 *  *             URI             *
 *  *    *********************    *
 *  *    * URL   * *   URN   *    *
 *  *    *********************    *
 *  * * * * * * * * * * * * * * * *
 *
 *
 *  Universal Resource Identifiers (URI)
 *  http://www.ietf.org/rfc/rfc1630.txt
 *
 *  Uniform Resource Locators (URL)
 *  http://www.ietf.org/rfc/rfc1738.txt
 *
 *  Uniform Resource Name (URN)
 *  http://www.ietf.org/rfc/rfc2141.txt
 */
template <class StringT>
class basic_uri
{
public:

	///  @name Member types

	///  Path typedef.
	using path_type = basic_path <StringT>;

	///  String typedef.
	using string_type = StringT;

	///  Char typedef.
	using char_type = typename StringT::value_type;

	///  Size typedef.  Used for size and port number.
	using size_type = size_t;

	///  @name Construction

	///  Default constructor.
	///  This constructs an URI with the empty URI ''.
	basic_uri () :
		   m_local (true),
		m_absolute (false),
		  m_scheme (),
		  m_slashs (),
		    m_host (),
		    m_port (0),
		    m_path ("", string_type (1, Signs::Slash)),
		   m_query (),
		m_fragment (),
		  m_string ()
	{ }

	///  Copy constructor.
	basic_uri (const basic_uri & other) :
		   m_local (other .m_local),
		m_absolute (other .m_absolute),
		  m_scheme (other .m_scheme),
		  m_slashs (other .m_slashs),
		    m_host (other .m_host),
		    m_port (other .m_port),
		    m_path (other .m_path),
		   m_query (other .m_query),
		m_fragment (other .m_fragment),
		  m_string (other .m_string)
	{ }

	///  Move constructor.
	basic_uri (basic_uri && other) :
		   m_local (std::exchange (other .m_local, true)),
		m_absolute (std::exchange (other .m_absolute, false)),
		  m_scheme (std::move (other .m_scheme)),
		  m_slashs (std::move (other .m_slashs)),
		    m_host (std::move (other .m_host)),
		    m_port (std::exchange (other .m_port, 0)),
		    m_path (std::move (other .m_path)),
		   m_query (std::move (other .m_query)),
		m_fragment (std::move (other .m_fragment)),
		  m_string (std::move (other .m_string))
	{ }

	///  Construct a URI from @a string.
	basic_uri (const string_type & path);

	///  Construct a URI from @a string.
	basic_uri (const char_type* path);

	///  Construct a URI from @a uri and @a base uri.
	basic_uri (const basic_uri & base, const basic_uri & uri);

	///  @name Assignment operator

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const basic_uri & other);

	///  Assign @a uri to this URI by moving the contents.
	basic_uri &
	operator = (basic_uri && other);

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const string_type & path);

	///  Assign @a uri to this URI.
	basic_uri &
	operator = (const char_type* path);

	/// @name Capacity

	///  Returns true if this uri is the empty uri ''.
	bool
	empty () const
	{ return m_string .empty (); }

	size_type
	length () const
	{ return m_string .length (); }

	size_type
	size () const
	{ return m_string .size (); }

	/// @name Filename Test

	///  Returns true if this uri is relative otherwise false.
	bool
	is_relative () const
	{ return not is_absolute (); }

	///  Returns true if this uri is an absolute uri otherwise false.
	bool
	is_absolute () const
	{ return m_absolute; }

	///  Returns true if this uri is local,
	//// i.e. it has a 'file:' scheme or no scheme and no authority, otherwise false.
	bool
	is_local () const
	{ return m_local; }

	///  Returns true if this uri is a network address, i.e. if it is not local.
	bool
	is_network () const
	{ return not is_local (); }

	///  Returns true if this uri looks like a directory, i.e. it ends with a '/', otherwise false.
	bool
	is_directory () const
	{ return m_path .empty () ? is_network () : m_path .trailing_separator (); }

	///  Returns true if this uri looks like a file, i.e. it is not a directory, otherwise false.
	bool
	is_file () const
	{ return not is_directory (); }

	/// @name Element access

	///  Returns the hierarchy part of this URI.
	string_type
	hierarchy () const;

	///  Returns the authority of this URI.
	string_type
	authority () const;

	///  Returns the scheme of this URI.
	const string_type &
	scheme () const
	{ return m_scheme; }

	///  Returns the host of this URI.
	const string_type &
	host () const
	{ return m_host; }

	///  Returns the port of this URI.
	const size_type &
	port () const
	{ return m_port; }

	///  Returns the well_known_port of this URI.
	size_type
	well_known_port () const;

	///  Returns the path of this URI.
	string_type
	path () const
	{ return m_path .str (); }

	///  Set the query of this URI.
	void
	query (const string_type & query)
	{
		m_query  = query;
		m_string = to_string ();
	}

	///  Returns the query of this URI.
	const string_type &
	query () const
	{ return m_query; }

	///  Set the fragment of this URI.
	void
	fragment (const string_type & fragment)
	{
		m_fragment = fragment;
		m_string   = to_string ();
	}

	///  Returns the fragment of this URI.
	const string_type &
	fragment () const
	{ return m_fragment; }

	///  Returns the string representation of this URI.
	const string_type &
	str () const
	{ return m_string; }

	///  Returns the string representation of this URI.
	operator const string_type & () const
	{ return m_string; }

	/// @name Path Operations

	///  Returns the root directory of this URI.
	basic_uri
	root () const
	{
		return basic_uri (is_local (),
		                  is_absolute (),
		                  scheme (),
		                  m_slashs,
		                  host (),
		                  port (),
		                  path_type (string_type (1, Signs::Slash), string_type (1, Signs::Slash)),
		                  "",
		                  "");
	}

	///  Returns the base directory of this URI.
	basic_uri
	base () const;

	///  Returns the parent directory of this URI.
	basic_uri
	parent () const
	{
		return basic_uri (is_local (),
		                  is_absolute (),
		                  scheme (),
		                  m_slashs,
		                  host (),
		                  port (),
		                  m_path .parent (),
		                  "",
		                  "");
	}

	///  Transforms @a reference to this URI's base.
	basic_uri
	transform (const basic_uri & reference) const;

	///  Returns a relative path, relative from base path.
	basic_uri <StringT>
	relative_path (const basic_uri & descendant) const;

	/// @name Filename Operations

	///  Returns the full basename of this URI with or without query. The default is without query.
	basic_uri
	filename (const bool query = false) const;

	///  Returns the full basename of this URI with extension.
	string_type
	basename () const
	{ return m_path .basename (); }

	///  Returns the basename of this URI without extension.
	string_type
	stem () const
	{ return m_path .stem (); }

	///  Adds @a extension to basename.
	void
	extension (const string_type & extension)
	{ m_path .extension (extension); }

	///  Returns the extension of this URI's basename.
	string_type
	extension () const
	{ return m_path .extension (); }

	///  Adds the file sheme if uri is local and absolute.
	basic_uri
	add_file_scheme () const;

	///  Escapes the uri.
	basic_uri
	escape () const;

	///  Unescapes the uri.
	basic_uri
	unescape () const;

	///  Swaps the contents.
	void
	swap (basic_uri & other);


private:

	///  Component constructor.
	basic_uri (const bool local,
	           const bool absolute,
	           const string_type & scheme,
	           const string_type & slashs,
	           const string_type & host,
	           const size_type port,
	           const path_type & path,
	           const string_type & query,
	           const string_type & fragment);

	///  Remove dot segments from path.
	path_type
	remove_dot_segments (const string_type &) const;

	///  Composes a string from all values.
	string_type
	to_string () const;

	class parser
	{
	public:

		using size_type = typename string_type::size_type;

		static
		void
		parse (basic_uri & uri, const string_type & string, size_type first = 0);


	private:

		parser (basic_uri & uri, const string_type & string) :
			   uri (uri),
			string (string)
		{ }

		void
		uriString (size_type first) const;

		size_type
		scheme (const size_type) const;

		size_type
		authority (const size_type) const;

		void
		host (const string_type &, const size_type, const size_type) const;

		void
		port (string_type &, const size_type, const size_type) const;

		void
		path (const size_type) const;

		void
		query (const size_type) const;

		void
		fragment (const size_type) const;

		basic_uri &         uri;
		const string_type & string;
	};

	///  @name Static members

	struct Signs
	{
		static const char_type Zero;
		static const char_type Colon;
		static const char_type Slash;
		static const char_type QuestionMark;
		static const char_type NumberSign;
		static const char_type Dot;
		static const char_type SlashQuestionNumber [3];
		static const char_type QuestionNumber [2];
	};

	static const string_type                 DataSchemeId;
	static const string_type                 FileSchemeId;
	static std::map <string_type, size_type> well_known_ports;

	///  @name Members

	bool        m_local;
	bool        m_absolute;
	string_type m_scheme;
	string_type m_slashs;
	string_type m_host;
	size_type   m_port;
	path_type   m_path;
	string_type m_query;
	string_type m_fragment;
	string_type m_string;

};

template <class StringT>
inline
basic_uri <StringT>::basic_uri (const bool local,
                                const bool absolute,
                                const string_type & scheme,
                                const string_type & slashs,
                                const string_type & host,
                                const size_type port,
                                const path_type & path,
                                const string_type & query,
                                const string_type & fragment) :
	   m_local (local),
	m_absolute (absolute),
	  m_scheme (scheme),
	  m_slashs (slashs),
	    m_host (host),
	    m_port (port),
	    m_path (path),
	   m_query (query),
	m_fragment (fragment),
	  m_string (to_string ())
{ }

template <class StringT>
inline
basic_uri <StringT>::basic_uri (const string_type & string) :
	basic_uri ()
{
	parser::parse (*this, string);
}

template <class StringT>
inline
basic_uri <StringT>::basic_uri (const char_type* string) :
	basic_uri ()
{
	parser::parse (*this, string);
}

template <class StringT>
inline
basic_uri <StringT>::basic_uri (const basic_uri & base, const basic_uri & uri) :
	basic_uri ()
{
	*this = base .transform (uri);
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const basic_uri & other)
{
	m_local    = other .m_local;
	m_absolute = other .m_absolute;
	m_scheme   = other .m_scheme;
	m_slashs   = other .m_slashs;
	m_host     = other .m_host;
	m_port     = other .m_port;
	m_path     = other .m_path;
	m_query    = other .m_query;
	m_fragment = other .m_fragment;
	m_string   = other .m_string;

	return *this;
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (basic_uri && other)
{
	m_local    = std::exchange (other .m_local, true);
	m_absolute = std::exchange (other .m_absolute, false);
	m_scheme   = std::move (other .m_scheme);
	m_slashs   = std::move (other .m_slashs);
	m_host     = std::move (other .m_host);
	m_port     = std::exchange (other .m_port, 0);
	m_path     = std::move (other .m_path);
	m_query    = std::move (other .m_query);
	m_fragment = std::move (other .m_fragment);
	m_string   = std::move (other .m_string);

	return *this;
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const string_type & string)
{
	return *this = basic_uri (string);
}

template <class StringT>
inline
typename basic_uri <StringT>::basic_uri &
basic_uri <StringT>::operator = (const char_type* string)
{
	return *this = basic_uri (string);
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::hierarchy () const
{
	string_type hierarchy;

	hierarchy += m_slashs;
	hierarchy += authority ();
	hierarchy += path ();

	return hierarchy;
}

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::authority () const
{
	auto authority = host ();

	if (port ())
	{
		authority += Signs::Colon;
		authority += basic::to_string (port (), std::locale::classic ());
	}

	return authority;
}

template <class StringT>
typename basic_uri <StringT>::size_type
basic_uri <StringT>::well_known_port () const
{
	auto well_known_port = well_known_ports .find (scheme ());

	if (well_known_port not_eq well_known_ports .end ())
		return well_known_port -> second;

	return 0;
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::base () const
{
	return is_directory ()
	       ? basic_uri (is_local (),
	                    is_absolute (),
	                    scheme (),
	                    m_slashs,
	                    host (),
	                    port (),
	                    m_path,
	                    "",
	                    "")
			 : parent ();
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::transform (const basic_uri & reference) const
{
	bool        T_local;
	bool        T_absolute;
	string_type T_scheme;
	string_type T_slashs;
	string_type T_host;
	size_type   T_port;
	string_type T_path;
	string_type T_query;
	string_type T_fragment;

	if (reference .scheme () .size ())
	{
		T_local     = reference .is_local ();
		T_absolute  = reference .is_absolute ();
		T_scheme    = reference .scheme ();
		T_slashs    = reference .m_slashs;
		T_host      = reference .host ();
		T_port      = reference .port ();
		T_path      = reference .path ();
		T_query     = reference .query ();
	}
	else
	{
		if (reference .authority () .size ())
		{
			T_local     = reference .is_local ();
			T_absolute  = reference .is_absolute ();
			T_host      = reference .host ();
			T_port      = reference .port ();
			T_path      = reference .path ();
			T_query     = reference .query ();
		}
		else
		{
			if (reference .path () .empty ())
			{
				T_path = path ();

				if (reference .query () .size ())
					T_query = reference .query ();
				else
					T_query = query ();
			}
			else
			{
				if (reference .path () .front () == Signs::Slash)
				{
					T_path = reference .path ();
				}
				else
				{
					// merge (Base .path (), reference .path ());

					basic_uri Base = base ();

					if (Base .path () .empty ())
						T_path = Signs::Slash;
					else
						T_path += Base .path ();

					T_path += reference .path ();
				}

				T_query = reference .query ();
			}

			T_local     = is_local ();
			T_absolute  = is_absolute () or reference .is_absolute ();
			T_host      = host ();
			T_port      = port ();
		}

		T_scheme = scheme ();
		T_slashs = m_slashs;
	}

	T_fragment = reference .fragment ();

	return basic_uri (T_local,
	                  T_absolute,
	                  T_scheme,
	                  T_slashs,
	                  T_host,
	                  T_port,
	                  remove_dot_segments (T_path),
	                  T_query,
	                  T_fragment);
}

template <class StringT>
inline
typename basic_uri <StringT>::path_type
basic_uri <StringT>::remove_dot_segments (const string_type & path) const
{
	return basic_path <string_type> (path, string_type (1, Signs::Slash)) .remove_dot_segments ();
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::relative_path (const basic_uri & descendant) const
{
	if (scheme () not_eq descendant .scheme ())
		return descendant;

	if (authority () not_eq descendant .authority ())
		return descendant;

	return basic_uri (true,
	                  false,
	                  string_type (),
	                  string_type (),
	                  string_type (),
	                  0,
	                  m_path .relative_path (descendant .m_path),
	                  descendant .query (),
	                  descendant .fragment ());
}

// Filename operations

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::filename (const bool q) const
{
	return basic_uri (is_local (),
	                  is_absolute (),
	                  scheme (),
	                  m_slashs,
	                  host (),
	                  port (),
	                  m_path,
	                  q ? query () : string_type (),
	                  "");
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::add_file_scheme () const
{
	if (is_local () and is_absolute ())
	{
		return basic_uri (is_local (),
		                  is_absolute (),
		                  FileSchemeId,
		                  StringT (2, Signs::Slash),
		                  host (),
		                  port (),
		                  m_path,
		                  query (),
		                  fragment ());
	}

	return *this;
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::escape () const
{
	return basic_uri (is_local (),
	                  is_absolute (),
	                  scheme (),
	                  m_slashs,
	                  host (),
	                  port (),
	                  m_path .escape (),
	                  query (),
	                  Glib::uri_escape_string (fragment ()));
}

template <class StringT>
basic_uri <StringT>
basic_uri <StringT>::unescape () const
{
	return basic_uri (is_local (),
	                  is_absolute (),
	                  scheme (),
	                  m_slashs,
	                  host (),
	                  port (),
	                  m_path .unescape (),
	                  query (),
	                  Glib::uri_unescape_string (fragment ()));
}

template <class StringT>
void
basic_uri <StringT>::swap (basic_uri & other)
{
	std::swap (m_local,    other .m_local);
	std::swap (m_absolute, other .m_absolute);
	std::swap (m_scheme,   other .m_scheme);
	std::swap (m_slashs,   other .m_slashs);
	std::swap (m_host,     other .m_host);
	std::swap (m_port,     other .m_port);
	std::swap (m_path,     other .m_path);
	std::swap (m_query,    other .m_query);
	std::swap (m_fragment, other .m_fragment);
	std::swap (m_string,   other .m_string);
}

// Private Funtions

template <class StringT>
typename basic_uri <StringT>::string_type
basic_uri <StringT>::to_string () const
{
	auto string = scheme ();

	if (scheme () .size ())
		string += Signs::Colon;

	string += hierarchy ();

	if (query () .size ())
	{
		string += Signs::QuestionMark;
		string += query ();
	}

	if (fragment () .size ())
	{
		string += Signs::NumberSign;
		string += fragment ();
	}

	return string;
}

// Parser

template <class StringT>
void
basic_uri <StringT>::parser::parse (basic_uri & uri, const string_type & string, const size_type first)
{
	if (string .empty ())
		return;

	uri .m_string = string;

	parser (uri, string) .uriString (first);
}

template <class StringT>
void
basic_uri <StringT>::parser::uriString (size_type first) const
{
	// URI Generic Syntax
	// ^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?

	first = scheme (first);

	// Network path

	if (string [first] == Signs::Colon)
	{
		++ first;
		uri .m_absolute = true;

		if (uri .m_scheme == DataSchemeId)
		{
			uri .m_local = true;
			return;
		}
	}

	auto begin = first;

	if (string [begin ++] == Signs::Slash)
	{
		if (string [begin ++] == Signs::Slash)
		{
			uri .m_absolute = true;
			uri .m_slashs   = string_type (2, Signs::Slash);
			first           = authority (begin);
		}
	}

	switch (string [first])
	{
		case 0:
			break;
		case Signs::Slash:
			path (first);
			uri .m_absolute = true;
			break;
		case Signs::QuestionMark:
			query (first + 1);
			break;
		case Signs::NumberSign:
			fragment (first + 1);
			break;
		default:
			path (first);
			break;
	}

	uri .m_local  = uri .m_scheme == FileSchemeId or (not uri .m_scheme .size () and not (uri .m_host .size () || uri .m_port));
	uri .m_string = uri .to_string ();
}

template <class StringT>
typename StringT::size_type
basic_uri <StringT>::parser::scheme (const size_type first) const
{
	if (std::isalpha (string [0]))
	{
		auto last = string .find (Signs::Colon, first);

		if (last == string_type::npos)
			return first;

		uri .m_scheme = string .substr (first, last - first);

		return last;
	}

	return first;
}

template <class StringT>
typename StringT::size_type
basic_uri <StringT>::parser::authority (const size_type first) const
{
	auto last = string .find_first_of (Signs::SlashQuestionNumber, first, 3);

	// Return if no authority.
	if (first == last)
		return last;

	if (last == string_type::npos)
		last = string .size ();

	auto       authority = string .substr (first, last - first);
	const auto colon     = authority .find (Signs::Colon);

	if (colon not_eq string_type::npos)
	{
		host (authority, 0, colon);
		port (authority, colon + 1, authority .size ());
	}
	else
		uri .m_host = Glib::uri_unescape_string (authority);

	return last;
}

template <class StringT>
inline
void
basic_uri <StringT>::parser::host (const string_type & authority, const size_type first, const size_type last) const
{
	uri .m_host = Glib::uri_unescape_string (authority .substr (first, last - first));
}

template <class StringT>
void
basic_uri <StringT>::parser::port (string_type & authority, const size_type first, const size_type last) const
{
	auto pos = first;

	string_type portString = authority .substr (first);

	try
	{
		uri .m_port = std::stoul (portString, &pos);
	}
	catch (...)
	{
		__LOG__ << "basic_uri parse error: Invalid port number." << std::endl;

		uri .m_port = 0;
	}
}

template <class StringT>
void
basic_uri <StringT>::parser::path (const size_type first) const
{
	auto last = string .find_first_of (Signs::QuestionNumber, first, 2);

	if (last == string_type::npos)
		last = string .size ();

	uri .m_path = path_type (Glib::uri_unescape_string (string .substr (first, last - first)), string_type (1, Signs::Slash));

	switch (string [last])
	{
		case Signs::QuestionMark:
			query (last + 1);
			break;
		case Signs::NumberSign:
			fragment (last + 1);
			break;
	}
}

template <class StringT>
void
basic_uri <StringT>::parser::query (const size_type first) const
{
	auto last = string .find (Signs::NumberSign, first);

	if (last == string_type::npos)
		last = string .size ();

	uri .m_query = string .substr (first, last - first);

	if (string [last] == Signs::NumberSign)
		fragment (last + 1);
}

template <class StringT>
inline
void
basic_uri <StringT>::parser::fragment (const size_type first) const
{
	uri .m_fragment = Glib::uri_unescape_string (string .substr (first));
}

///  @relates basic_uri
///  @name Comparision operations

///  Compares two basic_uri's.
///  Return true if @a a is equal to @a b.
template <class StringT>
inline
bool
operator == (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator not_eq (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () not_eq rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator == (const basic_uri <StringT> & lhs, const StringT & rhs)
{
	return lhs .str () == rhs;
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator == (const StringT & lhs, const basic_uri <StringT> & rhs)
{
	return lhs == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator not_eq (const basic_uri <StringT> & lhs, const StringT & rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator not_eq (const StringT & lhs, const basic_uri <StringT> & rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator == (const basic_uri <StringT> & lhs, const typename StringT::value_type* rhs)
{
	return lhs .str () == rhs;
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator == (const typename StringT::value_type* lhs, const basic_uri <StringT> & rhs)
{
	return lhs == rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator not_eq (const basic_uri <StringT> & lhs, const typename StringT::value_type* rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return false if URI @a a is not equal to @a b.
template <class StringT>
inline
bool
operator not_eq (const typename StringT::value_type* lhs, const basic_uri <StringT> & rhs)
{
	return not (lhs == rhs);
}

///  Compares two basic_uri's.
///  Return true if URI @a a is less than @a b.
template <class StringT>
bool
operator < (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () < rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is greater than @a b.
template <class StringT>
inline
bool
operator > (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () > rhs .str ();
}

///  Compares two basic_uri's.
///  Return true if URI @a a is less equal than @a b.
template <class StringT>
bool
operator <= (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () <= rhs .str ();
}

///  Compares two basic_uri's.
///  Return false if URI @a a is greater than equal @a b.
template <class StringT>
inline
bool
operator >= (const basic_uri <StringT> & lhs, const basic_uri <StringT> & rhs)
{
	return lhs .str () >= rhs .str ();
}

///  @relates basic_uri
///  @name String concatanation

///  Returns a string containing characters from @a uri followed by the characters from @a string.
template <class StringT>
inline
StringT
operator + (const basic_uri <StringT> & uri, const StringT & string)
{
	return uri .str () + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a uri.
template <class StringT>
inline
StringT
operator + (const StringT & string, const basic_uri <StringT> & uri)
{
	return string + uri .str ();
}

///  Returns a string containing characters from @a uri followed by the characters from @a string.
template <class StringT>
inline
StringT
operator + (const basic_uri <StringT> & uri, const typename StringT::value_type* string)
{
	return uri .str () + string;
}

///  Returns a string containing characters from @a string followed by the characters from @a uri.
template <class StringT>
inline
StringT
operator + (const typename StringT::value_type* string, const basic_uri <StringT> & uri)
{
	return string + uri .str ();
}

///  @relates basic_uri
///  @name Input/Output operations

///  Insertion operator for URI values.
template <class StringT, class Traits>
inline
std::basic_ostream <typename StringT::value_type, Traits> &
operator << (std::basic_ostream <typename StringT::value_type, Traits> & ostream, const basic_uri <StringT> & uri)
{
	return ostream << uri .str ();
}

///  @relates basic_uri

using uri = basic_uri <std::string>;

//
extern template class basic_uri <std::string>;

} // basic
} // titania

namespace std {

template <>
struct hash <titania::basic::uri>
{
	size_t
	operator () (const titania::basic::uri & uri) const
	{
		return m_hash (uri .str ());
	}

	hash <std::string> m_hash;

};

/// Specializes the std::swap algorithm for basic_uri.
template <class StringT>
inline
void
swap (titania::basic::basic_uri <StringT> & lhs, titania::basic::basic_uri <StringT> & rhs)
{
	lhs .swap (rhs);
}

} // std

#endif
