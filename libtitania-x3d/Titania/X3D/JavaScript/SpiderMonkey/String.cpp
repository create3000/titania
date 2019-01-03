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

#include "String.h"

#include <js/Conversions.h>

#include <glibmm/main.h>
#include <Titania/LOG.h>

namespace titania {
namespace X3D {
namespace spidermonkey {

JS::Value
StringValue (JSContext* const cx, const std::string & string)
{
	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	const auto utf16_string = g_utf8_to_utf16 (string .c_str (), string .length (), &items_read, &items_written, &error);

	if (error)
		throw std::invalid_argument (error -> message);

	if (not utf16_string)
		throw std::invalid_argument ("out of memory");

	const auto jsstring = JS_NewUCStringCopyN (cx, (char16_t*) utf16_string, items_written);

	g_free (utf16_string);

	if (not jsstring)
		throw std::invalid_argument ("out of memory");

	return JS::StringValue (jsstring);
}

std::string
to_string (JSContext* const cx, JSString* const jsstring)
{
	if (not jsstring)
		return "";

	JS::RootedString str (cx, jsstring);

	JSAutoByteString bytes;

	const auto utf8_string = bytes .encodeUtf8 (cx, str);

	if (not utf8_string)
		return "";

	return std::string (utf8_string);
}

std::string
to_string (JSContext* const cx, const JS::HandleValue & value)
{
	return to_string (cx, JS::ToString (cx, value));
}

std::string
to_string (const char16_t* ucstring, const size_t length)
{
	if (not ucstring)
		return "";

	glong   items_read    = 0;
	glong   items_written = 0;
	GError* error         = nullptr;

	const auto utf8string = g_utf16_to_utf8 ((gunichar2*) ucstring,
	                                         length,
	                                         &items_read,
	                                         &items_written,
	                                         &error);

	if (error)
		return "";

	if (not utf8string)
		return "";

	const auto string = std::string  (utf8string, utf8string + items_written);

	g_free (utf8string);

	return string;
}

} // spidermonkey
} // X3D
} // titania
