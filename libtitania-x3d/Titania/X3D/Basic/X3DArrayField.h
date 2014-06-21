/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#include <Titania/Algorithm.h>
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

	typedef ValueType         value_type;
	typedef Array <ValueType> internal_type;

	typedef basic::reference_iterator <typename internal_type::iterator, ValueType>                     iterator;
	typedef basic::reference_iterator <typename internal_type::reverse_iterator, ValueType>             reverse_iterator;
	typedef basic::reference_iterator <typename internal_type::const_iterator, const ValueType>         const_iterator;
	typedef basic::reference_iterator <typename internal_type::const_reverse_iterator, const ValueType> const_reverse_iterator;

	typedef typename internal_type::difference_type difference_type;
	typedef typename internal_type::size_type       size_type;

	using X3DField <internal_type>::addInterest;
	using X3DField <internal_type>::addEvent;
	using X3DField <internal_type>::getType;
	using X3DField <internal_type>::getValue;
	using X3DField <internal_type>::operator =;

	///  @name Construction

	///  Default constructor.
	X3DArrayField () :
		X3DField <internal_type> ()
	{ }

	///  Copy constructor.
	X3DArrayField (const X3DArrayField & field) :
		X3DArrayField <ValueType> (field .begin (), field .end ())
	{ }

	///  Move constructor.
	X3DArrayField (X3DArrayField && field) :
		X3DField <internal_type> (std::move (field .get ()))
	{
		for (const auto & value : getValue ())
			value -> replaceParent (&field, this);

		field .addEvent ();
	}

	///  Construct an X3DArrayField from basic type @a value.
	explicit
	X3DArrayField (const internal_type & value) :
		X3DArrayField <ValueType> (const_iterator (value .begin ()), const_iterator (value .end ()))
	{ }

	//	///  Moves elements from basic type @a value.
	//	explicit
	//	X3DArrayField (internal_type && value) :
	//		X3DField <internal_type> (value)
	//	{
	//		addChildren (get () .begin (), get () .end ());
	//	}

	template <const size_t Size>
	X3DArrayField (const ValueType (&value) [Size]) :
		X3DArrayField <ValueType> (value, value + Size)
	{ }

	template <const size_t Size>
	X3DArrayField (const typename ValueType::internal_type (&value) [Size]) :
		X3DArrayField <ValueType> (value, value + Size)
	{ }

	X3DArrayField (std::initializer_list <ValueType> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	X3DArrayField (std::initializer_list <const typename ValueType::internal_type> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	template <class InputIterator>
	X3DArrayField (const InputIterator &, const InputIterator &);

	virtual
	X3DArrayField*
	create () const override
	{ return new X3DArrayField (); }

	virtual
	X3DArrayField*
	clone () const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override
	{ return new X3DArrayField (*this); }

	virtual
	X3DArrayField*
	clone (X3DExecutionContext* const) const
	throw (Error <INVALID_NAME>,
	       Error <NOT_SUPPORTED>) override
	{ return clone (); }

	///  @name Assignment operators

	X3DArrayField &
	operator = (const X3DArrayField & field)
	{
		X3DField <internal_type>::operator = (field);
		return *this;
	}

	X3DArrayField &
	operator = (X3DArrayField &&);

	template <const size_t Size>
	X3DArrayField &
	operator = (const ValueType (&value) [Size])
	{
		assign (value, value + Size);
		return *this;
	}

	template <const size_t Size>
	X3DArrayField &
	operator = (const typename ValueType::internal_type (&value) [Size])
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
	operator = (std::initializer_list <typename ValueType::internal_type> list)
	{
		assign (list .begin (), list .end ());
		return *this;
	}

	///  @name Tests

	virtual
	bool
	isArray () const final override
	{ return true; }

	virtual
	bool
	operator == (const X3DFieldDefinition &) const final override;

	///  @name Event handling

	///  6.7.7 Add field interest.

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (const X3DArrayField &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (const X3DArrayField &)) const
	{ addInterest (object, memberFunction, std::cref (*this)); }

	///  @name Element access

	void
	set1Value (const size_type index, const ValueType &);

	void
	set1Value (const size_type index, const typename ValueType::internal_type &);

	ValueType*
	get1Value (const size_type);

	///  Set @a value to this field without notfying parents.
	virtual
	void
	set (const internal_type &) final override;

	template <class InputIterator>
	void
	set (InputIterator, const InputIterator &);

	ValueType &
	operator [ ] (const size_type index)
	{ return *get () [index]; }

	const ValueType &
	operator [ ] (const size_type index) const
	{ return *getValue () [index]; }

	template <class InputIterator>
	void
	assign (const InputIterator &, const InputIterator &);

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

	//size_type
	//capacity () const
	//{ return getValue () .capacity (); }

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
	erase (const iterator &);

	iterator
	erase (const iterator &, const iterator &);

	iterator
	remove (const ValueType & value)
	{ return erase (std::remove (begin (), end (), value), end ()); }

	ValueType &
	front ()
	{ return *get () .front (); }

	const ValueType &
	front () const
	{ return *getValue () .front (); }

	iterator
	insert (const iterator &, const ValueType &);

	iterator
	insert (const iterator &, const size_type, const ValueType &);

	template <class InputIterator>
	iterator
	insert (const iterator &, InputIterator, const InputIterator &);

	void
	pop_front ();

	void
	pop_back ();

	void
	push_front (const ValueType &);

	void
	push_back (const ValueType &);

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

	void
	reserve (const size_type size)
	{ get () .reserve (size); };

	size_type
	capacity () const
	{ return getValue () .capacity (); };

	void
	shrink_to_fit ()
	{ get () .shrink_to_fit (); };

	void
	resize (const size_type);

	void
	resize (const size_type, const ValueType &);

	size_type
	max_size () const
	{ return getValue () .max_size (); }

	size_type
	size () const
	{ return getValue () .size (); }

	///  @name Search

	std::vector <size_t>
	indices_of (const ValueType & value) const
	{ return indices_of (value .getValue ()); }

	std::vector <size_t>
	indices_of (const typename ValueType::internal_type & value) const
	{ return basic::indices_of (begin (), end (), value); }

	///  @name Input/Output

	virtual
	void
	fromStream (std::istream &)
	throw (Error <INVALID_X3D>,
	       Error <NOT_SUPPORTED>,
	       Error <INVALID_OPERATION_TIMING>,
	       Error <DISPOSED>) override;

	virtual
	void
	toStream (std::ostream &) const override;

	virtual
	void
	toXMLStream (std::ostream &) const override;

	///  @name Destruction

	virtual
	~X3DArrayField ();


protected:

	using X3DField <internal_type>::get;

	///  @name Element access

	virtual
	void
	reset () override;


private:

	///  @name Element handling

	void
	addChildren (const typename iterator::iterator_type &,
	             const typename iterator::iterator_type &);

	void
	addChild (ValueType*);

	void
	removeChildren (const typename iterator::iterator_type &,
	                const typename iterator::iterator_type &);

	void
	removeChild (ValueType*);

};

template <class ValueType>
template <class InputIterator>
X3DArrayField <ValueType>::X3DArrayField (const InputIterator & first, const InputIterator & last) :
	X3DField <internal_type> ()
{
	// Insert at end

	for (const auto & value : basic::adapter (first, last))
	{
		ValueType* const field = new ValueType (value);

		get () .emplace_back (field);

		addChild (field);
	}
}

template <class ValueType>
X3DArrayField <ValueType> &
X3DArrayField <ValueType>::operator = (X3DArrayField && field)
{
	if (&field == this)
		return *this;

	clear (); // addEvent ();

	std::swap (get (), field .get ());

	for (const auto & value : getValue ())
		value -> replaceParent (&field, this);

	field .addEvent ();

	return *this;
}

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
X3DArrayField <ValueType>::set1Value (const size_type index, const typename ValueType::internal_type & value)
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
X3DArrayField <ValueType>::set (const internal_type & value)
{
	set (const_iterator (value .cbegin ()), const_iterator (value .cend ()));
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::assign (const InputIterator & first, const InputIterator & last)
{
	set (first, last);

	addEvent ();
}

template <class ValueType>
template <class InputIterator>
void
X3DArrayField <ValueType>::set (InputIterator first, const InputIterator & last)
{
	iterator current = begin ();

	for (const auto end = this -> end (); first not_eq last && current not_eq end; ++ current, ++ first)
		current -> set (*first);

	if (first == last)
	{
		// Remove trailing fields

		const size_t count = current - begin ();

		removeChildren (get () .begin () + count, get () .end ());

		get () .resize (count);
	}
	else
	{
		// Insert at end

		for ( ; first not_eq last; ++ first)
		{
			ValueType* const field = new ValueType (*first);

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

	addEvent ();
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (const iterator & location)
{
	removeChild (*location .base ());

	const auto iter = get () .erase (location .base ());

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::erase (const iterator & first, const iterator & last)
{
	removeChildren (first .base (), last .base ());

	const auto iter = get () .erase (first .base (), last .base ());

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (const iterator & location, const ValueType & value)
{
	const auto iter = get () .insert (location .base (), new ValueType (value));

	addChild (*iter);

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (const iterator & location, const size_type count, const ValueType & value)
{
	const size_type pos = location - begin ();

	get () .insert (location .base (), count, nullptr);

	const auto iter = get () .begin () + pos;

	for (auto & field : basic::adapter (iter, iter + count))
	{
		field = new ValueType (value);

		addChild (field);
	}

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
template <class InputIterator>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::insert (const iterator & location, InputIterator first, const InputIterator & last)
{
	const size_type pos   = location - begin ();
	const size_type count = last - first;

	get () .insert (location .base (), count, nullptr);

	const auto iter = get () .begin () + pos;

	for (auto & field : basic::adapter (iter, iter + count))
	{
		field = new ValueType (*first);

		addChild (field);

		++ first;
	}

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
void
X3DArrayField <ValueType>::pop_front ()
{
	removeChild (get () .front ());

	get () .erase (get () .begin ());

	addEvent ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::pop_back ()
{
	removeChild (get () .back ());

	get () .pop_back ();

	addEvent ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_front (const ValueType & value)
{
	emplace_front (value);
}

template <class ValueType>
void
X3DArrayField <ValueType>::push_back (const ValueType & value)
{
	emplace_back (value);
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_front (Args && ... args)
{
	ValueType* const field = new ValueType (args ...);

	get () .insert (get () .begin (), field);

	addChild (field);

	addEvent ();
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_back (Args && ... args)
{
	ValueType* const field = new ValueType (args ...);

	get () .emplace_back (field);

	addChild (field);

	addEvent ();
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::resize (const size_type count)
{
	resize (count, ValueType ());
}

template <class ValueType>
void
X3DArrayField <ValueType>::resize (const size_type count, const ValueType & value)
{
	const size_type currentSize = size ();

	if (count > currentSize)
	{
		get () .resize (count, nullptr);

		for (auto & field : basic::adapter (get () .begin () + currentSize, get () .end ()))
		{
			field = new ValueType (value);
			addChild (field);
		}

		addEvent ();
	}
	else if (count < currentSize)
	{
		removeChildren (get () .begin () + count, get () .end ());

		get () .resize (count);

		addEvent ();
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
	if (istream)
	{
		std::string whiteSpaces;

		Grammar::WhiteSpaces (istream, whiteSpaces);

		if (Grammar::OpenBracket (istream))
		{
			X3DArrayField array;
			ValueType     value;

			while (istream >> value)
				array .emplace_back (value);

			istream .clear (~std::ios_base::failbit & istream .rdstate ());

			if (Grammar::CloseBracket (istream))
			{
				*this = array;
				return;
			}
			else
				istream .setstate (std::ios_base::failbit);
		}
		else
		{
			ValueType value;

			if (istream >> value)
			{
				clear ();
				emplace_back (value);
				return;
			}
		}
	}
}

template <class ValueType>
void
X3DArrayField <ValueType>::toStream (std::ostream & ostream) const
{
	switch (size ())
	{
		case 0:
		{
			ostream << X3DGenerator::EmptyBrackets;
			return;
		}
		case 1:
		{
			ostream << front ();
			return;
		}
		default:
		{
			ostream << X3DGenerator::OpenBracket;

			for (const auto & value : basic::adapter (cbegin (), cend () - 1))
			{
				ostream
					<< value
					<< X3DGenerator::Comma
					<< X3DGenerator::ListBreak;
					
				if (X3DGenerator::HasListBreak ())
					ostream << X3DGenerator::Indent;
			}

			ostream
				<< back ()
				<< X3DGenerator::CloseBracket;

			return;
		}
	}
}

template <class ValueType>
void
X3DArrayField <ValueType>::toXMLStream (std::ostream & ostream) const
{
	if (not empty ())
	{
		for (const auto & value : basic::adapter (cbegin (), cend () - 1))
		{
			ostream
				<< XMLEncode (value)
				<< X3DGenerator::Comma
				<< X3DGenerator::Space;
		}

		ostream << XMLEncode (back ());
	}
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChildren (const typename iterator::iterator_type & first,
                                        const typename iterator::iterator_type & last)
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
X3DArrayField <ValueType>::removeChildren (const typename iterator::iterator_type & first,
                                           const typename iterator::iterator_type & last)
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
