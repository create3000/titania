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
#include "../Bits/pbConstants.h"
#include "../Bits/pbMath.h"

#include <Titania/LOG.h>
#include <map>
#include <string>

#include <Titania/String/to_string.h>

namespace titania {
namespace pb {

class Identifier :
	public pbOutputStreamObject
{
public:

	///  @name Construction

	Identifier () :
		pbOutputStreamObject (),
		                  id (hash ("")),
		                name ()
	{ }

	Identifier (const Identifier & other) :
		pbOutputStreamObject (),
		                  id (other .id),
		                name (other .name)
	{ }

	Identifier (Identifier && other) :
		pbOutputStreamObject (),
		                  id (other .id),
		                name (std::move (other .name))
	{ }

	Identifier (const Glib::ustring & name) :
		pbOutputStreamObject (),
		                  id (hash (name .raw ())),
		                name (name)
	{ }

	Identifier (const std::string & name) :
		pbOutputStreamObject (),
		                  id (hash (name)),
		                name (name)
	{ }

	Identifier (std::string && name) :
		pbOutputStreamObject (),
		                  id (hash (name)),
		                name (std::move (name))
	{ }

	Identifier (const std::string::value_type* name) :
		pbOutputStreamObject (),
		                  id (hash (name)),
		                name (name)
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

	bool
	isEmpty () const
	{ return name .empty (); }

	const std::string &
	getName () const
	{ return name; }

	const size_t &
	getId () const
	{ return id; }

	///  @name Operations

	uint32_t
	toUInt32 () const
	{
		const auto index = pb::toUInt32 (name);
		
		return isIndex (index) ? index : PROPERTY;
	}

	///  @name Static members

	static const std::hash <std::string> hash;

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ ostream << name; }


private:

	///  @name Operations

	bool
	isIndex (const uint32_t index) const
	{ return basic::to_string (index) == name and index not_eq PROPERTY; }

	///  @name Members

	size_t      id;
	std::string name;

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