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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_VAR_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_VAR_H__

#include "../Base/pbOutputStreamObject.h"
#include "../Bits/pbConstants.h"
#include "../Primitives/ptr.h"

#include <cmath>

namespace titania {
namespace pb {

class pbExecutionContext;
class pbBaseObject;
class pbObject;

/**
 *  Class to represent a ECMAScript value. This is the base class for all ECMAScript values.
 */
class var :
	virtual public pbOutputStreamObject,
	public pbGarbageCollector
{
public:

	///  @name Construction

	///  Constructs new var.
	var ();

	///  Constructs new var.
	var (const var &);

	///  Constructs new var.
	var (var &&);

	var (const std::nullptr_t);

	///  Constructs new var.
	var (const bool);

	///  Constructs new var.
	var (const int32_t);

	///  Constructs new var.
	var (const uint32_t);

	///  Constructs new var.
	var (const double);

	///  Constructs new var.
	var (const Glib::ustring &);

	///  Constructs new var.
	var (Glib::ustring &&);

	///  Constructs new var.
	var (const std::string &);

	///  Constructs new var.
	template <class Up>
	var (const ptr <Up> & object) :
		pbOutputStreamObject (),
		  pbGarbageCollector (),
		               value ({ object_: new ptr <pbBaseObject> (object) }),
		                type (OBJECT)
	{ }

	///  Constructs new var.
	template <class Up>
	var (ptr <Up> && object) :
		pbOutputStreamObject (),
		  pbGarbageCollector (),
		               value ({ object_: new ptr <pbBaseObject> (std::move (object)) }),
		                type (OBJECT)
	{ }

	///  Constructs new var.
	var (pbBaseObject* const);

	var
	copy (pbExecutionContext* const) const;

	///  @name Assignment operators

	var &
	operator = (const var &);

	var &
	operator = (var &&);

	var &
	operator = (const std::nullptr_t);

	var &
	operator = (const bool);

	var &
	operator = (const int32_t);

	var &
	operator = (const uint32_t);

	var &
	operator = (const double);

	var &
	operator = (const Glib::ustring &);

	var &
	operator = (Glib::ustring &&);

	var &
	operator = (const std::string &);
	
	template <class Up>
	var &
	operator = (const ptr <Up> & other)
	{
		clear ();

		value .object_ = new ptr <pbBaseObject> (other);
		type           = OBJECT;

		return *this;
	}
	
	template <class Up>
	var &
	operator = (ptr <Up> && other)
	{
		clear ();

		value .object_ = new ptr <pbBaseObject> (std::move (other));
		type           = OBJECT;

		return *this;
	}

	var &
	operator = (pbBaseObject* const);

	///  @name Common members

	///  Returns the type of this value.
	ValueType
	getType () const
	{ return type; }

	///  @name Test

	bool
	isPrimitive () const;

	bool
	isUndefined () const
	{ return type == UNDEFINED; }

	bool
	isNull () const
	{ return type == NULL_OBJECT; }

	bool
	isObject () const
	{ return type == OBJECT; }

	///  @name Conversion operations

	///  Converts its argument to a value of type Boolean.
	var
	toPrimitive () const;

	///  Converts its argument to a value of type Boolean.
	bool
	toBoolean () const;

	///  Converts its argument to an integral unsigned value of 16 bit.
	uint16_t
	toUInt16 () const;

	///  Converts its argument to an integral signed value of 32 bit.
	int32_t
	toInt32 () const;

	///  Converts its argument to an integral unsigned value of 32 bit.
	uint32_t
	toUInt32 () const
	{ return toInt32 (); }

	///  Converts its argument to an integral numeric value.
	double
	toInteger () const;

	///  Converts its argument to a value of type Number.
	double
	toNumber () const;

	///  @name Member access

	///  Returns a value of type String.
	const Glib::ustring &
	getString () const
	{ return *value .string_; }

	///  Returns a value of type Object.
	ptr <pbBaseObject> &
	getObject ()
	{ return *value .object_; }

	///  Returns a value of type Object.
	const ptr <pbBaseObject> &
	getObject () const
	{ return *value .object_; }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override;

	///  @name Destruction

	~var ();


private:

	///  @name Operations
	
	void
	clear ();

	///  @name Member types

	union Value
	{
		bool bool_;
		double double_;
		int32_t int32_;
		ptr <pbBaseObject>* object_;
		Glib::ustring* string_;
		uint32_t uint32_;
	};

	///  @name Members

	Value     value;
	ValueType type;

};

} // pb
} // titania

#endif
