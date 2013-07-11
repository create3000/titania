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
#include <Titania/Basic/ReferenceIterator.h>
#include <Titania/Utility/Adapter.h>
#include <initializer_list>

namespace titania {
namespace X3D {

template <class ValueType>
class X3DArrayField :
	public X3DField <Array <ValueType>>
{
public:

	typedef ValueType         scalar_type;
	typedef Array <ValueType> value_type;

	typedef basic::reference_iterator <typename value_type::iterator, ValueType>                     iterator;
	typedef basic::reference_iterator <typename value_type::reverse_iterator, ValueType>             reverse_iterator;
	typedef basic::reference_iterator <typename value_type::const_iterator, const ValueType>         const_iterator;
	typedef basic::reference_iterator <typename value_type::const_reverse_iterator, const ValueType> const_reverse_iterator;

	typedef typename value_type::difference_type difference_type;
	typedef typename value_type::size_type       size_type;

	using X3DField <value_type>::addInterest;
	using X3DField <value_type>::getType;
	using X3DField <value_type>::getValue;
	using X3DField <value_type>::operator =;
	using X3DField <value_type>::getGarbageCollector;

	///  Default constructor.
	X3DArrayField () :
		X3DField <value_type> ()
	{ }

	///  Copy constructor.
	X3DArrayField (const X3DArrayField & field) :
		X3DArrayField <ValueType> (field .begin (), field .end ())
	{ }

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
		X3DArrayField <ValueType> (const_iterator (value .begin ()), const_iterator (value .end ()))
	{ }

	//	///  Moves elements from basic type @a value.
	//	explicit
	//	X3DArrayField (value_type && value) :
	//		X3DField <value_type> (value)
	//	{
	//		addChildren (get () .begin (), get () .end ());
	//	}

	template <const size_t Size>
	X3DArrayField (const ValueType (&value) [Size]) :
		X3DArrayField <ValueType> (value, value + Size)
	{ }

	template <const size_t Size>
	X3DArrayField (const typename ValueType::value_type (&value) [Size]) :
		X3DArrayField <ValueType> (value, value + Size)
	{ }

	X3DArrayField (std::initializer_list <ValueType> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	X3DArrayField (std::initializer_list <const typename ValueType::value_type> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	template <class InputIterator>
	X3DArrayField (InputIterator first, InputIterator last) :
		X3DField <value_type> ()
	{
		for (const auto & value : basic::adapter (first, last))
		{
			ValueType* field = new ValueType (value);

			get () .emplace_back (field);

			addChild (field);
		}
	}

	virtual
	X3DArrayField*
	clone () const override
	{ return new X3DArrayField (*this); }

	virtual
	X3DArrayField*
	clone (X3DExecutionContext* const) const override
	{ return clone (); }

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

	virtual
	bool
	operator == (const X3DFieldDefinition &) const;

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (const X3DArrayField &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DArrayField &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  Functions

	void
	set1Value (const size_type index, const ValueType &);

	void
	set1Value (const size_type index, const typename ValueType::value_type &);

	ValueType*
	get1Value (const size_type);

	///  Set @a value to this field without notfying parents.
	virtual
	void
	set (const value_type &);

	template <class InputIterator>
	void
	set (InputIterator, InputIterator);

	ValueType &
	operator [ ] (const size_type index)
	{ return *get () [index]; }

	const ValueType &
	operator [ ] (const size_type index) const
	{ return *getValue () [index]; }

	template <class InputIterator>
	void
	assign (InputIterator, InputIterator);

	ValueType &
	at (const size_type index)
	{ return *get () .at (index); }

	const ValueType &
	at (const size_type index) const
	{ return *getValue () .at (index); }

	ValueType &
	back ()
	{ return *get () .back (); }

	const ValueType &
	back () const
	{ return *getValue () .back (); }

	iterator
	begin ()
	{ return iterator (get () .begin ()); }

	const_iterator
	begin () const
	{ return const_iterator (getValue () .begin ()); }

	const_iterator
	cbegin () const
	{ return const_iterator (getValue () .cbegin ()); }

	//size_type capacity () const { return getValue () .capacity (); };

	void
	clear ();

	bool
	empty () const
	{ return getValue () .empty (); }

	iterator
	end ()
	{ return iterator (get () .end ()); }

	const_iterator
	end () const
	{ return const_iterator (getValue () .end ()); }

	const_iterator
	cend () const
	{ return const_iterator (getValue () .cend ()); }

	iterator
	erase (iterator);

	iterator
	erase (iterator, iterator);

	ValueType &
	front ()
	{ return *get () .front (); }

	const ValueType &
	front () const
	{ return *getValue () .front (); }

	iterator
	insert (iterator, const ValueType &);

	iterator
	insert (iterator, size_type, const ValueType &);

	template <class InputIterator>
	iterator
	insert (iterator, InputIterator, InputIterator);

	size_type
	max_size () const
	{ return getValue () .max_size (); }

	void
	pop_front ();

	void
	pop_back ();

	template <class ... Args>
	void
	emplace_front (Args && ... args);

	template <class ... Args>
	void
	emplace_back (Args && ... args);

	reverse_iterator
	rbegin ()
	{ return reverse_iterator (get () .rbegin ()); }

	const_reverse_iterator
	rbegin () const
	{ return const_reverse_iterator (getValue () .rbegin ()); }

	const_reverse_iterator
	crbegin () const
	{ return const_reverse_iterator (getValue () .crbegin ()); }

	reverse_iterator
	rend ()
	{ return reverse_iterator (get () .rend ()); }

	const_reverse_iterator
	rend () const
	{ return const_reverse_iterator (getValue () .rend ()); }

	const_reverse_iterator
	crend () const
	{ return const_reverse_iterator (getValue () .crend ()); }

	//	void reserve (size_type size) { get () .reserve (size); };

	void
	resize (size_type);

	void
	resize (size_type, const ValueType &);

	size_type
	size () const
	{ return getValue () .size (); }

	///  @name Input operator.
	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	///  @name Output operator.
	virtual
	void
	toStream (std::ostream &) const override;

	virtual
	~X3DArrayField ();


protected:

	using X3DField <value_type>::get;

	virtual
	void
	reset () override;


private:

	using X3DField <value_type>::notifyParents;

	void
	addChildren (typename iterator::iterator_type,
	             typename iterator::iterator_type);

	void
	addChild (ValueType*);

	void
	removeChildren (typename iterator::iterator_type,
	                typename iterator::iterator_type);

	void
	removeChild (ValueType*);

};

template <class ValueType>
bool
X3DArrayField <ValueType>::operator == (const X3DFieldDefinition & field) const
{
	if (getType () == field .getType ())
	{
		return size () == static_cast <const X3DArrayField &> (field) .size () &&
		       std::equal (begin (), end (),
		                   static_cast <const X3DArrayField &> (field) .begin ());
	}

	return false;
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::set1Value (const size_type index, const ValueType & field)
{
	set1Value (index, field .getValue ());
}

template <class ValueType>
void
X3DArrayField <ValueType>::set1Value (const size_type index, const typename ValueType::value_type & value)
{
	if (index >= size ())
		resize (index + 1);

	*get () [index] = value;
}

template <class ValueType>
ValueType*
X3DArrayField <ValueType>::get1Value (const size_type index)
{
	if (index >= size ())
		resize (index + 1);

	return get () [index];
}

template <class ValueType>
void
X3DArrayField <ValueType>::set (const value_type & value)
{
	set (const_iterator (value .cbegin ()), const_iterator (value .cend ()));
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::assign (InputIterator first, InputIterator last)
{
	set (first, last);

	notifyParents ();
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::set (InputIterator first, InputIterator last)
{
	iterator current = begin ();

	for (auto end = this -> end (); first not_eq last && current not_eq end; ++ current, ++ first)
		current -> set (*first);

	if (first == last)
	{
		// Remove trailing fields

		size_t count = current - begin ();

		removeChildren (get () .begin () + count, get () .end ());

		get () .resize (count);
	}
	else
	{
		// Insert at end

		for ( ; first not_eq last; ++ first)
		{
			ValueType* field = new ValueType (*first);

			get () .emplace_back (field);

			addChild (field);
		}
	}
}

template <class ValueType>
void
X3DArrayField <ValueType>::clear ()
{
	reset ();

	notifyParents ();
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (iterator location)
{
	removeChild (*location .base ());

	auto iter = get () .erase (location .base ());

	notifyParents ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (iterator first, iterator last)
{
	removeChildren (first .base (), last .base ());

	auto iter = get () .erase (first .base (), last .base ());

	notifyParents ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (iterator location, const ValueType & value)
{
	auto iter = get () .insert (location .base (), new ValueType (value));

	addChild (*iter);

	notifyParents ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (iterator location, size_type count, const ValueType & value)
{
	size_type pos = location - begin ();

	get () .insert (location .base (), count, NULL);

	auto iter = get () .begin () + pos;

	for (auto & field : basic::adapter (iter, iter + count))
	{
		field = new ValueType ();

		addChild (field);
	}

	notifyParents ();
	return iterator (iter);
}

template <class ValueType>
template <class InputIterator>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (iterator location, InputIterator first, InputIterator last)
{
	size_type pos   = location - begin ();
	size_type count = last - first;

	get () .insert (location .base (), count, NULL);

	auto iter = get () .begin () + pos;

	for (auto & field : basic::adapter (iter, iter + count))
	{
		field = new ValueType (*first);

		addChild (field);

		++ first;
	}

	notifyParents ();
	return iterator (iter);
}

template <class ValueType>
void
X3DArrayField <ValueType>::pop_front ()
{
	removeChild (get () .front ());

	get () .pop_front ();

	notifyParents ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::pop_back ()
{
	removeChild (get () .back ());

	get () .pop_back ();

	notifyParents ();
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_front (Args && ... args)
{
	ValueType* field = new ValueType (args ...);

	get () .emplace_front (field);

	addChild (field);

	notifyParents ();
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_back (Args && ... args)
{
	ValueType* field = new ValueType (args ...);

	get () .emplace_back (field);

	addChild (field);

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
		get () .resize (count, NULL);

		for (auto & field : basic::adapter (get () .begin () + currentSize, get () .end ()))
		{
			field = new ValueType (value);
			addChild (field);
		}

		notifyParents ();
	}
	else if (count < currentSize)
	{
		removeChildren (get () .begin () + count, get () .end ());

		get () .resize (count);

		notifyParents ();
	}
}

template <class ValueType>
void
X3DArrayField <ValueType>::fromStream (std::istream & istream)
throw (Error <INVALID_X3D>,
       Error <NOT_SUPPORTED>,
       Error <INVALID_OPERATION_TIMING>,
       Error <DISPOSED>)
{
	clear ();

	ValueType value;

	while (istream >> value)
		emplace_back (value);
}

template <class ValueType>
void
X3DArrayField <ValueType>::toStream (std::ostream & ostream) const
{
	if (size () > 1)
	{
		ostream << Generator::OpenBracket;

		std::copy (begin (),
		           end () - 1,
		           Generator::ListSeparator <ValueType> (ostream));

		ostream << back () << Generator::CloseBracket;

		return;
	}

	if (size () == 1)
	{
		ostream << front ();
		return;
	}

	ostream << Generator::EmptyBrackets;
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChildren (typename iterator::iterator_type first,
                                        typename iterator::iterator_type last)
{
	for (auto & field : basic::adapter (first, last))
		addChild (field);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChild (ValueType* field)
{
	field -> addParent (this);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::removeChildren (typename iterator::iterator_type first,
                                           typename iterator::iterator_type last)
{
	for (auto & field : basic::adapter (first, last))
		removeChild (field);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::removeChild (ValueType* field)
{
	field -> removeParent (this);
}

template <class ValueType>
void
X3DArrayField <ValueType>::reset ()
{
	removeChildren (get () .begin (), get () .end ());

	get () .clear ();
}

template <class ValueType>
X3DArrayField <ValueType>::~X3DArrayField ()
{
	reset ();
}

template <class ValueType>
inline
bool
operator == (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return lhs .size () == rhs .size () &&
	       std::equal (lhs .begin (), lhs .end (),
	                   rhs .begin ());
}

template <class ValueType>
inline
bool
operator not_eq (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return not (lhs == rhs);
}

template <class ValueType>
inline
bool
operator < (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return std::lexicographical_compare (lhs .begin (), lhs .end (),
	                                     rhs .begin (), rhs .end ());
}

template <class ValueType>
inline
bool
operator > (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return rhs < lhs;
}

template <class ValueType>
inline
bool
operator <= (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return not (rhs < lhs);
}

template <class ValueType>
inline
bool
operator >= (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return not (lhs < rhs);
}

} // X3D
} // titania

#endif
