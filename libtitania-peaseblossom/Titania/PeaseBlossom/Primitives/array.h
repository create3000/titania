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

#ifndef __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_ARRAY_H__
#define __TITANIA_X3D_PEASE_BLOSSOM_PRIMITIVES_ARRAY_H__

#include "../Base/pbChildObject.h"
#include "../Base/pbOutputStreamObject.h"
#include "../Primitives/ptr.h"
#include "../Primitives/var.h"
#include "../Primitives/iterator.h"

#include <Titania/Basic/ReferenceIterator.h>
#include <Titania/Utility/Range.h>

#include <vector>
#include <initializer_list>

namespace titania {
namespace pb {

template <class Type>
class array :
	public pbChildObject,
	public pbOutputStreamObject
{
public:

	using value_type = Type;


private:

	using pair_type  = std::pair <value_type*, std::list <pbChildObject*>::iterator>;
	using array_type = std::vector <pair_type>;


public:

	using iterator               = pb::iterator <typename array_type::iterator, Type>;
	using reverse_iterator       = pb::iterator <typename array_type::reverse_iterator, Type>;
	using const_iterator         = pb::iterator <typename array_type::const_iterator, const Type>;
	using const_reverse_iterator = pb::iterator <typename array_type::const_reverse_iterator, const Type>;

	using difference_type = typename array_type::difference_type;
	using size_type       = typename array_type::size_type;

	///  @name Construction

	///  Default constructor.
	array () :
		       pbChildObject (),
		pbOutputStreamObject (),
		               value ()
	{ }

	///  Copy constructor.
	array (const array & other) :
		array <Type> (other .begin (), other .end ())
	{ }

	///  Move constructor.
	array (array && other) :
		       pbChildObject (),
		pbOutputStreamObject (),
		               value (std::move (other .value))
	{
		for (const auto & pair : value)
			pair .first -> replaceParent (pair .second, this);
	}

	template <class InputIterator>
	array (const InputIterator &, const InputIterator &);

	///  Constructs new array
	array (const size_type size) :
		array <Type> ()
	{ resize (size); }

	///  @name Assignment operators

	array &
	operator = (const array & other)
	{
		assign (other .begin (), other .end ());
		return *this;
	}

	array &
	operator = (array &&);

	array &
	operator = (std::initializer_list <Type> list)
	{
		assign (list .begin (), list .end ());
		return *this;
	}

	Type &
	operator [ ] (const size_type index)
	{ return *value [index] .first; }

	const Type &
	operator [ ] (const size_type index) const
	{ return *value [index] .first; }

	template <class InputIterator>
	void
	assign (InputIterator, const InputIterator &);

	Type &
	at (const size_type index)
	{ return *value .at (index) .first; }

	const Type &
	at (const size_type index) const
	{ return *value .at (index) .first; }

	Type &
	back ()
	{ return *value .back () .first; }

	const Type &
	back () const
	{ return *value .back () .first; }

	iterator
	begin ()
	{ return iterator (value .begin ()); }

	const_iterator
	begin () const
	{ return const_iterator (value .begin ()); }

	const_iterator
	cbegin () const
	{ return const_iterator (value .cbegin ()); }

	//size_type
	//capacity () const
	//{ return value .capacity (); }

	void
	clear ();

	bool
	empty () const
	{ return value .empty (); }

	iterator
	end ()
	{ return iterator (value .end ()); }

	const_iterator
	end () const
	{ return const_iterator (value .end ()); }

	const_iterator
	cend () const
	{ return const_iterator (value .cend ()); }

	iterator
	erase (const iterator &);

	iterator
	erase (const iterator &, const iterator &);

	Type &
	front ()
	{ return *value .front () .first; }

	const Type &
	front () const
	{ return *value .front () .first; }

	iterator
	insert (const iterator &, const Type &);

	iterator
	insert (const iterator &, const size_type, const Type &);

	template <class InputIterator>
	iterator
	insert (const iterator &, InputIterator, const InputIterator &);

	///  Appends array @a to this array.
	array &
	prepend (const array & other);

	///  Appends array @a to this array.
	array &
	prepend (array && other);

	///  Appends array @a to this array.
	array &
	append (const array & other);

	///  Appends array @a to this array.
	array &
	append (array && other);

	void
	pop_front ();

	void
	pop_back ();

	void
	push_front (const Type &);

	void
	push_back (const Type &);

	template <class ... Args>
	void
	emplace_front (Args && ... args);

	template <class ... Args>
	void
	emplace_back (Args && ... args);

	reverse_iterator
	rbegin ()
	{ return reverse_iterator (value .rbegin ()); }

	const_reverse_iterator
	rbegin () const
	{ return const_reverse_iterator (value .rbegin ()); }

	const_reverse_iterator
	crbegin () const
	{ return const_reverse_iterator (value .crbegin ()); }

	reverse_iterator
	rend ()
	{ return reverse_iterator (value .rend ()); }

	const_reverse_iterator
	rend () const
	{ return const_reverse_iterator (value .rend ()); }

	const_reverse_iterator
	crend () const
	{ return const_reverse_iterator (value .crend ()); }

	void
	reserve (const size_type size)
	{ value .reserve (size); }

	size_type
	capacity () const
	{ return value .capacity (); }

	void
	shrink_to_fit ()
	{ value .shrink_to_fit (); }

	void
	resize (const size_type);

	void
	resize (const size_type, const Type &);

	size_type
	max_size () const
	{ return value .max_size (); }

	size_type
	size () const
	{ return value .size (); }

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

	virtual
	void
	toStream (std::ostream &) const final override;

	///  @name Destruction

	virtual
	void
	dispose () final override
	{
		clear ();

		pbChildObject::dispose ();
	}

	virtual
	~array ()
	{ clear (); }


private:

	///  @name Operations

	void
	add (const typename iterator::iterator_type & first,
	     const typename iterator::iterator_type & last)
	{
		for (auto & pair : std::pair (first, last))
			add (pair);
	}

	void
	add (pair_type & pair)
	{ pair .second = pair .first -> addParent (this); }

	void
	remove (const typename iterator::iterator_type & first,
	        const typename iterator::iterator_type & last)
	{
		for (auto & pair : std::pair (first, last))
			remove (pair);
	}

	void
	remove (pair_type & pair)
	{ pair .first -> removeParent (pair .second); }

	///  @name Static members

	static const std::string typeName;

	///  Members

	array_type value;

};

template <class Type>
const std::string array <Type>::typeName = "array";

template <class Type>
template <class InputIterator>
array <Type>::array (const InputIterator & first, const InputIterator & last) :
	       pbChildObject (),
	pbOutputStreamObject (),
	               value ()
{
	// Insert at end

	for (const auto & e : std::pair (first, last))
	{
		Type* const element = new Type (e);

		value .emplace_back (element, std::list <pbChildObject*>::iterator ());

		add (value .back ());
	}
}

template <class Type>
array <Type> &
array <Type>::operator = (array && other)
{
	if (&other == this)
		return *this;

	clear ();

	std::swap (value, other .value);

	for (const auto & element : value)
		element -> replaceParent (element .second, this);

	return *this;
}

template <class Type>
template <class InputIterator>
void
array <Type>::assign (InputIterator first, const InputIterator & last)
{
	iterator current = begin ();

	for (const auto end = this -> end (); first not_eq last && current not_eq end; ++ current, ++ first)
		*current = *first;

	if (first == last)
	{
		// Remove trailing elements

		const size_t count = current - begin ();

		remove (value .begin () + count, value .end ());

		value .resize (count);
	}
	else
	{
		// Insert at end

		for ( ; first not_eq last; ++ first)
		{
			Type* const element = new Type (*first);

			value .emplace_back (element, std::list <pbChildObject*>::iterator ());

			add (value .back ());
		}
	}
}

template <class Type>
inline
void
array <Type>::clear ()
{
	remove (value .begin (), value .end ());
	value .clear ();
}

template <class Type>
typename array <Type>::iterator
array <Type>::erase (const iterator & location)
{
	remove (*location .base ());

	const auto iter = value .erase (location .base ());

	return iterator (iter);
}

template <class Type>
typename array <Type>::iterator
array <Type>::erase (const iterator & first, const iterator & last)
{
	remove (first .base (), last .base ());

	const auto iter = value .erase (first .base (), last .base ());

	return iterator (iter);
}

template <class Type>
typename array <Type>::iterator
array <Type>::insert (const iterator & location, const Type & element)
{
	const auto iter = value .insert (location .base (), std::pair (new Type (element), std::list <pbChildObject*>::iterator ()));

	add (*iter);

	return iterator (iter);
}

template <class Type>
typename array <Type>::iterator
array <Type>::insert (const iterator & location, const size_type count, const Type & e)
{
	const size_type pos = location - begin ();

	value .insert (location .base (), count, std::pair (nullptr, std::list <pbChildObject*>::iterator ()));

	const auto iter = value .begin () + pos;

	for (auto & element : basic::make_range (iter, count))
	{
		element .first = new Type (e);

		add (element);
	}

	return iterator (iter);
}

template <class Type>
template <class InputIterator>
typename array <Type>::iterator
array <Type>::insert (const iterator & location, InputIterator first, const InputIterator & last)
{
	const size_type pos   = location - begin ();
	const size_type count = last - first;

	value .insert (location .base (), count, std::pair (nullptr, std::list <pbChildObject*>::iterator ()));

	const auto iter = value .begin () + pos;

	for (auto & element : basic::make_range (iter, count))
	{
		element .first = new Type (*first);

		add (element);

		++ first;
	}

	return iterator (iter);
}

template <class Type>
inline
array <Type> &
array <Type>::prepend (const array & other)
{
	insert (begin (), other .begin (), other .end ());
	return *this;
}

template <class Type>
inline
array <Type> &
array <Type>::prepend (array && other)
{
	for (auto & element : basic::make_reverse_range (other))
		insert (begin (), std::move (element));

	other .clear ();
	return *this;
}

template <class Type>
inline
array <Type> &
array <Type>::append (const array & other)
{
	insert (end (), other .begin (), other .end ());
	return *this;
}

template <class Type>
inline
array <Type> &
array <Type>::append (array && other)
{
	for (auto & element : other)
		emplace_back (std::move (element));

	other .clear ();
	return *this;
}

template <class Type>
void
array <Type>::pop_front ()
{
	remove (value .front ());

	value .pop_front ();
}

template <class Type>
inline
void
array <Type>::pop_back ()
{
	remove (value .back ());

	value .pop_back ();
}

template <class Type>
inline
void
array <Type>::push_front (const Type & element)
{
	emplace_front (element);
}

template <class Type>
inline
void
array <Type>::push_back (const Type & element)
{
	emplace_back (element);
}

template <class Type>
template <class ... Args>
void
array <Type>::emplace_front (Args && ... args)
{
	auto iter = value .insert (value .begin (),
	                           std::pair (new Type (std::forward <Args> (args) ...),
	                                           std::list <pbChildObject*>::iterator ()));

	add (*iter);
}

template <class Type>
template <class ... Args>
void
array <Type>::emplace_back (Args && ... args)
{
	Type* const element = new Type (std::forward <Args> (args) ...);

	value .emplace_back (element, std::list <pbChildObject*>::iterator ());

	add (value .back ());
}

template <class Type>
inline
void
array <Type>::resize (const size_type count)
{
	resize (count, Type ());
}

template <class Type>
void
array <Type>::resize (const size_type count, const Type & e)
{
	const size_type currentSize = size ();

	if (count > currentSize)
	{
		value .resize (count, std::pair (nullptr, std::list <pbChildObject*>::iterator ()));

		for (auto & element : std::pair (value .begin () + currentSize, value .end ()))
		{
			element .first = new Type (e);
			add (element);
		}
	}
	else if (count < currentSize)
	{
		remove (value .begin () + count, value .end ());

		value .resize (count);
	}
}

template <class Type>
void
array <Type>::toStream (std::ostream & ostream) const
{
	switch (size ())
	{
		case 0:
		{
			ostream << "[]";
			return;
		}
		case 1:
		{
			ostream << "[ " << front () << " ]";
			return;
		}
		default:
		{
			ostream << "[ ";

			for (const auto & element : std::pair (cbegin (), cend () - 1))
			{
				ostream
					<< element
					<< ", ";
			}

			ostream
				<< back ()
				<< " ]";

			return;
		}
	}
}

template <class Type>
inline
bool
operator == (const array <Type> & lhs, const array <Type> & rhs)
{
	return lhs .size () == rhs .size () &&
	       std::equal (lhs .begin (), lhs .end (),
	                   rhs .begin ());
}

template <class Type>
inline
bool
operator not_eq (const array <Type> & lhs, const array <Type> & rhs)
{
	return not (lhs == rhs);
}

template <class Type>
inline
bool
operator < (const array <Type> & lhs, const array <Type> & rhs)
{
	return std::lexicographical_compare (lhs .begin (), lhs .end (),
	                                     rhs .begin (), rhs .end ());
}

template <class Type>
inline
bool
operator > (const array <Type> & lhs, const array <Type> & rhs)
{
	return rhs < lhs;
}

template <class Type>
inline
bool
operator <= (const array <Type> & lhs, const array <Type> & rhs)
{
	return not (rhs < lhs);
}

template <class Type>
inline
bool
operator >= (const array <Type> & lhs, const array <Type> & rhs)
{
	return not (lhs < rhs);
}

} // pb
} // titania

#endif
