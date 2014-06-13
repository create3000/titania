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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_VALUES_VALUE_PTR_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_VALUES_VALUE_PTR_H__

#include "../Base/jsOutputStreamType.h"
#include "../Base/jsChildType.h"

namespace titania {
namespace pb {

template <class Type>
class basic_var :
	public jsChildType,
	public jsOutputStreamType
{
public:

	///  @name Construction

	basic_var () :
		       jsChildType (),
		jsOutputStreamType (),
		             value ()
	{ clear (); }

	basic_var (const basic_var & var) :
		basic_var (var .value)
	{ }

	basic_var (basic_var && var) :
		       jsChildType (),
		jsOutputStreamType (),
		             value (var .value)
	{
		if (var .value)
		{
			var .value -> replaceParent (&var, this);
			var .clear ();
		}
	}

	explicit
	basic_var (Type* const value) :
		       jsChildType (),
		jsOutputStreamType (),
		             value (value)
	{
		if (value)
			value -> addParent (this);
	}

	///  @name Assignment operators

	basic_var &
	operator = (const basic_var & var)
	{
		reset (var .value);
		return *this;
	}

	basic_var &
	operator = (basic_var && var)
	{
		if (&var == this)
			return *this;

		remove ();

		value = var .value;

		if (value)
		{
			value -> replaceParent (&var, this);
			var .clear ();
		}

		return *this;
	}
	
	///  @name Access operators

	Type*
	operator -> () const
	{ return value; }

	Type &
	operator * () const
	{ return *value; }

	///  @name Member access
	
	Type*
	get () const
	{ return value; }

	void
	reset (Type* const _value)
	{
		if (value not_eq _value)
		{
			if (_value)
				_value -> addParent (this);

			remove ();
		}

		value = _value;
	}
	
	///  @name Input/Output
	
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (value)
			ostream << *value;
		else
			ostream << "NULL";
	}

	///  @name Destruction

	virtual
	void
	dispose ()
	{
		remove ();

		value = nullptr;

		jsChildType::dispose ();
	}
	
	virtual
	~basic_var ()
	{ remove (); }


private:

	void
	remove ()
	{
		if (value)
			value -> removeParent (this);
	}

	void
	clear ()
	{ value = nullptr; }

	///  @name Members;

	Type* value;

};

class jsValue;

template <>
void
basic_var <jsValue>::clear ();

template <>
void
basic_var <jsValue>::toStream (std::ostream &) const;

using var = basic_var <jsValue>;

} // pb
} // titania

#endif
