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

#include "SFTime.h"

#include <Titania/Chrono/Now.h>

#include <iomanip>

namespace titania {
namespace X3D {

const std::string SFTime::typeName = "SFTime";
const FieldType SFTime::type       = X3DConstants::SFTime;

SFTime::SFTime () :
	X3DField <time_type> ()
{ }

SFTime::SFTime (const SFTime & other) :
	X3DField <time_type> (other)
{ }

SFTime::SFTime (const time_type value) :
	X3DField <time_type> (value)
{ }

///  Returns the number of seconds elapsed since 1 January 1970 00:00:00 UTC.
time_type
SFTime::now ()
{
	return chrono::basic_now <double> ();
}

//int32_t
//SFTime:getUTCMilliseconds () const
//{
//	time_type intpart;
//
//	return std::modf (getValue (), &intpart) * 1000;
//}

//int32_t
//SFTime:getUTCSeconds () const
//{
//	return getUTCTM () .tm_sec;
//}

//int32_t
//SFTime:getUTCMinutes() const
//{
//	return getUTCTM () .tm_min;
//}

//int32_t
//SFTime:getUTCHours () const
//{
//	return getUTCTM () .tm_hour;
//}

//int32_t
//SFTime:getUTCDay () const
//{
//	return getUTCTM () .tm_mday;
//}

//int32_t
//SFTime:getUTCMonth () const
//{
//	return getUTCTM () .tm_mon;
//}

//int32_t
//SFTime:getUTCFullYear () const
//{
//	return 1900 + getUTCTM () .tm_year;
//}

//int32_t
//SFTime:getUTCTM () const
//{
//	const auto time = getValue ();
//
//	return std::gmtime (&time)
//}

///  Coverts a valid UTC @a string.
bool
SFTime::fromUTCString (const std::string & string)
{
	return fromLocaleString (string, "%a, %d %b %Y %H:%M:%S GMT", std::locale::classic ());
}

///  Converts the value of this field to a valid UTC string.
std::string
SFTime::toUTCString () const
{
	return toLocaleString ("%a, %d %b %Y %H:%M:%S %Z", std::locale::classic ());
}

///  Parses a date according to std::get_time with locale support. Returns true on success, otherwise false.
bool
SFTime::fromLocaleString (const std::string & string, const std::string & format, const std::locale & locale)
{
	std::istringstream isstream (string);

	isstream .imbue (locale);

   fromStream (isstream, format);

	return isstream .fail ();
}

///  Parses a date according to std::get_time.
void
SFTime::fromStream (std::istream & istream, const std::string & format)
{
	std::tm tm = { 0 };

   istream >> std::get_time (&tm, format .c_str ());

	if (istream .fail ())
		return;

	setValue (std::mktime (&tm));
}

///  Converts the value of this field to a time string specified by format according to std::put_time.
std::string
SFTime::toLocaleString (const std::string & format, const std::locale & locale) const
{
	std::ostringstream osstream;

	osstream .imbue (locale);

	toStream (osstream, format);

	return osstream .str ();
}

///  Converts the value of this field to a time string specified by format according to std::put_time.
void
SFTime::toStream (std::ostream & ostream, const std::string & format) const
{
	const std::time_t time = getValue ();

	ostream << std::put_time (std::gmtime (&time), format .c_str ());
}

void
SFTime::fromStream (std::istream & istream)
{
	std::string whiteSpaces;
	
	Grammar::WhiteSpacesNoComma (istream, whiteSpaces);

	time_type value = 0;

	if (Grammar::Double (istream, value))
		setValue (value);
}

void
SFTime::toStream (std::ostream & ostream) const
{
	ostream << X3DGenerator::SetPrecision <value_type> << getValue ();
}

void
SFTime::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

void
SFTime::toJSONStream (std::ostream & ostream) const
{
	toStream (ostream);
}

} // X3D
} // titania
