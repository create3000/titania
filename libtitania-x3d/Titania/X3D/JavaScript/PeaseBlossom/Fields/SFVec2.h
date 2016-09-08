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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC2_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC2_H__

#include "../../../Fields/SFVec2.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class Type>
class SFVec2 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = Type;

	///  @name Common members

	static
	constexpr ObjectType
	getType ()
	{ throw std::invalid_argument ("SFVec2::getType"); }

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

	enum {X, Y};

	///  @name Static members

	static const std::string   typeName;
	static const pb::Callbacks callbacks;

};

template <class Type>
pb::ptr <pb::NativeFunction>
SFVec2 <Type>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 2);
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

template <class Type>
pb::var
SFVec2 <Type>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFVec2> (ec, object, new Type ());
			break;
		}
		case 2:
		{
			setUserData <SFVec2> (ec, object, new Type (get1Argument <double> (args, 0),
			                                            get1Argument <double> (args, 1)));
			break;
		}
		default:
			throw pb::Error (getTypeName () + ".constructor: wrong number of arguments.");
	}

	return pb::undefined;
}

template <class Type>
pb::var
SFVec2 <Type>::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);

		lhs -> set1Value (index, get1Argument <double> (args, 0));

		return pb::undefined;
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);

		return lhs -> get1Value (index);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype[" + basic::to_string (index, std::locale::classic ()) + "] is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::negate (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.negate: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);

		return create <SFVec2> (ec, lhs -> negate ());
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.negate is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::add (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.add: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <SFVec2> (args, 0);

		return create <SFVec2> (ec, lhs -> add (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.add is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::subtract (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.subtract: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <SFVec2> (args, 0);

		return create <SFVec2> (ec, lhs -> subtract (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.subtract is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::multiply (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multiply: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <double> (args, 0);

		return create <SFVec2> (ec, lhs -> multiply (rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multiply is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::multVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.multVec: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <SFVec2> (args, 0);

		return create <SFVec2> (ec, lhs -> multiply (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.multVec is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::divide (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.divide: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <double> (args, 0);

		return create <SFVec2> (ec, lhs -> divide (rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.divide is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::divVec (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.divVec: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <SFVec2> (args, 0);

		return create <SFVec2> (ec, lhs -> divide (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.divVec is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::dot (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error (getTypeName () + ".prototype.dot: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);
		const auto rhs = get1Argument <SFVec2> (args, 0);

		return lhs -> dot (*rhs);
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.dot is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::normalize (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.normalize: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);

		return create <SFVec2> (ec, lhs -> normalize ());
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.normalize is not generic.");
	}
}

template <class Type>
pb::var
SFVec2 <Type>::length (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 0)
		throw pb::Error (getTypeName () + ".prototype.length: wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec2> (ec, object);

		return lhs -> length ();
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.length is not generic.");
	}
}

using SFVec2d = SFVec2 <X3D::SFVec2d>;
using SFVec2f = SFVec2 <X3D::SFVec2f>;

template <>
constexpr ObjectType
SFVec2d::getType ()
{
	return ObjectType::SFVec2d;
}

template <>
constexpr ObjectType
SFVec2f::getType ()
{
	return ObjectType::SFVec2f;
}

extern template class SFVec2 <X3D::SFVec2d>;
extern template class SFVec2 <X3D::SFVec2f>;

} // peaseblossom
} // X3D
} // titania

#endif
