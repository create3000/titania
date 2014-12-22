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

#include "jsSFImage.h"

#include "../jsContext.h"
#include "../jsError.h"
#include "jsArrayFields.h"

namespace titania {
namespace X3D {
namespace MozillaSpiderMonkey {

JSClass jsSFImage::static_class = {
	"SFImage",
	JSCLASS_HAS_PRIVATE | JSCLASS_NEW_ENUMERATE,
	JS_PropertyStub,
	JS_DeletePropertyStub,
	JS_PropertyStub,
	JS_StrictPropertyStub,
	JS_EnumerateStub,
	JS_ResolveStub,
	JS_ConvertStub,
	finalize,
	JSCLASS_NO_OPTIONAL_MEMBERS

};

JSPropertySpec jsSFImage::properties [ ] = {
	JS_PSGS ("width",  getWidth,  setWidth,  JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_ENUMERATE),
	JS_PSGS ("x",      getWidth,  setWidth,  JSPROP_PERMANENT | JSPROP_SHARED), // VRML2
	JS_PSGS ("height", getHeight, setHeight, JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_ENUMERATE),
	JS_PSGS ("y",      getHeight, setHeight, JSPROP_PERMANENT | JSPROP_SHARED), // VRML2
	JS_PSGS ("comp",   getComp,   setComp,   JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_ENUMERATE),
	JS_PSGS ("array",  getArray,  setArray,  JSPROP_PERMANENT | JSPROP_SHARED | JSPROP_ENUMERATE),
	JS_PS_END

};

JSObject*
jsSFImage::init (JSContext* const cx, JSObject* const global, JSObject* const parent)
{
	const auto proto = JS_InitClass (cx, global, parent, &static_class, construct, 0, properties, nullptr, nullptr, nullptr);

	if (not proto)
		throw std::runtime_error ("Couldn't initialize JavaScript global object.");
	
	return proto;
}

JS::Value
jsSFImage::create (JSContext* const cx, X3D::SFImage* const field)
throw (std::invalid_argument)
{
	return jsX3DField::create (cx, &static_class, field);
}

JSBool
jsSFImage::construct (JSContext* cx, unsigned argc, JS::Value* vp)
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
				const auto array  = getArgument <jsMFInt32> (cx, args, ARRAY);

				args .rval () .set (create (cx, new X3D::SFImage (width, height, comp, *array)));
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
jsSFImage::setWidth (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto lhs   = getThis <jsSFImage> (cx, args);
		const auto value = getArgument <uint32_t> (cx, args, 0);

		lhs -> setWidth (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::getWidth (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFImage> (cx, args);
		
		args .rval () .setNumber ((uint32_t) lhs -> getWidth ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .width: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::setHeight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto lhs   = getThis <jsSFImage> (cx, args);
		const auto value = getArgument <uint32_t> (cx, args, 0);

		lhs -> setHeight (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::getHeight (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFImage> (cx, args);

		args .rval () .setNumber ((uint32_t) lhs -> getHeight ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .height: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::setComp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto lhs   = getThis <jsSFImage> (cx, args);
		const auto value = getArgument <uint32_t> (cx, args, 0);

		lhs -> setComponents (value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::getComp (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFImage> (cx, args);

		args .rval () .setNumber ((uint32_t) lhs -> getComponents ());
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .comp: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::setArray (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args  = JS::CallArgsFromVp (argc, vp);
		const auto lhs   = getThis <jsSFImage> (cx, args);
		const auto value = getArgument <jsMFInt32> (cx, args, 0);

		lhs -> setArray (*value);

		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

JSBool
jsSFImage::getArray (JSContext* cx, unsigned argc, JS::Value* vp)
{
	try
	{
		const auto args = JS::CallArgsFromVp (argc, vp);
		const auto lhs  = getThis <jsSFImage> (cx, args);

		args .rval () .set (jsMFInt32::create (cx, &lhs -> getArray ()));
		return true;
	}
	catch (const std::exception & error)
	{
		return ThrowException (cx, "%s .array: %s.", getClass () -> name, error .what ());
	}
}

} // MozillaSpiderMonkey
} // X3D
} // titania
