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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_STRING_OBJECT_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_STRING_OBJECT_H__

#include "../Primitives/jsBasicObject.h"
#include "../Primitives/jsBasicString.h"

namespace titania {
namespace pb {

/**
 *  Class to represent a »string« object.
 */
class StringObject :
	public jsBasicObject,
	public jsBasicString
{
public:

	///  @name Construction

	StringObject () :
		jsBasicString ()
	{ }

	explicit
	StringObject (const Glib::ustring & value) :
		jsBasicObject (),
		jsBasicString (value)
	{ }

	explicit
	StringObject (Glib::ustring && value) :
		jsBasicObject (),
		jsBasicString (std::move (value))
	{ }

	explicit
	StringObject (const jsValue & value) :
		jsBasicObject (),
		jsBasicString (value)
	{ }

	explicit
	StringObject (const std::string::value_type* value) :
		jsBasicObject (),
		jsBasicString (value)
	{ }

	///  @name Member access

	///  Returns the type of the value. For string objects this is »STRING_OBJECT«.
	virtual
	ValueType
	getType () const final override
	{ return STRING_OBJECT; }

	///  @name Operations

	virtual
	bool
	toBoolean () const final override
	{ return jsBasicString::toBoolean (); }

	virtual
	uint16_t
	toUInt16 () const final override
	{ return jsBasicString::toUInt16 (); }

	virtual
	int32_t
	toInt32 () const final override
	{ return jsBasicString::toInt32 (); }

	virtual
	uint32_t
	toUInt32 () const final override
	{ return jsBasicString::toUInt32 (); }

	virtual
	double
	toNumber () const final override
	{ return jsBasicString::toNumber (); }

	virtual
	var
	toObject () const
	throw (TypeError) final override
	{ return jsBasicObject::toObject (); }

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream & ostream) const final override
	{ jsBasicString::toStream (ostream); }

};

} // pb
} // titania

#endif
