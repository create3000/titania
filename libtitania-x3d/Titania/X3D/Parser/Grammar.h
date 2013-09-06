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

#ifndef __TITANIA_X3D_PARSER_GRAMMAR_H__
#define __TITANIA_X3D_PARSER_GRAMMAR_H__

#include <Titania/InputOutput.h>
#include <pcrecpp.h>
#include <set>

namespace titania {
namespace X3D {

// VRML lexical elements
class Grammar
{
public:

	static
	bool
	Int32 (std::istream &, int32_t &);

	///  @name General
	static io::sequence whitespaces;
	static io::comment  comment;

	///  @name Header
	static const pcrecpp::RE Header;

	///  @name Keywords
	static io::string AS;
	static io::string COMPONENT;
	static io::string DEF;
	static io::string EXPORT;
	static io::string EXTERNPROTO;
	static io::string _false;
	static io::string IMPORT;
	static io::string IS;
	static io::string META;
	static io::string _null;
	static io::string PROFILE;
	static io::string PROTO;
	static io::string ROUTE;
	static io::string TO;
	static io::string _true;
	static io::string UNIT;
	static io::string USE;

	static io::string initializeOnly;
	static io::string inputOnly;
	static io::string outputOnly;
	static io::string inputOutput;

	static io::string field;
	static io::string eventIn;
	static io::string eventOut;
	static io::string exposedField;

	static std::set <std::string> FieldType;

	///  @name Terminal symbols
	static io::character OpenBrace;
	static io::character CloseBrace;
	static io::character OpenBracket;
	static io::character CloseBracket;
	static io::character Period;
	static io::character Colon;

	static io::quoted_string string;
	static io::sequence      ComponentName;

private:

	static
	bool
	Hex (std::istream &, uint32_t &);

	static
	std::set <std::string>
	getFieldType ();

	///  @name Values
	static io::string hex;
	static io::string HEX;

};

} // X3D
} // titania

#endif
