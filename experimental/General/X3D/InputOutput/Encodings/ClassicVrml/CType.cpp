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

#include "CType.h"

namespace titania {
namespace X3D {
namespace Encodings {
namespace ClassicVrml {

const std::vector <std::ctype_base::mask> CType <char>::table = CType <char>::create_table ();

CType <char>::CType (size_t refs) :
	std::ctype <char> (table .data (), false, refs)
{ }

bool
CType <char>::is_space (char c)
{
	switch (c)
	{
		case 0x0d: // return
		case 0x0a: // linefeed
		case 0x20: // space
		case 0x09: // tab
		case 0x2c: // comma
			return true;
		default:
			return false;
	}
}

//bool
//CType<char>::is_id_first_char (char c)
//{
//	if (c >= 0x30 and c <= 0x3a)
//		return false;
//
//	if (c >= 0x00 and c <= 0x20)
//		return false;
//
//	switch (c)
//	{
//		case 0x22:
//		case 0x23:
//		case 0x27:
//		case 0x2b:
//		case 0x2c:
//		case 0x2d:
//		case 0x2e:
//		case 0x5b:
//		case 0x5c:
//		case 0x5d:
//		case 0x7b:
//		case 0x7d:
//		case 0x7f:
//			return false;
//	}
//
//	return true;
//}
//
//bool
//CType<char>::is_id_rest_char (char c)
//{
//	if (c >= 0x00 and c <= 0x20)
//		return false;
//
//	switch (c)
//	{
//		case 0x22:
//		case 0x23:
//		case 0x27:
//		case 0x2c:
//		case 0x2e:
//		case 0x3a:
//		case 0x5b:
//		case 0x5c:
//		case 0x5d:
//		case 0x7b:
//		case 0x7d:
//		case 0x7f:
//			return false;
//	}
//
//	return true;
//}

std::vector <std::ctype_base::mask>
CType <char>::create_table ()
{
	std::vector <mask> table (classic_table (),
	                          classic_table () + table_size);

	for (size_t c = 0; c < table_size; ++ c)
	{
		table [c] &= ~space;

		if (is_space (c))
			table [c] |= space;
	}

	return table;
}

template class CType <char>;
template class CType <wchar_t>;

} // ClassicVrml
} // Encodings
} // X3D
} // titania
