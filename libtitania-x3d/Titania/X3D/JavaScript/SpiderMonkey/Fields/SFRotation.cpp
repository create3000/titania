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

#include "SFRotation.h"

#include "SFVec3.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps SFRotation::class_ops = {
	nullptr, // addProperty
	nullptr, // delProperty
	nullptr, // getProperty
	nullptr, // setProperty
	nullptr, // enumerate
	nullptr, // resolve
	nullptr, // mayResolve
	finalize <SFRotation>, // finalize
	nullptr, // call
	nullptr, // hasInstance
	nullptr, // construct
	nullptr, // trace
};

const JSClass SFRotation::static_class = {
	"SFRotation",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec SFRotation::properties [ ] = {
	JS_PSGS ("x",     getProperty <X>,     setProperty <X>,     JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("y",     getProperty <Y>,     setProperty <Y>,     JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("z",     getProperty <Z>,     setProperty <Z>,     JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("angle", getProperty <ANGLE>, setProperty <ANGLE>, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("0",     getProperty <X>,     setProperty <X>,     JSPROP_PERMANENT),
	JS_PSGS ("1",     getProperty <Y>,     setProperty <Y>,     JSPROP_PERMANENT),
	JS_PSGS ("2",     getProperty <Z>,     setProperty <Z>,     JSPROP_PERMANENT),
	JS_PSGS ("3",     getProperty <ANGLE>, setProperty <ANGLE>, JSPROP_PERMANENT),
	JS_PS_END
};

const JSFunctionSpec SFRotation::functions [ ] = {
	JS_FS ("getAxis",  getAxis,  0, JSPROP_PERMANENT),
	JS_FS ("setAxis",  setAxis,  1, JSPROP_PERMANENT),
	JS_FS ("inverse",  inverse,  0, JSPROP_PERMANENT),
	JS_FS ("multiply", multiply, 1, JSPROP_PERMANENT),
	JS_FS ("multVec",  multVec,  1, JSPROP_PERMANENT),
	JS_FS ("slerp",    slerp,    2, JSPROP_PERMANENT),
	JS_FS_END
};

JSObject*
SFRotation::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

JS::Value
SFRotation::create (JSContext* const cx, X3D::SFRotation* const field)
{
	return X3DField::create <SFRotation> (cx, field);
}

bool
SFRotation::construct (JSContext* cx, unsigned argc, JS::Value* vp)
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
				const auto arg1 = getArgument <SFVec3f> (cx, args, 0);

				try
				{
					const auto arg2 = getArgument <SFVec3f> (cx, args, 1);

					args .rval () .set (create(cx, new X3D::SFRotation (X3D::SFVec3d (arg1 -> getValue ()), X3D::SFVec3d (arg2 -> getValue ()))));
				}
				catch (const std::exception & error)
				{
					const auto arg2 = getArgument <X3D::SFRotation::value_type> (cx, args, 1);

					args .rval () .set (create (cx, new X3D::SFRotation (X3D::SFVec3d (arg1 -> getValue ()), arg2)));				
				}

				return true;
			}
			case Size:
			{
				const auto args  = JS::CallArgsFromVp (argc, vp);
				const auto x     = getArgument <X3D::SFRotation::value_type> (cx, args, X);
				const auto y     = getArgument <X3D::SFRotation::value_type> (cx, args, Y);
				const auto z     = getArgument <X3D::SFRotation::value_type> (cx, args, Z);
				const auto angle = getArgument <X3D::SFRotation::value_type> (cx, args, ANGLE);

				args .rval () .set (create (cx, new X3D::SFRotation (x, y, z, angle)));
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
SFRotation::setProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		const auto rhs  = getArgument <X3D::SFRotation::value_type> (cx, args, 0);

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
SFRotation::getProperty (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);

		args .rval () .setDouble (lhs -> get1Value (Index));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s [%d]: %s.", getClass () -> name, Index, error .what ());
	}
}

bool
SFRotation::getAxis (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .getAxis: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		const auto axis = lhs -> getAxis ();

		args .rval () .set (SFVec3f::create (cx, new X3D::SFVec3f (axis)));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .getAxis: %s.", getClass () -> name, error .what ());
	}
}

bool
SFRotation::setAxis (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .setAxis: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		const auto axis = getArgument <SFVec3f> (cx, args, 0);

		lhs -> setAxis (axis -> getValue ());

		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .setAxis: %s.", getClass () -> name, error .what ());
	}
}

bool
SFRotation::inverse (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 0)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .inverse: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);

		args .rval () .set (create (cx, new X3D::SFRotation (lhs -> inverse ())));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .inverse: %s.", getClass () -> name, error .what ());
	}
}

bool
SFRotation::multiply (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multiply: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		const auto rhs  = getArgument <SFRotation> (cx, args, 0);

		args .rval () .set (create (cx, new X3D::SFRotation (lhs -> multiply (*rhs))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multiply: %s.", getClass () -> name, error .what ());
	}
}

bool
SFRotation::multVec (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 1)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .multVec: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		
		try
		{
			const auto rhs = getArgument <SFVec3f> (cx, args, 0);

			args .rval () .set (SFVec3f::create (cx, new X3D::SFVec3f (lhs -> multVec (rhs -> getValue ()))));
		}
		catch (const std::exception & error)
		{
			const auto rhs = getArgument <SFVec3d> (cx, args, 0);

			args .rval () .set (SFVec3d::create (cx, new X3D::SFVec3d (lhs -> multVec (rhs -> getValue ()))));
		}

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .multVec: %s.", getClass () -> name, error .what ());
	}
}

bool
SFRotation::slerp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		if (argc not_eq 2)
			return ThrowException <JSProto_Error> (cx, "%s .prototype .slerp: wrong number of arguments.", getClass () -> name);
	
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFRotation> (cx, args);
		const auto rhs  = getArgument <SFRotation> (cx, args, 0);
		const auto t    = getArgument <X3D::SFRotation::value_type> (cx, args, 1);

		args .rval () .set (create (cx, new X3D::SFRotation (lhs -> slerp (*rhs, t))));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .prototype .slerp: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
