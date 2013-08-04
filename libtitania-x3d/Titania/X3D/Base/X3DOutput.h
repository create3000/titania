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

#ifndef __TITANIA_X3D_BASE_X3DOUTPUT_H__
#define __TITANIA_X3D_BASE_X3DOUTPUT_H__

#include "../Base/X3DBase.h"
#include <functional>
#include <list>
#include <map>
#include <set>
#include <vector>

namespace titania {
namespace X3D {

class X3DInput;

typedef std::function <void (void)>           Requester;
typedef std::list <Requester>                 RequesterArray;
typedef RequesterArray::iterator              RequesterId;
typedef std::pair <const void*, const void*>  RequesterPair;
typedef std::map <RequesterPair, RequesterId> RequesterIndex;

class X3DOutput :
	virtual public X3DBase
{
public:

	virtual
	bool
	isOutput () const
	{ return true; }

	const RequesterArray &
	getRequesters () const;

	/// @name Has interest service

	template <class Class, class Function>
	inline
	bool
	hasInterest (Class* object, Function && memberFunction) const
	{
		return hasInterest ((X3DInput*) object, reinterpret_cast <void*> (object ->* memberFunction));
	}

	template <class Class, class Function>
	inline
	bool
	hasInterest (Class & object, Function && memberFunction) const
	{
		return hasInterest ((X3DInput*) &object, reinterpret_cast <void*> (object .* memberFunction));
	}

	/// @name Add interest service

	template <class Class, class Function, class ... Arguments>
	inline
	void
	addInterest (Class* object, Function && memberFunction, Arguments && ... arguments) const
	{
		insertInput (object, reinterpret_cast <void*> (object ->* memberFunction));
		insertInterest (std::bind (std::mem_fn (memberFunction), object, std::forward <Arguments> (arguments) ...),
		                (X3DInput*) object, reinterpret_cast <void*> (object ->* memberFunction));
	}

	template <class Class, class Function, class ... Arguments>
	inline
	void
	addInterest (Class & object, Function && memberFunction, Arguments && ... arguments) const
	{
		insertInput (&object, reinterpret_cast <void*> (object .* memberFunction));
		insertInterest (std::bind (std::mem_fn (memberFunction), object, std::forward <Arguments> (arguments) ...),
		                (X3DInput*) &object, reinterpret_cast <void*> (object .* memberFunction));
	}

	template <class Class>
	void
	addInterest (Class* object, void (Class::* memberFunction) (void)) const
	{
		insertInput (object, reinterpret_cast <void*> (object ->* memberFunction));
		insertInterest (std::bind (std::mem_fn (memberFunction), object),
		                (X3DInput*) object, reinterpret_cast <void*> (object ->* memberFunction));
	}

	template <class Class>
	void
	addInterest (Class & object, void (Class::* memberFunction) (void)) const
	{
		insertInput (&object, reinterpret_cast <void*> (object ->* memberFunction));
		insertInterest (std::bind (std::mem_fn (memberFunction), object),
		                (X3DInput*) &object, reinterpret_cast <void*> (object .* memberFunction));
	}

	void
	addInterest (const Requester &) const;

	///  @name Remove interest service

	template <class Class, class Function>
	inline
	void
	removeInterest (Class* object, Function && memberFunction) const
	{
		eraseInput (object);
		eraseInterest ((X3DInput*) object, reinterpret_cast <void*> (object ->* memberFunction));
	}

	template <class Class, class Function>
	inline
	void
	removeInterest (Class & object, Function && memberFunction) const
	{
		eraseInput (&object);
		eraseInterest ((X3DInput*) &object, reinterpret_cast <void*> (object .* memberFunction));
	}

	void
	removeInterest (const Requester &) const;

	///  @name Process interests service

	void
	processInterests () const;

	///  @name Dispose service

	virtual
	void
	dispose ();

	virtual
	~X3DOutput ();


protected:

	X3DOutput ();


private:

	typedef std::set <const X3DInput*> InputSet;

	bool
	hasInterest (const void*, const void*) const;

	///  Add basic interest.
	void
	insertInterest (const Requester &, const void*, const void*) const;

	void
	insertInput (const X3DInput*, const void*) const;

	void
	insertInput (const void*, const void*) const
	{ }

	void
	eraseInterest (const void*, const void*) const;

	void
	eraseInput (const X3DInput*) const;

	void
	eraseInput (const void*) const
	{ }

	mutable RequesterArray          requesters;
	mutable RequesterIndex          requesterIndex;
	mutable InputSet                inputs;
	mutable std::vector <Requester> copy;

};

} // X3D
} // titania

#endif
