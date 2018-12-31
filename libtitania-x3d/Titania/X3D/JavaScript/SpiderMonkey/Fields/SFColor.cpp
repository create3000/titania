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

#include "SFColor.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps SFColor::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	&finalize, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass SFColor::static_class = {
	"SFColor",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec SFColor::properties [ ] = {
	JS_PSGS ("r", getProperty <R>, setProperty <R>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("g", getProperty <G>, setProperty <G>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("b", getProperty <B>, setProperty <B>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("0", getProperty <R>, setProperty <R>, JSPROP_PERMANENT),
	JS_PSGS ("1", getProperty <G>, setProperty <G>, JSPROP_PERMANENT),
	JS_PSGS ("2", getProperty <B>, setProperty <B>, JSPROP_PERMANENT),
	JS_PS_END
};

const JSFunctionSpec SFColor::functions [ ] = {
	JS_FS ("getHSV", getHSV, 0, JSPROP_PERMANENT),
	JS_FS ("setHSV", setHSV, 3, JSPROP_PERMANENT),
	JS_FS ("lerp",   lerp,   2, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
SFColor::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

JS::Value
SFColor::create (JSContext* const cx, Type* const field)
{
	return X3DField::create (cx, &static_class, getId (), field);
}

bool
SFColor::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new Type ()));
				return true;
			}
			case Size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto r    = getArgument <Type::value_type> (cx, args, R);
				const auto g    = getArgument <Type::value_type> (cx, args, G);
				const auto b    = getArgument <Type::value_type> (cx, args, B);

				args .rval () .set (create (cx, new Type (r, g, b)));
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

template <size_t Index>
bool
SFColor::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFColor> (cx, args);
		const auto rhs  = getArgument <Type::value_type> (cx, args, 0);

		lhs -> set1Value (Index, rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

template <size_t Index>
bool
SFColor::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFColor> (cx, args);

		args .rval () .setDouble (lhs -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

bool
SFColor::getHSV (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getHSV: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFColor> (cx, args);
		const auto hsv  = lhs -> getHSV ();

		JS::AutoValueVector array (cx);

		array .append (JS::DoubleValue (hsv [0]));
		array .append (JS::DoubleValue (hsv [1]));
		array .append (JS::DoubleValue (hsv [2]));

		const auto result = JS_NewArrayObject (cx, array);

		if (not result)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getHSV: out of memory", getClass () -> name);

		args .rval () .setObjectOrNull (result);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getHSV: %s.", getClass () -> name, error .what ());
	}
}

bool
SFColor::setHSV (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 3)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setHSV: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFColor> (cx, args);
		const auto h    = getArgument <Type::value_type> (cx, args, 0);
		const auto s    = getArgument <Type::value_type> (cx, args, 1);
		const auto v    = getArgument <Type::value_type> (cx, args, 2);

		lhs -> setHSV (vector3 <Type::value_type> (h, s, v));

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .setHSV: %s.", getClass () -> name, error .what ());
	}
}

bool
SFColor::lerp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .lerp: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFColor> (cx, args);
		const auto rhs  = getArgument <SFColor> (cx, args, 0);
		const auto t    = getArgument <Type::value_type> (cx, args, 1);

		args .rval () .set (create (cx, new Type (lhs -> lerp (*rhs, t))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .lerp: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
