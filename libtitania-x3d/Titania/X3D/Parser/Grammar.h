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

#ifndef __TITANIA_X3D_PARSER_GRAMMAR_H__
#define __TITANIA_X3D_PARSER_GRAMMAR_H__

#include "../Types/Numbers.h"
#include "../Types/String.h"

#include <Titania/InputOutput.h>
#include <set>

namespace titania {
namespace X3D {

// VRML lexical elements
class Grammar
{
public:

	template <class Type>
	static
	std::enable_if_t <
		std::is_floating_point_v <Type>,
		bool>
	Number (std::istream & istream, Type & value);

	static
	bool
	LongDouble (std::istream & istream, long double & value);

	static
	bool
	Double (std::istream & istream, double & value);

	static
	bool
	Float (std::istream & istream, float & value);

	static
	bool
	Int32 (std::istream & istream, int32_t & value);

	static
	bool
	Hex (std::istream & istream, uint32_t & value);

	///  @name General
	static const io::sequence            WhiteSpaces;
	static const io::single_line_comment Comment;

	///  @name Header
	static
	bool
	Header (const std::string &, std::string &, std::string &, std::string &, std::string &);

	///  @name Keywords
	static const io::string AS;
	static const io::string COMPONENT;
	static const io::string DEF;
	static const io::string EXPORT;
	static const io::string EXTERNPROTO;
	static const io::multi_string FALSE_;
	static const io::string IMPORT;
	static const io::string IS;
	static const io::string META;
	static const io::string NULL_;
	static const io::string PROFILE;
	static const io::string PROTO;
	static const io::string ROUTE;
	static const io::string TO;
	static const io::multi_string TRUE_;
	static const io::string UNIT;
	static const io::string USE;

	static const io::multi_string initializeOnly;
	static const io::multi_string inputOnly;
	static const io::multi_string outputOnly;
	static const io::multi_string inputOutput;

	///  @name Terminal symbols
	static const io::character OpenBrace;
	static const io::character CloseBrace;
	static const io::character OpenBracket;
	static const io::character CloseBracket;
	static const io::character Period;
	static const io::character Colon;
	static const io::character NumberSign;

	static const io::quoted_string String;
	static const io::sequence      ComponentName;

	// Locale

	static
	bool
	WhiteSpacesNoComma (std::istream &, std::string &);

	static const io::sequence NamedColor;

	static const io::hex <int32_t> HexValue;


private:

	///  @name Values

	static const io::multi_string PosInfinity;
	static const io::multi_string NegInfinity;
	static const io::multi_string PosNaN;
	static const io::multi_string NegNaN;
	static const io::multi_string HEX;

	static const io::sequence WhiteSpacesNoCommaSequence;

};

template <class Type>
std::enable_if_t <
	std::is_floating_point_v <Type>,
	bool>
Grammar::Number (std::istream & istream, Type & value)
{
	const auto pos = istream .tellg ();

	if (istream >> value)
		return true;

	istream .clear ();

	for (size_t i = 0, size = istream .tellg () - pos; i < size; ++ i)
		istream .unget ();

	if (PosInfinity (istream) .second)
	{
		value = std::numeric_limits <Type>::infinity ();
		return true;
	}

	if (NegInfinity (istream) .second)
	{
		value = -std::numeric_limits <Type>::infinity ();
		return true;
	}

	if (PosNaN (istream) .second)
	{
		value = std::numeric_limits <Type>::quiet_NaN ();
		return true;
	}

	if (NegNaN (istream) .second)
	{
		value = -std::numeric_limits <Type>::quiet_NaN ();
		return true;
	}

	istream .setstate (std::ios::failbit);
	return false;
}

} // X3D
} // titania

#endif
