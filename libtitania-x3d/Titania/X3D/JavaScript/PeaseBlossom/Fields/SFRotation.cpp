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

#include "SFRotation.h"

#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

const std::string SFRotation::typeName = "SFRotation";

const pb::Callbacks SFRotation::callbacks = {
	pb::EnumerateCallback (),
	pb::HasPropertyCallback (),
	pb::PropertyGetter (),
	pb::PropertySetter (),
	pb::ResolveCallback (),
	dispose <SFRotation>
};

pb::ptr <pb::NativeFunction>
SFRotation::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 4);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addOwnProperty ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	// Properties

	pb::NativeFunction* getter = nullptr;
	pb::NativeFunction* setter = nullptr;

	prototype -> addOwnProperty ("x",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "x", std::bind (get1Value, _1, _2, _3, X), 0),
	                             setter = new pb::NativeFunction (ec, "x", std::bind (set1Value, _1, _2, _3, X), 1));

	prototype -> addOwnProperty ("0", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("y",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "y", std::bind (get1Value, _1, _2, _3, Y), 0),
	                             setter = new pb::NativeFunction (ec, "y", std::bind (set1Value, _1, _2, _3, Y), 1));

	prototype -> addOwnProperty ("1", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("z",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "z", std::bind (get1Value, _1, _2, _3, Z), 0),
	                             setter = new pb::NativeFunction (ec, "z", std::bind (set1Value, _1, _2, _3, Z), 1));

	prototype -> addOwnProperty ("2", pb::undefined, pb::ENUMERABLE, getter, setter);

	prototype -> addOwnProperty ("angle",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "angle", std::bind (get1Value, _1, _2, _3, ANGLE), 0),
	                             setter = new pb::NativeFunction (ec, "angle", std::bind (set1Value, _1, _2, _3, ANGLE), 1));

	prototype -> addOwnProperty ("3", pb::undefined, pb::ENUMERABLE, getter, setter);

	// Functions

	prototype -> addOwnProperty ("setAxis",  new pb::NativeFunction (ec, "setAxis",  setAxis,  1), pb::NONE);
	prototype -> addOwnProperty ("getAxis",  new pb::NativeFunction (ec, "getAxis",  getAxis,  0), pb::NONE);
	prototype -> addOwnProperty ("inverse",  new pb::NativeFunction (ec, "inverse",  inverse,  0), pb::NONE);
	prototype -> addOwnProperty ("multiply", new pb::NativeFunction (ec, "multiply", multiply, 1), pb::NONE);
	prototype -> addOwnProperty ("multVec",  new pb::NativeFunction (ec, "multVec",  multVec,  1), pb::NONE);
	prototype -> addOwnProperty ("slerp",    new pb::NativeFunction (ec, "slerp",    slerp,    2), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

pb::var
SFRotation::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFRotation> (ec, object, new X3D::SFRotation ());
			break;
		}
		case 2:
		{
			const auto arg1 = get1Argument <SFVec3f> (args, 0);

			try
			{
				const auto arg2 = get1Argument <SFVec3f> (args, 1);

				setUserData <SFRotation> (ec, object, new X3D::SFRotation (*arg1, *arg2));
				break;
			}
			catch (const pb::TypeError &)
			{
				const auto arg2 = get1Argument <double> (args, 1);

				setUserData <SFRotation> (ec, object, new X3D::SFRotation (*arg1, arg2));
				break;
			}
		}
		case 4:
		{
			setUserData <SFRotation> (ec, object, new X3D::SFRotation (get1Argument <double> (args, 0),
			                                                           get1Argument <double> (args, 1),
			                                                           get1Argument <double> (args, 2),
			                                                           get1Argument <double> (args, 3)));
			break;
		}
		default:
			throw pb::Error ("wrong number of arguments.");
	}

	return pb::undefined;
}

pb::var
SFRotation::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFRotation> (object);

		lhs -> set1Value (index, get1Argument <double> (args, 0));

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index) + "] is not generic.");
	}
}

pb::var
SFRotation::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFRotation> (object);

		return lhs -> get1Value (index);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index) + "] is not generic.");
	}
}

pb::var
SFRotation::setAxis (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);
		const auto rhs = get1Argument <SFVec3f> (args, 0);

		lhs -> setAxis (*rhs);

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.setAxis is not generic.");
	}
}

pb::var
SFRotation::getAxis (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);

		return create <SFVec3f> (ec, lhs -> getAxis ());
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.getAxis is not generic.");
	}
}

pb::var
SFRotation::inverse (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);

		return create <SFRotation> (ec, lhs -> inverse ());
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.inverse is not generic.");
	}
}

pb::var
SFRotation::multiply (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);
		const auto rhs = get1Argument <SFRotation> (args, 0);

		return create <SFRotation> (ec, lhs -> multiply (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multiply is not generic.");
	}
}

pb::var
SFRotation::multVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);
		const auto rhs = get1Argument <SFVec3f> (args, 0);

		return create <SFVec3f> (ec, lhs -> multVec (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multVec is not generic.");
	}
}

pb::var
SFRotation::slerp (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 2)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFRotation> (object);
		const auto rhs = get1Argument <SFRotation> (args, 0);
		const auto t   = get1Argument <double> (args, 1);

		return create <SFRotation> (ec, lhs -> slerp (*rhs, t));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.slerp is not generic.");
	}
}

} // peaseblossom
} // X3D
} // titania
