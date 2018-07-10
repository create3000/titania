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

#include "SFColorRGBA.h"

#include "../Context.h"
#include "../Error.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const size_t SFColorRGBA::size = 4;

JSClass SFColorRGBA::static_class = {
	"SFColorRGBA", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <SFColorRGBA>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec SFColorRGBA::properties [ ] = {
	{ "r", R, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "g", G, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "b", B, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "a", A, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec SFColorRGBA::functions [ ] = {
	{ "getHSVA",     getHSVA, 0, 0 },
	{ "setHSVA",     setHSVA, 4, 0 },
	{ "lerp",        lerp,    2, 0 },

	{ 0 }

};

JSObject*
SFColorRGBA::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) R, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) G, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) B, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) A, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	
	return proto;
}

JSBool
SFColorRGBA::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create <SFColorRGBA> (cx, new X3D::SFColorRGBA (), &JS_RVAL (cx, vp));
			}
			case 4:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto r    = getArgument <value_type> (cx, argv, R);
				const auto g    = getArgument <value_type> (cx, argv, G);
				const auto b    = getArgument <value_type> (cx, argv, B);
				const auto a    = getArgument <value_type> (cx, argv, A);

				return create <SFColorRGBA> (cx, new X3D::SFColorRGBA (r, g, b, a), &JS_RVAL (cx, vp));
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

JSBool
SFColorRGBA::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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

JSBool
SFColorRGBA::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFColorRGBA> (cx, obj);
		const auto value = getArgument <value_type> (cx, vp, 0);

		lhs -> set1Value (JSID_TO_INT (id), value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFColorRGBA::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFColorRGBA> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> get1Value (JSID_TO_INT (id)), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFColorRGBA::getHSVA (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getHSVA: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs  = getThis <SFColorRGBA> (cx, vp);
		const auto hsva = lhs -> getHSVA ();

		jsval array [4];

		if (not JS_NewNumberValue (cx, hsva [0], &array [0]))
			return false;

		if (not JS_NewNumberValue (cx, hsva [1], &array [1]))
			return false;

		if (not JS_NewNumberValue (cx, hsva [2], &array [2]))
			return false;

		if (not JS_NewNumberValue (cx, hsva [3], &array [3]))
			return false;

		const auto result = JS_NewArrayObject (cx, 4, array);

		if (result == nullptr)
			return ThrowException (cx, "out of memory");

		JS_SET_RVAL (cx, vp, OBJECT_TO_JSVAL (result));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getHSVA: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFColorRGBA::setHSVA (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 4)
		return ThrowException (cx, "%s .setHSVA: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFColorRGBA> (cx, vp);
		const auto h    = getArgument <value_type> (cx, argv, 0);
		const auto s    = getArgument <value_type> (cx, argv, 1);
		const auto v    = getArgument <value_type> (cx, argv, 2);
		const auto a    = getArgument <value_type> (cx, argv, 3);

		lhs -> setHSVA (vector4 <internal_type::value_type> (h, s, v, a));

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setHSVA: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFColorRGBA::lerp (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .lerp: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFColorRGBA> (cx, vp);
		const auto rhs  = getArgument <SFColorRGBA> (cx, argv, 0);
		const auto t    = getArgument <value_type> (cx, argv, 1);

		return create <SFColorRGBA> (cx, new X3D::SFColorRGBA (lhs -> lerp (*rhs, t)), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .lerp: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
