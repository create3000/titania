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

#include "SFColorRGBA.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string SFColorRGBA::typeName = "SFColorRGBA";

const pb::Callbacks SFColorRGBA::callbacks = {
	pb::EnumerateCallback (),
	pb::HasPropertyCallback (),
	pb::PropertyGetter (),
	pb::PropertySetter (),
	pb::ResolveCallback (),
	dispose <SFColorRGBA>

};

pb::ptr <pb::NativeFunction>
SFColorRGBA::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 4);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Properties

	pb::NativeFunction* getter = nullptr;
	pb::NativeFunction* setter = nullptr;

	prototype -> addOwnProperty ("r",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "r", std::bind (get1Value, _1, _2, _3, R), 0),
	                             setter = new pb::NativeFunction (ec, "r", std::bind (set1Value, _1, _2, _3, R), 1));

	prototype -> addOwnProperty ("0", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("g",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "g", std::bind (get1Value, _1, _2, _3, G), 0),
	                             setter = new pb::NativeFunction (ec, "g", std::bind (set1Value, _1, _2, _3, G), 1));

	prototype -> addOwnProperty ("1", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("b",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "b", std::bind (get1Value, _1, _2, _3, B), 0),
	                             setter = new pb::NativeFunction (ec, "b", std::bind (set1Value, _1, _2, _3, B), 1));

	prototype -> addOwnProperty ("2", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("a",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "a", std::bind (get1Value, _1, _2, _3, A), 0),
	                             setter = new pb::NativeFunction (ec, "a", std::bind (set1Value, _1, _2, _3, A), 1));

	prototype -> addOwnProperty ("3", pb::undefined, pb::ENUMERABLE, getter, setter);

	// Functions

	prototype -> addOwnProperty ("setHSV", new pb::NativeFunction (ec, "setHSV", setHSV, 3), pb::NONE);
	prototype -> addOwnProperty ("getHSV", new pb::NativeFunction (ec, "getHSV", getHSV, 0), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

pb::var
SFColorRGBA::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFColorRGBA> (ec, object, new X3D::SFColorRGBA ());
			break;
		}
		case 4:
		{
			setUserData <SFColorRGBA> (ec, object, new X3D::SFColorRGBA (get1Argument <double> (args, 0),
			                                                             get1Argument <double> (args, 1),
			                                                             get1Argument <double> (args, 2),
			                                                             get1Argument <double> (args, 3)));
			break;
		}
		default:
			throw pb::Error (getTypeName () + ".constructor: wrong number of arguments.");
	}

	return pb::undefined;
}

pb::var
SFColorRGBA::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFColorRGBA> (ec, object);

		lhs -> set1Value (index, get1Argument <double> (args, 0));

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index) + "] is not generic.");
	}
}

pb::var
SFColorRGBA::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFColorRGBA> (ec, object);

		return lhs -> get1Value (index);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index) + "] is not generic.");
	}
}

pb::var
SFColorRGBA::setHSV (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 3)
		throw pb::Error (getTypeName () + ".prototype.setHSV: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFColorRGBA> (ec, object);
		const auto h   = get1Argument <double> (args, 0);
		const auto s   = get1Argument <double> (args, 1);
		const auto v   = get1Argument <double> (args, 2);

		lhs -> setHSV (h, s, v);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.setHSV is not generic.");
	}
}

pb::var
SFColorRGBA::getHSV (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.getHSV: wrong number of arguments.");

	try
	{
		const auto lhs   = getThis <SFColorRGBA> (ec, object);
		const auto array = new pb::Array (ec);

		float h, s, v;

		lhs -> getHSV (h, s, v);

		array -> put ("0", h);
		array -> put ("1", s);
		array -> put ("2", v);

		return array;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getHSV is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania
