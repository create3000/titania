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

#ifndef __TITANIA_BASIC_INDEXED_MULTI_MAP_H__
#define __TITANIA_BASIC_INDEXED_MULTI_MAP_H__

#include "../Algorithm/Remove.h"
#include "../Basic/ReferenceIterator.h"

#include <deque>
#include <map>
#include <stdexcept>

namespace titania {
namespace basic {

template <class Key, class ValueType>
class indexed_multimap
{
public:

	typedef Key        key_type;
	typedef ValueType  value_type;
	typedef ValueType* pointer_type;
	typedef size_t     size_type;

	typedef std::deque <pointer_type>         array_type;
	typedef std::multimap <Key, pointer_type> map_type;

	typedef reference_iterator <typename array_type::iterator, ValueType>                     iterator;
	typedef reference_iterator <typename array_type::reverse_iterator, ValueType>             reverse_iterator;
	typedef reference_iterator <typename array_type::const_iterator, const ValueType>         const_iterator;
	typedef reference_iterator <typename array_type::const_reverse_iterator, const ValueType> const_reverse_iterator;

	/// @name Constructors

	///  Default constructor
	indexed_multimap () :
		array (),
		  map ()
	{ }

	///  Copy constructor
	indexed_multimap (const indexed_multimap & value) :
		array (),
		  map ()
	{
		*this = value;
	}

	///  Move constructor
	indexed_multimap (indexed_multimap && value) :
		array (std::move (value .array)),
		  map (std::move (value .map))
	{ }

	///  Assignment operator
	indexed_multimap &
	operator = (const indexed_multimap &);

	indexed_multimap &
	operator = (indexed_multimap && value)
	{
		clear ();

		array = std::move (value .array);
		map   = std::move (value .map);

		return *this;
	}

	/// @name Element access

	value_type &
	front ()
	{ return *array .front (); }

	const value_type &
	front () const
	{ return *array .front (); }

	value_type &
	back ()
	{ return *array .back (); }

	const value_type &
	back () const
	{ return *array .back (); }

	const value_type &
	at (const size_type & index) const
	{ return *array .at (index); }

	value_type &
	find (const key_type &);

	const value_type &
	find (const key_type &) const;

	value_type &
	rfind (const key_type &);

	const value_type &
	rfind (const key_type &) const;

	const value_type &
	operator [ ] (const size_type & index) const
	{ return *array [index]; }

	/// @name Lookup

	size_type
	count (const key_type & key) const
	{ return map .count (key); }

	/// @name Iterators

	iterator
	begin ()
	{ return iterator (array .begin ()); }

	const_iterator
	begin () const
	{ return const_iterator (array .begin ()); }

	const_iterator
	cbegin () const
	{ return const_iterator (array .cbegin ()); }

	iterator
	end ()
	{ return iterator (array .end ()); }

	const_iterator
	end () const
	{ return const_iterator (array .end ()); }

	const_iterator
	cend () const
	{ return const_iterator (array .cend ()); }

	/// @name Reverse iterators

	reverse_iterator
	rbegin ()
	{ return reverse_iterator (array .rbegin ()); }

	const_reverse_iterator
	rbegin () const
	{ return const_reverse_iterator (array .rbegin ()); }

	const_reverse_iterator
	crbegin () const
	{ return const_reverse_iterator (array .crbegin ()); }

	reverse_iterator
	rend ()
	{ return reverse_iterator (array .rend ()); }

	const_reverse_iterator
	rend () const
	{ return const_reverse_iterator (array .rend ()); }

	const_reverse_iterator
	crend () const
	{ return const_reverse_iterator (array .crend ()); }

	/// @name Capacity

	bool
	empty () const
	{ return array .empty (); }

	size_type
	size () const
	{ return array .size (); }

	size_type
	max_size () const
	{ return array .max_size (); }

	/// @name Modifiers

	void
	push_back (const key_type &, const value_type &);

	void
	remap (const key_type &, const key_type &);

	bool
	erase (const key_type &);

	void
	clear ();

	~indexed_multimap ();


private:

	array_type array;
	map_type   map;

};

template <class Key, class ValueType>
indexed_multimap <Key, ValueType> &
indexed_multimap <Key, ValueType>::operator = (const indexed_multimap & value)
{
	clear ();

	typename array_type::size_type index = 0;
	std::map <pointer_type, typename array_type::size_type> indices;

	for (const auto & e : value .array)
	{
		indices .emplace (e, index ++);

		const pointer_type element = new value_type (*e);

		array .emplace_back (element);
	}

	for (const auto & pair : value .map)
		map .emplace (pair .first, array [indices [pair .second]]);

	return *this;
}

template <class Key, class ValueType>
typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::find (const key_type &key)
{
	const auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return *range .first -> second;

	throw std::out_of_range ("indexed_multimap::first");
}

template <class Key, class ValueType>
const typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::find (const key_type &key) const
{
	const auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return *range .first -> second;

	throw std::out_of_range ("indexed_multimap::first");
}

template <class Key, class ValueType>
typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::rfind (const key_type &key)
{
	auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return *(-- range .second) -> second;

	throw std::out_of_range ("indexed_multimap::rfind");
}

template <class Key, class ValueType>
const typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::rfind (const key_type &key) const
{
	auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return *(-- range .second) -> second;

	throw std::out_of_range ("indexed_multimap::rfind");
}

template <class Key, class ValueType>
void
indexed_multimap <Key, ValueType>::push_back (const key_type & key, const value_type & value)
{
	const pointer_type element = new value_type (value);

	array .emplace_back (element);

	map .emplace (key, element);
}

template <class Key, class ValueType>
void
indexed_multimap <Key, ValueType>::remap (const key_type & oldKey, const key_type & newKey)
{
	const auto iter = map .find (oldKey);
	
	if (iter not_eq map .end ())
	{
		const auto element = iter -> second;
		
		map .erase (iter);
		map .emplace (newKey, element);
	}
}

template <class Key, class ValueType>
bool
indexed_multimap <Key, ValueType>::erase (const key_type & key)
{
	// find values

	const auto equal_range = map .equal_range (key);

	if (equal_range .first not_eq equal_range .second)
	{
		// delete elements

		for (const auto & pair : equal_range)
			delete pair .second;

		// remove range from array

		const auto new_end = basic::remove (array .begin (), array .end (),
		                                    equal_range .first, equal_range .second);

		// erase from map and resize array

		array .erase (new_end, array .end ());
		map .erase (equal_range .first, equal_range .second);

		return true;
	}

	return false;
}

template <class Key, class ValueType>
void
indexed_multimap <Key, ValueType>::clear ()
{
	for (const auto & element : array)
		delete element;

	array .clear ();
	map   .clear ();
}

template <class Key, class ValueType>
indexed_multimap <Key, ValueType>::~indexed_multimap ()
{
	clear ();
}

} // basic
} // titania

#endif
