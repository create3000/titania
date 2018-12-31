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

#include "SFImage.h"

//#include "ArrayFields.h"

namespace titania {
namespace X3D {
namespace spidermonkey {

const JSClassOps SFImage::class_ops = {
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

const JSClass SFImage::static_class = {
	"SFImage",
	JSCLASS_HAS_PRIVATE | JSCLASS_HAS_RESERVED_SLOTS (size_t (SlotType::SIZE)) | JSCLASS_FOREGROUND_FINALIZE,
	&class_ops
};

const JSPropertySpec SFImage::properties [ ] = {
	JS_PSGS ("width",  getWidth,  setWidth,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("x",      getWidth,  setWidth,  JSPROP_PERMANENT),
	JS_PSGS ("height", getHeight, setHeight, JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("y",      getHeight, setHeight, JSPROP_PERMANENT),
	JS_PSGS ("comp",   getComp,   setComp,   JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PSGS ("array",  getArray,  setArray,  JSPROP_PERMANENT | JSPROP_ENUMERATE),
	JS_PS_END
};

const JSFunctionSpec SFImage::functions [ ] = {
	JS_FS_END
};

JSObject*
SFImage::init (JSContext* const cx, JS::HandleObject global, JS::HandleObject parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, functions, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");

	return proto;
}

JS::Value
SFImage::create (JSContext* const cx, X3D::SFImage* const field)
{
	return X3DField::create (cx, &static_class, getId (), field);
}

bool
SFImage::construct (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		switch (argc)
		{
			case 0:
			{
				JS::CallArgsFromVp (argc, vp) .rval () .set (create (cx, new X3D::SFImage ()));
				return true;
			}
			case 4:
			{
				const auto args   = JS::CallArgsFromVp (argc, vp);
				const auto width  = getArgument <uint32_t> (cx, args, WIDTH);
				const auto height = getArgument <uint32_t> (cx, args, HEIGHT);
				const auto comp   = getArgument <uint32_t> (cx, args, COMP);
//				const auto array  = getArgument <MFInt32>  (cx, args, ARRAY);
//
//				args .rval () .set (create (cx, new X3D::SFImage (width, height, comp, *array)));
				args .rval () .setUndefined ();
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

bool
SFImage::setWidth (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);
		const auto rhs  = getArgument <uint32_t> (cx, args, 0);

		lhs -> setWidth (rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::getWidth (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);

		args .rval () .setNumber (uint32_t (lhs -> getWidth ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::setHeight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);
		const auto rhs  = getArgument <uint32_t> (cx, args, 0);

		lhs -> setHeight (rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::getHeight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);

		args .rval () .setNumber (uint32_t (lhs -> getHeight ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::setComp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);
		const auto rhs  = getArgument <uint32_t> (cx, args, 0);

		lhs -> setComponents (rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::getComp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);

		args .rval () .setNumber (uint32_t (lhs -> getComponents ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::setArray (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);
//		const auto rhs  = getArgument <MFInt32> (cx, args, 0);
//
//		lhs -> setArray (*rhs);
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

bool
SFImage::getArray (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <SFImage> (cx, args);

//		args .rval () .set (MFInt32::create (cx, &lhs -> getArray ()));
		args .rval () .setUndefined ();
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException <JSProto_Error> (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

} // spidermonkey
} // X3D
} // titania
