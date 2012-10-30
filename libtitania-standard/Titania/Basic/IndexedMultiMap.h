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
 ******************************************************************************/

#ifndef __TITANIA_BASIC_INDEXED_MULTI_MAP_H__
#define __TITANIA_BASIC_INDEXED_MULTI_MAP_H__

#include "../Bits/Algorithm/Remove.h"

#include <deque>
#include <map>
#include <stdexcept>

namespace titania {
namespace basic {

template <class Key, class ValueType>
class indexed_multimap
{
public:

	typedef Key       key_type;
	typedef ValueType value_type;
	typedef size_t    size_type;

	typedef std::deque <ValueType>         array_type;
	typedef std::multimap <Key, ValueType> map_type;

	typedef typename array_type::iterator       array_iterator;
	typedef typename array_type::const_iterator array_const_iterator;

	typedef typename map_type::iterator       map_iterator;
	typedef typename map_type::const_iterator map_const_iterator;

	/// @name Constructors

	///  Default constructor.
	indexed_multimap () :
		array (),
		map ()
	{ }

	/// @name Element access

	//@{
	value_type &
	front () { return array .front (); }

	const value_type &
	front () const { return array .front (); }
	//@}

	//@{
	value_type &
	back () { return array .back (); }

	const value_type &
	back () const { return array .back (); }
	//@}

	//@{
	const value_type &
	at (const size_type & index) const { return array .at (index); }

	const value_type &
	first (const key_type &) const;

	const value_type &
	last (const key_type &) const;
	//@}

	//@{
	const value_type &
	operator [ ] (const size_type & index) const
	{ return array [index]; }
	//@}

	/// @name Lookup

	//@{
	size_type
	count (const key_type & key) const { return map .count (key); }
	//@}

	/// @name Iterators

	//@{
	array_iterator
	begin () { return array .begin (); }

	array_const_iterator
	begin () const { return array .cbegin (); }

	array_const_iterator
	cbegin () const { return array .cbegin (); }
	//@}

	//@{
	array_iterator
	end () { return array .end (); }

	array_const_iterator
	end () const { return array .cend (); }

	array_const_iterator
	cend () const { return array .cend (); }
	//@}

	/// @name Capacity

	//@{
	bool
	empty () const { return array .empty (); }

	size_type
	size () const { return array .size (); }

	size_type
	max_size () const { return array .max_size (); }
	//@}

	/// @name Modifiers

	//@{
	ValueType &
	push_back (const key_type &, const value_type &);

	ValueType &
	replace (const key_type &, const key_type &, const value_type &);

	bool
	erase (const key_type &);

	void
	clear ();
	//@}


private:

	array_type array;
	map_type   map;

};

template <class Key, class ValueType>
const typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::first (const key_type &key) const
{
	const auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return range .first -> second;

	throw std::out_of_range ("indexed_multimap::first");
}

template <class Key, class ValueType>
const typename indexed_multimap <Key, ValueType>::value_type &
indexed_multimap <Key, ValueType>::last (const key_type &key) const
{
	auto range = map .equal_range (key);

	if (range .first not_eq range .second)
		return (-- range .second) -> second;

	throw std::out_of_range ("indexed_multimap::last");
}

template <class Key, class ValueType>
ValueType &
indexed_multimap <Key, ValueType>::push_back (const key_type & key, const value_type & value)
{
	array .emplace_back (value);
	return map .insert (std::make_pair (key, value)) -> second;
}

template <class Key, class ValueType>
ValueType &
indexed_multimap <Key, ValueType>::replace (const key_type & currentKey, const key_type & key, const value_type & value)
{
	erase (currentKey);
	return push_back (key, value);
}

template <class Key, class ValueType>
bool
indexed_multimap <Key, ValueType>::erase (const key_type & key)
{
	// find values

	auto equal_range = map .equal_range (key);

	if (equal_range .first not_eq equal_range .second)
	{
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
	array .clear ();
	map   .clear ();
}

} // basic
} // titania

#endif
