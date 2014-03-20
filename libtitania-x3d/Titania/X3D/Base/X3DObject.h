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

#ifndef __TITANIA_X3D_BASE_X3DOBJECT_H__
#define __TITANIA_X3D_BASE_X3DOBJECT_H__

#include "../Base/GarbageCollector.h"
#include "../Base/X3DInput.h"
#include "../Base/X3DOutput.h"
#include "../Bits/Error.h"
#include "../InputOutput/Generator.h"

#include <Titania/LOG.h>

#include <deque>
#include <istream>
#include <memory>
#include <ostream>

namespace titania {
namespace X3D {

typedef std::shared_ptr <X3DBase> UserDataPtr;

class X3DObject :
	public X3DInput, public X3DOutput
{
public:

	///  @name Type Information

	virtual
	void
	setName (const std::string & value)
	{ realize (); data -> name = value; }

	const std::string &
	getName () const
	{ realize (); return data -> name; }

	virtual
	const std::string &
	getTypeName () const
	throw (Error <DISPOSED>) = 0;

	///  @name Comment handling

	void
	addComments (const std::vector <std::string> & value)
	{ realize (); data -> comments .insert (data -> comments .end (), value .begin (), value .end ()); }

	const std::vector <std::string> &
	getComments () const
	{ realize (); return data -> comments; }

	///  @name User data handling

	virtual
	void
	setUserData (const UserDataPtr & value)
	{ realize (); data -> userData = value; }

	const UserDataPtr &
	getUserData () const
	{ realize (); return data -> userData; }

	///  @name Garbage Collection

	GarbageCollector &
	getGarbageCollector ()
	{ return garbageCollector; }

	///  @name String Creation

	bool
	fromString (const std::string &);

	std::string
	toString () const;

	std::string
	toXMLString () const;

	///  @name Stream Handling

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) = 0;

	virtual
	void
	toStream (std::ostream &) const = 0;

	virtual
	void
	toXMLStream (std::ostream &) const = 0;

	///  @name Destruction

	virtual
	void
	dispose () override;

	virtual
	~X3DObject ();


protected:

	///  @name Construction

	X3DObject ();


private:

	///  @name Construction

	void
	realize () const;

	///  @name Static members

	static GarbageCollector garbageCollector;

	///  @name Members

	struct Data
	{
		std::string name;
		std::vector <std::string> comments;
		UserDataPtr userData;
	};

	mutable std::unique_ptr <Data> data;

};

template <class CharT, class Traits>
inline
std::basic_istream <CharT, Traits> &
operator >> (std::basic_istream <CharT, Traits> & istream, X3DObject & object)
{
	object .fromStream (istream);
	return istream;
}

template <class CharT, class Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const X3DObject & object)
{
	object .toStream (ostream);
	return ostream;
}

struct XMLEncodeObjectType { const X3DObject* const object; };

inline
XMLEncodeObjectType
XMLEncode (const X3DObject & object)
{
	return XMLEncodeObjectType { &object };
}

inline
XMLEncodeObjectType
XMLEncode (const X3DObject* const object)
{
	return XMLEncodeObjectType { object };
}

template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const XMLEncodeObjectType & value)
{
	value .object -> toXMLStream (ostream);
	return ostream;
}

struct XMLEncodeStringType { const std::string & string; };

inline
XMLEncodeStringType
XMLEncode (const std::string & string)
{
	return XMLEncodeStringType { string };
}

template <typename CharT, typename Traits>
inline
std::basic_ostream <CharT, Traits> &
operator << (std::basic_ostream <CharT, Traits> & ostream, const XMLEncodeStringType & value)
{
	Generator::XMLEncodeToStream (ostream, value .string);
	return ostream;
}

} // X3D
} // titania

#endif
