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

#ifndef __TITANIA_X3D_BASIC_X3DARRAY_FIELD_ITERATOR_H__
#define __TITANIA_X3D_BASIC_X3DARRAY_FIELD_ITERATOR_H__

#include <cstddef>

namespace titania {
namespace X3D {

template <class IteratorType, class Type>
class X3DArrayFieldIterator
{
public:

	typedef IteratorType iterator_type;

	typedef typename IteratorType::iterator_category iterator_category;

	typedef Type*  pointer;
	typedef Type   value_type;
	typedef Type & reference;

	typedef size_t size_type;
	typedef size_t difference_type;

	explicit
	X3DArrayFieldIterator (const IteratorType & iter) :
		iter (iter)
	{ }

	X3DArrayFieldIterator (const X3DArrayFieldIterator & value) :
		iter (value .iter)
	{ }

	X3DArrayFieldIterator &
	operator = (const X3DArrayFieldIterator & value)
	{
		iter = value .iter;
		return *this;
	}

	const IteratorType &
	iterator () const
	{ return iter; }

	reference
	operator * ()
	{ return **iter; }

	pointer
	operator -> ()
	{ return *iter; }

	X3DArrayFieldIterator &
	operator ++ ()
	{
		++ iter;
		return *this;
	}

	X3DArrayFieldIterator
	operator ++ (int)
	{
		X3DArrayFieldIterator tmp = *this;

		++ iter;
		return tmp;
	}

	X3DArrayFieldIterator &
	operator -- ()
	{
		-- iter;
		return *this;
	}

	X3DArrayFieldIterator
	operator -- (int)
	{
		X3DArrayFieldIterator tmp = *this;

		-- iter;
		return tmp;
	}

	X3DArrayFieldIterator &
	operator += (size_type n)
	{
		iter += n;
		return *this;
	}

	X3DArrayFieldIterator &
	operator -= (size_type n)
	{
		iter -= n;
		return *this;
	}

	reference
	operator [ ] (size_type i)
	{ return *iter [i]; }


private:

	IteratorType iter;

};

template <class IteratorType, class Type>
inline
X3DArrayFieldIterator <IteratorType, Type>
operator + (const X3DArrayFieldIterator <IteratorType, Type> & iter, size_t n)
{
	return X3DArrayFieldIterator <IteratorType, Type> (iter) += n;
}

template <class IteratorType, class Type>
inline
X3DArrayFieldIterator <IteratorType, Type>
operator + (size_t n, const X3DArrayFieldIterator <IteratorType, Type> & iter)
{
	return X3DArrayFieldIterator <IteratorType, Type> (iter) += n;
}

template <class IteratorType, class Type>
inline
X3DArrayFieldIterator <IteratorType, Type>
operator - (const X3DArrayFieldIterator <IteratorType, Type> & iter, size_t n)
{
	return X3DArrayFieldIterator <IteratorType, Type> (iter) -= n;
}

template <class IteratorType, class Type>
inline
X3DArrayFieldIterator <IteratorType, Type>
operator - (size_t n, const X3DArrayFieldIterator <IteratorType, Type> & iter)
{
	return X3DArrayFieldIterator <IteratorType, Type> (iter) -= n;
}

template <class IteratorType, class Type>
inline
typename X3DArrayFieldIterator <IteratorType, Type>::difference_type
operator - (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () - rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator == (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () == rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator not_eq (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () not_eq rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator < (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () < rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator > (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () > rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator >= (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () >= rhs .iterator ();
}

template <class IteratorType, class Type>
inline
bool
operator <= (const X3DArrayFieldIterator <IteratorType, Type> & lhs, const X3DArrayFieldIterator <IteratorType, Type> & rhs)
{
	return lhs .iterator () <= rhs .iterator ();
}

} // X3D
} // titania

#endif
