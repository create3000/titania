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

#include "Filter.h"

#include <regex>

namespace titania {
namespace X3D {

std::string
get_display_name (const SFNode & node)
{
	static const std::regex _TrailingNumbers (R"/(_\d+$)/");

	if (not node)
		return "NULL";

	return std::regex_replace (node -> getName (), _TrailingNumbers, "");
}

std::string
get_name_from_uri (const basic::uri & uri)
{
	return get_name_from_string (uri .basename (false));
}

std::string
get_name_from_string (const std::string & name_)
{
	static const std::regex Spaces (R"/(\s+)/");

	auto name = std::regex_replace (name_, Spaces, "_");

	filter_non_id_characters (name);

	if (not name .empty ())
		return name;

	return "Unnamed";
}

void
filter_non_id_characters (std::string & string)
{
	static const std::regex NonIdFirstChar (R"/(^[\x30-\x39\x00-\x20\x22\x23\x27\x2b\x2c\x2d\x2e\x5b\x5c\x5d\x7b\x7d\x7f]*)/");
	static const std::regex NonIdChars (R"/([\x00-\x20\x22\x23\x27\x2c\x2e\x5b\x5c\x5d\x7b\x7d\x7f])/");
	
	string = std::regex_replace (string, NonIdFirstChar, "");
	string = std::regex_replace (string, NonIdChars,     "");
}

void
filter_control_characters (std::string & string)
{
	static const std::regex ControlCharacters (R"/([\x00-\x08\x0b\x0c\x0e-\x1f])/");

	string = std::regex_replace (string, ControlCharacters, "");
}

void
filter_bad_utf8_characters (std::string & string)
{
//	static const std::regex UTF8Characters (R"/(([\000-\177])/"
//	                                        R"/(|[\300-\337][\200-\277])/"
//	                                        R"/(|[\340-\357][\200-\277]{2})/"
//	                                        R"/(|[\360-\367][\200-\277]{3})/"
//	                                        R"/(|[\370-\373][\200-\277]{4})/"
//	                                        R"/(|[\374-\375][\200-\277]{5})/"
//	                                        R"/()|.)/");

	static const std::regex UTF8Characters (R"/(([\x00-\x7f])/"
	                                        R"/(|[\xc0-\xdf][\x80-\xbf])/"
	                                        R"/(|[\xe0-\xef][\x80-\xbf]{2})/"
	                                        R"/(|[\xf0-\xf7][\x80-\xbf]{3})/"
	                                        R"/(|[\xf8-\xfb][\x80-\xbf]{4})/"
	                                        R"/(|[\xfc-\xfd][\x80-\xbf]{5})/"
	                                        R"/()|.)/");

	string = std::regex_replace (string, UTF8Characters, "$1");
}

std::string
escape_cdata (const std::string & string)
{
	static const std::regex  cdata_end_pattern (R"/((\]\]\>))/");
	static const std::string cdata_end_subs (R"/(\\]\\]\\>)/");

	return std::regex_replace (string, cdata_end_pattern, cdata_end_subs);
}

} // X3D
} // titania
