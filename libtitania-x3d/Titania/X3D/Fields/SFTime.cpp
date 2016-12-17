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

#include <iomanip>

namespace titania {
namespace X3D {

const std::string             SFTime::typeName ("SFTime");
const X3DConstants::FieldType SFTime::type = X3DConstants::SFTime;

SFTime::SFTime () :
	X3DField <time_type> ()
{ }

SFTime::SFTime (const SFTime & field) :
	X3DField <time_type> (field)
{ }

SFTime::SFTime (const time_type value) :
	X3DField <time_type> (value)
{ }

std::string
SFTime::toUTCString () const
{
//	const std::time_t time = getValue ();
//
//	std::ostringstream ostream;
//
//	ostream .imbue (std::locale::classic ());
//	ostream << std::put_time (std::gmtime (&time), "%a, %d %b %Y %H:%M:%S %Z");
//
//	return ostream .str ();

	constexpr size_t BUFFER_SIZE = 128;

	const auto        locale = std::locale::global (std::locale::classic ());
	const std::time_t time   = getValue ();

	char buffer [BUFFER_SIZE];
	const size_t size = std::strftime (buffer, 80, "%a, %d %b %Y %H:%M:%S %Z", std::gmtime (&time));
	const std::string string (buffer, buffer + size);

	std::locale::global (locale);

	return string;
}

void
SFTime::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
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
	ostream << X3DGenerator::Precision <value_type> << getValue ();
}

void
SFTime::toXMLStream (std::ostream & ostream) const
{
	toStream (ostream);
}

void
SFTime::toJSONStream (std::ostream & ostream) const
{
}

} // X3D
} // titania
