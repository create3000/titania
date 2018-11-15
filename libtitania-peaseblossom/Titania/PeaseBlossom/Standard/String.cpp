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

///  Checks whether this object can be converted into an object.
///  throws TypeError
void
CheckObjectCoercible (const var & value)
{
	switch (value .getType ())
	{
		case UNDEFINED:
		case NULL_OBJECT:
			throw TypeError ("Argument list has wrong type.");
		default:
			return;
	}
}

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
		if (not args .empty ())
			static_cast <pb::String*> (object .getObject () .get ()) -> setValue (args [0] .toString ());

		return object;
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

/// concat ([string1 [, string2 [, ...]]])
struct concat
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		auto string = object .toString ();

		for (const auto & arg : args)
			string += arg .toString ();

		return string;
	}

};

/// localeCompare (that)
struct localeCompare
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (args .empty ())
			return -1;	

		const auto string = object .toString ();
		const auto that   = args [0] .toString ();

		const auto lhs = string .collate_key ();
		const auto rhs = that .collate_key ();

		if (lhs == rhs)
			return 0;

		if (lhs < rhs)
			return -1;

		return 1;
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

/// toLowerCase ()
struct toLowerCase
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		return object .toString () .lowercase ();
	}

};

/// toLowerCase ()
struct toUpperCase
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		return object .toString () .uppercase ();
	}

};

/// trim ()
struct trim
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		const auto string = object .toString ();
		
		if (string .empty ())
			return string;

		Glib::ustring::size_type length = string .length ();
		Glib::ustring::size_type start  = 0;
		Glib::ustring::size_type end    = length - 1;

		for (; start < length; ++ start)
		{
			if (not g_unichar_isspace (string [start]))
				break;
		}

		for (; end > start; -- end)
		{
			if (not g_unichar_isspace (string [end]))
				break;
		}

		return string .substr (start, end - start + 1);
	}

};

/// toString ()
struct toString
{
	var
	operator () (const ptr <pbExecutionContext> & ec, const var & object, const std::vector <var> & args)
	{
		if (object .isString ())
			return object;
			
		if (object .isObject ())
		{
			const auto string = dynamic_cast <pb::String*> (object .getObject () .get ());

			if (string)
				return string -> getValue ();
		}

		throw TypeError ("String.prototype." + name + " is not generic.");
	}

	const std::string name;

};

ptr <NativeFunction>
initialize (pbExecutionContext* const ec)
{
	const auto & standardObject = ec -> getStandardObject ();
	const auto   constructor    = make_ptr <NativeFunction> (ec, "String", Constructor { }, Function { }, 1);
	const auto   prototype      = new pb::String (ec, nullptr);

	prototype -> setConstructor (constructor);
	prototype -> setProto (standardObject);

	prototype -> addOwnProperty ("charAt",            new NativeFunction (ec, "charAt",            charAt { },              1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("charCodeAt",        new NativeFunction (ec, "charCodeAt",        charCodeAt { },          1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("concat",            new NativeFunction (ec, "concat",            concat { },              1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("localeCompare",     new NativeFunction (ec, "localeCompare",     localeCompare { },       1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("indexOf",           new NativeFunction (ec, "indexOf",           indexOf { },             1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("lastIndexOf",       new NativeFunction (ec, "lastIndexOf",       lastIndexOf { },         1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("substring",         new NativeFunction (ec, "substring",         substring { },           1), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toLowerCase",       new NativeFunction (ec, "toLowerCase",       toLowerCase { },         0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toLocaleLowerCase", new NativeFunction (ec, "toLocaleLowerCase", toLowerCase { },         0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toUpperCase",       new NativeFunction (ec, "toUpperCase",       toUpperCase { },         0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toLocaleUpperCase", new NativeFunction (ec, "toLocaleUpperCase", toUpperCase { },         0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("trim",              new NativeFunction (ec, "trim",              trim { },                0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("valueOf",           new NativeFunction (ec, "valueOf",           toString { "valueOf" },  0), WRITABLE | CONFIGURABLE);
	prototype -> addOwnProperty ("toString",          new NativeFunction (ec, "toString",          toString { "toString" }, 0), WRITABLE | CONFIGURABLE);

	constructor -> addOwnProperty ("prototype", prototype, NONE);
	return constructor;
}

} // String
} // Standard
} // pb
} // titania
