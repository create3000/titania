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

#include "SFTime.h"

namespace titania {
namespace X3D {

template <>
const std::string SFTime::typeName ("SFTime");

template <>
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

SFTime*
SFTime::clone () const
{
	return new SFTime (*this);
}

const std::string &
SFTime::getTypeName () const
{
	return typeName;
}

X3DConstants::FieldType
SFTime::getType () const
{
	return type;
}

std::string
SFTime::toLocaleString () const
{
	time_t time = getValue ();

	char   buffer [80];
	size_t size = std::strftime (buffer, 80, "%a, %d %b %Y %H:%M:%S %Z", std::localtime (&time));

	std::string string (buffer, buffer + size);

	return string;
}

std::string
SFTime::toUTCString () const
{
	auto locale = std::locale::global (std::locale::classic ());

	time_t time = getValue ();

	char   buffer [80];
	size_t size = std::strftime (buffer, 80, "%a, %d %b %Y %H:%M:%S %Z", std::gmtime (&time));

	std::string string (buffer, buffer + size);

	std::locale::global (locale);

	return string;
}

void
SFTime::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{ }

void
SFTime::toStream (std::ostream & ostream) const
{
	ostream << Generator::Precision <scalar_type><< getValue ();
}

} // X3D
} // titania
