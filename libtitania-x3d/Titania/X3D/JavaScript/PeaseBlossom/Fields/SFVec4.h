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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class InternalType>
class SFVec4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = InternalType;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ throw std::invalid_argument ("SFVec4::getType"); }

	static
	const std::string &
	getTypeName ()
	{ return typeName; }

	static
	const pb::Callbacks &
	getCallbacks ()
	{ return callbacks; }

	///  @name Construction

	static
	pb::ptr <pb::NativeFunction>
	initialize (Context* const, const pb::ptr <pb::Program> &);


private:

	///  @name Construction

	static
	pb::var
	construct (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Member access

	static
	pb::var
	set1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	static
	pb::var
	get1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	///  @name Functions

	static
	pb::var
	negate (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	add (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	subtract (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multiply (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	multVec (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	divide (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	divVec (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	dot (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	normalize (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	static
	pb::var
	length (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Member types

	enum {X, Y, Z, W};

	///  @name Static members

	static const std::string   typeName;
	static const pb::Callbacks callbacks;

};

template <class InternalType>
pb::ptr <pb::NativeFunction>
SFVec4 <InternalType>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
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

	prototype -> addOwnProperty ("w",
	                             pb::undefined,
	                             pb::NONE,
	                             getter = new pb::NativeFunction (ec, "w", std::bind (get1Value, _1, _2, _3, W), 0),
	                             setter = new pb::NativeFunction (ec, "w", std::bind (set1Value, _1, _2, _3, W), 1));

	prototype -> addOwnProperty ("3", pb::undefined, pb::ENUMERABLE, getter, setter);

	// Functions

	prototype -> addOwnProperty ("negate",    new pb::NativeFunction (ec, "negate",    negate,    0), pb::NONE);
	prototype -> addOwnProperty ("add",       new pb::NativeFunction (ec, "add",       add,       1), pb::NONE);
	prototype -> addOwnProperty ("subtract",  new pb::NativeFunction (ec, "subtract",  subtract,  1), pb::NONE);
	prototype -> addOwnProperty ("multiply",  new pb::NativeFunction (ec, "multiply",  multiply,  1), pb::NONE);
	prototype -> addOwnProperty ("multVec",   new pb::NativeFunction (ec, "multVec",   multVec,   1), pb::NONE);
	prototype -> addOwnProperty ("divide",    new pb::NativeFunction (ec, "divide",    divide,    1), pb::NONE);
	prototype -> addOwnProperty ("divVec",    new pb::NativeFunction (ec, "divVec",    divVec,    1), pb::NONE);
	prototype -> addOwnProperty ("dot",       new pb::NativeFunction (ec, "dot",       dot,       1), pb::NONE);
	prototype -> addOwnProperty ("normalize", new pb::NativeFunction (ec, "normalize", normalize, 0), pb::NONE);
	prototype -> addOwnProperty ("length",    new pb::NativeFunction (ec, "length",    length,    0), pb::NONE);

	function -> addOwnProperty ("prototype", prototype, pb::NONE);
	return function;
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFVec4> (ec, object, new InternalType ());
			break;
		}
		case 4:
		{
			setUserData <SFVec4> (ec, object, new InternalType (get1Argument <double> (args, 0),
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

template <class InternalType>
pb::var
SFVec4 <InternalType>::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);

		lhs -> set1Value (index, get1Argument <double> (args, 0));

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);

		return lhs -> get1Value (index);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::negate (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.negate: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);

		return create <SFVec4> (ec, new InternalType (lhs -> negate ()));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.negate is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::add (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.add: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> add (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.add is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::subtract (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.subtract: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> subtract (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.subtract is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::multiply (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multiply: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <double> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> multiply (rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multiply is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::multVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multVec: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> multVec (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multVec is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::divide (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.divide: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <double> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> divide (rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.divide is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::divVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.divVec: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return create <SFVec4> (ec, new InternalType (lhs -> divVec (*rhs)));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.divVec is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::dot (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.dot: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return lhs -> dot (*rhs);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.dot is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::normalize (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.normalize: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);

		return create <SFVec4> (ec, new InternalType (lhs -> normalize ()));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.normalize is not generic.");
	}
}

template <class InternalType>
pb::var
SFVec4 <InternalType>::length (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.length: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (ec, object);

		return lhs -> length ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.length is not generic.");
	}
}

using SFVec4d = SFVec4 <X3D::SFVec4d>;
using SFVec4f = SFVec4 <X3D::SFVec4f>;

template <>
constexpr ObjectType
SFVec4d::getType ()
{
	return ObjectType::SFVec4d;
}

template <>
constexpr ObjectType
SFVec4f::getType ()
{
	return ObjectType::SFVec4f;
}

} // peaseblossom
} // X3D
} // titania

#endif
