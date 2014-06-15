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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_DEBUG_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_DEBUG_H__

namespace titania {
namespace pb {

inline
void
debug_roots (jsChildObject* node, std::set <jsChildObject*> & seen)
{
	if (not seen .emplace (node) .second)
		return;

	static int i = 0;

	__LOG__ << std::string (i, '\t') << node -> getParents () .size () << " : " << node -> getTypeName () << " ";

	const auto value = dynamic_cast <jsValue*> (node);
	
	if (value)
	{
		switch (value -> getType ())
		{
			case UNDEFINED:            std::clog << "UNDEFINED";            break;
			case BOOLEAN:              std::clog << "BOOLEAN";              break;
			case NUMBER:               std::clog << "NUMBER";               break;
			case STRING:               std::clog << "STRING";               break;
			case NULL_OBJECT:          std::clog << "NULL_OBJECT";          break;
			case OBJECT:               std::clog << "OBJECT";               break;
			case BOOLEAN_OBJECT:       std::clog << "BOOLEAN_OBJECT";       break;
			case NUMBER_OBJECT:        std::clog << "NUMBER_OBJECT";        break;
			case STRING_OBJECT:        std::clog << "STRING_OBJECT";        break;
			case ARRAY_OBJECT:         std::clog << "ARRAY_OBJECT";         break;
			case DATE_OBJECT:          std::clog << "DATE_OBJECT";          break;
			case REGEX_OBJECT:         std::clog << "REGEX_OBJECT";         break;
			case FUNCTION_OBJECT:      std::clog << "FUNCTION_OBJECT";      break;
			case ADDITION:             std::clog << "ADDITION";             break;
			case DIVISION:             std::clog << "DIVISION";             break;
			case SUBTRACTION:          std::clog << "SUBTRACTION";          break;
			case IDENTIFIER:           std::clog << "IDENTIFIER";           break;
			case MULTIPLICATION:       std::clog << "MULTIPLICATION";       break;
			case REMAINDER:            std::clog << "REMAINDER";            break;
			case VARIABLE_DECLARATION: std::clog << "VARIABLE_DECLARATION"; break;
			case CUSTOM:               std::clog << "CUSTOM";               break;
		}
	}
	
	std::clog << std::endl;

	if (node -> getParents () .size ())
	{
		++ i;

		for (auto & child : node -> getParents ())
			debug_roots (child, seen);

		-- i;

		return;
	}
}

inline
void
debug_roots (jsChildObject* node)
{
	std::set <jsChildObject*> seen;
	debug_roots (node, seen);
}

} // pb
} // titania

#endif
