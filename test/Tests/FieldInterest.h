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

#include <Titania/Time.h>
#include <Titania/X3D/Browser/Browser.h>

namespace Test {
namespace FieldInterest {

using namespace titania;
using namespace titania::X3D;

typedef std::function <void (void)> Interest;

class X3DObject
{
private:

	typedef std::list <Interest> InterestArray;


public:

	typedef typename InterestArray::iterator InterestId;

	X3DObject () = default;

	//throw (Error <INVALID_OPERATION_TIMING>,
	//       Error <INVALID_ACCESS_TYPE>,
	//       Error <INSUFFICIENT_CAPABILITIES>,
	//       Error <NODE_IN_USE>,
	//       Error <DISPOSED>)

	// add interest

	template <class Requester>
	inline
	const InterestId
	addInterest (void (Requester::* memberFunction) (void), Requester* requester)
	{
		return addInterest (memberFunction, *requester);
	}

	template <class Requester>
	inline
	const InterestId
	addInterest (void (Requester::* memberFunction) (void), Requester & requester)
	{
		return addInterest (std::bind (std::mem_fn (memberFunction), requester));
	}

	const InterestId
	addInterest (const Interest & callback)
	{
		return interests .insert (interests .end (), callback);
	}

	// remove interest

	void
	removeInterest (const InterestId & id)
	{
		interests .erase (id);
	}

	// process event

	void
	processEvent ()
	{
		std::for_each (interests .cbegin (),
		               interests .cend (),
		               std::mem_fn (&interest::operator ()));
	}

	virtual
	~X3DObject () { }


private:

	InterestArray interests;

};

template <typename Type>
class X3DField :
	public X3DObject
{
public:

	using X3DObject::addInterest;

	X3DField (const Type & value) :
		value (value)
	{ }

	// add field interest

	template <class Requester>
	inline
	const InterestId
	addInterest (void (Requester::* memberFunction) (const X3DField <Type>&), Requester* requester)
	{
		return addInterest (memberFunction, *requester);
	}

	template <class Requester>
	inline
	const InterestId
	addInterest (void (Requester::* memberFunction) (const X3DField <Type>&), Requester & requester)
	{
		return addInterest (std::bind (std::mem_fn (memberFunction), requester, std::cref (*this)));
	}

	inline
	const InterestId
	addInterest (void (* function) (const X3DField <Type> &))
	{
		return addInterest (std::bind (function, std::cref (*this)));
	}

	Type value;

};

template <typename Type>
std::ostream &
operator << (std::ostream & osteam, const X3DField <Type> & field)
{
	return osteam << field .value;
}

class TestCallback
{
public:

	void
	callback ()
	{
		std::clog << "TestCallback::callback: " << Time::now <double> () << std::endl;
	}

	void
	set_SFInt32 (const X3DField <double> & value)
	{
		std::clog << "TestCallback::set_SFInt32: " << value << std::endl;
	}

};

void
callback ()
{
	std::clog << "callback: " << Time::now <double> () << std::endl;
}

void
set_SFInt32 (const X3DField <double> & value)
{
	std::clog << "set_SFInt32: " << value << std::endl;
}

void
TestFieldInterest ()
{
	// init

	auto field      = X3DField <double> (666);
	auto requester1 = TestCallback ();
	auto requester2 = new TestCallback ();

	// start

	//std::clog << "Start" << std::endl;

	field .processEvent ();

	//std::clog << std::endl;

	// add interest

	auto interestId1 = field .addInterest (&callback);
	auto interestId2 = field .addInterest (&TestCallback::callback, requester1);
	auto interestId3 = field .addInterest (&TestCallback::callback, requester2);

	// add field interest

	auto interestId4 = field .addInterest (&set_SFInt32);
	auto interestId5 = field .addInterest (&TestCallback::set_SFInt32, requester1);
	auto interestId6 = field .addInterest (&TestCallback::set_SFInt32, requester2);

	//std::clog << field << std::endl;
	field .processEvent ();

	// 42

	field .value = 42;

	//std::clog << field << std::endl;
	field .processEvent ();

	// 23

	field .value = 23;

	//std::clog << field << std::endl;
	field .processEvent ();

	// remove interest

	field .removeInterest (interestId1);
	field .removeInterest (interestId2);
	field .removeInterest (interestId3);
	field .removeInterest (interestId4);
	field .removeInterest (interestId5);
	field .removeInterest (interestId6);

	// end

	//std::clog << "End" << std::endl;

	field .processEvent ();

	//std::clog << std::endl;

	//addInterest (std::mem_fn (&TestCallback::set_scene));

	//		int iterations = 1000000000;
	//
	//		titania::Test::benchmark ("callback1", callback1, iterations);
	//		titania::Test::benchmark ("callback2", callback2, iterations);
	//		titania::Test::benchmark ("callback3", callback3, iterations);
	//
	//		titania::Test::benchmark ("callback4", callback4, iterations);
	//		titania::Test::benchmark ("callback5", callback5, iterations);
	//		titania::Test::benchmark ("callback6", callback6, iterations);
	//
	//		typedef sigc::slot <void> FieldCallback;

	//FieldCallback slot2 = sigc::mem_fun (requester1, &TestCallback::callback);
	//titania::Test::benchmark ("slot2", slot2, iterations);

	delete requester2;
}

} // FieldInterest
} // Test
