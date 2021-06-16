/* -*- Mode: C++; coding: utf-8; tab-width: 3; indent-tabs-mode: tab; c-basic-offset: 3 -*-
 *******************************************************************************
 *
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * Copyright create3000, Scheffelstra�e 31a, Leipzig, Germany 2011.
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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_SFVEC2_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_SFVEC2_H__

#include "../X3DField.h"

#include "../../../Fields/SFVec2.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class InternalType>
class SFVec2 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = InternalType;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent);

	static
	JS::Value
	create (JSContext* const cx, InternalType* const field);

	static
	const JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr
	ObjectType
	getId ()
	{ throw std::runtime_error ("getId"); return ObjectType (); }


private:

	///  @name Member types

	enum Property {X, Y};

	///  @name Construction

	static bool construct (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Member access

	template <size_t Index>
	static bool setProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	template <size_t Index>
	static bool getProperty (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Functions

	static bool add       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool cross     (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool distance  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool divide    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool divVec    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool dot       (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool length    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool lerp      (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multiply  (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool multVec   (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool negate    (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool normalize (JSContext* cx, unsigned argc, JS::Value* vp);
	static bool subtract  (JSContext* cx, unsigned argc, JS::Value* vp);

	///  @name Static members

	static constexpr size_t Size = std::tuple_size <typename InternalType::internal_type> ();

	static const JSClassOps     class_ops;
	static const JSClass        static_class;
	static const JSPropertySpec properties [ ];
	static const JSFunctionSpec functions [ ];

};

template <class InternalType>
const JSClassOps SFVec2 <InternalType>::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // enumerate
	nullptr, // newEnumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

template <class InternalType>
const JSPropertySpec SFVec2 <InternalType>::properties [ ] = {
	JS_PSGS ("x", getProperty <X>, setProperty <X>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("y", getProperty <Y>, setProperty <Y>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("0", getProperty <X>, setProperty <X>, JSPROP_PERMANENT),
	JS_PSGS ("1", getProperty <Y>, setProperty <Y>, JSPROP_PERMANENT),
	JS_PS_END
};

template <class InternalType>
const JSFunctionSpec SFVec2 <InternalType>::functions [ ] = {
	JS_FN ("add",       add,       1, JSPROP_PERMANENT),
	JS_FN ("distance",  distance,  1, JSPROP_PERMANENT),
	JS_FN ("divide",    divide,    1, JSPROP_PERMANENT),
	JS_FN ("divVec",    divVec,    1, JSPROP_PERMANENT),
	JS_FN ("dot",       dot,       1, JSPROP_PERMANENT),
	JS_FN ("length",    length,    0, JSPROP_PERMANENT),
	JS_FN ("lerp",      lerp,      2, JSPROP_PERMANENT),
	JS_FN ("multiply",  multiply,  1, JSPROP_PERMANENT),
	JS_FN ("multVec",   multVec,   1, JSPROP_PERMANENT),
	JS_FN ("negate",    negate,    0, JSPROP_PERMANENT),
	JS_FN ("normalize", normalize, 0, JSPROP_PERMANENT),
	JS_FN ("subtract",  subtract,  1, JSPROP_PERMANENT),
	JS_FS_END
};

template <class InternalType>
JSObject*
SFVec2 <InternalType>::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

template <class InternalType>
JS::Value
SFVec2 <InternalType>::create (JSContext* const cx, InternalType* const field)
{
	return X3DField::create <SFVec2> (cx, field) .first;
}

template <class InternalType>
bool
SFVec2 <InternalType>::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new InternalType ()));
				return true;
			}
			case Size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto x    = getArgument <typename InternalType::value_type> (cx, args, X);
				const auto y    = getArgument <typename InternalType::value_type> (cx, args, Y);

				args .rval () .set (create (cx, new InternalType (x, y)));
				return true;
			}
			default:
				return ThrowException <JSProto_Error> (cx, "new %s: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "new %s: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
template <size_t Index>
bool
SFVec2 <InternalType>::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto self  = getThis <SFVec2> (cx, args);
		const auto value = getArgument <typename InternalType::value_type> (cx, args, 0);

		self -> set1Value (Index, value);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

template <class InternalType>
template <size_t Index>
bool
SFVec2 <InternalType>::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFVec2> (cx, args);

		args .rval () .setDouble (self -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::add (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .add: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> add (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .add: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::distance (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .distance: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .setDouble (self -> distance (*vector));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .distance: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::divide (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .divide: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto scalar = getArgument <typename InternalType::value_type> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> divide (scalar))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .divide: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::divVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .divVec: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> divVec (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .divVec: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::dot (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .dot: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .setDouble (self -> dot (*vector));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .dot: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::length (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .length: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFVec2> (cx, args);

		args .rval () .setDouble (self -> length ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .length: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::lerp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .lerp: wrong number of arguments.", getClass () -> name);

		const auto args        = JS::CallArgsFromVp (argc, vp);
		const auto self        = getThis <SFVec2> (cx, args);
		const auto destination = getArgument <SFVec2> (cx, args, 0);
		const auto t           = getArgument <typename InternalType::value_type> (cx, args, 1);

		args .rval () .set (create (cx, new InternalType (self -> lerp (*destination, t))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .lerp: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::multiply (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multiply: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto scalar = getArgument <typename InternalType::value_type> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> multiply (scalar))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multiply: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::multVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multVec: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> multVec (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multVec: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::negate (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .negate: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFVec2> (cx, args);

		args .rval () .set (create (cx, new InternalType (self -> negate ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .negate: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::normalize (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .normalize: wrong number of arguments.", getClass () -> name);

		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto self = getThis <SFVec2> (cx, args);

		args .rval () .set (create (cx, new InternalType (self -> normalize ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .normalize: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
bool
SFVec2 <InternalType>::subtract (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .subtract: wrong number of arguments.", getClass () -> name);

		const auto args   = JS::CallArgsFromVp (argc, vp);
		const auto self   = getThis <SFVec2> (cx, args);
		const auto vector = getArgument <SFVec2> (cx, args, 0);

		args .rval () .set (create (cx, new InternalType (self -> subtract (*vector))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .subtract: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr
ObjectType
SFVec2 <X3D::SFVec2d>::getId ()
{
	return ObjectType::SFVec2d;
}

template <>
constexpr
ObjectType
SFVec2 <X3D::SFVec2f>::getId ()
{
	return ObjectType::SFVec2f;
}

using SFVec2d = SFVec2 <X3D::SFVec2d>;
using SFVec2f = SFVec2 <X3D::SFVec2f>;

#ifdef __APPLE__
template <>
const JSClass SFVec2d::static_class;

template <>
const JSClass SFVec2f::static_class;
#endif

extern template class SFVec2 <X3D::SFVec2d>;
extern template class SFVec2 <X3D::SFVec2f>;

} // spidermonkey
} // X3D
} // titania

#endif
