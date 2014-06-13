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

/**
 *  Template to represent a pointer that can handle circular references and that does
 *  automatic garbage collection.
 *
 *  Extern instantiations for jsValue are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of pointer.
 */
template <class Type>
class basic_ptr :
	public jsChildType,
	public jsOutputStreamType
{
public:

	///  @name Construction

	///  Constructs new basic_ptr.
	basic_ptr () :
		       jsChildType (),
		jsOutputStreamType (),
		             value (nullptr)
	{ }

	///  Constructs new basic_ptr.
	basic_ptr (const basic_ptr & var) :
		basic_ptr (var .value)
	{ }

	///  Constructs new basic_ptr.
	basic_ptr (basic_ptr && var) :
		basic_ptr ()
	{ move (var); }

	///  Constructs new basic_ptr.
	explicit
	basic_ptr (Type* const value) :
		       jsChildType (),
		jsOutputStreamType (),
		             value (value)
	{
		if (value)
			value -> addParent (this);
	}

	///  @name Assignment operators

	///  Assigns the basic_ptr.
	basic_ptr &
	operator = (const basic_ptr & var)
	{
		reset (var .value);
		return *this;
	}

	///  Assigns the basic_ptr.
	basic_ptr &
	operator = (basic_ptr && var)
	{
		if (&var == this)
			return *this;

		remove (get ());
		move (var);

		return *this;
	}
	
	///  @name Observers

	///  Returns a pointer to the managed object.
	Type*
	get () const
	{ return value; }

	///  Dereferences pointer to the managed object.
	Type*
	operator -> () const
	{ return value; }

	///  Dereferences pointer to the managed object.
	Type &
	operator * () const
	{ return *value; }

	///  Checks if there is associated managed object.
	operator bool () const
	{ return value; }

	///  Replaces the managed object.
	void
	reset (Type* const value)
	{
		if (get () not_eq value)
		{
			remove (get ());
			add (value);
		}

		set (value);
	}

	///  @name Input/Output

	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (value)
			ostream << *value;

		else
			throw Error ("basic_ptr::toStream");
	}

	///  @name Destruction

	///  Destructs the owned object if no more basic_ptrs link to it 
	virtual
	void
	dispose ()
	{
		remove (get ());
		set (nullptr);

		jsChildType::dispose ();
	}

	///  Destructs the owned object if no more basic_ptrs link to it 
	virtual
	~basic_ptr ()
	{ remove (get ()); }


private:

	void
	add (Type* const value)
	{
		if (value)
			value -> addParent (this);
	}

	void
	move (basic_ptr & var)
	{
		set (var .get ());

		if (get ())
		{
			var .get () -> replaceParent (&var, this);
			var .set (nullptr);
		}
	}

	void
	remove (Type* const value)
	{
		if (value)
			value -> removeParent (this);
	}

	void
	set (Type* const _value)
	{ value = _value; }

	///  @name Members;

	Type* value;

};

///  @relates basic_ptr
///  @name Comparision operations

///  Compares two basic_ptr.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
inline
bool
operator == (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () == rhs .get (); }

///  Compares two basic_ptr.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
inline
bool
operator not_eq (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () not_eq rhs .get (); }

///  Compares two basic_ptr.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () < rhs .get (); }

///  Compares two basic_ptr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator <= (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () <= rhs .get (); }

///  Compares two basic_ptr.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator > (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () > rhs .get (); }

///  Compares two basic_ptr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const basic_ptr <Type> & lhs, const basic_ptr <Type> & rhs)
{ return lhs .get () >= rhs .get (); }

//

class jsValue;

///  @relates basic_ptr
///  @name Typedef

///  Typedef for jsValue.
using var = basic_ptr <jsValue>;

} // pb
} // titania

#endif
