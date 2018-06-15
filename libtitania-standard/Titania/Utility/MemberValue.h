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

#ifndef __TITANIA_UTILITY_MEMBER_VALUE_H__
#define __TITANIA_UTILITY_MEMBER_VALUE_H__

#include <functional>
#include <type_traits>

namespace titania {
namespace basic {

template <class Type>
class member_value
{
public:

	///  @name Member types

	using getter_type = std::function <Type ()>;
	using setter_type = std::function <void (const Type &)>;

	///  @name Construction

	member_value () = delete;

	member_value (const member_value & other) = default;

	member_value (member_value && other) = default;

	constexpr
	member_value (const setter_type & setter, const getter_type & getter) :
		setter (setter),
		getter (getter)
	{ }

	///  @name Assignment operator

	member_value &
	operator = (const member_value & other)
	{
		setter (other .getter ());
		return *this;
	}

	member_value &
	operator = (member_value && other)
	{
		setter (other .getter ());
		return *this;
	}

	member_value &
	operator = (const Type & value)
	{
		setter (value);
		return *this;
	}

	///  @name Element access

	void
	set (const Type & value)
	{ setter (value); }

	Type
	get () const
	{ return getter (); }

	operator Type () const
	{ return getter (); }

	///  @name Operations

// C++17
//	template <class ... Args>
//	std::invoke_result_t <Type, Args ...>
//	operator () (Args && ... args) const
//	{ return std::invoke (get (), std::forward <Args> (args) ...); }

	///  Swaps the contents.
	void
	swap (member_value && other)
	{
		const auto tmp = getter ();

		setter (other .getter ());
		other .setter (tmp);
	}

	///  @name Destruction

	~member_value () = default;


private:

	///  @name Members

	const setter_type setter;
	const getter_type getter;

};

/// Specializes the std::swap algorithm for member_value.
template <class Type>
inline
void
swap (member_value <Type> && lhs, member_value <Type> && rhs)
{
	lhs .swap (std::move (rhs));
}

} // basic
} // titania

namespace std {

/// Specializes the std::swap algorithm for member_value.
template <class Type>
inline
void
swap (titania::basic::member_value <Type> && lhs, titania::basic::member_value <Type> && rhs)
{
	lhs .swap (std::move (rhs));
}

} // std

#endif
