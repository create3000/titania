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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_PTR_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_PTR_H__

#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Bits/Exception.h"

namespace titania {
namespace pb {

/**
 *  Base class for ptr.
 */
class ptr_base :
	virtual public pbBase
{
public:

	///  @name Operations

	virtual
	pbChildObject*
	get_object () const = 0;


protected:

	ptr_base () :
		pbBase ()
	{ }

};

/**
 *  Template to represent a pointer that can handle circular references and that does
 *  automatic garbage collection.
 *
 *  Extern instantiations for vsValue are part of the
 *  library.  Results with any other type are not guaranteed.
 *
 *  @param  Type  Type of pointer.
 */
template <class Type>
class ptr :
	public ptr_base,
	public pbChildObject,
	public pbOutputStreamObject
{
public:

	///  @name Member type

	using element_type = Type;

	///  @name Construction

	///  Constructs new ptr.
	ptr () :
		            ptr_base (),
		       pbChildObject (),
		pbOutputStreamObject (),
		               value (nullptr),
		                iter ()
	{ }

	///  Constructs new ptr.
	ptr (const ptr & other) :
		ptr (other .value)
	{ }
	
	///  Constructs new ptr.
	template <class Up>
	ptr (const ptr <Up> & other) :
		ptr (dynamic_cast <Type*> (other .get_object ()))
	{ }

	///  Constructs new ptr.
	ptr (ptr && other) :
		ptr ()
	{ move (other); }

	///  Constructs new ptr.
	template <class Up>
	ptr (ptr <Up> && other) :
		ptr ()
	{ move (other); }

	///  Constructs new ptr.
	ptr (Type* const value) :
		            ptr_base (),
		       pbChildObject (),
		pbOutputStreamObject (),
		               value (value),
		                iter ()
	{ add (value); }

	///  Constructs new ptr.
	template <class Up>
	ptr (Up* const value) :
		ptr (dynamic_cast <Type*> (value))
	{ }

	///  @name Assignment operators

	///  Assigns the ptr.
	ptr &
	operator = (const ptr & other)
	{
		reset (other .value);
		return *this;
	}

	///  Assigns the ptr.
	ptr &
	operator = (ptr && other)
	{
		if (&other == this)
			return *this;

		move (other);

		return *this;
	}

	///  Assigns the ptr.
	template <class Up>
	ptr &
	operator = (const ptr <Up> & other)
	{
		reset (dynamic_cast <Type*> (other .get ()));
		return *this;
	}

	///  Assigns the ptr.
	template <class Up>
	ptr &
	operator = (ptr <Up> && other)
	{
		if (static_cast <pbBase*> (&other) == this)
			return *this;

		move (other);

		return *this;
	}

	///  Assigns the ptr.
	ptr &
	operator = (Type* const other)
	{
		reset (other);
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
	operator Type* () const
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

//	///  Exchanges the contents of *this and @a ptr.
//	void
//	swap (ptr & other)
//	{
//		if (value)
//			value -> replaceParent (this, &other);
//
//		if (other .value)
//			other .value -> replaceParent (&other, this);
//
//		std::swap (value, other .value);
//	}

	///  Removes the managed object.
	void
	reset ()
	{ reset (nullptr); }

	///  Replaces the managed object.
	void
	reset (Type* const value)
	{
		if (value == get ())
			return;

		const auto temp = this -> iter;

		add (value);
		remove (get (), temp);
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
	getTypeName () const
	noexcept (true) final override
	{ return typeName; }

	///  @name Garbage collection

	virtual
	bool
	hasRootedObjects (std::set <pbChildObject*> & seen) final override
	{ return hasRootedObjectsDontCollectObject (seen); }

	///  @name Input/Output

	///  Inserts this object into the output stream @a ostream.
	virtual
	void
	toStream (std::ostream & ostream) const final override
	{
		if (value)
			ostream << *get ();

		else
			throw Error ("ptr::toStream");
	}

	///  @name Destruction

	///  Destructs the owned object if no more ptrs link to it
	virtual
	void
	dispose () final override
	{
		remove (get (), iter);

		pbChildObject::dispose ();
	}

	///  Destructs the owned object if no more ptrs link to it
	virtual
	~ptr ()
	{ remove (get (), iter); }


private:

	///  @name Friends

	template <class Up>
	friend class ptr;

	///  @name Operations

	void
	add (Type* const value)
	{
		if (value)
			iter = value -> addParent (this);
	}

	bool
	move (ptr & other)
	{
		const auto value = other .get ();
		
		if (value == get ())
		{
			// If both values are NULL or equal, only set other to NULL.
			other .reset ();
			return false;
		}

		const auto temp = iter;

		if (value)
		{
			iter = value -> replaceParent (other .iter, this);
			other .set (nullptr);
		}

		remove (get (), temp);
		set (value);
		return true;
	}

	template <class Up>
	bool
	move (ptr <Up> & other)
	{
		const auto value = dynamic_cast <Type*> (other .get ());

		if (value == get ())
		{
			// If both values are NULL or equal, only set other to NULL.
			other .reset ();
			return false;
		}

		const auto temp = iter;

		if (value)
		{
			iter = value -> replaceParent (other .iter, this);
			other .set (nullptr);
		}
		else
			other .reset ();

		remove (get (), temp);
		set (value);
		return true;
	}

	void
	remove (Type* const value, const std::list <pbChildObject*>::iterator & iter)
	{
		if (value)
		{
			set (nullptr);
			value -> removeParent (iter);
		}
	}

	void
	set (Type* const _value)
	{ value = _value; }

	virtual
	pbChildObject*
	get_object () const final override
	{ return get (); }

	///  @name Static members

	static const std::string typeName;

	///  @name Members;

	Type*                                value;
	std::list <pbChildObject*>::iterator iter;

};

template <class Type>
const std::string ptr <Type>::typeName = "ptr";

///  @relates ptr
///  @name Utiliy functions

///  Constructs an object of type Type and wraps it in a ptr using
///  args as the parameter list for the constructor of Type.
template <class Type, class ... Args>
ptr <Type>
make_ptr (Args && ... args)
{
	return ptr <Type> (new Type (std::forward <Args> (args) ...));
}

///  @relates ptr
///  @name Comparision operations

///  Compares two ptr.
///  Return true if @a lhs is equal to @a rhs.
template <class Type>
inline
bool
operator == (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () == rhs .get ();
}

///  Compares two ptr.
///  Return true if @a lhs is not equal to @a rhs.
template <class Type>
inline
bool
operator not_eq (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () not_eq rhs .get ();
}

///  Compares two ptr.
///  Returns true if @a lhs less than @a rhs.
template <class Type>
inline
bool
operator < (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () < rhs .get ();
}

///  Compares two ptr.
///  Returns true if @a lhs less than equal to @a rhs.
template <class Type>
inline
bool
operator <= (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () <= rhs .get ();
}

///  Compares two ptr.
///  Returns true if @a lhs greater than @a rhs.
template <class Type>
inline
bool
operator > (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () > rhs .get ();
}

///  Compares two ptr.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class Type>
inline
bool
operator >= (const ptr <Type> & lhs, const ptr <Type> & rhs)
{
	return lhs .get () >= rhs .get ();
}

} // pb
} // titania

#endif
