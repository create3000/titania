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

#include <Titania/X3D.h>

#include <btBulletDynamicsCommon.h>
//#include <filesystem>

using namespace titania;
using namespace titania::X3D;

template <class Key, class T>
using Map = std::map <Key, T>;

template <class KeyType, class MappedType>
class X3DMapField :
	public X3DField <Map <KeyType, MappedType*>> 
{
public:

	using internal_type          = Map <KeyType, MappedType*>;
	using key_type               = typename internal_type::key_type;
	using mapped_type            = typename internal_type::mapped_type;
	using value_type             = typename internal_type::value_type;
	using iterator               = typename internal_type::iterator;
	using const_iterator         = typename internal_type::const_iterator;
	using reverse_iterator       = typename internal_type::reverse_iterator;
	using const_reverse_iterator = typename internal_type::const_reverse_iterator;

	using X3DField <internal_type>::addEvent;
	using X3DField <internal_type>::getValue;

	///  @name Construction

	///  Construct new X3DMapField.
	X3DMapField () :
		X3DField <internal_type> ()
	{ }

	///  Construct new X3DMapField.
	X3DMapField (const X3DMapField & other) :
		X3DField <internal_type> ()
	{
		for (const auto & [key, child] : other)
			insert (key, *child);
	}

	///  Construct new X3DMapField.
	virtual
	X3DMapField*
	create () const final override
	{ return new X3DMapField (); }

	///  Construct new X3DMapField.
	virtual
	X3DMapField*
	copy (const CopyType) const final override
	{ return new X3DMapField (*this); }

	///  Construct new X3DMapField.
	virtual
	X3DMapField*
	copy (X3DExecutionContext* const, const CopyType type) const final override
	{ return copy (type); }

	///  @name Iterators

	///  Returns an iterator to the beginning.
	iterator
	begin ()
	{ return get () .begin (); }

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
	{ return get () .end (); }

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
	{ return get () .rbegin (); }

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
	{ return get () .rend (); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	rend () const
	{ return getValue () .rend (); }

	///  Returns a reverse iterator to the end.
	const_reverse_iterator
	crend () const
	{ return getValue () .crend (); }

	///  @name Capacity

	bool
	empty () const
	{ return getValue () .empty (); }

	size_type
	size () const
	{ return getValue () .size (); }

	size_type
	max_size () const
	{ return getValue () .max_size (); }

	///  @name Modifiers

	void
	clear ()
	{
		for (const auto & [key, child] : get ())
			child -> removeParent (this);

		get () .clear ();

		addEvent ();
	}

	std::pair <iterator, bool>
	insert (const KeyType & key, const MappedType & value)
	{
		const auto pair = get () .emplace (key, nullptr);

		if (pair .second)
		{
			const auto child = new MappedType (value);

			child -> addParent (this);

			pair .first -> second = child;
		}

		addEvent ();

		return pair;	
	}

	size_type
	erase (const KeyType & key)
	{
		const auto iter = get () .find (key);

		if (iter == get () .end ())
			return 0;

		iter -> second -> removeParent (this);

		get () .erase (iter);

		addEvent ();

		return 1;
	}

	///  @name Lookup

	size_type
	count (const KeyType & key) const
	{ return getValue () .count (key); }

	iterator
	find (const KeyType & key)
	{ return get () .find (key); }
	
	const_iterator
	find (const KeyType & key) const
	{ return getValue () .find (key); }

	///  @name Input/Output

	///  Extracts the value for this object from @a istream. The contents of @a istream must be in VRML Classic Encoding.
	virtual
	void
	fromStream (std::istream &) final override
	{ }

	///  Inserts this object into @a ostream in VRML Classic Encoding style.
	virtual
	void
	toStream (std::ostream &) const final override
	{ }

	///  Inserts this object into @a ostream in X3D XML Encoding style.
	virtual
	void
	toXMLStream (std::ostream &) const final override
	{ }

	///  Inserts this object into @a ostream in X3D JSON Encoding style.
	virtual
	void
	toJSONStream (std::ostream &) const final override
	{ }

	///  @name Destruction

	~X3DMapField ()
	{ reset (); }


protected:

	using X3DField <internal_type>::get;

	///  @name Destruction

	virtual
	void
	reset () final override
	{ clear (); }

};

int
main (int argc, char** argv)
{
	std::clog << "Starting main ..." << std::endl;
	std::cout << std::boolalpha;
	std::clog << std::boolalpha;
	std::cerr << std::boolalpha;
	std::cout << std::setprecision (std::numeric_limits <float>::digits10);
	std::clog << std::setprecision (std::numeric_limits <float>::digits10);
	std::cerr << std::setprecision (std::numeric_limits <float>::digits10);
	//std::cout << std::setprecision (std::numeric_limits <double>::digits10);
	//std::clog << std::setprecision (std::numeric_limits <double>::digits10);
	//std::cerr << std::setprecision (std::numeric_limits <double>::digits10);
	std::cout .imbue (std::locale (""));
	std::clog .imbue (std::locale (""));
	std::cerr .imbue (std::locale (""));

	std::locale::global (std::locale (""));

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	using RouteIndex = X3DMapField <RouteId, RoutePtr>;

	RouteIndex routes;

	routes .insert (RouteId (), RoutePtr ());
	routes .erase (RouteId ());

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::clog << "Function main done." << std::endl;
	return 0;
}
