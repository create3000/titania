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

#ifndef __TITANIA_BASIC_INPUT_OUTPUT_GENERATOR_H__
#define __TITANIA_BASIC_INPUT_OUTPUT_GENERATOR_H__

#include <iomanip>
#include <limits>
#include <ostream>
#include <string>

namespace titania {
namespace io {

template <class CharT, class Traits = std::char_traits <CharT>>
class basic_generator
{
public:

	basic_generator () = delete;

	basic_generator (const basic_generator &) = delete;

	static
	std::basic_ostream <CharT, Traits> &
	Comment (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << comment; }

	static
	std::basic_ostream <CharT, Traits> &
	Space (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << space; }

	static
	std::basic_ostream <CharT, Traits> &
	TidySpace (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << tidySpace; }

	static std::basic_ostream <CharT, Traits> &
	Break (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << endl; }

	static
	std::basic_ostream <CharT, Traits> &
	TidyBreak (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << tidyBreak; }

	static
	std::basic_ostream <CharT, Traits> &
	ListBreak (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << listBreak; }

	static
	std::basic_ostream <CharT, Traits> &
	Comma (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << comma; }

	static
	const bool &
	HasListBreak ()
	{ return hasListBreak; }

	static
	std::basic_ostream <CharT, Traits> &
	Indent (std::basic_ostream <CharT, Traits> & ostream)
	{ return ostream << indent; }

	static
	std::basic_ostream <CharT, Traits> &
	IncIndent (std::basic_ostream <CharT, Traits> & ostream)
	{
		indent += indentChar;
		return ostream;
	}

	static
	std::basic_ostream <CharT, Traits> &
	DecIndent (std::basic_ostream <CharT, Traits> & ostream)
	{
		indent = std::string (indent, 0, indent .length () - indentChar .length ());
		return ostream;
	}

	template <class Type>
	class ListSeparator :
		public std::iterator <std::output_iterator_tag, void, void, void, void>
	{
	public:

		typedef CharT                              char_type;
		typedef Traits                             traits_type;
		typedef std::basic_ostream <CharT, Traits> ostream_type;

		ListSeparator (ostream_type & ostream) :
			ostream (ostream) { }

		~ListSeparator () { }

		ListSeparator &
		operator = (const Type & value)
		{
			ostream << value;

			ostream << Comma << ListBreak;

			if (HasListBreak ())
				ostream << Indent;

			return *this;
		}

		ListSeparator &
		operator * () { return *this; }

		ListSeparator &
		operator ++ () { return *this; }

		ListSeparator &
		operator ++ (int) { return *this; }


	private:

		ostream_type & ostream;

	};

	static
	std::basic_ostream <CharT, Traits> &
	OpenBracket (std::basic_ostream <CharT, Traits> &);

	static
	std::basic_ostream <CharT, Traits> &
	CloseBracket (std::basic_ostream <CharT, Traits> &);

	static
	std::basic_ostream <CharT, Traits> &
	EmptyBrackets (std::basic_ostream <CharT, Traits> &);

	template <class Type>
	static
	void
	Precision (size_t value)
	{ NumericLimits <Type>::digits10 = value; }

	template <class Type>
	static
	size_t
	Precision ()
	{ return NumericLimits <Type>::digits10; }

	template <class Type>
	static
	std::basic_ostream <CharT, Traits> &
	Precision (std::basic_ostream <CharT, Traits> &);


protected:

	static std::string comment;
	static std::string space;
	static std::string tidySpace;
	static std::string endl;
	static std::string tidyBreak;
	static std::string listBreak;
	static std::string comma;
	static bool        hasListBreak;

	static std::string indent;
	static std::string indentChar;

	template <class Type>
	struct NumericLimits
	{
		static size_t digits10;
	};

};

template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::comment = "#";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::space = " ";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::tidySpace = " ";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::endl = "\n";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::tidyBreak = "\n";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::listBreak = "\n";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::comma = ",";
template <class CharT, class Traits>
bool basic_generator <CharT, Traits>::hasListBreak = true;

template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::indent = "";
template <class CharT, class Traits>
std::string basic_generator <CharT, Traits>::indentChar = "  ";

template <class CharT, class Traits>
template <class Type>
size_t basic_generator <CharT, Traits>::NumericLimits <Type>::digits10 = std::numeric_limits <Type>::digits10;

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
basic_generator <CharT, Traits>::OpenBracket (std::basic_ostream <CharT, Traits> & stream)
{
	stream .put (stream .widen ('['));

	stream << ListBreak << IncIndent;

	if (HasListBreak ())
		stream << Indent;

	return stream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
basic_generator <CharT, Traits>::CloseBracket (std::basic_ostream <CharT, Traits> & stream)
{
	stream << ListBreak << DecIndent;

	if (HasListBreak ())
		stream << Indent;

	stream .put (stream .widen (']'));
	return stream;
}

template <class CharT, class Traits>
std::basic_ostream <CharT, Traits> &
basic_generator <CharT, Traits>::EmptyBrackets (std::basic_ostream <CharT, Traits> & stream)
{
	stream .put (stream .widen ('['));
	stream << TidySpace;
	stream .put (stream .widen (']'));
	return stream;
}

template <class CharT, class Traits>
template <class Type>
std::basic_ostream <CharT, Traits> &
basic_generator <CharT, Traits>::Precision (std::basic_ostream <CharT, Traits> & stream)
{
	return stream << std::setprecision (NumericLimits <Type>::digits10);
}

typedef basic_generator <char> Generator;

} // io
} // titania

extern template std::ostream & titania::io::Generator::Comment       (std::ostream &);
extern template std::ostream & titania::io::Generator::Space         (std::ostream &);
extern template std::ostream & titania::io::Generator::TidySpace     (std::ostream &);
extern template std::ostream & titania::io::Generator::Break         (std::ostream &);
extern template std::ostream & titania::io::Generator::TidyBreak     (std::ostream &);
extern template std::ostream & titania::io::Generator::ListBreak     (std::ostream &);
extern template std::ostream & titania::io::Generator::Comma         (std::ostream &);
extern template std::ostream & titania::io::Generator::Indent        (std::ostream &);
extern template std::ostream & titania::io::Generator::IncIndent     (std::ostream &);
extern template std::ostream & titania::io::Generator::DecIndent     (std::ostream &);
extern template std::ostream & titania::io::Generator::OpenBracket   (std::ostream &);
extern template std::ostream & titania::io::Generator::CloseBracket  (std::ostream &);
extern template std::ostream & titania::io::Generator::EmptyBrackets (std::ostream &);

#endif
