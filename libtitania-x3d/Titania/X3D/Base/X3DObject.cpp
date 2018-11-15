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

#include "X3DObject.h"

#include <sstream>

namespace titania {
namespace X3D {

X3DObject::X3DObject () :
	 X3DInput (),
	X3DOutput (),
	     data ()
{ }

void
X3DObject::realize () const
{
	if (data)
		return;

	data .reset (new Data ());
}

void
X3DObject::setName (const std::string & value)
{
	realize ();
	data -> name = value;
}

const std::string &
X3DObject::getName () const
{
	realize ();
	return data -> name;
}

void
X3DObject::addComments (const std::vector <std::string> & value)
{
	realize ();
	data -> comments .insert (data -> comments .end (), value .cbegin (), value .cend ());
}

const std::vector <std::string> &
X3DObject::getComments () const
{
	realize ();
	return data -> comments;
}

void
X3DObject::setUserData (const UserDataPtr & value)
{
	realize ();
	data -> userData = value;
}

const UserDataPtr &
X3DObject::getUserData () const
{
	realize ();
	return data -> userData;
}

// String

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
bool
X3DObject::fromString (const std::string & string)
{
	return fromLocaleString (string, std::locale::classic ());
}

///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
bool
X3DObject::fromLocaleString (const std::string & string, const std::locale & locale)
{
	std::istringstream istringstream (string);

	istringstream .imbue (locale);

	fromStream (istringstream);

	return static_cast <bool> (istringstream);
}

std::string
X3DObject::toString () const
{
	return toLocaleString (std::locale::classic ());
}

std::string
X3DObject::toLocaleString (const std::locale & locale) const
{
	std::ostringstream ostringstream;

	ostringstream .imbue (locale);

	toStream (ostringstream);

	return ostringstream .str ();
}

std::string
X3DObject::toXMLString () const
{
	std::ostringstream ostringstream;

	ostringstream .imbue (std::locale::classic ());

	toXMLStream (ostringstream);

	return ostringstream .str ();
}

std::string
X3DObject::toJSONString () const
{
	std::ostringstream ostringstream;

	ostringstream .imbue (std::locale::classic ());

	toJSONStream (ostringstream);

	return ostringstream .str ();
}

// Object

void
X3DObject::dispose ()
{
	if (data)
	{
		if (data -> userData)
			data -> userData -> dispose ();
	}

	X3DInput::dispose  ();
	X3DOutput::dispose ();
}

X3DObject::~X3DObject ()
{ }

} // X3D
} // titania
