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

#ifndef __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC2_H__
#define __TITANIA_X3D_JAVA_SCRIPT_SPIDER_MONKEY_FIELDS_JS_SFVEC2_H__

#include "../../../Fields/SFVec2.h"
#include "../jsContext.h"
#include "../jsError.h"
#include "../jsX3DField.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

template <class Type>
class jsSFVec2 :
	public jsX3DField
{
public:

	///  @name Member types

	using internal_type = Type;

	///  @name Construction

	static
	JSObject*
	init (JSContext* const, JSObject* const, JSObject* const);

	static
	JSBool
	create (JSContext* const, Type* const, jsval* const);

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

	enum Property {X, Y};

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
	static JSBool length    (JSContext*, uint32_t, jsval*);

	///  @name Static members

	static const size_t   size;
	static JSClass        static_class;
	static JSPropertySpec properties [ ];
	static JSFunctionSpec functions [ ];

};

template <class Type>
const size_t jsSFVec2 <Type>::size = 2;

template <class Type>
JSClass jsSFVec2 <Type>::static_class = {
	"SFVec2", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

template <class Type>
JSPropertySpec jsSFVec2 <Type>::properties [ ] = {
	{ "x", X, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y", Y, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

template <class Type>
JSFunctionSpec jsSFVec2 <Type>::functions [ ] = {
	{ "negate",      negate,      0, 0 },
	{ "add",         add,         1, 0 },
	{ "subtract",    subtract,    1, 0 },
	{ "multiply",    multiply,    1, 0 },
	{ "multVec",     multVec,     1, 0 },
	{ "divide",      divide,      1, 0 },
	{ "divVec",      divVec,      1, 0 },
	{ "normalize",   normalize,   0, 0 },
	{ "dot",         dot,         1, 0 },
	{ "length",      length,      0, 0 },

	{ 0 }

};

template <class Type>
JSObject*
jsSFVec2 <Type>::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) X, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) Y, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	
	return proto;
}

template <class Type>
JSBool
jsSFVec2 <Type>::create (JSContext* const cx, Type* const field, jsval* const vp)
{
	return jsX3DField::create (cx, &static_class, field, vp);
}

template <class Type>
JSBool
jsSFVec2 <Type>::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create (cx, new Type (), &JS_RVAL (cx, vp));
			}
			case 2:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto x    = getArgument <double> (cx, argv, X);
				const auto y    = getArgument <double> (cx, argv, Y);

				return create (cx, new Type (x, y), &JS_RVAL (cx, vp));
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

template <class Type>
JSBool
jsSFVec2 <Type>::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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

template <class Type>
JSBool
jsSFVec2 <Type>::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <jsSFVec2> (cx, obj);
		const auto value = getArgument <double> (cx, vp, 0);

		lhs -> set1Value (JSID_TO_INT (id), value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <jsSFVec2> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> get1Value (JSID_TO_INT (id)), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::negate (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .negate: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFVec2> (cx, vp);

		return create (cx, lhs -> negate (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .negate: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::add (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .add: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <jsSFVec2> (cx, argv, 0);

		return create (cx, lhs -> add (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .add: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::subtract (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .subtract: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <jsSFVec2> (cx, argv, 0);

		return create (cx, lhs -> subtract (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .subtract: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::multiply (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <double> (cx, argv, 0);

		return create (cx, lhs -> multiply (rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::multVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <jsSFVec2> (cx, argv, 0);

		return create (cx, lhs -> multiply (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::divide (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divide: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <double> (cx, argv, 0);

		return create (cx, lhs -> divide (rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divide: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::divVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .divVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <jsSFVec2> (cx, argv, 0);

		return create (cx, lhs -> divide (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .divVec: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::dot (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .dot: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <jsSFVec2> (cx, vp);
		const auto rhs  = getArgument <jsSFVec2> (cx, argv, 0);

		return JS_NewNumberValue (cx, lhs -> dot (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .dot: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::normalize (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .normalize: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFVec2> (cx, vp);

		return create (cx, lhs -> normalize (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .normalize: %s.", getClass () -> name, error .what ());
	}
}

template <class Type>
JSBool
jsSFVec2 <Type>::length (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .length: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <jsSFVec2> (cx, vp);

		return JS_NewNumberValue (cx, lhs -> length (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .length: %s.", getClass () -> name, error .what ());
	}
}

template <>
constexpr ObjectType
jsSFVec2 <X3D::SFVec2d>::getId ()
{ return ObjectType::SFVec2d; }

template <>
constexpr ObjectType
jsSFVec2 <X3D::SFVec2f>::getId ()
{ return ObjectType::SFVec2f; }

using jsSFVec2d = jsSFVec2 <X3D::SFVec2d>;
using jsSFVec2f = jsSFVec2 <X3D::SFVec2f>;

extern template class jsSFVec2 <X3D::SFVec2d>;
extern template class jsSFVec2 <X3D::SFVec2f>;

} // MozillaSpiderMonkey
} // X3D
} // titania

#endif
