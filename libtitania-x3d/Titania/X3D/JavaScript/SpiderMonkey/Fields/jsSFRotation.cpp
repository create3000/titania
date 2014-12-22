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

#include "jsSFRotation.h"

#include "../jsContext.h"
#include "../jsError.h"
#include "jsSFVec3.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsSFRotation::static_class = {
	"SFRotation",
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

JSPropertySpec jsSFRotation::properties [ ] = {
	JS_PSGS ("x",     getProperty <X>,     setProperty <X>,     JSPROP_PERMANENT),
	JS_PSGS ("y",     getProperty <Y>,     setProperty <Y>,     JSPROP_PERMANENT),
	JS_PSGS ("z",     getProperty <Z>,     setProperty <Z>,     JSPROP_PERMANENT),
	JS_PSGS ("angle", getProperty <ANGLE>, setProperty <ANGLE>, JSPROP_PERMANENT),
	JS_PS_END

};

JSFunctionSpec jsSFRotation::functions [ ] = {
	JS_FS ("setAxis", setAxis,     0, JSPROP_PERMANENT),
	JS_FS ("getAxis", getAxis,     0, JSPROP_PERMANENT),

	JS_FS ("inverse", inverse,     0, JSPROP_PERMANENT),
	JS_FS ("multiply",multiply,    1, JSPROP_PERMANENT),
	JS_FS ("multVec", multVec,     1, JSPROP_PERMANENT),
	JS_FS ("slerp",   slerp,       2, JSPROP_PERMANENT),
	JS_FS_END

};

JSObject*
jsSFRotation::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	JS_DefineProperty (cx, proto, (char*) X,     JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) Y,     JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) Z,     JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	JS_DefineProperty (cx, proto, (char*) ANGLE, JS::UndefinedValue (), get1Value, set1Value, JSPROP_INDEX | JSPROP_PERMANENT | JSPROP_SHARED);
	
	return proto;
}

JS::Value
jsSFRotation::create (JSContext* const cx, X3D::SFRotation* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

JSBool
jsSFRotation::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::SFRotation ()));
				return true;
			}
			case 2:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto arg1 = getArgument <jsSFVec3f> (cx, args, 0);

				try
				{
					const auto arg2 = getArgument <jsSFVec3f> (cx, args, 1);

					args .rval () .set (create (cx, new SFRotation (*arg1, *arg2)));
					return true;
				}
				catch (const std::exception &)
				{
					const auto arg2 = getArgument <double> (cx, args, 1);

					args .rval () .set (create (cx, new SFRotation (*arg1, arg2)));
					return true;
				}
			}
			case size:
			{
				const auto args = JS::CallArgsFromVp (argc, vp);
				const auto x     = getArgument <double> (cx, args, X);
				const auto y     = getArgument <double> (cx, args, Y);
				const auto z     = getArgument <double> (cx, args, Z);
				const auto angle = getArgument <double> (cx, args, ANGLE);

				args .rval () .set (create (cx, new X3D::SFRotation (x, y, z, angle)));
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
jsSFRotation::set1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JSBool strict, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFRotation> (cx, obj);
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
jsSFRotation::get1Value (JSContext* cx, JS::HandleObject obj, JS::HandleId id, JS::MutableHandleValue vp)
{
	try
	{
		const auto lhs = getThis <jsSFRotation> (cx, obj);

		vp .setDouble (lhs -> get1Value (JSID_TO_INT (id)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .get1Value: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::setAxis (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .setAxis: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);
		const auto rhs  = getArgument <jsSFVec3f> (cx, args, 0);

		lhs -> setAxis (*rhs);

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .setAxis: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::getAxis (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .getAxis: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);

		args .rval () .set (jsSFVec3f::create (cx, lhs -> getAxis ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .getAxis: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::inverse (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 0)
		return ThrowException (cx, "%s .inverse: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);

		args .rval () .set (create (cx, lhs -> inverse ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .inverse: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::multiply (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multiply: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);
		const auto rhs  = getArgument <jsSFRotation> (cx, args, 0);

		args .rval () .set (create (cx, lhs -> multiply (*rhs)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multiply: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::multVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 1)
		return ThrowException (cx, "%s .multVec: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);
		
		try
		{
			const auto rhs = getArgument <jsSFVec3f> (cx, args, 0);

			args .rval () .set (jsSFVec3f::create (cx, lhs -> multVec (*rhs)));
			return true;
		}
		catch (const std::exception &)
		{
			const auto rhs = getArgument <jsSFVec3d> (cx, args, 0);

			args .rval () .set (jsSFVec3d::create (cx, SFRotation4d (lhs -> getValue ()) .multVec (*rhs)));
			return true;
		}
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .multVec: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFRotation::slerp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	if (argc not_eq 2)
		return ThrowException (cx, "%s .slerp: wrong number of arguments.", getClass () -> name);

	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFRotation> (cx, args);
		const auto rhs  = getArgument <jsSFRotation> (cx, args, 0);
		const auto t    = getArgument <double> (cx, args, 1);

		args .rval () .set (create (cx, lhs -> slerp (*rhs, t)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .slerp: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania
