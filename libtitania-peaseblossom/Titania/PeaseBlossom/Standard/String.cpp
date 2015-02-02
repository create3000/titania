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

#include "Array.h"

#include "../Bits/pbMath.h"
#include "../Objects/pbObject.h"
#include "../Objects/String.h"
#include "../Objects/NativeFunction.h"

#include <locale>
//#include <codecvt>

namespace titania {
namespace pb {
namespace Standard {
namespace String {

/// String ([arg1])
struct Function
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (args .empty ())
			return "";

		return args [0] .toString ();
	}

};

/// new String ([arg1])
struct Constructor
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (args .empty ())
			return new pb::String (ec);

		return new pb::String (ec, args [0] .toString ());
	}

};

/// charAt (position)
struct charAt
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		const auto string = object .toString ();
		const auto length = string .length ();
		
		Glib::ustring::size_type position = 0;

		if (args .size () > 0)
			position = args [0] .toInteger ();

		if (position < 0)
			return "";
		
		if (position >= length)
			return "";

		return Glib::ustring (1, string [position]);
	}

};

/// charCodeAt (position)
struct charCodeAt
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		const auto string = object .toString ();
		const auto length = string .length ();
		
		Glib::ustring::size_type position = 0;

		if (args .size () > 0)
			position = args [0] .toInteger ();

		if (position < 0)
			return NaN ();
		
		if (position >= length)
			return NaN ();

		return string [position];

		//std::string source = string .substr (position, 1);
		//
		//std::wstring_convert <std::codecvt_utf8_utf16 <char16_t>, char16_t> convert;
		//std::u16string character = convert .from_bytes (source);    
		//
		//return character [0];
	}

};

/// indexOf (string [, start])
struct indexOf
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (args .size () == 0)
			return -1;

		const auto string = object .toString ();
		const auto length = string .length ();

		Glib::ustring::size_type start = 0;

		if (args .size () > 1)
			start = std::min <size_t> (std::max <size_t> (0, args [1] .toInteger ()), length);

		const auto pos = string .find (args [0] .toString (), start);

		return pos == Glib::ustring::npos ? -1 : pos;
	}

};

/// lastIndexOf (string [, start])
struct lastIndexOf
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (args .size () == 0)
			return -1;

		const auto string = object .toString ();
		const auto length = string .length ();

		Glib::ustring::size_type start = Glib::ustring::npos;

		if (args .size () > 1)
			start = std::min <size_t> (std::max <size_t> (0, args [1] .toInteger ()), length);

		const auto pos = string .rfind (args [0] .toString (), start);

		return pos == Glib::ustring::npos ? -1 : pos;
	}

};

/// substring (start [, end])
struct substring
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		const auto string = object .toString ();

		Glib::ustring::size_type start = 0;
		Glib::ustring::size_type end   = string .length ();

		if (args .size () > 0)
			start = std::min <size_t> (std::max <size_t> (0, args [0] .toInteger ()), end);

		if (args .size () > 1)
			end = std::min <size_t> (std::max <size_t> (0, args [1] .toInteger ()), end);

		if (start > end)
			std::swap (start, end);

		return string .substr (start, end - start);
	}

};

/// toString ()
struct toString
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (object .getType () == STRING)
			return object;

		if (object .getType () == OBJECT)
		{
			const auto string = dynamic_cast <pb::String*> (object .getObject () .get ());

			if (string)
				return string -> getValue ();
		}

		throw TypeError ("String.prototype.indexOf is not generic.");
	}

};

ptr <NativeFunction>
initialize (pbExecutionContext* const ec)
{
	const auto & standardObject = ec -> getStandardObject ();
	const auto   constructor    = make_ptr <NativeFunction> (ec, "String", Constructor { }, Function { }, 1);
	const auto   prototype      = new pb::String (ec, nullptr);

	prototype -> setConstructor (constructor);
	prototype -> setProto (standardObject);

	prototype -> addOwnProperty ("charAt",      new NativeFunction (ec, "charAt",      charAt { },      1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("charCodeAt",  new NativeFunction (ec, "charCodeAt",  charCodeAt { },  1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("indexOf",     new NativeFunction (ec, "indexOf",     indexOf { },     1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("lastIndexOf", new NativeFunction (ec, "lastIndexOf", lastIndexOf { }, 1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("substring",   new NativeFunction (ec, "substring",   substring { },   1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toString",    new NativeFunction (ec, "toString",    toString { },    0), WRITABLE | CONFIGURABLE);

	constructor -> addOwnProperty ("prototype", prototype, NONE);
	return constructor;
}

} // String
} // Standard
} // pb
} // titania
