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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_PEASE_BLOSSOM_FIELDS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace peaseblossom {

template <class Type>
class SFVec4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = Type;

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
	get1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	static
	pb::var
	set1Value (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &, const size_t);

	///  @name Functions

	static
	pb::var
	add (const pb::ptr <pb::pbExecutionContext> &, const pb::var &, const std::vector <pb::var> &);

	///  @name Member types

	enum {X, Y, Z, W};

	///  @name Static members

	static const std::string   typeName;
	static const pb::Callbacks callbacks;

};

template <class Type>
pb::ptr <pb::NativeFunction>
SFVec4 <Type>::initialize (Context* const context, const pb::ptr <pb::Program> & ec)
{
	using namespace std::placeholders;

	const auto function  = pb::make_ptr <pb::NativeFunction> (ec, getTypeName (), construct, nullptr, 4);
	const auto prototype = context -> getClass (ObjectType::X3DField) -> createInstance (ec);

	prototype -> addPropertyDescriptor ("constructor", function, pb::WRITABLE | pb::CONFIGURABLE);

	prototype -> addPropertyDescriptor ("x",
	                                    pb::Undefined,
	                                    pb::DEFAULT,
	                                    new pb::NativeFunction (ec, "x", std::bind (get1Value, _1, _2, _3, X), 0),
	                                    new pb::NativeFunction (ec, "x", std::bind (set1Value, _1, _2, _3, X), 1));

	prototype -> addPropertyDescriptor ("y",
	                                    pb::Undefined,
	                                    pb::DEFAULT,
	                                    new pb::NativeFunction (ec, "y", std::bind (get1Value, _1, _2, _3, Y), 0),
	                                    new pb::NativeFunction (ec, "y", std::bind (set1Value, _1, _2, _3, Y), 1));

	prototype -> addPropertyDescriptor ("z",
	                                    pb::Undefined,
	                                    pb::DEFAULT,
	                                    new pb::NativeFunction (ec, "z", std::bind (get1Value, _1, _2, _3, Z), 0),
	                                    new pb::NativeFunction (ec, "z", std::bind (set1Value, _1, _2, _3, Z), 1));

	prototype -> addPropertyDescriptor ("w",
	                                    pb::Undefined,
	                                    pb::DEFAULT,
	                                    new pb::NativeFunction (ec, "w", std::bind (get1Value, _1, _2, _3, W), 0),
	                                    new pb::NativeFunction (ec, "w", std::bind (set1Value, _1, _2, _3, W), 1));

	prototype -> addPropertyDescriptor ("add", new pb::NativeFunction (ec, "add", add, 1), pb::NONE);

	function -> addPropertyDescriptor ("prototype", prototype, pb::NONE);
	return function;
}

template <class Type>
pb::var
SFVec4 <Type>::construct (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	switch (args .size ())
	{
		case 0:
		{
			setUserData <SFVec4> (ec, object, new Type ());
			break;
		}
		case 4:
		{
			setUserData <SFVec4> (ec, object, new Type (args [0] .toNumber (),
			                                            args [1] .toNumber (),
			                                            args [2] .toNumber (),
			                                            args [3] .toNumber ()));
			break;
		}
		default:
			throw pb::Error ("wrong number of arguments.");
	}

	return pb::Undefined;
}

template <class Type>
pb::var
SFVec4 <Type>::get1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	const auto lhs = getObject <Type> (object);

	return lhs -> get1Value (index);
}

template <class Type>
pb::var
SFVec4 <Type>::set1Value (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args, const size_t index)
{
	const auto lhs = getObject <Type> (object);

	lhs -> set1Value (index, args [0] .toNumber ());

	return pb::Undefined;
}

template <class Type>
pb::var
SFVec4 <Type>::add (const pb::ptr <pb::pbExecutionContext> & ec, const pb::var & object, const std::vector <pb::var> & args)
{
	if (args .size () not_eq 1)
		throw pb::Error ("wrong number of arguments.");

	try
	{
		const auto lhs = getThis <SFVec4> (object);
		const auto rhs = get1Argument <SFVec4> (args, 0);

		return create <SFVec4> (ec, lhs -> add (*rhs));
	}
	catch (const std::invalid_argument &)
	{
		throw pb::TypeError (getTypeName () + ".prototype.add is not generic.");
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
