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

#include "SFRotation.h"

#include "../Context.h"
#include "../Error.h"
#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const size_t SFRotation::size = 4;

JSClass SFRotation::static_class = {
	"SFRotation", JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate, JS_ResolveStub, JS_ConvertStub, finalize <SFRotation>,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec SFRotation::properties [ ] = {
	{ "x",     X,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "y",     Y,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "z",     Z,     JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ "angle", ANGLE, JSPROP_SHARED | JSPROP_PERMANENT, get1Value, set1Value },
	{ 0 }

};

JSFunctionSpec SFRotation::functions [ ] = {
	{ "getAxis",     getAxis,     0, 0 },
	{ "setAxis",     setAxis,     0, 0 },

	{ "inverse",     inverse,     0, 0 },
	{ "multiply",    multiply,    1, 0 },
	{ "multVec",     multVec,     1, 0 },
	{ "slerp",       slerp,       2, 0 },

	{ 0 }

};

JSObject*
SFRotation::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) X,     JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) Y,     JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) Z,     JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	JS_DefineProperty (cx, proto, (char*) ANGLE, JSVAL_VOID, get1Value, set1Value, JSPROP_INDEX | JSPROP_SHARED | JSPROP_PERMANENT | JSPROP_ENUMERATE);
	
	return proto;
}

JSBool
SFRotation::construct (JSContext* cx, uint32_t argc, jsval* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				return create <SFRotation> (cx, new X3D::SFRotation (), &JS_RVAL (cx, vp));
			}
			case 2:
			{
				const auto argv = JS_ARGV (cx, vp);
				const auto arg1 = getArgument <SFVec3f> (cx, argv, 0);

				try
				{
					const auto arg2 = getArgument <SFVec3f> (cx, argv, 1);

					return create <SFRotation> (cx, new X3D::SFRotation (*arg1, *arg2), &JS_RVAL (cx, vp));
				}
				catch (const std::exception &)
				{
					const auto arg2 = getArgument <value_type> (cx, argv, 1);

					return create <SFRotation> (cx, new X3D::SFRotation (*arg1, arg2), &JS_RVAL (cx, vp));				
				}
			}
			case 4:
			{
				const auto argv  = JS_ARGV (cx, vp);
				const auto x     = getArgument <value_type> (cx, argv, X);
				const auto y     = getArgument <value_type> (cx, argv, Y);
				const auto z     = getArgument <value_type> (cx, argv, Z);
				const auto angle = getArgument <value_type> (cx, argv, ANGLE);

				return create <SFRotation> (cx, new X3D::SFRotation (x, y, z, angle), &JS_RVAL (cx, vp));
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
SFRotation::enumerate (JSContext* cx, JSObject* obj, JSIterateOp enum_op, jsval* statep, jsid* idp)
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
SFRotation::set1Value (JSContext* cx, JSObject* obj, jsid id, JSBool strict, jsval* vp)
{
	try
	{
		const auto lhs   = getThis <SFRotation> (cx, obj);
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
SFRotation::get1Value (JSContext* cx, JSObject* obj, jsid id, jsval* vp)
{
	try
	{
		const auto lhs = getThis <SFRotation> (cx, obj);

		return JS_NewNumberValue (cx, lhs -> get1Value (JSID_TO_INT (id)), vp);
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::setAxis (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .setAxis: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFRotation> (cx, vp);
		const auto rhs  = getArgument <SFVec3f> (cx, argv, 0);

		lhs -> setAxis (*rhs);

		JS_SET_RVAL (cx, vp, JSVAL_VOID);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setAxis: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::getAxis (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getAxis: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFRotation> (cx, vp);

		return create <SFVec3f> (cx, lhs -> getAxis (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getAxis: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::inverse (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .inverse: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto lhs = getThis <SFRotation> (cx, vp);

		return create <SFRotation> (cx, lhs -> inverse (), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .inverse: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::multiply (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFRotation> (cx, vp);
		const auto rhs  = getArgument <SFRotation> (cx, argv, 0);

		return create <SFRotation> (cx, lhs -> multiply (*rhs), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::multVec (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFRotation> (cx, vp);
		
		try
		{
			const auto rhs = getArgument <SFVec3f> (cx, argv, 0);

			return create <SFVec3f> (cx, lhs -> multVec (*rhs), &JS_RVAL (cx, vp));
		}
		catch (const std::exception &)
		{
			const auto rhs = getArgument <SFVec3d> (cx, argv, 0);

			return create <SFVec3d> (cx, SFRotation4d (lhs -> getValue ()) .multVec (*rhs), &JS_RVAL (cx, vp));
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

JSBool
SFRotation::slerp (JSContext* cx, uint32_t argc, jsval* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .slerp: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto argv = JS_ARGV (cx, vp);
		const auto lhs  = getThis <SFRotation> (cx, vp);
		const auto rhs  = getArgument <SFRotation> (cx, argv, 0);
		const auto t    = getArgument <value_type> (cx, argv, 1);

		return create <SFRotation> (cx, lhs -> slerp (*rhs, t), &JS_RVAL (cx, vp));
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .slerp: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
