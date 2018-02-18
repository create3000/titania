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

#ifndef __TITANIA_BASIC_SUBSCRIPT_ITERATOR_H__
#define __TITANIA_BASIC_SUBSCRIPT_ITERATOR_H__

#include <cstddef>
#include <functional>

namespace titania {
namespace basic {

template <class Container>
class subscript_iterator
{
public:

	using iterator_category = std::random_access_iterator_tag;

	using pointer    = typename Container::pointer;
	using value_type = typename Container::value_type;
	using reference  = typename Container::reference;

	using size_type       = size_t;
	using difference_type = size_t;

	subscript_iterator () :
		m_container (nullptr),
		    m_index (-1)
	{ }

	explicit
	subscript_iterator (Container* container, const size_t index) :
		m_container (container),
		    m_index (index)
	{ }

	subscript_iterator (const subscript_iterator & other) :
		m_container (other .m_container),
		    m_index (other .m_index)
	{ }

	subscript_iterator &
	operator = (const subscript_iterator & other)
	{
		m_container = other .m_container;
		m_index     = other .m_index;
		return *this;
	}

	size_type
	index () const
	{ return m_index; }

	reference
	operator * () const
	{ return m_container -> operator [ ] (m_index); }

	pointer
	operator -> () const
	{ return &m_container -> operator [ ] (m_index); }

	subscript_iterator &
	operator ++ ()
	{
		++ m_index;
		return *this;
	}

	subscript_iterator
	operator ++ (int)
	{
		subscript_iterator tmp = *this;

		++ m_index;
		return tmp;
	}

	subscript_iterator &
	operator -- ()
	{
		-- m_index;
		return *this;
	}

	subscript_iterator
	operator -- (int)
	{
		subscript_iterator tmp = *this;

		-- m_index;
		return tmp;
	}

	subscript_iterator &
	operator += (difference_type n)
	{
		m_index += n;
		return *this;
	}

	subscript_iterator &
	operator -= (difference_type n)
	{
		m_index -= n;
		return *this;
	}

	reference
	operator [ ] (const size_type i) const
	{ return m_container -> operator [ ] (i); }


private:

	Container* m_container;
	size_type  m_index;

};

template <class Container>
inline
subscript_iterator <Container>
operator + (const subscript_iterator <Container> & iter, size_t n)
{
	return subscript_iterator <Container> (iter) += n;
}

template <class Container>
inline
subscript_iterator <Container>
operator + (size_t n, const subscript_iterator <Container> & iter)
{
	return subscript_iterator <Container> (iter) += n;
}

template <class Container>
inline
subscript_iterator <Container>
operator - (const subscript_iterator <Container> & iter, size_t n)
{
	return subscript_iterator <Container> (iter) -= n;
}

template <class Container>
inline
subscript_iterator <Container>
operator - (size_t n, const subscript_iterator <Container> & iter)
{
	return subscript_iterator <Container> (iter) -= n;
}

template <class Container>
inline
typename subscript_iterator <Container>::difference_type
operator - (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () - rhs .index ();
}

template <class Container>
inline
bool
operator == (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () == rhs .index ();
}

template <class Container>
inline
bool
operator not_eq (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () not_eq rhs .index ();
}

template <class Container>
inline
bool
operator < (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () < rhs .index ();
}

template <class Container>
inline
bool
operator > (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () > rhs .index ();
}

template <class Container>
inline
bool
operator >= (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () >= rhs .index ();
}

template <class Container>
inline
bool
operator <= (const subscript_iterator <Container> & lhs, const subscript_iterator <Container> & rhs)
{
	return lhs .index () <= rhs .index ();
}

} // basic
} // titania

#endif
