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

var::var () :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value (),
	                type (UNDEFINED)
{ }

var::var (const var & other) :
	 var ()
{
	*this = other;
}

var::var (var && other) :
	 value (other .value),
	  type (other .type)
{
	other .type = UNDEFINED;
}

var::var (const std::nullptr_t) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value (),
	               type (NULL_OBJECT)
{ }

var::var (const bool boolean) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ bool_: boolean }),
	                type (BOOLEAN)
{ }

var::var (const int32_t interger) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ int32_: interger }),
	                type (INT32)
{ }

var::var (const uint32_t interger) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ uint32_: interger }),
	                type (UINT32)
{ }

var::var (const double number) :
	pbOutputStreamObject (),
	               value ({ double_: number }),
	                type (DOUBLE)
{ }

var::var (const Glib::ustring & string) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ string_: new Glib::ustring (string) }),
	                type (STRING)
{ }

var::var (Glib::ustring && string) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ string_: new Glib::ustring () }),
	                type (STRING)
{
	value .string_ -> swap (string);
}

var::var (const std::string & string) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ string_: new Glib::ustring (string) }),
	                type (STRING)
{ }

var::var (pbBaseObject* const object) :
	pbOutputStreamObject (),
	  pbGarbageCollector (),
	               value ({ object_: new ptr <pbBaseObject> (object) }),
	                type (OBJECT)
{ }

var
var::copy (pbExecutionContext* const executionContext) const
{
	switch (type)
	{
		case STRING:
			return *value .string_;
		case OBJECT:
			//return value .object_ -> copy (executionContext);
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
			value .object_ = new ptr <pbBaseObject> (*other .value .object_);
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
var::operator = (const std::nullptr_t)
{
	clear ();

	type = NULL_OBJECT;

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
var::operator = (const int32_t integer)
{
	clear ();

	value .int32_ = integer;
	type          = INT32;

	return *this;
}

var &
var::operator = (const uint32_t integer)
{
	clear ();

	value .uint32_ = integer;
	type           = UINT32;

	return *this;
}

var &
var::operator = (const double number)
{
	clear ();

	value .double_ = number;
	type           = DOUBLE;

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
var::operator = (pbBaseObject* const object)
{
	clear ();

	value .object_ = new ptr <pbBaseObject> (object);
	type           = OBJECT;

	return *this;
}

bool
var::isPrimitive () const
{
	if (type == OBJECT)
		return value .object_ -> get () -> isPrimitive ();

	return true;
}

var
var::toPrimitive () const
{
	switch (type)
	{
		case UNDEFINED:
			return var ();
		case BOOLEAN:
			return value .bool_;
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return value .double_;
		case STRING:
			return *value .string_;
		case NULL_OBJECT:
			return nullptr;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive ();
	}

	return var ();
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
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return value .double_;
		case STRING:
			return not value .string_ -> empty ();
		case NULL_OBJECT:
			return false;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive () .toBoolean ();
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
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return value .double_;
		case STRING:
			return parseFloat (*value .string_);
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive () .toUInt16 ();
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
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return value .double_;
		case STRING:
			return parseFloat (*value .string_);
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive () .toInt32 ();
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
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return std::copysign (std::floor (std::abs (value .double_)), value .double_);
		case STRING:
		{
			const double number = parseFloat (*value .string_);

			return std::copysign (std::floor (std::abs (number)), number);
		}
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive () .toInteger ();
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
		case INT32:
			return value .int32_;
		case UINT32:
			return value .uint32_;
		case DOUBLE:
			return value .double_;
		case STRING:
			return parseFloat (*value .string_);
		case NULL_OBJECT:
			return 0;
		case OBJECT:
			return value .object_ -> get () -> toPrimitive () .toNumber ();
	}

	return 0;
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
		case INT32:
			ostream << value .int32_;
			return;
		case UINT32:
			ostream << value .uint32_;
			return;
		case DOUBLE:
		{
			if (isNaN (value .double_))
				ostream << "NaN";

			else if (value .double_ == NEGATIVE_INFINITY ())
				ostream << "-Infinity";

			else if (value .double_ == POSITIVE_INFINITY ())
				ostream << "Infinity";

			else
				ostream << std::setprecision (std::numeric_limits <double>::max_digits10) << value .double_;

			return;
		}
		case STRING:
			ostream << *value .string_;
			return;
		case NULL_OBJECT:
			ostream << "null";
			return;
		case OBJECT:
			ostream << *value .object_;
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
			addDisposedObject (value .object_);
			break;
		default:
			break;
	}

	type = UNDEFINED;
}

var::~var ()
{
	clear ();
}

} // pb
} // titania
