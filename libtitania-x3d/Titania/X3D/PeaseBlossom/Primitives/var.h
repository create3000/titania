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

#include "../Base/jsChildObject.h"
#include "../Base/jsOutputStreamObject.h"

namespace titania {
namespace pb {

/**
 *  Base class for shared_ptr.
 */
class ptr_base :
	virtual public jsBase
{
public:

	///  @name Operations

	virtual
	jsChildObject*
	get_type () const = 0;


protected:

	ptr_base () :
		jsBase ()
	{ }

};

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
class shared_ptr :
	public ptr_base,
	public jsChildObject,
	public jsOutputStreamObject
{
public:

	///  @name Member type

	using element_type = Type;

	///  @name Construction

	///  Constructs new shared_ptr.
	shared_ptr () :
		            ptr_base (),
		       jsChildObject (),
		jsOutputStreamObject (),
		               value (nullptr)
	{ }

	///  Constructs new shared_ptr.
	shared_ptr (const shared_ptr & var) :
		shared_ptr (var .value)
	{ }

	///  Constructs new shared_ptr.
	shared_ptr (const ptr_base & var) :
		shared_ptr (dynamic_cast <Type*> (var .get_type ()))
	{ }

	///  Constructs new shared_ptr.
	shared_ptr (shared_ptr && var) :
		shared_ptr ()
	{ move (var); }

	///  Constructs new shared_ptr.
	template <class Up>
	shared_ptr (shared_ptr <Up> && var) :
		shared_ptr ()
	{ move (var); }

	///  Constructs new shared_ptr.
	explicit
	shared_ptr (Type* const value) :
		            ptr_base (),
		       jsChildObject (),
		jsOutputStreamObject (),
		               value (value)
	{ add (value); }

	///  Constructs new shared_ptr.
	template <class Up>
	explicit
	shared_ptr (Up* const value) :
		shared_ptr (dynamic_cast <Type*> (value))
	{ }

	///  @name Assignment operators

	///  Assigns the shared_ptr.
	shared_ptr &
	operator = (const shared_ptr & var)
	{
		reset (var .value);
		return *this;
	}

	///  Assigns the shared_ptr.
	shared_ptr &
	operator = (const ptr_base & var)
	{
		reset (dynamic_cast <Type*> (var .get_type ()));
		return *this;
	}

	///  Assigns the shared_ptr.
	shared_ptr &
	operator = (shared_ptr && var)
	{
		if (&var == this)
			return *this;

		remove (get ());
		move (var);

		return *this;
	}

	///  Assigns the shared_ptr.
	template <class Up>
	shared_ptr &
	operator = (shared_ptr <Up> && var)
	{
		if (&var == this)
			return *this;

		remove (get ());
		move (var);

		return *this;
	}

	///  @name Observers

	///  Returns a pointer to the managed object.
	constexpr
	Type*
	get () const
	{ return value; }

	///  Dereferences pointer to the managed object.
	constexpr
	Type*
	operator -> () const
	{ return value; }

	///  Dereferences pointer to the managed object.
	constexpr
	Type &
	operator * () const
	{ return *value; }

	///  Checks if there is associated managed object.
	constexpr
	operator bool () const
	{ return value; }

	///  @name Modifiers

	///  Exchanges the contents of *this and @a var.
	void
	swap (shared_ptr & var)
	{
		shared_ptr temp = std::move (var);

		var   = std::move (*this);
		*this = std::move (temp);
	}

	///  Replaces the managed object.
	void
	reset (Type* const value)
	{
		if (get () not_eq value)
		{
			// First add object to avoid dispose.
			add (value);
			remove (get ());
		}

		set (value);
	}

	///  Replaces the managed object.
	template <class Up>
	void
	reset (Up* const value)
	{ reset (dynamic_cast <Type*> (value)); }

	///  @name Common members

	///  Returns the type name of this object.
	virtual
	const std::string &
	getTypeName () const final override
	{ return typeName; }

	virtual
	bool
	hasRootedObjects (ChildObjectSet & seen) final override
	{ return hasRootedObjectsDontCollectObject (seen); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (value)
			ostream << *value;

		else
			throw Error ("shared_ptr::toStream");
	}

	///  @name Destruction

	///  Destructs the owned object if no more ptrs link to it
	virtual
	void
	dispose ()
	{
		remove (get ());

		jsChildObject::dispose ();
	}

	///  Destructs the owned object if no more ptrs link to it
	virtual
	~shared_ptr ()
	{ remove (get ()); }


private:

	///  @name Friends

	template <class Up>
	friend class shared_ptr;

	///  @name Operations

	void
	add (Type* const value)
	{
		if (value)
			value -> addParent (this);
	}

	void
	move (shared_ptr & var)
	{
		set (var .get ());

		if (get ())
		{
			var .get () -> replaceParent (&var, this);
			var .set (nullptr);
		}
	}

	template <class Up>
	void
	move (shared_ptr <Up> & var)
	{
		set (dynamic_cast <Type*> (var .get ()));

		if (get ())
		{
			var .get () -> replaceParent (&var, this);
			var .set (nullptr);
		}
		else
			var .reset (nullptr);
	}

	void
	remove (Type* const value)
	{
		if (value)
		{
			set (nullptr);

			value -> removeParent (this);
		}
	}

	void
	set (Type* const _value)
	{ value = _value; }

	virtual
	jsChildObject*
	get_type () const final override
	{ return get (); }

	///  @name Static members

	static const std::string typeName;

	///  @name Members;

	Type* value;

};

template <class Type>
const std::string shared_ptr <Type>::typeName = "shared_ptr";

///  @relates shared_ptr
///  @name Utiliy functions

///  Constructs an object of type Type and wraps it in a shared_ptr using
///  args as the parameter list for the constructor of Type.
template <class Type, class ... Args>
shared_ptr <Type>
make_shared (Args && ... args)
{
	return shared_ptr <Type> (new Type (std::forward <Args> (args) ...));
}

///  @relates shared_ptr
///  @name Comparision operations

///  Compares two shared_ptr.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
inline
bool
operator == (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () == rhs .get ();
}

///  Compares two shared_ptr.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
inline
bool
operator not_eq (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () not_eq rhs .get ();
}

///  Compares two shared_ptr.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () < rhs .get ();
}

///  Compares two shared_ptr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator <= (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () <= rhs .get ();
}

///  Compares two shared_ptr.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator > (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () > rhs .get ();
}

///  Compares two shared_ptr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const shared_ptr <Type> & lhs, const shared_ptr <Type> & rhs)
{
	return lhs .get () >= rhs .get ();
}

///  @relates shared_ptr
///  @name Typedef

class jsValue;

///  Typedef for jsValue.
using var = shared_ptr <jsValue>;

///  @relates shared_ptr
///  @name Utiliy functions

///  Constructs an object of type Type and wraps it in a var using
///  args as the parameter list for the constructor of Type.
template <class Type, class ... Args>
shared_ptr <Type>
create (Args && ... args)
{
	return var (new Type (std::forward <Args> (args) ...));
}

} // pb
} // titania

#endif
