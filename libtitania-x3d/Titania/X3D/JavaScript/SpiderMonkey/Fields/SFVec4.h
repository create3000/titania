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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC4_H__

#include "../../../Fields/SFVec4.h"
#include "../Context.h"
#include "../Error.h"
#include "../X3DField.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

template <class InternalType>
class SFVec4 :
	public X3DField
{
public:

	///  @name Member types

	using internal_type = InternalType;
	using value_type    = typename InternalType::value_type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSClass*
	getClass ()
	{ return &static_class; }

	static
	constexpr ObjectType
	getId ()
	{ throw std::domain_error ("getId"); }


private:

	///  @name Member types

	enum Property {X, Y, Z, W};

	///  @name Construction

	static JSBool construct (JSContext*, uint32_t, jsval*);

	///  @name Member access

	static JSBool enumerate (JSContext*, JSObject*, JSIterateOp, jsval*, jsid*);
	static JSBool set1Value (JSContext*, JSObject*, jsid, JSBool, jsval*);
	static JSBool get1Value (JSContext*, JSObject*, jsid, jsval*);

	///  @name Functions

	static JSBool negate    (JSContext*, uint32_t, jsval*);
	static JSBool add       (JSContext*, uint32_t, jsval*);
	static JSBool subtract  (JSContext*, uint32_t, jsval*);
	static JSBool multiply  (JSContext*, uint32_t, jsval*);
	static JSBool multVec   (JSContext*, uint32_t, jsval*);
	static JSBool divide    (JSContext*, uint32_t, jsval*);
	static JSBool divVec    (JSContext*, uint32_t, jsval*);
	static JSBool dot       (JSContext*, uint32_t, jsval*);
	static JSBool normalize (JSContext*, uint32_t, jsval*);
	static JSBool lerp      (JSContext*, uint32_t, jsval*);
	static JSBool length    (JSContext*, uint32_t, jsval*);

	///  @name Static members

	static const size_t   size;
	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class InternalType>
const size_t SFVec4 <InternalType>::size = 4;

template <class InternalType>
JSPropertySpec SFVec4 <InternalType>::properties [ ] = {
	{ "x", X, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y", Y, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z", Z, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "w", W, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

template <class InternalType>
JSFunctionSpec SFVec4 <InternalType>::functions [ ] = {
	{ "negate",    negate,    0, 0 },
	{ "add",       add,       1, 0 },
	{ "subtract",  subtract,  1, 0 },
	{ "multiply",  multiply,  1, 0 },
	{ "multVec",   multVec,   1, 0 },
	{ "divide",    divide,    1, 0 },
	{ "divVec",    divVec,    1, 0 },
	{ "normalize", normalize, 0, 0 },
	{ "dot",       dot,       1, 0 },
	{ "lerp",      lerp,      2, 0 },
	{ "length",    length,    0, 0 },

	{ 0 }

};

template <class InternalType>
JSObject*
SFVec4 <InternalType>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) X, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) Y, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) Z, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) W, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	
	return proto;
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create <SFVec4> (cx, new InternalType (), &JS_RVAL (cx, vp));
			}
			case 4:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto x    = getArgument <value_type> (cx, argv, X);
				const auto y    = getArgument <value_type> (cx, argv, Y);
				const auto z    = getArgument <value_type> (cx, argv, Z);
				const auto w    = getArgument <value_type> (cx, argv, W);

				return create <SFVec4> (cx, new InternalType (x, y, z, w), &JS_RVAL (cx, vp));
			}
			default:
				return ThrowException (cx, "%s .new: wrong number of arguments.", getClass () -> name);
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .new: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
{
	if (not JS_GetPrivate (cx, obj))
	{
		*statep = JSVAL_NULL;
		return true;
	}

	size_t* index;

	switch (enum_op)
	{
		case JSENUMERATE_INIT:
		case JSENUMERATE_INIT_ALL:
		{
			index   = new size_t (0);
			*statep = PRIVATE_TO_JSVAL (index);

			if (idp)
				*idp = INT_TO_JSID (size);

			break;
		}
		case JSENUMERATE_NEXT:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);

			if (*index < size)
			{
				if (idp)
					*idp = INT_TO_JSID (*index);

				*index = *index + 1;
				break;
			}

			//else done -- cleanup.
		}
		case JSENUMERATE_DESTROY:
		{
			index = (size_t*) JSVAL_TO_PRIVATE (*statep);
			delete index;
			*statep = JSVAL_NULL;
		}
	}

	return true;
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFVec4> (cx, obj);
		const auto value = getArgument <value_type> (cx, vp, 0);

		lhs -> set1Value (JSID_TO_INT (id), value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFVec4> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> get1Value (JSID_TO_INT (id)), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::negate (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .negate: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFVec4> (cx, vp);

		return create <SFVec4> (cx, new InternalType (lhs -> negate ()), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .negate: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::add (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .add: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <SFVec4> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> add (*rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .add: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::subtract (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .subtract: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <SFVec4> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> subtract (*rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .subtract: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::multiply (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <value_type> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> multiply (rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::multVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <SFVec4> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> multiply (*rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::divide (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divide: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <value_type> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> divide (rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divide: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::divVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <SFVec4> (cx, argv, 0);

		return create <SFVec4> (cx, new InternalType (lhs -> divide (*rhs)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divVec: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::dot (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .dot: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFVec4> (cx, vp);
		const auto rhs  = getArgument <SFVec4> (cx, argv, 0);

		return JS_NewNumberValue (cx, lhs -> dot (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .dot: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::normalize (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .normalize: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFVec4> (cx, vp);

		return create <SFVec4> (cx, new InternalType (lhs -> normalize ()), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .normalize: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::lerp (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .lerp: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv        = JS_ARGV (cx, vp);
		const auto source      = getThis <SFVec4> (cx, vp);
		const auto destination = getArgument <SFVec4> (cx, argv, 0);
		const auto t           = getArgument <value_type> (cx, argv, 1);

		return create <SFVec4> (cx, new InternalType (source -> lerp (*destination, t)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .lerp: %s.", getClass () -> name, error .what ());
	}
}

template <class InternalType>
JSBool
SFVec4 <InternalType>::length (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .length: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFVec4> (cx, vp);

		return JS_NewNumberValue (cx, lhs -> length (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

using SFVec4d = SFVec4 <X3D::SFVec4d>;
using SFVec4f = SFVec4 <X3D::SFVec4f>;

template <>
constexpr ObjectType
SFVec4d::getId ()
{ return ObjectType::SFVec4d; }

template <>
constexpr ObjectType
SFVec4f::getId ()
{ return ObjectType::SFVec4f; }

} // spidermonkey
} // X3D
} // titania

#endif
