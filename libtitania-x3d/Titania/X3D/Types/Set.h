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

#ifndef __TITANIA_X3D_TYPES_SET_H__
#define __TITANIA_X3D_TYPES_SET_H__

#include "../Base/X3DOutput.h"
#include <set>

namespace titania {
namespace X3D {

template <class Key,
          class Compare   = std::less <Key>,
          class Allocator = std::allocator <Key>>
class Set :
	public X3DOutput
{
public:

	typedef std::set <Key, Compare, Allocator>        set_type;
	typedef typename set_type::key_type               key_type;
	typedef typename set_type::value_type             value_type;
	typedef typename set_type::allocator_type         allocator_type;
	typedef typename set_type::iterator               iterator;
	typedef typename set_type::const_iterator         const_iterator;
	typedef typename set_type::reverse_iterator       reverse_iterator;
	typedef typename set_type::const_reverse_iterator const_reverse_iterator;
	typedef typename set_type::size_type              size_type;

	//  @name Constructors

	Set () :
		X3DOutput (),
		set ()
	{ }
	
	Set (const Set & value) :
		X3DOutput (),
		set (value .set)
	{ }

	Set (Set && value) :
		X3DOutput (),
		set (std::move (value .set))
	{ }

	//  @name Assignment
	
	Set &
	operator = (const Set & value)
	{
		set = value .set;
		return *this;
	}

	Set &
	operator = (Set && value)
	{
		set = std::move (value .set);
		return *this;
	}
	
	allocator_type
	get_allocator () const
	{ return set .get_allocator (); }

	//  @name Iterators
	
	iterator
	begin ()
	{ return set .begin (); }

	const_iterator
	begin () const
	{ return set .begin (); }

	const_iterator
	cbegin () const
	{ return set .cbegin (); }

	iterator
	end ()
	{ return set .end (); }

	const_iterator
	end () const
	{ return set .end (); }

	const_iterator
	cend () const
	{ return set .cend (); }

	reverse_iterator
	rbegin ()
	{ return set .rbegin (); }

	const_reverse_iterator
	rbegin () const
	{ return set .rbegin (); }

	const_reverse_iterator
	crbegin () const
	{ return set .crbegin (); }

	reverse_iterator
	rend ()
	{ return set .rend (); }

	const_reverse_iterator
	rend () const
	{ return set .rend (); }

	const_reverse_iterator
	crend () const
	{ return set .crend (); }

	//  @name Capacity

	bool
	empty () const
	{ return set .empty (); }

	size_type
	size () const
	{ return set .size (); }

	size_type
	max_size () const
	{ return set .max_size (); }
	
	//  @name Modifiers

	void
	clear ()
	{
		set .clear ();
		processInterests ();
	}

	std::pair <iterator, bool>
	insert (const value_type & value)
	{ return emplace (value); }

	template <class ... Args>
	std::pair <iterator, bool>
	emplace (Args && ... args)
	{
		std::pair <iterator, bool> pair = set .emplace (std::forward <Args> (args) ...);

		if (pair .second)
			processInterests ();

		return pair;
	}
	
	size_type
	erase (const key_type & key)
	{
		size_type count = set .erase (key);

		if (count)
			processInterests ();

		return count;
	}

	void
	swap (Set & other)
	{
		set .swap (other .set);
		processInterests ();
	}

private:

	std::set <Key, Compare, Allocator> set;

};

} // X3D
} // titania

#endif
