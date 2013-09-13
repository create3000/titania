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

#include "Grammar.h"

#include "../../X3D.h"

namespace titania {
namespace X3D {

// VRML lexical elements
// General
io::sequence Grammar::whitespaces ("\r\n \t,");
io::comment  Grammar::comment ('#');

// Header
const pcrecpp::RE Grammar::Header ("(VRML|X3D) V(.*?) (utf8)(?: (.*?)[\r\n]*)?");

// Keywords
io::string Grammar::AS ("AS");
io::string Grammar::COMPONENT ("COMPONENT");
io::string Grammar::DEF ("DEF");
io::string Grammar::EXPORT ("EXPORT");
io::string Grammar::EXTERNPROTO ("EXTERNPROTO");
io::string Grammar::_false ("FALSE");
io::string Grammar::IMPORT ("IMPORT");
io::string Grammar::IS ("IS");
io::string Grammar::META ("META");
io::string Grammar::_null ("NULL");
io::string Grammar::PROFILE ("PROFILE");
io::string Grammar::PROTO ("PROTO");
io::string Grammar::ROUTE ("ROUTE");
io::string Grammar::TO ("TO");
io::string Grammar::_true ("TRUE");
io::string Grammar::UNIT ("UNIT");
io::string Grammar::USE ("USE");

io::string Grammar::initializeOnly ("initializeOnly");
io::string Grammar::inputOnly ("inputOnly");
io::string Grammar::outputOnly ("outputOnly");
io::string Grammar::inputOutput ("inputOutput");

io::string Grammar::field ("field");
io::string Grammar::eventIn ("eventIn");
io::string Grammar::eventOut ("eventOut");
io::string Grammar::exposedField ("exposedField");

std::set <std::string> Grammar::FieldType = std::move (getFieldType ());

// Terminal symbols
io::character Grammar::OpenBrace ('{');
io::character Grammar::CloseBrace ('}');
io::character Grammar::OpenBracket ('[');
io::character Grammar::CloseBracket (']');
io::character Grammar::Period ('.');
io::character Grammar::Colon (':');

// Values
io::string Grammar::hex ("0x");
io::string Grammar::HEX ("0X");

io::quoted_string Grammar::string ('\"');

io::sequence Grammar::ComponentName ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-");

bool
Grammar::Int32 (std::istream & istream, int32_t & _value)
{
	if (Hex (istream, (uint32_t &)_value))
		return true;

	return istream >> std::dec >> _value;
}

bool
Grammar::Hex (std::istream & istream, uint32_t & value)
{
	if (hex (istream) or HEX (istream))
		return istream >> std::hex >> value;
}

std::set <std::string>
Grammar::getFieldType ()
{
	std::set <std::string> FieldType;

	for (const auto & field : getBrowser () -> getSupportedFields ())
		FieldType .insert (field -> getTypeName ());

	return FieldType;
}

} // X3D
} // titania
