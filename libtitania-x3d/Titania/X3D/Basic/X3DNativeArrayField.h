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

#ifndef __TITANIA_X3D_BASIC_X3DNATIVE_ARRAY_FIELD_H__
#define __TITANIA_X3D_BASIC_X3DNATIVE_ARRAY_FIELD_H__

#include "../Basic/X3DField.h"
#include "../InputOutput/Generator.h"
#include "../InputOutput/XMLGenerator.h"
#include "../InputOutput/JSONGenerator.h"
#include "../Parser/Grammar.h"
#include "../Parser/MiniParser.h"
#include "../Types/Array.h"

#include <Titania/Algorithm.h>
#include <Titania/Basic/SubscriptIterator.h>
#include <Titania/Utility/MemberValue.h>
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
class X3DNativeArrayField :
	public X3DField <Array <ValueType>> 
{
public:

	///  @name Member types

	using value_type    = ValueType;
	using internal_type = Array <ValueType>;

	using allocator_type  = typename internal_type::allocator_type;
	using size_type       = typename internal_type::size_type;
	using difference_type = typename internal_type::difference_type;

	using reference       = basic::member_value <value_type>;
	using const_reference = typename internal_type::const_reference;

	using pointer       = typename internal_type::pointer;
	using const_pointer = typename internal_type::const_pointer;

	using iterator               = basic::subscript_iterator <X3DNativeArrayField>;
	using const_iterator         = typename internal_type::const_iterator;
	using reverse_iterator       = std::reverse_iterator <basic::subscript_iterator <X3DNativeArrayField>>;
	using const_reverse_iterator = typename internal_type::const_reverse_iterator;

	using X3DField <internal_type>::addInterest;
	using X3DField <internal_type>::addEvent;
	using X3DField <internal_type>::getType;
	using X3DField <internal_type>::getValue;
	using X3DField <internal_type>::getUnit;
	using X3DField <internal_type>::operator =;

	///  @name Construction

	///  Default constructor.
	X3DNativeArrayField () :
		X3DField <internal_type> ()
	{ }

	///  Copy constructor.
	X3DNativeArrayField (const X3DNativeArrayField & other) :
		X3DNativeArrayField <ValueType> (other .cbegin (), other .cend ())
	{ }

	///  Move constructor.
	X3DNativeArrayField (X3DNativeArrayField && other) :
		X3DField <internal_type> (std::move (other .get ()))
	{
		other .addEvent ();
	}

	///  Construct new X3DNativeArrayField.
	X3DNativeArrayField (std::initializer_list <value_type> list) :
		X3DNativeArrayField <ValueType> (list .begin (), list .end ())
	{ }

	///  Construct new X3DNativeArrayField.
	explicit
	X3DNativeArrayField (const size_type count) :
		X3DNativeArrayField <ValueType> ()
	{ resize (count); }

	///  Construct new X3DNativeArrayField.
	X3DNativeArrayField (const size_type count, const value_type & value) :
		X3DNativeArrayField ()
	{ resize (count, value); }

	///  Construct new X3DNativeArrayField.
	template <class InputIterator>
	X3DNativeArrayField (const InputIterator &, const InputIterator &);

	///  Construct new X3DNativeArrayField.
	virtual
	X3DNativeArrayField*
	create () const override
	{ return new X3DNativeArrayField (); }

	///  Construct new X3DNativeArrayField.
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DNativeArrayField*
	copy (const CopyType) const override
	{ return new X3DNativeArrayField (*this); }

	///  Construct new X3DNativeArrayField.
	///  throws Error <INVALID_NAME>, Error <NOT_SUPPORTED>
	virtual
	X3DNativeArrayField*
	copy (X3DExecutionContext* const, const CopyType type) const override
	{ return copy (type); }

	///  @name Assignment operators

	/// Assigns values to the array.
	X3DNativeArrayField &
	operator = (const X3DNativeArrayField & other)
	{
		assign (other .cbegin (), other .cend ());
		return *this;
	}

	/// Assigns values to the array.
	X3DNativeArrayField &
	operator = (X3DNativeArrayField && other);

	/// Assigns values to the array.
	X3DNativeArrayField &
	operator = (std::initializer_list <value_type> list)
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
	{ set (value .cbegin (), value .cend ()); }

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
	void
	set1Value (const size_type index, const value_type & value);

	///  Access specified element with bounds checking and element creation.
	const_reference
	get1Value (const size_type);

	///  Access specified element.
	reference
	operator [ ] (const size_type index)
	{
		using namespace std::placeholders;

		return reference (std::bind (&X3DNativeArrayField::set1, this, index, _1),
		                  std::bind (&X3DNativeArrayField::get1, this, index));
	}

	///  Access specified element.
	const_reference
	operator [ ] (const size_type index) const
	{ return getValue () [index]; }

	///  Access specified element with bounds checking.
	reference
	at (const size_type index)
	{
		using namespace std::placeholders;

		if (index < get () .size ())
			return operator [ ] (index);

		throw std::out_of_range ("X3DNativeArrayField::at");
	}

	///  Access specified element with bounds checking.
	const_reference
	at (const size_type index) const
	{ return getValue () .at (index); }

	///  Access the first element.
	reference
	front ()
	{ return operator [ ] (0); }

	///  Access the first element.
	const_reference
	front () const
	{ return getValue () .front (); }

	///  Access the last element.
	reference
	back ()
	{ return operator [ ] (get () .size () - 1); }

	///  Access the last element.
	const_reference
	back () const
	{ return getValue () .back (); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	iterator
	begin ()
	{ return iterator (this, 0); }

	///  Returns an iterator to the beginning.
	const_iterator
	begin () const
	{ return getValue () .begin (); }

	///  Returns an iterator to the beginning.
	const_iterator
	cbegin () const
	{ return getValue () .cbegin (); }

	///  Returns an iterator to the end.
	iterator
	end ()
	{ return iterator (this, get () .size ()); }

	///  Returns an iterator to the end.
	const_iterator
	end () const
	{ return getValue () .end (); }

	///  Returns an iterator to the end.
	const_iterator
	cend () const
	{ return getValue () .cend (); }

	///  Returns a reverse iterator to the beginning.
	reverse_iterator
	rbegin ()
	{ return reverse_iterator (end ()); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	rbegin () const
	{ return getValue () .rbegin (); }

	///  Returns a reverse iterator to the beginning.
	const_reverse_iterator
	crbegin () const
	{ return getValue () .crbegin (); }

	///  Returns a reverse iterator to the end.
	reverse_iterator
	rend ()
	{ return reverse_iterator (begin ()); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	rend () const
	{ return getValue () .rend (); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	crend () const
	{ return getValue () .crend (); }

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
	insert (const iterator & location, const value_type & value)
	{ return emplace (location, value); }

	///  Inserts elements.
	iterator
	insert (const iterator & location, const size_type count, const value_type & value);

	///  Inserts elements.
	template <class InputIterator>
	iterator
	insert (const iterator &, InputIterator, const InputIterator &);

	template <class Arg>
	iterator
	emplace (const iterator & location, Arg && value);

	///  Appends X3DNativeArrayField @a field.
	X3DNativeArrayField &
	append (const X3DNativeArrayField & field);

	///  Erases elements.
	iterator
	erase (const iterator &);

	///  Erases elements.
	iterator
	erase (const iterator &, const iterator &);

	///  Removes elements completely.
	void
	remove (const value_type & value)
	{
		get () .erase (std::remove (get () .begin (), get () .end (), value), get () .end ());
		addEvent ();
	}

	///  Inserts elements to the beginning.
	void
	push_front (const value_type & value)
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
	push_back (const value_type & value)
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
	{ resize (count, value_type ()); }

	///  Changes the number of elements stored.
	void
	resize (const size_type count, const value_type & value);

	///  Swaps the contents.
	void
	swap (X3DNativeArrayField &);

	///  @name Interest service

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DNativeArrayField &), Class* const object) const
	{ addInterest (memberFunction, object, std::cref (*this)); }

	///  Adds an interest to this object.  The requester is then notified about a change of this object.
	template <class Class>
	void
	addInterest (void (Class::* memberFunction) (const X3DNativeArrayField &), Class & object) const
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

	///  Destructs the X3DNativeArrayField.
	virtual
	~X3DNativeArrayField ()
	{ reset (); }


protected:

	using X3DField <internal_type>::get;

	///  @name Element access

	///  Clears the contents without notifying its parents.
	virtual
	void
	reset () override;

private:

	///  @name Element access

	///  Access specified element with bounds checking and element creation.
	void
	set1 (const size_type index, const value_type & value)
	{
		get () [index] = value;
		addEvent ();
	}

	///  Access specified element with bounds checking and element creation.
	const_reference
	get1 (const size_type index) const
	{ return getValue () [index]; }

};

template <class ValueType>
template <class InputIterator>
X3DNativeArrayField <ValueType>::X3DNativeArrayField (const InputIterator & first, const InputIterator & last) :
	X3DField <internal_type> ()
{
	// Insert at end

	get () .assign (first, last);
}

template <class ValueType>
X3DNativeArrayField <ValueType> &
X3DNativeArrayField <ValueType>::operator = (X3DNativeArrayField && other)
{
	if (&other == this)
		return *this;

	get () = std::move (other .get ());

	other .addEvent ();
	addEvent ();

	return *this;
}

template <class ValueType>
bool
X3DNativeArrayField <ValueType>::equals (const X3DFieldDefinition & field) const
{
	if (getType () == field .getType ())
	{
		return size () == static_cast <const X3DNativeArrayField &> (field) .size () &&
		       std::equal (cbegin (), cend (),
		                   static_cast <const X3DNativeArrayField &> (field) .cbegin ());
	}

	return false;
}

template <class ValueType>
template <class InputIterator>
inline
void
X3DNativeArrayField <ValueType>::set (InputIterator first, const InputIterator & last)
{
	get () .assign (first, last);
}

template <class ValueType>
inline
void
X3DNativeArrayField <ValueType>::set1Value (const size_type index, const value_type & value)
{
	if (index >= size ())
		resize (index + 1);

	get () [index] = value;

	addEvent ();
}

template <class ValueType>
inline
typename X3DNativeArrayField <ValueType>::const_reference
X3DNativeArrayField <ValueType>::get1Value (const size_type index)
{
	if (index >= size ())
		resize (index + 1);

	return get () [index];
}

template <class ValueType>
inline
void
X3DNativeArrayField <ValueType>::clear ()
{
	reset ();

	addEvent ();
}

template <class ValueType>
template <class Arg>
typename X3DNativeArrayField <ValueType>::iterator
X3DNativeArrayField <ValueType>::emplace (const iterator & location, Arg && value)
{
	const auto iter = get () .emplace (get () .begin () + location .index (), std::forward <Arg> (value));

	addEvent ();

	return iterator (this, iter - get () .begin ());
}

template <class ValueType>
typename X3DNativeArrayField <ValueType>::iterator
X3DNativeArrayField <ValueType>::insert (const iterator & location, const size_type count, const value_type & value)
{
	const auto iter = get () .insert (get () .begin () + location .index (), count, value);

	addEvent ();

	return iterator (this, iter - get () .begin ());
}

template <class ValueType>
template <class InputIterator>
typename X3DNativeArrayField <ValueType>::iterator
X3DNativeArrayField <ValueType>::insert (const iterator & location, InputIterator first, const InputIterator & last)
{
	const auto iter = get () .insert (get () .begin () + location .index (), first, last);

	addEvent ();

	return iterator (this, iter - get () .begin ());
}

template <class ValueType>
inline
X3DNativeArrayField <ValueType> &
X3DNativeArrayField <ValueType>::append (const X3DNativeArrayField & field)
{
	insert (end (), field .begin (), field .end ());
	return *this;
}

template <class ValueType>
typename X3DNativeArrayField <ValueType>::iterator
X3DNativeArrayField <ValueType>::erase (const iterator & location)
{
	const auto iter = get () .erase (get () .begin () + location .index ());

	addEvent ();

	return iterator (this, iter - get () .begin ());
}

template <class ValueType>
typename X3DNativeArrayField <ValueType>::iterator
X3DNativeArrayField <ValueType>::erase (const iterator & first, const iterator & last)
{
	const auto iter = get () .erase (get () .begin () + first .index (), get () .begin () + last .index ());

	addEvent ();

	return iterator (this, iter - get () .begin ());
}

template <class ValueType>
template <class ... Args>
void
X3DNativeArrayField <ValueType>::emplace_front (Args && ... args)
{
	get () .emplace (get () .begin (), std::forward <Args> (args) ...);

	addEvent ();
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::pop_front ()
{
	get () .erase (get () .begin ());

	addEvent ();
}

template <class ValueType>
template <class ... Args>
void
X3DNativeArrayField <ValueType>::emplace_back (Args && ... args)
{
	get () .emplace_back (std::forward <Args> (args) ...);

	addEvent ();
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::pop_back ()
{
	get () .pop_back ();

	addEvent ();
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::resize (const size_type count, const value_type & value)
{
	if (count not_eq get () .size ())
	{
		get () .resize (count, value);

		addEvent ();
	}
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::swap (X3DNativeArrayField & other)
{
	if (&other == this)
		return;

	std::swap (get (), other .get ());

	addEvent ();
	other .addEvent ();
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::fromStream (std::istream & istream)
{
	if (istream)
	{
		std::string whiteSpaces;

		Grammar::WhiteSpaces (istream, whiteSpaces);

		if (Grammar::OpenBracket (istream))
		{
			X3DNativeArrayField array;
			ValueType           value;

			while (MiniParser::Decode (istream, value))
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

			if (MiniParser::Decode (istream, value))
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
X3DNativeArrayField <ValueType>::toStream (std::ostream & ostream) const
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

			VRMLGenerator::Encode (ostream, front (), getUnit ());

			Generator::PopUnitCategory (ostream);
			return;
		}
		default:
		{
			Generator::PushUnitCategory (ostream, getUnit ());

			ostream << Generator::OpenBracket;

			for (const auto & value : std::make_pair (cbegin (), cend () - 1))
			{
				VRMLGenerator::Encode (ostream, value, getUnit ());

				ostream
					<< Generator::Comma
					<< Generator::ListBreak;

				if (Generator::HasListBreak (ostream))
					ostream << Generator::Indent;
			}

			VRMLGenerator::Encode (ostream, back (), getUnit ());

			ostream << Generator::CloseBracket;

			Generator::PopUnitCategory (ostream);

			return;
		}
	}
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::toXMLStream (std::ostream & ostream) const
{
	if (empty ())
		return;

	Generator::PushUnitCategory (ostream, getUnit ());

	for (const auto & value : std::make_pair (cbegin (), cend () - 1))
	{
		XMLGenerator::Encode (ostream, value, getUnit ());

		ostream
			<< Generator::Comma
			<< Generator::TidySpace;
	}

	XMLGenerator::Encode (ostream, back (), getUnit ());

	Generator::PopUnitCategory (ostream);
}

template <class ValueType>
void
X3DNativeArrayField <ValueType>::toJSONStream (std::ostream & ostream) const
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

		for (const auto & value : std::make_pair (cbegin (), cend () - 1))
		{
			if (Generator::HasListBreak (ostream))
				ostream << Generator::Indent;

			JSONGenerator::Encode (ostream, value, getUnit ());

			ostream
				<< ','
				<< Generator::ListBreak;
		}

		if (Generator::HasListBreak (ostream))
			ostream << Generator::Indent;

		JSONGenerator::Encode (ostream, back (), getUnit ());

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
X3DNativeArrayField <ValueType>::reset ()
{
	get () .clear ();
}

///  @relates X3DNativeArrayField
///  @name Comparision operations

///  Compares two X3DNativeArrayField.
///  Returns true if @a lhs is equal to @a rhs.
template <class ValueType>
inline
bool
operator == (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return lhs .size () == rhs .size () &&
	       std::equal (lhs .cbegin (), lhs .cend (),
	                   rhs .cbegin ());
}

///  Compares two X3DNativeArrayField.
///  Returns true if @a lhs is not equal to @a rhs.
template <class ValueType>
inline
bool
operator not_eq (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return not (lhs == rhs);
}

///  Lexicographically compares two X3DNativeArrayField.
///  Returns true if @a lhs less than @a rhs.
template <class ValueType>
inline
bool
operator < (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return std::lexicographical_compare (lhs .cbegin (), lhs .cend (),
	                                     rhs .cbegin (), rhs .cend ());
}

///  Lexicographically compares two X3DNativeArrayField.
///  Returns true if @a lhs less than equal to @a rhs.
template <class ValueType>
inline
bool
operator > (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return rhs < lhs;
}

///  Lexicographically compares two X3DNativeArrayField.
///  Returns true if @a lhs greater than @a rhs.
template <class ValueType>
inline
bool
operator <= (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return not (rhs < lhs);
}

///  Lexicographically compares two X3DNativeArrayField.
///  Returns true if @a lhs greater than equal to @a rhs.
template <class ValueType>
inline
bool
operator >= (const X3DNativeArrayField <ValueType> & lhs, const X3DNativeArrayField <ValueType> & rhs)
{
	return not (lhs < rhs);
}

} // X3D
} // titania

#endif
