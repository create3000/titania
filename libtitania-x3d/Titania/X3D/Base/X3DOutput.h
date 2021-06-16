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

#ifndef __TITANIA_X3D_BASE_X3DOUTPUT_H__
#define __TITANIA_X3D_BASE_X3DOUTPUT_H__

#ifndef __APPLE__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
#endif

#include "../Base/X3DBase.h"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <vector>

namespace titania {
namespace X3D {

class X3DInput;
class X3DOutput;

typedef std::function <void (void)>                             Requester;
typedef std::list <Requester>                                   RequesterArray;
typedef RequesterArray::iterator                                RequesterId;
typedef std::tuple <const X3DOutput*, const void*, const void*> RequesterPair;
typedef std::map <RequesterPair, RequesterId>                   RequesterIndex;

class X3DOutput :
	virtual public X3DBase
{
public:

	///  @name Member access

	virtual
	bool
	isOutput () const
	{ return true; }

	const RequesterArray &
	getRequesters () const;

	///  @name Has interest service

	template <class Function, class Class>
	bool
	hasInterest (Function && memberFunction, Class* const object) const
	{
		union u { typename std::remove_reference <Function>::type f; void* p; };

		return checkInterest ((X3DInput*) object, u { memberFunction } .p);
	}

	template <class Function, class Class>
	bool
	hasInterest (Function && memberFunction, Class & object) const
	{ return hasInterest (memberFunction, &object); }

	///  @name Add interest service

	template <class Function, class Class, class ... Arguments>
	void
	addInterest (Function && memberFunction, Class* const object, Arguments && ... arguments) const
	{
		union u { typename std::remove_reference <Function>::type f; void* p; };

		bool inserted = insertInterest (std::bind (memberFunction, object, std::forward <Arguments> (arguments) ...),
		                                (X3DInput*) object, u { memberFunction } .p);

		if (inserted)
			insertInput (object, u { memberFunction } .p);
	}

	template <class Function, class Class, class ... Arguments>
	void
	addInterest (Function && memberFunction, Class & object, Arguments && ... arguments) const
	{ addInterest (memberFunction, &object, std::forward <Arguments> (arguments) ...); }

	template <class Class>
	void
	addInterest (Class* const object, void (Class::* memberFunction) (void)) const
	{
		union u { void (Class::* f) (void); void* p; };

		bool inserted = insertInterest (std::bind (memberFunction, object),
		                                (X3DInput*) object, u { memberFunction } .p);

		if (inserted)
			insertInput (object, u { memberFunction } .p);
	}

	template <class Function, class Class>
	void
	addInterest (void (Class::* memberFunction) (void), Class & object) const
	{ addInterest (memberFunction, &object); }

	void
	addInterest (const Requester &) const;

	///  @name Remove interest service

	template <class Function, class Class>
	void
	removeInterest (Function && memberFunction, Class* const object) const
	{
		union u { typename std::remove_reference <Function>::type f; void* p; };

		eraseInput (object, u { memberFunction } .p);
		eraseInterest ((X3DInput*) object, u { memberFunction } .p);
	}

	template <class Function, class Class>
	void
	removeInterest (Function && memberFunction, Class & object) const
	{ removeInterest (memberFunction, &object); }

	void
	removeInterest (const Requester &) const;

	///  @name Process interests service

	void
	processInterests () const;

	///  @name Virtual functions

	virtual
	bool
	checkInterest (const void* const, const void* const) const;

	virtual
	bool
	insertInterest (const Requester &, const void* const, const void* const) const;

	virtual
	void
	insertInput (const X3DInput* const, const void* const) const;

	virtual
	void
	eraseInterest (const void* const, const void* const) const;

	virtual
	void
	eraseInput (const X3DInput* const, void* const) const;

	///  @name Destruction

	virtual
	void
	dispose () override
	{ clear (); }

	///  Destructs this object.
	virtual
	~X3DOutput () override;


protected:

	///  @name Construction

	///  Constructs new X3DOutput.
	X3DOutput ();


private:

	///  @name Member types

	using InputSet = std::set <std::pair <const X3DInput*, const void*>>;

	///  @name Construction

	///  Realizes this object if needed.
	void
	realize () const;

	///  @name Operations

	void
	insertInput (const void* const, const void* const) const
	{ }

	void
	eraseInput (const void* const, void* const) const
	{ }

	void
	insertDeleter (const X3DOutput* const, const void* const, const void* const) const;

	void
	removeDeleter (const X3DOutput* const, const void* const, const void* const) const;

	void
	clear ();

	///  @name Members

	struct Data
	{
		RequesterArray requesters;
		RequesterIndex requesterIndex;
		InputSet inputs;
	};

	mutable std::unique_ptr <Data> data;

};

} // X3D
} // titania

#ifndef __APPLE__
#pragma GCC diagnostic pop
#endif

#endif
