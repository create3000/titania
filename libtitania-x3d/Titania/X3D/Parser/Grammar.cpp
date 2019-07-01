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

#include "Grammar.h"

#include "../Configuration/SupportedFields.h"
#include "../Parser/Colors.h"

#include <regex>

namespace titania {
namespace X3D {

// VRML lexical elements
// General
const io::sequence            Grammar::WhiteSpaces ("\r\n \t,");
const io::single_line_comment Grammar::Comment ("#");

// Keywords
const io::string Grammar::AS ("AS");
const io::string Grammar::COMPONENT ("COMPONENT");
const io::string Grammar::DEF ("DEF");
const io::string Grammar::EXPORT ("EXPORT");
const io::string Grammar::EXTERNPROTO ("EXTERNPROTO");
const io::multi_string Grammar::FALSE_ ("FALSE|false");
const io::string Grammar::IMPORT ("IMPORT");
const io::string Grammar::IS ("IS");
const io::string Grammar::META ("META");
const io::string Grammar::NULL_ ("NULL");
const io::string Grammar::PROFILE ("PROFILE");
const io::string Grammar::PROTO ("PROTO");
const io::string Grammar::ROUTE ("ROUTE");
const io::string Grammar::TO ("TO");
const io::multi_string Grammar::TRUE_ ("TRUE|true");
const io::string Grammar::UNIT ("UNIT");
const io::string Grammar::USE ("USE");

const io::multi_string Grammar::initializeOnly ("initializeOnly|field");
const io::multi_string Grammar::inputOnly ("inputOnly|eventIn");
const io::multi_string Grammar::outputOnly ("outputOnly|eventOut");
const io::multi_string Grammar::inputOutput ("inputOutput|exposedField");

// Terminal symbols
const io::character Grammar::OpenBrace ('{');
const io::character Grammar::CloseBrace ('}');
const io::character Grammar::OpenBracket ('[');
const io::character Grammar::CloseBracket (']');
const io::character Grammar::Period ('.');
const io::character Grammar::Colon (':');
const io::character Grammar::NumberSign ('#');

// Misc
const io::quoted_string Grammar::String ('\"');
const io::sequence      Grammar::ComponentName ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_");

// Values

const io::multi_string Grammar::PosInfinity ("+inf|inf|+Infinity|Infinity");
const io::multi_string Grammar::NegInfinity ("-inf|-Infinity");
const io::multi_string Grammar::PosNaN ("+nan|nan|+NaN|NaN");
const io::multi_string Grammar::NegNaN ("-nan|-NaN");
const io::multi_string Grammar::HEX ("0x|0X");

const io::sequence Grammar::NamedColor ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

const io::hex <int32_t> Grammar::HexValue;

// Locale

const io::sequence Grammar::WhiteSpacesNoCommaSequence ("\r\n \t");

bool
Grammar::Header (const std::string & header, std::string & encoding, std::string & specificationVersion, std::string & characterEncoding, std::string & comment)
{
	static const std::regex Header (R"/((VRML|X3D) V(.*?) (utf8)(?:[ \t]+(.*?)[ \t]*[\r\n]*)?)/");

	std::smatch match;

	if (std::regex_match (header, match, Header))
	{
		encoding             = match .str (1);
		specificationVersion = match .str (2);
		characterEncoding    = match .str (3);
		comment              = match .str (4);

		return true;
	}

	return false;
}

bool
Grammar::LongDouble (std::istream & istream, long double & value)
{
	return Number <long double> (istream, value);
}

bool
Grammar::Double (std::istream & istream, double & value)
{
	return Number <double> (istream, value);
}

bool
Grammar::Float (std::istream & istream, float & value)
{
	return Number <float> (istream, value);
}

bool
Grammar::Int32 (std::istream & istream, int32_t & value)
{
	if (Hex (istream, (uint32_t &) value))
		return true;

	return static_cast <bool> (istream >> std::dec >> value);
}

bool
Grammar::Hex (std::istream & istream, uint32_t & value)
{
	if (HEX (istream) .second)
		return static_cast <bool> (istream >> std::hex >> value);

	return false;
}

bool
Grammar::WhiteSpacesNoComma (std::istream & istream, std::string & string)
{
	if (istream .getloc () == std::locale::classic ())
		return WhiteSpaces (istream, string);

	return WhiteSpacesNoCommaSequence (istream, string);
}

} // X3D
} // titania
