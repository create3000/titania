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

#ifndef __TITANIA_X3D_PARSER_REG_EX_H__
#define __TITANIA_X3D_PARSER_REG_EX_H__

#include <pcrecpp.h>

namespace titania {
namespace X3D {

class RegEx
{
public:

	// VRML lexical elements
	// General
	static const pcrecpp::RE Header;
	static const pcrecpp::RE Whitespaces;
	static const pcrecpp::RE Comment;

	// Keywords
	static const pcrecpp::RE AS;
	static const pcrecpp::RE COMPONENT;
	static const pcrecpp::RE DEF;
	static const pcrecpp::RE EXPORT;
	static const pcrecpp::RE EXTERNPROTO;
	static const pcrecpp::RE _false;
	static const pcrecpp::RE IMPORT;
	static const pcrecpp::RE IS;
	static const pcrecpp::RE META;
	static const pcrecpp::RE _null;
	static const pcrecpp::RE PROFILE;
	static const pcrecpp::RE PROTO;
	static const pcrecpp::RE ROUTE;
	static const pcrecpp::RE TO;
	static const pcrecpp::RE _true;
	static const pcrecpp::RE UNIT;
	static const pcrecpp::RE USE;

	static const pcrecpp::RE inputOnly;
	static const pcrecpp::RE outputOnly;
	static const pcrecpp::RE inputOutput;
	static const pcrecpp::RE initializeOnly;

	// Terminal symbols
	static const pcrecpp::RE OpenBrace;
	static const pcrecpp::RE CloseBrace;
	static const pcrecpp::RE OpenBracket;
	static const pcrecpp::RE CloseBracket;
	static const pcrecpp::RE Period;
	static const pcrecpp::RE Colon;
	static const pcrecpp::RE Brackets;

	static const pcrecpp::RE QuotationMark;
	static const pcrecpp::RE EscapedQuotationMark;

	// Field Values Symbols
	static const pcrecpp::RE _hex;
	static const pcrecpp::RE _float;
	static const pcrecpp::RE _int32;
	static const pcrecpp::RE _double;
	static const pcrecpp::RE _string;

	// Other Symbols
	static const pcrecpp::RE Id;
	static const pcrecpp::RE ComponentNameId;
	static const pcrecpp::RE FieldType;
	static const pcrecpp::RE ScriptNodeInterfaceIS;

	// Execution

	static const pcrecpp::RE _Num;

	static const pcrecpp::RE ECMAScript;

};

} // X3D
} // titania

#endif
