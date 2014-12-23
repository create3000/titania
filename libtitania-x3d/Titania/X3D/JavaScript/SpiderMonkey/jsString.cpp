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

#include "jsString.h"

#include <Titania/LOG.h>
#include <glibmm/main.h>

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSBool
JS_NewStringValue (JSContext* const cx, const std::string & string, jsval* const vp)
{
	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	gunichar2* utf16_string = g_utf8_to_utf16 (string .c_str (), string .length (), &items_read, &items_written, &error);

	if (error)
	{
		__LOG__ << g_quark_to_string (error -> domain) << ": " << error -> code << ": " << error -> message << std::endl;
		return false;
	}

	JSString* const result = JS_NewUCStringCopyN (cx, utf16_string, items_written);

	g_free (utf16_string);

	if (result)
	{
		*vp = STRING_TO_JSVAL (result);

		return true;
	}

	return false;
}

std::string
to_string (JSContext* const cx, JSString* const jsstring)
{
	if (jsstring)
	{
		size_t utf16_length = 0;

		const jschar* utf16_string = JS_GetStringCharsAndLength (cx, jsstring, &utf16_length);

		glong   items_read    = 0;
		glong   items_written = 0;
		GError* error         = nullptr;

		char* const utf8_string = g_utf16_to_utf8 (utf16_string, utf16_length, &items_read, &items_written, &error);

		if (error)
		{
			__LOG__ << g_quark_to_string (error -> domain) << ": " << error -> code << ": " << error -> message << std::endl;
			return "";
		}

		std::string string (utf8_string, items_written);

		g_free (utf8_string);

		return string;
	}

	return "";
}

} // MozillaSpiderMonkey
} // X3D
} // titania
