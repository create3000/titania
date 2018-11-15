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

#include "var.h"

#include "../Bits/pbMath.h"
#include "../Objects/pbFunction.h"
#include "../Objects/pbObject.h"
#include "../Objects/Array.h"
#include "../Objects/Boolean.h"
#include "../Objects/Number.h"
#include "../Objects/String.h"

#include <iomanip>

#include <Titania/String/dtoa.h>

namespace titania {
namespace pb {

var::var (const var & other) :
	 value (),
	  type (other .type)
{
	switch (type)
	{
		case STRING:
			value .string_ = new Glib::ustring (*other .value .string_);
			break;
		case OBJECT:
			value .object_ = Cache <ptr <pbObject>>::get (*other .value .object_);
			break;
		default:
			value = other .value;
			break;
	}
}

var::var (pbObject* const object) :
	pbOutputStreamObject (),
	               value ({ object_: Cache <ptr <pbObject>>::get (object) }),
	                type (OBJECT)
{
	assert (*value .object_);
}

var &
var::operator = (const var & other)
{
	clear ();
	
	type = other .type;

	switch (type)
	{
		case STRING:
			value .string_ = new Glib::ustring (*other .value .string_);
			break;
		case OBJECT:
			value .object_ = Cache <ptr <pbObject>>::get (*other .value .object_);
			break;
		default:
			value = other .value;
			break;
	}

	return *this;
}

var &
var::operator = (var && other)
{
	clear ();

	value = other .value;
	type  = other .type;

	other .type = UNDEFINED;
	
	return *this;
}

var &
var::operator = (const UndefinedType &)
{
	clear ();

	type = UNDEFINED;

	return *this;
}

var &
var::operator = (const bool boolean)
{
	clear ();

	value .bool_ = boolean;
	type         = BOOLEAN;

	return *this;
}

var &
var::operator = (const double number)
{
	clear ();

	value .number_ = number;
	type           = NUMBER;

	return *this;
}

var &
var::operator = (const Glib::ustring & string)
{
	clear ();

	value .string_ = new Glib::ustring (string);
	type           = STRING;

	return *this;
}

var &
var::operator = (Glib::ustring && string)
{
	clear ();

	value .string_ = new Glib::ustring ();
	type           = STRING;

	value .string_ -> swap (string);

	return *this;
}

var &
var::operator = (const std::string & string)
{
	clear ();

	value .string_ = new Glib::ustring (string);
	type           = STRING;

	return *this;
}

var &
var::operator = (const std::string::value_type* const string)
{
	clear ();

	value .string_ = new Glib::ustring (string);
	type           = STRING;

	return *this;
}

var &
var::operator = (const std::nullptr_t)
{
	clear ();

	type = NULL_OBJECT;

	return *this;
}

var &
var::operator = (pbObject* const object)
{
	clear ();
	
	value .object_ = Cache <ptr <pbObject>>::get (object);
	type           = OBJECT;

	assert (*value .object_);

	return *this;
}

///  throws pbError
var
var::toPrimitive (const ValueType preferedType) const
{
	switch (type)
	{
		case UNDEFINED:
			return undefined;
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return value .number_;
		case STRING:
			return *value .string_;
		case NULL_OBJECT:
			return nullptr;
		case OBJECT:
			return value .object_ -> get () -> getDefaultValue (preferedType);
	}

	return undefined;
}

bool
var::toBoolean () const
{
	switch (type)
	{
		case UNDEFINED:
			return false;
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return value .number_;
		case STRING:
			return not value .string_ -> empty ();
		case NULL_OBJECT:
			return false;
		case OBJECT:
			return true;
	}

	return false;
}

uint16_t
var::toUInt16 () const
{
	switch (type)
	{
		case UNDEFINED:
			return 0;
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return pb::toUInt16 (value .number_);
		case STRING:
			return pb::toUInt16 (*value .string_);
		case NULL_OBJECT:
			return 0;
		default:
			return pb::toUInt16 (value .object_ -> get () -> getDefaultValue (NUMBER) .toNumber ());
	}

	return 0;
}

int32_t
var::toInt32 () const
{
	switch (type)
	{
		case UNDEFINED:
			return 0;
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return pb::toInt32 (value .number_);
		case STRING:
			return pb::toInt32 (*value .string_);
		case NULL_OBJECT:
			return 0;
		default:
			return pb::toInt32 (value .object_ -> get () -> getDefaultValue (NUMBER) .toNumber ());
	}

	return 0;
}

double
var::toInteger () const
{
	switch (type)
	{
		case UNDEFINED:
			return NaN ();
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return pb::toInteger (value .number_);
		case STRING:
			return parseInt (*value .string_);
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> getDefaultValue (NUMBER) .toInteger ();
	}

	return 0;
}

double
var::toNumber () const
{
	switch (type)
	{
		case UNDEFINED:
			return NaN ();
		case BOOLEAN:
			return value .bool_;
		case NUMBER:
			return value .number_;
		case STRING:
			return parseFloat (*value .string_);
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> getDefaultValue (NUMBER) .toNumber ();
	}

	return 0;
}

///  throws TypeError
ptr <pbObject>
var::toObject (const ptr <pbExecutionContext> & ec) const
{
	switch (type)
	{
		case UNDEFINED:
			throw TypeError ("Cannot coerce 'undefined' into object.");
		case BOOLEAN:
			return new Boolean (ec, value .bool_);
		case NUMBER:
			return new Number (ec, value .number_);
		case STRING:
			return new String (ec, *value .string_);
		case NULL_OBJECT:
			throw TypeError ("Cannot coerce 'null' into object.");
		case OBJECT:
			return *value .object_;
	}

	throw std::invalid_argument ("toObject");
}

Glib::ustring
var::toLocaleString (const std::locale & locale) const
{
	switch (type)
	{
		case UNDEFINED:
			return "undefined";
		case BOOLEAN:
			return value .bool_ ? "true" : "false";
		case NUMBER:
		{
			if (isNaN (value .number_))
				return "NaN";

			else if (value .number_ == NEGATIVE_INFINITY ())
				return "-Infinity";

			else if (value .number_ == POSITIVE_INFINITY ())
				return "Infinity";

			char buffer [32];

			dtoa (value .number_, buffer);

			return buffer;
		}
		case STRING:
			return *value .string_;
		case NULL_OBJECT:
			return "null";
		case OBJECT:
			return value .object_ -> get () -> getDefaultValue (STRING) .toString ();
	}

	return "";
}

void
var::toStream (std::ostream & ostream) const
{
	switch (type)
	{
		case UNDEFINED:
			ostream << "undefined";
			return;
		case BOOLEAN:
			ostream << (value .bool_ ? "true" : "false");
			return;
		case NUMBER:
		{
			if (isNaN (value .number_))
				ostream << "NaN";

			else if (value .number_ == NEGATIVE_INFINITY ())
				ostream << "-Infinity";

			else if (value .number_ == POSITIVE_INFINITY ())
				ostream << "Infinity";

			else
				ostream << std::setprecision (std::numeric_limits <double>::max_digits10 - 1) << value .number_;

			return;
		}
		case STRING:
			ostream << value .string_ -> raw ();
			return;
		case NULL_OBJECT:
			ostream << "null";
			return;
		case OBJECT:
			ostream << value .object_ -> get () -> getDefaultValue (STRING);
			return;
	}
}

void
var::clear ()
{
	switch (type)
	{
		case STRING:
			delete value .string_;
			break;
		case OBJECT:
			value .object_ -> dispose ();
			Cache <ptr <pbObject>>::add (value .object_);
			break;
		default:
			break;
	}
}

} // pb
} // titania
