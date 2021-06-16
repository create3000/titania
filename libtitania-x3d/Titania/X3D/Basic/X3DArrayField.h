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

#ifndef __TITANIA_X3D_BASIC_X3DARRAY_FIELD_H__
#define __TITANIA_X3D_BASIC_X3DARRAY_FIELD_H__

#include "../Basic/X3DField.h"
#include "../InputOutput/Generator.h"
#include "../Types/Array.h"

#include <Titania/Algorithm.h>
#include <Titania/Basic/ReferenceIterator.h>
#include <Titania/Utility/Range.h>
#include <initializer_list>

namespace titania {
namespace X3D {

/**
 *  Template to represent a X3D array field.
 *
 *  Extern instantiations for all single X3DField types are part of the
 *  library.  Results with any other X3DField type are not guaranteed.
 *
 *  @param  ValueType  Type of the values of the array field.
 */
template <class ValueType>
class X3DArrayField :
	public X3DField <Array <ValueType*>>
{
public:

	///  @name Member types

	using value_type    = ValueType;
	using internal_type = Array <ValueType*>;

	using allocator_type  = typename internal_type::allocator_type;
	using size_type       = typename internal_type::size_type;
	using difference_type = typename internal_type::difference_type;

	using reference       = value_type &;
	using const_reference = const value_type &;

	using pointer       = value_type*;
	using const_pointer = const value_type*;

	using iterator               = basic::reference_iterator <typename internal_type::iterator, ValueType>;
	using reverse_iterator       = basic::reference_iterator <typename internal_type::reverse_iterator, ValueType>;
	using const_iterator         = basic::reference_iterator <typename internal_type::const_iterator, const ValueType>;
	using const_reverse_iterator = basic::reference_iterator <typename internal_type::const_reverse_iterator, const ValueType>;

	using X3DField <internal_type>::addInterest;
	using X3DField <internal_type>::addEvent;
	using X3DField <internal_type>::copy;
	using X3DField <internal_type>::getType;
	using X3DField <internal_type>::getValue;
	using X3DField <internal_type>::getUnit;
	using X3DField <internal_type>::operator =;

	///  @name Construction

	///  Default constructor.
	X3DArrayField () :
		X3DField <internal_type> ()
	{ }

	///  Copy constructor.
	X3DArrayField (const X3DArrayField & other) :
		X3DArrayField <ValueType> (other .begin (), other .end ())
	{ }

	///  Move constructor.
	X3DArrayField (X3DArrayField && other) :
		X3DField <internal_type> (std::move (other .get ()))
	{
		for (const auto & value : getValue ())
			moveChild (other, value);

		other .addEvent ();
	}

	///  Construct new X3DArrayField.
	X3DArrayField (std::initializer_list <ValueType> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	///  Construct new X3DArrayField.
	X3DArrayField (std::initializer_list <const typename ValueType::internal_type> list) :
		X3DArrayField <ValueType> (list .begin (), list .end ())
	{ }

	///  Construct new X3DArrayField.
	explicit
	X3DArrayField (const size_type count) :
		X3DArrayField <ValueType> ()
	{ resize (count); }

	///  Construct new X3DArrayField.
	X3DArrayField (const size_type count, const ValueType & value) :
		X3DArrayField ()
	{ resize (count, value); }

	///  Construct new X3DArrayField.
	template <class InputIterator>
	X3DArrayField (const InputIterator &, const InputIterator &);

	///  Construct new X3DArrayField.
	virtual
	X3DArrayField*
	create () const override
	{ return new X3DArrayField (); }

	///  Construct new X3DArrayField.
   ///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DArrayField*
	copy (const CopyType) const override
	{ return new X3DArrayField (*this); }

	///  Construct new X3DArrayField.
   ///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DArrayField*
	copy (X3DExecutionContext* const, const CopyType type) const override
	{ return copy (type); }

	///  @name Assignment operators

	/// Assigns values to the array.
	X3DArrayField &
	operator = (const X3DArrayField & other)
	{
		assign (other .begin (), other .end ());
		return *this;
	}

	/// Assigns values to the array.
	X3DArrayField &
	operator = (X3DArrayField && other);

//	/// Assigns values to the array.
//	X3DArrayField &
//	operator = (std::initializer_list <ValueType> list)
//	{
//		assign (list .begin (), list .end ());
//		return *this;
//	}

	/// Assigns values to the array.
	X3DArrayField &
	operator = (std::initializer_list <typename ValueType::internal_type> list)
	{
		assign (list .begin (), list .end ());
		return *this;
	}

	/// Assigns values to the array.
	template <class InputIterator>
	void
	assign (const InputIterator & first, const InputIterator & last)
	{
		set (first, last);
		addEvent ();
	}

	///  Assigns @a value to the array without notfying its parents.
	virtual
	void
	set (const internal_type & value) override
	{ set (const_iterator (value .cbegin ()), const_iterator (value .cend ())); }

	///  Assigns values to the array without notfying its parents.
	template <class InputIterator>
	void
	set (InputIterator, const InputIterator &);

	///  @name Boolean operations

	///  Always returs true.
	virtual
	bool
	isArray () const override
	{ return true; }

	///  @name Comparision operations

	///  Returns true if the contents of the arrays are equal, false otherwise.
	virtual
	bool
	equals (const X3DFieldDefinition &) const final override;

	///  @name Element access

	///  Access specified element with bounds checking and element creation.
	template <class Arg>
	void
	set1Value (const size_type index, Arg &&);

	///  Access specified element with bounds checking and element creation.
	reference
	get1Value (const size_type);

	///  Access specified element.
	reference
	operator [ ] (const size_type index)
	{ return *get () [index]; }

	///  Access specified element.
	const_reference
	operator [ ] (const size_type index) const
	{ return *getValue () [index]; }

	///  Access specified element with bounds checking.
	reference
	at (const size_type index)
	{ return *get () .at (index); }

	///  Access specified element with bounds checking.
	const_reference
	at (const size_type index) const
	{ return *getValue () .at (index); }

	///  Access the first element.
	reference
	front ()
	{ return *get () .front (); }

	///  Access the first element.
	const_reference
	front () const
	{ return *getValue () .front (); }

	///  Access the last element.
	reference
	back ()
	{ return *get () .back (); }

	///  Access the last element.
	const_reference
	back () const
	{ return *getValue () .back (); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	iterator
	begin ()
	{ return iterator (get () .begin ()); }

	///  Returns an iterator to the beginning.
	const_iterator
	begin () const
	{ return const_iterator (getValue () .begin ()); }

	///  Returns an iterator to the beginning.
	const_iterator
	cbegin () const
	{ return const_iterator (getValue () .cbegin ()); }

	///  Returns an iterator to the end.
	iterator
	end ()
	{ return iterator (get () .end ()); }

	///  Returns an iterator to the end.
	const_iterator
	end () const
	{ return const_iterator (getValue () .end ()); }

	///  Returns an iterator to the end.
	const_iterator
	cend () const
	{ return const_iterator (getValue () .cend ()); }

	///  Returns a reverse iterator to the beginning.
	reverse_iterator
	rbegin ()
	{ return reverse_iterator (get () .rbegin ()); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	rbegin () const
	{ return const_reverse_iterator (getValue () .rbegin ()); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	crbegin () const
	{ return const_reverse_iterator (getValue () .crbegin ()); }

	///  Returns a reverse iterator to the end.
	reverse_iterator
	rend ()
	{ return reverse_iterator (get () .rend ()); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	rend () const
	{ return const_reverse_iterator (getValue () .rend ()); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	crend () const
	{ return const_reverse_iterator (getValue () .crend ()); }

	///  @name Capacity

	///  Checks whether the container is empty.
	[[nodiscard]]
	bool
	empty () const
	{ return getValue () .empty (); }

	///  Returns the number of elements.
	size_type
	size () const
	{ return getValue () .size (); }

	///  Returns the maximum possible number of elements.
	size_type
	max_size () const
	{ return getValue () .max_size (); }

	///  Reserves storage.
	void
	reserve (const size_type size)
	{ get () .reserve (size); }

	///  Returns the number of elements that can be held in currently allocated storage.
	size_type
	capacity () const
	{ return getValue () .capacity (); }

	///  Reduces memory usage by freeing unused memory.
	void
	shrink_to_fit ()
	{ get () .shrink_to_fit (); }

	///  @name Modifiers

	///  Clears the contents.
	void
	clear ();

	///  Inserts elements.
	iterator
	insert (const iterator & location, const ValueType & value)
	{ return emplace (location, value); }

	///  Inserts elements.
	iterator
	insert (const iterator & location, const size_type count, const ValueType & value)
	{ return emplace (location, count, value); }

	///  Inserts elements.
	template <class InputIterator>
	iterator
	insert (const iterator &, InputIterator, const InputIterator &);

	template <class Arg>
	iterator
	emplace (const iterator &, Arg &&);

	///  Inserts elements.
	template <class Arg>
	iterator
	emplace (const iterator &, const size_type, Arg &&);

	///  Appends X3DArrayField @a field.
	X3DArrayField &
	append (const X3DArrayField & field);

	///  Appends X3DArrayField @a field.
	X3DArrayField &
	append (X3DArrayField && field);

	///  Erases elements.
	iterator
	erase (const iterator &);

	///  Erases elements.
	iterator
	erase (const iterator &, const iterator &);

	///  Removes elements completely.
	void
	remove (const ValueType & value)
	{ erase (std::remove (begin (), end (), value), end ()); }

	///  Inserts elements to the beginning.
	void
	push_front (const ValueType & value)
	{ emplace_front (value); }

	///  Constructs elements in-place at the beginning.
	template <class ... Args>
	void
	emplace_front (Args && ... args);

	///  Removes the first element.
	void
	pop_front ();

	///  Adds elements to the end.
	void
	push_back (const ValueType & value)
	{ emplace_back (value); }

	///  Constructs elements in-place at the end.
	template <class ... Args>
	void
	emplace_back (Args && ... args);

	///  Removes the last element.
	void
	pop_back ();

	///  Changes the number of elements stored.
	void
	resize (const size_type count)
	{ resize (count, ValueType ()); }

	///  Changes the number of elements stored.
	void
	resize (const size_type count, const ValueType & value);

	///  Swaps the contents.
	void
	swap (X3DArrayField &);

	///  @name Algorithms

	///  Returns an array with all indices where the value is equal @a value.
	std::vector <size_t>
	indices_of (const ValueType & value) const
	{ return indices_of (value .getValue ()); }

	///  Returns an array with all indices where the value is equal @a value.
	std::vector <size_t>
	indices_of (const typename ValueType::internal_type & value) const
	{ return basic::indices_of (begin (), end (), value); }

	///  @name Interest service

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DArrayField &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DArrayField &), Class & object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  @name Input/Output

	///  Extracts the value for this object from @a istream. The contents of @a istream must be in VRML Classic Encoding.
	///  throws Error <INVALID_X3D>, Error <NOT_SUPPORTED>, Error <INVALID_OPERATION_TIMING>, Error <DISPOSED>
	virtual
	void
	fromStream (std::istream &) override;

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream &) const override;

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream &) const override;

	///  Inserts this object into @a ostream in X3D JSON Encoding style.
	virtual
	void
	toJSONStream (std::ostream &) const override;

	///  @name Destruction

	///  Destructs the X3DArrayField.
	virtual
	~X3DArrayField ()
	{ reset (); }


protected:

	using X3DField <internal_type>::get;

	///  @name Element handling

	void
	addChildren (const typename iterator::iterator_type &,
	             const typename iterator::iterator_type &);

	virtual
	void
	addChild (ValueType*);

	virtual
	void
	moveChild (X3DArrayField & other, ValueType*);

	void
	removeChildren (const typename iterator::iterator_type &,
	                const typename iterator::iterator_type &);

	virtual
	void
	removeChild (ValueType*);

	///  @name Element access

	///  Clears the contents without notifying its parents.
	virtual
	void
	reset () override;

};

template <class ValueType>
template <class InputIterator>
X3DArrayField <ValueType>::X3DArrayField (const InputIterator & first, const InputIterator & last) :
	X3DField <internal_type> ()
{
	// Insert at end

	for (const auto & value : std::pair (first, last))
	{
		ValueType* const field = new ValueType (value);

		get () .emplace_back (field);

		addChild (field);
	}
}

template <class ValueType>
X3DArrayField <ValueType> &
X3DArrayField <ValueType>::operator = (X3DArrayField && other)
{
	if (&other == this)
	{
		addEvent ();
		return *this;
	}

	clear ();

	std::swap (get (), other .get ());

	for (const auto & value : getValue ())
		moveChild (other, value);

	other .addEvent ();
	addEvent ();

	return *this;
}

template <class ValueType>
bool
X3DArrayField <ValueType>::equals (const X3DFieldDefinition & field) const
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

		for (; first not_eq last; ++ first)
		{
			ValueType* const field = new ValueType (*first);

			get () .emplace_back (field);

			addChild (field);
		}
	}
}

template <class ValueType>
template <class Arg>
inline
void
X3DArrayField <ValueType>::set1Value (const size_type index, Arg && value)
{
	if (index >= size ())
		resize (index + 1);

	*get () [index] = std::forward <Arg> (value);
}

template <class ValueType>
inline
ValueType &
X3DArrayField <ValueType>::get1Value (const size_type index)
{
	if (index >= size ())
		resize (index + 1);

	return *get () [index];
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::clear ()
{
	reset ();

	addEvent ();
}

template <class ValueType>
template <class Arg>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::emplace (const iterator & location, Arg && value)
{
	const auto iter = get () .insert (location .base (), new ValueType (std::forward <Arg> (value)));

	addChild (*iter);

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
template <class Arg>
typename X3DArrayField <ValueType>::iterator
X3DArrayField <ValueType>::emplace (const iterator & location, const size_type count, Arg && value)
{
	const size_type pos = location - begin ();

	get () .insert (location .base (), count, nullptr);

	const auto iter = get () .begin () + pos;

	for (auto & field : basic::make_range (iter, count))
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

	if (count)
	{
		for (auto & field : basic::make_range (iter, count))
		{
			field = new ValueType (*first);

			addChild (field);

			++ first;
		}

		addEvent ();
	}

	return iterator (iter);
}

template <class ValueType>
inline
X3DArrayField <ValueType> &
X3DArrayField <ValueType>::append (const X3DArrayField & field)
{
	insert (end (), field .begin (), field .end ());
	return *this;
}

template <class ValueType>
inline
X3DArrayField <ValueType> &
X3DArrayField <ValueType>::append (X3DArrayField && field)
{
	for (auto & value : field)
		emplace_back (std::move (value));

	field .clear ();
	return *this;
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
	if (first == last)
		return first;

	removeChildren (first .base (), last .base ());

	const auto iter = get () .erase (first .base (), last .base ());

	addEvent ();
	return iterator (iter);
}

template <class ValueType>
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_front (Args && ... args)
{
	ValueType* const field = new ValueType (std::forward <Args> (args) ...);

	get () .insert (get () .begin (), field);

	addChild (field);

	addEvent ();
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
template <class ... Args>
void
X3DArrayField <ValueType>::emplace_back (Args && ... args)
{
	ValueType* const field = new ValueType (std::forward <Args> (args) ...);

	get () .emplace_back (field);

	addChild (field);

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
X3DArrayField <ValueType>::resize (const size_type count, const ValueType & value)
{
	const size_type currentSize = size ();

	if (count > currentSize)
	{
		get () .resize (count, nullptr);

		for (auto & field : std::pair (get () .begin () + currentSize, get () .end ()))
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
X3DArrayField <ValueType>::swap (X3DArrayField & other)
{
	for (auto & value : get ())
		other .moveChild (*this, value);

	for (auto & value : other .get ())
		moveChild (other, value);

	std::swap (get (), other .get ());

	addEvent ();
	other .addEvent ();
}

template <class ValueType>
void
X3DArrayField <ValueType>::fromStream (std::istream & istream)
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
			ostream << Generator::EmptyBrackets;
			return;
		}
		case 1:
		{
			Generator::PushUnitCategory (ostream, getUnit ());

			ostream << front ();

			Generator::PopUnitCategory (ostream);
			return;
		}
		default:
		{
			Generator::PushUnitCategory (ostream, getUnit ());

			ostream << Generator::OpenBracket;

			for (const auto & value : std::pair (cbegin (), cend () - 1))
			{
				ostream
					<< value
					<< Generator::Comma
					<< Generator::ListBreak;

				if (Generator::HasListBreak (ostream))
					ostream << Generator::Indent;
			}

			ostream
				<< back ()
				<< Generator::CloseBracket;

			Generator::PopUnitCategory (ostream);

			return;
		}
	}
}

template <class ValueType>
void
X3DArrayField <ValueType>::toXMLStream (std::ostream & ostream) const
{
	if (empty ())
		return;

	Generator::PushUnitCategory (ostream, getUnit ());

	for (const auto & value : std::pair (cbegin (), cend () - 1))
	{
		ostream
			<< XMLEncode (value)
			<< Generator::Comma
			<< Generator::TidySpace;
	}

	ostream << XMLEncode (back ());

	Generator::PopUnitCategory (ostream);
}

template <class ValueType>
void
X3DArrayField <ValueType>::toJSONStream (std::ostream & ostream) const
{
	if (empty ())
	{
		ostream
			<< '['
			<< Generator::TidySpace
			<< ']';
	}
	else
	{
		Generator::PushUnitCategory (ostream, getUnit ());

		ostream
			<< '['
			<< Generator::ListBreak
			<< Generator::IncIndent;

		for (const auto & value : std::pair (cbegin (), cend () - 1))
		{
			if (Generator::HasListBreak (ostream))
				ostream << Generator::Indent;

			value .toJSONStreamValue (ostream);

			ostream
				<< ','
				<< Generator::ListBreak;
		}

		if (Generator::HasListBreak (ostream))
			ostream << Generator::Indent;

		back () .toJSONStreamValue (ostream);

		ostream
			<< Generator::ListBreak
 			<< Generator::DecIndent;

		if (Generator::HasListBreak (ostream))
			ostream << Generator::Indent;

		ostream << ']';

		Generator::PopUnitCategory (ostream);
	}
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChildren (const typename iterator::iterator_type & first,
                                        const typename iterator::iterator_type & last)
{
	for (auto & value : std::pair (first, last))
		addChild (value);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::addChild (ValueType* value)
{
	value -> addParent (this);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::moveChild (X3DArrayField & other, ValueType* value)
{
	value -> replaceParent (&other, this);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::removeChildren (const typename iterator::iterator_type & first,
                                           const typename iterator::iterator_type & last)
{
	for (auto & value : std::pair (first, last))
		removeChild (value);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::removeChild (ValueType* value)
{
	value -> removeParent (this);
}

template <class ValueType>
inline
void
X3DArrayField <ValueType>::reset ()
{
	removeChildren (get () .begin (), get () .end ());

	get () .clear ();
}

///  @relates X3DArrayField
///  @name Comparision operations

///  Compares two X3DArrayField.
///  Returns true if @a lhs is equal to @a rhs.
template <class ValueType>
inline
bool
operator == (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return lhs .size () == rhs .size () &&
	       std::equal (lhs .begin (), lhs .end (),
	                   rhs .begin ());
}

///  Compares two X3DArrayField.
///  Returns true if @a lhs is not equal to @a rhs.
template <class ValueType>
inline
bool
operator not_eq (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return not (lhs == rhs);
}

///  Lexicographically compares two X3DArrayField.
///  Returns true if @a lhs less than @a rhs.
template <class ValueType>
inline
bool
operator < (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return std::lexicographical_compare (lhs .begin (), lhs .end (),
	                                     rhs .begin (), rhs .end ());
}

///  Lexicographically compares two X3DArrayField.
///  Returns true if @a lhs less than equal to @a rhs.
template <class ValueType>
inline
bool
operator > (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two X3DArrayField.
///  Returns true if @a lhs greater than @a rhs.
template <class ValueType>
inline
bool
operator <= (const X3DArrayField <ValueType> & lhs, const X3DArrayField <ValueType> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two X3DArrayField.
///  Returns true if @a lhs greater than equal to @a rhs.
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
