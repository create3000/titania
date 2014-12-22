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

#include "jsSFColor.h"

#include "../jsContext.h"
#include "../jsError.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsSFColor::static_class = {
	"SFColor",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	JS_PropertyStub,
	JS_StrictPropertyStub,
	(JSEnumerateOp) enumerate <size>,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFColor::properties [ ] = {
	JS_PSGS ("r", getProperty <R>, setProperty <R>, JSPROP_PERMANENT),
	JS_PSGS ("g", getProperty <G>, setProperty <G>, JSPROP_PERMANENT),
	JS_PSGS ("b", getProperty <B>, setProperty <B>, JSPROP_PERMANENT),
	JS_PS_END

};

JSFunctionSpec jsSFColor::functions [ ] = {
	JS_FS ("setHSV", setHSV, 3, JSPROP_PERMANENT),
	JS_FS ("getHSV", getHSV, 0, JSPROP_PERMANENT),
	JS_FS_END

};

JSObject*
jsSFColor::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) R, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) G, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) B, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	
	return proto;
}

JS::Value
jsSFColor::create (JSContext* const cx, X3D::SFColor* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

JSBool
jsSFColor::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::SFColor ()));
				return true;
			}
			case size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto r    = getArgument <double> (cx, args, R);
				const auto g    = getArgument <double> (cx, args, G);
				const auto b    = getArgument <double> (cx, args, B);

				args .rval () .set (create (cx, new X3D::SFColor (r, g, b)));
				return true;
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
jsSFColor::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFColor> (cx, obj);
		const auto rhs = getArgument <double> (cx, vp .get (), 0);

		lhs -> set1Value (JSID_TO_INT (id), rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .set1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFColor::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFColor> (cx, obj);

		vp .setDouble (lhs -> get1Value (JSID_TO_INT (id)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFColor::getHSV (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getHSV: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFColor> (cx, args);
		
		float h, s, v;

		lhs -> getHSV (h, s, v);

		JS::Value array [3] = {
			JS::DoubleValue (h),
			JS::DoubleValue (s),
			JS::DoubleValue (v)
		};

		const auto result = JS_NewArrayObject (cx, 3, array);

		if (result == nullptr)
			return ThrowException (cx, "out of memory");

		args .rval () .setObjectOrNull (result);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getHSV: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFColor::setHSV (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 3)
		return ThrowException (cx, "%s .setHSV: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFColor> (cx, args);
		const auto h    = getArgument <double> (cx, args, 0);
		const auto s    = getArgument <double> (cx, args, 1);
		const auto v    = getArgument <double> (cx, args, 2);

		lhs -> setHSV (h, s, v);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setHSV: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania
