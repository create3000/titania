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

#include "var.h"

#include "../Bits/pbMath.h"
#include "../Objects/pbFunction.h"
#include "../Objects/pbObject.h"

#include <iomanip>

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
			value .object_  = GarbageCollector::getObject <ptr <pbBaseObject>> ();
			*value .object_ = *other .value .object_;
			break;
		default:
			value = other .value;
			break;
	}
}

var::var (pbBaseObject* const object) :
	pbOutputStreamObject (),
	               value ({ object_: GarbageCollector::getObject <ptr <pbBaseObject>> () }),
	                type (OBJECT)
{
	value .object_ -> reset (object);
}

var
var::copy (pbExecutionContext* executionContext) const
throw (pbException,
       pbControlFlowException)
{
	switch (type)
	{
		case STRING:
			return *value .string_;
		case OBJECT:
			return value .object_ -> get () -> copy (executionContext);
		default:
			return *this;
	}
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
			value .object_  = GarbageCollector::getObject <ptr <pbBaseObject>> ();
			*value .object_ = *other .value .object_;
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
	value = other .value;
	type  = other .type;

	other .type = UNDEFINED;
	
	return *this;
}

var &
var::operator = (const Undefined &)
{
	clear ();

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
var::operator = (const std::nullptr_t)
{
	clear ();

	type = NULL_OBJECT;

	return *this;
}

var &
var::operator = (pbBaseObject* const object)
{
	clear ();
	
	value .object_ = GarbageCollector::getObject <ptr <pbBaseObject>> ();
	value .object_ -> reset (object);
	type = OBJECT;

	return *this;
}

var
var::toPrimitive (const ValueType preferedType) const
throw (pbException)
{
	if (type == OBJECT)
		return value .object_ -> get () -> getDefaultValue (preferedType);

	return *this;
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
		{
			const auto v = value .object_ -> get () -> getValue ();

			if (v .isPrimitive ())
				return v .toBoolean ();

			return true;
		}
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
		case NULL_OBJECT:
			return 0;
		case NUMBER:
		default:
		{
			const double number = toNumber ();

			if (isNaN (number))
				return 0;

			if (number == POSITIVE_INFINITY ())
				return 0;

			if (number == NEGATIVE_INFINITY ())
				return 0;

			const double posInt   = std::copysign (std::floor (std::abs (number)), number);
			const double int16bit = std::fmod (posInt, M_2_16);

			return int16bit;
		}
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
		case NULL_OBJECT:
			return 0;
		default:
		{
			const double number = toNumber ();

			if (isNaN (number))
				return 0;

			if (number == POSITIVE_INFINITY ())
				return 0;

			if (number == NEGATIVE_INFINITY ())
				return 0;

			const double posInt   = std::copysign (std::floor (std::abs (number)), number);
			const double int32bit = std::fmod (posInt, M_2_32);

			if (int32bit >= M_2_31)
				return int32bit - M_2_32;

			return int32bit;
		}
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
			return std::copysign (std::floor (std::abs (value .number_)), value .number_);
		case STRING:
		{
			const double number = parseFloat (*value .string_);

			return std::copysign (std::floor (std::abs (number)), number);
		}
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

var
var::getValue () const
throw (pbException,
       pbControlFlowException)
{
	if (type == OBJECT)
		return value .object_ -> get () -> getValue ();

	return *this;
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
				ostream << std::setprecision (std::numeric_limits <double>::max_digits10) << value .number_;

			return;
		}
		case STRING:
			ostream << *value .string_;
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
			GarbageCollector::addObject (value .object_);
			break;
		default:
			break;
	}

	type = UNDEFINED;
}

} // pb
} // titania
