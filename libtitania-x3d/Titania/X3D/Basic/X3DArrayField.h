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

#ifndef __TITANIA_X3D_BASIC_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_BASIC_X3DARRAY_FIELD_H__

#include "../Basic/X3DField.h"
#include "../Types/Array.h"

#include <Titania/Algorithm/Remove.h>
#include <Titania/Utility/Adapter.h>
#include <initializer_list>

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField :
	public X3DField <Array <ValueType>>
{
public:

	typedef ValueType                                          scalar_type;
	typedef typename X3DField <Array <ValueType>>::value_type value_type;

	typedef typename value_type::iterator               iterator;
	typedef typename value_type::const_iterator         const_iterator;
	typedef typename value_type::reverse_iterator       reverse_iterator;
	typedef typename value_type::const_reverse_iterator const_reverse_iterator;
	typedef typename value_type::difference_type        difference_type;
	typedef typename value_type::size_type              size_type;

	using X3DField <value_type>::getValue;
	using X3DField <value_type>::operator =;

	///  Default constructor.
	X3DArrayField () :
		X3DField <value_type> ()
	{ }

	///  Copy constructor.
	X3DArrayField (const X3DArrayField & field) :
		X3DField <value_type> (field)
	{
		addChildren (begin (), end ());
	}

	//	///  Move constructor.
	//	X3DArrayField (X3DArrayField && field) :
	//		X3DArray (),
	//		X3DField <value_type> (field)
	//	{
	//		addChildren (begin (), end ());
	//	}

	///  Construct an X3DArrayField from basic type @a value.
	explicit
	X3DArrayField (const value_type & value) :
		X3DField <value_type> (value)
	{
		addChildren (begin (), end ());
	}

	///  Moves elements from basic type @a value.
	explicit
	X3DArrayField (value_type && value) :
		X3DField <value_type> (value)
	{
		addChildren (begin (), end ());
	}

	template <const size_t Size>
	X3DArrayField (const ValueType (&value) [Size]) :
		X3DField <value_type> (std::move (value_type (value, value + Size)))
	{
		addChildren (begin (), end ());
	}

	template <const size_t Size>
	X3DArrayField (const typename ValueType::value_type (&value) [Size]) :
		X3DField <value_type> (std::move (value_type (value, value + Size)))
	{
		addChildren (begin (), end ());
	}

	X3DArrayField (std::initializer_list <ValueType> initializer_list) :
		X3DField <value_type> (std::move (value_type (initializer_list .begin (), initializer_list .end ())))
	{
		addChildren (begin (), end ());
	}

	X3DArrayField (std::initializer_list <typename ValueType::value_type> initializer_list) :
		X3DField <value_type> (std::move (value_type (initializer_list .begin (), initializer_list .end ())))
	{
		addChildren (begin (), end ());
	}

	template <class InputIterator>
	X3DArrayField (InputIterator first, InputIterator last) :
		X3DField <value_type> (std::move (value_type (first, last)))
	{
		addChildren (begin (), end ());
	}

	virtual
	X3DArrayField*
	clone () const { return new X3DArrayField (*this); }

	virtual
	X3DArrayField*
	clone (X3DExecutionContext* const) const { return clone (); }

	void
	set1Value (const size_type index, const typename ValueType::value_type &);

	ValueType &
	get1Value (const size_type);

	///  Set @a value to this field without notfying parents.
	virtual
	void
	set (const value_type &);

	template <class InputIterator>
	void set (InputIterator, InputIterator);

	template <const size_t Size>
	X3DArrayField &
	operator = (const ValueType (&value) [Size])
	{
		assign (value, value + Size);
		return *this;
	}

	template <const size_t Size>
	X3DArrayField &
	operator = (const typename ValueType::value_type (&value) [Size])
	{
		assign (value, value + Size);
		return *this;
	}

	X3DArrayField &
	operator = (std::initializer_list <ValueType> list)
	{
		assign (list .begin (), list .end ());
		return *this;
	}

	X3DArrayField &
	operator = (std::initializer_list <typename ValueType::value_type> list)
	{
		assign (list .begin (), list .end ());
		return *this;
	}

	ValueType &
	operator [ ] (const size_type index) { return get () [index]; }

	const ValueType &
	operator [ ] (const size_type index) const { return getValue () [index]; }

	template <class InputIterator>
	void assign (InputIterator, InputIterator);

	ValueType &
	at (const size_type index) { return get () .at (index); }

	const ValueType &
	at (const size_type index) const { return getValue () .at (index); }

	ValueType &
	back () { return get () .back (); }

	const ValueType &
	back () const { return getValue () .back (); }

	iterator
	begin () { return get () .begin (); }

	const_iterator
	begin () const { return getValue () .begin (); }

	const_iterator
	cbegin () const { return getValue () .cbegin (); }

	//size_type capacity () const { return getValue () .capacity (); };

	void
	clear ();

	bool
	empty () const { return getValue () .empty (); }

	iterator
	end () { return get () .end (); }

	const_iterator
	end () const { return getValue () .end (); }

	const_iterator
	cend () const { return getValue () .cend (); }

	iterator
	erase (iterator);

	iterator
	erase (iterator, iterator);

	ValueType &
	front () { return get () .front (); }

	const ValueType &
	front () const { return getValue () .front (); }

	iterator
	insert (iterator, const ValueType &);

	void insert (iterator, size_type, const ValueType &);

	template <class InputIterator>
	void insert (iterator, InputIterator, InputIterator);

	size_type
	max_size () const { return getValue () .max_size (); }

	void
	pop_front ();

	void
	pop_back ();

	void
	push_front (const ValueType &);

	void
	push_front (const typename ValueType::value_type &);

	void
	push_back (const ValueType &);

	void
	push_back (const typename ValueType::value_type &);

	template <class ... Args>
	void
	emplace_front (Args && ... args);

	template <class ... Args>
	void
	emplace_back (Args && ... args);

	reverse_iterator
	rbegin () { return get () .rbegin (); }

	const_reverse_iterator
	rbegin () const { return getValue () .rbegin (); }

	const_reverse_iterator
	crbegin () const { return getValue () .crbegin (); }

	reverse_iterator
	rend () { return get () .rend (); }

	const_reverse_iterator
	rend () const { return getValue () .rend (); }

	const_reverse_iterator
	crend () const { return getValue () .crend (); }

	//	void reserve (size_type size) { get () .reserve (size); };

	void
	resize (size_type);

	void
	resize (size_type, const ValueType &);

	size_type
	size () const { return getValue () .size (); }

	virtual
	~X3DArrayField ()
	{ }


protected:

	using X3DField <value_type>::get;


private:

	using X3DField <value_type>::notifyParents;

	///  Inserts values in the range [@a first, @a last) to the end without notifying parents.
	template <class InputIterator>
	void
	insert_at_end (InputIterator, InputIterator);

	void
	addChildren (iterator, iterator);

};

template <class ValueType>
void
X3DArrayField <ValueType>::set1Value (const size_type index, const typename ValueType::value_type & value)
{
	if (index >= size ())
		resize (index + 1);

	get () [index] = value;
}

template <class ValueType>
ValueType &
X3DArrayField <ValueType>::get1Value (const size_type index)
{
	if (index >= size ())
		resize (index + 1);

	return get () [index];
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::set (const value_type & value)
{
	set (value .cbegin (), value .cend ());
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::set (InputIterator first, InputIterator last)
{
	iterator current = begin ();

	for ( ; first not_eq last && current not_eq end (); ++ current, ++ first)
		current -> set (*first);

	if (first == last)
		get () .resize (current - begin ());

	else
		insert_at_end (first, last);
}

template <class ValueType>
template <class InputIterator>
inline
void
X3DArrayField <ValueType>::assign (InputIterator first, InputIterator last)
{
	set (first, last);
	notifyParents ();
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::clear ()
{
	get () .clear ();
	notifyParents ();
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (iterator location)
{
	iterator iter;

	if (location == begin () or location == end ())
		iter = get () .erase (location);
	else
	{
		iter = get () .erase (location);
		addChildren (iter, end ());
	}

	notifyParents ();
	return iter;
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (iterator first, iterator last)
{
	iterator iter;

	if (first == begin () or last == end ())
		iter = get () .erase (first, last);
	else
	{
		iter = get () .erase (first, last);
		addChildren (iter, end ());
	}

	notifyParents ();
	return iter;
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (iterator location, const ValueType & value)
{
	iterator iter = get () .insert (location, value);

	addChildren (iter, end ());

	notifyParents ();

	return iter;
}

template <class ValueType>
void
X3DArrayField <ValueType>::insert (iterator location, size_type count, const ValueType & value)
{
	difference_type i = location - begin ();

	get () .insert (location, count, value);

	addChildren (begin () + i, end ());

	notifyParents ();
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::insert (iterator location, InputIterator first, InputIterator last)
{
	difference_type i = location - begin ();

	get () .insert (location, first, last);

	addChildren (begin () + i, end ());

	notifyParents ();
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::pop_front ()
{
	get () .pop_front ();
	notifyParents ();
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::pop_back ()
{
	get () .pop_back ();
	notifyParents ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_front (const ValueType & field)
{
	get () .emplace_front (field);
	get () .front () .addParent (this);
	notifyParents ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_front (const typename ValueType::value_type & value)
{
	get () .emplace_front (value);
	get () .front () .addParent (this);
	notifyParents ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_back (const ValueType & field)
{
	get () .emplace_back (field);
	get () .back () .addParent (this);
	notifyParents ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_back (const typename ValueType::value_type & value)
{
	get () .emplace_back (value);
	get () .back () .addParent (this);
	notifyParents ();
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_front (Args && ... args)
{
	get () .emplace_front (args ...);
	get () .front () .addParent (this);
	notifyParents ();
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_back (Args && ... args)
{
	get () .emplace_back (args ...);
	get () .back () .addParent (this);
	notifyParents ();
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::resize (size_type count)
{
	resize (count, ValueType ());
}

template <class ValueType>
void
X3DArrayField <ValueType>::resize (size_type count, const ValueType & value)
{
	size_type currentSize = size ();

	if (count > currentSize)
	{
		get () .resize (count, value);
		addChildren (begin () + currentSize, end ());
		notifyParents ();
	}
	else if (count < currentSize)
	{
		get () .resize (count);
		notifyParents ();
	}
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::insert_at_end (InputIterator first, InputIterator last)
{
	size_type current_size = size ();

	while (first not_eq last)
		get () .emplace_back (*first ++);

	addChildren (begin () + current_size, end ());
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChildren (iterator first, iterator last)
{
	for (auto & child : basic::adapter (first, last))
		child .addParent (this);
}

} // X3D
} // titania

#endif
