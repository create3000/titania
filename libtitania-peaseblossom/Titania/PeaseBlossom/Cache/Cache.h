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

#ifndef __TITANIA_PEASE_BLOSSOM_CACHE_CACHE_H__
#define __TITANIA_PEASE_BLOSSOM_CACHE_CACHE_H__

#include "../Base/GarbageCollector.h"

#include <atomic>
#include <mutex>
#include <vector>

namespace titania {
namespace pb {

class pbCache
{
public:

	///  @name Capacity

	static
	bool
	empty ()
	{ return size_ == 0; }

	static
	size_t
	size ()
	{ return size_; }


protected:

	static
	void
	increment ()
	{ ++ size_; }

	static
	void
	decrement ()
	{ -- size_; }

	static
	void
	decrement (const size_t value)
	{ size_ -= value; }


private:

	///  @name Static members

	static std::atomic <size_t> size_;

};

template <class Type>
class Cache :
	public pbCache
{
public:

	using array_type = std::vector <Type*>;

	static
	void
	add (Type* const object);

	template <class ... Args>
	static
	Type*
	get (Args && ... args);

	static
	void
	clear (GarbageCollector::ObjectArray & objects);


private:

	template <class ... Args>
	static
	void
	prepare (Type* const object, Args && ... args)
	{
		object -> operator = (std::forward <Args> (args) ...);
	}

	static array_type cache;
	static size_t     unused;
	static std::mutex mutex;

};

template <class Type>
std::vector <Type*> Cache <Type>::cache;

template <class Type>
size_t Cache <Type>::unused (0);

template <class Type>
std::mutex Cache <Type>::mutex;

template <class Type>
inline
void
Cache <Type>::add (Type* const object)
{
	std::lock_guard <std::mutex> lock (mutex);

	increment ();

	cache .emplace_back (object);
}

template <class Type>
template <class ... Args>
Type*
Cache <Type>::get (Args && ... args)
{
	std::lock_guard <std::mutex> lock (mutex);

	if (cache .empty ())
		return new Type (std::forward <Args> (args) ...);

	decrement ();

	const auto object = cache .back ();

	cache .pop_back ();

	unused = std::min (unused, cache .size ());

	prepare (object, std::forward <Args> (args) ...);

	return object;
}

template <class Type>
void
Cache <Type>::clear (GarbageCollector::ObjectArray & objects)
{
	std::lock_guard <std::mutex> lock (mutex);

	decrement (unused);

	const auto size = cache .size () - unused;

	//__LOG__ << cache .size () << " : " << size << " : " << unused << std::endl;

	objects .insert (objects .end (), cache .begin () + size, cache .end ());

	cache .resize (size);

	unused = size;
}

} // pb
} // titania

#endif
