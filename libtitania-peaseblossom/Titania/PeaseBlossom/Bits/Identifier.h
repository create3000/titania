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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_BITS_IDENTIFIER_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_BITS_IDENTIFIER_H__

#include "../Base/pbOutputStreamObject.h"

#include <map>
#include <string>

namespace titania {
namespace pb {

class Identifier :
	public pbOutputStreamObject
{
public:

	///  @name Construction

	Identifier () :
		pbOutputStreamObject (),
		                name (),
		                  id (-1)
	{ }

	Identifier (const Identifier & other) :
		pbOutputStreamObject (),
		                name (other .name),
		                  id (other .id)
	{ }

	Identifier (Identifier && other) :
		pbOutputStreamObject (),
		                name (std::move (other .name)),
		                  id (other .id)
	{ }

	Identifier (const Glib::ustring & name) :
		Identifier (getIdentifier (name .raw ()))
	{ }

	Identifier (const std::string & name) :
		Identifier (getIdentifier (name))
	{ }

	Identifier (std::string && name) :
		Identifier (getIdentifier (std::move (name)))
	{ }

	Identifier (const std::string::value_type* name) :
		Identifier (getIdentifier (name))
	{ }

	///  @name Member functions

	Identifier &
	operator = (const Identifier & other)
	{
		name = other .name;
		id   = other .id;
		return *this;
	}

	Identifier &
	operator = (Identifier && other)
	{
		name = std::move (other .name);
		id   = other .id;
		return *this;
	}

	///  @name Member access

	const std::string &
	getName () const
	{ return name; }

	const size_t &
	getId () const
	{ return id; }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ ostream << name; }


private:

	///  @name Member types

	using IdentifierIndex = std::map <std::string, size_t>;

	///  @name Construction

	Identifier (const IdentifierIndex::value_type & pair) :
		pbOutputStreamObject (),
		                name (pair .first),
		                  id (pair .second)
	{ }

	///  @name Operations

	static
	const IdentifierIndex::value_type &
	getIdentifier (const std::string & identifier)
	{ return *identifiers .emplace (identifier, identifiers .size ()) .first; }

	static
	const IdentifierIndex::value_type &
	getIdentifier (std::string && identifier)
	{ return *identifiers .emplace (std::move (identifier), identifiers .size ()) .first; }

	///  @name Static members

	static IdentifierIndex identifiers;

	///  @name Members

	std::string name;
	size_t      id;

};

///  @relates Identifier
///  @name Relational operators

inline
bool
operator < (const Identifier & lhs, const Identifier & rhs)
{
	return lhs .getId () < rhs .getId ();
}

} // pb
} // titania

#endif
