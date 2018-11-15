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

#include "SFString.h"

#include "../InputOutput/Generator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Parser/MiniParser.h"
#include "../Parser/Grammar.h"

namespace titania {
namespace X3D {

template <>
const std::string X3DField <String>::typeName = "SFString";

template <>
const FieldType X3DField <String>::type = X3DConstants::SFString;

template class X3DField <String>;

SFString::SFString () :
	X3DField <String> ()
{ }

SFString::SFString (const SFString & other) :
	X3DField <String> (other)
{ }

SFString::SFString (SFString && other) :
	SFString (std::move (other .get ()))
{
	other .addEvent ();
}

SFString::SFString (const String & value) :
	X3DField <String> (value)
{ }

SFString::SFString (String && value) :
	X3DField <String> ()
{
	get () .swap (value);
	value .clear ();
}

SFString::SFString (const std::string & value) :
	X3DField <String> (value)
{ }

SFString::SFString (const char_type* const value) :
	X3DField <String> (value)
{ }

SFString &
SFString::operator = (const SFString & other)
{
	setValue (other);
	return *this;
}

SFString &
SFString::operator = (SFString && other)
{
	*this = std::move (other .get ());
	other .addEvent ();
	return *this;
}

SFString &
SFString::operator = (String && value)
{
	get () .swap (value);
	value .clear ();
	addEvent ();
	return *this;
}

SFString &
SFString::operator = (const String & other)
{
	setValue (other);
	return *this;
}

SFString &
SFString::operator = (const std::string & value)
{
	setValue (value);
	return *this;
}

SFString &
SFString::operator = (const char_type* const value)
{
	setValue (value);
	return *this;
}

SFString::operator const std::string & () const
{
	return getValue () .raw ();
}

const std::string &
SFString::str () const
{
	return getValue () .raw ();
}

const SFString::char_type*
SFString::c_str () const
{
	return getValue () .c_str ();
}

bool
SFString::empty () const
{
	return getValue () .empty ();
}

SFString::size_type
SFString::length () const
{
	return getValue () .length ();
}

void
SFString::clear ()
{
	get () .clear ();
	addEvent ();
}

SFString &
SFString::append (const String & string)
{
	get () .append (string);
	addEvent ();
	return *this;
}

SFString &
SFString::append (const char_type & character)
{
	get () += character;
	addEvent ();
	return *this;
}

void
SFString::fromStream (std::istream & istream)
{
	String value;

	if (MiniParser::Decode (istream, value))
		*this = std::move (value);
}

void
SFString::toStream (std::ostream & ostream) const
{
	VRMLGenerator::Encode (ostream, getValue (), getUnit ());
}

void
SFString::toXMLStream (std::ostream & ostream) const
{
	XMLGenerator::Encode (ostream, getValue (), getUnit ());
}

void
SFString::toJSONStream (std::ostream & ostream) const
{
	JSONGenerator::Encode (ostream, getValue (), getUnit ());
}

} // X3D
} // titania
