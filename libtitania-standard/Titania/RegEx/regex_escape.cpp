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

#include "regex_escape.h"

namespace titania {
namespace basic {

std::string
regex_escape (const std::string & unquoted)
{
	// Taken from pcrecpp RE::QuoteMeta

	std::string result;

	// Escape any ascii character not in [A-Za-z_0-9].
	//
	// Note that it's legal to escape a character even if it has no
	// special meaning in a regular expression -- so this function does
	// that.  (This also makes it identical to the perl function of the
	// same name; see `perldoc -f quotemeta`.)  The one exception is
	// escaping NUL: rather than doing backslash + NUL, like perl does,
	// we do '\0', because pcre itself doesn't take embedded NUL chars.
	for (size_t ii = 0; ii < unquoted .size (); ++ ii)
	{
		// Note that using 'isalnum' here raises the benchmark time from
		// 32ns to 58ns:
		if (unquoted [ii] == '\0')
		{
			result += "\\0";
		}
		else if ((unquoted [ii] < 'a' or unquoted [ii] > 'z') &&
		         (unquoted [ii] < 'A' or unquoted [ii] > 'Z') &&
		         (unquoted [ii] < '0' or unquoted [ii] > '9') &&
		         unquoted [ii] not_eq '_' &&
		         // If this is the part of a UTF8 or Latin1 character, we need
		         // to copy this byte without escaping.  Experimentally this is
		         // what works correctly with the regexp library.
		         ! (unquoted [ii] & 128))
		{
			result += '\\';
			result += unquoted [ii];
		}
		else
		{
			result += unquoted [ii];
		}
	}

	return result;
}

} // basic
} // titania
