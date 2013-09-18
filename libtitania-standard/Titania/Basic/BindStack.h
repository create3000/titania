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

#ifndef __TITANIA_BASIC_BIND_STACK_H__
#define __TITANIA_BASIC_BIND_STACK_H__

#include <functional>
#include <initializer_list>
#include <list>
#include <map>

namespace titania {
namespace basic {

template <class Type, class Compare = std::less <Type>>
class bind_stack
{
private:

	typedef std::list <Type>                                       list_type;
	typedef std::map <Type, typename list_type::iterator, Compare> map_type;


public:

	typedef typename list_type::size_type      size_type;
	typedef typename list_type::iterator       iterator;
	typedef typename list_type::const_iterator const_iterator;

	///  Standard constructor.
	bind_stack () :
		list (),
		 map ()
	{ }

	bind_stack (std::initializer_list <Type> list) :
		list (),
		 map ()
	{
		for (const auto & value : list)
			push_back (value);
	}

	// Modifiers:

	bool
	push (const Type & value)
	{
		// If value is on top then return.
		if (value == top ())
			return false;

		// If value is already in container then move value to top.
		const auto map_iter = map .find (value);

		if (map_iter not_eq map .end ())
		{
			move_to_top (map_iter -> second, value);
			return false;
		}

		// Emplace new values on top.
		push_front (value);

		return true;
	}

	void
	pop () { erase (top ()); }

	void
	erase (const Type & value)
	{
		// Erase value if in container otherwise silently return.
		const auto map_iter = map .find (value);

		if (map_iter not_eq map .end ())
		{
			list .erase (map_iter -> second);
			map .erase (map_iter);
		}
	}

	void
	clear ()
	{
		list .clear ();
		map .clear ();
	}

	// Element access:

	const Type &
	top () const
	{ return list .front (); }

	const Type &
	bottom () const
	{ return list .back (); }

	// Iterators:

	///  begin
	const_iterator
	begin () const
	{ return list .begin (); }

	const_iterator
	cbegin () const
	{ return list .cbegin (); }

	///  begin
	const_iterator
	end () const
	{ return list .end (); }

	const_iterator
	cend () const
	{ return list .cend (); }

	// Capacity:

	bool
	empty () const
	{ return list .empty (); }

	size_type
	size () const
	{ return list .size (); }


private:

	void
	push_front (const Type & value)
	{
		list .push_front (value);
		map .insert ({ std::make_pair (value, list .begin ()) });
	}

	void
	push_back (const Type & value)
	{
		list .push_back (value);
		map .insert ({ std::make_pair (value, list .begin ()) });
	}

	void
	move_to_top (iterator & list_iter, const Type & value)
	{
		list .erase (list_iter);

		list .push_front (value);
		list_iter = list .begin ();
	}

	list_type list;
	map_type  map;

};

} // basic
} // titania

#endif
